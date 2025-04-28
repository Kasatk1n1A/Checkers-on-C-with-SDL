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
    char* Name;
    char* minutes;
    char* seconds;
} leader;

void LoseMenu(Window* window);
leader* read_leaders(int type);
void SetDifficult(Window* window);
void showMainMenu(Window* window);
void free_leaders(leader* leaders);
void ShowLeaderBoard(Window* window);
void WinMenu(Window* window, GameInfo info);
int InputText(Window* window, GameInfo info);
void ShowMiniMenu(Window* window, Board* CheckersBoard, GameInfo info);
void PrintLeaders(SDL_Renderer* renderer, leader* leaders, TTF_Font* font, int x, int y);
void renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y, SDL_Color text_color, SDL_Color *outline_color);

#endif