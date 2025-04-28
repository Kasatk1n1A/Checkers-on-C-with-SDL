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

void SaveForLeaderBoard(char* name, GameInfo info)
{
    const char* filename;
    if (info.difficult == 1) filename = "saves/leaderboard/easy board.txt";
    else if (info.difficult == 3) filename = "saves/leaderboard/medium board.txt";
    else filename = "saves/leaderboard/hard board.txt";
    
    // Открываем файл для чтения (если он существует)
    FILE* file = fopen(filename, "r");
    if (!file) {
        // Если файла нет, создаём новый
        file = fopen(filename, "w");
        if (!file) {
            perror("Failed to create leaderboard file");
            return;
        }
        fprintf(file, "Name: %s Time: %f\n", name, info.Time);
        fclose(file);
        return;
    }

    // Читаем существующие записи
    char** entries = (char**)malloc(20 * sizeof(char*));
    int entry_count = 0;
    
    for (int i = 0; i < 20; i++) {
        entries[i] = (char*)malloc(100 * sizeof(char));
        if (fgets(entries[i], 100, file)) {
            entries[i][strlen(entries[i]) - 1] = '\0';
            entry_count++;
        } else
            entries[i][0] = '\0'; // Помечаем как пустую
    }
    fclose(file);

    // Формируем новую запись
    char new_entry[100];
    snprintf(new_entry, sizeof(new_entry), "Name: %s Time: %f", name, info.Time);

    // Вставляем новую запись в правильную позицию
    bool inserted = false;
    file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open leaderboard for writing");
        goto cleanup;
    }

    for (int i = 0; i < 20; i++) {
        // Если текущая запись хуже (больше времени) или пустая, вставляем новую
        if (!inserted && (i >= entry_count || is_worse_than(entries[i], info.Time))) {
            fprintf(file, "%s\n", new_entry);
            inserted = true;
        }

        // Записываем текущую запись, если она не пустая и не все записи уже записаны
        if (i < entry_count && entries[i][0] != '\0') {
            fprintf(file, "%s\n", entries[i]); // fgets сохраняет \n
        }
    }

    // Если не вставили (все записи лучше), добавляем в конец
    if (!inserted && entry_count < 20) {
        fprintf(file, "%s\n", new_entry);
    }

    fclose(file);

cleanup:
    for (int i = 0; i < 20; i++) {
        free(entries[i]);
    }
    free(entries);
}

// Вспомогательная функция для сравнения записей
bool is_worse_than(const char* entry, double new_time) 
{
    double entry_time;
    char tmp_name[100];
    if (sscanf(entry, "Name: %99s Time: %lf", tmp_name, &entry_time) == 2) {
        return entry_time > new_time;
    }
    return true; // Если запись некорректна, считаем её "худшей"
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