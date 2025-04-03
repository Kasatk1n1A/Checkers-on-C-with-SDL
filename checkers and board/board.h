#ifndef BOARD
#define BOARD

#include "StructsAndEnum.h"

//----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$

CH_Type** add_board(void);
void freeBoard(void** board);
void out_board(CH_Type** board);

//----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$

Game* game;

//----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$

//Создаёт изначальную доску в качестве массива из динамических массивов
CH_Type** add_board(void)
{
    CH_Type** board = (CH_Type**)malloc(sizeof(CH_Type*) * 8);       

    //{ 0, r, 0, r, 0, r, 0, r },
    //{ r, 0, r, 0, r, 0, r, 0 },
    //{ 0, r, 0, r, 0, r, 0, r },
    //{ 0, 0, 0, 0, 0, 0, 0, 0 },
    //{ 0, w, 0, 0, 0, 0, 0, 0 },
    //{ w, 0, w, 0, w, 0, w, 0 },
    //{ 0, w, 0, w, 0, w, 0, w },
    //{ w, 0, w, 0, w, 0, w, 0 },

    for (int i = 0; i < 8; i++){
        board[i] = (CH_Type*)malloc(sizeof(CH_Type) * 8);
        memset(board[i], 0, sizeof(CH_Type) * 8);
    }

    for (int i = 1; i < 8; i += 2)
        board[0][i] = RED_PAWN;
    for (int i = 0; i < 8; i += 2)
        board[1][i] = RED_PAWN;
    for (int i = 1; i < 8; i += 2)
        board[2][i] = RED_PAWN;

    for (int i = 0; i < 8; i += 2)
        board[5][i] = WHITE_PAWN;
    for (int i = 1; i < 8; i += 2)
        board[6][i] = WHITE_PAWN;
    for (int i = 0; i < 8; i += 2)
        board[7][i] = WHITE_PAWN;

    return board;
}

// Удаляет массив из динамических массивов (игральную доску)
void freeBoard(void** board) 
{
    if (!board) 
        return;
    for (int i = 0; i < 8; i++) {
        if (board[i]) {
            free(board[i]);
        }
    }
    free(board);
}

void out_board(CH_Type** board)
{
    for (int i = 0; i < 8; i++) {
        printf("%c|\t", 'a' + i);
        for (int j = 0; j < 8; j++) {
            if (board[i][j])
                printf("%d\t", board[i][j]);
            else
                printf("0\t");
        }
        printf("\n");
        printf("-----------------------------------------------------------------");
        printf("\n");

    }

    printf("\t");
    for (int i = 1; i <= 8; i++) {
        printf("%d\t", i);
    }
    printf("\n");
}

#endif