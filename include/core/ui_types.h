#ifndef UI_TYPES_H
#define UI_TYPES_H

#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

typedef struct 
{
    SDL_Window *window;
    SDL_Renderer *renderer;
} Window;

typedef struct {
    SDL_Rect rect;
    const char* text;
    bool hovered;
} MenuItem;

#endif