#ifndef SAVES_H
#define SAVES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/core/Checkers_types.h"
#include "board.h"

typedef enum
{
    START_POINTS = 10000,
    EASY_POINTS = 3,
    MIDDLE_POINTS = 2,
    HARD_POINTS = 1
} HardPoints;

void SaveGame(CH_Type** board, GameInfo info);
void SaveForLeaderBoard(char* name, GameInfo info);
bool is_worse_than(const char* entry, double new_time);
CH_Type** load_from_save(double* Total, Player* player, int* difficult);

#endif