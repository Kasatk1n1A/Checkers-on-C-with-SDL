// // bot.h
// #ifndef AI_H
// #define AI_H

// #include "StructsAndEnum.h"
// #include "board.h"
// #include "move.h"
// #include "attack.h"

// // Основная функция для хода бота
// void bot_make_move(CH_Type** board, int difficult, Player player, Board* CheckersBoard);

// // Вспомогательные функции
// int evaluate_position(CH_Type** board, Player player);
// int minimax(CH_Type** board, int depth, int alpha, int beta, bool isMaximizing, Player player, int maxDepth);
// Move* find_best_move(CH_Type** board, Player player, int maxDepth);
// Move* generate_all_moves(CH_Type** board, Player player);
// void free_move(Move* move);

// #endif