#ifndef VISUALISE
#define VISUALISE

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>
#include "checkers and board/StructsAndEnum.h"

//----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$

#define MAIN_MENU_BACK "Checkers"
#define SCREEN_WIDTH 1400
#define SCREEN_HEIGHT 923
#define IMAGE_FLAGS IMG_INIT_PNG

typedef struct 
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *background;
} Game;

//----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$

Game* game;

//----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$

bool sdl_initialize(Game *game);
void game_cleanup(Game *game, int exit_status);

//----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$

bool sdl_initialize(Game *game)
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

    // Создание окна приложения
    game->window = SDL_CreateWindow("Checkers", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!game->window){
        fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
        return true;  
    }
    
    // Создание рендерера для отрисовки
    game->renderer = SDL_CreateRenderer(game->window, -1, 0);
    if (!game->renderer){
        fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
        return true;  
    }

    // Инициализация генератора случайных чисел
    srand((unsigned)time(NULL));

    return false;
}

void game_cleanup(Game *game, int exit_status)
{        
    // Освобождение графических ресурсов
    SDL_DestroyTexture(game->background);
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    
    // Завершение работы библиотек
    TTF_Quit();  // Библиотека шрифтов
    IMG_Quit();  // Библиотека загрузки изображений
    SDL_Quit();  // Основная SDL библиотека
    
    // Освобождение структуры игры
    free(game);
    
    // Завершение программы с указанным статусом
    exit(exit_status);
}



#endif