#ifndef MOUSE_ACTIONS
#define MOUSE_ACTIONS

#include "visualise.h"

Game* game;

void playerAction(Game* game);
int get_mouse_click(int* x, int* y, Game* game);

void playerAction(Game* game)
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) 
    {
        switch (event.type) 
        {
        case SDL_QUIT:  // Обработка закрытия окна
            printf("Quit\n");
            SDL_LockMutex(game->mutex);
            game_cleanup(game, EXIT_SUCCESS);
            SDL_UnlockMutex(game->mutex);
            break;  
        case SDL_KEYDOWN:  // Обработка нажатий клавиш
            switch (event.key.keysym.scancode)
            {
            case SDL_SCANCODE_ESCAPE:  // Закрытие по ESC
                SDL_LockMutex(game->mutex);
                game_cleanup(game, EXIT_SUCCESS);
                SDL_UnlockMutex(game->mutex);
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
}

int get_mouse_click(int* x, int* y, Game* game)
{
    SDL_Event event;
    int click_detected = 0;

    // Проверяем события в очереди
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
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT) // ЛКМ
            {            
                *x = event.button.x;
                *y = event.button.y;
                click_detected = 1;
            }
            // Выходим после первого обнаруженного клика
            break;
    }
    }
    return click_detected;
}

#endif