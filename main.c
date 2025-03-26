#include "libraries.h"

// void board_cleanup(int** board);
bool add_board_SDL(Game* game);
void out_board_SDL(Game* game);

Game* game;

int main()
{
    game = (Game*)malloc(sizeof(Game));
    game->window = NULL;
    game->renderer = NULL;
    game->background = NULL;
    game->board = NULL;   
    game->text_count = 0; 
    
    if (sdl_initialize(game)) 
        game_cleanup(game, EXIT_FAILURE);
    
    if (add_board_SDL(game))
        game_cleanup(game, EXIT_FAILURE);
    
    SDL_Thread* test_thread = SDL_CreateThread(checkers, "checkers", NULL);

    // if (load_media(game)){
    //     game_cleanup(game, EXIT_FAILURE);
    // }

    while (true)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event)) 
        {
            switch (event.type) 
            {
            case SDL_QUIT:
                game_cleanup(game, EXIT_SUCCESS);
                break;  
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode)
                {
                    //Close game if press esc
                case SDL_SCANCODE_ESCAPE:
                    game_cleanup(game, EXIT_SUCCESS);
                    break;
                    // case SDL_MOUSEBUTTONDOWN:
                    //     // Проверяем, что нажата левая кнопка мыши
                    //     if (event.button.button == SDL_BUTTON_LEFT) {
                    //         // Получаем координаты мыши
                    //         SDL_LockMutex(game->mutex);
                    //         game->Mouse.x = event.button.x;
                    //         game->Mouse.y = event.button.y;
                    //         SDL_CondSignal(game->cond);
                    //         SDL_UnlockMutex(game->mutex);
                    //     }
                    //     break;
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
        SDL_LockMutex(game->mutex);
        
        SDL_RenderClear(game->renderer);
            
        SDL_RenderCopy(game->renderer, game->background, NULL, NULL);
    
        out_board_SDL(game);

        // text_out(game->texts, game->text_count);
           
        SDL_RenderPresent(game->renderer);

        SDL_UnlockMutex(game->mutex);

        SDL_Delay(16);
    }
    
    SDL_WaitThread(test_thread, NULL);

    game_cleanup(game, EXIT_SUCCESS);

    return 0;
}

//Создаёт изначальную доску в качестве массива из динамических массивов
bool add_board_SDL(Game* game)
{
    game->board = (Board*)malloc(sizeof(Board)); 
    game->board->image = IMG_LoadTexture(game->renderer, "images/chessboard.png");     
    if (!game->board->image){
        fprintf(stderr, "Error creating Texture: %s\n", IMG_GetError());
        return true;
    }
    game->board->rect.x = 239; 
    game->board->rect.y = 0; 
    game->board->rect.h = 0; 
    game->board->rect.w = 0; 
    if (SDL_QueryTexture(game->board->image, NULL, NULL, &game->board->rect.w, &game->board->rect.h)){
        fprintf(stderr, "Error quering Texture: %s\n", SDL_GetError());
        return true;
    }


    game->board->checkers = (Checker***)malloc(sizeof(Checker**) * 8);
    //{ 0, r, 0, r, 0, r, 0, r },
    //{ r, 0, r, 0, r, 0, r, 0 },
    //{ 0, r, 0, r, 0, r, 0, r },
    //{ 0, 0, 0, 0, 0, 0, 0, 0 },
    //{ 0, w, 0, 0, 0, 0, 0, 0 },
    //{ w, 0, w, 0, w, 0, w, 0 },
    //{ 0, w, 0, w, 0, w, 0, w },
    //{ w, 0, w, 0, w, 0, w, 0 },

    for (int i = 0; i < 8; i++){
        game->board->checkers[i] = (Checker**)malloc(sizeof(Checker*) * 8);
        memset(game->board->checkers[i], 0, sizeof(Checker*) * 8);
    }

    for (int j = 0; j < 3; j++) 
    {
        for (int i = 1 - j % 2; i < 8; i += 2) 
        {
            game->board->checkers[j][i] = (Checker*)malloc(sizeof(Checker));
            game->board->checkers[j][i]->default_image = IMG_LoadTexture(game->renderer, "images/red_checker.png");
            game->board->checkers[j][i]->picked_image = IMG_LoadTexture(game->renderer, "images/picked_red_checker.png");
            game->board->checkers[j][i]->King_image = IMG_LoadTexture(game->renderer, "images/red_super_checker.png");
            game->board->checkers[j][i]->King_picked_image = IMG_LoadTexture(game->renderer, "images/picked_red_superchecker.png");
            game->board->checkers[j][i]->flag = RED_PAWN;
            game->board->checkers[j][i]->rect.x = 282 + 105 * i;
            game->board->checkers[j][i]->rect.y = 43 + 105 * j;
            game->board->checkers[j][i]->rect.w = 0; 
            game->board->checkers[j][i]->rect.h = 0;
            if (SDL_QueryTexture(game->board->checkers[j][i]->default_image, NULL, NULL, &game->board->checkers[j][i]->rect.w, &game->board->checkers[j][i]->rect.h)){
                fprintf(stderr, "Error quering Texture: %s\n", SDL_GetError());
                return true;
            }
            // printf("%d %d checker created\n", j, i);
        }
    }

    for (int j = 5; j < 8; j++) 
    {
        for (int i = 1 - j % 2; i < 8; i += 2) 
        {
            game->board->checkers[j][i] = (Checker*)malloc(sizeof(Checker));
            game->board->checkers[j][i]->default_image = IMG_LoadTexture(game->renderer, "images/white_checker.png");
            game->board->checkers[j][i]->picked_image = IMG_LoadTexture(game->renderer, "images/picked_white_checker.png");
            game->board->checkers[j][i]->King_image = IMG_LoadTexture(game->renderer, "images/white_super_checker.png");
            game->board->checkers[j][i]->King_picked_image = IMG_LoadTexture(game->renderer, "images/picked_white_superchecker.png");
            game->board->checkers[j][i]->flag = WHITE_PAWN;
            game->board->checkers[j][i]->rect.x = 282 + 105 * i;
            game->board->checkers[j][i]->rect.y = 43 + 105 * j; 
            game->board->checkers[j][i]->rect.w = 0; 
            game->board->checkers[j][i]->rect.h = 0;
            if (SDL_QueryTexture(game->board->checkers[j][i]->default_image, NULL, NULL, &game->board->checkers[j][i]->rect.w, &game->board->checkers[j][i]->rect.h)){
                fprintf(stderr, "Error quering Texture: %s\n", SDL_GetError());
                return true;
            }
            // printf("%d %d checker created\n", j, i);
        }
    }
    return false;
}



void out_board_SDL(Game* game)
{
    SDL_RenderCopy(game->renderer, game->board->image, NULL, &game->board->rect);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (!game->board->checkers[i][j])
                continue;
            // printf("%d %d checker copied\n", i, j);

            switch (game->board->checkers[i][j]->flag)
            {
            case RED_PAWN:
            case WHITE_PAWN:
                SDL_RenderCopy(game->renderer, game->board->checkers[i][j]->default_image, NULL, &game->board->checkers[i][j]->rect);
                break;
            case RED_KING:
            case WHITE_KING:
                SDL_RenderCopy(game->renderer, game->board->checkers[i][j]->King_image, NULL, &game->board->checkers[i][j]->rect);
                break; 
            case PICKED_WHITE_PAWN:
            case PICKED_RED_PAWN:
                SDL_RenderCopy(game->renderer, game->board->checkers[i][j]->picked_image, NULL, &game->board->checkers[i][j]->rect);
                // printf("%d %d checker printed\n", i, j);
                break; 
            case PICKED_WHITE_KING:
            case PICKED_RED_KING:
                SDL_RenderCopy(game->renderer, game->board->checkers[i][j]->King_picked_image, NULL, &game->board->checkers[i][j]->rect);
                // printf("%d %d checker printed\n", i, j);
                break; 
            default:
                break;
            }
            // printf("%d %d checker copyied\n", i, j);
        }
    }
}