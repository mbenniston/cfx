#ifndef CFX_DRAW_H
#define CFX_DRAW_H

#include "framebuffer.h"
#include "texture.h"
#include "types.h"

void dwDrawPoint(int x, int y, Color col);
void dwDrawRect(int x, int y, int w, int h, Color col);
void dwBlitImage(int x, int y, int w, int h, FilterMode filterMode,  Texture tex);
void dwDrawLine(int startX, int startY, int endX, int endY, Color col);

void dwDrawPointToTexture(int x, int y, Color col, Texture dest);
void dwDrawRectToTexture(int x, int y, int w, int h, Color col, Texture dest);
void dwBlitImageToTexture(int x, int y, int w, int h, FilterMode filterMode,  Texture tex, Texture dest);
void dwDrawLineToTexture(int startX, int startY, int endX, int endY, Color col,  Texture dest);

#endif