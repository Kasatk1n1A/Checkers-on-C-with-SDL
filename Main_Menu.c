#include "libraries.h"







int main()
{
    if (sdl_initialize(game)) 
        app_cleanup(game, EXIT_FAILURE);
}