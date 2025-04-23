#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

SDL_Texture* Create_colored_rect(SDL_Renderer* renderer, int w, int h, int r, int g, int b, int a);

#endif