#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>

#define MAIN_MENU_BACK "Main menu back"
#define SCREEN_WIDTH 1400
#define SCREEN_HEIGHT 923

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
    char color;
    char flag;
    SDL_Rect rect;
    SDL_Texture *image;
    int check_xvel;
    int check_yvel;
};
struct Board
{
    SDL_Rect rect;
    SDL_Texture *image;
    int arrangment[8][8];
};

void create_board()
void arrangment(int *arrang);
bool add_checker(struct Checker *checker, struct Game *game, char flag);
void checker_cleanup(struct Checker *checker);
bool add_chessboard(struct Board *board, struct Game *game);
void board_cleanup(struct Board *board);
void Lets_game(struct Game *game);
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

    Lets_game(&game);

    game_cleanup(&game, EXIT_SUCCESS);
}

void Lets_game(struct Game *game)
{
    struct Board board = 
    {
        .rect = {239, 0, 0, 0},
        .image = NULL,
        .arrangment = {
        {0, 1, 0, 1, 0, 1, 0, 1};
        {1, 0, 1, 0, 1, 0, 1, 0};
        {0, 1, 0, 1, 0, 1, 0, 1};
        {0, 0, 0, 0, 0, 0, 0, 0};
        {0, 0, 0, 0, 0, 0, 0, 0};
        {2, 0, 2, 0, 2, 0, 2, 0};
        {0, 2, 0, 2, 0, 2, 0, 2};
        {2, 0, 2, 0, 2, 0, 2, 0};
        },
    };

    if (add_chessboard(&board, game)){
        board_cleanup(&board);
        game_cleanup(game, EXIT_FAILURE);
    }   

    struct Checker checker = 
    {
        .color = 'r',
        .flag = 'u',
        .rect = {282, 43, 0, 0},
        .image = NULL,
        .check_xvel = 0,
        .check_yvel = 0,
    };

    if (add_checker(&checker, game, checker.color)){
        checker_cleanup(&checker);
        game_cleanup(game, EXIT_FAILURE);
    }   

    while (true) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                game_cleanup(game, EXIT_SUCCESS);
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_ESCAPE:
                    game_cleanup(game, EXIT_SUCCESS);
                    break;
                default:
                    break;
                }
            default:
                break;
            }
        }
        
        SDL_RenderClear(game->renderer);

        SDL_RenderCopy(game->renderer, game->background, NULL, NULL);
        
        SDL_RenderCopy(game->renderer, board.image, NULL, &board.rect);
        
        SDL_RenderCopy(game->renderer, checker.image, NULL, &checker.rect);

        SDL_RenderPresent(game->renderer);

        SDL_Delay(16);
    }
    board_cleanup(&board);
    checker_cleanup(&checker);
}

bool add_checker(struct Checker *checker, struct Game *game, char flag)
{
    if (flag == 'w')
        checker->image = IMG_LoadTexture(game->renderer, "images/white_checker.png");
    else
        checker->image = IMG_LoadTexture(game->renderer, "images/red_checker.png");
    if (!checker->image) {
        fprintf(stderr, "Error creating Texture: %s\n", IMG_GetError());
        return true;
    }
    if (SDL_QueryTexture(checker->image, NULL, NULL, &checker->rect.w, &checker->rect.h)){
        fprintf(stderr, "Error quering Texture: %s\n", SDL_GetError());
        return true;
    }

    return false;
}

void checker_cleanup(struct Checker *checker)
{
    SDL_DestroyTexture(checker->image);
}

bool add_chessboard(struct Board *board, struct Game *game)
{
    board->image = IMG_LoadTexture(game->renderer, "images/chessboard.png");
    if (!board->image) {
        fprintf(stderr, "Error creating Texture: %s\n", IMG_GetError());
        return true;
    }
    if (SDL_QueryTexture(board->image, NULL, NULL, &board->rect.w, &board->rect.h)){
        fprintf(stderr, "Error quering Texture: %s\n", SDL_GetError());
        return true;
    }

    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
        {
            if (board->arrangement[i][j] == 1){
                add_checker()
            }

        }

    return false;
}

void board_cleanup(struct Board *board)
{
    SDL_DestroyTexture(board->image);
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