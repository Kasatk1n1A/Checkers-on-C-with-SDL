#ifndef GAME_RULES_H
#define GAME_RULES_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../../include/core/ui_types.h"
#include "../../include/core/Board_types.h"
#include "../../include/core/Checkers_types.h"
#include "ai.h"
#include "board.h"
#include "moves.h"

bool Win_Check(CH_Type** board, Player player);
int checkers(Window* window, CH_Type** board, int difficult, double Time, Player player);

#endif