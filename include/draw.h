#ifndef CFX_DRAW_H
#define CFX_DRAW_H

#include "framebuffer.h"
#include "texture.h"
#include "types.h"

void dwDrawPoint(int x, int y, Color col, FrameBuffer fb);
void dwDrawRect(int x, int y, int w, int h, Color col, FrameBuffer fb);
void dwBlitImage(int x, int y, Texture tex, FrameBuffer fb);


#endif