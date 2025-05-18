#ifndef BOARD_TYPES_H
#define BOARD_TYPES_H

#include "Checkers_types.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>


typedef struct 
{
    int flag;
    SDL_Rect rect;
    SDL_Texture *white_image;
    SDL_Texture *picked_white_image;
    SDL_Texture *white_King_image;
    SDL_Texture *picked_white_king_image;
    SDL_Texture *red_image;
    SDL_Texture *picked_red_image;
    SDL_Texture *red_King_image;
    SDL_Texture *picked_red_king_image;
} Checker;

typedef struct
{
    CH_Type** board;
    SDL_Rect rect;
    SDL_Texture *image;
    Checker* checkers;
} Board;

#endif 