#ifndef SAVES_H
#define SAVES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game_rules.h"
#include "../../include/core/Checkers_types.h"
#include "board.h"

typedef enum
{
    START_POINTS = 10000,
    EASY_POINTS = 3,
    MIDDLE_POINTS = 2,
    HARD_POINTS = 1
} HardPoints;

typedef struct 
{
    SDL_Texture* Name;
    SDL_Rect rect;
    bool hovered;
} save;

int count_paragraphs(void);
void free_saves(save* saves);
char* read_save(int name_count);
void load_from_save(Window* window, char* str);
int SaveForLeaderBoard(char* name, GameInfo info);
save* read_saves(SDL_Renderer* renderer, int page);
bool is_worse_than(const char* entry, double new_time);
void SaveGame_file(char* str, CH_Type** board, GameInfo info);
int delete_line_from_file(const char *filename, int line_to_delete);

#endif