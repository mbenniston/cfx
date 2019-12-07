#ifndef CFX_DW_CMDS
#define CFX_DW_CMDS

#include "types.h"
#include "texture.h"

typedef enum { DM_IMMEDIATE, DM_BUFFERED } DrawMode;

typedef enum { NULL_CMD, POINT_CMD, RECT_CMD, LINE_CMD, BLIT_IMAGE_CMD, CHAR_CMD } Cmd_Type;
typedef struct { Cmd_Type type; } Cmd;

typedef struct {
    Cmd_Type type;
    int x, y;
    Color color;
    Texture texture;
} Cmd_Point;

typedef struct {
    Cmd_Type type;
    int x, y, w, h;
    Color color;
    Texture texture;
} Cmd_Rect;

typedef struct {
    Cmd_Type type;
    int startX, startY, endX, endY;
    Color color;
    Texture texture;
} Cmd_Line;

typedef struct {
    Cmd_Type type;
    int x,y, w,h;
    FilterMode filterMode;
    Texture srcTexture;
    Texture destTexture;
} Cmd_Image;

typedef struct {
    Cmd_Type type;
    int x,y, w,h;
    char character;
    Color color;
    Texture destTexture;
} Cmd_Char;

void pushCmd(const Cmd* const cmd);

void setDrawMode(DrawMode dm);

void process_point(Cmd_Point);
void process_rect(Cmd_Rect);
void process_image(Cmd_Image);
void process_line(Cmd_Line);
void process_char(Cmd_Char);

#endif