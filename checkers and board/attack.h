#ifndef ATTACK
#define ATTACK

#include "board_visual.h"
#include "StructsAndEnum.h"
#include "move.h"

//----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$

void showCaptureOptions(bool** board);
bool** canCapture(CH_Type** board, bool isWhiteTurn);
bool CanContinue(CH_Type** board, int x, int y, Player player);
bool CanCheckerAttack(CH_Type** board, int x, int y, bool isWhite);
void executeCaptureMove(CH_Type** board, bool** attack_board, Player player, Board* CheckersBoard);

//----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$

Game* game;

//----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$

bool CanCheckerAttack(CH_Type** board, int x, int y, bool isWhite)
{
    CH_Type ourPawn = isWhite ? WHITE_PAWN : RED_PAWN;    // Наши пешки
    CH_Type ourKing = isWhite ? WHITE_KING : RED_KING;     // Наши дамки
    CH_Type enemyPawn = isWhite ? RED_PAWN : WHITE_PAWN;   // Пешки противника
    CH_Type enemyKing = isWhite ? RED_KING : WHITE_KING;   // Дамки противника
    out_board(board);
    CH_Type current = board[y][x];  // Фигура в текущей клетке
    const int directions[4][2] = {
        { 1, 1},   // Вправо-вниз
        { 1,-1},   // Вправо-вверх
        {-1, 1},   // Влево-вниз
        {-1,-1}    // Влево-вверх
    };
    // Проверяем только свои фигуры
    if (current == ourPawn) 
    {
        // Определяем количество направлений для проверки
        int dirCount = 4;  // Дамки ходят во всех направлениях
         // Проверяем все допустимые направления
        for (int d = 0; d < dirCount; d++) 
        {
            int dx = directions[d][0];  // Смещение по X
            int dy = directions[d][1];  // Смещение по Y 

            // Координаты соседней клетки и клетки за ней
            int nx = x + dx;  // X соседа
            int ny = y + dy;  // Y соседа
            int jx = x + 2*dx; // X клетки за соседом
            int jy = y + 2*dy; // Y клетки за соседом
            printf("%d %d %d %d\n", nx, ny, jx, jy);
            // Проверка выхода за границы доски
            if (nx < 0 || nx >= 8 || ny < 0 || ny >= 8 ||  // Соседняя клетка
                jx < 0 || jx >= 8 || jy < 0 || jy >= 8)    // Клетка за соседом
                continue;

            // Проверка условий для взятия:
            // 1. В соседней клетке вражеская фигура
            // 2. Клетка за ней свободна
            if (board[ny][nx] != EMPTY && board[jy][jx] == EMPTY) 
            {
               CH_Type between = board[ny][nx];  // Фигура между нами и целью

               // Проверяем, что это фигура противника
               if (between == enemyPawn || between == enemyKing) 
                   return true;
            }
        }
    }
    else if (current == ourKing)  // Если текущая фигура - наша дамка
    {
        // Специальная логика для дамок
        for (int d = 0; d < 4; d++)  // Проверяем все 4 диагональных направления
        {
            int dx = directions[d][0];  // Получаем смещение по X для текущего направления
            int dy = directions[d][1];  // Получаем смещение по Y для текущего направления
            int nx = x + dx;  // Начальная позиция проверки по X
            int ny = y + dy;  // Начальная позиция проверки по Y
            
            // Ищем первую вражескую фигуру в данном направлении
            while (nx >= 0 && nx < 8 && ny >= 0 && ny < 8)  // Пока не вышли за границы доски
            {
                if (board[ny][nx] != EMPTY)  // Если клетка не пустая
                {
                    // Проверяем, что это вражеская фигура
                    if (board[ny][nx] == enemyPawn || board[ny][nx] == enemyKing) 
                    {
                        // Проверяем клетку за вражеской фигурой
                        int jx = nx + dx;  // X-позиция за вражеской фигурой
                        int jy = ny + dy;  // Y-позиция за вражеской фигурой
                        
                        // Если клетка за врагом существует и пуста - можно рубить
                        if (jx >= 0 && jx < 8 && jy >= 0 && jy < 8 && board[jy][jx] == 0) 
                        {
                            return true;    // Устанавливаем флаг наличия взятий
                        }
                    }
                    break; // Прерываем поиск после нахождения любой фигуры (дальше нельзя перепрыгнуть)
                }
                nx += dx;  // Продолжаем движение по диагонали
                ny += dy;  // пока не найдем фигуру или край доски
            }
        }
    }

    return false;
}




/*
* Проверяет, может ли игрок продолжить серию взятий (атаку) после хода шашки.
* 
* @param board Игровая доска (двумерный массив)
* @param x Текущая x-координата шашки
* @param y Текущая y-координата шашки
* @param player Текущий игрок (WHITE или RED)
* @return true - если можно продолжить атаку, false - если ход завершён
*/
bool CanContinue(CH_Type** board, int x, int y, Player player)
{
    return CanCheckerAttack(board, x, y, player == WHITE ? true : false); 
}

/*
* Выполняет ход со взятием фигур противника с возможностью множественных взятий
* @param board Игровая доска
* @param attack_board Матрица возможных взятий
* @param player Текущий игрок (WHITE/RED)
*/
void executeCaptureMove(CH_Type** board, bool** attack_board, Player player, Board* CheckersBoard)
{
    // 1. Отображаем доступные варианты взятия
    showCaptureOptions(attack_board);

    char input[3];  // Буфер для ввода координат (формат "a1", "h8" и т.д.)
    int fromX, fromY, toX, toY;  // Координаты начальной и конечной позиции

    // 2. Выбор шашки для хода
    while (true) 
    {        
        // Чтение и проверка формата ввода
        if (CheckerCoordinates(game, &fromX, &fromY) != MOUSE_LEFT)
            continue;
        printf("%d %d\n", fromX, fromY);
        // Проверка допустимости выбора
        if (fromX < 0 || fromX >= 8 || fromY < 0 || fromY >= 8) {
            continue;
        }

        if (board[fromY][fromX] != (player == WHITE ? WHITE_PAWN : RED_PAWN) && board[fromY][fromX] != (player == WHITE ? WHITE_KING : RED_KING))
            continue;

        if (!attack_board[fromY][fromX]) {
            continue;
        }
        break;
    }

    // 3. Визуально выделяем выбранную шашку
    highlightChecker(game, fromX, fromY, board);

    // 4. Основной цикл выполнения хода (с возможностью продолжения взятий)
    bool FirstMove = true;
    while (true) 
    {
        // Ввод целевой позиции
        Choice c = CheckerCoordinates(game, &toX, &toY);
        printf("%d\n", c);
        if (c == ENTER && !FirstMove)
            break;

        if (c != MOUSE_LEFT)
            continue;
        
        printf("%d %d\n", toX, toY);
        // Проверка целевой клетки
        if (board[toY][toX] != EMPTY) {
            continue;
        }
        if (toX < 0 || toX >= 8 || toY < 0 || toY >= 8) {
            continue;
        }
        
        // Проверка правил перемещения
        bool isKing = (board[fromY][fromX] == PICKED_WHITE_KING || board[fromY][fromX] == PICKED_RED_KING);
        if(!canCheckerMove(fromX, fromY, toX, toY, board[fromY][fromX] == PICKED_WHITE_KING || board[fromY][fromX] == PICKED_RED_KING ? true : false, player, board))
        {
            printf("This move violates game rules.\n");
            continue;
        }
            
        // 5. Выполнение взятия
        performCapture(board, fromX, fromY, toX, toY);
        unhighlightChecker(game, board, toX, toY);

        FirstMove = false;
        // 6. Проверка возможности продолжения взятия
        if (CanContinue(board, toX, toY, player)) 
        {
            fromX = toX;  // Продолжаем с новой позиции
            fromY = toY;
            highlightChecker(game, fromX, fromY, board);
        }
        else
            break;  // Завершение хода
    }
    unhighlightChecker(game, board, toX, toY);
}

//Вывод вариантов хода при необходимости атаки
void showCaptureOptions(bool** board)
{
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j])
                printf("%c%d\t", 'a' + i, j + 1);
        }
    }
    printf("\n");
}



/*
* Проверяет возможность взятия (рубки) фигур противника для всех шашек текущего игрока.
* Возвращает матрицу 8x8, где true указывает на возможность взятия с данной позиции.
* Если возможных взятий нет, возвращает NULL (с предварительным освобождением памяти).
* @param board Двумерный массив 8x8, представляющий текущее состояние доски
* @param isWhiteTurn Флаг, указывающий, ходят ли белые (true) или красные (false)
* @return Указатель на матрицу возможных взятий или NULL, если взятий нет
*/
bool** canCapture(CH_Type** board, bool isWhiteTurn) 
{
    // Настройки правил игры
    bool canPawnCaptureBackward = true;  // Разрешена ли рубка назад для обычных шашек
    bool hasAnyCapture = false;          // Флаг наличия хотя бы одного возможного взятия

    // Выделение памяти под матрицу результатов (8x8)
    bool** result = (bool**)malloc(8 * sizeof(bool*));
    for (int i = 0; i < 8; i++)
        result[i] = (bool*)calloc(8, sizeof(bool));  // Инициализируем нулями

    // Определяем типы фигур для текущего игрока и противника
    CH_Type ourPawn = isWhiteTurn ? WHITE_PAWN : RED_PAWN;    // Наши пешки
    CH_Type ourKing = isWhiteTurn ? WHITE_KING : RED_KING;     // Наши дамки
    CH_Type enemyPawn = isWhiteTurn ? RED_PAWN : WHITE_PAWN;   // Пешки противника
    CH_Type enemyKing = isWhiteTurn ? RED_KING : WHITE_KING;   // Дамки противника
    // Все возможные направления движения (4 диагональных направления)
    const int directions[4][2] = {
        { 1, 1},   // Вправо-вниз
        { 1,-1},   // Вправо-вверх
        {-1, 1},   // Влево-вниз
        {-1,-1}    // Влево-вверх
    };

    // Перебираем все клетки доски
    for (int y = 0; y < 8; y++) 
    {
        for (int x = 0; x < 8; x++) 
        {
            CH_Type current = board[y][x];  // Фигура в текущей клетке
            
            // Проверяем только свои фигуры
            if (current == ourPawn) 
            {
                // Определяем количество направлений для проверки
                int dirCount = 4;  // Дамки ходят во всех направлениях
                int forwardDir = isWhiteTurn ? -1 : 1;        // Направление "вперед" для текущего игрока

                // Проверяем все допустимые направления
                for (int d = 0; d < dirCount; d++) 
                {
                    int dx = directions[d][0];  // Смещение по X
                    int dy = directions[d][1];  // Смещение по Y

                    // Координаты соседней клетки и клетки за ней
                    int nx = x + dx;  // X соседа
                    int ny = y + dy;  // Y соседа
                    int jx = x + 2*dx; // X клетки за соседом
                    int jy = y + 2*dy; // Y клетки за соседом

                    // Проверка выхода за границы доски
                    if (nx < 0 || nx >= 8 || ny < 0 || ny >= 8 ||  // Соседняя клетка
                        jx < 0 || jx >= 8 || jy < 0 || jy >= 8)    // Клетка за соседом
                        continue;

                    // Проверка условий для взятия:
                    // 1. В соседней клетке вражеская фигура
                    // 2. Клетка за ней свободна
                    if (board[ny][nx] != EMPTY && board[jy][jx] == EMPTY) 
                    {
                        CH_Type between = board[ny][nx];  // Фигура между нами и целью
                        
                        // Проверяем, что это фигура противника
                        if (between == enemyPawn || between == enemyKing) 
                        {
                            result[y][x] = true;    // Помечаем возможность взятия
                            hasAnyCapture = true;    // Устанавливаем флаг наличия взятий
                            break;                  // Переходим к следующей клетке
                        }
                    }
                }
            }
            else if (current == ourKing)  // Если текущая фигура - наша дамка
            {
                // Специальная логика для дамок
                for (int d = 0; d < 4; d++)  // Проверяем все 4 диагональных направления
                {
                    int dx = directions[d][0];  // Получаем смещение по X для текущего направления
                    int dy = directions[d][1];  // Получаем смещение по Y для текущего направления
                    int nx = x + dx;  // Начальная позиция проверки по X
                    int ny = y + dy;  // Начальная позиция проверки по Y
                    
                    // Ищем первую вражескую фигуру в данном направлении
                    while (nx >= 0 && nx < 8 && ny >= 0 && ny < 8)  // Пока не вышли за границы доски
                    {
                        if (board[ny][nx] != EMPTY)  // Если клетка не пустая
                        {
                            // Проверяем, что это вражеская фигура
                            if (board[ny][nx] == enemyPawn || board[ny][nx] == enemyKing) 
                            {
                                // Проверяем клетку за вражеской фигурой
                                int jx = nx + dx;  // X-позиция за вражеской фигурой
                                int jy = ny + dy;  // Y-позиция за вражеской фигурой
                                
                                // Если клетка за врагом существует и пуста - можно рубить
                                if (jx >= 0 && jx < 8 && jy >= 0 && jy < 8 && board[jy][jx] == 0) 
                                {
                                    result[y][x] = true;      // Помечаем возможность взятия
                                    hasAnyCapture = true;     // Устанавливаем флаг наличия взятий
                                }
                            }
                            break; // Прерываем поиск после нахождения любой фигуры (дальше нельзя перепрыгнуть)
                        }
                        nx += dx;  // Продолжаем движение по диагонали
                        ny += dy;  // пока не найдем фигуру или край доски
                    }
                }
            }
        }
    }

    // Обработка результата
    if (!hasAnyCapture) 
    {
        freeBoard((void**)result);  // Освобождаем память, если взятий нет
        return NULL;
    }

    return result;  // Возвращаем матрицу возможных взятий
}

#endif