#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>

#define MAIN_MENU_BACK "Main menu back"
#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000

struct Game
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *background;
};
struct Text
{
    TTF_Font *Text_font;
    SDL_Color Text_color;
    SDL_Rect Text_rect;
    SDL_Texture *text_image;
};
struct Checker
{
    char flag;
    SDL_Texture *sprite_image;
    SDL_Rect sprite_rect;
    int check_xvel;
    int check_yvel;
};

void Lets_game(struct Game game);
bool load_back(struct Game *game, char* file);
void Back_cleanup(struct Game *game);
bool Load_Media(struct Game *game);
void Media_cleanup(struct Game *game);
bool sdl_initialize(struct Game *game);
void game_cleanup(struct Game *game, int exit_status);

int main()
{
    struct Game game = {
        .window = NULL,
        .renderer = NULL,
        .background = NULL,
    };

    if (sdl_initialize(&game)){
        game_cleanup(&game, EXIT_FAILURE);
    }

    while (true) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                game_cleanup(&game, EXIT_SUCCESS);
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_ESCAPE:
                    game_cleanup(&game, EXIT_SUCCESS);
                    break;
                default:
                    break;
                }
            default:
                break;
            }
        }
        
        SDL_RenderClear(game.renderer);

        SDL_RenderCopy(game.renderer, game.background, NULL, NULL);
        
        SDL_RenderPresent(game.renderer);

        SDL_Delay(16);
    }

    Back_cleanup(&game);
    game_cleanup(&game, EXIT_SUCCESS);
}

void Lets_game(struct Game game)
{
    while (true) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                game_cleanup(&game, EXIT_SUCCESS);
                break;
            default:
                break;
            }
        }
        SDL_RenderClear(game.renderer);

        SDL_RenderCopy(game.renderer, game.background, NULL, NULL);

        SDL_RenderPresent(game.renderer);

        SDL_Delay(16);
    }
}

void game_cleanup(struct Game *game, int exit_status)
{
    SDL_DestroyTexture(game->background);
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    exit(exit_status);
}

bool sdl_initialize(struct Game *game)
{
    //initializing SDL tools
    if (SDL_Init(SDL_INIT_EVERYTHING)){
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        return true;  
    }

    int img_init = IMG_Init(IMG_INIT_PNG);
    if ((img_init & IMG_INIT_PNG) != IMG_INIT_PNG){
        fprintf(stderr, "Error initializing SDL_IMAGE_PNG: %s\n", IMG_GetError());
        return true;  
    }

    if (TTF_Init()){
        fprintf(stderr, "Error initializing SDL_TTF: %s\n", TTF_GetError());
        return true;  
    }

    //create window
    game->window = SDL_CreateWindow("Checkers", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!game->window){
        fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
        return true;  
    }
    //create renderer
    game->renderer = SDL_CreateRenderer(game->window, -1, 0);
    if (!game->renderer){
        fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
        return true;  
    }

    game->background = IMG_LoadTexture(game->renderer, "images/background.png");
    if (!game->background) {
        fprintf(stderr, "Error creating Texture: %s\n", IMG_GetError());
        return true;
    }

    srand((unsigned)time(NULL));

    return false;
}