#include "libraries.h"

// void board_cleanup(int** board);

Game* game;

int main()
{

    
    // Инициализация SDL
    if (sdl_initialize(game)) 
        game_cleanup(game, EXIT_FAILURE);
    
    NIGGERS(game);
    
    // Ожидание завершения потока игровой логики
    // SDL_WaitThread(test_thread, NULL);

    // Очистка ресурсов
    game_cleanup(game, EXIT_SUCCESS);

    return 0;
}