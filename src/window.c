#include "../modules/minifb/include/MiniFB.h"
#include <stdlib.h>
#include <stdio.h>
#include "window.h"
#include "framebuffer.h"

FrameBuffer window_Fb;

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

    window_Fb = fbMake(width, height, 4);

    mfb_keyboard_callback(s_Window, win_kb_func);
    mfb_mouse_move_callback(s_Window, win_mouse_move_func);
    mfb_mouse_button_callback(s_Window, win_mouse_button_func);
}

void winClose() {
    fbFree(&window_Fb);
    mfb_close(s_Window);
}

void winClear(){
    fbClear(&window_Fb);
}

bool winShouldClose() {
    return shouldClose;
}

int winGetKey(int keyCode) {
    return keyMap[keyCode];
}

void winUpdate() {
    UpdateState state = mfb_update(s_Window, window_Fb.data);
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


