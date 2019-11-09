#include "draw.h"
#include <stdio.h>
#include <math.h>

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

double lerp(double a, double b, double c) {
    return a + (b - a) * c;
}

void dwBlitImage(int x, int y, int w, int h, FilterMode filterMode,  Texture tex, FrameBuffer fb){
    //check if the rect is completely off the fb
    int mx = x, my = y;
    int mw = w, mh = w;

    if(((x + w) <= 0) || ((y + h) <= 0) || (x >= (long)fb.width) || (y >= (long)fb.height)) {
        return;
    }

    //clip occordingly 
    if(x < 0) { mw = x + w; mx = 0; }
    if(y < 0) { mh = y + h; my = 0; }

    if(mx + w >= fb.width) { mw = fb.width - mx;  }
    if(my + h >= fb.height){ mh = fb.height - my; }
    
    //draw each pixel in the rect
    for(int i = mx; i < mx+mw; i++) {
        for(int j = my; j < my+mh; j++) {
            double tx = tex.width * (i - x) / (double)w;
            double ty = tex.height * (j - y) / (double)h;

            switch (filterMode)
            {
            case FM_NEAREST:
                dwDrawPoint(i,j,texGetPixel(round(tx), round(ty), tex), fb);
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
                dwDrawPoint(i,j, (Color){out_r, out_g, out_b},  fb);
            }

                break;
            default:
                break;
            }
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
