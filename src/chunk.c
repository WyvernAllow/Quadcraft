#include "chunk.h"

#include <stdbool.h>

static bool in_bounds(int x, int y, int z) {
    return x >= 0 && x < CHUNK_SIZE_X && y >= 0 && y < CHUNK_SIZE_Y && z >= 0 &&
           z < CHUNK_SIZE_Z;
}

static size_t get_index(int x, int y, int z) {
    return x + y * CHUNK_SIZE_X + z * CHUNK_SIZE_X * CHUNK_SIZE_Y;
}

void chunk_init(struct chunk *chunk) {
    chunk->x = 0;
    chunk->y = 0;
    chunk->z = 0;

    for (size_t i = 0; i < CHUNK_VOLUME; i++) {
        chunk->blocks[i] = BLOCK_DIRT;
    }
}

enum block_type chunk_get_block(const struct chunk *chunk, int x, int y,
                                int z) {
    if (!in_bounds(x, y, z)) {
        return BLOCK_AIR;
    }

    return chunk->blocks[get_index(x, y, z)];
}

void chunk_set_block(struct chunk *chunk, int x, int y, int z,
                     enum block_type type) {
    if (!in_bounds(x, y, z)) {
        return;
    }

    chunk->blocks[get_index(x, y, z)] = type;
}
