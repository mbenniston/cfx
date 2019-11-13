#include "../modules/minifb/include/MiniFB.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "window.h"
#include "dw_cmds.h"

DrawMode window_DrawMode = DM_IMMEDIATE;
void* window_CmdBuf;
size_t window_CmdBufSize, window_CmdBufMaxSize;

Texture window_Fb;

int keyMap[400];
int mouseMap[12];

static struct Window* s_Window = NULL;
static bool shouldClose = false;
static int s_mouseX = 0, s_mouseY = 0, s_WindowWidth = 0, s_WindowHeight = 0;

void process_CmdBuf();

void setDrawMode(DrawMode dm)
{
    window_DrawMode = dm;
}

void pushCmd(const Cmd* const cmd)
{
    int size = 0;
    switch (cmd->type)
    {
    case POINT_CMD:
        size += sizeof(Cmd_Point);
        break;
    case RECT_CMD:
        size += sizeof(Cmd_Rect);
        break;
    case BLIT_IMAGE_CMD:
        size += sizeof(Cmd_Image);
        break;
    case LINE_CMD:
        size += sizeof(Cmd_Line);
        break;
    default:
        break;
    }

    if(window_CmdBufSize + size > window_CmdBufMaxSize) {
        window_CmdBuf = realloc(window_CmdBuf, window_CmdBufMaxSize + size * 10);
        window_CmdBufMaxSize += size * 10;
    }

    memcpy((char*)window_CmdBuf + window_CmdBufSize, cmd, size);
    window_CmdBufSize += size;
}

static double lerp(double a, double b, double c) {
    return a + (b - a) * c;
}

static void win_kb_func(struct Window *window, Key key, KeyMod mod, bool isPressed) {
    keyMap[key] = isPressed;
}

static void win_mouse_move_func(struct Window *window, int x, int y) {
    s_mouseX = x;
    s_mouseY = y;
}

static void win_mouse_button_func(struct Window *window, MouseButton button, KeyMod mod, bool isPressed) {
    mouseMap[button] = isPressed;
}

void winOpen(int width, int height) {
    s_WindowWidth = width;
    s_WindowHeight = height;
    s_Window = mfb_open("Test Window", width, height);

    window_Fb = texMakeEmpty(width, height, 4);

    mfb_keyboard_callback(s_Window, win_kb_func);
    mfb_mouse_move_callback(s_Window, win_mouse_move_func);
    mfb_mouse_button_callback(s_Window, win_mouse_button_func);

    window_CmdBufSize = 0;
    window_CmdBufMaxSize = 4086 * 8;
    window_CmdBuf = malloc(window_CmdBufMaxSize);
}

void winClose() {
    texFree(window_Fb);
    free(window_CmdBuf);
    mfb_close(s_Window);
}

void winClear(){
    texClear(window_Fb);
}

void winBlit(const Texture* const readbuf)
{
    texBlit(readbuf, &window_Fb);
}


bool winShouldClose() {
    return shouldClose;
}

int winGetKey(int keyCode) {
    return keyMap[keyCode];
}

void winUpdate() {
    if(window_DrawMode == DM_BUFFERED) process_CmdBuf();
    UpdateState state = mfb_update(s_Window, window_Fb.pixels);
    shouldClose = state < 0;    
}

int winGetMouseX() {
    return s_mouseX;
}

int winGetMouseY() {
    return s_mouseY;
}

int winGetMouseButton(int button) {
    return mouseMap[button];
}

size_t window_CmdCount;

void process_CmdBuf() 
{
    window_CmdCount = 0;

    int position = 0;
    while(position < window_CmdBufSize)
    {
        Cmd* cmd = (Cmd*)(window_CmdBuf + position);

        switch (cmd->type)
        {
        case POINT_CMD:
        {
            Cmd_Point* point = (Cmd_Point*)cmd;
            process_point(*point);
            position += sizeof(Cmd_Point);   
            window_CmdCount++;
        }
            break;
        case RECT_CMD:
        {
            Cmd_Rect* rect = (Cmd_Rect*)cmd;
            process_rect(*rect);
            position += sizeof(Cmd_Rect);   
            window_CmdCount++;
        }
        break;        
        case LINE_CMD:
        {
            Cmd_Line* line = (Cmd_Line*)cmd;
            process_line(*line);
            position += sizeof(Cmd_Line);   
            window_CmdCount++;
        }
        break;            
        case BLIT_IMAGE_CMD:
        {
            Cmd_Image* image = (Cmd_Image*)cmd;
            process_image(*image);
            position += sizeof(Cmd_Image);   
            window_CmdCount++;
        }
        break;
        default:
            break;
        }
    }
    window_CmdBufSize = 0;
}

void process_point(Cmd_Point point) {
    //check if the point exists in the fb
    if(point.x < 0 || point.y < 0 || point.x >= point.texture.width || point.y >= point.texture.height || point.texture.channels < 3) 
        return;

    //get a ptr to the 'r' element of the color
    unsigned char* colPtr = &point.texture.pixels[(point.x + point.y * point.texture.width) * point.texture.channels];
    colPtr[0] = point.color.b;
    colPtr[1] = point.color.g;
    colPtr[2] = point.color.r;
    colPtr[3] = 255;
}

void process_rect(Cmd_Rect rect) {
    //check if the rect is completely off the fb
    if(((rect.x + rect.w) <= 0) || ((rect.y + rect.h) <= 0) || (rect.x >= (long)rect.texture.width) || (rect.y >= (long)rect.texture.height)) {
        return;
    }

    //clip occordingly 
    if(rect.x < 0) { rect.w = rect.x + rect.w; rect.x = 0; }
    if(rect.y < 0) { rect.h = rect.y + rect.h; rect.y = 0; }

    if(rect.x + rect.w >= rect.texture.width) { rect.w = rect.texture.width -  rect.x;  }
    if(rect.y + rect.h >= rect.texture.height){ rect.h = rect.texture.height - rect.y; }
    
    //draw each pixel in the rect
    for(int i = rect.x; i < rect.x+rect.w; i++) {
        for(int j = rect.y; j < rect.y+rect.h; j++) {
            texSetPixelFast(i, j, rect.color, rect.texture);
        }
    }
}

void process_line(Cmd_Line line) {
    double dx =  abs(line.endX-line.startX);
    double sx = line.startX<line.endX ? 1 : -1;
    double dy = -abs(line.endY-line.startY);
    double sy = line.startY<line.endY ? 1 : -1;
    double err = dx+dy;  /* error value e_xy */
    while (true) {  /* loop */
        if (line.startX==line.endX && line.startY==line.endY) break;
        double e2 = 2*err;
        if (e2 >= dy) {
            err += dy; /* e_xy+e_x > 0 */
            line.startX += sx;
        }
        if (e2 <= dx) {/* e_xy+e_y < 0 */
            err += dx;
            line.startY += sy;
        }
        texSetPixel(line.startX, line.startY, line.color, line.texture);
    }
}

void process_image(Cmd_Image image) {

    //check if the rect is completely off the fb
    int mx = image.x, my = image.y;
    int mw = image.w, mh = image.h;

    if(((image.x + image.w) <= 0) || ((image.y + image.h) <= 0) || (image.x >= (long)image.destTexture.width) || (image.y >= (long)image.destTexture.height)) {
        return;
    }

    //clip occordingly 
    if(image.x < 0) { mw = image.x + image.w; mx = 0; }
    if(image.y < 0) { mh = image.y + image.h; my = 0; }

    if(mx + image.w >= image.destTexture.width) { mw = image.destTexture.width - mx;  }
    if(my + image.h >= image.destTexture.height){ mh = image.destTexture.height - my; }
    
    //draw each pixel in the rect
    for(int i = mx; i < mx+mw; i++) {
        for(int j = my; j < my+mh; j++) {
            double tx = image.srcTexture.width * (i - image.x) / (double)image.w;
            double ty = image.srcTexture.height * (j - image.y) / (double)image.h;

            switch (image.filterMode)
            {
            case FM_NEAREST:
                texSetPixelFast(i,j, texGetPixel(round(tx), round(ty), image.srcTexture),  image.destTexture);
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
                
                Color topL = texGetPixel(x1, y2, image.srcTexture);
                Color topR = texGetPixel(x2, y2, image.srcTexture);
                double top_r = lerp(topL.r, topR.r, sx); 
                double top_g = lerp(topL.g, topR.g, sx);
                double top_b = lerp(topL.b, topR.b, sx);
                
                Color bottomL = texGetPixel(x1, y1, image.srcTexture);
                Color bottomR = texGetPixel(x2, y1, image.srcTexture);
                double bottom_r = lerp(bottomL.r, bottomR.r, sx); 
                double bottom_g = lerp(bottomL.g, bottomR.g, sx);
                double bottom_b = lerp(bottomL.b, bottomR.b, sx); 

                double out_r = lerp(bottom_r, top_r, sy); 
                double out_g = lerp(bottom_g, top_g, sy); 
                double out_b = lerp(bottom_b, top_b, sy); 
                texSetPixelFast(i,j, (Color){out_r, out_g, out_b},  image.destTexture);
            }

                break;
            default:
                break;
            }
        }
    }
}