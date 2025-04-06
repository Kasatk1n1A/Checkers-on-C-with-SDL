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
#include "board.h"
#include "moves.h"

int checkers(Window* window);
bool Win_Check(CH_Type** board, Player player);

#endif