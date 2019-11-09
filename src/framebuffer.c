#include <stdlib.h>
#include <string.h>
#include "framebuffer.h"

FrameBuffer fbMake(unsigned int width, unsigned int height, unsigned int channels) {
    FrameBuffer out;
    out.width = width;
    out.height = height;
    out.channels = channels;
    out.data = malloc(width * height * channels);
    return out;    
}

void fbClear(FrameBuffer* const fb) {
    memset(fb->data, 0x00, fb->width * fb->height * fb->channels);
}

void fbFree(FrameBuffer* const fb) {
    free(fb->data);
}

void fbBlit(FrameBuffer* const readbuf, FrameBuffer* const writebuf) {
    if(readbuf->width == writebuf->width && readbuf->height == writebuf->height && readbuf->channels == writebuf->channels) {
        memcpy(writebuf->data, readbuf->data, readbuf->width * readbuf->height * readbuf->channels);
    }
    for(int i = 0; i < writebuf->width; i++) {
        for(int j = 0; j < writebuf->height; j++) {
            double si = i / (double)writebuf->width;
            double sj = j / (double)writebuf->height;
            int x = si * readbuf->width;
            int y = sj * readbuf->height;
            unsigned char* destcol = &writebuf->data[(i + j * writebuf->width) * writebuf->channels];
            unsigned char* readcol = &readbuf->data[(x + y * readbuf->width) * readbuf->channels];
            memcpy(destcol, readcol, sizeof(unsigned char) * readbuf->channels);
        }
    }
}

