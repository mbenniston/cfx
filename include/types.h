#ifndef CFX_TYPES_H
#define CFX_TYPES_H

typedef struct {
    unsigned char r,g,b;
} Color;

typedef enum {
    FM_NEAREST, FM_BILINEAR
} FilterMode;

#endif