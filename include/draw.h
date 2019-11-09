#ifndef CFX_DRAW_H
#define CFX_DRAW_H

#include "framebuffer.h"
#include "texture.h"
#include "types.h"

void dwDrawPoint(int x, int y, Color col, FrameBuffer fb);
void dwDrawRect(int x, int y, int w, int h, Color col, FrameBuffer fb);
void dwBlitImage(int x, int y, int w, int h, FilterMode filterMode,  Texture tex, FrameBuffer fb);
void dwDrawLine(int startX, int startY, int endX, int endY, Color col, FrameBuffer fb);

#endif