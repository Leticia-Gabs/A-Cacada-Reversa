#include "screen.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void init_screen() {
    printf("\033[2J"); // Limpa a tela
    fflush(stdout);
}

void draw_text(int x, int y, const char *format, ...) {
    va_list args;
    va_start(args, format);
    printf("\033[%d;%dH", y + 1, x + 1); // Posiciona o cursor
    vprintf(format, args);             // Imprime o texto formatado
    fflush(stdout);
    va_end(args);
}

void refresh_screen(int player_x, int player_y, const char map[MAP_HEIGHT][MAP_WIDTH + 1]) {
    printf("\033[H"); // Move cursor para o topo da tela
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            if (j == player_x && i == player_y) {
                printf("@"); // Jogador
            } else {
                printf("%c", map[i][j]);
            }
        }
        printf("\n");
    }
    fflush(stdout);
}

void draw_enemy(int x, int y) {
    printf("\033[%d;%dH", y + 1, x + 1); // Posiciona no terminal
    printf("X");
    fflush(stdout);
}

void draw_point(int x, int y) {
    printf("\033[%d;%dH", y + 1, x + 1);
    printf(".");
    fflush(stdout);
}

void screenClear() {
    printf("\033[2J\033[H");
    fflush(stdout);
}