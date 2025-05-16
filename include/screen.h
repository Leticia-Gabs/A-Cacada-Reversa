#ifndef SCREEN_H
#define SCREEN_H

#define MAP_WIDTH 25
#define MAP_HEIGHT 12

void init_screen();
void screenClear();
void draw_map(const char map[][MAP_WIDTH + 1]);
void draw_player(int x, int y);
void draw_enemy(int x, int y);
void refresh_screen(int player_x, int player_y, const char map[][MAP_WIDTH + 1]);

#endif
