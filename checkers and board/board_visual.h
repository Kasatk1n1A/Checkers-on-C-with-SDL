#ifndef BOARD_VISUAL
#define BOARD_VISUAL

//----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$

#include "board.h"
#include "StructsAndEnum.h"
#include "checkers_main.h"
#include "../mouse actions.h"

//----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$

Game* game;

//----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$

void board_cleanup_SDL(Board* CheckersBoard);
void out_board_SDL(Game* game, Board* CheckersBoard);
bool LoadCheckersTextures(Board* CheckersBoard, Game* game);
void highlightChecker(Game* game, int x1, int y1, CH_Type** board);
void unhighlightChecker(Game* game, CH_Type** board, int x2, int y2);

//----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$

void renderBoardFrame(Board* CheckersBoard)
{
    // Очистка экрана
    SDL_RenderClear(game->renderer);
        
    // Отрисовка фона
    SDL_RenderCopy(game->renderer, game->background, NULL, NULL);

    // Отрисовка игровой доски
    out_board_SDL(game, CheckersBoard);

    // Обновление экрана
    SDL_RenderPresent(game->renderer);

    // Задержка для контроля FPS
    SDL_Delay(16);
}

void highlightChecker(Game* game, int x1, int y1, CH_Type** board)
{
    switch (board[y1][x1])
    {
    case WHITE_PAWN:
        board[y1][x1] = PICKED_WHITE_PAWN;
        break;
    case RED_PAWN:
        board[y1][x1] = PICKED_RED_PAWN;
        break;
    case WHITE_KING:
        board[y1][x1] = PICKED_WHITE_KING;
        break;
    case RED_KING:
        board[y1][x1] = PICKED_RED_KING;
        break;
    }
}

void unhighlightChecker(Game* game, CH_Type** board, int x2, int y2)
{
    switch (board[y2][x2]) 
    {
    case PICKED_WHITE_PAWN:
        board[y2][x2] = WHITE_PAWN;
        break;
    case PICKED_RED_PAWN:
        board[y2][x2] = RED_PAWN;
        break;
    case PICKED_WHITE_KING:
        board[y2][x2] = WHITE_KING;
        break;
    case PICKED_RED_KING:
        board[y2][x2] = RED_KING;
        break;
    }
}

bool LoadBoardTextures(Board* CheckersBoard, Game* game)
{    
    CheckersBoard->image = IMG_LoadTexture(game->renderer, "checkers and board/pictures/chessboard.png");     
    if (!CheckersBoard->image)
    {
        fprintf(stderr, "Error creating Texture: %s\n", IMG_GetError());
        return true;
    }
    // Установка позиции и размеров доски
    CheckersBoard->rect.x = 239; 
    CheckersBoard->rect.y = 0; 
    if (SDL_QueryTexture(CheckersBoard->image, NULL, NULL, &CheckersBoard->rect.w, &CheckersBoard->rect.h)){
        fprintf(stderr, "Error quering Texture: %s\n", SDL_GetError());
        return true;
    }
    
    // Получение реальных размеров текстуры
    CheckersBoard->checkers = (Checker*)malloc(sizeof(Checker));

    CheckersBoard->checkers->white_image = IMG_LoadTexture(game->renderer, "checkers and board/pictures/white_pawn.png");
    if (!CheckersBoard->checkers->white_image )
    {
        fprintf(stderr, "Error creating Texture: %s\n", IMG_GetError());
        return true;
    }
    CheckersBoard->checkers->picked_white_image = IMG_LoadTexture(game->renderer, "checkers and board/pictures/picked_white_pawn.png");
    if (!CheckersBoard->checkers->picked_white_image)
    {
        fprintf(stderr, "Error creating Texture: %s\n", IMG_GetError());
        return true;
    }
    CheckersBoard->checkers->white_King_image = IMG_LoadTexture(game->renderer, "checkers and board/pictures/white_king.png");
    if (!CheckersBoard->checkers->white_King_image)
    {
        fprintf(stderr, "Error creating Texture: %s\n", IMG_GetError());
        return true;
    }
    CheckersBoard->checkers->picked_white_king_image = IMG_LoadTexture(game->renderer, "checkers and board/pictures/picked_white_king.png");
    if (!CheckersBoard->checkers->picked_white_king_image)
    {
        fprintf(stderr, "Error creating Texture: %s\n", IMG_GetError());
        return true;
    }

    CheckersBoard->checkers->red_image = IMG_LoadTexture(game->renderer, "checkers and board/pictures/red_pawn.png");
    if (!CheckersBoard->checkers->red_image)
    {
        fprintf(stderr, "Error creating Texture: %s\n", IMG_GetError());
        return true;
    }
    CheckersBoard->checkers->picked_red_image = IMG_LoadTexture(game->renderer, "checkers and board/pictures/picked_red_pawn.png");
    if (!CheckersBoard->checkers->picked_red_image)
    {
        fprintf(stderr, "Error creating Texture: %s\n", IMG_GetError());
        return true;
    }
    CheckersBoard->checkers->red_King_image = IMG_LoadTexture(game->renderer, "checkers and board/pictures/red_king.png");
    if (!CheckersBoard->checkers->red_King_image)
    {
        fprintf(stderr, "Error creating Texture: %s\n", IMG_GetError());
        return true;
    }
    CheckersBoard->checkers->picked_red_king_image = IMG_LoadTexture(game->renderer, "checkers and board/pictures/picked_red_king.png");
    if (!CheckersBoard->checkers->picked_red_king_image)
    {
        fprintf(stderr, "Error creating Texture: %s\n", IMG_GetError());
        return true;
    }

    return false;
}

void out_board_SDL(Game* game, Board* CheckersBoard)
{
    // Отрисовка фона доски
    SDL_RenderCopy(game->renderer, CheckersBoard->image, NULL, &CheckersBoard->rect);
    
    // Отрисовка шашек
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (!CheckersBoard->board[i][j])
                continue;

            CheckersBoard->checkers->rect.x = 105 * j + 239 + 43;
            CheckersBoard->checkers->rect.y = 105 * i + 41;

            // Выбор текстуры в зависимости от состояния шашки
            switch (CheckersBoard->board[i][j])
            {
                // Отрисовка обычной шашки
            case RED_PAWN:
                if (SDL_QueryTexture(CheckersBoard->checkers->red_image, NULL, NULL, &CheckersBoard->checkers->rect.w, &CheckersBoard->checkers->rect.h))
                    fprintf(stderr, "Error quering Texture: %s\n", SDL_GetError());

                SDL_RenderCopy(game->renderer, CheckersBoard->checkers->red_image, NULL, &CheckersBoard->checkers->rect);
                break;
            case WHITE_PAWN:
                if (SDL_QueryTexture(CheckersBoard->checkers->white_image, NULL, NULL, &CheckersBoard->checkers->rect.w, &CheckersBoard->checkers->rect.h))
                    fprintf(stderr, "Error quering Texture: %s\n", SDL_GetError());
                    
                SDL_RenderCopy(game->renderer, CheckersBoard->checkers->white_image, NULL, &CheckersBoard->checkers->rect);
                break;
                // Отрисовка дамки
            case RED_KING:
                if (SDL_QueryTexture(CheckersBoard->checkers->red_King_image, NULL, NULL, &CheckersBoard->checkers->rect.w, &CheckersBoard->checkers->rect.h))
                    fprintf(stderr, "Error quering Texture: %s\n", SDL_GetError());
                
                SDL_RenderCopy(game->renderer, CheckersBoard->checkers->red_King_image, NULL, &CheckersBoard->checkers->rect);
                break;
            case WHITE_KING:
                if (SDL_QueryTexture(CheckersBoard->checkers->white_King_image, NULL, NULL, &CheckersBoard->checkers->rect.w, &CheckersBoard->checkers->rect.h))
                    fprintf(stderr, "Error quering Texture: %s\n", SDL_GetError());
                
                SDL_RenderCopy(game->renderer, CheckersBoard->checkers->white_King_image, NULL, &CheckersBoard->checkers->rect);
                break; 
                // Отрисовка выделенной шашки
            case PICKED_WHITE_PAWN:
                if (SDL_QueryTexture(CheckersBoard->checkers->picked_white_image, NULL, NULL, &CheckersBoard->checkers->rect.w, &CheckersBoard->checkers->rect.h))
                    fprintf(stderr, "Error quering Texture: %s\n", SDL_GetError());
                
                SDL_RenderCopy(game->renderer, CheckersBoard->checkers->picked_white_image, NULL, &CheckersBoard->checkers->rect);
                break;
            case PICKED_RED_PAWN:
                if (SDL_QueryTexture(CheckersBoard->checkers->picked_red_image, NULL, NULL, &CheckersBoard->checkers->rect.w, &CheckersBoard->checkers->rect.h))
                    fprintf(stderr, "Error quering Texture: %s\n", SDL_GetError());

                SDL_RenderCopy(game->renderer, CheckersBoard->checkers->picked_red_image, NULL, &CheckersBoard->checkers->rect);
                break; 
                // Отрисовка выделенной дамки
            case PICKED_WHITE_KING:
                if (SDL_QueryTexture(CheckersBoard->checkers->picked_white_king_image, NULL, NULL, &CheckersBoard->checkers->rect.w, &CheckersBoard->checkers->rect.h))
                    fprintf(stderr, "Error quering Texture: %s\n", SDL_GetError());
                
                SDL_RenderCopy(game->renderer, CheckersBoard->checkers->picked_white_king_image, NULL, &CheckersBoard->checkers->rect);
                break;
            case PICKED_RED_KING:
                if (SDL_QueryTexture(CheckersBoard->checkers->picked_red_king_image, NULL, NULL, &CheckersBoard->checkers->rect.w, &CheckersBoard->checkers->rect.h))
                    fprintf(stderr, "Error quering Texture: %s\n", SDL_GetError());
                    
                SDL_RenderCopy(game->renderer, CheckersBoard->checkers->picked_red_king_image, NULL, &CheckersBoard->checkers->rect);
                break; 
            default:
                break;
            }
        }
    }
}

void board_cleanup_SDL(Board* CheckersBoard)
{
    if (CheckersBoard->board)
        freeBoard((void**)CheckersBoard->board);

    // Удаление текстуры доски
    if (CheckersBoard->image)
        SDL_DestroyTexture(CheckersBoard->image);
    
    if (CheckersBoard->checkers->white_image)
        SDL_DestroyTexture(CheckersBoard->checkers->white_image);

    if (CheckersBoard->checkers->picked_white_image)
        SDL_DestroyTexture(CheckersBoard->checkers->picked_white_image);

    if (CheckersBoard->checkers->white_King_image)
        SDL_DestroyTexture(CheckersBoard->checkers->white_King_image);

    if (CheckersBoard->checkers->picked_white_king_image)
        SDL_DestroyTexture(CheckersBoard->checkers->picked_white_king_image);

    if (CheckersBoard->checkers->red_image)
        SDL_DestroyTexture(CheckersBoard->checkers->red_image);

    if (CheckersBoard->checkers->picked_red_image)
        SDL_DestroyTexture(CheckersBoard->checkers->picked_red_image);

    if (CheckersBoard->checkers->red_King_image)
        SDL_DestroyTexture(CheckersBoard->checkers->red_King_image);

    if (CheckersBoard->checkers->picked_red_king_image)
        SDL_DestroyTexture(CheckersBoard->checkers->picked_red_king_image);

    // Освобождение основного массива шашек
    free(CheckersBoard->checkers);
    
    freeBoard((void**)CheckersBoard->board); 

    // Освобождение структуры доски
    free(CheckersBoard);
}




#endif