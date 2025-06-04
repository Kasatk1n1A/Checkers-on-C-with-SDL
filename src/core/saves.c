#include "saves.h"

void SaveGame_file(char* str, CH_Type** board, GameInfo info)
{
    char save_name[100] = "saves/";
    strcat(save_name, str);
    char* path = GetExecutableRelativePath(save_name);

    FILE* save_file = fopen(path, "w");
    if (!save_file)
    {
        fprintf(stderr, "Error with open file \"%s\"", save_name);
        return;
    }
    free(path);

    fprintf(save_file, "Save name: %s\n", str);
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

    path = GetExecutableRelativePath("saves/saves_name.txt");
    FILE* sigma = fopen(path, "a");
    str[strlen(str)] = '\n';
    fputs(str, sigma);
    fclose(sigma);
    free(path);
}

int SaveForLeaderBoard(char* name, GameInfo info)
{
    char* filename;
    if (info.difficult == 1) filename = GetExecutableRelativePath("saves/leaderboard/easy board.txt");
    else if (info.difficult == 3) filename = GetExecutableRelativePath("saves/leaderboard/medium board.txt");
    else filename = GetExecutableRelativePath("saves/leaderboard/hard board.txt");
    
    // Открываем файл для чтения (если он существует)
    FILE* file = fopen(filename, "r");
    if (!file) {
        // Если файла нет, создаём новый
        file = fopen(filename, "w");
        if (!file) {
            perror("Failed to create leaderboard file");
            return 1;
        }
        fprintf(file, "Name: %s Time: %f\n", name, info.Time);
        fclose(file);
        return 1;
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
        fprintf(stderr, "Failed to open leaderboard for writing");
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

    free(filename);
    fclose(file);

cleanup:
    for (int i = 0; i < 20; i++)
        free(entries[i]);

    free(entries);
    return 1;
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

void load_from_save(Window* window, char* str)
{
    char save_name[100] = "saves/";
    strcat(save_name, str);

    char* path = GetExecutableRelativePath(save_name);
    FILE* save_file = fopen(path, "r");
    if (!save_file)
    {
        fprintf(stderr, "Error with open file \"%s\": No such file", save_name);
        return;
    }

    char tmp[20];
    double Total;
    Player player;
    int difficult;
    fscanf(save_file, "Save name: %s\n", tmp);
    fscanf(save_file, "Time: %lf sec\n", &Total);
    fscanf(save_file, "Player: %d\n", &player);
    fscanf(save_file, "Difficult: %d\n", &difficult);

    fseek(save_file, 7 * sizeof(char), SEEK_CUR);

    CH_Type** board = add_board();
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            fscanf(save_file, "%d ", &board[i][j]);

    fclose(save_file);
    remove(save_name);
    free(path);
    free(str);   

    checkers(window, board, difficult, Total, player);
}

int delete_line_from_file(const char *filename, int line_to_delete) 
{
    char* path1 = GetExecutableRelativePath(filename);
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file for reading");
        return -1;
    }
    free(path1);

    // Создаем временный файл
    // char* path2 = GetExecutableRelativePath()
    FILE *temp_file = fopen("temp.txt", "w");
    if (temp_file == NULL) {
        perror("Failed to create temporary file");
        fclose(file);
        return -1;
    }

    char buffer[20];
    int current_line = 1;
    int line_found = 0;

    // Читаем исходный файл построчно
    while (fgets(buffer, 20, file) != NULL) {
        // Пропускаем строку, которую нужно удалить
        if (current_line != line_to_delete) {
            fputs(buffer, temp_file);
        } else {
            line_found = 1;
        }
        current_line++;
    }

    fclose(file);
    fclose(temp_file);

    // Если строка не найдена
    if (!line_found && line_to_delete > 0) {
        remove("temp.txt");
        return 0;
    }

    // Заменяем оригинальный файл временным
    if (remove(filename) != 0) {
        fprintf(stderr, "Failed to remove original file");
        return -1;
    }

    if (rename("temp.txt", filename) != 0) {
        fprintf(stderr, "Failed to rename temporary file");
        return -1;
    }

    return 1;
}

save* read_saves(SDL_Renderer* renderer, int page)
{
    char* path1 = GetExecutableRelativePath("saves/saves_name.txt");
    char* path2 = GetExecutableRelativePath("assets/fonts/freesansbold.ttf");
    FILE* file = fopen(path1, "r");
    TTF_Font* font = TTF_OpenFont(path2, 30);
    SDL_Color Black = {0, 0, 0, 255};

    save* saves_names = (save*)malloc(sizeof(save) * 21);
    for (int i = 0; i < 21; i++)
    {
        saves_names[i].hovered = false;
        saves_names[i].Name = NULL;
    }
    
    char* Name = (char*)malloc(sizeof(char) * 20);
    memset(Name, 0, sizeof(char) * 20);
    for (int i = 0; !feof(file) && i < (page - 1) * 20; i++)
    {
        fgets(Name, 20, file);
        memset(Name, 0, sizeof(char) * 20);
    }

    {//генерация текста о том какая страница из списка
        char str_page[10];
        sprintf(str_page, "page %d", page);
        SDL_Surface* Name_surface = TTF_RenderText_Blended(font, str_page, Black);
        saves_names[0].Name = SDL_CreateTextureFromSurface(renderer, Name_surface);
        saves_names[0].rect.x = 648; saves_names[0].rect.y = 100; 
        saves_names[0].rect.w = Name_surface->w; saves_names[0].rect.h = Name_surface->h;
        
        SDL_FreeSurface(Name_surface);
    }

    for (int i = 1; i < 21 && !feof(file); i++)
    {
        fscanf(file, "%s\n", Name);

        if (strlen(Name) == 0)
            break;
        SDL_Surface* Name_surface = TTF_RenderText_Blended(font, Name, Black);
        saves_names[i].Name = SDL_CreateTextureFromSurface(renderer, Name_surface);
        saves_names[i].rect.x = 575; saves_names[i].rect.y = 100 + i * 30; saves_names[i].rect.w = Name_surface->w; saves_names[i].rect.h = Name_surface->h;
        
        SDL_FreeSurface(Name_surface);
    }
    
    free(path1);
    free(path2);
    TTF_CloseFont(font);
    fclose(file);
    free(Name);

    return saves_names;
}

int count_paragraphs(void)
{
    char* path = GetExecutableRelativePath("saves/saves_name.txt");
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        perror("Failed to open file");
        return -1;
    }

    int paragraph_count = 0;
    int empty_line_count = 0;
    int prev_char = '\0';
    int current_char;

    while ((current_char = fgetc(file)) != EOF) {
        if (current_char == '\n') {
            // Для Windows-style переносов (\r\n)
            if (prev_char == '\r') {
                empty_line_count++;
            } else {
                // Для Unix-style переносов (\n)
                empty_line_count++;
            }
        } else if (current_char != '\r' && current_char != '\n') {
            // Если после пустых строк идет не пустая строка
            if (empty_line_count >= 1) {
                paragraph_count++;
                empty_line_count = 0;
            }
        }

        prev_char = current_char;
    }

    // Последний абзац (если файл не заканчивается пустыми строками)
    if (prev_char != '\n' && prev_char != '\r') {
        paragraph_count++;
    }

    free(path);
    fclose(file);
    printf("%d\n", paragraph_count);
    return paragraph_count; // Минимум 1 абзац
}

char* read_save(int name_count)
{
    char* Name = (char*)malloc(sizeof(char) * 20);
    char* path = GetExecutableRelativePath("saves/saves_name.txt");
    FILE* file = fopen(path, "r");

    for (int i = 0; i < name_count; i++)
        fgets(Name, 20, file);
    Name[strlen(Name) - 1] = '\0';
        
    fclose(file);
    free(path);
    
    delete_line_from_file("saves/saves_name.txt", name_count);

    return Name;
}

void print_saves(SDL_Renderer* renderer, save* saves)
{
    for (int i = 0; i < 21 && saves[i].Name; i++)
    {
        SDL_RenderCopy(renderer, saves[i].Name, NULL, &saves[i].rect);
    }
}

void free_saves(save* saves)
{
    if (!saves)
        return;

    for (int i = 0; i < 21; i++)
        if (saves[i].Name)
            SDL_DestroyTexture(saves[i].Name);
    free(saves);
}