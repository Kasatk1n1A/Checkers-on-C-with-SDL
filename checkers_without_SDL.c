#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


bool continue_attack(char** board, int x, int y, char player);
bool Win_Check(char** board, char player);
void attack_options(char** board);
void board_cleanup(char** board);
char** add_board(void);
void attack(char** board, char** atack_board, char player);
void attacking(char** board, int x1, int y1, int x2, int y2);
char** attack_check(char** board, char player);
void out_board(char** board);
bool try_to_move(char** board, int x, int y, char player);
void move_checker(char** board, int x1, int y1, int x2, int y2);
void move_without_attack(char** board, char player);
void King_check(char** board, int x, int y, char player);

int main()
{
    char** board = add_board();
    //  a { 0, r, 0, r, 0, r, 0, r } n
    //  b { r, 0, r, 0, r, 0, r, 0 } i
    //  c { 0, r, 0, r, 0, r, 0, r } g
    //  d { 0, 0, 0, 0, 0, 0, 0, 0 } g
    //  e { 0, 0, 0, 0, 0, 0, 0, 0 } e
    //  f { w, 0, w, 0, w, 0, w, 0 } r
    //  g { 0, w, 0, w, 0, w, 0, w } s
    //  h { w, 0, w, 0, w, 0, w, 0 } !
    //      1  2  3  4  5  6  7  8

    board[5][0] = 'W';
    char player = 'w';
    while (true) {
        out_board(board);
        if (player == 'r')
            printf("Red turn.\n");
        else
            printf("White turn.\n");

        //проверка на необходимость атаки
        char** attack_board = attack_check(board, player);
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
            case 'w':
                printf("White won!\n");
                break;
            case 'r':
                printf("Red won!\n");
                break;
            }
            break;
        }
        //смена игрока
        player = (player == 'w') ? 'r' : 'w';
    }

    board_cleanup(board);
    return 0;
}

bool Win_Check(char** board, char player)
{
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++) {
            switch (player) 
            {
            case 'w':
                if (board[i][j] == 'r' || board[i][j] == 'R')
                    return false;
                break;
            case 'r':
                if (board[i][j] == 'w' || board[i][j] == 'R')
                    return false;
                break;
            }
        }

    return true;
}

//Проверяет условие появления дамки
void King_check(char** board, int x, int y, char player)
{
    if (!board[y][x])
        return;
    switch (player) 
    {
        //Превращает белую шашку в дамку
    case 'w':
        if (y == 0) {
            board[y][x] = 'W';
        }
        break;
        //Превращает красную шашку в дамку
    case 'r':
        if (y == 7) {
            board[y][x] = 'R';
        }
        break;
    default:
        return;
    }
}

//Делает обычный ход, никого не съедая
void move_without_attack(char** board, char player)
{
    int x1, y1;
    while (true)
    {
        //Выбор фигуры
        printf("\nPick checker:");
        char str[3];
        if (!scanf("%s", str) || !(str[0] >= 'a' && str[0] <= 'h') || !(str[1] >= '1' && str[1] <= '8'))
            exit(EXIT_FAILURE);

        //Ввод координат передвигаемой фигуры
        x1 = str[1] - '1';
        y1 = str[0] - 'a';

        //Проверка на корректность введёных координат и может ли фигура двигаться
        if (x1 < 0 && x1 > 8 || y1 < 0 && y1 > 8) {
            printf("Incorrect, try again.\n");
            continue;
        }
        if (try_to_move(board, x1, y1, player)) {
            continue;
        }
        break;
    }

    int x2, y2;
    //Ожидание выбора куда сходить
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
        if (board[y2][x2]){
            printf("Incorrect move, try again.\n");
            continue;
        }

        switch (player) 
        {   //Надо добавить дамку
        case 'w':
            switch (board[y1][x1])
            {
                case 'w':
                    if ((x2 == x1 - 1 || x2 == x1 + 1) && y2 == y1 - 1){
                        move_checker(board, x1, y1, x2, y2);
                        goto end_move;
                    }
                    break;
                case 'W':
                    for (int n = 1; y1 - n > 0 && x1 - n > 0; n++) {
                        if (y1 - n == y2 && x1 - n == x2) {
                            move_checker(board, x1, y1, x2, y2);
                            goto end_move;
                        }
                        if (board[y1 - n][x1 - n] == 'w' || board[y1 - n][x1 - n] == 'W') {
                            break;
                        }
                    }
                    for (int n = 1; y1 + n > 0 && x1 - n > 0; n++) {
                        if (y1 + n == y2 && x1 - n == x2) {
                            move_checker(board, x1, y1, x2, y2);
                            goto end_move;
                        }
                        if (board[y1 + n][x1 - n] == 'w' || board[y1 + n][x1 - n] == 'W') {
                            break;
                        }
                    }
                    for (int n = 1; y1 - n > 0 && x1 + n > 0; n++) {
                        if (y1 - n == y2 && x1 + n == x2) {
                            move_checker(board, x1, y1, x2, y2);
                            goto end_move;
                        }
                        if (board[y1 - n][x1 + n] == 'w' || board[y1 - n][x1 + n] == 'W') {
                            break;
                        }
                    }
                    for (int n = 1; y1 + n > 0 && x1 + n > 0; n++) {
                        if (y1 + n  == y2 && x1 + n == x2) {
                            move_checker(board, x1, y1, x2, y2);
                            goto end_move;
                        }
                        if (board[y1 + n][x1 + n] == 'w' || board[y1 + n][x1 + n] == 'W') {
                            break;
                        }
                    }
                    break;
            }
        case 'r':
            switch (board[x1][y1])
            {
            case 'r':
                if ((x2 == x1 - 1 || x2 == x1 + 1) && y2 == y1 + 1) {
                    move_checker(board, x1, y1, x2, y2);
                    goto end_move;
                }
                break;
            case 'R':
                for (int n = 1; y1 - n > 0 && x1 - n > 0; n++) {
                    if (y1 - n == y2 && x1 - n == x2) {
                        move_checker(board, x1, y1, x2, y2);
                        goto end_move;
                    }
                    if (board[y1 - n][x1 - n] == 'r' || board[y1 - n][x1 - n] == 'R') {
                        break;
                    }
                }
                for (int n = 1; y1 + n > 0 && x1 - n > 0; n++) {
                    if (y1 + n  == y2 && x1 - n == x2) {
                        move_checker(board, x1, y1, x2, y2);
                        goto end_move;
                    }
                    if (board[y1 + n][x1 - n] == 'r' || board[y1 + n][x1 - n] == 'R') {
                        break;
                    }
                }
                for (int n = 1; y1 - n > 0 && x1 + n > 0; n++) {
                    if (y1 - n  == y2 && x1 + n == x2) {
                        move_checker(board, x1, y1, x2, y2);
                        goto end_move;
                    }
                    if (board[y1 - n][x1 + n] == 'r' || board[y1 - n][x1 + n] == 'R') {
                        break;
                    }
                }
                for (int n = 1; y1 + n > 0 && x1 + n > 0; n++) {
                    if (y1 + n  == y2 && x1 + n == x2) {
                        move_checker(board, x1, y1, x2, y2);
                        goto end_move;
                    }
                    if (board[y1 + n][x1 + n] == 'r' || board[y1 + n][x1 + n] == 'R') {
                        break;
                    }
                }
                break;
            }
        }
        break;
    }
    end_move:
    King_check(board, x2, y2, player);
}

bool continue_attack(char** board, int x, int y, char player)
{
    char** board_of_rub = attack_check(board, player);
    if (board_of_rub == NULL) {
        return false;
    }
    else if (!board_of_rub[y][x]){ //необходимо 2 условия потому что в первом случае доска указывает на NULL и её не надо очищать
        board_cleanup(board_of_rub);
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
void attack(char** board, char** atack_board, char player)
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
        case 'w':
        case 'r':
            if (!((x1 == x2 - 2 && y1 == y2 + 2) || (x1 == x2 + 2 && y1 == y2 + 2) || (x1 == x2 - 2 && y1 == y2 - 2) || (x1 == x2 + 2 && y1 == y2 - 2))) {
                printf("Incorrect, try again.\n");
                continue;
            }
            goto lets_attack;
            break;
        case 'W':   //может можно как-то получше сделать
            if (x2 < x1 && y2 < y1) {
                for (int n = 1; x1 - n > 0 && y1 - n > 0; n++) {
                    if (board[y1 - n][x1 - n] == 'w' || board[y1 - n][x1 - n] == 'W') {
                        break;
                    }
                    if (board[y1 - n][x1 - n] == 'r' || board[y1 - n][x1 - n] == 'R') {
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
                    if (board[y1 - n][x1 + n] == 'w' || board[y1 - n][x1 + n] == 'W') {
                        break;
                    }
                    if (board[y1 - n][x1 + n] == 'r' || board[y1 - n][x1 + n] == 'R') {
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
                    if (board[y1 + n][x1 - n] == 'w' || board[y1 + n][x1 - n] == 'W') {
                        break;
                    }
                    if (board[y1 + n][x1 - n] == 'r' || board[y1 + n][x1 - n] == 'R') {
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
                    if (board[y1 + n][x1 + n] == 'w' || board[y1 + n][x1 + n] == 'W') {
                        break;
                    }
                    if (board[y1 + n][x1 + n] == 'r' || board[y1 + n][x1 + n] == 'R') {
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
        case 'R':
            if (x2 < x1 && y2 < y1) {
                for (int n = 1; x1 - n > 0 && y1 - n > 0; n++) {
                    if (board[y1 - n][x1 - n] == 'r' || board[y1 - n][x1 - n] == 'R') {
                        break;
                    }
                    if (board[y1 - n][x1 - n] == 'w' || board[y1 - n][x1 - n] == 'W') {
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
                    if (board[y1 - n][x1 + n] == 'r' || board[y1 - n][x1 + n] == 'R') {
                        break;
                    }
                    if (board[y1 - n][x1 + n] == 'w' || board[y1 - n][x1 + n] == 'W') {
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
                    if (board[y1 + n][x1 - n] == 'r' || board[y1 + n][x1 - n] == 'R') {
                        break;
                    }
                    if (board[y1 + n][x1 - n] == 'w' || board[y1 + n][x1 - n] == 'W') {
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
                    if (board[y1 + n][x1 + n] == 'r' || board[y1 + n][x1 + n] == 'R') {
                        break;
                    }
                    if (board[y1 + n][x1 + n] == 'w' || board[y1 + n][x1 + n] == 'W') {
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
void attack_options(char** board) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j])
                printf("%c%d\t", 'a' + i, j + 1);
        }
    }
    printf("\n");
}

//Передвигает фигуру с позиции (x1, y1) на позицию  (x2, y2) удаляя шашку между ними
void attacking(char** board, int x1, int y1, int x2, int y2)
{
    move_checker(board, x1, y1, x2, y2);
    switch (board[y2][x2])
    {
    case 'r':
    case 'w':
        board[(y1 + y2) / 2][(x1 + x2) / 2] = 0;
        break;
    case 'R':
    case 'W':
        if (y2 < y1 && x2 < x1) {
            board[y2 + 1][x2 + 1] = 0;
            break;
        }
        if (y2 < y1 && x2 > x1) {
            board[y2 + 1][x2 - 1] = 0;
            break;
        }
        if (y2 > y1 && x2 < x1) {
            board[y2 - 1][x2 + 1] = 0;
            break;
        }
        if (y2 > y1 && x2 > x1) {
            board[y2 - 1][x2 - 1] = 0;
            break;
        }
    }
}

//Проверяет все фигуры player-а на возможность рубить и возвращает указатель на динамический массив массивов, где указаны фигуры которые могут и должны рубить,
//если таковых нет то возвращает NULL
char** attack_check(char** board, char player)
{
    char** board_of_rub = (char**)malloc(sizeof(char*) * 8);
    for (int i = 0; i < 8; i++) {
        board_of_rub[i] = (char*)malloc(sizeof(char) * 8);
        memset(board_of_rub[i], 0, sizeof(char) * 8);
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
    case 'w':
        for (int i = 0; i < 8; i++) 
        {
            for (int j = 0; j < 8; j++) 
            {                        
                //Обычная шашка
                switch (board[i][j]) 
                {
                case 'w':
                    if (j >= 0 && j <= 5) {
                        //Бьёт ли фигура вправо вверх
                        if (i >= 2 && (board[i - 1][j + 1] == 'r' || board[i - 1][j + 1] == 'R') && board[i - 2][j + 2] == 0) {
                            board_of_rub[i][j] = 'w';
                            flag++;
                        }
                        //Бьёт ли фигура вправо вниз
                        if (i <= 5 && (board[i + 1][j + 1] == 'r' || board[i + 1][j + 1] == 'R') && board[i + 2][j + 2] == 0) {
                            board_of_rub[i][j] = 'w';
                            flag++;
                        }
                    }
                    if (j >= 2 && j <= 7) {
                        //Бьёт ли фигура влево вверх
                        if (i >= 2 && (board[i - 1][j - 1] == 'r' || board[i - 1][j - 1] == 'R') && board[i - 2][j - 2] == 0) {
                            board_of_rub[i][j] = 'w';
                            flag++;
                        }
                        //Бьёт ли фигура влево вниз
                        if (i <= 5 && (board[i + 1][j - 1] == 'r' || board[i + 1][j - 1] == 'R') && board[i + 2][j - 2] == 0) {
                            board_of_rub[i][j] = 'w';
                            flag++;
                        }
                    }
                    break;
                //Если фигура - Дамка
                case 'W':
                    //Бьёт ли фигура влево вверх
                    for (int n = 1; i - n > 0 && j - n > 0; n++) {
                        if (board[i - n][j - n] == 'w' || board[i - n][j - n] == 'W') {
                            break;
                        }
                        if ((board[i - n][j - n] == 'r' || board[i - n][j - n] == 'R') && board[i - n - 1][j - n - 1] == 0) {
                            board_of_rub[i][j] = 'W';
                            flag++;
                            break;
                        }
                    }
                    //Бьёт ли фигура вправо вверх
                    for (int n = 1; i - n > 0 && j + n < 8; n++) {
                        if (board[i - n][j + n] == 'w' || board[i - n][j + n] == 'W') {
                            break;
                        }
                        if ((board[i - n][j + n] == 'r' || board[i - n][j + n] == 'R') && board[i - n - 1][j + n + 1] == 0) {
                            board_of_rub[i][j] = 'W';
                            flag++;
                            break;
                        }
                    }
                    //Бьёт ли фигура влево вниз
                    for (int n = 1; i + n < 8 && j - n > 0; n++) {
                        if (board[i + n][j - n] == 'w' || board[i + n][j - n] == 'W') {
                            break;
                        }
                        if ((board[i + n][j - n] == 'r' || board[i + n][j - n] == 'R') && board[i + n + 1][j - n - 1] == 0) {
                            board_of_rub[i][j] = 'W';
                            flag++;
                            break;
                        }
                    }
                    //Бьёт ли фигура вправо вниз
                    for (int n = 1; i + n < 8 && j + n < 8; n++) {
                        if (board[i + n][j + n] == 'w' || board[i + n][j + n] == 'W') {
                            break;
                        }
                        if ((board[i + n][j + n] == 'r' || board[i + n][j + n] == 'R') && board[i + n + 1][j + n + 1] == 0) {
                            board_of_rub[i][j] = 'W';
                            flag++;
                            break;
                        }
                    }
                    break;
                }
            }
        }
        break;
    case 'r':
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (i < 8 && board[i][j] == 'r')
                {
                    if (j >= 0 && j <= 5) {
                        //Бьёт ли фигура вправо вверх
                        if (i >= 2 && (board[i - 1][j + 1] == 'w' || board[i - 1][j + 1] == 'W') && board[i - 2][j + 2] == 0) {
                            board_of_rub[i][j] = 'r';
                            flag++;
                        }
                        //Бьёт ли фигура вправо вниз
                        if (i <= 5 && (board[i + 1][j + 1] == 'w' || board[i + 1][j + 1] == 'W') && board[i + 2][j + 2] == 0) {
                            board_of_rub[i][j] = 'r';
                            flag++;
                        }
                    }
                    if (j >= 2 && j <= 7) {
                        //Бьёт ли фигура влево вверх
                        if (i >= 2 && (board[i - 1][j - 1] == 'w' || board[i - 1][j - 1] == 'W') && board[i - 2][j - 2] == 0) {
                            board_of_rub[i][j] = 'r';
                            flag++;
                        }
                        //Бьёт ли фигура влево вниз
                        if (i <= 5 && (board[i + 1][j - 1] == 'w' || board[i + 1][j - 1] == 'W') && board[i + 2][j - 2] == 0) {
                            board_of_rub[i][j] = 'r';
                            flag++;
                        }
                    }
                    continue;
                }
                //Если фигура - Дамка
                if (board[i][j] == 'R')
                {
                    //Бьёт ли фигура влево вверх
                    for (int n = 1; i - n > 0 && j - n > 0; n++) {
                        if (board[i - n][j - n] == 'r' || board[i - n][j - n] == 'R') {
                            break;
                        }
                        if ((board[i - n][j - n] == 'w' || board[i - n][j - n] == 'W') && board[i - n - 1][j - n - 1] == 0) {
                            board_of_rub[i][j] = 'R';
                            flag++;
                            break;
                        }
                    }
                    //Бьёт ли фигура вправо вверх
                    for (int n = 1; i - n > 0 && j + n < 8; n++) {
                        if (board[i - n][j + n] == 'r' || board[i - n][j + n] == 'R') {
                            break;
                        }
                        if ((board[i - n][j + n] == 'w' || board[i - n][j + n] == 'W') && board[i - n - 1][j + n + 1] == 0) {
                            board_of_rub[i][j] = 'R';
                            flag++;
                            break;
                        }
                    }
                    //Бьёт ли фигура влево вниз
                    for (int n = 1; i + n < 8 && j - n > 0; n++) {
                        if (board[i + n][j - n] == 'r' || board[i + n][j - n] == 'R') {
                            break;
                        }
                        if ((board[i + n][j - n] == 'w' || board[i + n][j - n] == 'W') && board[i + n + 1][j - n - 1] == 0) {
                            board_of_rub[i][j] = 'R';
                            flag++;
                            break;
                        }
                    }
                    //Бьёт ли фигура вправо вниз
                    for (int n = 1; i + n < 8 && j + n < 8; n++) {
                        if (board[i + n][j + n] == 'r' || board[i + n][j + n] == 'R') {
                            break;
                        }
                        if ((board[i + n][j + n] == 'w' || board[i + n][j + n] == 'W') && board[i + n + 1][j + n + 1] == 0) {
                            board_of_rub[i][j] = 'R';
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
char** add_board(void)
{
    char** board = (char**)malloc(sizeof(char*) * 8);       

    //{ 0, r, 0, r, 0, r, 0, r },
    //{ r, 0, r, 0, r, 0, r, 0 },
    //{ 0, r, 0, r, 0, r, 0, r },
    //{ 0, 0, 0, 0, 0, 0, 0, 0 },
    //{ 0, w, 0, 0, 0, 0, 0, 0 },
    //{ w, 0, w, 0, w, 0, w, 0 },
    //{ 0, w, 0, w, 0, w, 0, w },
    //{ w, 0, w, 0, w, 0, w, 0 },

    for (int i = 0; i < 8; i++){
        board[i] = (char*)malloc(sizeof(char) * 8);
        memset(board[i], 0, sizeof(char) * 8);
    }

    for (int i = 1; i < 8; i += 2)
        board[0][i] = 'r';
    for (int i = 0; i < 8; i += 2)
        board[1][i] = 'r';
    for (int i = 1; i < 8; i += 2)
        board[2][i] = 'r';

    for (int i = 0; i < 8; i += 2)
        board[5][i] = 'w';
    for (int i = 1; i < 8; i += 2)
        board[6][i] = 'w';
    for (int i = 0; i < 8; i += 2)
        board[7][i] = 'w';

    return board;
}

//удаляет массив из динамических массивов (игральную доску)
void board_cleanup(char** board)
{
    for (int i = 0; i < 8; i++) {
        free(board[i]);
    }
    free(board);
}

//передвигает фигуру с позиции x1 y1 на позицию x2 y2
void move_checker(char** board, int x1, int y1, int x2, int y2)
{
    board[y2][x2] = board[y1][x1];
    board[y1][x1] = 0;
}
//Попытка фигуры двигаться без съедания, возвращает false если невозможно
bool try_to_move(char** board, int x, int y, char player)
{
    switch (player)
    {
    case ('w'):
        if (!(board[y][x] == 'w' || board[y][x] == 'W')) {
            printf("Incorrect figure, try another.\n");
            return true;
        }
        if (x > 0 && x < 7) {   //проверка ходьбы влево вверх и вправо вверх когда шашка не на краю доски
            if (board[y - 1][x - 1] != 0 && board[y - 1][x + 1] != 0) {
                printf("This checker locked, try another.\n");
                return true;
            }
        }
        else if (x == 0) {  //тут если на левом краю доски
            if (board[y - 1][x + 1] != 0) {
                printf("This checker locked, try another.\n");
                return true;
            }
        }
        else {  //тут если на правом краю
            if (board[y - 1][x - 1] != 0)
                printf("This checker locked, try another.\n");
            return true;
        }
        break;
    case ('r'): //тут всё аналогично белой фигуре но вниз
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
//Вывод доски в терминал
void out_board(char** board)
{
    for (int i = 0; i < 8; i++) {
        printf("%c|\t", 'a' + i);
        for (int j = 0; j < 8; j++) {
            if (board[i][j])
                printf("%c\t", board[i][j]);
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