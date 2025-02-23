#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define WINDOW_TITLE "Pudge"
#define SCREEN_WIDTH 1896
#define SCREEN_HEIGHT 1065

struct Game
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *background;
};

void game_cleanup(struct Game *game, int exit_status);
bool load_media(struct Game* game);
bool sdl_initialize(struct Game *game);

int main()
{
    struct Game game = {
        .window = NULL,
        .renderer = NULL,
        
    };
    //creating window, if error then stop the programm
    if (sdl_initialize(&game)){
        game_cleanup(&game, EXIT_FAILURE);
    }
    if (load_media(&game)){
        game_cleanup(&game, EXIT_FAILURE);
    }

    while (true)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch (event.type){
                case SDL_QUIT:
                    game_cleanup(&game, EXIT_SUCCESS);
                    break;
                //close window when you press escape
                case SDL_KEYDOWN:
                    switch (event.key.keysym.scancode)
                        case SDL_SCANCODE_ESCAPE:
                            game_cleanup(&game, EXIT_SUCCESS);
                            break;
                default:
                    break;
            }
        }
        SDL_RenderClear(game.renderer);

        SDL_RenderCopy(game.renderer, game.background, NULL, NULL);

        SDL_RenderPresent(game.renderer);
        //window stay 5000 miliseconds
        SDL_Delay(16);
    }
    //close window
    game_cleanup(&game, EXIT_SUCCESS);
    printf("All good!");
    return 0;
}

bool load_media(struct Game* game){
    game->background = IMG_LoadTexture(game->renderer, "images/background.png");
    if (!game->background) {
        fprintf(stderr, "Error creating Texture: %s\n", IMG_GetError());
        return true;
    }

    return false;
}

void game_cleanup(struct Game *game, int exit_status)
{
    SDL_DestroyTexture(game->background);
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
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

    //create window
    game->window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,SCREEN_HEIGHT, 0);
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

    return false;
}