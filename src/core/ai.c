// bot.c - Реализация ИИ для игры в шашки
#include "ai.h"
#include "hash_table.h"
#include "LRU_cache.h"

static struct HashTable* hash_table;
static struct LRUCache* cache;

// Оценка текущей позиции на доске для указанного игрока
int evaluate_position(CH_Type** board, Player player) {
    int score = 0;
    const int pawn_value = 10;   // Значение пешки
    const int king_value = 30;   // Значение дамки

    // Коэффициенты для разных ситуаций     
    const int EAT_BONUS = 30;         // За съедение шашки
    const int EAT_KING_BONUS = 100;    // За съедение дамки
    const int KING_BONUS = 300;        // За превращение в дамку
    const int DIE_PENALTY = -40;       // За потерю шашки
    const int DIE_KING_PENALTY = -100; // За потерю дамки
    const int MOBILITY_BONUS = 2;      // За возможность хода

    // Проход по всем клеткам доски
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            CH_Type piece = board[y][x];

            // Обработка белых фигур
            if (piece == WHITE_PAWN || piece == PICKED_WHITE_PAWN ||
                piece == WHITE_KING || piece == PICKED_WHITE_KING) {

                bool isKing = (piece == WHITE_KING || piece == PICKED_WHITE_KING);
                int value = isKing ? king_value : pawn_value;

                if (player == WHITE) {
                    score += value;
                    // Бонус за продвижение вперёд (чем ближе к дамке, тем лучше)
                    if (!isKing) score += (7 - y);
                    // Бонус за центральное положение
                    if (x >= 3 && x <= 4) score += 1;
                }
                else {
                    score -= value;
                    // Штраф за продвижение противника
                    if (!isKing) score -= (7 - y);
                    // Штраф за центральное положение противника
                    if (x >= 3 && x <= 4) score -= 1;
                }

                // Дополнительный бонус за дамку
                if (isKing && player == WHITE) score += KING_BONUS / 10;
            }
            // Обработка красных фигур
            else if (piece == RED_PAWN || piece == PICKED_RED_PAWN ||
                     piece == RED_KING || piece == PICKED_RED_KING) {

                bool isKing = (piece == RED_KING || piece == PICKED_RED_KING);
                int value = isKing ? king_value : pawn_value;

                if (player == RED) {
                    score += value;
                    // Бонус за продвижение вперёд (для красных направление обратное)
                    if (!isKing) score += y;
                    // Бонус за центральное положение
                    if (x >= 3 && x <= 4) score += 1;
                }
                else {
                    score -= value;
                    // Штраф за продвижение противника
                    if (!isKing) score -= y;
                    // Штраф за центральное положение противника
                    if (x >= 3 && x <= 4) score -= 1;
                }

                // Дополнительный бонус за дамку
                if (isKing && player == RED) score += KING_BONUS / 10;
            }
        }
    }

    // Оценка мобильности (количество возможных ходов)
    Move* moves = generate_all_moves(board, player);
    int mobility = 0;
    for (Move* m = moves; m != NULL; m = m->next) {
        mobility++;
        // Дополнительный бонус за ходы со взятием
        if (m->captures != NULL) {
            score += EAT_BONUS;
            // Проверяем, была ли съедена дамка
            for (Move* cap = m->captures; cap != NULL; cap = cap->next) {
                CH_Type captured = board[cap->fromY][cap->fromX];
                if ((player == WHITE && (captured == RED_KING || captured == PICKED_RED_KING)) ||
                    (player == RED && (captured == WHITE_KING || captured == PICKED_WHITE_KING))) {
                    score += EAT_KING_BONUS;
                }
            }
        }
    }
    free_move(moves);
    score += mobility * MOBILITY_BONUS;

    return score;
}

Move* generate_moves_for_piece(CH_Type** board, int x, int y, Player player) {
    Move* moves = NULL;
    Move* last = NULL;
    CH_Type piece = board[y][x];

    bool isKing = (piece == WHITE_KING || piece == PICKED_WHITE_KING ||
                   piece == RED_KING || piece == PICKED_RED_KING);
    bool isWhite = (player == WHITE);

    int directions[4][2] = { {1,1}, {-1,1}, {1,-1}, {-1,-1} };

    // Проверка всех направлений
    for (int d = 0; d < 4; d++) {
        int dx = directions[d][0];
        int dy = directions[d][1];

        // Для обычных шашек корректируем направление
        if (!isKing) {
            dy = isWhite ? -1 : 1;
        }

        // Проверка ходов для дамки
        if (isKing) {
            // Проверяем все 4 диагональных направления
            for (int dir = 0; dir < 4; dir++) {
                int step = 1;
                while (true) 
                {
                    int nx = x + dx * step;
                    int ny = y + dy * step;
                    
                    // Проверяем, находится ли новая позиция в пределах доски
                    if (nx < 0 || nx >= 8 || ny < 0 || ny >= 8) break;
                    
                    // Если клетка пустая - добавляем ход
                    if (board[ny][nx] == EMPTY) {
                        Move* newMove = create_move(x, y, nx, ny, NULL);
                        if (!moves) moves = newMove;
                        else last->next = newMove;
                        last = newMove;
                        step++;
                    } else {
                        // Если клетка занята - дальше по этому направлению ходить нельзя
                        break;
                    }
                }
            }
        } 
        else {
            // Оригинальный ход для простой шашки (один шаг)
            int nx = x + dx;
            int ny = y + dy;
            if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8 && board[ny][nx] == EMPTY) {
                Move* newMove = create_move(x, y, nx, ny, NULL);
                if (!moves) moves = newMove;
                else last->next = newMove;
                last = newMove;
            }
        }

        // Проверка взятий (для дамок - рекурсивно на всю длину доски)
        if (isKing) {
            // Дамка может брать на любое расстояние
            int jx = x, jy = y;
            bool foundEnemy = false;
            
            // Идём по диагонали пока не выйдем за пределы доски
            while (1) {
                jx += dx;
                jy += dy;
                if (jx < 0 || jx >= 8 || jy < 0 || jy >= 8) break;
                
                CH_Type cell = board[jy][jx];
                
                if (cell == EMPTY) {
                    if (foundEnemy) {
                        // Нашли пустую клетку после вражеской фигуры - возможное взятие
                        Move* newMove = create_move(x, y, jx, jy, NULL);
                        newMove->captures = create_move(jx-dx, jy-dy, 0, 0, NULL);
                        
                        if (!moves) moves = newMove;
                        else last->next = newMove;
                        last = newMove;
                    }
                    continue;
                }
                
                // Проверяем, вражеская ли это фигура
                bool isEnemy = (isWhite && (cell == RED_PAWN || cell == RED_KING)) ||
                              (!isWhite && (cell == WHITE_PAWN || cell == WHITE_KING));
                
                if (isEnemy && !foundEnemy) {
                    foundEnemy = true;
                } else {
                    // Встретили вторую фигуру (свою или вражескую) - дальше нельзя
                    break;
                }
            }
        } else {
            // Для обычных шашек - только через одну клетку
            dy = directions[d][1];
            int jx = x + 2*dx;
            int jy = y + 2*dy;
            
            if (jx >= 0 && jx < 8 && jy >= 0 && jy < 8) {
                CH_Type between = board[y+dy][x+dx];
                bool isEnemy = (isWhite && (between == RED_PAWN || between == RED_KING)) ||
                              (!isWhite && (between == WHITE_PAWN || between == WHITE_KING));
                
                if (isEnemy && board[jy][jx] == EMPTY) {
                    Move* newMove = create_move(x, y, jx, jy, NULL);
                    newMove->captures = create_move(x+dx, y+dy, 0, 0, NULL);
                    
                    if (!moves) moves = newMove;
                    else last->next = newMove;
                    last = newMove;
                }
            }
        }
    }

    return moves;
}

// Вспомогательная функция для создания хода
Move* create_move(int fromX, int fromY, int toX, int toY, Move* captures) {
    Move* move = (Move*)malloc(sizeof(Move));
    move->fromX = fromX;
    move->fromY = fromY;
    move->toX = toX;
    move->toY = toY;
    move->captures = captures;
    move->next = NULL;
    return move;
}

Move* Move_Copy(const Move* src) 
{
    if (src == NULL)
        return NULL;

    // Создаем новую структуру Move
    Move* dest = (Move*)malloc(sizeof(Move));
    if (dest == NULL)
        return NULL; // Обработка ошибки выделения памяти

    // Копируем простые поля
    dest->fromX = src->fromX;
    dest->fromY = src->fromY;
    dest->toX = src->toX;
    dest->toY = src->toY;
    dest->next = NULL; // Связь устанавливается отдельно

    // Рекурсивно копируем список captures
    dest->captures = Move_Copy(src->captures);

    return dest;
}

// Генерация всех возможных ходов для игрока
Move* generate_all_moves(CH_Type** board, Player player) 
{
    Move* allMoves = NULL;  // Список всех ходов
    Move* last = NULL;      // Последний ход в списке

    // Проверка, есть ли обязательные взятия
    bool** attack_board = canCapture(board, player == WHITE);
    bool hasCaptures = (attack_board != NULL);

    // Проход по всем клеткам доски
    for (int y = 0; y < 8; y++) 
    {
        for (int x = 0; x < 8; x++) 
        {
            CH_Type piece = board[y][x];
            // Проверка, что это наша фигура
            bool isOurPiece = (player == WHITE && (piece == WHITE_PAWN || piece == WHITE_KING)) ||
                              (player == RED && (piece == RED_PAWN || piece == RED_KING));

            if (isOurPiece) {
                // Если есть обязательные взятия, пропускаем фигуры без взятий
                if (hasCaptures && !attack_board[y][x]) {
                    continue;
                }

                // Генерируем ходы для текущей фигуры
                Move* pieceMoves = generate_moves_for_piece(board, x, y, player);

                // Если есть обязательные взятия, удаляем ходы без взятий
                if (hasCaptures) 
                {
                    Move* current = pieceMoves;
                    Move* prev = NULL;
                    while (current) {
                        if (current->captures == NULL) {
                            // Удаляем ход без взятия
                            Move* toDelete = current;
                            if (prev) {
                                prev->next = current->next;
                                current = current->next;
                            }
                            else {
                                pieceMoves = current->next;
                                current = pieceMoves;
                            }
                            toDelete->next = NULL;
                            free_move(toDelete);
                        }
                        else {
                            prev = current;
                            current = current->next;
                        }
                    }
                }

                // Добавляем ходы в общий список
                if (pieceMoves) {
                    if (allMoves == NULL) {
                        allMoves = pieceMoves;
                        last = pieceMoves;
                    }
                    else
                        last->next = pieceMoves;

                    // Переходим в конец списка
                    while (last->next) {
                        last = last->next;
                    }
                }
            }
        }
    }

    // Освобождаем память
    if (attack_board) {
        freeBoard((void**)attack_board);
        attack_board = NULL;
    }

    return allMoves;
}

// Временное выполнение хода (для анализа)
void make_temp_move(CH_Type** board, Move* move) {
    // Перемещаем фигуру
    CH_Type piece = board[move->fromY][move->fromX];
    board[move->fromY][move->fromX] = EMPTY;
    board[move->toY][move->toX] = piece;

    // Удаляем съеденные фигуры
    Move* capture = move->captures;
    while (capture) {
        board[capture->fromY][capture->fromX] = EMPTY;
        capture = capture->next;
    }

    // Проверка на превращение в дамку
    if ((piece == WHITE_PAWN || piece == PICKED_WHITE_PAWN) && move->toY == 0) {
        board[move->toY][move->toX] = WHITE_KING;  // Белая пешка достигла последней линии
    }
    else if ((piece == RED_PAWN || piece == PICKED_RED_PAWN) && move->toY == 7) {
        board[move->toY][move->toX] = RED_KING;   // Красная пешка достигла последней линии
    }
}

// Отмена временного хода (возврат к предыдущему состоянию)
void undo_temp_move(CH_Type** board, Move* move) {
    // Возвращаем фигуру на место
    CH_Type piece = board[move->toY][move->toX];
    board[move->toY][move->toX] = EMPTY;
    board[move->fromY][move->fromX] = piece;

    // Восстанавливаем съеденные фигуры
    Move* capture = move->captures;
    while (capture) {
        bool isWhite = (piece == WHITE_PAWN || piece == WHITE_KING ||
            piece == PICKED_WHITE_PAWN || piece == PICKED_WHITE_KING);

        // Восстанавливаем как пешку (упрощение)
        if (isWhite) {
            board[capture->fromY][capture->fromX] = RED_PAWN;
        }
        else {
            board[capture->fromY][capture->fromX] = WHITE_PAWN;
        }
        capture = capture->next;
    }
}

static int best_evaluate = 0;

// Алгоритм минимакса с альфа-бета отсечением
int minimax(CH_Type** board, CH_Type** tmp_board, int depth, int alpha, int beta, bool isMaximizing, Player player, int maxDepth) 
{
    Player curr_player = player == WHITE ? (isMaximizing == true ? WHITE : RED) : (isMaximizing == true ? RED : WHITE);
    int curr_eval = evaluate_position(tmp_board, player);
    // Если достигнута максимальная глубина, возвращаем оценку позиции
    if (depth == maxDepth)
        return curr_eval - evaluate_position(board, player);

    /*  checking cache if it already has such an element
        skip this board position    */
    Transposition* trans = Transposition_Create(tmp_board, curr_player, curr_eval, depth, NULL);
    Transposition* tmp = (Transposition*)HashTable_Find(hash_table, trans);
    if (tmp && tmp->depth <= trans->depth){
        Transposition_Delete(&trans);
        return tmp->eval;
    }

    /*  Generating all possible moves */
    Move* moves = generate_all_moves(tmp_board, isMaximizing ? player : (player == WHITE ? RED : WHITE));
    if (!moves)
        return isMaximizing ? -9999 : 9999;

    int best_eval = isMaximizing ? INT_MIN : INT_MAX; 
    Move* current = moves;
    Move* BestMove = NULL;

    /*  Check all possible moves to find best */
    while (current){
        make_temp_move(tmp_board, current);
        int eval = minimax(board, tmp_board, depth + 1, alpha, beta, isMaximizing ? false : true, player, maxDepth);
        undo_temp_move(tmp_board, current);

        if (isMaximizing){  /* player`s turn */
            if (eval > best_eval)
            {
                best_eval = eval;
                free_move(BestMove);
                BestMove = Move_Copy(current);
            }
            alpha = (alpha > eval) ? alpha : eval;
        } else {    /* enemy`s turn */
            if (eval < best_eval)
            {
                best_eval = eval;
                free_move(BestMove);
                BestMove = Move_Copy(current);
            }
            beta = (beta < eval) ? beta : eval;
        }
        /*  alpha-beta clipping */
        if (beta <= alpha) {
            break;
        }

        current = current->next;
    }

    /*  add into cache */
    trans->eval = best_eval;
    if (tmp)
    {
        tmp->depth = trans->depth;
        tmp->eval = trans->eval;
        free(trans);
    }
    else{
        HashTable_Add(hash_table, trans);
    }
    free_move(moves);
    return best_eval;
}

// Поиск лучшего хода
Move* find_best_move(CH_Type** board, Player player, int maxDepth)
{
    CH_Type** tmp_board = add_board();
    CopyBoard(board, tmp_board);

    Move* bestMove = NULL;   // Лучший ход
    int bestValue = INT_MIN; // Его оценка
    Move* moves = generate_all_moves(board, player);

    // Если нет доступных ходов
    if (moves == NULL) {
        return NULL;
    }
    
    // Перебираем все возможные ходы
    Move* current = moves;
    while (current) 
    {
        // Пробуем ход
        make_temp_move(tmp_board, current);
        // Оцениваем его
        int moveValue = minimax(board, tmp_board, 0, INT_MIN, INT_MAX, false, player, maxDepth);
        // Отменяем ход
        undo_temp_move(tmp_board, current);

        // Если этот ход лучше предыдущих
        if (moveValue > bestValue) 
        {
            bestValue = moveValue;
            // Освобождаем память предыдущего лучшего хода
            
            // Копируем текущий ход как новый лучший
            free_move(bestMove);
            bestMove = Move_Copy(current);
        }

        current = current->next;
    }
    
    freeBoard((void**)tmp_board);
    free_move(moves);
    tmp_board = NULL;
    moves = NULL;
    return bestMove;
}

// Основная функция для выполнения хода ботом
void bot_make_move(CH_Type** board, int difficult, Player player)
{
    hash_table = HashTable_create(Transposition_Compare, Transposition_hash, _Transposition_Delete_);
    // struct LRUCache* cache = LRUCache_create();

    // Установка глубины поиска в зависимости от сложности
    int maxDepth = difficult;

    // Проверяем, есть ли обязательные взятия
    bool** attack_board = canCapture(board, player == WHITE);

    // Если есть взятия - выполняем лучший ход со взятием
    Move* bestMove = find_best_move(board, player, maxDepth);

    if (bestMove) 
    {
        // Выполняем взятие
        performCapture(board, bestMove->fromX, bestMove->fromY, bestMove->toX, bestMove->toY);
        
        printf("bestMove toY: %d, toX: %d\n", bestMove->toY, bestMove->toX);
        if (board[bestMove->toY][bestMove->toX] == WHITE_PAWN && bestMove->toY == 0) {
            // Белая пешка достигла последней линии (нижний край доски)
            board[bestMove->toY][bestMove->toX] = WHITE_KING;
        }
        else if (board[bestMove->toY][bestMove->toX] == RED_PAWN && bestMove->toY == 7) {
            // Красная пешка достигла последней линии (верхний край доски)
            board[bestMove->toY][bestMove->toX] = RED_KING;
        }
        // Проверяем, можно ли продолжить взятия
        if (attack_board && CanContinue(board, bestMove->toX, bestMove->toY, player)) 
            bot_make_move(board, difficult, player);
    }
    if (attack_board) 
    {
        freeBoard((void**)attack_board);
        attack_board = NULL;
    }
    
    free_move(bestMove);

    printf("size: %d\n", hash_table->size);
    HashTable_clean(hash_table);
    HashTable_delete(hash_table);
}