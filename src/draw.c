#include "draw.h"
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include "../modules/font8x8/font8x8.h"

extern Texture window_Fb;

static double lerp(double a, double b, double c) {
    return a + (b - a) * c;
}

void dwDrawPoint(int x, int y, Color col) {
    dwDrawPointToTexture(x,y,col, window_Fb);
}

void dwDrawRect(int x, int y, int w, int h, Color col) {
    dwDrawRectToTexture(x,y, w,h, col, window_Fb);
}

void dwBlitImage(int x, int y, int w, int h, FilterMode filterMode,  Texture tex){
    dwBlitImageToTexture(x,y,w,h,filterMode, tex, window_Fb);
}

void dwDrawLine(int startX, int startY, int endX, int endY, Color col){
    dwDrawLineToTexture(startX, startY, endX, endY, col, window_Fb);
}


void dwDrawPointToTexture(int x, int y, Color col, Texture dest) {
    //check if the point exists in the fb
    if(x < 0 || y < 0 || x >= dest.width || y >= dest.height || dest.channels < 3) 
        return;

    //get a ptr to the 'r' element of the color
    unsigned char* colPtr = &dest.pixels[(x + y * dest.width) * dest.channels];
    colPtr[0] = col.b;
    colPtr[1] = col.g;
    colPtr[2] = col.r;
    colPtr[3] = 255;
}

void dwDrawRectToTexture(int x, int y, int w, int h, Color col, Texture dest) {
    //check if the rect is completely off the fb
    if(((x + w) <= 0) || ((y + h) <= 0) || (x >= (long)dest.width) || (y >= (long)dest.height)) {
        return;
    }

    //clip occordingly 
    if(x < 0) { w = x + w; x = 0; }
    if(y < 0) { h = y + h; y = 0; }

    if(x + w >= dest.width) { w = dest.width - x;  }
    if(y + h >= dest.height){ h = dest.height - y; }
    
    //draw each pixel in the rect
    for(int i = x; i < x+w; i++) {
        for(int j = y; j < y+h; j++) {
            dwDrawPointToTexture(i,j, col, dest);
        }
    }
}

void dwBlitImageToTexture(int x, int y, int w, int h, FilterMode filterMode,  Texture tex, Texture dest){
    //check if the rect is completely off the fb
    int mx = x, my = y;
    int mw = w, mh = w;

    if(((x + w) <= 0) || ((y + h) <= 0) || (x >= (long)dest.width) || (y >= (long)dest.height)) {
        return;
    }

    //clip occordingly 
    if(x < 0) { mw = x + w; mx = 0; }
    if(y < 0) { mh = y + h; my = 0; }

    if(mx + w >= dest.width) { mw = dest.width - mx;  }
    if(my + h >= dest.height){ mh = dest.height - my; }
    
    //draw each pixel in the rect
    for(int i = mx; i < mx+mw; i++) {
        for(int j = my; j < my+mh; j++) {
            double tx = tex.width * (i - x) / (double)w;
            double ty = tex.height * (j - y) / (double)h;

            switch (filterMode)
            {
            case FM_NEAREST:
                dwDrawPointToTexture( i,j,texGetPixel(round(tx), round(ty), tex), dest);
                break;
            case FM_BILINEAR:
            {
                double x1 = floor(tx); 
                double x2 = ceil(tx); 

                double y1 = floor(ty); 
                double y2 = ceil(ty); 

                double sx = (tx - x1) / (x2 - x1);               
                double sy = (ty - y1) / (y2 - y1);

                if(x2 - x1 == 0) sx = 0;
                if(y2 - y1 == 0) sy = 0;
                
                Color topL = texGetPixel(x1, y2, tex);
                Color topR = texGetPixel(x2, y2, tex);
                double top_r = lerp(topL.r, topR.r, sx); 
                double top_g = lerp(topL.g, topR.g, sx);
                double top_b = lerp(topL.b, topR.b, sx);
                
                Color bottomL = texGetPixel(x1, y1, tex);
                Color bottomR = texGetPixel(x2, y1, tex);
                double bottom_r = lerp(bottomL.r, bottomR.r, sx); 
                double bottom_g = lerp(bottomL.g, bottomR.g, sx);
                double bottom_b = lerp(bottomL.b, bottomR.b, sx); 

                double out_r = lerp(bottom_r, top_r, sy); 
                double out_g = lerp(bottom_g, top_g, sy); 
                double out_b = lerp(bottom_b, top_b, sy); 
                dwDrawPointToTexture(i,j, (Color){out_r, out_g, out_b},  dest);
            }

                break;
            default:
                break;
            }
        }
    }
}

void dwDrawLineToTexture(int startX, int startY, int endX, int endY, Color col, Texture dest){

    int stepX = endX - startX;
    int stepY = endY - startY;
    for(int x = startX; x < endX; x++){
        int y = startY + stepY * (x - startX) / (double)stepX;
        dwDrawPointToTexture(x,y, col, dest);
    }
}

void dwDrawCharToTexture(int x, int y, int size, char c, Color col, Texture dest)
{
    char* character = font8x8_basic[(int)c];

    for (int i=0; i < 8; i++) {
        for (int j=0; j < 8; j++) {
            bool s = character[j] & 1 << i;
            if(s) {
                dwDrawRectToTexture(x + i * size, y + j * size, size, size, col, dest);
            }
        }
    }    
}

void dwDrawStringToTexture(int x, int y, int size, const char* c, Color col, Texture dest)
{
    for(int i = 0; i < strlen(c); i++){
        dwDrawCharToTexture(x + i * 8 * size, y, size, c[i], col, dest);
    }
}

void dwDrawChar(int x, int y, int size, char c, Color col) 
{
    dwDrawCharToTexture(x, y, size, c, col, window_Fb);
}

void dwDrawString(int x, int y, int size, const char* c, Color col)
{
    dwDrawStringToTexture(x, y, size, c, col, window_Fb);
}

