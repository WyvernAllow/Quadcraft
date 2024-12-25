#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "camera.h"
#include "utils.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>

static void glfw_error_callback(int error_code, const char *description) {
    (void)error_code;
    fprintf(stderr, "GLFW: %s\n", description);
}

struct camera camera;

static void glfw_framebuffer_size_callback(GLFWwindow *window, int width,
                                           int height) {
    (void)window;
    camera.aspect = width / (float)height;
    glViewport(0, 0, width, height);
}

float last_x = 800 / 2.0f;
float last_y = 450 / 2.0f;
bool first_mouse = true;
bool unfocused = false;

static void glfw_cursor_pos_callback(GLFWwindow *window, double x, double y) {
    (void)window;

    if (unfocused) {
        return;
    }

    if (first_mouse) {
        last_x = x;
        last_y = y;
        first_mouse = false;
    }

    float delta_x = x - last_x;
    float delta_y = y - last_y;

    last_x = x;
    last_y = y;

    camera.pitch_deg -= delta_y * 0.25f;
    camera.yaw_deg += delta_x * 0.25f;
}

static void glfw_key_callback(GLFWwindow *window, int key, int scancode,
                              int action, int mods) {
    (void)window;
    (void)scancode;
    (void)mods;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        unfocused = true;
        first_mouse = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

static GLuint compile_shader(const char *source, GLenum type) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint did_compile;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &did_compile);
    if (!did_compile) {
        char info_log[512];
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        fprintf(stderr, "Shader compilation failed: %s\n", info_log);
        return 0;
    }

    return shader;
}

static GLuint load_program(const char *vert_filename,
                           const char *frag_filename) {
    const char *vert_source = load_file_str(vert_filename);
    const char *frag_source = load_file_str(frag_filename);
    if (!vert_source || !frag_source) {
        fprintf(stderr, "Failed to load shader source\n");
        return 0;
    }

    GLuint vert_shader = compile_shader(vert_source, GL_VERTEX_SHADER);
    GLuint frag_shader = compile_shader(frag_source, GL_FRAGMENT_SHADER);
    free((void *)vert_source);
    free((void *)frag_source);

    if (!vert_shader || !frag_shader) {
        fprintf(stderr, "Failed to compile shaders\n");
        return 0;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vert_shader);
    glAttachShader(program, frag_shader);
    glLinkProgram(program);

    GLint did_link;
    glGetProgramiv(program, GL_LINK_STATUS, &did_link);
    if (!did_link) {
        char info_log[512];
        glGetProgramInfoLog(program, 512, NULL, info_log);
        fprintf(stderr, "Program linking failed: %s\n", info_log);
        return 0;
    }

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);

    return program;
}

struct vertex {
    float position[3];
};

#define MAX_QUAD_COUNT 1024
#define MAX_VERTEX_COUNT (MAX_QUAD_COUNT * 4)
#define MAX_INDEX_COUNT (MAX_QUAD_COUNT * 6)

int main(void) {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        fprintf(stderr, "glfwInit failed\n");
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window = glfwCreateWindow(800, 450, "Quadcraft", NULL, NULL);
    if (!window) {
        fprintf(stderr, "glfwCreateWindow failed\n");
        return EXIT_FAILURE;
    }

    glfwSetFramebufferSizeCallback(window, glfw_framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, glfw_cursor_pos_callback);
    glfwSetKeyCallback(window, glfw_key_callback);

    glfwMakeContextCurrent(window);

    if (!gladLoadGL(glfwGetProcAddress)) {
        fprintf(stderr, "gladLoadGL failed\n");
        return EXIT_FAILURE;
    }

    glEnable(GL_CULL_FACE);

    GLuint program =
        load_program("res/shaders/chunk.vert", "res/shaders/chunk.frag");
    if (!program) {
        fprintf(stderr, "load_program failed\n");
        return EXIT_FAILURE;
    }

    struct vertex vertices[] = {
        {0.5f, 0.5f, -1.0f},    // top right
        {0.5f, -0.5f, -1.0f},   // bottom right
        {-0.5f, -0.5f, -1.0f},  // bottom left
        {-0.5f, 0.5f, -1.0f},   // top left
    };

    uint16_t indices[MAX_INDEX_COUNT];
    uint16_t offset = 0;
    for (uint16_t i = 0; i < MAX_INDEX_COUNT; i += 6) {
        indices[i + 0] = offset + 3;
        indices[i + 1] = offset + 1;
        indices[i + 2] = offset + 0;
        indices[i + 3] = offset + 3;
        indices[i + 4] = offset + 2;
        indices[i + 5] = offset + 1;
    }

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    /* Position attribute */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex),
                          (const void *)offsetof(struct vertex, position));
    glEnableVertexAttribArray(0);

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    glBindVertexArray(0);

    camera_init(&camera, 90.0f, 800.0f / 450.0f);

    camera.position.z = 3.0f;

    camera_update(&camera);

    float current_time = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        float new_time = glfwGetTime();
        float delta_time = new_time - current_time;
        current_time = new_time;

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) && unfocused) {
            unfocused = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }

        struct vec3 wishdir = {0};
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            wishdir = vec3_add(wishdir, camera.forward);
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            wishdir = vec3_sub(wishdir, camera.forward);
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            wishdir = vec3_sub(wishdir, camera.right);
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            wishdir = vec3_add(wishdir, camera.right);
        }

        camera.position =
            vec3_add(camera.position,
                     vec3_scale(vec3_normalize(wishdir), delta_time * 5.0f));

        camera_update(&camera);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        glUniformMatrix4fv(glGetUniformLocation(program, "u_mvp"), 1, GL_FALSE,
                           camera.view_proj.m);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}
