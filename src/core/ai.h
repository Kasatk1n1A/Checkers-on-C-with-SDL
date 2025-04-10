// bot.h
#ifndef AI_H
#define AI_H

#include "../../include/core/Checkers_types.h"
#include "../../include/core/Board_types.h"
#include "board.h"
#include "moves.h"

typedef struct Move {
    int fromX, fromY;    // Координаты начальной позиции (откуда)
    int toX, toY;        // Координаты конечной позиции (куда)
    struct Move* next;    // Указатель на следующий ход (для списка)
    struct Move* captures; // Список съеденных фигур (для взятий)
} Move;

// Основная функция для хода бота
void bot_make_move(CH_Type** board, int difficult, Player player);

// Вспомогательные функции
int evaluate_position(CH_Type** board, Player player);
int minimax(CH_Type** board, int depth, int alpha, int beta, bool isMaximizing, Player player, int maxDepth);
Move* find_best_move(CH_Type** board, Player player, int maxDepth);
Move* generate_all_moves(CH_Type** board, Player player);
void free_move(Move* move);

#endif