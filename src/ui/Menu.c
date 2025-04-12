#include "Menu.h"

#define IMAGE_FLAGS IMG_INIT_PNG
#define SCREEN_WIDTH 1400
#define SCREEN_HEIGHT 923
#define FONT_SIZE 72  // Увеличенный размер шрифта
#define FPS 60

void showMainMenu(Window* window)
{
    SDL_Renderer* renderer = window->renderer;

    // Загрузка шрифта с увеличенным размером
    TTF_Font* font = TTF_OpenFont("assets/fonts/bleedingcowboysrus.ttf", FONT_SIZE);
    if (!font) 
    {
        fprintf(stderr, "Failed to load font: %s", TTF_GetError());
        return;
    }

    // Пункты меню с увеличенными размерами и отступами
    MenuItem items[5] = 
    {
        {{SCREEN_WIDTH/2 - 200 - 50, 200, 400, 80}, "New game", false},
        {{SCREEN_WIDTH/2 - 200, 300, 400, 80}, "Load game", false},
        {{SCREEN_WIDTH/2 - 200, 400, 400, 80}, "Leaderboard", false},
        {{SCREEN_WIDTH/2 - 200, 500, 400, 80}, "About", false},
        {{SCREEN_WIDTH/2 - 200, 600, 400, 80}, "Quit", false}
    };

    SDL_Color black = {0, 255, 0, 255};
    SDL_Color red = {255, 0, 0, 255};

    while (true)
    {
        bool running = true;
        int selectedItem = -1;

        while (running) 
        {
            Uint32 frameStart = SDL_GetTicks();

            SDL_Event event;
            while (SDL_PollEvent(&event)) 
            {
                switch (event.type) 
                {
                case SDL_QUIT:
                    Game_cleanup(window, EXIT_SUCCESS);
                    break;

                case SDL_MOUSEMOTION:
                    int x = event.motion.x;
                    int y = event.motion.y;

                    for (int i = 0; i < 5; i++) 
                    {
                        items[i].hovered = (x >= items[i].rect.x && x <= items[i].rect.x + items[i].rect.w && 
                                            y >= items[i].rect.y && y <= items[i].rect.y + items[i].rect.h);
                    }
                    break;
                    
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        for (int i = 0; i < 5; i++) {
                            if (items[i].hovered) {
                                selectedItem = i;
                                running = false;
                                break;
                            }
                        }
                    }
                    break;
                }
            }

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, window->background, NULL, NULL);

            for (int i = 0; i < 5; i++) 
            {
                SDL_Color color = items[i].hovered ? red : black;
                // Центрирование текста внутри увеличенных прямоугольников
                int textWidth, textHeight;
                TTF_SizeText(font, items[i].text, &textWidth, &textHeight);
                int textX = items[i].rect.x + (items[i].rect.w - textWidth) / 2;
                int textY = items[i].rect.y + (items[i].rect.h - textHeight) / 2;

                renderText(renderer, font, items[i].text, textX, textY, color);
            }

            SDL_RenderPresent(renderer);

            Uint32 frameTime = SDL_GetTicks() - frameStart;
            if (frameTime < 1000/FPS)
            SDL_Delay(1000/FPS - frameTime);
        }

        switch (selectedItem)
        {
            case 0: 
                SetDifficult(window);
                printf("returned\n");
                break;
            case 1: 
                double Time;
                Player player;
                int difficult;
                CH_Type** board = load_from_save(&Time, &player, &difficult);
                if (board == NULL)
                    break;

                checkers(window, board, difficult, Time, player);
                break;
            case 2: printf("Leaderboard selected\n"); break;
            case 3: printf("About selected\n"); break;
            case 4: 
                printf("Quit selected\n"); 
                Game_cleanup(window, EXIT_SUCCESS);
                break;
            default: 
                break;
        }
    }
    
}

void SetDifficult(Window* window)
{
    SDL_Renderer* renderer = window->renderer;

    // Загрузка шрифта с увеличенным размером
    TTF_Font* font = TTF_OpenFont("assets/fonts/bleedingcowboysrus.ttf", FONT_SIZE);
    if (!font) 
    {
        fprintf(stderr, "Failed to load font: %s", TTF_GetError());
        return;
    }

    // Пункты меню с увеличенными размерами и отступами
    MenuItem items[4] = 
    {
        {{SCREEN_WIDTH/2 - 230, 200, 400, 80}, "Baby", false},
        {{SCREEN_WIDTH/2 - 200, 300, 400, 80}, "Your grandfather", false},
        {{SCREEN_WIDTH/2 - 200, 410, 400, 80}, "GOD", false},
        {{SCREEN_WIDTH/2 - 210, 500, 400, 80}, "Back", false},
    };

    SDL_Color black = {0, 255, 0, 255};
    SDL_Color red = {255, 0, 0, 255};

    bool running = true;
    int selectedItem = -1;

    while (running) 
    {
        Uint32 frameStart = SDL_GetTicks();

        SDL_Event event;
        while (SDL_PollEvent(&event)) 
        {
            switch (event.type) 
            {
            case SDL_QUIT:
                Game_cleanup(window, EXIT_SUCCESS);
                break;
            case SDL_MOUSEMOTION:
                int x = event.motion.x;
                int y = event.motion.y;

                for (int i = 0; i < 4; i++) 
                {
                    items[i].hovered = (x >= items[i].rect.x && x <= items[i].rect.x + items[i].rect.w && 
                                        y >= items[i].rect.y && y <= items[i].rect.y + items[i].rect.h);
                }
                break;
                
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    for (int i = 0; i < 5; i++) {
                        if (items[i].hovered) {
                            selectedItem = i;
                            running = false;
                            break;
                        }
                    }
                }
                break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, window->background, NULL, NULL);

        for (int i = 0; i < 4; i++) 
        {
            SDL_Color color = items[i].hovered ? red : black;
            // Центрирование текста внутри увеличенных прямоугольников
            int textWidth, textHeight;
            TTF_SizeText(font, items[i].text, &textWidth, &textHeight);
            int textX = items[i].rect.x + (items[i].rect.w - textWidth) / 2;
            int textY = items[i].rect.y + (items[i].rect.h - textHeight) / 2;

            renderText(renderer, font, items[i].text, textX, textY, color);
        }

        SDL_RenderPresent(renderer);

        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < 1000/FPS)
        SDL_Delay(1000/FPS - frameTime);
    }

    TTF_CloseFont(font);
    switch (selectedItem)
    {
        case 0: 
            printf("Baby difficult\n"); 
            checkers(window, NULL, 1, 0, 0);
            break;
        case 1: 
            printf("Middle difficult\n"); 
            checkers(window, NULL, 3, 0, 0);
            break;
        case 2: 
            printf("GOD difficult\n"); 
            checkers(window, NULL, 7, 0, 0);
            break;
        case 3: 
            printf("Back selected\n"); 
            return;
            break;
        default: 
            break;
    }
}

void ShowMiniMenu(Window* window, Board* CheckersBoard, GameInfo info)
{
    SDL_Renderer* renderer = window->renderer;

    // Загрузка шрифта с увеличенным размером
    TTF_Font* font = TTF_OpenFont("assets/fonts/bleedingcowboysrus.ttf", FONT_SIZE);
    if (!font) 
    {
        fprintf(stderr, "Failed to load font: %s", TTF_GetError());
        return;
    }

    // Пункты меню с увеличенными размерами и отступами
    MenuItem items[3] = 
    {
        {{SCREEN_WIDTH/2 - 220, 200, 400, 80}, "Continue", false},
        {{SCREEN_WIDTH/2 - 220, 310, 400, 80}, "Save", false},
        {{SCREEN_WIDTH/2 - 200, 400, 400, 80}, "Main Menu", false}
    };

    SDL_Color black = {0, 255, 0, 255};
    SDL_Color red = {255, 0, 0, 255};

    while (true)
    {

        bool running = true;
        int selectedItem = -1;

        while (running) 
        {
            Uint32 frameStart = SDL_GetTicks();

            SDL_Event event;
            while (SDL_PollEvent(&event)) 
            {
                switch (event.type) 
                {
                case SDL_QUIT:
                    Game_cleanup(window, EXIT_SUCCESS);
                    break;
                case SDL_MOUSEMOTION:
                    int x = event.motion.x;
                    int y = event.motion.y;

                    for (int i = 0; i < 3; i++) 
                    {
                        items[i].hovered = (x >= items[i].rect.x && x <= items[i].rect.x + items[i].rect.w && 
                                            y >= items[i].rect.y && y <= items[i].rect.y + items[i].rect.h);
                    }
                    break;
                    
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        for (int i = 0; i < 3; i++) {
                            if (items[i].hovered) {
                                selectedItem = i;
                                running = false;
                                break;
                            }
                        }
                    }
                    break;
                }
            }

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, window->background, NULL, NULL);

            for (int i = 0; i < 3; i++) 
            {
                SDL_Color color = items[i].hovered ? red : black;
                // Центрирование текста внутри увеличенных прямоугольников
                int textWidth, textHeight;
                TTF_SizeText(font, items[i].text, &textWidth, &textHeight);
                int textX = items[i].rect.x + (items[i].rect.w - textWidth) / 2;
                int textY = items[i].rect.y + (items[i].rect.h - textHeight) / 2;

                renderText(renderer, font, items[i].text, textX, textY, color);
            }

            SDL_RenderPresent(renderer);

            Uint32 frameTime = SDL_GetTicks() - frameStart;
            if (frameTime < 1000/FPS)
            SDL_Delay(1000/FPS - frameTime);
        }

        switch (selectedItem)
        {
            case 0: 
                renderBoardFrame(window, CheckersBoard);
                return;
                break;
            case 1: 
                SaveGame(CheckersBoard->board, info);
                break;
            case 2: 
                board_cleanup_SDL(CheckersBoard);
                showMainMenu(window);
                break;
            default: 
                break;
        }
    }
}

void renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y, SDL_Color color) 
{
    SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}