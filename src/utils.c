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
