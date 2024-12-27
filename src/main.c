#include <math.h>
#include <stb_image.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "camera.h"
#include "chunk.h"
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
    struct vec3 position;
    struct vec3 normal;
    float layer;
};

#define MAX_QUAD_COUNT 5000000
#define MAX_VERTEX_COUNT (MAX_QUAD_COUNT * 4)
#define MAX_INDEX_COUNT (MAX_QUAD_COUNT * 6)

struct vertex *vertices;
size_t vertex_count;

// clang-format off
static const struct vec3 NORMAL_LUT[DIR_COUNT] = {
    [DIR_POS_X] = { 1.0f,  0.0f,  0.0f},
    [DIR_POS_Y] = { 0.0f,  1.0f,  0.0f},
    [DIR_POS_Z] = { 0.0f,  0.0f,  1.0f},
    [DIR_NEG_X] = {-1.0f,  0.0f,  0.0f},
    [DIR_NEG_Y] = { 0.0f, -1.0f,  0.0f},
    [DIR_NEG_Z] = { 0.0f,  0.0f, -1.0f},
};
// clang-format on

static void add_quad(struct vec3 offset, struct vec3 normal, float layer) {
    struct vec3 up = {0.0f, 1.0f, 0.0f};
    if (fabsf(normal.y) > 0.9f) {
        up = (struct vec3){1.0f, 0.0f, 0.0f};
    }

    struct vec3 tangent = vec3_normalize(vec3_cross(up, normal));
    struct vec3 bitangent = vec3_cross(normal, tangent);

    struct vec3 positions[4] = {
        vec3_add(offset, vec3_add(vec3_scale(tangent, -0.5f),
                                  vec3_scale(bitangent, -0.5f))),
        vec3_add(offset, vec3_add(vec3_scale(tangent, 0.5f),
                                  vec3_scale(bitangent, -0.5f))),
        vec3_add(offset, vec3_add(vec3_scale(tangent, 0.5f),
                                  vec3_scale(bitangent, 0.5f))),
        vec3_add(offset, vec3_add(vec3_scale(tangent, -0.5f),
                                  vec3_scale(bitangent, 0.5f))),
    };

    for (int i = 0; i < 4; i++) {
        vertices[vertex_count].position =
            vec3_add(positions[i], (struct vec3){0.5f, 0.5f, 0.5f});
        vertices[vertex_count].normal = normal;
        vertices[vertex_count].layer = layer;
        vertex_count++;
    }
}

void mesh_chunk(struct chunk *chunk) {
    vertex_count = 0;

    for (size_t z = 0; z < CHUNK_SIZE_Z; z++) {
        for (size_t y = 0; y < CHUNK_SIZE_Y; y++) {
            for (size_t x = 0; x < CHUNK_SIZE_X; x++) {
                enum block_type current_block = chunk_get_block(chunk, x, y, z);
                if (current_block != BLOCK_AIR) {
                    for (enum direction dir = 0; dir < DIR_COUNT; dir++) {
                        struct vec3 position = {x, y, z};

                        const struct block_properties *properties = get_block_properties(current_block);

                        struct vec3 normal = NORMAL_LUT[dir];
                        struct vec3 adjacent = vec3_add(position, normal);

                        enum block_type compare_block =
                            chunk_get_block(chunk, (int)adjacent.x,
                                            (int)adjacent.y, (int)adjacent.z);

                        if (compare_block == BLOCK_AIR) {
                            add_quad(
                                vec3_add(vec3_scale(normal, 0.5), position),
                                normal, properties->textures[dir]);
                        }
                    }
                }
            }
        }
    }

    float buffer_usage = ((float)vertex_count / MAX_VERTEX_COUNT) * 100.0f;
    printf("Buffer usage: %.2f%%\n", buffer_usage);
}

enum block_type place_block = BLOCK_STONE;

void glfw_scroll_callback(GLFWwindow *window, double x, double y) {
    (void)window;
    (void)x;

    if(y < 0) {
        place_block = (place_block - 1) % BLOCK_TYPE_COUNT;
    }
    else if(y > 0) {
        place_block = (place_block + 1) % BLOCK_TYPE_COUNT;
    }

    printf("Block type: %i\n", place_block);
}

int main(void) {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        fprintf(stderr, "glfwInit failed\n");
        return EXIT_FAILURE;
    }

    GLFWmonitor *primary_monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *video_mode = glfwGetVideoMode(primary_monitor);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RED_BITS, video_mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, video_mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, video_mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, video_mode->refreshRate);
    GLFWwindow *window = glfwCreateWindow(video_mode->width, video_mode->height,
                                          "Quadcraft", primary_monitor, NULL);
    if (!window) {
        fprintf(stderr, "glfwCreateWindow failed\n");
        return EXIT_FAILURE;
    }

    glfwSetFramebufferSizeCallback(window, glfw_framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, glfw_cursor_pos_callback);
    glfwSetKeyCallback(window, glfw_key_callback);
    glfwSetScrollCallback(window, glfw_scroll_callback);

    glfwMakeContextCurrent(window);

    if (!gladLoadGL(glfwGetProcAddress)) {
        fprintf(stderr, "gladLoadGL failed\n");
        return EXIT_FAILURE;
    }

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    struct chunk chunk0;
    chunk_init(&chunk0);

    GLuint program =
        load_program("res/shaders/chunk.vert", "res/shaders/chunk.frag");
    if (!program) {
        fprintf(stderr, "load_program failed\n");
        return EXIT_FAILURE;
    }

    uint32_t *indices = malloc(sizeof(uint32_t) * MAX_INDEX_COUNT);
    uint32_t offset = 0;
    for (uint32_t i = 0; i < MAX_INDEX_COUNT; i += 6) {
        indices[i + 0] = offset + 0;
        indices[i + 1] = offset + 1;
        indices[i + 2] = offset + 2;
        indices[i + 3] = offset + 0;
        indices[i + 4] = offset + 2;
        indices[i + 5] = offset + 3;

        offset += 4;
    }

    vertices = malloc(sizeof(struct vertex) * MAX_VERTEX_COUNT);

    mesh_chunk(&chunk0);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(struct vertex) * MAX_VERTEX_COUNT,
                 NULL, GL_DYNAMIC_DRAW);

    /* Position attribute */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex),
                          (const void *)offsetof(struct vertex, position));
    glEnableVertexAttribArray(0);

    /* Normal attribute */
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex),
                          (const void *)offsetof(struct vertex, normal));
    glEnableVertexAttribArray(1);

    /* Layer attribute */
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(struct vertex),
                          (const void *)offsetof(struct vertex, layer));
    glEnableVertexAttribArray(2);

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * MAX_INDEX_COUNT,
                 indices, GL_STATIC_DRAW);

    glBindVertexArray(0);

    GLuint texture_array;
    glGenTextures(1, &texture_array);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture_array);

    size_t num_layers = TEXTURE_ID_COUNT;

    glTexImage3D(GL_TEXTURE_2D_ARRAY,  // Target
                 0,                    // Mipmap level
                 GL_RGBA8,             // Internal format
                 16,                   // Texture width
                 16,                   // Texture height
                 num_layers,                  // Number of layers
                 0,                    // Border (must be 0)
                 GL_RGBA,              // Format of the pixel data
                 GL_UNSIGNED_BYTE,     // Data type
                 NULL);                // Data (NULL means uninitialized)

    uint8_t *error_texture = gen_error_texture_rgba8(16, 16);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, TEXTURE_ERROR, 16, 16, 1, GL_RGBA, GL_UNSIGNED_BYTE, error_texture);

    stbi_set_flip_vertically_on_load(true);

    for (enum texture_id id = 1; id < TEXTURE_ID_COUNT; id++) {
        const char *texture_path = get_texture_filename(id);

        int w;
        int h;
        int num_channels;
        unsigned char *data =
            stbi_load(texture_path, &w, &h, &num_channels, STBI_rgb_alpha);
        if (!data) {
            fprintf(stderr, "Failed to load texture: %s\n", texture_path);
            free(error_texture);
            return EXIT_FAILURE;
        }

        if (w != 16 || h != 16 || num_channels != 4) {
            fprintf(stderr,
                    "Texture dimensions are not 16x16 or have 4 channels\n");
            free(error_texture);
            return EXIT_FAILURE;
        }

        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, id, 16, 16, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    camera_init(&camera, 90.0f, 800.0f / 450.0f);

    camera.position.z = 3.0f;

    camera_update(&camera);

    float current_time = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        float new_time = glfwGetTime();
        float delta_time = new_time - current_time;
        current_time = new_time;

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1)) {
            int x = (int)(camera.position.x + camera.forward.x);
            int y = (int)(camera.position.y + camera.forward.y);
            int z = (int)(camera.position.z + camera.forward.z);

            chunk_set_block(&chunk0, x, y, z, BLOCK_AIR);
        }

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2)) {
            int x = (int)(camera.position.x + camera.forward.x);
            int y = (int)(camera.position.y + camera.forward.y);
            int z = (int)(camera.position.z + camera.forward.z);

            chunk_set_block(&chunk0, x, y, z, place_block);
        }

        if (chunk0.is_dirty) {
            mesh_chunk(&chunk0);

            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0,
                            sizeof(struct vertex) * vertex_count, vertices);
            chunk0.is_dirty = false;
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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(program);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, texture_array);
        glUniform1i(glGetUniformLocation(program, "u_texture"), 0);

        glBindVertexArray(vao);
        glUniformMatrix4fv(glGetUniformLocation(program, "u_mvp"), 1, GL_FALSE,
                           camera.view_proj.m);

        size_t quad_count = (vertex_count / 4);

        glDrawElements(GL_TRIANGLES, quad_count * 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}
