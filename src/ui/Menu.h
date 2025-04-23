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

int InputText(Window* window);
void LoseMenu(Window* window);
void showMainMenu(Window* window);
void SetDifficult(Window* window);
void WinMenu(Window* window, GameInfo info);
void ShowMiniMenu(Window* window, Board* CheckersBoard, GameInfo info);
void renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y, SDL_Color text_color, SDL_Color *outline_color);

#endif