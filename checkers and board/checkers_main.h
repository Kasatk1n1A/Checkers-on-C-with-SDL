#ifndef CHECKERS_MAIN
#define CHECKERS_MAIN

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "board.h"
#include "move.h"
#include "attack.h"
#include "board_visual.h"

//----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$

int checkers(void* ptr);
bool Win_Check(CH_Type** board, Player player);

//----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$

Game* game;

//----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$

int checkers(void* ptr)
{
    CH_Type** board = add_board();
    //  a { 0, r, 0, r, 0, r, 0, r } n
    //  b { r, 0, r, 0, r, 0, r, 0 } i
    //  c { 0, r, 0, r, 0, r, 0, r } g
    //  d { 0, 0, 0, 0, 0, 0, 0, 0 } g
    //  e { 0, 0, 0, 0, 0, 0, 0, 0 } e
    //  f { w, 0, w, 0, w, 0, w, 0 } r
    //  g { 0, w, 0, w, 0, w, 0, w } s
    //  h { w, 0, w, 0, w, 0, w, 0 } !
    //      1  2  3  4  5  6  7  8

    Player player = WHITE;
    while (true) {
        out_board(board);
        if (player == RED)
            printf("Red turn.\n");            
        else
            printf("White turn.\n");

        //проверка на необходимость атаки
        bool** attack_board = canCapture(board, player == WHITE ? true : false);
        if (attack_board) {
            executeCaptureMove(board, attack_board, player);
            freeBoard((void**)attack_board);
        }
        else {
            //Обычный ход
            executeRegularMove(board, player);
        }

        if (Win_Check(board, player)) {
            switch (player)
            {
            case WHITE:
                printf("White won!\n");
                break;
            case RED:
                printf("Red won!\n");
                    break;
            }
            break;
        }
        //смена игрока
        player = (player == WHITE) ? RED : WHITE;
    }

    freeBoard((void**)board);
    return 0;
}

bool Win_Check(CH_Type** board, Player player)
{
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++) {
            switch (player) 
            {
            case WHITE:
                if (board[i][j] == RED_PAWN || board[i][j] == RED_KING)
                    return false;
                break;
            case RED:
                if (board[i][j] == WHITE_PAWN || board[i][j] == RED_KING)
                    return false;
                break;
            }
        }

    return true;
}

#endif