#include "chunk.h"

#include <stdbool.h>

static bool in_bounds(int x, int y, int z) {
    return x >= 0 && x < CHUNK_SIZE_X && y >= 0 && y < CHUNK_SIZE_Y && z >= 0 &&
           z < CHUNK_SIZE_Z;
}

static size_t get_index(int x, int y, int z) {
    return x + y * CHUNK_SIZE_X + z * CHUNK_SIZE_X * CHUNK_SIZE_Y;
}

void chunk_init(chunk *chunk) {
    chunk->x = 0;
    chunk->y = 0;
    chunk->z = 0;

    for (size_t z = 0; z < CHUNK_SIZE_Z; z++) {
        for (size_t y = 0; y < CHUNK_SIZE_Y; y++) {
            for (size_t x = 0; x < CHUNK_SIZE_X; x++) {
                chunk->blocks[get_index(x, y, z)] = BLOCK_AIR;

                if (y == 8) {
                    chunk->blocks[get_index(x, y, z)] = BLOCK_GRASS;
                } else if (y < 3) {
                    chunk->blocks[get_index(x, y, z)] = BLOCK_STONE;
                } else if (y < 8) {
                    chunk->blocks[get_index(x, y, z)] = BLOCK_DIRT;
                }
            }
        }
    }

    chunk->is_dirty = true;
}

block_type chunk_get_block(const chunk *chunk, int x, int y, int z) {
    if (!in_bounds(x, y, z)) {
        return BLOCK_AIR;
    }

    return chunk->blocks[get_index(x, y, z)];
}

void chunk_set_block(chunk *chunk, int x, int y, int z, block_type type) {
    if (!in_bounds(x, y, z)) {
        return;
    }

    block_type old_block = chunk->blocks[get_index(x, y, z)];
    if (type == old_block) {
        return;
    }

    chunk->blocks[get_index(x, y, z)] = type;
    chunk->is_dirty = true;
}
