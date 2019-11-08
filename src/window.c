#include "../modules/minifb/include/MiniFB.h"
#include <stdlib.h>
#include <stdio.h>
#include "window.h"

int keyMap[400];

static struct Window* s_Window = NULL;
static bool shouldClose = false;

static void win_kb_func(struct Window *window, Key key, KeyMod mod, bool isPressed) {
    keyMap[key] = isPressed;
}

void winOpen(int width, int height) {
    s_Window = mfb_open_ex("Test Window", width, height, 0x00);
    mfb_keyboard_callback(s_Window, win_kb_func);
}

void winClose() {
    mfb_close(s_Window);
}

bool winShouldClose() {
    return shouldClose;
}

int winGetKey(int keyCode) {
    return keyMap[keyCode];
}


void winDisplay(FrameBuffer fb) {
    UpdateState state = mfb_update(s_Window, fb.data);
    shouldClose = state < 0;
    
}

