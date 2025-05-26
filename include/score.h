#ifndef SCORE_H
#define SCORE_H

#define MAX_NAME_LENGTH 100
#define MAX_SCORES 100

typedef struct
{
    char name[MAX_NAME_LENGTH];
    int score;
} PlayerScore;

int score_get();
void score_add(int value);
void score_reset();

int save_score(const char *filename, const char *name, int score);
int load_scores(const char *filename, PlayerScore scores[], int max_scores);
void sort_scores(PlayerScore scores[], int count);
void print_ranking(const PlayerScore scores[], int count);

#endif
