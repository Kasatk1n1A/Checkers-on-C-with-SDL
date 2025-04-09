
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

Choice get_mouse_click(int* x, int* y, Window* window, Board* CheckersBoard, double Time, Player player)
{
    SDL_Event event;
    int last_highlighted_x = -1, last_highlighted_y = -1; // Для хранения последнего подсвеченного поля
    
    // Константы для доски
    const int CELL_SIZE = 105;
    const int BOARD_START_X = 282;
    const int BOARD_START_Y = 41;
    
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
                    ShowMiniMenu(window, CheckersBoard, Time, player);
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
        
        // Обработка движения мыши (не событие, проверяется каждый кадр)
        int mouse_x, mouse_y;
        SDL_GetMouseState(&mouse_x, &mouse_y);
        
        // Проверяем, находится ли курсор над доской
        if (mouse_x >= BOARD_START_X && mouse_x < BOARD_START_X + 8*CELL_SIZE &&
            mouse_y >= BOARD_START_Y && mouse_y < BOARD_START_Y + 8*CELL_SIZE)
        {
            // Вычисляем координаты клетки
            int cell_x = (mouse_x - BOARD_START_X) / CELL_SIZE;
            int cell_y = (mouse_y - BOARD_START_Y) / CELL_SIZE;
            
            // Если курсор перешел на новую клетку
            if (cell_x != last_highlighted_x || cell_y != last_highlighted_y)
            {
                // Убираем подсветку с предыдущей клетки
                if (last_highlighted_x != -1 && last_highlighted_y != -1)
                {
                    // Здесь нужно добавить код для отрисовки клетки без подсветки
                    // Например: DrawBoardCell(window, CheckersBoard, last_highlighted_x, last_highlighted_y, false);
                }
                
                // Подсвечиваем новую клетку
                // Например: DrawBoardCell(window, CheckersBoard, cell_x, cell_y, true);
                
                // Обновляем экран
                SDL_RenderPresent(window->renderer);
                
                // Запоминаем новую подсвеченную клетку
                last_highlighted_x = cell_x;
                last_highlighted_y = cell_y;
            }
        }
        else if (last_highlighted_x != -1 && last_highlighted_y != -1)
        {
            // Курсор вне доски - убираем подсветку
            // Например: DrawBoardCell(window, CheckersBoard, last_highlighted_x, last_highlighted_y, false);
            SDL_RenderPresent(window->renderer);
            last_highlighted_x = -1;
            last_highlighted_y = -1;
        }
        
        // Небольшая задержка для уменьшения нагрузки на CPU
        SDL_Delay(10);
    }
    
    return 0;
}