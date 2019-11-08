#include <unistd.h>
#include "../include/window.h"

int main(int argc, char** argv) {

    winOpen(1280, 720);
    sleep(3);
    winClose();

    return 0;
}