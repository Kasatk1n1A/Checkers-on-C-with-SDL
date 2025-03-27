#ifndef BOARD_VISUAL
#define BOARD_VISUAL

#include "StructsAndEnum.h"

Game* game;

void checker_cleanup_SDL(Checker* checker)
{
    if (!checker->flag)
        return;
    if (checker->default_image)
        SDL_DestroyTexture(checker->default_image);
    
    if (checker->picked_image)
        SDL_DestroyTexture(checker->picked_image);
    
    if (checker->King_image)
        SDL_DestroyTexture(checker->King_image);
    
    if (checker->King_picked_image)
        SDL_DestroyTexture(checker->King_picked_image);
    
    free(checker);
    checker = NULL;
}

/*
 * Освобождает графические ресурсы шашки (текстуру и т.д.)
 * @param checker Указатель на объект шашки
 * нужна чтобы удалять срубленные шашки
 */
void destroyCheckerTexture(Checker* checker) {
    SDL_LockMutex(game->mutex);
    checker_cleanup_SDL(checker);  // Внутренняя функция очистки
    SDL_UnlockMutex(game->mutex);
}

void highlightChecker(Game* game, int x1, int y1, CH_Type** board)
{
    SDL_LockMutex(game->mutex);
    switch (board[y1][x1])
    {
    case WHITE_PAWN:
        game->board->checkers[y1][x1]->flag = PICKED_WHITE_PAWN;
        break;
    case RED_PAWN:
        game->board->checkers[y1][x1]->flag = PICKED_RED_PAWN;
        break;
    case WHITE_KING:
        game->board->checkers[y1][x1]->flag = PICKED_WHITE_KING;
        break;
    case RED_KING:
        game->board->checkers[y1][x1]->flag = PICKED_RED_KING;
        break;
    }
    SDL_UnlockMutex(game->mutex);
}

void unhighlightChecker(Game* game, int x2, int y2)
{
    SDL_LockMutex(game->mutex);
    switch (game->board->checkers[y2][x2]->flag) {
        case PICKED_WHITE_PAWN:
            game->board->checkers[y2][x2]->flag = WHITE_PAWN;
            break;
        case PICKED_RED_PAWN:
            game->board->checkers[y2][x2]->flag = RED_PAWN;
            break;
        case PICKED_WHITE_KING:
            game->board->checkers[y2][x2]->flag = WHITE_KING;
            break;
        case PICKED_RED_KING:
            game->board->checkers[y2][x2]->flag = RED_KING;
            break;
    }
    SDL_UnlockMutex(game->mutex);
}

/*
 * Перемещает шашку на доске и обновляет ее графическую позицию
 * @param game Указатель на структуру игры
 * @param fromX,fromY Исходные координаты
 * @param toX,toY Целевые координаты
 * @param board Текущее состояние доски (для обновления флага)
 */
void moveCheckerOnBoard(Game* game, int fromX, int fromY, int toX, int toY, CH_Type** board)
{
    SDL_LockMutex(game->mutex);
    
    // Перемещаем шашку
    game->board->checkers[toY][toX] = game->board->checkers[fromY][fromX];
    game->board->checkers[fromY][fromX] = NULL;
    
    // Обновляем графические координаты
    game->board->checkers[toY][toX]->rect.x = 282 + 105 * toX;  // TODO: Вынести константы в настройки
    game->board->checkers[toY][toX]->rect.y = 43 + 105 * toY;
    
    SDL_UnlockMutex(game->mutex);
}

#endif