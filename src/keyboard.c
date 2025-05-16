#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include "keyboard.h"

static struct termios oldt, newt;

void keyboardInit() {
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
}

void keyboardDestroy() {
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

int keyhit() {
    int ch = getchar();
    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

int readch() {
    return getchar();
}
