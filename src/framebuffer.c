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
    memset(fb->data, 0xFF, fb->width * fb->height * fb->channels);
}

void fbFree(FrameBuffer* const fb) {
    free(fb->data);
}


