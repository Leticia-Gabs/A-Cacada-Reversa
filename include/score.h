#ifndef SCORE_H
#define SCORE_H

#define MAX_NAME_LENGTH 50
#define MAX_SCORES 100

typedef struct {
    char name[MAX_NAME_LENGTH];
    int score;
} PlayerScore;

int load_scores(const char *filename, PlayerScore scores[], int max_scores);
int save_score(const char *filename, const char *name, int score);
void sort_scores(PlayerScore scores[], int count);
void print_ranking(const PlayerScore scores[], int count);

#endif 
