#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>

#define MAIN_MENU_BACK "Checkers"
#define SCREEN_WIDTH 1400
#define SCREEN_HEIGHT 923
#define IMAGE_FLAGS IMG_INIT_PNG

struct Text;
struct Checker;
struct Board;
struct Game;

typedef struct
{
    bool flag;
    TTF_Font *font;
    SDL_Color color;
    SDL_Rect Rect;
    SDL_Texture *image;
} Text;

typedef struct 
{
    int flag;
    SDL_Rect rect;
    SDL_Texture *default_image;
    SDL_Texture *picked_image;
    SDL_Texture *King_image;
    SDL_Texture *King_picked_image;
} Checker;

typedef struct
{
    int x; 
    int y;
} MousePos;

typedef struct
{
    SDL_Rect rect;
    SDL_Texture *image;
    Checker*** checkers;
} Board;

typedef struct 
{
    Text* texts;
    int text_count;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *background;
    Board* board;
    SDL_mutex* mutex;
    MousePos Mouse;
    SDL_cond* cond;
} Game;

Game* game;

void checker_cleanup_SDL(Checker* checker);
void board_cleanup_SDL(Board* board);
void text_cleanup(Text* text, int N);
bool load_media(Game* game);
bool load_text(Game* game, char* text, int r, int g, int b, int a, int x, int y, int size);
void text_out(Text* Texts, int N);
// void create_board();
// void arrangment(int *arrang);
// bool add_checker(struct Checker *checker, struct Game *game, char flag);
// void checker_cleanup(struct Checker *checker);
// void checker_cleanup(struct Checker* checker);
// bool add_chessboard(struct Board *board, struct Game *game);
// void board_cleanup(struct Board *board);
// void Lets_game(struct Game *game);
// bool Load_Media(struct Game *game);
// void Media_cleanup(struct Game *game);
// bool sdl_initialize(struct Game *game);
// void game_cleanup(struct Game *game, int exit_status);

void game_cleanup(Game *game, int exit_status)
{
    if (game->board){
        board_cleanup_SDL(game->board);
    }
    if (game->texts){
        text_cleanup(game->texts, game->text_count);
    }
    SDL_DestroyMutex(game->mutex);
    SDL_DestroyCond(game->cond);
    SDL_DestroyTexture(game->background);
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    free(game);
    exit(exit_status);
}

void text_out(Text* Texts, int N)
{
    for (int i = 0; i < N; i++)
    {
        if (Texts[i].flag)
        {
            SDL_RenderCopy(game->renderer, Texts[i].image, NULL, &Texts[i].Rect);
        }
    }
}

bool load_media(Game* game)
{
    if (load_text(game, "Red Turn", 0, 0, 0, 255, 100, 400, 80))
        return true;
    if (load_text(game, "White Turn", 0, 0, 0, 255, 100, 400, 80))
        return true;
    return false;
}

bool load_text(Game* game, char* text, int r, int g, int b, int a, int x, int y, int size)
{
    if (game->text_count == 0)
        game->texts = (Text*)malloc(sizeof(Text));
    else
    {
        Text* tmp = (Text*)malloc(sizeof(Text) * (game->text_count + 1));
        for (int i = 0; i < game->text_count + 1; i++)
        {
            memcpy(&tmp[i], &game->texts[i], sizeof(Text));
        }
        free(game->texts);
        game->texts = tmp;
    }
    game->text_count++;

    game->texts[game->text_count - 1].color.r = r;
    game->texts[game->text_count - 1].color.g = g;
    game->texts[game->text_count - 1].color.b = b;
    game->texts[game->text_count - 1].color.a = a;

    game->texts[game->text_count - 1].font = TTF_OpenFont("fonts/bleedingcowboysrus.ttf", size);
    if (!game->texts[game->text_count - 1].font) {
        fprintf(stderr, "Error creating Font: %s\n", TTF_GetError());
        return true;
    }

    SDL_Surface *surface = TTF_RenderText_Blended(game->texts[game->text_count - 1].font, text, game->texts[game->text_count - 1].color);
    if (!surface) {
        fprintf(stderr, "Error creating Surface: %s\n", SDL_GetError());
        return true;
    }

    game->texts[game->text_count - 1].image = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);
    if (!game->texts[game->text_count - 1].image) {
        fprintf(stderr, "Error creating Texture: %s\n", SDL_GetError());
        return true;
    }

    game->texts[game->text_count - 1].Rect.w = surface->w;
    game->texts[game->text_count - 1].Rect.h = surface->h;
    game->texts[game->text_count - 1].Rect.x = 700 - surface->w / 2;
    game->texts[game->text_count - 1].Rect.y = y;
    game->texts[game->text_count - 1].flag = false;

    return false;
}

void text_cleanup(Text* text, int N)
{
    for (int i = 0; i < N; i++)
    {
        if (text[i].image)
            SDL_DestroyTexture(text[i].image);
        if (text[i].font)
            TTF_CloseFont(text[i].font);
    }
    free(text);
}

void board_cleanup_SDL(Board* board)
{
    if (board->image)
        SDL_DestroyTexture(board->image);
    
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if (board->checkers[i][j])
                checker_cleanup_SDL(board->checkers[i][j]);
        }
        free(board->checkers[i]);
    }
    free(board->checkers);
    free(board);
}

void checker_cleanup_SDL(Checker* checker)
{
    if (!checker->flag)
        return;
    if (checker->default_image){
        SDL_DestroyTexture(checker->default_image);
    }
    if (checker->picked_image){
        SDL_DestroyTexture(checker->picked_image);
    }
    if (checker->King_image){
        SDL_DestroyTexture(checker->King_image);
    }
    if (checker->King_picked_image){
        SDL_DestroyTexture(checker->King_picked_image);
    }
    free(checker);
    checker = NULL;
}

bool sdl_initialize(Game *game)
{
    //initializing SDL tools
    if (SDL_Init(SDL_INIT_EVERYTHING)){
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        return true;  
    }

    int img_init = IMG_Init(IMAGE_FLAGS);
    if ((img_init & IMAGE_FLAGS) != IMAGE_FLAGS) {
        fprintf(stderr, "Error initializing SDL_image: %s\n", IMG_GetError());
        return true;
    }

    // if (TTF_Init()) {
    //     fprintf(stderr, "Error initializing SDL_ttf: %s\n", IMG_GetError());
    //     return true;
    // }

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
    
    //Инициализирует текстуру заднего фона
    game->background = IMG_LoadTexture(game->renderer, "images/background.png");
    if (!game->background) {
        fprintf(stderr, "Error creating Texture: %s\n", IMG_GetError());
        return true;
    }

    game->mutex = SDL_CreateMutex();
    game->cond = SDL_CreateCond();

    srand((unsigned)time(NULL));

    return false;
}

// bool add_checker(struct Checker *checker, struct Game* game, char flag, int x, int y)
// {
//     checker->flag = flag;
//     checker->rect.x += 105 * x;
//     checker->rect.y += 105 * y;
//     switch (flag)
//     {
//         case 'w':
//             checker->image = IMG_LoadTexture(game->renderer, "images/white_checker.png");
//             break;
//         case 'r':
//             checker->image = IMG_LoadTexture(game->renderer, "images/red_checker.png");
//             break;
//         case 'R':
//             checker->image = IMG_LoadTexture(game->renderer, "images/red_super_checker.png");
//             break;
//         case 'W':
//             checker->image = IMG_LoadTexture(game->renderer, "images/white_super_checker.png");
//     }
//     if (!checker->image) {
//         fprintf(stderr, "Error creating Texture: %s\n", IMG_GetError());
//         return true;
//     }
//     if (SDL_QueryTexture(checker->image, NULL, NULL, &checker->rect.w, &checker->rect.h)){
//         fprintf(stderr, "Error quering Texture: %s\n", SDL_GetError());
//         return true;
//     }

//     return false;
// }

// void checker_cleanup(struct Checker *checker)
// {
//     SDL_DestroyTexture(checker->image);
// }

// bool add_chessboard(struct Board *board, struct Game *game)
// {
//     board->image = IMG_LoadTexture(game->renderer, "images/chessboard.png");
//     if (!board->image) {
//         fprintf(stderr, "Error creating Texture: %s\n", IMG_GetError());
//         return true;
//     }
//     if (SDL_QueryTexture(board->image, NULL, NULL, &board->rect.w, &board->rect.h)){
//         fprintf(stderr, "Error quering Texture: %s\n", SDL_GetError());
//         return true;
//     }
//     return false;
// }

// void board_cleanup(struct Board *board)
// {
//     SDL_DestroyTexture(board->image);
// }



