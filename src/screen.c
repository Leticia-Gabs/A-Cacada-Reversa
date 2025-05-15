#include <stdio.h>
#include <stdlib.h>
#include "screen.h"

#define MAP_WIDTH 20
#define MAP_HEIGHT 10

static const char map[MAP_HEIGHT][MAP_WIDTH + 1] = {
    "--------------------",
    "|        |         |",
    "|  ---   |  ----   |",
    "|  |     |     |   |",
    "|  |     |     |   |",
    "|  |     |     |   |",
    "|        |         |",
    "|   ----------     |",
    "|                  |",
    "--------------------"
};

void init_screen() {
}

void clear_screen() {
    printf("\e[1;1H\e[2J");
}

void draw_map() {
    for (int i = 0; i < MAP_HEIGHT; i++) {
        printf("%s\n", map[i]);
    }
}

void draw_player(int x, int y) {
    printf("\e[%d;%dH@", y + 1, x + 1);
    fflush(stdout);
}

void refresh_screen() {
    clear_screen();
    draw_map();
}
