#ifndef CFX_TEXTURE_H
#define CFX_TEXTURE_H

#include "types.h"

typedef struct {
    unsigned int width, height, channels;
    unsigned char* pixels;
} Texture;

//Makes an empty texture of width w and height h with a number of channels
Texture texMakeEmpty(unsigned int w, unsigned int h, unsigned int channels);

//Loads a texture from file into memory using the stb_image library
Texture texLoad(const char* path);

//Fill the textures pixel data with zeros
void texClear(Texture tex);

//Blits one texture to another, upscaling / downscaling where neccessary using nearest neighboar sampling
void texBlit(const Texture* const readbuf, Texture* const writebuf);

//Frees pixel data allocated in memory
void texFree(Texture tex);

//Returns the Color of a pixel at i and j, clamps i and j to the edge of the texture if bounds are passed
Color texGetPixel(int i, int j, Texture tex);

//Sets a pixels color in a texture, with  bouds checking
void texSetPixel(int i, int j, Color col, Texture tex);

//Sets a pixels color in a texture, without bouds checking
void texSetPixelFast(int i, int j, Color col, Texture tex);

#endif