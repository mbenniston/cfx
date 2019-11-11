#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "../include/cfx.h"

extern size_t window_CmdCount;

int main(int argc, char** argv) {
    winOpen(1280, 720);
    Texture tex = texLoad("./tests/sample.png");

    clock_t timer = clock();
    unsigned int frames = 0;

    while(!winShouldClose() && !winGetKey(WIN_KEY_Q)){
        winClear();
        dwBlitImage(0, fabs(sin(0.5 * clock() / (double)CLOCKS_PER_SEC) * 200), 128, 128, FM_NEAREST,tex);
        dwBlitImage(128, fabs(sin(0.35 * clock() / (double)CLOCKS_PER_SEC) * 200), 128, 128, FM_NEAREST,tex);
        dwBlitImage(256, fabs(sin(0.25 * clock() / (double)CLOCKS_PER_SEC) * 200), 128, 128, FM_NEAREST,tex);
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
        
        dwDrawLine(0, fabs(cos(0.5 * clock() / (double)CLOCKS_PER_SEC) * 200), strlen(buffer) * 64, fabs(sin(0.5 * clock() / (double)CLOCKS_PER_SEC) * 200), (Color){0,255,0});

        winUpdate();
        frames ++;
        if((clock()-timer) / (double)CLOCKS_PER_SEC > 1) {
            printf("%d fps\n", frames);
            frames = 0;
            timer = clock();
        }
    }

    puts("closed");


    winClose();

    texFree(tex);

    return 0;
}
