#include "libraries.h"

// void board_cleanup(int** board);
bool add_board_SDL(Game* game);
void out_board_SDL(Game* game);

Game* game;

int main()
{
    // Инициализация структуры игры
    game = (Game*)malloc(sizeof(Game));
    game->window = NULL;
    game->renderer = NULL;
    game->background = NULL;
    game->board = NULL;   
    game->text_count = 0; 
    
    // Инициализация SDL
    if (sdl_initialize(game)) 
        game_cleanup(game, EXIT_FAILURE);
    
    // Создание игровой доски
    if (add_board_SDL(game))
        game_cleanup(game, EXIT_FAILURE);
    
    // Создание потока для игровой логики
    SDL_Thread* test_thread = SDL_CreateThread(checkers, "checkers", NULL);

    // Основной цикл обработки событий и отрисовки
    while (true)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event)) 
        {
            switch (event.type) 
            {
            case SDL_QUIT:  // Обработка закрытия окна
                game_cleanup(game, EXIT_SUCCESS);
                break;  
            case SDL_KEYDOWN:  // Обработка нажатий клавиш
                switch (event.key.keysym.scancode)
                {
                case SDL_SCANCODE_ESCAPE:  // Закрытие по ESC
                    game_cleanup(game, EXIT_SUCCESS);
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
        }

        // Блокировка мьютекса для безопасного доступа к ресурсам
        SDL_LockMutex(game->mutex);
        
        // Очистка экрана
        SDL_RenderClear(game->renderer);
            
        // Отрисовка фона
        SDL_RenderCopy(game->renderer, game->background, NULL, NULL);
    
        // Отрисовка игровой доски
        out_board_SDL(game);
           
        // Обновление экрана
        SDL_RenderPresent(game->renderer);

        // Разблокировка мьютекса
        SDL_UnlockMutex(game->mutex);

        // Задержка для контроля FPS
        SDL_Delay(16);
    }
    
    // Ожидание завершения потока игровой логики
    SDL_WaitThread(test_thread, NULL);

    // Очистка ресурсов
    game_cleanup(game, EXIT_SUCCESS);

    return 0;
}

bool add_board_SDL(Game* game)
{
    // Выделение памяти и загрузка текстуры доски
    game->board = (Board*)malloc(sizeof(Board)); 
    game->board->image = IMG_LoadTexture(game->renderer, "images/chessboard.png");     
    if (!game->board->image){
        fprintf(stderr, "Error creating Texture: %s\n", IMG_GetError());
        return true;
    }
    
    // Установка позиции и размеров доски
    game->board->rect.x = 239; 
    game->board->rect.y = 0; 
    game->board->rect.h = 0; 
    game->board->rect.w = 0; 
    
    // Получение реальных размеров текстуры
    if (SDL_QueryTexture(game->board->image, NULL, NULL, &game->board->rect.w, &game->board->rect.h)){
        fprintf(stderr, "Error quering Texture: %s\n", SDL_GetError());
        return true;
    }

    // Создание массива шашек
    game->board->checkers = (Checker***)malloc(sizeof(Checker**) * 8);

    // Инициализация массива шашек NULL-указателями
    for (int i = 0; i < 8; i++){
        game->board->checkers[i] = (Checker**)malloc(sizeof(Checker*) * 8);
        memset(game->board->checkers[i], 0, sizeof(Checker*) * 8);
    }

    // Создание красных шашек (верхние 3 ряда)
    for (int j = 0; j < 3; j++) 
    {
        for (int i = 1 - j % 2; i < 8; i += 2) 
        {
            game->board->checkers[j][i] = (Checker*)malloc(sizeof(Checker));
            // Загрузка текстур для обычной и выделенной шашки
            game->board->checkers[j][i]->default_image = IMG_LoadTexture(game->renderer, "images/red_checker.png");
            game->board->checkers[j][i]->picked_image = IMG_LoadTexture(game->renderer, "images/picked_red_checker.png");
            // Загрузка текстур для дамки и выделенной дамки
            game->board->checkers[j][i]->King_image = IMG_LoadTexture(game->renderer, "images/red_super_checker.png");
            game->board->checkers[j][i]->King_picked_image = IMG_LoadTexture(game->renderer, "images/picked_red_superchecker.png");
            // Установка начального состояния (красная шашка)
            game->board->checkers[j][i]->flag = RED_PAWN;
            // Установка позиции на доске
            game->board->checkers[j][i]->rect.x = 282 + 105 * i;
            game->board->checkers[j][i]->rect.y = 43 + 105 * j;
            game->board->checkers[j][i]->rect.w = 0; 
            game->board->checkers[j][i]->rect.h = 0;
            // Получение реальных размеров текстуры
            if (SDL_QueryTexture(game->board->checkers[j][i]->default_image, NULL, NULL, &game->board->checkers[j][i]->rect.w, &game->board->checkers[j][i]->rect.h)){
                fprintf(stderr, "Error quering Texture: %s\n", SDL_GetError());
                return true;
            }
        }
    }

    // Создание белых шашек (нижние 3 ряда)
    for (int j = 5; j < 8; j++) 
    {
        for (int i = 1 - j % 2; i < 8; i += 2) 
        {
            game->board->checkers[j][i] = (Checker*)malloc(sizeof(Checker));
            // Загрузка текстур для обычной и выделенной шашки
            game->board->checkers[j][i]->default_image = IMG_LoadTexture(game->renderer, "images/white_checker.png");
            game->board->checkers[j][i]->picked_image = IMG_LoadTexture(game->renderer, "images/picked_white_checker.png");
            // Загрузка текстур для дамки и выделенной дамки
            game->board->checkers[j][i]->King_image = IMG_LoadTexture(game->renderer, "images/white_super_checker.png");
            game->board->checkers[j][i]->King_picked_image = IMG_LoadTexture(game->renderer, "images/picked_white_superchecker.png");
            // Установка начального состояния (белая шашка)
            game->board->checkers[j][i]->flag = WHITE_PAWN;
            // Установка позиции на доске
            game->board->checkers[j][i]->rect.x = 282 + 105 * i;
            game->board->checkers[j][i]->rect.y = 43 + 105 * j; 
            game->board->checkers[j][i]->rect.w = 0; 
            game->board->checkers[j][i]->rect.h = 0;
            // Получение реальных размеров текстуры
            if (SDL_QueryTexture(game->board->checkers[j][i]->default_image, NULL, NULL, &game->board->checkers[j][i]->rect.w, &game->board->checkers[j][i]->rect.h)){
                fprintf(stderr, "Error quering Texture: %s\n", SDL_GetError());
                return true;
            }
        }
    }
    return false;
}

void out_board_SDL(Game* game)
{
    // Отрисовка фона доски
    SDL_RenderCopy(game->renderer, game->board->image, NULL, &game->board->rect);
    
    // Отрисовка шашек
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (!game->board->checkers[i][j])
                continue;

            // Выбор текстуры в зависимости от состояния шашки
            switch (game->board->checkers[i][j]->flag)
            {
            case RED_PAWN:
            case WHITE_PAWN:
                // Отрисовка обычной шашки
                SDL_RenderCopy(game->renderer, game->board->checkers[i][j]->default_image, NULL, &game->board->checkers[i][j]->rect);
                break;
            case RED_KING:
            case WHITE_KING:
                // Отрисовка дамки
                SDL_RenderCopy(game->renderer, game->board->checkers[i][j]->King_image, NULL, &game->board->checkers[i][j]->rect);
                break; 
            case PICKED_WHITE_PAWN:
            case PICKED_RED_PAWN:
                // Отрисовка выделенной шашки
                SDL_RenderCopy(game->renderer, game->board->checkers[i][j]->picked_image, NULL, &game->board->checkers[i][j]->rect);
                break; 
            case PICKED_WHITE_KING:
            case PICKED_RED_KING:
                // Отрисовка выделенной дамки
                SDL_RenderCopy(game->renderer, game->board->checkers[i][j]->King_picked_image, NULL, &game->board->checkers[i][j]->rect);
                break; 
            default:
                break;
            }
        }
    }
}