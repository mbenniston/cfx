#include "../modules/minifb/include/MiniFB.h"
#include <stdlib.h>
#include <stdio.h>
#include "window.h"

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

    mfb_keyboard_callback(s_Window, win_kb_func);
    mfb_mouse_move_callback(s_Window, win_mouse_move_func);
    mfb_mouse_button_callback(s_Window, win_mouse_button_func);
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
    if(fb.width != s_WindowWidth || fb.height != s_WindowHeight || fb.channels != 4) {
        printf("Framebuffer not compatible for displaying on screen\n");
        winClose();
        exit(1);
    }

    UpdateState state = mfb_update(s_Window, fb.data);
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


