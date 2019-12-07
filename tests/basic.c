#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "../include/cfx.h"
#include "../src/dw_cmds.h"

extern size_t window_CmdCount;
extern DrawMode window_DrawMode;

int main(int argc, char** argv) {
    winOpen(1280, 720, "Basic Test");
    Texture tex = texLoad("./tests/sample.png");

    window_DrawMode = DM_BUFFERED;

    double timer = winGetTime();
    unsigned int frames = 0;

    while(!winShouldClose() && !winGetKey(WIN_KEY_Q)){
        winClear();
        const int SIZE = 256;
        dwBlitImage(0, fabs(sin(0.5 * winGetTime()) * 200), SIZE/2, SIZE/2, FM_BILINEAR,tex);
        dwBlitImage(SIZE/2.0, fabs(sin(0.35 * winGetTime()) * 200), SIZE, SIZE, FM_BILINEAR,tex);
        dwBlitImage(1.5*SIZE, fabs(sin(0.25 * winGetTime()) * 200), 2*SIZE, 2*SIZE, FM_BILINEAR,tex);
        char buffer[255];
        sprintf(buffer, "hello world %d\n", (int)(clock() / (double)CLOCKS_PER_SEC));
        dwDrawString(0,0, 8, buffer, (Color){55,0 ,255});
        sprintf(buffer, "%d\n", (int)window_CmdCount);
        dwDrawString(0,8 * 8 * 3, 8, buffer, (Color){55,0 ,255});

        char buffer2[255];
        sprintf(buffer2, "%d %d %d %d %d %d %d\n", winGetKey(WIN_KEY_LEFT), winGetKey(WIN_KEY_RIGHT), winGetKey(WIN_KEY_UP), winGetKey(WIN_KEY_DOWN),
            winGetMouseX(), winGetMouseY(), winGetMouseButton(1));
        dwDrawString(0,8*8, 8, buffer2, (Color){155, 255, 55});
        
        dwDrawLine(0, 64 + 64, strlen(buffer) * 64, 128, (Color){0, 255, 0});
        dwDrawLine(0, fabs(cos(0.5 * winGetTime()) * 200), strlen(buffer) * 64, fabs(sin(0.5 * winGetTime()) * 200), (Color){0,255,0});

        winUpdate();
        frames ++;
        double dt = (winGetTime()-timer);
        if(dt > 1) {
            printf("%d fps\n", (int)(frames / dt));
            frames = 0;
            timer = winGetTime();
        }
    }

    puts("closed");

    texFree(tex);
    
    winClose();

    return 0;
}
