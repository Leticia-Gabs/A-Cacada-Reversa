#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include "keyboard.h"

static struct termios oldt, newt;

void init_keyboard() {
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
}

void close_keyboard() {
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

int keyboard_hit() {
    int ch = getchar();
    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

int get_key() {
    return getchar();
}
