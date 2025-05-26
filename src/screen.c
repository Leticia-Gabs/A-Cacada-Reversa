#include "screen.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void init_screen()
{
    printf("\033[2J"); // Limpa a tela
    fflush(stdout);
}

void draw_text(int x, int y, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    printf("\033[%d;%dH", y + 1, x + 1); // Posiciona o cursor
    vprintf(format, args);
    fflush(stdout);
    va_end(args);
}

void refresh_screen(const char map[MAP_HEIGHT][MAP_WIDTH + 1])
{
    printf("\033[H"); // Move cursor para o topo da tela
    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        printf("%.*s\n", MAP_WIDTH, map[i]); // Imprime a linha inteira do mapa
    }
    fflush(stdout);
}

void draw_player(int x, int y)
{
    printf("\033[%d;%dH@", y + 1, x + 1); // Desenha o jogador '@'
    fflush(stdout);
}

void draw_enemy(int x, int y)
{
    printf("\033[%d;%dHX", y + 1, x + 1);
    fflush(stdout);
}

void draw_point(int x, int y)
{
    printf("\033[%d;%dH.", y + 1, x + 1);
    fflush(stdout);
}

void screenClear()
{
    printf("\033[2J\033[H"); // Limpa e move o cursor para (0,0)
    fflush(stdout);
}
