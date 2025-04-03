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

bool load_media(Game* game);
void text_out(Text* Texts, int N);
void board_cleanup_SDL(Board* board);
void text_cleanup(Text* text, int N);
void checker_cleanup_SDL(Checker* checker);
bool load_text(Game* game, char* text, int r, int g, int b, int a, int x, int y, int size);

//----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$

void app_cleanup(Game *game, int exit_status)
{    
    // Очистка текстовых элементов (если существуют)
    if (game->texts){
        text_cleanup(game->texts, game->text_count);
    }
    
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

bool load_media(Game* game)
{
    // Загрузка текстовых сообщений для игры
    if (load_text(game, "Red Turn", 0, 0, 0, 255, 100, 400, 80))
        return true;
    if (load_text(game, "White Turn", 0, 0, 0, 255, 100, 400, 80))
        return true;
    return false;
}

bool load_text(Game* game, char* text, int r, int g, int b, int a, int x, int y, int size)
{
    // Выделение памяти для нового текста (расширение массива при необходимости)
    if (game->text_count == 0)
        game->texts = (Text*)malloc(sizeof(Text));
    else
    {
        // Создание временного массива с увеличенным размером
        Text* tmp = (Text*)malloc(sizeof(Text) * (game->text_count + 1));
        
        // Копирование существующих текстов
        for (int i = 0; i < game->text_count + 1; i++)
        {
            memcpy(&tmp[i], &game->texts[i], sizeof(Text));
        }
        
        // Замена старого массива новым
        free(game->texts);
        game->texts = tmp;
    }
    
    // Увеличение счетчика текстов
    game->text_count++;

    // Установка цвета текста
    game->texts[game->text_count - 1].color.r = r;
    game->texts[game->text_count - 1].color.g = g;
    game->texts[game->text_count - 1].color.b = b;
    game->texts[game->text_count - 1].color.a = a;

    // Загрузка шрифта указанного размера
    game->texts[game->text_count - 1].font = TTF_OpenFont("fonts/bleedingcowboysrus.ttf", size);
    if (!game->texts[game->text_count - 1].font) {
        fprintf(stderr, "Error creating Font: %s\n", TTF_GetError());
        return true;
    }

    // Создание поверхности с текстом
    SDL_Surface *surface = TTF_RenderText_Blended(game->texts[game->text_count - 1].font, text, game->texts[game->text_count - 1].color);
    if (!surface) {
        fprintf(stderr, "Error creating Surface: %s\n", SDL_GetError());
        return true;
    }

    // Создание текстуры из поверхности
    game->texts[game->text_count - 1].image = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);  // Освобождение поверхности
    if (!game->texts[game->text_count - 1].image) {
        fprintf(stderr, "Error creating Texture: %s\n", SDL_GetError());
        return true;
    }

    // Установка параметров отображения текста
    game->texts[game->text_count - 1].Rect.w = surface->w;
    game->texts[game->text_count - 1].Rect.h = surface->h;
    game->texts[game->text_count - 1].Rect.x = 700 - surface->w / 2;  // Центрирование по X
    game->texts[game->text_count - 1].Rect.y = y;
    game->texts[game->text_count - 1].flag = false;  // По умолчанию текст скрыт

    return false;
}

void text_cleanup(Text* text, int N)
{
    // Удаление всех текстовых элементов
    for (int i = 0; i < N; i++)
    {
        // Удаление текстуры текста
        if (text[i].image)
            SDL_DestroyTexture(text[i].image);
        
        // Закрытие шрифта
        if (text[i].font)
            TTF_CloseFont(text[i].font);
    }
    
    // Освобождение массива текстов
    free(text);
}

bool sdl_initialize(Game *game)
{
    // Инициализация основных компонентов SDL
    if (SDL_Init(SDL_INIT_EVERYTHING)){
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
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