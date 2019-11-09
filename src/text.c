#include <stdbool.h>
#include <string.h>
#include "../modules/font8x8/font8x8.h"
#include "text.h"
#include "draw.h"

void textDrawChar(int x, int y, int size, char c, Color col, FrameBuffer fb) {
    char* character = font8x8_basic[(int)c];

    for (int i=0; i < 8; i++) {
        for (int j=0; j < 8; j++) {
            bool s = character[j] & 1 << i;
            if(s) {
                dwDrawRect(x + i * size, y + j * size, size, size, col, fb);
            }
        }
    }    
}

void textDrawString(int x, int y, int size, const char* c, Color col, FrameBuffer fb) {
    for(int i = 0; i < strlen(c); i++){
        textDrawChar(x + i * 8 * size, y, size, c[i], col, fb);
    }
}