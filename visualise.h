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

struct Text;
struct Checker;
struct Board;
struct Game;

typedef struct
{
    bool flag;
    TTF_Font *font;
    SDL_Color color;
    SDL_Rect Rect;
    SDL_Texture *image;
} Text;

typedef struct
{
    bool ClickDetected;
    int x; 
    int y;
} MousePos;

typedef struct 
{
    Text* texts;
    int text_count;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *background;
    SDL_mutex* mutex;
    MousePos Mouse;
    SDL_cond* cond;
} Game;

//----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$

Game* game;

//----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$

void board_cleanup_SDL(Board* board);
void checker_cleanup_SDL(Checker* checker);

//----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$

void app_cleanup(Game *game, int exit_status)
{        
    // Уничтожение объектов синхронизации
    SDL_DestroyMutex(game->mutex);
    SDL_DestroyCond(game->cond);
    
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

void text_out(Text* Texts, int N)
{
    // Отрисовка всех активных текстовых элементов
    for (int i = 0; i < N; i++)
    {
        if (Texts[i].flag)  // Проверка флага видимости
        {
            // Отрисовка текста
            SDL_RenderCopy(game->renderer, Texts[i].image, NULL, &Texts[i].Rect);
        }
    }
}

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
    
    // Загрузка фоновой текстуры
    game->background = IMG_LoadTexture(game->renderer, "images/background.png");
    if (!game->background) {
        fprintf(stderr, "Error creating Texture: %s\n", IMG_GetError());
        return true;
    }

    // Инициализация объектов синхронизации
    game->mutex = SDL_CreateMutex();
    game->cond = SDL_CreateCond();

    // Инициализация генератора случайных чисел
    srand((unsigned)time(NULL));

    return false;
}

#endif