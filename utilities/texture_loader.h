#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#define STB_IMAGE_IMPLEMENTATION
#define GL_CLAMP_TO_EDGE 0x812F

#include "stb_image.h"

static void load_texture(GLuint& texture_id, const char* filename) {
    int width, height, components;
    unsigned char* data;

    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(filename, &width, &height, &components, STBI_rgb_alpha);
    glPushAttrib(GL_TEXTURE_BIT);
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    free(data);
    glPopAttrib();
}

#endif // !TEXTURE_LOADER_H
