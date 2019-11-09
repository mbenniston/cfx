#include "texture.h"
#include <stdlib.h>
#include "../modules/stb/stb_image.h"

Texture texMakeEmpty(unsigned int w, unsigned int h, unsigned int channels){
    Texture out;
    out.width = w;
    out.height = h;
    out.channels = channels;
    out.pixels = malloc(w * h * channels);
    return out;
}

Texture texLoad(const char* path){
    Texture out;
    int w,h,c;
    out.pixels = stbi_load(path, &w, &h, &c, 0);
    out.width = w;
    out.height = h;
    out.channels = c;
    return out;
}

void texFree(Texture tex){
    free(tex.pixels);
}

Color texGetPixel(int i, int j, Texture tex) {
    if(i < 0) i =0;
    if(j < 0) j = 0;
    if(i >= tex.width) i = tex.width -1;
    if(j >= tex.height) j = tex.height - 1;

    Color col = {0,0,0};

    unsigned char* colPtr = &tex.pixels[(i + j * tex.width) * tex.channels];
    col.r = colPtr[0];
    col.g = colPtr[1];
    col.b = colPtr[2];
    return col;
}


