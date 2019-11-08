#include "../modules/minifb/include/MiniFB.h"
#include <stdlib.h>
#include <stdio.h>
#include "window.h"

static struct Window* s_Window = NULL;

void winOpen(int width, int height) {
    s_Window = mfb_open("Test Window", width, height);
}

void winClose() {
    mfb_close(s_Window);
}

