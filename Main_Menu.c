#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#define IMAGE_FLAGS IMG_INIT_PNG
#define SCREEN_WIDTH 1400
#define SCREEN_HEIGHT 923
#define FPS 60
#define FONT_SIZE 72  // Увеличенный размер шрифта

typedef struct {
    SDL_Rect rect;
    const char* text;
    bool hovered;
} MenuItem;

void renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, 
                int x, int y, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

int showMainMenu(SDL_Renderer* renderer) {
    if (TTF_Init() == -1) {
        fprintf(stderr, "TTF_Init error: %s", TTF_GetError());
        return -1;
    }

    // Загрузка шрифта с увеличенным размером
    TTF_Font* font = TTF_OpenFont("fonts/freesansbold.ttf", FONT_SIZE);
    if (!font) {
        fprintf(stderr, "Failed to load font: %s", TTF_GetError());
        TTF_Quit();
        return -1;
    }

    SDL_Texture* background = IMG_LoadTexture(renderer, "images/Kover2.png");
    if (!background) {
        fprintf(stderr, "Error creating Texture: %s\n", IMG_GetError());
        return -1;
    }

    // Пункты меню с увеличенными размерами и отступами
    MenuItem items[4] = {
        {{SCREEN_WIDTH/2 - 200, 200, 400, 80}, "New game", false},
        {{SCREEN_WIDTH/2 - 200, 300, 400, 80}, "Load game", false},
        {{SCREEN_WIDTH/2 - 200, 400, 400, 80}, "Leaderboard", false},
        {{SCREEN_WIDTH/2 - 200, 500, 400, 80}, "Quit", false}
    };

    SDL_Color black = {255, 255, 255, 255};
    SDL_Color red = {0, 0, 255, 255};

    bool running = true;
    int selectedItem = -1;

    while (running) {
        Uint32 frameStart = SDL_GetTicks();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    selectedItem = 3;
                    break;
                case SDL_MOUSEMOTION: {
                    int x = event.motion.x;
                    int y = event.motion.y;

                    for (int i = 0; i < 4; i++) {
                        items[i].hovered = (x >= items[i].rect.x && 
                                          x <= items[i].rect.x + items[i].rect.w && 
                                          y >= items[i].rect.y && 
                                          y <= items[i].rect.y + items[i].rect.h);
                    }
                    break;
                }
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        for (int i = 0; i < 4; i++) {
                            if (items[i].hovered) {
                                selectedItem = i;
                                running = false;
                                break;
                            }
                        }
                    }
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, background, NULL, NULL);

        for (int i = 0; i < 4; i++) {
            SDL_Color color = items[i].hovered ? red : black;
            // Центрирование текста внутри увеличенных прямоугольников
            int textWidth, textHeight;
            TTF_SizeText(font, items[i].text, &textWidth, &textHeight);
            int textX = items[i].rect.x + (items[i].rect.w - textWidth) / 2;
            int textY = items[i].rect.y + (items[i].rect.h - textHeight) / 2;
            
            renderText(renderer, font, items[i].text, textX, textY, color);
        }

        SDL_RenderPresent(renderer);

        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < 1000/FPS) {
            SDL_Delay(1000/FPS - frameTime);
        }
    }

    // switch (selectedItem)
    // {
    //     case 0:
    //         checkers()
    // }

    SDL_DestroyTexture(background);
    TTF_CloseFont(font);
    TTF_Quit();

}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Main Menu", 
                                        SDL_WINDOWPOS_CENTERED, 
                                        SDL_WINDOWPOS_CENTERED,
                                        SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    int img_init = IMG_Init(IMAGE_FLAGS);
    if ((img_init & IMAGE_FLAGS) != IMAGE_FLAGS) {
        fprintf(stderr, "Error initializing SDL_image: %s\n", IMG_GetError());
    }

    int choice = showMainMenu(renderer);

    switch (choice) {
        case 0: printf("New game selected\n"); break;
        case 1: printf("Load game selected\n"); break;
        case 2: printf("Leaderboard selected\n"); break;
        case 3: printf("Quit selected\n"); break;
        default: break;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}