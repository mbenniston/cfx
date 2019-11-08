#ifndef CFX_TEXTURE_H
#define CFX_TEXTURE_H

#include "types.h"

typedef struct {
    unsigned int width, height, channels;
    unsigned char* pixels;
} Texture;

Texture texMakeEmpty(unsigned int w, unsigned int h, unsigned int channels);
Texture texLoad(const char* path);
void texFree(Texture tex);

Color texGetPixel(int i, int j, Texture tex);

#endif