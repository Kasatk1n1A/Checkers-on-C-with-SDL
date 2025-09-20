#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "../core/game_rules.h"
#include "../utils/sdl_utils.h"
#include "../../include/core/ui_types.h"
#include "../../include/core/Board_types.h"
#include "../core/saves.h"
#include <pthread.h>

typedef struct
{
    SDL_Texture* Name;
    SDL_Texture* minutes;
    SDL_Texture* seconds;
    SDL_Rect Name_rect;
    SDL_Rect minutes_rect;
    SDL_Rect seconds_rect;
} leader;

typedef struct 
{
    SDL_Texture** text_arr;
    SDL_Rect* rect_arr;
} about;

typedef struct 
{
    SDL_Rect out_rect;
    SDL_Rect inner_rect;
    bool hovered;
    union {
        struct {
            SDL_Texture* inner_texture_disable;
            SDL_Texture* inner_texture_enable;
        };
        SDL_Texture* inner_textures[2]; // Альтернативный доступ к тем же текстурам
    };
    SDL_Texture* out_texture;
} Button;

void selectMenu(Window* window);
void ShowMiniMenu(Window* window, Board* CheckersBoard, GameInfo info);
void SetDifficult(void);
void showMainMenu(void);

void SaveGame(Window* window, Board* CheckersBoard, GameInfo info);
void SaveGame_InputText(Window* window, Board* CheckersBoard, GameInfo info);

void LoadGame(void);
void LoadGame_InputText(Window* window);

void LoseMenu(Window* window);
void WinMenu(Window* window, GameInfo info);
int WinMenu_InputText(Window* window, GameInfo info);

void free_about(about* About);
void ShowAbout(Window* window);
about* CreateAboutText(SDL_Renderer* renderer);

void free_leaders(leader* leaders);
void ShowLeaderBoard(Window* window);
void PrintLeaders(SDL_Renderer* renderer, leader* leaders);
leader* read_leaders(SDL_Renderer* renderer, int type, int x, int y);

void renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y, SDL_Color text_color, SDL_Color *outline_color);

void FreeButtons(Button* buttons, int count);
void renderButton(SDL_Renderer* renderer, Button* buttons);
void CreateTextButton(SDL_Renderer* renderer, Button* buttons, TTF_Font* font, const char* text, int x, int y);

#endif