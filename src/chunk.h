#ifndef CHUNK_H
#define CHUNK_H
#include <stdbool.h>
#include <stddef.h>

#include "block.h"

#define CHUNK_SIZE_X 16
#define CHUNK_SIZE_Y 16
#define CHUNK_SIZE_Z 16
#define CHUNK_VOLUME (CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z)

typedef struct chunk {
    int x;
    int y;
    int z;

    bool is_dirty;

    block_type blocks[CHUNK_VOLUME];
} chunk;

void chunk_init(chunk *chunk);
block_type chunk_get_block(const chunk *chunk, int x, int y, int z);
void chunk_set_block(chunk *chunk, int x, int y, int z, block_type type);

#endif /* CHUNK_H */
