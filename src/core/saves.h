#ifndef SAVES_H
#define SAVES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/core/Checkers_types.h"
#include "board.h"

void SaveGame(CH_Type** board, GameInfo info);
CH_Type** load_from_save(double* Total, Player* player, int* difficult);

#endif