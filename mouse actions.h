#ifndef MOUSE_ACTIONS
#define MOUSE_ACTIONS



int get_mouse_click(int* x, int* y, Game* game);

int get_mouse_click(int* x, int* y, Game* game)
{
    SDL_Event event;
    int click_detected = 0;

    // Проверяем события в очереди
    while (SDL_PollEvent(&event)) 
    {
        if (event.type == SDL_MOUSEBUTTONDOWN) 
        {
            if (event.button.button == SDL_BUTTON_LEFT) // ЛКМ
            {
            // Блокируем мьютекс для безопасной записи
            SDL_LockMutex(game->mutex);
            
            *x = event.button.x;
            *y = event.button.y;
            click_detected = 1;
            
            // Разблокируем мьютекс
            SDL_UnlockMutex(game->mutex);
            
            // Выходим после первого обнаруженного клика
            break;
            }
        }
    }
    return click_detected;
}

#endif