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

void showMainMenu(Window* window);
void ShowMiniMenu(Window* window, Board* CheckersBoard, double Time, Player player);
void renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y, SDL_Color color);

#endif