#include "moves.h"
//----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$

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

bool CanContinue(CH_Type** board, int x, int y, Player player)
{
    return CanCheckerAttack(board, x, y, player == WHITE ? true : false); 
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
 * Проверяет, будет ли взята вражеская шашка при ходе с (x1,y1) на (x2,y2)
 * 
 * @param board Игровая доска 8x8
 * @param x1,y1 Начальная позиция шашки
 * @param x2,y2 Конечная позиция шашки
 * @param player Текущий игрок (WHITE или RED)
 * 
 * @return true - если будет взятие, false - если ход без взятия
 */
bool isCaptureMove(CH_Type** board, int x1, int y1, int x2, int y2, Player player)
{
    // 2. Определяем типы фигур
    CH_Type our_pawn = (player == WHITE) ? PICKED_WHITE_PAWN : PICKED_RED_PAWN;
    CH_Type our_king = (player == WHITE) ? PICKED_WHITE_KING : PICKED_RED_KING;
    CH_Type enemy_pawn = (player == WHITE) ? RED_PAWN : WHITE_PAWN;
    CH_Type enemy_king = (player == WHITE) ? RED_KING : WHITE_KING;

    // 5. Вычисляем направление движения
    int dx = x2 - x1;
    int dy = y2 - y1;
    int stepX = (dx > 0) ? 1 : -1;
    int stepY = (dy > 0) ? 1 : -1;

    // 6. Для обычной шашки проверяем прыжок через одну клетку
    if (board[y1][x1] == our_pawn) 
    {
        if (abs(dx) == 2 && abs(dy) == 2) 
        {
            int midX = x1 + stepX;
            int midY = y1 + stepY;
            // Проверяем, что между нами и целью - вражеская фигура
            if (board[midY][midX] == enemy_pawn || board[midY][midX] == enemy_king)
                return true;
        }
        return false;
    }
    // 7. Для дамки проверяем все клетки по пути
    else if (board[y1][x1] == our_king) 
    {
        int x = x1 + stepX;
        int y = y1 + stepY;

        while (x != x2 && y != y2) 
        {
            if (board[y][x] != EMPTY) 
                if (board[y][x] == enemy_pawn || board[y][x] == enemy_king) 
                    return true;

            x += stepX;
            y += stepY;
        }

    }

    return false;
}

/*
* Выполняет ход со взятием фигур противника с возможностью множественных взятий
* @param board Игровая доска
* @param attack_board Матрица возможных взятий
* @param player Текущий игрок (WHITE/RED)
*/
void executeCaptureMove(Window* window, CH_Type** board, bool** attack_board, Player player, Board* CheckersBoard, double Time)
{
    SDL_Texture* background = IMG_LoadTexture(window->renderer, "assets/images/Main/Kover.png");
    if (!background) 
    {
        fprintf(stderr, "Error creating Texture: %s\n", IMG_GetError());
        return;
    }

    // 1. Отображаем доступные варианты взятия
    showCaptureOptions(attack_board);

    int fromX, fromY, toX, toY;  // Координаты начальной и конечной позиции

    // 2. Выбор шашки для хода
    while (true) 
    {        
        // Чтение и проверка формата ввода
        if (CheckerCoordinates(window, CheckersBoard, Time, player, &fromX, &fromY) != MOUSE_LEFT)
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
    highlightChecker(board, fromX, fromY);
    // 4. Основной цикл выполнения хода (с возможностью продолжения взятий)
    bool FirstMove = true;
    while (true) 
    {
        // Ввод целевой позиции
        renderBoardFrame(window, CheckersBoard, background);
        Choice c = CheckerCoordinates(window, CheckersBoard, Time, player, &toX, &toY);
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
        if (!canCheckerMove(fromX, fromY, toX, toY, isKing, player, board))
        {
            printf("This move violates game rules.\n");
            continue;
        }

        if (!isCaptureMove(board, fromX, fromY, toX, toY, player))
        {
            printf("This move violates game rules.\n");
            continue;
        }
            
        // 5. Выполнение взятия
        performCapture(board, fromX, fromY, toX, toY);
        unhighlightChecker(board, toX, toY);

        FirstMove = false;
        // 6. Проверка возможности продолжения взятия
        if (CanContinue(board, toX, toY, player)) 
        {
            fromX = toX;  // Продолжаем с новой позиции
            fromY = toY;
            highlightChecker(board, fromX, fromY);
        }
        else
            break;  // Завершение хода
    }
    unhighlightChecker(board, toX, toY);
}

//----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$

/*
 * Проверяет, заблокирована ли шашка (нет возможных ходов или взятий)
 * @param board Игровая доска 8x8
 * @param x,y Координаты проверяемой шашки
 * @param canPawnCaptureBackward Может ли обычная шашка рубить назад
 * @return true если шашка заблокирована, false если есть возможные ходы
 */
bool isCheckerBlocked(CH_Type** board, int x, int y)
{
    CH_Type current = board[y][x];
    
    // Пустая клетка не может быть заблокирована
    if (current == EMPTY) 
        return true;
    
    bool isWhite = (current == WHITE_PAWN || current == WHITE_KING);
    bool isKing = (current == WHITE_KING || current == RED_KING);

    // Все возможные направления движения
    int directions[4][2] = {{1,1}, {1,-1}, {-1,1}, {-1,-1}};
    int dirCount = isKing ? 4 : 2; // Для обычных шашек только 2 направления (вперед)
    
    for (int i = 0; i < dirCount; i++) {
        int dy = directions[i][0];
        int dx = directions[i][1];
        
        // Для обычных шашек корректируем направление
        if (!isKing) {
            // Основное направление движения (вперед)
            dy = isWhite ? -1 : 1;
        }
        
        int nx = x + dx;
        int ny = y + dy;
        
        // Проверяем возможность обычного хода
        if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
            // Если клетка свободна - шашка не заблокирована
            if (board[ny][nx] == EMPTY) {
                return false;
            }
            
            // Проверяем возможность взятия
            if (nx + dx >= 0 && nx + dx < 8 && ny + dy >= 0 && ny + dy < 8) {
                CH_Type target = board[ny][nx];
                bool isEnemy = (isWhite && (target == RED_PAWN || target == RED_KING)) ||
                               (!isWhite && (target == WHITE_PAWN || target == WHITE_KING));
                
                if (isEnemy && board[ny + dy][nx + dx] == EMPTY) {
                    return false; // Есть возможность взятия
                }
            }
        }
    }
    
    // Если ни один ход невозможен - шашка заблокирована
    printf("123412341234\n");
    return true;
}

/*
 * Выполняет обычный ход без взятия фигур противника
 * @param board Игровая доска
 * @param player Текущий игрок (WHITE/RED)
 */
void executeRegularMove(Window* window, CH_Type** board, Player player, Board* CheckersBoard, double Time)
{
    SDL_Texture* background = IMG_LoadTexture(window->renderer, "assets/images/Main/Kover.png");
    if (!background) 
    {
        fprintf(stderr, "Error creating Texture: %s\n", IMG_GetError());
        return;
    }

    int fromX, fromY; // Координаты исходной позиции
    
    // 1. Фаза выбора шашки для хода
    while (true)
    {
        // Ввод координат шашки
        if (CheckerCoordinates(window, CheckersBoard, Time, player, &fromX, &fromY) != MOUSE_LEFT)
            continue;

        printf("%d %d\n", fromX, fromY);

        // Проверка корректности координат
        if (fromX < 0 || fromX > 7 || fromY < 0 || fromY > 7) {
            continue;
        }
        
        if (board[fromY][fromX] != (player == WHITE ? WHITE_PAWN : RED_PAWN) && board[fromY][fromX] != (player == WHITE ? WHITE_KING : RED_KING))
        {
            printf("Not your\n");
            continue;
        }
        // Проверка, что шашка не заблокирована
        if (isCheckerBlocked(board, fromX, fromY)) 
        {
            printf("Selected checker cannot move. Choose another.\n");
            continue;
        }
        break;
    }

    // 2. Визуальное выделение выбранной шашки
    highlightChecker(board, fromX, fromY);
    renderBoardFrame(window, CheckersBoard, background);
    // 3. Фаза выбора целевой позиции
    int toX, toY;
    while (true) 
    {
        // Ввод координат для хода

        if (CheckerCoordinates(window, CheckersBoard, Time, player, &toX, &toY) != MOUSE_LEFT)
            continue;

        printf("%d %d\n", toX, toY);
        
        // Проверка границ доски
        if (toX < 0 || toX > 7 || toY < 0 || toY > 7)
            continue;
        
        // Проверка, что целевая клетка свободна
        if (board[toY][toX] != EMPTY)
            continue;

        // Проверка правильности хода
        bool isKing = (board[fromY][fromX] == PICKED_WHITE_KING || board[fromY][fromX] == PICKED_RED_KING);
        if(!canCheckerMove(fromX, fromY, toX, toY, isKing, player, board))
        {
            printf("This move is not allowed by game rules.\n");
            continue;
        }
        
        // 4. Выполнение хода
        performCapture(board, fromX, fromY, toX, toY);

        // 5. Снятие визуального выделения
        unhighlightChecker(board, toX, toY);
        renderBoardFrame(window, CheckersBoard, background);
        break;
    }
}

/*
 * Проверяет допустимость хода шашки с учетом всех правил
 * @param x1,y1 Начальные координаты шашки
 * @param x2,y2 Целевые координаты
 * @param isKing Флаг, является ли шашка дамкой
 * @param color Цвет шашки (WHITE/RED)
 * @param board Игровая доска 8x8
 * @return true - ход допустим, false - нарушены правила
 */
bool canCheckerMove(int x1, int y1, int x2, int y2, bool isKing, Player color, CH_Type** board)
{
    printf(color == WHITE ? "WHITE\n" : "RED\n");
    /***********************
     * Базовые проверки
     ***********************/
    
    // Проверка выхода за границы доски
    if (x1 < 0 || x1 >= 8 || y1 < 0 || y1 >= 8 || 
        x2 < 0 || x2 >= 8 || y2 < 0 || y2 >= 8) {
        return false; // Координаты вне доски
    }

    // Клетка назначения должна быть пуста
    if (board[y2][x2] != EMPTY) {
        return false; // Целевая клетка занята
    }

    // Вычисляем смещение по осям
    int dx = x2 - x1;
    int dy = y2 - y1;
    printf("dx: %d  dy: %d\n", dx, dy);
    /***********************
     * Логика для обычных шашек
     ***********************/
    if (!isKing) 
    {
        // Определяем основное направление движения:
        // Белые шашки ходят вверх (dy = -1), красные - вниз (dy = 1)
        int forward = (color == WHITE) ? -1 : 1;

        // Проверка обычного хода (без взятия)
        // Должно быть: смещение на 1 клетку по диагонали вперед
        if (dy == forward && abs(dx) == 1) {
            return true; // Стандартный ход допустим
        }

        // Проверка хода со взятием (через одну клетку)
        // Должно быть: смещение на 2 клетки по диагонали
        if (abs(dx) == 2 && abs(dy) == 2) {
            // Координаты промежуточной клетки
            int mid_x = (x1 + x2) / 2;
            int mid_y = (y1 + y2) / 2;

            // Проверяем, что между позициями находится вражеская шашка
            bool isEnemy = (color == WHITE && board[mid_y][mid_x] < 0) || // Для белых - красные шашки
                          (color == RED && board[mid_y][mid_x] > 0);      // Для красных - белые шашки
            
            if (isEnemy) {
                return true; // Взятие допустимо
            }
        }
    }
    /***********************
     * Логика для дамок
     ***********************/
    else 
    {
        // Дамка может ходить по диагонали на любое расстояние
        // Проверяем движение строго по диагонали
        if (abs(dx) == abs(dy)) {
            // Определяем шаг движения
            int step_x = (dx > 0) ? 1 : -1;
            int step_y = (dy > 0) ? 1 : -1;
            
            // Текущие координаты при движении
            int x = x1 + step_x;
            int y = y1 + step_y;
            
            int enemy_count = 0; // Счетчик встреченных вражеских шашек

            // Проверяем все клетки по пути
            while (x != x2 && y != y2) {
                CH_Type cell = board[y][x]; // Фигура в текущей клетке
                
                if (cell != EMPTY) {
                    // Проверка на "свои" фигуры
                    if ((color == WHITE && cell > 0) ||  // Для белых - белые фигуры
                        (color == RED && cell < 0)) {    // Для красных - красные фигуры
                        return false; // Нельзя перепрыгивать свои фигуры
                    }
                    
                    // Фигура противника
                    enemy_count++;
                    
                    // Дамка может перепрыгнуть только одну фигуру за ход
                    if (enemy_count > 1) {
                        return false; // Найдено более одной вражеской фигуры
                    }
                }
                
                // Переход к следующей клетке
                x += step_x;
                y += step_y;
            }

            // Дамка может:
            // 1. Ходить без взятия (enemy_count == 0)
            // 2. Взять одну фигуру (enemy_count == 1)
            return (enemy_count <= 1);
        }
    }

    // Если ни одно из условий не выполнилось - ход невозможен
    return false;
}

//----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$

bool CheckerCoordinates(Window* window, Board* CheckersBoard, double Time, Player player, int* x, int* y)
{
    Choice c = get_mouse_click(x, y, window, CheckersBoard, Time, player);
    if (c == MOUSE_LEFT)
    {
        printf("%d %d\n", *x, *y);
        *x = (*x - (239 + 43)) / 105;
        *y = (*y - 41) / 105;
    }
    return c;
}

/*
 * Выполняет взятие (рубку) шашки, включая:
 * - Перемещение шашки
 * - Удаление побитых фигур
 * - Превращение в дамку при достижении последней линии
 * - Обновление графического представления
 * 
 * @param board Указатель на игровую доску
 * @param fromX,fromY Начальные координаты шашки
 * @param toX,toY Конечные координаты после взятия
 */
void performCapture(CH_Type** board, int fromX, int fromY, int toX, int toY)
{    
    // 2. Определяем направление движения
    int dx = (toX > fromX) ? 1 : -1;  // Шаг по X (1 вправо, -1 влево)
    int dy = (toY > fromY) ? 1 : -1;  // Шаг по Y (1 вниз, -1 вверх)
    
    // 3. Сохраняем исходное состояние
    int x = fromX;  // Текущая позиция X
    int y = fromY;  // Текущая позиция Y
    CH_Type movingPiece = board[y][x];  // Сохраняем тип перемещаемой фигуры
    board[y][x] = EMPTY;  // Очищаем стартовую позицию

    // 4. Обрабатываем путь перемещения
    while (x != toX || y != toY) {  // Пока не достигли цели
        x += dx;  // Двигаемся по X
        y += dy;  // Двигаемся по Y
        
        // Если встретили непустую клетку (вражескую фигуру)
        if (board[y][x] != EMPTY) {
            // 4.1. Удаляем вражескую фигуру с доски
            board[y][x] = EMPTY;
            
            // 4.3. Перепрыгиваем через побитую фигуру
            x += dx;
            y += dy;
        }
    }

    // 5. Проверяем превращение в дамку
    if (movingPiece == PICKED_WHITE_PAWN && toY == 0) {
        // Белая пешка достигла последней линии (нижний край доски)
        movingPiece = PICKED_WHITE_KING;
    }
    else if (movingPiece == PICKED_RED_PAWN && toY == 7) {
        // Красная пешка достигла последней линии (верхний край доски)
        movingPiece = PICKED_RED_KING;
    }
    
    // 6. Устанавливаем фигуру на новую позицию
    board[toY][toX] = movingPiece;
}