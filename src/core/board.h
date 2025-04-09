#ifndef BOARD_H
#define BOARD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../../include/core/ui_types.h"
#include "../../include/core/Board_types.h"
#include "../../include/core/Checkers_types.h"

CH_Type** add_board(void);
void freeBoard(void** board);
void out_board(CH_Type** board);

void out_board_SDL(Window* window, Board* CheckersBoard);
void renderBoardFrame(Window* window, Board* CheckersBoard);

void board_cleanup_SDL(Board* CheckersBoard);
bool LoadBoardTextures(Board* CheckersBoard, Window* window);

void highlightChecker(CH_Type** board, int x1, int y1);
void unhighlightChecker(CH_Type** board, int x2, int y2);

#endif