#include "game_rules.h"


int checkers(Window* window, CH_Type** board, double Time, Player color)
{
    SDL_Texture* background = IMG_LoadTexture(window->renderer, "assets/images/Main/Kover.png");
    if (!background) 
    {
        fprintf(stderr, "Error creating Texture: %s\n", IMG_GetError());
        return -1;
    }

    Board* CheckersBoard = (Board*)malloc(sizeof(Board));

    if (board == NULL)
        CheckersBoard->board = add_board();
    else
        CheckersBoard->board = board;
    
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
    
    
    clock_t start, end;
    double white_time_used = Time;  // Общее время для белых

    Player player = color;
    while (true)
    {
        out_board(CheckersBoard->board);
        if (player == RED)
            printf("Red turn.\n");            
        else
        {
            printf("White turn.\n");
            start = clock();  // Начинаем замер времени только для белых
        }
        
        renderBoardFrame(window, CheckersBoard, background);
        //проверка на необходимость атаки
        bool** attack_board = canCapture(CheckersBoard->board, player == WHITE ? true : false);
        if (attack_board) {
            executeCaptureMove(window, CheckersBoard->board, attack_board, player, CheckersBoard, white_time_used);
            freeBoard((void**)attack_board);
        }
        else
            //Обычный ход
            executeRegularMove(window, CheckersBoard->board, player, CheckersBoard, white_time_used);

        if (player == WHITE) {
            end = clock();
            white_time_used += ((double)(end - start)) / CLOCKS_PER_SEC;
            printf("White's move time: %.3f sec (Total: %.3f sec)\n", 
                ((double)(end - start)) / CLOCKS_PER_SEC, white_time_used);
        }

        if (Win_Check(CheckersBoard->board, player))
        {
            player == WHITE ? printf("White won!\n") : printf("Red won!\n");
            break;
        }
        //смена игрока
        player = (player == WHITE) ? RED : WHITE;
    }

    printf("Total: %.3f sec\n", white_time_used);

    SDL_DestroyTexture(background);
    board_cleanup_SDL(CheckersBoard);
    showMainMenu(window);
    return 0;
}

bool Win_Check(CH_Type** board, Player player)
{
    CH_Type enemy_pawn = (player == WHITE) ? RED_PAWN : WHITE_PAWN;
    CH_Type enemy_king = (player == WHITE) ? RED_KING : WHITE_KING;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j] == enemy_pawn || board[i][j] == enemy_king) {
                return false;
            }
        }
    }
    return true;
}