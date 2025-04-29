#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "../core/game_rules.h"
#include "../utils/sdl_utils.h"
#include "../../include/core/ui_types.h"
#include "../../include/core/Board_types.h"
#include "texture_manager.h"
#include "../core/saves.h"

typedef struct
{
    SDL_Texture* Name;
    SDL_Texture* minutes;
    SDL_Texture* seconds;
    SDL_Rect Name_rect;
    SDL_Rect minutes_rect;
    SDL_Rect seconds_rect;
} leader;

void LoseMenu(Window* window);
void WinMenu(Window* window, GameInfo info);
int SaveForLeaders(Window* window, GameInfo info);

void SetDifficult(Window* window);
void showMainMenu(Window* window);

void ShowMiniMenu(Window* window, Board* CheckersBoard, GameInfo info);

void free_leaders(leader* leaders);
void ShowLeaderBoard(Window* window);
void PrintLeaders(SDL_Renderer* renderer, leader* leaders);
leader* read_leaders(SDL_Renderer* renderer, int type, int x, int y);

void CreateTextButton(MenuItem* item, TTF_Font* font, const char* text, int x, int y, int w, int h);
void renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y, SDL_Color text_color, SDL_Color *outline_color);

#endif