#include "saves.h"

void SaveGame(CH_Type** board, double Total)
{
    char save_name[100] = "saves/";
    fgets(save_name + 6, 94, stdin);
    char* ent = strchr(save_name, '\n');
    *ent = '\0';

    FILE* save_file = fopen(save_name, "w");

    fprintf(save_file, "Time: %.3f sec\n", Total);

    fprintf(save_file, "Board:\n");
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            fprintf(save_file, "%d ", board[i][j]);
        }
        fprintf(save_file, "\n");
    }
    fclose(save_file);
}

void load_from_save(CH_Type** board, double* Total)
{
    char save_name[100] = "saves/";
    fgets(save_name + 6, 94, stdin);
    char* ent = strchr(save_name, '\n');
    *ent = '\0';

    FILE* save_file = fopen(save_name, "r");

    fscanf(save_file, "Time: %lf sec\n", Total);

    fseek(save_file, 7 * sizeof(char), SEEK_CUR);

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
            fscanf(save_file, "%d ", &board[i][j]);

        fseek(save_file, sizeof(char), SEEK_CUR);
    }
    fclose(save_file);
}