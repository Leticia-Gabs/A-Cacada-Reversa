#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "keyboard.h"
#include "screen.h"
#include "timer.h"

static const char map[MAP_HEIGHT][MAP_WIDTH + 1] = {
    "-------------------------",
    "|        |        P     |",
    "|  ---   |  ----       |",
    "|  |     |     |       |",
    "|  |     |     |       |",
    "|  |     |     |       |",
    "|        |             |",
    "|   ----------         |",
    "|                      |",
    "|     -----            |",
    "|                      |",
    "-------------------------"
};

typedef struct {
    int x, y;
    int alive;
} Enemy;

int is_walkable(int x, int y) {
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT)
        return 0;
    char c = map[y][x];
    return (c == ' ' || c == 'P');
}

int main() {
    int player_x = 1, player_y = 1;
    Enemy enemies[2] = { {20, 1, 1}, {22, 10, 1} };
    int running = 1;
    time_t start_time = time(NULL);

    srand(time(NULL));
    init_keyboard();
    init_screen();

    while (running) {
        refresh_screen(player_x, player_y, map);

        // Desenha inimigos vivos
        for (int i = 0; i < 2; i++) {
            if (enemies[i].alive) {
                draw_enemy(enemies[i].x, enemies[i].y);
            }
        }

        // Verifica colisão jogador-inimigos (matar inimigo)
        for (int i = 0; i < 2; i++) {
            if (enemies[i].alive && enemies[i].x == player_x && enemies[i].y == player_y) {
                enemies[i].alive = 0; // Inimigo "morre"
            }
        }

        // Movimento dos inimigos
        for (int i = 0; i < 2; i++) {
            if (!enemies[i].alive) continue;
            int direction = rand() % 4;
            int new_x = enemies[i].x;
            int new_y = enemies[i].y;

            switch (direction) {
                case 0: new_y--; break;
                case 1: new_y++; break;
                case 2: new_x--; break;
                case 3: new_x++; break;
            }

            if (is_walkable(new_x, new_y) && !(new_x == player_x && new_y == player_y)) {
                enemies[i].x = new_x;
                enemies[i].y = new_y;
            }
        }

        // Movimento do jogador
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

        // Condição de vitória: jogador alcança portal
        if (map[player_y][player_x] == 'P') {
            clear_screen();
            printf("O jogador escapou pelo portal! Você venceu!\n");
            break;
        }

        // Condição de derrota: tempo esgotado
        time_t elapsed = time(NULL) - start_time;
        if (elapsed >= 120) {
            clear_screen();
            printf("Tempo esgotado! Game Over.\n");
            break;
        }

        // Se todos inimigos mortos = vitória
        int enemies_alive = 0;
        for (int i = 0; i < 2; i++) {
            if (enemies[i].alive) enemies_alive++;
        }
        if (enemies_alive == 0) {
            clear_screen();
            printf("Todos os inimigos foram pegos! Você venceu!\n");
            break;
        }

        delay_ms(100);
    }

    close_keyboard();
    return 0;
}
