#ifndef SCREEN_H
#define SCREEN_H

#define MAP_HEIGHT 12
#define MAP_WIDTH 25

void init_screen();
void draw_text(int x, int y, const char *format, ...);
void refresh_screen(const char map[MAP_HEIGHT][MAP_WIDTH + 1]);
void draw_player(int x, int y);
void draw_enemy(int x, int y);
void draw_point(int x, int y);
void screenClear();

#endif
