#include <stdio.h>
#include "screen.h"

void init_screen() {
    // Pode ficar vazio ou configurar o terminal se quiser
}

void screenClear() {
    printf("\e[1;1H\e[2J");
}

void draw_map(const char map[][MAP_WIDTH + 1]) {
    for (int i = 0; i < MAP_HEIGHT; i++) {
        printf("%s\n", map[i]);
    }
}

void draw_player(int x, int y) {
    printf("\e[%d;%dH@", y + 1, x + 1);
    fflush(stdout);
}

void draw_enemy(int x, int y) {
    printf("\e[%d;%dHX", y + 1, x + 1);
    fflush(stdout);
}

void refresh_screen(int player_x, int player_y, const char map[][MAP_WIDTH + 1]) {
    screenClear();
    draw_map(map);
    draw_player(player_x, player_y);
}
