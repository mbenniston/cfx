#ifndef CFX_TEXT_H
#define CFX_TEXT_H
#include "types.h"

void textDrawChar(int x, int y, int size, char c, Color col);
void textDrawString(int x, int y, int size, const char* c, Color col);

#endif