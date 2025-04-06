#include "game_rules.h"


int checkers(Window* window)
{
    SDL_Texture* background = IMG_LoadTexture(window->renderer, "assets/images/Main/Kover2.png");
    if (!background) 
    {
        fprintf(stderr, "Error creating Texture: %s\n", IMG_GetError());
        return -1;
    }

    Board* CheckersBoard = (Board*)malloc(sizeof(Board));
    CheckersBoard->board = add_board();
    LoadBoardTextures(CheckersBoard, window);
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
        out_board(CheckersBoard->board);
        if (player == RED)
            printf("Red turn.\n");            
        else
            printf("White turn.\n");
        
        renderBoardFrame(window, CheckersBoard, background);
        //проверка на необходимость атаки
        bool** attack_board = canCapture(CheckersBoard->board, player == WHITE ? true : false);
        if (attack_board) {
            executeCaptureMove(window, CheckersBoard->board, attack_board, player, CheckersBoard);
            freeBoard((void**)attack_board);
        }
        else
            //Обычный ход
            executeRegularMove(window, CheckersBoard->board, player, CheckersBoard);

        if (Win_Check(CheckersBoard->board, player))
            player == WHITE ? printf("White won!\n") : printf("Red won!\n");

        //смена игрока
        player = (player == WHITE) ? RED : WHITE;
    }

    freeBoard((void**)CheckersBoard->board);
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