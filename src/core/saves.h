#ifndef SAVES_H
#define SAVES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/core/Checkers_types.h"

void SaveGame(CH_Type** board, double Total);
void load_from_save(CH_Type** board, double* Total);

#endif