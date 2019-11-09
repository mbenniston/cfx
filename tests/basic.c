#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "../include/cfx.h"

int main(int argc, char** argv) {
    FrameBuffer fb = fbMake(1280, 720, 4);

    winOpen(1280, 720);
    Texture tex = texLoad("./tests/sample.png");

    while(!winShouldClose() && !winGetKey(WIN_KEY_Q)){
        fbClear(&fb);
        dwBlitImage(0, fabs(sin(0.5 * clock() / (double)CLOCKS_PER_SEC) * 200), 128, 128, tex, fb);
        char buffer[255];
        sprintf(buffer, "hello world %d\n", (int)(clock() / (double)CLOCKS_PER_SEC));
        textDrawString(0,0, 8, buffer, (Color){55,0 ,255}, fb);

        char buffer2[255];
        sprintf(buffer2, "%d %d %d %d %d %d\n", winGetKey(WIN_KEY_LEFT), winGetKey(WIN_KEY_RIGHT), winGetKey(WIN_KEY_UP), winGetKey(WIN_KEY_DOWN),
            winGetMouseX(), winGetMouseY());
        textDrawString(0,8*8, 8, buffer2, (Color){155,255 ,55}, fb);
        dwDrawLine(0, 64 + 64, strlen(buffer) * 64, 128, (Color){0,255,0}, fb);
        
        dwDrawLine(0, fabs(cos(0.5 * clock() / (double)CLOCKS_PER_SEC) * 200), strlen(buffer) * 64, fabs(sin(0.5 * clock() / (double)CLOCKS_PER_SEC) * 200), (Color){0,255,0}, fb);
        dwBlitImage(winGetMouseX(), winGetMouseY(),128, 53, tex, fb);

        winDisplay(fb);
    }

    puts("closed");


    winClose();

    texFree(tex);
    fbFree(&fb);

    return 0;
}