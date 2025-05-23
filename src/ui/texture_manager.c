#include "texture_manager.h"

SDL_Texture* Create_colored_rect(SDL_Renderer* renderer, int w, int h, int r, int g, int b, int a)
{
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(
        0,              // флаги (0 - по умолчанию)
        w,          // ширина
        h,         // высота
        32,             // глубина цвета (32 бита)
        SDL_PIXELFORMAT_RGBA32  // формат пикселей
    );
    if (!surface)
    {
        fprintf(stderr, "Failed to create surface:%s", SDL_GetError());
        return NULL;
    }

    SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, r, g, b, a));
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture)
    {
        fprintf(stderr, "Failed to create texture from surface:%s", SDL_GetError());
        SDL_FreeSurface(surface);
        return NULL;
    }

    SDL_FreeSurface(surface);

    return texture;
}