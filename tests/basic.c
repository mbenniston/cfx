#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include "../include/cfx.h"

int main(int argc, char** argv) {
    FrameBuffer fb = fbMake(1280, 720, 4);

    winOpen(1280, 720);
    
    while(!winShouldClose() && !winGetKey(WIN_KEY_Q)){
        fbClear(&fb);
        dwDrawRect(sin(0.5 * clock() / (double)CLOCKS_PER_SEC) * 2000, 0, 100, 100, (Color){255,255,0}, fb);
        winDisplay(fb);
    }

    puts("closed");

    winClose();

    fbFree(&fb);

    return 0;
}