#include "draw.h"
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "dw_cmds.h"
#include "cfx_memtest.h"

extern Texture window_Fb;
extern DrawMode window_DrawMode;

void dwDrawPointToTexture(int x, int y, Color col, Texture dest)
{
    Cmd_Point pointCmd;
    pointCmd.type = POINT_CMD;
    pointCmd.x = x;
    pointCmd.y = y;
    pointCmd.color = col;
    pointCmd.texture = dest;

    if(window_DrawMode == DM_BUFFERED){
        pushCmd((Cmd*)&pointCmd);
    } else {
        process_point(pointCmd);
    }
}

void dwDrawRectToTexture(int x, int y, int w, int h, Color col, Texture dest)
{
    Cmd_Rect cmd;
    cmd.type = RECT_CMD;
    cmd.x = x;
    cmd.y = y;
    cmd.w = w;
    cmd.h = h;
    cmd.color = col;
    cmd.texture = dest;

    if(window_DrawMode == DM_BUFFERED){
        pushCmd((Cmd*)&cmd);
    } else {
        process_rect(cmd);
    }
}

void dwBlitImageToTexture(int x, int y, int w, int h, FilterMode filterMode,  Texture tex, Texture dest)
{
    Cmd_Image cmd;
    cmd.type = BLIT_IMAGE_CMD;
    cmd.filterMode = filterMode;
    cmd.x = x;
    cmd.y = y;
    cmd.w = w;
    cmd.h = h;
    cmd.srcTexture = tex;
    cmd.destTexture = dest;

    if(window_DrawMode == DM_BUFFERED){
        pushCmd((Cmd*)&cmd);
    } else {
        process_image(cmd);
    }
}

void dwDrawLineToTexture(int startX, int startY, int endX, int endY, Color col, Texture dest)
{
    Cmd_Line cmd;
    cmd.type = LINE_CMD;
    cmd.startX = startX;
    cmd.startY = startY;
    cmd.endX = endX;
    cmd.endY = endY;
    cmd.color = col;
    cmd.texture = dest;

    if(window_DrawMode == DM_BUFFERED){
        pushCmd((Cmd*)&cmd);
    } else {
        process_line(cmd);
    }
}

void dwDrawPoint(int x, int y, Color col)
{
    dwDrawPointToTexture(x,y,col, window_Fb);
}

void dwDrawRect(int x, int y, int w, int h, Color col)
{
    dwDrawRectToTexture(x, y, w,h, col, window_Fb);
}

void dwBlitImage(int x, int y, int w, int h, FilterMode filterMode,  Texture tex)
{
    dwBlitImageToTexture(x, y, w, h,filterMode, tex, window_Fb);
}

void dwDrawLine(int startX, int startY, int endX, int endY, Color col)
{
    dwDrawLineToTexture(startX, startY, endX, endY, col, window_Fb);
}

void dwDrawCharToTexture(int x, int y, int size, char c, Color col, Texture dest)
{
    Cmd_Char cmd;
    cmd.type = CHAR_CMD;
    cmd.x = x;
    cmd.y = y;
    cmd.w = size;
    cmd.h = size;
    cmd.color = col;
    cmd.character = c;
    cmd.destTexture = dest;

    if(window_DrawMode == DM_BUFFERED){
        pushCmd((Cmd*)&cmd);
    } else {
        process_char(cmd);
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

