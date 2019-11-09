#include "draw.h"
#include <stdio.h>

void dwDrawPoint(int x, int y, Color col, FrameBuffer fb) {
    //check if the point exists in the fb
    if(x < 0 || y < 0 || x >= fb.width || y >= fb.height || fb.channels < 3) 
        return;

    //get a ptr to the 'r' element of the color
    unsigned char* colPtr = &fb.data[(x + y * fb.width) * fb.channels];
    colPtr[0] = col.b;
    colPtr[1] = col.g;
    colPtr[2] = col.r;
    colPtr[3] = 255;
}

void dwDrawRect(int x, int y, int w, int h, Color col, FrameBuffer fb) {
    //check if the rect is completely off the fb
    if(((x + w) <= 0) || ((y + h) <= 0) || (x >= (long)fb.width) || (y >= (long)fb.height)) {
        return;
    }

    //clip occordingly 
    if(x < 0) { w = x + w; x = 0; }
    if(y < 0) { h = y + h; y = 0; }

    if(x + w >= fb.width) { w = fb.width - x;  }
    if(y + h >= fb.height){ h = fb.height - y; }
    
    //draw each pixel in the rect
    for(int i = x; i < x+w; i++) {
        for(int j = y; j < y+h; j++) {
            dwDrawPoint(i,j, col, fb);
        }
    }
}

void dwBlitImage(int x, int y, int w, int h, Texture tex, FrameBuffer fb) {
    //check if the rect is completely off the fb
    int mx = x, my = y;

    if(((x + w) <= 0) || ((y + h) <= 0) || (x >= (long)fb.width) || (y >= (long)fb.height)) {
        return;
    }

    //clip occordingly 
    if(x < 0) { w = x + w; mx = 0; }
    if(y < 0) { h = y + h; my = 0; }

    if(mx + w >= fb.width) { w = fb.width - mx;  }
    if(my + h >= fb.height){ h = fb.height - my; }
    
    //draw each pixel in the rect
    for(int i = mx; i < mx+w; i++) {
        for(int j = my; j < my+h; j++) {
            int tx = tex.width * (i - x) / (double)w;
            int ty = tex.height * (j - y) / (double)h;

            dwDrawPoint(i,j,texGetPixel(tx, ty, tex), fb);
        }
    }
}

void dwDrawLine(int startX, int startY, int endX, int endY, Color col, FrameBuffer fb){

    int stepX = endX - startX;
    int stepY = endY - startY;
    for(int x = startX; x < endX; x++){
        int y = startY + stepY * (x - startX) / (double)stepX;
        dwDrawPoint(x,y, col, fb);
    }
}
