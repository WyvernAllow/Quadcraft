#ifndef TEXTURES_H
#define TEXTURES_H

enum texture_id {
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
};

const char *get_texture_filename(enum texture_id id);

#endif /* TEXTURES_H */
