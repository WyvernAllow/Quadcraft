#include "block.h"
#include "textures.h"

// clang-format off
static const struct block_properties block_table[BLOCK_TYPE_COUNT] = {
    [BLOCK_AIR] = {
        .is_transparent = true,
        .textures = {0},
    },
    [BLOCK_DIRT] = {
        .is_transparent = false,
        .textures = {
            [DIR_POS_X] = TEXTURE_DIRT,
            [DIR_POS_Y] = TEXTURE_DIRT,
            [DIR_POS_Z] = TEXTURE_DIRT,
            [DIR_NEG_X] = TEXTURE_DIRT,
            [DIR_NEG_Y] = TEXTURE_DIRT,
            [DIR_NEG_Z] = TEXTURE_DIRT,
        },
    },
    [BLOCK_STONE] = {
        .is_transparent = false,
        .textures = {
            [DIR_POS_X] = TEXTURE_STONE,
            [DIR_POS_Y] = TEXTURE_STONE,
            [DIR_POS_Z] = TEXTURE_STONE,
            [DIR_NEG_X] = TEXTURE_STONE,
            [DIR_NEG_Y] = TEXTURE_STONE,
            [DIR_NEG_Z] = TEXTURE_STONE,
        },
    },
    [BLOCK_GRASS]= {
        .is_transparent = false,
        .textures = {
            [DIR_POS_X] = TEXTURE_GRASS_SIDE,
            [DIR_POS_Y] = TEXTURE_GRASS_TOP,
            [DIR_POS_Z] = TEXTURE_GRASS_SIDE,
            [DIR_NEG_X] = TEXTURE_GRASS_SIDE,
            [DIR_NEG_Y] = TEXTURE_DIRT,
            [DIR_NEG_Z] = TEXTURE_GRASS_SIDE,
        },
    },

    [BLOCK_BRICK] = {
        .is_transparent = false,
        .textures = {
            [DIR_POS_X] = TEXTURE_BRICK,
            [DIR_POS_Y] = TEXTURE_BRICK,
            [DIR_POS_Z] = TEXTURE_BRICK,
            [DIR_NEG_X] = TEXTURE_BRICK,
            [DIR_NEG_Y] = TEXTURE_BRICK,
            [DIR_NEG_Z] = TEXTURE_BRICK,
        },
    },
    [BLOCK_PLANKS] = {
        .is_transparent = false,
        .textures = {
            [DIR_POS_X] = TEXTURE_PLANKS,
            [DIR_POS_Y] = TEXTURE_PLANKS,
            [DIR_POS_Z] = TEXTURE_PLANKS,
            [DIR_NEG_X] = TEXTURE_PLANKS,
            [DIR_NEG_Y] = TEXTURE_PLANKS,
            [DIR_NEG_Z] = TEXTURE_PLANKS,
        },
    },
    [BLOCK_LOG] = {
        .is_transparent = false,
        .textures = {
            [DIR_POS_X] = TEXTURE_LOG_SIDE,
            [DIR_POS_Y] = TEXTURE_LOG_TOP,
            [DIR_POS_Z] = TEXTURE_LOG_SIDE,
            [DIR_NEG_X] = TEXTURE_LOG_SIDE,
            [DIR_NEG_Y] = TEXTURE_LOG_TOP,
            [DIR_NEG_Z] = TEXTURE_LOG_SIDE,
        },
    },
};
// clang-format on

const struct block_properties *get_block_properties(enum block_type type) {
    return &block_table[type];
}