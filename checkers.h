#define _CRT_SECURE_NO_WARNINGS
// #include <stdio.h>
// #include <stdbool.h>
// #include <stdlib.h>
// #include <string.h>
// #include <time.h>
// #include "visualise.h"

Game* game;

enum checker_type
{
    WHITE_CH = 1,
    RED_CH,
    PICKED_WHITE_CH,
    PICKED_RED_CH,
    KING_WHITE_CH,
    KING_RED_CH,
    KING_PICKED_WHITE_CH,
    KING_PICKED_RED_CH
};

enum player
{
    WHITE_PLAYER = 1,
    RED_PLAYER
};

int checkers(void* ptr);
bool continue_attack(int** board, int x, int y, int player);
bool Win_Check(int** board, int player);
void attack_options(int** board);
void board_cleanup(int** board);
int** add_board(void);
void attack(int** board, int** atack_board, int player);
void attacking(int** board, int x1, int y1, int x2, int y2);
int** attack_check(int** board, int player);
void out_board(int** board);
bool try_to_move(int** board, int x, int y, int player);
void move_checker(int** board, int x1, int y1, int x2, int y2);
void move_without_attack(int** board, int player);
void King_check(int** board, int x, int y, int player);

int checkers(void* ptr)
{
    int** board = add_board();
    //  a { 0, r, 0, r, 0, r, 0, r } n
    //  b { r, 0, r, 0, r, 0, r, 0 } i
    //  c { 0, r, 0, r, 0, r, 0, r } g
    //  d { 0, 0, 0, 0, 0, 0, 0, 0 } g
    //  e { 0, 0, 0, 0, 0, 0, 0, 0 } e
    //  f { w, 0, w, 0, w, 0, w, 0 } r
    //  g { 0, w, 0, w, 0, w, 0, w } s
    //  h { w, 0, w, 0, w, 0, w, 0 } !
    //      1  2  3  4  5  6  7  8

    int player = WHITE_PLAYER;
    while (true) {
        out_board(board);
        if (player == RED_PLAYER)
            printf("Red turn.\n");            
        else
            printf("White turn.\n");

        //проверка на необходимость атаки
        int** attack_board = attack_check(board, player);
        if (attack_board) {
            attack(board, attack_board, player);
            board_cleanup(attack_board);
        }
        else {
            //Обычный ход
            move_without_attack(board, player);
        }

        if (Win_Check(board, player)) {
            switch (player)
            {
            case WHITE_PLAYER:
                printf("White won!\n");
                break;
            case RED_PLAYER:
                printf("Red won!\n");
                    break;
            }
            break;
        }
        //смена игрока
        player = (player == WHITE_PLAYER) ? RED_PLAYER : WHITE_PLAYER;
    }

    board_cleanup(board);
    return 0;
}

bool Win_Check(int** board, int player)
{
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++) {
            switch (player) 
            {
            case WHITE_PLAYER:
                if (board[i][j] == RED_CH || board[i][j] == KING_RED_CH)
                    return false;
                break;
            case RED_PLAYER:
                if (board[i][j] == WHITE_CH || board[i][j] == KING_RED_CH)
                    return false;
                break;
            }
        }

    return true;
}

//Проверяет условие появления дамки
void King_check(int** board, int x, int y, int player)
{
    if (!board[y][x])
        return;
    switch (board[y][x]) 
    {
        //Превращает белую шашку в дамку
    case WHITE_CH:
        if (y == 0) {
            board[y][x] = KING_WHITE_CH;
        }
        break;
        //Превращает красную шашку в дамку
    case RED_CH:
        if (y == 7) {
            board[y][x] = KING_RED_CH;
        }
        break;
    default:
        return;
    }
}

//Делает обычный ход, никого не съедая
void move_without_attack(int** board, int player)
{
    int x1, y1;
    while (true)
    {
        //Выбор фигуры
        printf("\nPick checker:");
        char str[3];
        if (!scanf("%s", str) || !(str[0] >= 'a' && str[0] <= 'h') || !(str[1] >= '1' && str[1] <= '8'))
            exit(EXIT_FAILURE);
        x1 = str[1] - '1';
        y1 = str[0] - 'a';
        // SDL_LockMutex(game->mutex);
        // while (SDL_CondWait(game->cond, game->mutex)) 
        //     ;
        // x1 = game->Mouse.x;
        // y1 = game->Mouse.y;
        // SDL_UnlockMutex(game->mutex);

        // Вывод координат в консоль
        // printf("Координаты нажатия: (%d, %d)\n", x1, y1);
        //Ввод координат передвигаемой фигуры
        // x1 = (game->Mouse.x - 239 - 43) / 105;
        // y1 = (game->Mouse.y - 41) / 105;
        // printf("%d %d\n", x1, y1);
        // SDL_UnlockMutex(game->mutex);
        
        //Проверка на корректность введёных координат и может ли фигура двигаться
        if (x1 < 0 || x1 > 7 || y1 < 0 || y1 > 7) {
            printf("Incorrect, try again.\n");
            continue;
        }
        if (try_to_move(board, x1, y1, player)) {
            continue;
        }
        break;
    }

    // SDL_LockMutex(game->mutex);
    // game->board->checkers[y1][x1]->flag = KING_PICKED_WHITE_CH;
    // SDL_UnlockMutex(game->mutex);

    //Ожидание выбора куда сходить
    int x2, y2;
    while (true) 
    {
        //Ввод координат куда двигать фигуру
        printf("Where to move:");
        char str[3];
        if (!scanf("%s", str) || !(str[0] >= 'a' && str[0] <= 'h') || !(str[1] >= '1' && str[1] <= '8'))
            exit(EXIT_FAILURE);

        x2 = str[1] - '1';
        y2 = str[0] - 'a';

        if (x2 < 0 && x2 > 7 || y2 < 0 && y2 > 7) {
            printf("Incorrect move, try again.\n");
            continue;
        }
        if (board[y2][x2])
            continue;

        bool f = false;
        switch (board[y1][x1]) {
        case WHITE_CH:
            if ((x2 == x1 - 1 || x2 == x1 + 1) && y2 == y1 - 1) {
                f = true;
                move_checker(board, x1, y1, x2, y2);
            }
            break;
        case RED_CH:
            if ((x2 == x1 - 1 || x2 == x1 + 1) && y2 == y1 + 1) {
                f = true;
                move_checker(board, x1, y1, x2, y2);
            }
            break;
        case KING_WHITE_CH:
        case KING_RED_CH:
            for (int n = 1; x1 + n < 8 && y1 + n < 8; n++)
            {
                if (board[y1 + n][x1 + n])
                    break;
                if (x1 + n == x2 && y1 + n == y2) {
                    f = true;
                    move_checker(board, x1, y1, x2, y2);
                    break;
                }
            }
            for (int n = 1; x1 - n >= 0 && y1 + n < 8; n++)
            {
                if (board[y1 + n][x1 - n])
                    break;
                if (x1 - n == x2 && y1 + n == y2) {
                    f = true;
                    move_checker(board, x1, y1, x2, y2);
                    break;
                }
            }
            for (int n = 1; x1 + n < 8 && y1 - n >= 0; n++)
            {
                if (board[y1 - n][x1 + n])
                    break;
                if (x1 + n == x2 && y1 - n == y2) {
                    f = true;
                    move_checker(board, x1, y1, x2, y2);
                    break;
                }
            }
            for (int n = 1; x1 - n >= 0 && y1 - n >= 0; n++)
            {
                if (board[y1 - n][x1 - n])
                    break;
                if (x1 - n == x2 && y1 - n == y2) {
                    f = true;
                    move_checker(board, x1, y1, x2, y2);
                    break;
                }
            }
            break;
        default:
            continue;
            break;
        }
        if (f) {
            King_check(board, x2, y2, player);
            break;
        }
    }
}

bool continue_attack(int** board, int x, int y, int player)
{
    int** board_of_rub = attack_check(board, player);
    if (board_of_rub == NULL || !board_of_rub[y][x]) {
        return false;
    }

    printf("End turn? (Y/N): ");
    char c = getchar();
    getchar();
    
    if (c == 'Y') {
        return false;
    }
    else {
        return true;
    }
    return false;
}

//Ход со съеданием
void attack(int** board, int** atack_board, int player)
{
    attack_options(atack_board);

    char str[3];
    int x1, y1, x2, y2;
    while (true) {
        //Ввод координат передвигаемой фигуры
        printf("\nPick checker:");
        if (!scanf("%s", str) || !(str[0] >= 'a' && str[0] <= 'h') || !(str[1] >= '1' && str[1] <= '8'))
            exit(EXIT_FAILURE);
        y1 = str[0] - 'a';
        x1 = str[1] - '1';

        //Проверка на корректность введёных координат (x1, y1), если некорректно, то попробовать снова
        if (x1 < 0 && x1 > 8 || y1 < 0 && y1 > 8) {
            printf("Incorrect, try again.\n");
            continue;
        }
        if (!atack_board[y1][x1]) {
            printf("Incorrect, try again.\n");
            continue;
        }
        break;
    }

    while (true) {
        //Ввод координат куда двигать фигуру
        printf("Where to move:");
        if (!scanf("%s", str) || !(str[0] >= 'a' && str[0] <= 'h') || !(str[1] >= '1' && str[1] <= '8'))
            exit(EXIT_FAILURE);
        y2 = str[0] - 'a';  
        x2 = str[1] - '1';

        //Проверка на корректность введёных координат (x2, y2), если некорректно, то попробовать снова
        if (board[y2][x2] != 0) {
            printf("Incorrect, try again.\n");
            continue;
        }
        if (x2 < 0 && x2 > 8 || y2 < 0 && y2 > 8) {
            printf("Incorrect, try again.\n");
            continue;
        }
        switch (board[y1][x1]) {
        case WHITE_CH:
        case RED_CH:
            if (!((x1 == x2 - 2 && y1 == y2 + 2) || (x1 == x2 + 2 && y1 == y2 + 2) || (x1 == x2 - 2 && y1 == y2 - 2) || (x1 == x2 + 2 && y1 == y2 - 2))) {
                printf("Incorrect, try again.\n");
                continue;
            }
            goto lets_attack;
            break;
        case KING_WHITE_CH:   //может можно как-то получше сделать
            if (x2 < x1 && y2 < y1) {
                for (int n = 1; x1 - n > 0 && y1 - n > 0; n++) {
                    if (board[y1 - n][x1 - n] == WHITE_CH || board[y1 - n][x1 - n] == KING_WHITE_CH) {
                        break;
                    }
                    if (board[y1 - n][x1 - n] == RED_CH || board[y1 - n][x1 - n] == KING_RED_CH) {
                        if (x1 - n - 1 == x2 && y1 - n - 1 == y2)
                            goto lets_attack;
                        else
                            break;
                    }
                }
                printf("Incorrect, try again.\n");
                continue;
            }
            if (x2 > x1 && y2 < y1) {
                for (int n = 1; x1 + n < 8 && y1 - n > 0; n++) {
                    if (board[y1 - n][x1 + n] == WHITE_CH || board[y1 - n][x1 + n] == KING_WHITE_CH) {
                        break;
                    }
                    if (board[y1 - n][x1 + n] == RED_CH || board[y1 - n][x1 + n] == KING_RED_CH) {
                        if (x1 + n + 1 == x2 && y1 - n - 1 == y2)
                            goto lets_attack;
                        else
                            break;
                    }
                }
                printf("Incorrect, try again.\n");
                continue;
            }
            if (x2 < x1 && y2 > y1) {
                for (int n = 1; x1 - n > 0 && y1 + n < 8; n++) {
                    if (board[y1 + n][x1 - n] == WHITE_CH || board[y1 + n][x1 - n] == KING_WHITE_CH) {
                        break;
                    }
                    if (board[y1 + n][x1 - n] == RED_CH || board[y1 + n][x1 - n] == KING_RED_CH) {
                        if (x1 - n - 1 == x2 && y1 + n + 1 == y2)
                            goto lets_attack;
                        else
                            break;
                    }
                }
                printf("Incorrect, try again.\n");
                continue;
            }
            if (x2 > x1 && y2 > y1) {
                for (int n = 1; x1 + n < 8 && y1 + n < 8; n++) {
                    if (board[y1 + n][x1 + n] == WHITE_CH || board[y1 + n][x1 + n] == KING_WHITE_CH) {
                        break;
                    }
                    if (board[y1 + n][x1 + n] == RED_CH || board[y1 + n][x1 + n] == KING_RED_CH) {
                        if (x1 + n + 1 == x2 && y1 + n + 1 == y2)
                            goto lets_attack;
                        else
                            break;
                    }
                }
                printf("Incorrect, try again.\n");
                continue;
            }
            break;
        case KING_RED_CH:
            if (x2 < x1 && y2 < y1) {
                for (int n = 1; x1 - n > 0 && y1 - n > 0; n++) {
                    if (board[y1 - n][x1 - n] == RED_CH || board[y1 - n][x1 - n] == KING_RED_CH) {
                        break;
                    }
                    if (board[y1 - n][x1 - n] == WHITE_CH || board[y1 - n][x1 - n] == KING_WHITE_CH) {
                        if (x1 - n - 1 == x2 && y1 - n - 1 == y2)
                            goto lets_attack;
                        else
                            break;
                    }
                }
                printf("Incorrect, try again.\n");
                continue;
            }
            if (x2 > x1 && y2 < y1) {
                for (int n = 1; x1 + n < 8 && y1 - n > 0; n++) {
                    if (board[y1 - n][x1 + n] == RED_CH || board[y1 - n][x1 + n] == KING_RED_CH) {
                        break;
                    }
                    if (board[y1 - n][x1 + n] == WHITE_CH || board[y1 - n][x1 + n] == KING_WHITE_CH) {
                        if (x1 + n + 1 == x2 && y1 - n - 1 == y2)
                            goto lets_attack;
                        else
                            break;
                    }
                }
                printf("Incorrect, try again.\n");
                continue;
            }
            if (x2 < x1 && y2 > y1) {
                for (int n = 1; x1 - n > 0 && y1 + n < 8; n++) {
                    if (board[y1 + n][x1 - n] == RED_CH || board[y1 + n][x1 - n] == KING_RED_CH) {
                        break;
                    }
                    if (board[y1 + n][x1 - n] == WHITE_CH || board[y1 + n][x1 - n] == KING_WHITE_CH) {
                        if (x1 - n - 1 == x2 && y1 + n + 1 == y2)
                            goto lets_attack;
                        else
                            break;
                    }
                }
                printf("Incorrect, try again.\n");
                continue;
            }
            if (x2 > x1 && y2 > y1) {
                for (int n = 1; x1 + n < 8 && y1 + n < 8; n++) {
                    if (board[y1 + n][x1 + n] == RED_CH || board[y1 + n][x1 + n] == KING_RED_CH) {
                        break;
                    }
                    if (board[y1 + n][x1 + n] == WHITE_CH || board[y1 + n][x1 + n] == KING_WHITE_CH) {
                        if (x1 + n + 1 == x2 && y1 + n + 1 == y2)
                            goto lets_attack;
                        else
                            break;
                    }
                }
                printf("Incorrect, try again.\n");
                continue;
            }
            break;
        }
        lets_attack:
        attacking(board, x1, y1, x2, y2);
        King_check(board, x2, y2, player);
        if (continue_attack(board, x2, y2, player)) {
            x1 = x2;
            y1 = y2;
        }
        else {
            break;
        }
    }
}

//Вывод вариантов хода при необходимости атаки
void attack_options(int** board) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j])
                printf("%c%d\t", 'a' + i, j + 1);
        }
    }
    printf("\n");
}

//Передвигает фигуру с позиции (x1, y1) на позицию  (x2, y2) удаляя шашку между ними
void attacking(int** board, int x1, int y1, int x2, int y2)
{
    move_checker(board, x1, y1, x2, y2);
    switch (board[y2][x2])
    {
    case RED_CH:
    case WHITE_CH:
        board[(y1 + y2) / 2][(x1 + x2) / 2] = 0;
        SDL_LockMutex(game->mutex);
        checker_cleanup_SDL(game->board->checkers[(y1 + y2) / 2][(x1 + x2) / 2]);
        SDL_UnlockMutex(game->mutex);
        break;
    case KING_RED_CH:
    case KING_WHITE_CH:
        if (y2 < y1 && x2 < x1) {
            board[y2 + 1][x2 + 1] = 0;
            SDL_LockMutex(game->mutex);
            checker_cleanup_SDL(game->board->checkers[y2 + 1][x2 + 1]);
            SDL_UnlockMutex(game->mutex);
            break;
        }
        if (y2 < y1 && x2 > x1) {
            board[y2 + 1][x2 - 1] = 0;
            SDL_LockMutex(game->mutex);
            checker_cleanup_SDL(game->board->checkers[y2 + 1][x2 - 1]);
            SDL_UnlockMutex(game->mutex);
            break;
        }
        if (y2 > y1 && x2 < x1) {
            board[y2 - 1][x2 + 1] = 0;
            SDL_LockMutex(game->mutex);
            checker_cleanup_SDL(game->board->checkers[y2 - 1][x2 + 1]);
            SDL_UnlockMutex(game->mutex);
            break;
        }
        if (y2 > y1 && x2 > x1) {
            board[y2 - 1][x2 - 1] = 0;
            SDL_LockMutex(game->mutex);
            checker_cleanup_SDL(game->board->checkers[y2 - 1][x2 - 1]);
            SDL_UnlockMutex(game->mutex);
            break;
        }
    }
}

//Проверяет все фигуры player-а на возможность рубить и возвращает указатель на динамический массив массивов, где указаны фигуры которые могут и должны рубить,
//если таковых нет то возвращает NULL
int** attack_check(int** board, int player)
{
    int** board_of_rub = (int**)malloc(sizeof(int*) * 8);
    for (int i = 0; i < 8; i++) {
        board_of_rub[i] = (int*)malloc(sizeof(int) * 8);
        memset(board_of_rub[i], 0, sizeof(int) * 8);
    }
    //    {0, 0, 0, 0, 0, 0, 0, 0},
    //    {0, 0, 0, 0, 0, 0, 0, 0},
    //    {0, 0, 0, 0, 0, 0, 0, 0},
    //    {0, 0, 0, 0, 0, 0, 0, 0},
    //    {0, 0, 0, 0, 0, 0, 0, 0},
    //    {0, 0, 0, 0, 0, 0, 0, 0},
    //    {0, 0, 0, 0, 0, 0, 0, 0},
    //    {0, 0, 0, 0, 0, 0, 0, 0},

    //Флаг для определения есть ли необходимость рубить
    int flag = 0;

    switch (player) 
    {
    case WHITE_PLAYER:
        for (int i = 0; i < 8; i++) 
        {
            for (int j = 0; j < 8; j++) 
            {                        
                //Обычная шашка
                switch (board[i][j]) 
                {
                case WHITE_CH:
                    if (j >= 0 && j <= 5) {
                        //Бьёт ли фигура вправо вверх
                        if (i >= 2 && (board[i - 1][j + 1] == RED_CH || board[i - 1][j + 1] == KING_RED_CH) && board[i - 2][j + 2] == 0) {
                            board_of_rub[i][j] = WHITE_CH;
                            flag++;
                        }
                        //Бьёт ли фигура вправо вниз
                        if (i <= 5 && (board[i + 1][j + 1] == RED_CH || board[i + 1][j + 1] == KING_RED_CH) && board[i + 2][j + 2] == 0) {
                            board_of_rub[i][j] = WHITE_CH;
                            flag++;
                        }
                    }
                    if (j >= 2 && j <= 7) {
                        //Бьёт ли фигура влево вверх
                        if (i >= 2 && (board[i - 1][j - 1] == RED_CH || board[i - 1][j - 1] == KING_RED_CH) && board[i - 2][j - 2] == 0) {
                            board_of_rub[i][j] = WHITE_CH;
                            flag++;
                        }
                        //Бьёт ли фигура влево вниз
                        if (i <= 5 && (board[i + 1][j - 1] == RED_CH || board[i + 1][j - 1] == KING_RED_CH) && board[i + 2][j - 2] == 0) {
                            board_of_rub[i][j] = WHITE_CH;
                            flag++;
                        }
                    }
                    break;
                //Если фигура - Дамка
                case KING_WHITE_CH:
                    //Бьёт ли фигура влево вверх
                    for (int n = 1; i - n > 0 && j - n > 0; n++) {
                        if (board[i - n][j - n] == WHITE_CH || board[i - n][j - n] == KING_WHITE_CH) {
                            break;
                        }
                        if ((board[i - n][j - n] == RED_CH || board[i - n][j - n] == KING_RED_CH) && board[i - n - 1][j - n - 1] == 0) {
                            board_of_rub[i][j] = KING_WHITE_CH;
                            flag++;
                            break;
                        }
                    }
                    //Бьёт ли фигура вправо вверх
                    for (int n = 1; i - n > 0 && j + n < 8; n++) {
                        if (board[i - n][j + n] == WHITE_CH || board[i - n][j + n] == KING_WHITE_CH) {
                            break;
                        }
                        if ((board[i - n][j + n] == RED_CH || board[i - n][j + n] == KING_RED_CH) && board[i - n - 1][j + n + 1] == 0) {
                            board_of_rub[i][j] = KING_WHITE_CH;
                            flag++;
                            break;
                        }
                    }
                    //Бьёт ли фигура влево вниз
                    for (int n = 1; i + n < 8 && j - n > 0; n++) {
                        if (board[i + n][j - n] == WHITE_CH || board[i + n][j - n] == KING_WHITE_CH) {
                            break;
                        }
                        if ((board[i + n][j - n] == RED_CH || board[i + n][j - n] == KING_RED_CH) && board[i + n + 1][j - n - 1] == 0) {
                            board_of_rub[i][j] = KING_WHITE_CH;
                            flag++;
                            break;
                        }
                    }
                    //Бьёт ли фигура вправо вниз
                    for (int n = 1; i + n < 8 && j + n < 8; n++) {
                        if (board[i + n][j + n] == WHITE_CH || board[i + n][j + n] == KING_WHITE_CH) {
                            break;
                        }
                        if ((board[i + n][j + n] == RED_CH || board[i + n][j + n] == KING_RED_CH) && board[i + n + 1][j + n + 1] == 0) {
                            board_of_rub[i][j] = KING_WHITE_CH;
                            flag++;
                            break;
                        }
                    }
                    break;
                }
            }
        }
        break;
    case RED_PLAYER:
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (i < 8 && board[i][j] == RED_CH)
                {
                    if (j >= 0 && j <= 5) {
                        //Бьёт ли фигура вправо вверх
                        if (i >= 2 && (board[i - 1][j + 1] == WHITE_CH || board[i - 1][j + 1] == KING_WHITE_CH) && board[i - 2][j + 2] == 0) {
                            board_of_rub[i][j] = RED_CH;
                            flag++;
                        }
                        //Бьёт ли фигура вправо вниз
                        if (i <= 5 && (board[i + 1][j + 1] == WHITE_CH || board[i + 1][j + 1] == KING_WHITE_CH) && board[i + 2][j + 2] == 0) {
                            board_of_rub[i][j] = RED_CH;
                            flag++;
                        }
                    }
                    if (j >= 2 && j <= 7) {
                        //Бьёт ли фигура влево вверх
                        if (i >= 2 && (board[i - 1][j - 1] == WHITE_CH || board[i - 1][j - 1] == KING_WHITE_CH) && board[i - 2][j - 2] == 0) {
                            board_of_rub[i][j] = RED_CH;
                            flag++;
                        }
                        //Бьёт ли фигура влево вниз
                        if (i <= 5 && (board[i + 1][j - 1] == WHITE_CH || board[i + 1][j - 1] == KING_WHITE_CH) && board[i + 2][j - 2] == 0) {
                            board_of_rub[i][j] = RED_CH;
                            flag++;
                        }
                    }
                    continue;
                }
                //Если фигура - Дамка
                if (board[i][j] == KING_RED_CH)
                {
                    //Бьёт ли фигура влево вверх
                    for (int n = 1; i - n > 0 && j - n > 0; n++) {
                        if (board[i - n][j - n] == RED_CH || board[i - n][j - n] == KING_RED_CH) {
                            break;
                        }
                        if ((board[i - n][j - n] == WHITE_CH || board[i - n][j - n] == KING_WHITE_CH) && board[i - n - 1][j - n - 1] == 0) {
                            board_of_rub[i][j] = KING_RED_CH;
                            flag++;
                            break;
                        }
                    }
                    //Бьёт ли фигура вправо вверх
                    for (int n = 1; i - n > 0 && j + n < 8; n++) {
                        if (board[i - n][j + n] == RED_CH || board[i - n][j + n] == KING_RED_CH) {
                            break;
                        }
                        if ((board[i - n][j + n] == WHITE_CH || board[i - n][j + n] == KING_WHITE_CH) && board[i - n - 1][j + n + 1] == 0) {
                            board_of_rub[i][j] = KING_RED_CH;
                            flag++;
                            break;
                        }
                    }
                    //Бьёт ли фигура влево вниз
                    for (int n = 1; i + n < 8 && j - n > 0; n++) {
                        if (board[i + n][j - n] == RED_CH || board[i + n][j - n] == KING_RED_CH) {
                            break;
                        }
                        if ((board[i + n][j - n] == WHITE_CH || board[i + n][j - n] == KING_WHITE_CH) && board[i + n + 1][j - n - 1] == 0) {
                            board_of_rub[i][j] = KING_RED_CH;
                            flag++;
                            break;
                        }
                    }
                    //Бьёт ли фигура вправо вниз
                    for (int n = 1; i + n < 8 && j + n < 8; n++) {
                        if (board[i + n][j + n] == RED_CH || board[i + n][j + n] == KING_RED_CH) {
                            break;
                        }
                        if ((board[i + n][j + n] == WHITE_CH || board[i + n][j + n] == KING_WHITE_CH) && board[i + n + 1][j + n + 1] == 0) {
                            board_of_rub[i][j] = KING_RED_CH;
                            flag++;
                            break;
                        }
                    }
                }
            }
        }
        break;
    }

    if (!flag) {
        board_cleanup(board_of_rub);
        return NULL;
    }
    return board_of_rub;
}

//Создаёт изначальную доску в качестве массива из динамических массивов
int** add_board(void)
{
    int** board = (int**)malloc(sizeof(int*) * 8);       

    //{ 0, r, 0, r, 0, r, 0, r },
    //{ r, 0, r, 0, r, 0, r, 0 },
    //{ 0, r, 0, r, 0, r, 0, r },
    //{ 0, 0, 0, 0, 0, 0, 0, 0 },
    //{ 0, w, 0, 0, 0, 0, 0, 0 },
    //{ w, 0, w, 0, w, 0, w, 0 },
    //{ 0, w, 0, w, 0, w, 0, w },
    //{ w, 0, w, 0, w, 0, w, 0 },

    for (int i = 0; i < 8; i++){
        board[i] = (int*)malloc(sizeof(int) * 8);
        memset(board[i], 0, sizeof(int) * 8);
    }

    for (int i = 1; i < 8; i += 2)
        board[0][i] = RED_CH;
    for (int i = 0; i < 8; i += 2)
        board[1][i] = RED_CH;
    for (int i = 1; i < 8; i += 2)
        board[2][i] = RED_CH;

    for (int i = 0; i < 8; i += 2)
        board[5][i] = WHITE_CH;
    for (int i = 1; i < 8; i += 2)
        board[6][i] = WHITE_CH;
    for (int i = 0; i < 8; i += 2)
        board[7][i] = WHITE_CH;

    return board;
}

//удаляет массив из динамических массивов (игральную доску)
void board_cleanup(int** board) {
    if (!board) 
        return;
    for (int i = 0; i < 8; i++) {
        if (board[i]) {
            free(board[i]);
        }
    }
    free(board);
}

//передвигает фигуру с позиции x1 y1 на позицию x2 y2
void move_checker(int** board, int x1, int y1, int x2, int y2)
{
    int f = board[y1][x1];
    board[y1][x1] = 0;
    board[y2][x2] = f;

    SDL_LockMutex(game->mutex);
    Checker* tmp = game->board->checkers[y1][x1];
    game->board->checkers[y2][x2] = game->board->checkers[y1][x1];
    game->board->checkers[y1][x1] = NULL;
    game->board->checkers[y2][x2]->rect.x = 282 + 105 * x2;
    game->board->checkers[y2][x2]->rect.y = 43 + 105 * y2;
    SDL_UnlockMutex(game->mutex);
}

bool try_to_move(int** board, int x, int y, int player)
{
    switch (player)
    {
    case (WHITE_PLAYER):
        if (board[y][x] != WHITE_CH && board[y][x] != KING_WHITE_CH) {
            printf("Incorrect figure, try another.\n");
            return true;
        }
        if (x > 0 && x < 7) {
            if (board[y - 1][x - 1] != 0 && board[y - 1][x + 1] != 0) {
                printf("This checker locked, try another.\n");
                return true;
            }
        }
        else if (x == 0) {
            if (board[y - 1][x + 1] != 0) {
                printf("This checker locked, try another.\n");
                return true;
            }
        }
        else {
            if (board[y - 1][x - 1] != 0)
                printf("This checker locked, try another.\n");
            return true;
        }
        break;
    case (RED_PLAYER):
        if (board[y][x] != RED_CH && board[y][x] != KING_RED_CH) {
            printf("Incorrect figure, try another.\n");
            return true;
        }
        if (x > 0 && x < 7) {
            if (board[y + 1][x - 1] != 0 && board[y + 1][x + 1] != 0) {
                printf("This checker locked, try another.\n");
                return true;
            }
        }
        else if (x == 0) {
            if (board[y + 1][x + 1] != 0) {
                printf("This checker locked, try another.\n");
                return true;
            }
        }
        else {
            if (board[y + 1][x - 1] != 0)
                printf("This checker locked, try another.\n");
            return true;
        }
        break;
    }

    return false;
}

void out_board(int** board)
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