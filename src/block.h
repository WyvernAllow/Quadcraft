#ifndef BLOCK_H
#define BLOCK_H

#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>

#include "textures.h"

enum direction {
    DIR_POS_X,
    DIR_POS_Y,
    DIR_POS_Z,
    DIR_NEG_X,
    DIR_NEG_Y,
    DIR_NEG_Z,

    DIR_COUNT,
};

enum block_type {
    BLOCK_AIR,
    BLOCK_DIRT,
    BLOCK_STONE,
    BLOCK_GRASS,
    BLOCK_BRICK,
    BLOCK_PLANKS,
    BLOCK_LOG,

    BLOCK_TYPE_COUNT,
};

struct block_properties {
    bool is_transparent;
    enum texture_id textures[DIR_COUNT];
};

const struct block_properties *get_block_properties(enum block_type type);

#endif /* BLOCK_H */
