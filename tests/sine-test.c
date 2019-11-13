#include <math.h>
#include "../include/cfx.h"

int main(int argc, char** argv) {

    winOpen(1280, 720, "Sine Test");

    int x = 0;
    while(!winGetKey(WIN_KEY_Q)){
        int y;
        y = 720/2 + (720/2) *sinf(2 * 3.14 * x/1280.0);
        dwDrawPoint(x,y, (Color){255, 0,0});

        y = 720/2 + (720/2) *cosf(2 * 3.14 * x/1280.0);
        dwDrawPoint(x,y, (Color){0, 255,0});

        y = 720/2 + (720/2) *tanf(2 * 3.14 * x/1280.0);
        dwDrawPoint(x,y, (Color){0, 0,255});

        x++;
        if(x > 1280) {
            x = 0;
            winClear();
        }
        winUpdate();
    }

    winClose();

    return 0;
}