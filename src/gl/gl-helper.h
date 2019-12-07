#ifndef CFX_GLHELPER_H
#define CFX_GLHELPER_H

#include "types.h"
#include "./glad/glad.h"

unsigned int glhLoadTexture(const char* path); 
void glhBindTexture(unsigned int t);
void glhUnbindTexture();
void glhUnloadTexture(unsigned int texture);

void glhInit();
void glhRect(int x, int y, int w, int h);
void glhRectColor(int x, int y, int w, int h, Color col);
void glhLineColor(int startX, int startY, int endX, int endY, Color col);
void glhCharColor(int x, int y, int w, int h, char c, Color col);
void glhCleanup();

#endif