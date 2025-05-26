#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "score.h"

int load_scores(const char *filename, PlayerScore scores[], int max_scores)
{
    FILE *file = fopen(filename, "r");
    if (!file)
        return 0;

    int count = 0;
    while (count < max_scores && fscanf(file, "%49[^,],%d\n", scores[count].name, &scores[count].score) == 2)
    {
        count++;
    }
    fclose(file);
    return count;
}

int save_score(const char *filename, const char *name, int score)
{
    FILE *file = fopen(filename, "a"); // append
    if (!file)
        return 0;

    fprintf(file, "%s,%d\n", name, score);
    fclose(file);
    return 1;
}

// Ordena scores do maior para o menor
void sort_scores(PlayerScore scores[], int count)
{
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            if (scores[j].score > scores[i].score)
            {
                PlayerScore temp = scores[i];
                scores[i] = scores[j];
                scores[j] = temp;
            }
        }
    }
}

// Imprime ranking formatado no console
void print_ranking(const PlayerScore scores[], int count)
{
    printf("===== Ranking =====\n");
    printf("%-4s %-20s %s\n", "Pos", "Nome", "Pontuação");
    for (int i = 0; i < count && i < 10; i++)
    { // mostra só top 10
        printf("%-4d %-20s %d\n", i + 1, scores[i].name, scores[i].score);
    }
    printf("===================\n");
}
