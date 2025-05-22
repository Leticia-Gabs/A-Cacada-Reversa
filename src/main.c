#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "keyboard.h"
#include "screen.h"
#include "timer.h"

#define MAP_HEIGHT 12
#define MAP_WIDTH 25
#define NUM_ENEMIES 2
#define MAX_ROUNDS 5
#define ROUND_TIME_LIMIT 120 // segundos

// Mapas diferentes para cada rodada
static const char maps[MAX_ROUNDS][MAP_HEIGHT][MAP_WIDTH + 1] = {
    { // Mapa da Rodada 1
        "-------------------------",
        "|         |         P   |",
        "| ---     | ----        |",
        "| |       |   |         |",
        "| |       |   |         |",
        "| |       |   |         |",
        "|           |           |",
        "|    ----------         |",
        "|                       |",
        "|     -----             |",
        "|                       |",
        "-------------------------"
    },
    { // Mapa da Rodada 2
        "-------------------------",
        "|P     |                |",
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
    { // Mapa da Rodada 3
        "-------------------------",
        "|         |             |",
        "| ---     | ----    | P |",
        "| |       |   |    ----|",
        "| |       |   |        |",
        "| |       |   |        |",
        "|           |          |",
        "|    ----------         |",
        "|                       |",
        "|     -----             |",
        "|                       |",
        "-------------------------"
    },
    { // Mapa da Rodada 4
        "-------------------------",
        "|         |         P   |",
        "|     -----            |",
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
    { // Mapa da Rodada 5
        "-------------------------",
        "| P |        |         |",
        "|---|   ---- | ----    |",
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
    return (c == ' ' || c == 'P');
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
    Enemy enemies[NUM_ENEMIES];
    Point points[MAP_HEIGHT * MAP_WIDTH];
    int num_points;
    int round = 0;
    int score = 0;
    int running = 1;
    time_t round_start_time;

    srand(time(NULL));
    keyboardInit();
    init_screen();

    while (running && round < MAX_ROUNDS) {
        load_map(round);
        num_points = initialize_points(points, (MAP_HEIGHT * MAP_WIDTH) - ((round + 1) * 2));

        do {
            player_x = rand() % MAP_WIDTH;
            player_y = rand() % MAP_HEIGHT;
        } while (!is_walkable(player_x, player_y));

        enemies[0] = (Enemy){20, 1, 1};
        enemies[1] = (Enemy){22, 10, 1};

        round_start_time = time(NULL);
        int round_over = 0;

        while (running && !round_over) {
            refresh_screen(player_x, player_y, map);
            draw_text(0, 0, "Rodada: %d  Pontuacao: %d", round + 1, score);

            for (int i = 0; i < num_points; i++) {
                if (!points[i].collected) draw_point(points[i].x, points[i].y);
            }

            for (int i = 0; i < NUM_ENEMIES; i++) {
                if (enemies[i].alive) draw_enemy(enemies[i].x, enemies[i].y);
            }

            for (int i = 0; i < NUM_ENEMIES; i++) {
                if (enemies[i].alive && enemies[i].x == player_x && enemies[i].y == player_y) {
                    enemies[i].alive = 0;
                    score += 10;
                }
            }

            for (int i = 0; i < num_points; i++) {
                if (!points[i].collected && points[i].x == player_x && points[i].y == player_y) {
                    points[i].collected = 1;
                }
            }

            int enemy_speed_delay = 150 - (round * 20);
            if (enemy_speed_delay < 50) enemy_speed_delay = 50;

            for (int i = 0; i < NUM_ENEMIES; i++) {
                if (!enemies[i].alive) continue;
                if (rand() % enemy_speed_delay == 0) {
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
            }

            if (keyhit()) {
                int key = readch();
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

            int all_enemies_captured = 1;
            for (int i = 0; i < NUM_ENEMIES; i++) {
                if (enemies[i].alive) {
                    all_enemies_captured = 0;
                    break;
                }
            }

            int all_points_collected = 1;
            for (int i = 0; i < num_points; i++) {
                if (!points[i].collected) {
                    all_points_collected = 0;
                    break;
                }
            }

            time_t round_elapsed = time(NULL) - round_start_time;
            if (round_elapsed >= ROUND_TIME_LIMIT) {
                screenClear();
                printf("Tempo esgotado na Rodada %d! Game Over.\n", round + 1);
                running = 0;
                break;
            }

            if (all_points_collected) {
                screenClear();
                printf("Os humanos coletaram todos os pontos na Rodada %d! Game Over.\n", round + 1);
                running = 0;
                break;
            }

            if (all_enemies_captured) {
                for (int i = 0; i < num_points; i++) {
                    if (!points[i].collected) score += 1;
                }
                screenClear();
                printf("Rodada %d concluida! Todos os invasores foram capturados.\n", round + 1);
                printf("Pontuacao atual: %d\n", score);
                round++;
                round_over = 1;
                delay_ms(2000);
            }

            delay_ms(100);
        }
    }

    screenClear();
    if (round >= MAX_ROUNDS) {
        printf("Parabens! Voce completou todas as %d rodadas!\n", MAX_ROUNDS);
    }
    printf("Pontuacao final: %d\n", score);

    keyboardDestroy();
    return 0;
}