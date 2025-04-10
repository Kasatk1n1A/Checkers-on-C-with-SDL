#include "main.h"

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

    showMainMenu(window);

    Game_cleanup(window, EXIT_FAILURE);

    return 0;
}