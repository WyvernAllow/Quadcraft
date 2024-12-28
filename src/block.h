#ifndef BLOCK_H
#define BLOCK_H

#include <stdbool.h>
#include <stddef.h>

typedef enum texture_id {
    TEXTURE_ERROR,
    TEXTURE_DIRT,
    TEXTURE_GRASS_TOP,
    TEXTURE_GRASS_SIDE,
    TEXTURE_LOG_TOP,
    TEXTURE_LOG_SIDE,
    TEXTURE_PLANKS,
    TEXTURE_STONE,
    TEXTURE_BRICK,

    TEXTURE_ID_COUNT,
} texture_id;

const char *get_texture_filename(texture_id id);

typedef enum direction {
    DIR_POS_X,
    DIR_POS_Y,
    DIR_POS_Z,
    DIR_NEG_X,
    DIR_NEG_Y,
    DIR_NEG_Z,

    DIR_COUNT,
} direction;

typedef enum block_type {
    BLOCK_AIR,
    BLOCK_DIRT,
    BLOCK_STONE,
    BLOCK_GRASS,
    BLOCK_BRICK,
    BLOCK_PLANKS,
    BLOCK_LOG,

    BLOCK_TYPE_COUNT,
} block_type;

typedef struct block_properties {
    bool is_transparent;
    texture_id textures[DIR_COUNT];
} block_properties;

const block_properties *get_block_properties(block_type type);

#endif /* BLOCK_H */
