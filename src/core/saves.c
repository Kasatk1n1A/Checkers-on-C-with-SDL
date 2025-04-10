#include "saves.h"

void SaveGame(CH_Type** board, GameInfo info)
{
    char save_name[100] = "saves/";
    fgets(save_name + 6, 94, stdin);
    char* ent = strchr(save_name, '\n');
    *ent = '\0';

    FILE* save_file = fopen(save_name, "w");
    if (!save_file)
    {
        fprintf(stderr, "Error with open file \"%s\"", save_name);
        return;
    }

    fprintf(save_file, "Time: %.3f sec\n", info.Time);
    fprintf(save_file, "Player: %d\n", info.player);
    fprintf(save_file, "Difficult: %d\n", info.difficult);

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

CH_Type** load_from_save(double* Total, Player* player, int* difficult)
{
    char save_name[100] = "saves/";
    fgets(save_name + 6, 94, stdin);
    char* ent = strchr(save_name, '\n');
    *ent = '\0';

    FILE* save_file = fopen(save_name, "r");
    if (!save_file)
    {
        fprintf(stderr, "Error with open file \"%s\": No such file", save_name);
        return NULL;
    }

    int tmp;
    fscanf(save_file, "Time: %lf sec\n", Total);
    fscanf(save_file, "Player: %d\n", &tmp);
    *player = tmp;
    fscanf(save_file, "Difficult: %d\n", &tmp);
    *difficult = tmp;

    fseek(save_file, 7 * sizeof(char), SEEK_CUR);

    CH_Type** board = add_board();
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
            fscanf(save_file, "%d ", &board[i][j]);

        // fseek(save_file, sizeof(char), SEEK_CUR);
    }
    fclose(save_file);

    return board;
}