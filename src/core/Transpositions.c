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

void _Transposition_Delete_(void* _trans){
    Transposition* trans = (Transposition*)_trans;
    Transposition_Delete(trans);
}

void Transposition_Delete(Transposition* trans)
{
    if (!trans)
        return;

    printf("Deleting trans -> ");

    printf("%d\n", (trans)->depth);
    freeBoard((void**)trans->board_positions);
    free_move(trans->BestMove);
    free(trans);
    trans = NULL;

    printf("Trans`s delete success\n");
    return;
}

bool Transposition_Compare(void* _trans1, void* _trans2)
{
    Transposition* trans1 = (Transposition*)_trans1;
    Transposition* trans2 = (Transposition*)_trans2;

    printf("Comparing trans -> ");
    if (trans1->player != trans2->player)
    {
        printf("not equal\n");
        return false;
    }
    
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if (trans1->board_positions[i][j] != trans2->board_positions[i][j]){
                printf("not equal\n");
                return false;
            }
        }
    }
    
    printf("equal\n");
    return true;
}

uint32_t Transposition_hash(void* _trans, uint32_t size) {
    Transposition* trans = (Transposition*)_trans;
    const uint32_t prime = 31;
    uint32_t hash = (uint32_t)trans->player;
    
    // printf("%d\n", trans->board_positions[1][0]);

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            hash = hash * prime + (uint32_t)trans->board_positions[i][j];
            // printf("NIGGER\n");
        }
        // Добавляем разделитель между строками
        hash = hash * prime + prime;
    }
    
    return hash % size;
}



// void Transpositions_Delete(Transposition*** Cash, int *n)
// {
//     printf("Deleting all trans -> ");

//     for (int i = 0; i < *n; i++)
//         Transposition_Delete(&Cash[0][i]);
    
//     free(*Cash);
//     *Cash = NULL;
//     *n = 0;

//     printf("Transpositions deleted success\n");
// }

// void Transpositions_Add(Transposition*** Cash, int* n, Transposition* trans)
// {
//     printf("Adding trans -> ");

//     for (int i = 0; i < *n; i++)
//     {
//         if (Transposition_Compare(Cash[0][i], trans))
//         {
//             if (Cash[0][i]->depth <= trans->depth)
//                 Cash[0][i]->depth = Cash[0][i]->depth <= trans->depth ? trans->depth : Cash[0][i]->depth;
//             Transposition_Delete(&trans);
//             printf("New transposition add success\n");
//             return;
//         }
//     }

//     Transposition** tmp = (Transposition**)malloc(sizeof(Transposition*) * (*n + 1));
//     int tmp_n = *n;
//     for (int i = 0; i < *n + 1; i++)
//         tmp[i] = NULL;
//     memcpy(tmp, *Cash, sizeof(Transposition*) * (*n));
//     Transpositions_Delete(Cash, &tmp_n);
//     *n = tmp_n;
//     *Cash = tmp;

//     Cash[0][*n] = trans;
//     (*n)++;

//     printf("New transposition add success\n");
// }