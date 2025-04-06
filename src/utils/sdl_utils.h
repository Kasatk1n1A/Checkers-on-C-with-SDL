#ifndef SDL_UTILS_H
#define SDL_UTILS_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../../include/core/ui_types.h"

void SDL_cleanup();
bool sdl_initialize();
bool CreateWindow(Window* window);
void Game_cleanup(Window* window, int exit_status);

#endif