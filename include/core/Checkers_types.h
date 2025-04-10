#ifndef CHECKERS_TYPES_H
#define CHECKERS_TYPES_H

typedef enum {
    WHITE,
    RED
} Player;

typedef struct 
{
    int difficult;
    double Time;
    Player player;
} GameInfo;

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

#endif