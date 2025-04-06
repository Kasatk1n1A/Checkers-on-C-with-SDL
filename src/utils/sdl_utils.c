#include "sdl_utils.h"

#define IMAGE_FLAGS IMG_INIT_PNG
#define MAIN_MENU_BACK "Checkers"
#define SCREEN_WIDTH 1400
#define SCREEN_HEIGHT 923

void SDL_cleanup()
{
    TTF_Quit();  // Библиотека шрифтов
    IMG_Quit();  // Библиотека загрузки изображений
    SDL_Quit();  // Основная SDL библиотека
}

bool sdl_initialize()
{
    // Инициализация основных компонентов SDL
    if (SDL_Init(SDL_INIT_EVERYTHING)){
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        return true;  
    }

    if (TTF_Init()){
        fprintf(stderr, "Error initializing TTF: %s\n", TTF_GetError());
        return true;
    }

    // Инициализация SDL_image для работы с изображениями
    int img_init = IMG_Init(IMAGE_FLAGS);
    if ((img_init & IMAGE_FLAGS) != IMAGE_FLAGS) {
        fprintf(stderr, "Error initializing SDL_image: %s\n", IMG_GetError());
        return true;
    }

    // Инициализация генератора случайных чисел
    srand((unsigned)time(NULL));

    return false;
}

bool CreateWindow(Window* window)
{
    // Создание окна приложения
    window->window = SDL_CreateWindow("Checkers", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!window->window){
        fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
        return true;  
    }
    
    // Создание рендерера для отрисовки
    window->renderer = SDL_CreateRenderer(window->window, -1, 0);
    if (!window->renderer){
        fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
        return true;  
    }

    return false;
}

void Window_cleanup(Window* window)
{
    SDL_DestroyRenderer(window->renderer);
    SDL_DestroyWindow(window->window);
    free(window);
}

void Game_cleanup(Window* window, int exit_status)
{        
    Window_cleanup(window);    
    SDL_cleanup();

    // Завершение программы с указанным статусом
    exit(exit_status);
}