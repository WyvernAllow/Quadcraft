#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>

static void glfw_error_callback(int error_code, const char *description) {
    (void)error_code;
    fprintf(stderr, "GLFW: %s\n", description);
}

static void glfw_framebuffer_size_callback(GLFWwindow *window, int width,
                                           int height) {
    (void)window;
    glViewport(0, 0, width, height);
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

    glfwMakeContextCurrent(window);

    if (!gladLoadGL(glfwGetProcAddress)) {
        fprintf(stderr, "gladLoadGL failed\n");
        return EXIT_FAILURE;
    }

    GLuint program =
        load_program("res/shaders/chunk.vert", "res/shaders/chunk.frag");
    if (!program) {
        fprintf(stderr, "load_program failed\n");
        return EXIT_FAILURE;
    }

    struct vertex vertices[] = {
        {0.5f, 0.5f, 0.0f},    // top right
        {0.5f, -0.5f, 0.0f},   // bottom right
        {-0.5f, -0.5f, 0.0f},  // bottom left
        {-0.5f, 0.5f, 0.0f},   // top left
    };

    uint16_t indices[] = {
        0, 1, 3,  // first triangle
        1, 2, 3   // second triangle
    };

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

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
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
