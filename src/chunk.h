#ifndef CHUNK_H
#define CHUNK_H
#include <stddef.h>
#include <stdbool.h>

#include "block.h"

#define CHUNK_SIZE_X 16
#define CHUNK_SIZE_Y 16
#define CHUNK_SIZE_Z 16
#define CHUNK_VOLUME (CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z)

struct chunk {
    int x;
    int y;
    int z;

    bool is_dirty;

    enum block_type blocks[CHUNK_VOLUME];
};

void chunk_init(struct chunk *chunk);

enum block_type chunk_get_block(const struct chunk *chunk, int x, int y, int z);
void chunk_set_block(struct chunk *chunk, int x, int y, int z,
                     enum block_type type);

#endif /* CHUNK_H */
