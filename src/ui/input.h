#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include "../../include/core/ui_types.h"
#include "../../include/core/Board_types.h"
#include "../utils/sdl_utils.h"
#include "Menu.h"

void playerAction(Window* window);
Choice get_mouse_click(int* x, int* y, Window* window, Board* CheckersBoard, GameInfo info);

#endif