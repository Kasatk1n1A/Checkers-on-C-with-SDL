#ifndef STRUCTSANDENUM
#define STRUCTSANDENUM

typedef enum {
    WHITE,
    RED
} Player;

typedef enum 
{
    EMPTY = 0,
    WHITE_PAWN = 1,
    PICKED_WHITE_PAWN = 2,
    WHITE_KING = 3,
    PICKED_WHITE_KING = 4,
    RED_PAWN = -1,
    PICKED_RED_PAWN = -2,
    RED_KING = -3,
    PICKED_RED_KING = -4
} CH_Type;

typedef enum
{
    ENTER,
    MOUSE_LEFT
} Choice;

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