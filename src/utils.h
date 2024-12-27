#ifndef UTILS_H
#define UTILS_H
#include <stddef.h>
#include <stdint.h>

#define ARRAY_LENGTH(array) (sizeof(array) / sizeof((array)[0]))
char *load_file_str(const char *filename);
unsigned char *gen_error_texture_rgba8(int width, int height);

#endif /* UTILS_H */
