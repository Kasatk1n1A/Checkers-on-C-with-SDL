#include "Transpositions.h"


// Освобождение памяти, занятой списком ходов
void free_move(Move* move) 
{
    if (!move) return;

    Move* current = move;
    while (current) 
    {
        Move* next = current->next;
        if (current->captures) {
            free_move(current->captures);
            current->captures = NULL;
        }
        free(current);
        current = next;
    }
}

Transposition* Transposition_Create(CH_Type** Board, Player WhoPlay, int evaluation, int depth, Move* BestMove)
{
    printf("Trans creating -> ");

    Transposition* trans = (Transposition*)malloc(sizeof(Transposition));

    trans->board_positions = add_board();
    CopyBoard(Board, trans->board_positions);

    trans->player = WhoPlay;
    trans->eval = evaluation;
    trans->depth = depth;
    trans->BestMove = BestMove;

    printf("Creating success\n");
    return trans;
}

void Transposition_Delete(Transposition** trans)
{
    if (!*trans)
        return;

    printf("Deleting trans -> ");

    printf("%d\n", (*trans)->depth);
    freeBoard((void**)(*trans)->board_positions);
    free_move((*trans)->BestMove);
    free(*trans);
    *trans = NULL;

    printf("Trans`s delete success\n");
    return;
}

bool Transposition_Compare(Transposition* trans1, Transposition* trans2)
{
    printf("Comparing trans -> ");
    if (trans1->player != trans2->player)
    {
        printf("Compare success\n");
        return false;
    }
    
    for (int i = 0; i < 8; i++)
        if (memcmp(trans1->board_positions[i], trans2->board_positions[i], sizeof(CH_Type) * 8))
        {
            printf("This trans is unique\n");
            return false;
        }

    printf("This trans have yet\n");
    return true;
}

void Transpositions_Delete(Transposition*** Cash, int *n)
{
    printf("Deleting all trans -> ");

    for (int i = 0; i < *n; i++)
        Transposition_Delete(&Cash[0][i]);
    
    free(*Cash);
    *Cash = NULL;
    *n = 0;

    printf("Transpositions deleted success\n");
}

void Transpositions_Add(Transposition*** Cash, int* n, Transposition* trans)
{
    printf("Adding trans -> ");

    for (int i = 0; i < *n; i++)
    {
        if (Transposition_Compare(Cash[0][i], trans))
        {
            if (Cash[0][i]->depth <= trans->depth)
                Cash[0][i]->depth = Cash[0][i]->depth <= trans->depth ? trans->depth : Cash[0][i]->depth;
            Transposition_Delete(&trans);
            printf("New transposition add success\n");
            return;
        }
    }

    Transposition** tmp = (Transposition**)malloc(sizeof(Transposition*) * (*n + 1));
    int tmp_n = *n;
    for (int i = 0; i < *n + 1; i++)
        tmp[i] = NULL;
    memcpy(tmp, *Cash, sizeof(Transposition*) * (*n));
    Transpositions_Delete(Cash, &tmp_n);
    *n = tmp_n;
    *Cash = tmp;

    Cash[0][*n] = trans;
    (*n)++;

    printf("New transposition add success\n");
}