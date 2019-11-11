#include "../modules/minifb/include/MiniFB.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "window.h"
#include "texture.h"
#include "dw_cmds.h"

void* window_CmdBuf;
size_t window_CmdBufSize, window_CmdBufMaxSize;

Texture window_Fb;

int keyMap[400];
int mouseMap[12];

static struct Window* s_Window = NULL;
static bool shouldClose = false;
static int s_mouseX = 0, s_mouseY = 0, s_WindowWidth = 0, s_WindowHeight = 0;

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
    s_Window = mfb_open_ex("Test Window", width, height, 0x00);

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
    mfb_close(s_Window);
}

void winClear(){
    texClear(window_Fb);
}

bool winShouldClose() {
    return shouldClose;
}

int winGetKey(int keyCode) {
    return keyMap[keyCode];
}

void winUpdate() {
    process_CmdBuf();
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

void process_point(Cmd_Point rect);
void process_rect(Cmd_Rect rect);

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
            texSetPixel(i, j, rect.color, rect.texture);
        }
    }
}

void process_line(Cmd_Line line) {
    int stepX = line.endX - line.startX;
    int stepY = line.endY - line.startY;
    for(int x = line.startX; x < line.endX; x++){
        int y = line.startY + stepY * (x - line.startX) / (double)stepX;
        texSetPixel(x, y, line.color, line.texture);
    }
}