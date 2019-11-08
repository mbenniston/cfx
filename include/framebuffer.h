#ifndef CFX_FRAMEBUFFER_H
#define CFX_FRAMEBUFFER_H

typedef struct {
    unsigned char* data;
    unsigned int width, height, channels;
} FrameBuffer;

FrameBuffer fbMake(unsigned int width, unsigned int height, unsigned int channels);
void fbClear(FrameBuffer* const fb);
void fbFree(FrameBuffer* const fb);

#endif