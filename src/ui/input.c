
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

Choice get_mouse_click(int* x, int* y, Window* window, Board* CheckersBoard, GameInfo info)
{
    SDL_Event event;
    
    while (true)
    {
        // Проверяем события в очереди
        while (SDL_PollEvent(&event)) 
        {
            switch (event.type)
            {
            case SDL_QUIT:
                Game_cleanup(window, EXIT_SUCCESS);
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode)
                {
                case SDL_SCANCODE_ESCAPE:
                    ShowMiniMenu(window, CheckersBoard, info);
                    break;
                case SDL_SCANCODE_RETURN:
                    return ENTER;
                default:
                    break;
                }
                break;
                
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {            
                    *x = event.button.x;
                    *y = event.button.y;
                    return MOUSE_LEFT;
                }
                break;
            }
        }
        
        // Небольшая задержка для уменьшения нагрузки на CPU
        SDL_Delay(10);
    }
    
    return 0;
}