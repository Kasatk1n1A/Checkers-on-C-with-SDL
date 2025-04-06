#ifndef MOVES_H
#define MOVES_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../../include/core/ui_types.h"
#include "../../include/core/Board_types.h"
#include "../../include/core/Checkers_types.h"
#include "../ui/input.h"
#include "board.h"

void showCaptureOptions(bool** board);
bool** canCapture(CH_Type** board, bool isWhiteTurn);
bool CanContinue(CH_Type** board, int x, int y, Player player);
bool CanCheckerAttack(CH_Type** board, int x, int y, bool isWhite);
bool isCaptureMove(CH_Type** board, int x1, int y1, int x2, int y2, Player player);
void executeCaptureMove(Window* window, CH_Type** board, bool** attack_board, Player player, Board* CheckersBoard);

bool isCheckerBlocked(CH_Type** board, int x, int y);
void executeRegularMove(Window* window, CH_Type** board, Player player, Board* CheckersBoard);
bool canCheckerMove(int x1, int y1, int x2, int y2, bool isKing, Player color, CH_Type** board);

void performCapture(CH_Type** board, int fromX, int fromY, int toX, int toY);
bool CheckerCoordinates(Window* window, Board* CheckersBoard, int* x, int* y);

#endif