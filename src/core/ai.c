// bot.c - Реализация ИИ для игры в шашки
#include "ai.h"
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

// Оценка текущей позиции на доске для указанного игрока
int evaluate_position(CH_Type** board, Player player) {
    int score = 0;
    const int pawn_value = 10;   // Значение пешки
    const int king_value = 30;   // Значение дамки

    // Проход по всем клеткам доски
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            CH_Type piece = board[y][x];

            // Обработка белых пешек
            if (piece == WHITE_PAWN || piece == PICKED_WHITE_PAWN) {
                score += (player == WHITE) ? pawn_value : -pawn_value;
                // Бонус за продвижение вперёд (чем ближе к дамке, тем лучше)
                score += (player == WHITE) ? (7 - y) : y;
            }
            // Обработка белых дамок
            else if (piece == WHITE_KING || piece == PICKED_WHITE_KING) {
                score += (player == WHITE) ? king_value : -king_value;
            }
            // Обработка красных пешек
            else if (piece == RED_PAWN || piece == PICKED_RED_PAWN) {
                score += (player == RED) ? pawn_value : -pawn_value;
                // Бонус за продвижение вперёд (для красных направление обратное)
                score += (player == RED) ? y : (7 - y);
            }
            // Обработка красных дамок
            else if (piece == RED_KING || piece == PICKED_RED_KING) {
                score += (player == RED) ? king_value : -king_value;
            }
        }
    }
    return score;
}

// Генерация всех возможных ходов для конкретной фигуры
Move* generate_moves_for_piece(CH_Type** board, int x, int y, Player player) {
    Move* moves = NULL;  // Список возможных ходов
    Move* last = NULL;   // Последний ход в списке
    CH_Type piece = board[y][x];

    // Проверка типа фигуры
    bool isKing = (piece == WHITE_KING || piece == PICKED_WHITE_KING ||
        piece == RED_KING || piece == PICKED_RED_KING);
    bool isWhite = (player == WHITE);

    // Направления движения (для дамок - все 4 направления, для пешек - только вперёд)
    int directions[4][2] = { {1,1}, {1,-1}, {-1,1}, {-1,-1} };
    int dirCount = isKing ? 4 : 2;

    // Проверка всех возможных направлений
    for (int d = 0; d < dirCount; d++) {
        int dx = directions[d][0];
        int dy = directions[d][1];

        // Для обычных шашек корректируем направление (белые ходят вверх, красные - вниз)
        if (!isKing) {
            dy = isWhite ? -1 : 1;
        }

        // Координаты соседней клетки
        int nx = x + dx;
        int ny = y + dy;

        // Проверка на обычный ход (клетка в пределах доски и пуста)
        if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8 && board[ny][nx] == EMPTY) {
            // Создаём новый ход
            Move* newMove = (Move*)malloc(sizeof(Move));
            newMove->fromX = x;
            newMove->fromY = y;
            newMove->toX = nx;
            newMove->toY = ny;
            newMove->captures = NULL;
            newMove->next = NULL;

            // Добавляем ход в список
            if (moves == NULL) {
                moves = newMove;
                last = newMove;
            }
            else {
                last->next = newMove;
                last = newMove;
            }
        }

        // Проверка на взятие (прыжок через фигуру противника)
        int jx = x + 2 * dx;  // Клетка после прыжка
        int jy = y + 2 * dy;

        // Проверка границ доски
        if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8 &&
            jx >= 0 && jx < 8 && jy >= 0 && jy < 8) {

            // Фигура между текущей и целевой клеткой
            CH_Type between = board[ny][nx];
            // Проверка, что это фигура противника
            bool isEnemy = (isWhite && (between == RED_PAWN || between == RED_KING)) ||
                (!isWhite && (between == WHITE_PAWN || between == WHITE_KING));

            // Если между клетками фигура противника, а целевая клетка пуста
            if (isEnemy && board[jy][jx] == EMPTY) {
                // Создаём ход со взятием
                Move* newMove = (Move*)malloc(sizeof(Move));
                newMove->fromX = x;
                newMove->fromY = y;
                newMove->toX = jx;
                newMove->toY = jy;

                // Запоминаем съеденную фигуру
                newMove->captures = (Move*)malloc(sizeof(Move));
                newMove->captures->fromX = nx;
                newMove->captures->fromY = ny;
                newMove->captures->next = NULL;
                newMove->next = NULL;

                // Добавляем ход в список
                if (moves == NULL) {
                    moves = newMove;
                    last = newMove;
                }
                else {
                    last->next = newMove;
                    last = newMove;
                }
            }
        }
    }

    return moves;
}

// Генерация всех возможных ходов для игрока
Move* generate_all_moves(CH_Type** board, Player player) {
    Move* allMoves = NULL;  // Список всех ходов
    Move* last = NULL;      // Последний ход в списке

    // Проверка, есть ли обязательные взятия
    bool** attack_board = canCapture(board, player == WHITE);
    bool hasCaptures = (attack_board != NULL);

    // Проход по всем клеткам доски
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
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
                if (hasCaptures) {
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
                            free(toDelete);
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
                    else {
                        last->next = pieceMoves;
                    }

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

// Алгоритм минимакса с альфа-бета отсечением
int minimax(CH_Type** board, int depth, int alpha, int beta, bool isMaximizing, Player player, int maxDepth) {
    // Если достигнута максимальная глубина, возвращаем оценку позиции
    if (depth == maxDepth) {
        return evaluate_position(board, player);
    }

    // Генерируем все возможные ходы
    Move* moves = generate_all_moves(board, isMaximizing ? player : (player == WHITE ? RED : WHITE));

    // Если нет доступных ходов - поражение или ничья
    if (moves == NULL) {
        free_move(moves);
        return evaluate_position(board, player);
    }

    // Максимизация (ход игрока)
    if (isMaximizing) {
        int maxEval = INT_MIN;
        Move* current = moves;

        while (current) {
            // Пробуем ход
            make_temp_move(board, current);
            // Рекурсивно оцениваем позицию
            int eval = minimax(board, depth + 1, alpha, beta, false, player, maxDepth);
            // Отменяем ход
            undo_temp_move(board, current);

            // Обновляем максимальную оценку
            maxEval = (eval > maxEval) ? eval : maxEval;
            alpha = (alpha > eval) ? alpha : eval;

            // Альфа-бета отсечение
            if (beta <= alpha) {
                break;
            }

            current = current->next;
        }

        free_move(moves);
        return maxEval;
    }
    // Минимизация (ход противника)
    else {
        int minEval = INT_MAX;
        Move* current = moves;

        while (current) {
            // Пробуем ход
            make_temp_move(board, current);
            // Рекурсивно оцениваем позицию
            int eval = minimax(board, depth + 1, alpha, beta, true, player, maxDepth);
            // Отменяем ход
            undo_temp_move(board, current);

            // Обновляем минимальную оценку
            minEval = (eval < minEval) ? eval : minEval;
            beta = (beta < eval) ? beta : eval;

            // Альфа-бета отсечение
            if (beta <= alpha) {
                break;
            }

            current = current->next;
        }

        free_move(moves);
        return minEval;
    }
}

// Поиск лучшего хода
Move* find_best_move(CH_Type** board, Player player, int maxDepth) {
    Move* bestMove = NULL;   // Лучший ход
    int bestValue = INT_MIN; // Его оценка
    Move* moves = generate_all_moves(board, player);

    // Если нет доступных ходов
    if (moves == NULL) {
        return NULL;
    }

    // Перебираем все возможные ходы
    Move* current = moves;
    while (current) {
        // Пробуем ход
        make_temp_move(board, current);
        // Оцениваем его
        int moveValue = minimax(board, 0, INT_MIN, INT_MAX, false, player, maxDepth);
        // Отменяем ход
        undo_temp_move(board, current);

        // Если этот ход лучше предыдущих
        if (moveValue > bestValue) {
            bestValue = moveValue;
            // Освобождаем память предыдущего лучшего хода
            if (bestMove) {
                free(bestMove);
            }
            // Копируем текущий ход как новый лучший
            bestMove = (Move*)malloc(sizeof(Move));
            *bestMove = *current;
            bestMove->next = NULL;

            // Копируем информацию о съеденных фигурах
            if (current->captures) {
                bestMove->captures = (Move*)malloc(sizeof(Move));
                *bestMove->captures = *current->captures;
                bestMove->captures->next = NULL;
            }
            else {
                bestMove->captures = NULL;
            }
        }

        current = current->next;
    }

    free_move(moves);
    return bestMove;
}

// Освобождение памяти, занятой списком ходов
void free_move(Move* move) {
    if (!move) return;

    Move* current = move;
    while (current) {
        Move* next = current->next;
        if (current->captures) {
            free(current->captures);
        }
        free(current);
        current = next;
    }
}

// Основная функция для выполнения хода ботом
void bot_make_move(CH_Type** board, int difficult, Player player)
{
    // Установка глубины поиска в зависимости от сложности
    int maxDepth = difficult;

    // Проверяем, есть ли обязательные взятия
    bool** attack_board = canCapture(board, player == WHITE);
    CH_Type** tmp_board = add_board();
    CopyBoard(board, tmp_board);

    if (attack_board) 
    {
        // Если есть взятия - выполняем лучший ход со взятием
        Move* bestMove = find_best_move(tmp_board, player, maxDepth);

        if (bestMove) {
            // Выполняем взятие
            performCapture(board, bestMove->fromX, bestMove->fromY, bestMove->toX, bestMove->toY);

            // Проверяем, можно ли продолжить взятия
            if (CanContinue(board, bestMove->toX, bestMove->toY, player)) {
                // Создаем временную копию доски
                CH_Type** tempBoard = (CH_Type**)malloc(sizeof(CH_Type*) * 8);
                for (int i = 0; i < 8; i++) {
                    tempBoard[i] = (CH_Type*)malloc(sizeof(CH_Type) * 8);
                    memcpy(tempBoard[i], board[i], sizeof(CH_Type) * 8);
                }

                // Рекурсивно продолжаем взятия
                bot_make_move(tempBoard, difficult, player);

                // Освобождаем временную доску
                freeBoard((void**)tempBoard);
            }

            free_move(bestMove);
        }

        freeBoard((void**)attack_board);
    }
    else
    {
        // Если нет обязательных взятий - выполняем обычный ход
        Move* bestMove = find_best_move(tmp_board, player, maxDepth);

        if (bestMove) {
            // Перемещаем фигуру
            CH_Type piece = board[bestMove->fromY][bestMove->fromX];
            board[bestMove->fromY][bestMove->fromX] = EMPTY;
            board[bestMove->toY][bestMove->toX] = piece;

            // Проверка на превращение в дамку
            if ((piece == WHITE_PAWN || piece == PICKED_WHITE_PAWN) && bestMove->toY == 0) {
                board[bestMove->toY][bestMove->toX] = WHITE_KING;
            }
            else if ((piece == RED_PAWN || piece == PICKED_RED_PAWN) && bestMove->toY == 7) {
                board[bestMove->toY][bestMove->toX] = RED_KING;
            }

            free_move(bestMove);
        }
    }
    freeBoard((void**)tmp_board);
}