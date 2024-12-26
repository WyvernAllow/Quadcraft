#ifndef BLOCK_H
#define BLOCK_H

#include <stddef.h>
#include <stdio.h>

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

    BLOCK_TYPE_COUNT,
};

#endif /* BLOCK_H */
