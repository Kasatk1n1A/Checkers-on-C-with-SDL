#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>

#define WINDOW_TITLE "Pudge"
#define SCREEN_WIDTH 632
#define SCREEN_HEIGHT 355
#define TEXT_SIZE 80

struct Game
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *background;
    TTF_Font *Text_font;
    SDL_Color Text_color;
    SDL_Rect Text_rect;
    SDL_Texture *text_image;
    int text_xvel;
    int text_yvel;
    SDL_Texture *sprite_image;
    SDL_Rect sprite_rect;
    int sprite_vel;
    const Uint8* keystate;
};

void game_cleanup(struct Game *game, int exit_status);
bool load_media(struct Game* game);
bool sdl_initialize(struct Game *game);
void text_update(struct Game *game);

int main()
{
    struct Game game = {
        .window = NULL,
        .renderer = NULL,
        .background = NULL,
        .Text_font = NULL,
        .Text_color = {0, 0, 0, 255},
        .Text_rect = {0, 0, 0, 0},
        .text_image = NULL,
        .text_xvel = 3,
        .text_yvel = 3,
        .sprite_image = NULL, 
        .sprite_rect = {100, 100, 0, 0},
        .keystate = SDL_GetKeyboardState(NULL);
    };
    //creating window, if error then stop the programm
    if (sdl_initialize(&game)){
        game_cleanup(&game, EXIT_FAILURE);
    }
    //load image on background, if error then stop the programm
    if (load_media(&game)){
        game_cleanup(&game, EXIT_FAILURE);
    }
    //window stay and check 60 times in second on action
    while (true)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch (event.type)
            {
                case SDL_QUIT:
                    game_cleanup(&game, EXIT_SUCCESS);
                    break;
                //close window when you press escape
                case SDL_KEYDOWN:
                    switch (event.key.keysym.scancode)
                    {
                        //Close game if press esc
                        case SDL_SCANCODE_ESCAPE:
                            game_cleanup(&game, EXIT_SUCCESS);
                            break;
                            //change color of background on white
                        // case SDL_SCANCODE_CAPSLOCK:
                        //     SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 255);
                        //     break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }

        text_update(&game);

        SDL_RenderClear(game.renderer);

        SDL_RenderCopy(game.renderer, game.background, NULL, NULL);

        SDL_RenderCopy(game.renderer, game.text_image, NULL, &game.Text_rect);
        
        SDL_RenderCopy(game.renderer, game.sprite_image, NULL, &game.sprite_rect);

        SDL_RenderPresent(game.renderer);

        SDL_Delay(16);
    }
    //close window
    game_cleanup(&game, EXIT_SUCCESS);

    return 0;
}

bool load_media(struct Game* game){
    game->background = IMG_LoadTexture(game->renderer, "images/background.png");
    if (!game->background) {
        fprintf(stderr, "Error creating Texture: %s\n", IMG_GetError());
        return true;
    }
    //инициализация шрифта в 
    game->Text_font = TTF_OpenFont("fonts/bleedingcowboysrus.ttf", TEXT_SIZE);
    if (!game->Text_font){
        fprintf(stderr, "Error init font: %s\n", TTF_GetError());
        return true;
    }
    //создание текста
    SDL_Surface* surface = TTF_RenderText_Blended(game->Text_font, "PUDGE", game->Text_color);
    if (!surface){
        fprintf(stderr, "Error create surface: %s\n", TTF_GetError());
        return true;
    }
    game->Text_rect.h = surface->h;
    game->Text_rect.w = surface->w;
    game->text_image = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);
    if (!game->text_image){
        fprintf(stderr, "Error create Texture: %s\n", SDL_GetError());
        return true;
    }

    game->sprite_image = IMG_LoadTexture(game->renderer, "images/CM.png");
    if (!game->sprite_image) {
        fprintf(stderr, "Error creating Texture: %s\n", IMG_GetError());
        return true;
    }
    if (SDL_QueryTexture(game->sprite_image, NULL, NULL, &game->sprite_rect.w, &game->sprite_rect.h)){
        fprintf(stderr, "Error quering Texture: %s\n", SDL_GetError());
        return true;
    }

    return false;
}

void game_cleanup(struct Game *game, int exit_status)
{
    SDL_DestroyTexture(game->sprite_image);
    SDL_DestroyTexture(game->text_image);
    SDL_DestroyTexture(game->background);
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    TTF_CloseFont(game->Text_font);
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

    srand((unsigned)time(NULL));

    return false;
}

void text_update(struct Game *game)
{
    game->Text_rect.x += game->text_xvel;
    game->Text_rect.y += game->text_yvel;
    if (game->Text_rect.w + game->Text_rect.x > SCREEN_WIDTH){
        game->text_xvel = -3;
    }
    if (game->Text_rect.x < 0){
        game->text_xvel = 3;
    }
    if (game->Text_rect.h + game->Text_rect.y > SCREEN_HEIGHT){
        game->text_yvel = -3;
    }
    if (game->Text_rect.y < 0){
        game->text_yvel = 3;
    }
}