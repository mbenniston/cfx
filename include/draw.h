#ifndef CFX_DRAW_H
#define CFX_DRAW_H

#include "texture.h"
#include "types.h"

//Draw commands that can be used on off screen textures
void dwDrawPointToTexture(int x, int y, Color col, Texture dest);
void dwDrawRectToTexture(int x, int y, int w, int h, Color col, Texture dest);
void dwBlitImageToTexture(int x, int y, int w, int h, FilterMode filterMode,  Texture tex, Texture dest);
void dwDrawLineToTexture(int startX, int startY, int endX, int endY, Color col,  Texture dest);
void dwDrawCharToTexture(int x, int y, int size, char c, Color col, Texture dest);
void dwDrawStringToTexture(int x, int y, int size, const char* c, Color col, Texture dest);

//Draw Commands that use the window framebuffer as the drawing destination
void dwDrawPoint(int x, int y, Color col);
void dwDrawRect(int x, int y, int w, int h, Color col);
void dwBlitImage(int x, int y, int w, int h, FilterMode filterMode,  Texture tex);
void dwDrawLine(int startX, int startY, int endX, int endY, Color col);
void dwDrawChar(int x, int y, int size, char c, Color col);
void dwDrawString(int x, int y, int size, const char* c, Color col);

#endif