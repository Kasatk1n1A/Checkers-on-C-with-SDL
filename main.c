#include "libraries.h"

// void board_cleanup(int** board);

Game* game;

int main()
{
    // Инициализация структуры игры
    game = (Game*)malloc(sizeof(Game));
    game->window = NULL;
    game->renderer = NULL;
    game->background = NULL; 
    game->text_count = 0; 
    
    // Инициализация SDL
    if (sdl_initialize(game)) 
        app_cleanup(game, EXIT_FAILURE);
    
    NIGGERS(game);
    
    // Ожидание завершения потока игровой логики
    // SDL_WaitThread(test_thread, NULL);

    // Очистка ресурсов
    app_cleanup(game, EXIT_SUCCESS);

    return 0;
}