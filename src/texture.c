#include "texture.h"
#include <stdlib.h>
#include <string.h>
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
    
    if(c >= 3) {
        for(int x = 0; x < w; x++){
            for(int y = 0; y < h; y++){
                unsigned char* col = &out.pixels[(x + y * w) * c];
                unsigned char temp = col[0];
                col[0] = col[2];
                col[2] = temp;
            }
        }
    } 
    
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
    col.b = colPtr[0];
    col.g = colPtr[1];
    col.r = colPtr[2];
    return col;
}

void texSetPixel(int i, int j, Color col, Texture tex)
{
    if(i < 0) i =0;
    if(j < 0) j = 0;
    if(i >= tex.width) i = tex.width -1;
    if(j >= tex.height) j = tex.height - 1;

    unsigned char* colPtr = &tex.pixels[(i + j * tex.width) * tex.channels];
    colPtr[0] = col.b;
    colPtr[1] = col.g;
    colPtr[2] = col.r;
}

void texClear(Texture tex) {
    memset(tex.pixels, 0x00, tex.width * tex.height * tex.channels);
}

void texBlit(Texture* const readbuf, Texture* const writebuf) {
    if(readbuf->width == writebuf->width && readbuf->height == writebuf->height && readbuf->channels == writebuf->channels) {
        memcpy(writebuf->pixels, readbuf->pixels, readbuf->width * readbuf->height * readbuf->channels);
    }
    for(int i = 0; i < writebuf->width; i++) {
        for(int j = 0; j < writebuf->height; j++) {
            double si = i / (double)writebuf->width;
            double sj = j / (double)writebuf->height;
            int x = si * readbuf->width;
            int y = sj * readbuf->height;
            unsigned char* destcol = &writebuf->pixels[(i + j * writebuf->width) * writebuf->channels];
            unsigned char* readcol = &readbuf->pixels[(x + y * readbuf->width) * readbuf->channels];
            memcpy(destcol, readcol, sizeof(unsigned char) * readbuf->channels);
        }
    }
}
