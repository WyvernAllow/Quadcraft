#include "utils.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

char *load_file_str(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror(filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = malloc(size + 1);
    if (!buffer) {
        perror(filename);
        fclose(file);
        return NULL;
    }

    if (fread(buffer, 1, size, file) != size) {
        perror(filename);
        free(buffer);
        fclose(file);
        return NULL;
    }

    fclose(file);

    buffer[size] = '\0';
    return buffer;
}

unsigned char *gen_error_texture_rgba8(int width, int height) {
    if (width % 2 != 0 || height % 2 != 0) {
        return NULL;
    }

    unsigned char *texture = malloc(width * height * 4 * sizeof(uint8_t));
    if (!texture) {
        return NULL;
    }

    unsigned char magenta[4] = {255, 0, 255, 255};
    unsigned char black[4] = {0, 0, 0, 255};

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            unsigned char *color =
                ((x / (width / 2)) ^ (y / (height / 2))) ? magenta : black;

            int index = (y * width + x) * 4;

            texture[index + 0] = color[0];
            texture[index + 1] = color[1];
            texture[index + 2] = color[2];
            texture[index + 3] = color[3];
        }
    }

    return texture;
}
