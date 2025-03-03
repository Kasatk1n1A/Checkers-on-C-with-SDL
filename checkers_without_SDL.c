#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
    board[2][3] = 0;
    board[3][2] = 'r';

    char player = 'w';
    while (true) {
        out_board(board);
        if (player == 'r')
            printf("Red turn.\n");
        else
            printf("White turn\n");

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

        //смена игрока
        player = (player == 'w') ? 'r' : 'w';
    }

    board_cleanup(board);
    return 0;
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

    //Ожидание выбора куда сходить
    while (true) 
    {
       //Ввод координат куда двигать фигуру
        printf("Where to move:");
        char str[3];
        if (!scanf("%s", str) || !(str[0] >= 'a' && str[0] <= 'h') || !(str[1] >= '1' && str[1] <= '8'))
            exit(EXIT_FAILURE);

        int x2 = str[1] - '1';
        int y2 = str[0] - 'a';

        if (x2 < 0 && x2 > 7 || y2 < 0 && y2 > 7) {
            printf("Incorrect move, try again.\n");
            continue;
        }

        switch (player) {
        case 'w':
           if ((x2 == x1 - 1 || x2 == x1 + 1) && y2 == y1 - 1)
                move_checker(board, x1, y1, x2, y2);
            break;
        case 'r':
            if ((x2 == x1 - 1 || x2 == x1 + 1) && y2 == y1 + 1)
               move_checker(board, x1, y1, x2, y2);
            break;
        }
        King_check(board, x2, y2, player);
        break;
    }
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
        x1 = str[1] - '1';
        y1 = str[0] - 'a';

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
        x2 = str[1] - '1';
        y2 = str[0] - 'a';

        //Проверка на корректность введёных координат (x1, y1), если некорректно, то попробовать снова
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
            if (!((x1 == x2 - 2 && y1 == y2 + 2) || (x1 == x2 + 2 && y1 == y2 + 2))) {
                printf("Incorrect, try again.\n");
                continue;
            }
            goto lets_attack;
            break;
        case 'r':
            if (!((x1 == x2 - 2 && y1 == y2 - 2) || (x1 == x2 + 2 && y1 == y2 - 2))) {
                printf("Incorrect, try again.\n");
                continue;
            }
            goto lets_attack;
            break;
        case 'W':
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
            if (x2 < x1&& y2 < y1) {
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
    }

    lets_attack:
    attacking(board, x1, y1, x2, y2);
    King_check(board, x2, y2, player);
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
                if (i >= 2 && board[i][j] == 'w') 
                {
                    //Бьёт ли фигура вправо вверх
                    if (j >= 0 && j <= 5) {
                        if ((board[i - 1][j + 1] == 'r' || board[i - 1][j + 1] == 'R') && board[i - 2][j + 2] == 0) {
                            board_of_rub[i][j] = 'w';
                            flag++;
                        }
                    }
                    //Бьёт ли фигура влево вверх
                    if (j >= 2 && j <= 7) {
                        if ((board[i - 1][j - 1] == 'r' || board[i - 1][j - 1] == 'R') && board[i - 2][j - 2] == 0) {
                            board_of_rub[i][j] = 'w';
                            flag++;
                        }
                    }
                }
                //Если фигура - Дамка
                if (board[i][j] == 'W') 
                {
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
                    if (j >= 0 && j < 6) {
                        if ((board[i + 1][j + 1] == 'w' || board[i + 1][j + 1] == 'W') && board[i + 2][j + 2] == 0) {
                            board_of_rub[i][j] = 'r';
                            flag++;
                        }
                    }
                    //Бьёт ли фигура влево вверх
                    if (j > 1 && j <= 7) {
                        if ((board[i + 1][j - 1] == 'w' || board[i + 1][j - 1] == 'W') && board[i + 2][j - 2] == 0) {
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
    char f = board[y1][x1];
    board[y1][x1] = 0;
    board[y2][x2] = f;
}

bool try_to_move(char** board, int x, int y, char player)
{
    if (board[y][x] != player) {
        printf("Incorrect figure, try another.\n");
        return true;
    }
    switch (player)
    {
    case ('w'):
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
    case ('r'):
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