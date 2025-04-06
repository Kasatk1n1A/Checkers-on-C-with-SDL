
#include "input.h"

void playerAction(Window* window)
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) 
    {
        switch (event.type) 
        {
        case SDL_QUIT:  // Обработка закрытия окна
            Game_cleanup(window, EXIT_SUCCESS);
            break;  
        case SDL_KEYDOWN:  // Обработка нажатий клавиш
            switch (event.key.keysym.scancode)
            {
            case SDL_SCANCODE_ESCAPE:  // Закрытие по ESC
                Game_cleanup(window, EXIT_SUCCESS);
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

Choice get_mouse_click(int* x, int* y, Window* window)
{
    SDL_Event event;

    while (true)
    {
    // Проверяем события в очереди
        while (SDL_PollEvent(&event)) 
        {
            switch (event.type)
            {
            case SDL_QUIT:  // Обработка закрытия окна
                Game_cleanup(window, EXIT_SUCCESS);
                break;  

            case SDL_KEYDOWN:  // Обработка нажатий клавиш
                switch (event.key.keysym.scancode)
                {
                case SDL_SCANCODE_ESCAPE:  // Закрытие по ESC
                    // MiniMenu(game, CheckersBoard);
                    break;
                case SDL_SCANCODE_RETURN:
                    return ENTER;
                default:
                    break;
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) // ЛКМ
                {            
                    *x = event.button.x;
                    *y = event.button.y;
                }
                // Выходим после первого обнаруженного клика
                return MOUSE_LEFT;
                break;
            }
        }
    }
    return 0;
}