#include <stdio.h>
#include "keyboard.h"
#include "screen.h"
#include "timer.h"

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

int is_walkable(int x, int y) {
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT)
        return 0;
    return map[y][x] == ' ';
}

int main() {
    int player_x = 1, player_y = 1;
    int running = 1;

    init_keyboard();
    init_screen();

    while (running) {
        refresh_screen();
        draw_player(player_x, player_y);

        if (keyboard_hit()) {
            int key = get_key();
            int new_x = player_x;
            int new_y = player_y;

            switch (key) {
                case 'w': case 'W': new_y--; break;
                case 's': case 'S': new_y++; break;
                case 'a': case 'A': new_x--; break;
                case 'd': case 'D': new_x++; break;
                case 'q': case 'Q': running = 0; break;
            }

            if (is_walkable(new_x, new_y)) {
                player_x = new_x;
                player_y = new_y;
            }
        }
        delay_ms(50);
    }

    close_keyboard();
    clear_screen();
    printf("Jogo encerrado. Obrigado por jogar!\n");
    return 0;
}
