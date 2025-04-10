#include "board.h"

void CopyBoard(CH_Type** board, CH_Type** new)
{
    for (int i = 0; i < 8; i++)
        memcpy(new[i], board[i], 8 * sizeof(CH_Type));
}


//Создаёт изначальную доску в качестве массива из динамических массивов
CH_Type** add_board(void)
{
    CH_Type** board = (CH_Type**)malloc(sizeof(CH_Type*) * 8);       

    //{ 0, r, 0, r, 0, r, 0, r },
    //{ r, 0, r, 0, r, 0, r, 0 },
    //{ 0, r, 0, r, 0, r, 0, r },
    //{ 0, 0, 0, 0, 0, 0, 0, 0 },
    //{ 0, w, 0, 0, 0, 0, 0, 0 },
    //{ w, 0, w, 0, w, 0, w, 0 },
    //{ 0, w, 0, w, 0, w, 0, w },
    //{ w, 0, w, 0, w, 0, w, 0 },

    for (int i = 0; i < 8; i++){
        board[i] = (CH_Type*)malloc(sizeof(CH_Type) * 8);
        memset(board[i], 0, sizeof(CH_Type) * 8);
    }

    for (int j = 0; j < 3; j++)
        for (int i = 1 - j % 2; i < 8; i += 2)
            board[j][i] = RED_PAWN;

    for (int j = 5; j < 8; j++)
        for (int i = 1 - j % 2; i < 8; i += 2)
            board[j][i] = WHITE_PAWN;

    return board;
}

void out_board(CH_Type** board)
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

// Удаляет массив из динамических массивов (игральную доску)
void freeBoard(void** board)
{
    if (!board) 
        return;
    for (int i = 0; i < 8; i++)
        if (board[i])
            free(board[i]);

    free(board);
}

//----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$

void out_board_SDL(Window* window, Board* CheckersBoard)
{
    // Отрисовка фона доски
    SDL_RenderCopy(window->renderer, CheckersBoard->image, NULL, &CheckersBoard->rect);
    
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

                SDL_RenderCopy(window->renderer, CheckersBoard->checkers->red_image, NULL, &CheckersBoard->checkers->rect);
                break;
            case WHITE_PAWN:
                if (SDL_QueryTexture(CheckersBoard->checkers->white_image, NULL, NULL, &CheckersBoard->checkers->rect.w, &CheckersBoard->checkers->rect.h))
                    fprintf(stderr, "Error quering Texture: %s\n", SDL_GetError());
                    
                SDL_RenderCopy(window->renderer, CheckersBoard->checkers->white_image, NULL, &CheckersBoard->checkers->rect);
                break;
                // Отрисовка дамки
            case RED_KING:
                if (SDL_QueryTexture(CheckersBoard->checkers->red_King_image, NULL, NULL, &CheckersBoard->checkers->rect.w, &CheckersBoard->checkers->rect.h))
                    fprintf(stderr, "Error quering Texture: %s\n", SDL_GetError());
                
                SDL_RenderCopy(window->renderer, CheckersBoard->checkers->red_King_image, NULL, &CheckersBoard->checkers->rect);
                break;
            case WHITE_KING:
                if (SDL_QueryTexture(CheckersBoard->checkers->white_King_image, NULL, NULL, &CheckersBoard->checkers->rect.w, &CheckersBoard->checkers->rect.h))
                    fprintf(stderr, "Error quering Texture: %s\n", SDL_GetError());
                
                SDL_RenderCopy(window->renderer, CheckersBoard->checkers->white_King_image, NULL, &CheckersBoard->checkers->rect);
                break; 
                // Отрисовка выделенной шашки
            case PICKED_WHITE_PAWN:
                if (SDL_QueryTexture(CheckersBoard->checkers->picked_white_image, NULL, NULL, &CheckersBoard->checkers->rect.w, &CheckersBoard->checkers->rect.h))
                    fprintf(stderr, "Error quering Texture: %s\n", SDL_GetError());
                
                SDL_RenderCopy(window->renderer, CheckersBoard->checkers->picked_white_image, NULL, &CheckersBoard->checkers->rect);
                break;
            case PICKED_RED_PAWN:
                if (SDL_QueryTexture(CheckersBoard->checkers->picked_red_image, NULL, NULL, &CheckersBoard->checkers->rect.w, &CheckersBoard->checkers->rect.h))
                    fprintf(stderr, "Error quering Texture: %s\n", SDL_GetError());

                SDL_RenderCopy(window->renderer, CheckersBoard->checkers->picked_red_image, NULL, &CheckersBoard->checkers->rect);
                break; 
                // Отрисовка выделенной дамки
            case PICKED_WHITE_KING:
                if (SDL_QueryTexture(CheckersBoard->checkers->picked_white_king_image, NULL, NULL, &CheckersBoard->checkers->rect.w, &CheckersBoard->checkers->rect.h))
                    fprintf(stderr, "Error quering Texture: %s\n", SDL_GetError());
                
                SDL_RenderCopy(window->renderer, CheckersBoard->checkers->picked_white_king_image, NULL, &CheckersBoard->checkers->rect);
                break;
            case PICKED_RED_KING:
                if (SDL_QueryTexture(CheckersBoard->checkers->picked_red_king_image, NULL, NULL, &CheckersBoard->checkers->rect.w, &CheckersBoard->checkers->rect.h))
                    fprintf(stderr, "Error quering Texture: %s\n", SDL_GetError());
                    
                SDL_RenderCopy(window->renderer, CheckersBoard->checkers->picked_red_king_image, NULL, &CheckersBoard->checkers->rect);
                break; 
            default:
                break;
            }
        }
    }
}

void renderBoardFrame(Window* window, Board* CheckersBoard)
{
    // Очистка экрана
    SDL_RenderClear(window->renderer);
        
    // Отрисовка фона
    SDL_RenderCopy(window->renderer, window->background, NULL, NULL);

    // Отрисовка игровой доски
    out_board_SDL(window, CheckersBoard);

    // Обновление экрана
    SDL_RenderPresent(window->renderer);

    // Задержка для контроля FPS
    SDL_Delay(16);
}

//----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$

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

    // Освобождение структуры доски
    free(CheckersBoard);
}

bool LoadBoardTextures(Board* CheckersBoard, Window* window)
{    
    CheckersBoard->image = IMG_LoadTexture(window->renderer, "assets/images/Checkers/chessboard.png");     
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

    CheckersBoard->checkers->white_image = IMG_LoadTexture(window->renderer, "assets/images/Checkers/white_pawn.png");
    if (!CheckersBoard->checkers->white_image )
    {
        fprintf(stderr, "Error creating Texture: %s\n", IMG_GetError());
        return true;
    }
    CheckersBoard->checkers->picked_white_image = IMG_LoadTexture(window->renderer, "assets/images/Checkers/picked_white_pawn.png");
    if (!CheckersBoard->checkers->picked_white_image)
    {
        fprintf(stderr, "Error creating Texture: %s\n", IMG_GetError());
        return true;
    }
    CheckersBoard->checkers->white_King_image = IMG_LoadTexture(window->renderer, "assets/images/Checkers/white_king.png");
    if (!CheckersBoard->checkers->white_King_image)
    {
        fprintf(stderr, "Error creating Texture: %s\n", IMG_GetError());
        return true;
    }
    CheckersBoard->checkers->picked_white_king_image = IMG_LoadTexture(window->renderer, "assets/images/Checkers/picked_white_king.png");
    if (!CheckersBoard->checkers->picked_white_king_image)
    {
        fprintf(stderr, "Error creating Texture: %s\n", IMG_GetError());
        return true;
    }

    CheckersBoard->checkers->red_image = IMG_LoadTexture(window->renderer, "assets/images/Checkers/red_pawn.png");
    if (!CheckersBoard->checkers->red_image)
    {
        fprintf(stderr, "Error creating Texture: %s\n", IMG_GetError());
        return true;
    }
    CheckersBoard->checkers->picked_red_image = IMG_LoadTexture(window->renderer, "assets/images/Checkers/picked_red_pawn.png");
    if (!CheckersBoard->checkers->picked_red_image)
    {
        fprintf(stderr, "Error creating Texture: %s\n", IMG_GetError());
        return true;
    }
    CheckersBoard->checkers->red_King_image = IMG_LoadTexture(window->renderer, "assets/images/Checkers/red_king.png");
    if (!CheckersBoard->checkers->red_King_image)
    {
        fprintf(stderr, "Error creating Texture: %s\n", IMG_GetError());
        return true;
    }
    CheckersBoard->checkers->picked_red_king_image = IMG_LoadTexture(window->renderer, "assets/images/Checkers/picked_red_king.png");
    if (!CheckersBoard->checkers->picked_red_king_image)
    {
        fprintf(stderr, "Error creating Texture: %s\n", IMG_GetError());
        return true;
    }

    return false;
}

//----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$

void highlightChecker(CH_Type** board, int x1, int y1)
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
    default:
        break;
    }
}

void unhighlightChecker(CH_Type** board, int x2, int y2)
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
    default:
        break;
    }
}