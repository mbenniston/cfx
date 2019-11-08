#ifndef CFX_TEXT_H
#define CFX_TEXT_H
#include "types.h"
#include "framebuffer.h"

void textDrawChar(int x, int y, int size, char c, Color col, FrameBuffer fb);
void textDrawString(int x, int y, int size, const char* c, Color col, FrameBuffer fb);

#endif