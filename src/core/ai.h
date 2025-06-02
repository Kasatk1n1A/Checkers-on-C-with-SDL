// bot.h
#ifndef AI_H
#define AI_H

#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include "../../include/core/Checkers_types.h"
#include "../../include/core/Board_types.h"
#include "board.h"
#include "moves.h"
#include "game_rules.h"
#include "Transpositions.h"

#define CASH_MAX_SIZE 100

// Основная функция для хода бота
void bot_make_move(CH_Type** board, int difficult, Player player);

// Вспомогательные функции
Move* Move_Copy(const Move* src);
int evaluate_position(CH_Type** board, Player player);
Move* generate_all_moves(CH_Type** board, Player player);
Move* find_best_move(CH_Type** board, Player player, int maxDepth);
Move* create_move(int fromX, int fromY, int toX, int toY, Move* captures);
int minimax(CH_Type** board, CH_Type** tmp_board, int depth, int alpha, int beta, bool isMaximizing, Player player, int maxDepth);

#endif