#include "block.h"

static const char *texture_filenames[TEXTURE_ID_COUNT] = {
    [TEXTURE_DIRT] = "res/textures/dirt.png",
    [TEXTURE_GRASS_TOP] = "res/textures/grass_top.png",
    [TEXTURE_GRASS_SIDE] = "res/textures/grass_side.png",
    [TEXTURE_LOG_TOP] = "res/textures/log_top.png",
    [TEXTURE_LOG_SIDE] = "res/textures/log_side.png",
    [TEXTURE_PLANKS] = "res/textures/planks.png",
    [TEXTURE_STONE] = "res/textures/stone.png",
    [TEXTURE_BRICK] = "res/textures/brick.png",
};

const char *get_texture_filename(texture_id id) {
    return texture_filenames[id];
}

// clang-format off
static const block_properties block_table[BLOCK_TYPE_COUNT] = {
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

const block_properties *get_block_properties(block_type type) {
    return &block_table[type];
}
