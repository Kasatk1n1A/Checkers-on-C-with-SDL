#ifndef MOVE
#define MOVE

#include "StructsAndEnum.h"
#include "board_visual.h"
#include "../mouse actions.h"

// //----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$

bool CheckerCoordinates(Game* game, int* x, int* y);
bool isCheckerBlocked(CH_Type** board, int x, int y);
void King_check(CH_Type** board, int x, int y, Player player);
void performCapture(CH_Type** board, int fromX, int fromY, int toX, int toY);
void executeRegularMove(CH_Type** board, Player player, Board* CheckersBoard);
bool canCheckerMove(int x1, int y1, int x2, int y2, bool isKing, Player color, CH_Type** board);

// //----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$

Game* game;

// //----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$

bool CheckerCoordinates(Game* game, int* x, int* y)
{
    Choice c = get_mouse_click(x, y, game);
    if (c == MOUSE_LEFT)
    {
        printf("%d %d\n", *x, *y);
        *x = (*x - (239 + 43)) / 105;
        *y = (*y - 41) / 105;
    }
    return c;
}

//Проверяет условие появления дамки
void King_check(CH_Type** board, int x, int y, Player player)
{
    if (!board[y][x])
        return;
    //Превращает белую шашку в дамку
    if (board[y][x] == WHITE_PAWN && y == 0)
    {
        board[y][x] = WHITE_KING;
        board[y][x] = WHITE_KING;
    }
    // Превращает красную шашку в дамку
    if (board[y][x] == RED_PAWN && y == 7)
        board[y][x] = RED_KING;
}
/*
 * Выполняет обычный ход без взятия фигур противника
 * @param board Игровая доска
 * @param player Текущий игрок (WHITE/RED)
 */
void executeRegularMove(CH_Type** board, Player player, Board* CheckersBoard)
{
    int fromX, fromY; // Координаты исходной позиции
    
    // 1. Фаза выбора шашки для хода
    while (true)
    {
        // Ввод координат шашки
        if (CheckerCoordinates(game, &fromX, &fromY) != MOUSE_LEFT)
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
    highlightChecker(game, fromX, fromY, board);
    renderBoardFrame(CheckersBoard);
    // 3. Фаза выбора целевой позиции
    int toX, toY;
    while (true) 
    {
        // Ввод координат для хода

        if (CheckerCoordinates(game, &toX, &toY) != MOUSE_LEFT)
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
        unhighlightChecker(game, board, toX, toY);
        renderBoardFrame(CheckersBoard);
        // 6. Проверка на превращение в дамку
        // King_check(board, toX, toY, player);
        break;
    }
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
    // 1. Сначала обновляем графическое представление перемещения
    // moveCheckerOnBoard(game, fromX, fromY, toX, toY, board);
    
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
    return true;
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

#endif