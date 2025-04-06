#include "main.h"
#include "../../include/core/ui_types.h"
#include "../utils/sdl_utils.h"
#include "../../src/core/game_rules.h"
#include "Menu.h"



int main(void)
{
    // Инициализация структуры игры
    Window* window = (Window*)malloc(sizeof(Window));
    window->window = NULL;
    window->renderer = NULL;

    if (sdl_initialize()) 
        SDL_cleanup();

    if (CreateWindow(window))
        Game_cleanup(window, EXIT_FAILURE);

    int choice = showMainMenu(window);

    switch (choice) 
    {
        case 0: 
            printf("New game selected\n"); 
            checkers(window);
            break;
        case 1: printf("Load game selected\n"); break;
        case 2: printf("Leaderboard selected\n"); break;
        case 3: 
            printf("Quit selected\n"); 
            Game_cleanup(window, EXIT_SUCCESS);
            break;
        default: 
            break;
    }

    return 0;
}