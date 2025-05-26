#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "keyboard.h"
#include "screen.h"
#include "timer.h"
#include "menu.h"
#include "score.h"  // NOVO: inclusão do sistema de pontuação

#define MAP_HEIGHT 12
#define MAP_WIDTH 25
#define MAX_ROUNDS 5
#define ROUND_TIME_LIMIT 120 // segundos

// Mapas diferentes para cada rodada
static const char maps[MAX_ROUNDS][MAP_HEIGHT][MAP_WIDTH + 1] = {
    {
        "-------------------------",
        "|         |             |",
        "| ---     |   --        |",
        "| |       |   |         |",
        "| |       |   |         |",
        "| |       |   |         |",
        "|           |           |",
        "|    -------            |",
        "|                       |",
        "|     -----             |",
        "|                       |",
        "-------------------------"
    },
    {
        "-------------------------",
        "|      |                |",
        "| ---- | -----          |",
        "|      |     |          |",
        "|      |     |          |",
        "|      |     |          |",
        "|            |          |",
        "|  ----------           |",
        "|                       |",
        "|     -----             |",
        "|                       |",
        "-------------------------"
    },
    {
        "-------------------------",
        "|         |             |",
        "| ---     |  --     |   |",
        "| |       |   |     --  |",
        "| |       |   |         |",
        "| |       |   |         |",
        "|                       |",
        "|    ----------         |",
        "|                       |",
        "|     -----             |",
        "|                       |",
        "-------------------------"
    },
    {
        "-------------------------",
        "|         |            |",
        "|     --               |",
        "|     |   |            |",
        "|     |   |            |",
        "|     |   |            |",
        "|         |            |",
        "|  ----------          |",
        "|                      |",
        "|     -----            |",
        "|                      |",
        "-------------------------"
    },
    {
        "-------------------------",
        "|                      |",
        "|---|   ---  |  ---    |",
        "|   |   |        |     |",
        "|   |   |        |     |",
        "|   |   |        |     |",
        "|       |              |",
        "|  ----------          |",
        "|                      |",
        "|     -----            |",
        "|                      |",
        "-------------------------"
    }
};

static char map[MAP_HEIGHT][MAP_WIDTH + 1];

typedef struct {
    int x, y;
    int alive;
} Enemy;

typedef struct {
    int x, y;
    int collected;
} Point;

int is_walkable(int x, int y) {
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT)
        return 0;
    char c = map[y][x];
    return (c == ' ');
}

int initialize_points(Point points[], int max_points) {
    int num_points = 0;
    for (int i = 0; i < MAP_HEIGHT && num_points < max_points; i++) {
        for (int j = 0; j < MAP_WIDTH && num_points < max_points; j++) {
            if (map[i][j] == ' ') {
                points[num_points].x = j;
                points[num_points].y = i;
                points[num_points].collected = 0;
                num_points++;
            }
        }
    }
    return num_points;
}

void load_map(int round_index) {
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH + 1; j++) {
            map[i][j] = maps[round_index][i][j];
        }
    }
}

int main() {
    int player_x, player_y;
    Enemy enemy;
    Point points[MAP_HEIGHT * MAP_WIDTH];
    int num_points;
    int round = 0;
    int running = 1;
    time_t round_start_time;
    char nome_jogador[50];

    srand(time(NULL));
    keyboardInit();
    init_screen();

    exibir_menu(nome_jogador, sizeof(nome_jogador));
    score_init(); // NOVO: inicia pontuação

    while (running && round < MAX_ROUNDS) {
        load_map(round);
        num_points = initialize_points(points, (MAP_HEIGHT * MAP_WIDTH) - ((round + 1) * 2));

        do {
            player_x = rand() % MAP_WIDTH;
            player_y = rand() % MAP_HEIGHT;
        } while (!is_walkable(player_x, player_y));

        enemy = (Enemy){20, 1, 1};
        round_start_time = time(NULL);
        int round_over = 0;

        while (running && !round_over) {
            refresh_screen(map);

            // Desenha status
            char status[50];
            sprintf(status, "Rodada: %d | Pontuação: %d", round + 1, score_get());
            draw_text(0, 0, status);

            // Pontos
            for (int i = 0; i < num_points; i++) {
                if (!points[i].collected) draw_point(points[i].x, points[i].y);
            }

            if (enemy.alive) {
                draw_text(enemy.x, enemy.y, "🟡");
            }

            draw_text(player_x, player_y, "👻");

            // Captura do inimigo
            if (enemy.alive && enemy.x == player_x && enemy.y == player_y) {
                enemy.alive = 0;
                score_add(10); // NOVO: pontuação por capturar
            }

            // Movimento do inimigo
            if (enemy.alive) {
                for (int attempt = 0; attempt < 4; attempt++) {
                    int dir = rand() % 4;
                    int nx = enemy.x;
                    int ny = enemy.y;

                    switch (dir) {
                        case 0: ny--; break;
                        case 1: ny++; break;
                        case 2: nx--; break;
                        case 3: nx++; break;
                    }

                    if (is_walkable(nx, ny) && !(nx == player_x && ny == player_y)) {
                        enemy.x = nx;
                        enemy.y = ny;
                        break;
                    }
                }

                for (int i = 0; i < num_points; i++) {
                    if (!points[i].collected && points[i].x == enemy.x && points[i].y == enemy.y) {
                        points[i].collected = 1;
                    }
                }
            }

            // Movimento do jogador
            if (keyhit()) {
                int key = readch();
                int nx = player_x;
                int ny = player_y;
                switch (key) {
                    case 'w': case 'W': ny--; break;
                    case 's': case 'S': ny++; break;
                    case 'a': case 'A': nx--; break;
                    case 'd': case 'D': nx++; break;
                    case 'q': case 'Q': running = 0; break;
                }
                if (is_walkable(nx, ny)) {
                    player_x = nx;
                    player_y = ny;
                }
            }

            // Verifica se todos os pontos foram coletados
            int all_collected = 1;
            for (int i = 0; i < num_points; i++) {
                if (!points[i].collected) {
                    all_collected = 0;
                    break;
                }
            }

            time_t elapsed = time(NULL) - round_start_time;
            if (elapsed >= ROUND_TIME_LIMIT) {
                keyboardDestroy();
                screenClear();
                printf("⏳ Tempo esgotado na Rodada %d! Game Over.\n", round + 1);
                printf("Pontuação final: %d\n", score_get());
                return 0;
            }

            if (all_collected) {
                keyboardDestroy();
                screenClear();
                printf("💀 Os humanos coletaram todos os pontos! Game Over.\n");
                printf("Pontuação final: %d\n", score_get());
                return 0;
            }

            if (!enemy.alive) {
                for (int i = 0; i < num_points; i++) {
                    if (!points[i].collected) score_add(1); // NOVO: pontuação por ponto não coletado
                }
                keyboardDestroy();
                screenClear();
                printf("✅ Rodada %d concluída! Inimigo capturado.\n", round + 1);
                printf("Pontuação atual: %d\n", score_get());
                delay_ms(2000);
                keyboardInit();
                init_screen();
                round++;
                round_over = 1;
            }

            delay_ms(115);
        }
    }

    keyboardDestroy();
    screenClear();
    if (round >= MAX_ROUNDS) {
        printf("🏆 Parabéns! Você completou todas as %d rodadas!\n", MAX_ROUNDS);
    }
    printf("Pontuação final: %d\n", score_get());
    return 0;
}
