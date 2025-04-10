#include "game_rules.h"


int checkers(Window* window, CH_Type** board, int difficult, double Time, Player color)
{
    Board* CheckersBoard = (Board*)malloc(sizeof(Board));
    LoadBoardTextures(CheckersBoard, window);

    if (board == NULL)
        CheckersBoard->board = add_board();
    else
        CheckersBoard->board = board;
    
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
    GameInfo info;
    info.Time = Time;
    info.difficult = difficult;
    info.player = color;

    while (true)
    {
        out_board(CheckersBoard->board);
        if (info.player == RED)
        {
            // Ход бота
            printf("Red turn.\n");
            bot_make_move(CheckersBoard->board, info.difficult, RED);
        }
        else
        {
            printf("White turn.\n");
            start = clock();  // Начинаем замер времени только для белых

            renderBoardFrame(window, CheckersBoard);
            //проверка на необходимость атаки
            bool** attack_board = canCapture(CheckersBoard->board, info.player == WHITE ? true : false);
            if (attack_board) {
                executeCaptureMove(window, CheckersBoard->board, attack_board, CheckersBoard, info);
                freeBoard((void**)attack_board);
            }
            else    //Обычный ход
                executeRegularMove(window, CheckersBoard->board, info, CheckersBoard);
    
            end = clock();
            info.Time += ((double)(end - start)) / CLOCKS_PER_SEC;
            printf("White's move time: %.3f sec (Total: %.3f sec)\n", 
                ((double)(end - start)) / CLOCKS_PER_SEC, info.Time);
        }

        if (Win_Check(CheckersBoard->board, info.player))
        {
            info.player == WHITE ? printf("White won!\n") : printf("Red won!\n");
            break;
        }

        info.player = info.player == WHITE ? RED : WHITE;
    }

    printf("Total: %.3f sec\n", info.Time);

    board_cleanup_SDL(CheckersBoard);
    showMainMenu(window);
    return 0;
}

bool Win_Check(CH_Type** board, Player player)
{
    CH_Type enemy_pawn = (player == WHITE) ? RED_PAWN : WHITE_PAWN;
    CH_Type enemy_king = (player == WHITE) ? RED_KING : WHITE_KING;

    bool EnemyHasCheckers = false;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if (board[i][j] == enemy_pawn || board[i][j] == enemy_king)
                EnemyHasCheckers = true;
    
    if (!EnemyHasCheckers)
    {
        printf("Enemy hasn`t checkers\n");
        return true;
    }

    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if (board[i][j] == enemy_pawn || board[i][j] == enemy_king)
            {
                printf("y: %d x: %d\n", i, j);
                if (!isCheckerBlocked(board, j, i))
                    return false;
            }

    printf("All enemy`s checkers blocked\n");
    return true;
}