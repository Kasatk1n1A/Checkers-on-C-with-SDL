#ifndef TRANSPOSITION_H
#define TRANSPOSITION_H

#include "../../include/core/Checkers_types.h"
#include "../../include/core/Board_types.h"
#include "board.h"



typedef struct Move {
    int fromX, fromY;    // Координаты начальной позиции (откуда)
    int toX, toY;        // Координаты конечной позиции (куда)
    struct Move* next;    // Указатель на следующий ход (для списка)
    struct Move* captures; // Список съеденных фигур (для взятий)
} Move;

typedef struct
{
    CH_Type** board_positions;
    Player player;
    int eval;
    int depth;
    Move* BestMove;
} Transposition;

//Функции для транспозиций(одинаковых результатов ходов)
void free_move(Move* move);
Transposition* Transposition_Create(CH_Type** Board, Player WhoPlay, int evaluation, int depth, Move* BestMove);
void Transposition_Delete(Transposition** trans);
bool Transposition_Compare(void* _trans1, void* _trans2);
void Transpositions_Add(Transposition*** Cash, int* n, Transposition* trans);
void Transpositions_Delete(Transposition*** Cash, int *n);
uint32_t Transposition_hash(void* _trans, uint32_t size);

#endif