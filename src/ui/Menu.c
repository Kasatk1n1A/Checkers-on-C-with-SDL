#include "Menu.h"

#define IMAGE_FLAGS IMG_INIT_PNG
#define SCREEN_WIDTH 1400
#define SCREEN_HEIGHT 923
#define FONT_SIZE 72  // Увеличенный размер шрифта
#define FPS 60


void CreateTextButton(MenuItem* item, TTF_Font* font, const char* text, int x, int y, int w, int h)
{
    int textWidth, textHeight;
    TTF_SizeText(font, text, &textWidth, &textHeight);
    item->rect.x = x + (w - textWidth) / 2;
    item->rect.y = y + (h - textHeight) / 2;

    item->rect.w = w;
    item->rect.h = h;
    item->text = text;
    item->hovered = false;
}

void LoseMenu(Window* window)
{
    SDL_Renderer* renderer = window->renderer;

    SDL_Color Black = {0, 0, 0, 255};
    SDL_Color Green = {0, 255, 0, 255};
    SDL_Color red = {255, 0, 0, 255};

    // Загрузка шрифта с увеличенным размером
    TTF_Font* font_small = TTF_OpenFont("assets/fonts/bleedingcowboysrus.ttf", FONT_SIZE);
    TTF_Font* font_big = TTF_OpenFont("assets/fonts/bleedingcowboysrus.ttf", 120);
    if (!font_small || !font_big) 
    {
        fprintf(stderr, "Failed to load font: %s", TTF_GetError());
        return;
    }

    // Пункты меню с увеличенными размерами и отступами
    MenuItem items[1];

    CreateTextButton(&items[1], font_small, "Main menu", SCREEN_WIDTH/2 - 200, 500, 400, 80);

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

                    items[1].hovered = (x >= items[1].rect.x && x <= items[1].rect.x + items[1].rect.w && 
                                        y >= items[1].rect.y && y <= items[1].rect.y + items[1].rect.h);
                    break;
                    
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT)
                            if (items[0].hovered) {
                                selectedItem = 0;
                                running = false;
                                break;
                            }
                    break;
                
                default:
                    break;
                }
            }

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, window->background, NULL, NULL);

            SDL_Color color = items[0].hovered ? red : Green;
            renderText(renderer, font_small, items[0].text, items[0].rect.x, items[0].rect.y, Green, &Black);
            renderText(renderer, font_big, "YOU LOOOOSEEEER!!!", SCREEN_WIDTH/2 - 200, 200, red, &Black);

            SDL_RenderPresent(renderer);

            Uint32 frameTime = SDL_GetTicks() - frameStart;
            if (frameTime < 1000/FPS)
            SDL_Delay(1000/FPS - frameTime);
        }

        switch (selectedItem)
        {
            case 0:
                showMainMenu(window);
                break;
            default: 
                break;
        }
    }
}

void WinMenu(Window* window, GameInfo info)
{
    SDL_Renderer* renderer = window->renderer;

    SDL_Color Black = {0, 0, 0, 255};
    SDL_Color Green = {0, 255, 0, 255};
    SDL_Color red = {255, 0, 0, 255};
    SDL_Color Grey = {178, 178, 178, 255};

    // Загрузка шрифта с увеличенным размером
    TTF_Font* font_small = TTF_OpenFont("assets/fonts/bleedingcowboysrus.ttf", FONT_SIZE);
    TTF_Font* font_in_button = TTF_OpenFont("assets/fonts/freesansbold.ttf", 80);
    TTF_Font* font_big = TTF_OpenFont("assets/fonts/bleedingcowboysrus.ttf", 120);
    if (!font_small || !font_big) 
    {
        fprintf(stderr, "Failed to load font: %s", TTF_GetError());
        return;
    }

    // Пункты меню с увеличенными размерами и отступами
    MenuItem items[3];

    SDL_Texture* button = Create_colored_rect(renderer, 400, 80, 255, 255, 255, 255);
    items[0].rect.x = SCREEN_WIDTH/2 - 250; items[0].rect.y = 400;
    items[0].rect.w = 500;  items[0].rect.h = 80;

    SDL_Texture* button_outline = Create_colored_rect(renderer, 410, 90, 0, 0, 0, 255);
    items[1].rect.x = SCREEN_WIDTH/2 - 255; items[1].rect.y = 395;
    items[1].rect.w = 510;  items[1].rect.h = 90;

    CreateTextButton(&items[2], font_small, "Main menu", SCREEN_WIDTH/2 - 200, 600, 400, 80);

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

            SDL_Color color = items[2].hovered ? red : Green;
            renderText(renderer, font_small, items[2].text, items[2].rect.x, items[2].rect.y, color, &Black);
            renderText(renderer, font_big, "YOU WIIIIIIIIN!!!", SCREEN_WIDTH/2 - 400, 200, red, &Black);
            
            
            SDL_RenderCopy(renderer, button_outline, NULL, &items[1].rect);
            SDL_RenderCopy(renderer, button, NULL, &items[0].rect);
            renderText(renderer, font_in_button, "Your name", items[0].rect.x, items[0].rect.y, Grey, NULL);

            SDL_RenderPresent(renderer);

            Uint32 frameTime = SDL_GetTicks() - frameStart;
            if (frameTime < 1000/FPS)
            SDL_Delay(1000/FPS - frameTime);
        }

        switch (selectedItem)
        {
            case 0:
            case 1:
                if (InputText(window, info)) showMainMenu(window);
                break;
            case 2: 
                showMainMenu(window);
            default: 
                break;
        }
    }
}

int InputText(Window* window, GameInfo info)
{
    SDL_Renderer* renderer = window->renderer;

    SDL_Color Black = {0, 0, 0, 255};
    SDL_Color Green = {0, 255, 0, 255};
    SDL_Color red = {255, 0, 0, 255};
    SDL_Color White = {255, 255, 255, 255};

    // Загрузка шрифта с увеличенным размером
    TTF_Font* font1 = TTF_OpenFont("assets/fonts/bleedingcowboysrus.ttf", FONT_SIZE);
    TTF_Font* font_big = TTF_OpenFont("assets/fonts/bleedingcowboysrus.ttf", 120);
    TTF_Font* font2 = TTF_OpenFont("assets/fonts/freesansbold.ttf", 80);
    if (!font1 || !font2) 
    {
        fprintf(stderr, "Failed to load font: %s", TTF_GetError());
        return 0;
    }

    // Пункты меню с увеличенными размерами и отступами
    MenuItem items[3];

    SDL_Texture* button = Create_colored_rect(renderer, 400, 80, 255, 255, 255, 255);
    items[0].rect.x = SCREEN_WIDTH/2 - 250; items[0].rect.y = 400;
    items[0].rect.w = 500;  items[0].rect.h = 80;

    SDL_Texture* button_outline = Create_colored_rect(renderer, 410, 90, 0, 255, 0, 255);
    items[1].rect.x = SCREEN_WIDTH/2 - 255; items[1].rect.y = 395;
    items[1].rect.w = 510;  items[1].rect.h = 90;

    CreateTextButton(&items[2], font1, "Main menu", SCREEN_WIDTH/2 - 200, 600, 400, 80);

    while (true)
    {
        bool running = true;
        int selectedItem = -1;
        char input_text[10] = {'\0'};

        while (running) 
        {
            Uint32 frameStart = SDL_GetTicks();

            SDL_StartTextInput();

            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                switch (event.type) 
                {
                case SDL_QUIT:
                    SDL_StopTextInput();
                    Game_cleanup(window, EXIT_SUCCESS);
                    break;

                case SDL_MOUSEMOTION:
                    int x = event.motion.x;
                    int y = event.motion.y;

                    items[2].hovered = (x >= items[2].rect.x && x <= items[2].rect.x + items[2].rect.w && 
                                        y >= items[2].rect.y && y <= items[2].rect.y + items[2].rect.h);
                    break;
                    
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT)
                        if (items[2].hovered)
                        {
                            selectedItem = 2;
                            running = false;
                            break;
                        }
                    break;

                case SDL_TEXTINPUT:
                    if (strlen(input_text) < 6)
                    {
                        strncat(input_text, event.text.text, sizeof(input_text) - strlen(input_text) - 1);
                        printf("%s\n", input_text);
                    }
                    break;

                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_RETURN:
                        SaveForLeaderBoard(input_text, info);
                        SDL_StopTextInput();    //Нужно добавить сюда запись в список рекордов
                        return 1;

                    case SDLK_BACKSPACE:
                        if (strlen(input_text) > 0)
                            input_text[strlen(input_text) - 1] = '\0';
                        break;
                    default:
                        break;
                    }
                    
                default:
                    break;
                }
            }

            SDL_StopTextInput();

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, window->background, NULL, NULL);
            
            SDL_Color color = items[2].hovered ? red : Green;
            
            renderText(renderer, font1, items[2].text, items[2].rect.x, items[2].rect.y, color, &Black);
            renderText(renderer, font_big, "YOU WIIIIIIIIN!!!", SCREEN_WIDTH/2 - 400, 200, red, &Black);

            SDL_RenderCopy(renderer, button_outline, NULL, &items[1].rect);
            SDL_RenderCopy(renderer, button, NULL, &items[0].rect);

            if (strlen(input_text) > 0)
                renderText(renderer, font2, input_text, items[0].rect.x, items[0].rect.y, Black, NULL);

            SDL_RenderPresent(renderer);

            Uint32 frameTime = SDL_GetTicks() - frameStart;
            if (frameTime < 1000/FPS)
            SDL_Delay(1000/FPS - frameTime);
        }

        switch (selectedItem)
        {
            case 2: 
                SDL_StopTextInput();
                return 1;
            default: 
                break;
        }
    }
}

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
    MenuItem items[5];
    CreateTextButton(&items[0], font, "New game",   SCREEN_WIDTH/2 - 250, 200, 400, 80);
    CreateTextButton(&items[1], font, "Load game",  SCREEN_WIDTH/2 - 200, 300, 400, 80);
    CreateTextButton(&items[2], font, "Leaderboard",SCREEN_WIDTH/2 - 200, 400, 400, 80);
    CreateTextButton(&items[3], font, "About",      SCREEN_WIDTH/2 - 200, 500, 400, 80);
    CreateTextButton(&items[4], font, "Quit",       SCREEN_WIDTH/2 - 200, 600, 400, 80);

    SDL_Color Black = {0, 0, 0, 255};
    SDL_Color Green = {0, 255, 0, 255};
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
                SDL_Color color = items[i].hovered ? red : Green;

                renderText(renderer, font, items[i].text, items[i].rect.x, items[i].rect.y, color, &Black);
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
            case 2: ShowLeaderBoard(window); break;
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
    MenuItem items[4];
    CreateTextButton(&items[0], font, "Baby",           SCREEN_WIDTH/2 - 230, 200, 400, 80);
    CreateTextButton(&items[1], font, "Grandfather",    SCREEN_WIDTH/2 - 200, 300, 400, 80);
    CreateTextButton(&items[2], font, "GOD",            SCREEN_WIDTH/2 - 200, 410, 400, 80);
    CreateTextButton(&items[3], font, "Back",           SCREEN_WIDTH/2 - 210, 500, 400, 80);

    SDL_Color Black = {0, 0, 0, 255};
    SDL_Color Green = {0, 255, 0, 255};
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
            SDL_Color color = items[i].hovered ? red : Green;

            renderText(renderer, font, items[i].text, items[i].rect.x, items[i].rect.y, color, &Black);
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

void ShowLeaderBoard(Window* window)
{
    SDL_Renderer* renderer = window->renderer;

    SDL_Color Black = {0, 0, 0, 255};
    SDL_Color Green = {0, 255, 0, 255};
    SDL_Color red = {255, 0, 0, 255};

    // Загрузка шрифта с увеличенным размером
    TTF_Font* font_small = TTF_OpenFont("assets/fonts/bleedingcowboysrus.ttf", FONT_SIZE);
    TTF_Font* font_for_leaders = TTF_OpenFont("assets/fonts/freesansbold.ttf", 30);
    if (!font_small || !font_for_leaders) 
    {
        fprintf(stderr, "Failed to load font: %s", TTF_GetError());
        return;
    }

    // Пункты меню с увеличенными размерами и отступами
    MenuItem items[7];

    items[1].rect.x = 100; items[1].rect.y = 100; items[1].rect.h = 630; items[1].rect.w = 250;
    items[2].rect.x = 575; items[2].rect.y = 100; items[2].rect.h = 630; items[2].rect.w = 250;
    items[3].rect.x = 1050; items[3].rect.y = 100; items[3].rect.h = 630; items[3].rect.w = 250;

    items[4].rect.x = 95; items[4].rect.y = 95; items[4].rect.h = 640; items[4].rect.w = 260;
    items[5].rect.x = 570; items[5].rect.y = 95; items[5].rect.h = 640; items[5].rect.w = 260;
    items[6].rect.x = 1045; items[6].rect.y = 95; items[6].rect.h = 640; items[6].rect.w = 260;

    leader* easyLeaders = read_leaders(renderer, 1, items[1].rect.x, items[1].rect.y);
    leader* mediumLeaders = read_leaders(renderer, 2, items[2].rect.x, items[2].rect.y);
    leader* hardLeaders = read_leaders(renderer, 3, items[3].rect.x, items[3].rect.y);

    CreateTextButton(&items[0], font_small, "Main menu", SCREEN_WIDTH/2 - 200, 800, 400, 80);

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

                items[0].hovered = (x >= items[0].rect.x && x <= items[0].rect.x + items[0].rect.w && 
                                    y >= items[0].rect.y && y <= items[0].rect.y + items[0].rect.h);

                break;
                
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                    if (items[0].hovered) {
                        selectedItem = 0;
                        running = false;
                        break;
                    }
                break;
            }
        }
        
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, window->background, NULL, NULL);
        
        SDL_Color color = items[0].hovered ? red : Green;
        renderText(renderer, font_small, items[0].text, items[0].rect.x, items[0].rect.y, color, &Black);
        renderText(renderer, font_small, "EASY", items[1].rect.x, items[1].rect.y - 80, Green, &Black);
        renderText(renderer, font_small, "MIDL", items[2].rect.x, items[2].rect.y - 80, Green, &Black);
        renderText(renderer, font_small, "HARD", items[3].rect.x, items[3].rect.y - 130, Green, &Black);
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &items[4].rect);
        SDL_RenderFillRect(renderer, &items[5].rect);
        SDL_RenderFillRect(renderer, &items[6].rect);
        
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &items[1].rect);
        SDL_RenderFillRect(renderer, &items[2].rect);
        SDL_RenderFillRect(renderer, &items[3].rect);

        PrintLeaders(renderer, easyLeaders);
        PrintLeaders(renderer, mediumLeaders);
        PrintLeaders(renderer, hardLeaders);
        
        SDL_RenderPresent(renderer);

        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < 1000/FPS)
        SDL_Delay(1000/FPS - frameTime);
    }

    free_leaders(easyLeaders);
    free_leaders(mediumLeaders);
    free_leaders(hardLeaders);

    switch (selectedItem)
    {
        case 0:
            return;
        default: 
            break;
    }
}

void free_leaders(leader* leaders)
{
    for (int i = 0; i < 20; i++)
    {
        SDL_DestroyTexture(leaders[i].Name);
        SDL_DestroyTexture(leaders[i].minutes);
        SDL_DestroyTexture(leaders[i].seconds);
    }
    free(leaders);
}

leader* read_leaders(SDL_Renderer* renderer, int type, int x, int y)
{
    FILE* file;
    if (type == 1) file = fopen("saves/leaderboard/easy board.txt", "r");
    else if (type == 2) file = fopen("saves/leaderboard/medium board.txt", "r");
    else file = fopen("saves/leaderboard/hard board.txt", "r");

    TTF_Font* font = TTF_OpenFont("assets/fonts/freesansbold.ttf", 30);
    SDL_Color Black = {0, 0, 0, 255};

    char Name[10] = {'\0'}, minutes[10] = {'\0'}, seconds[10] = {'\0'};

    leader* leaders = (leader*)malloc(sizeof(leader) * 21);

    // Добавляем надписи Name Min Sec
    {
    SDL_Surface* Name_surface = TTF_RenderText_Blended(font, "Name", Black);
    SDL_Surface* minutes_surface = TTF_RenderText_Blended(font, "Min", Black);
    SDL_Surface* seconds_surface = TTF_RenderText_Blended(font, "Sec", Black);

    leaders[0].Name = SDL_CreateTextureFromSurface(renderer, Name_surface);
    leaders[0].minutes = SDL_CreateTextureFromSurface(renderer, minutes_surface);
    leaders[0].seconds = SDL_CreateTextureFromSurface(renderer, seconds_surface);

    leaders[0].Name_rect.x = x;             leaders[0].Name_rect.y = y;     leaders[0].Name_rect.w = Name_surface->w;        leaders[0].Name_rect.h = Name_surface->h;
    leaders[0].minutes_rect.x = x + 130;    leaders[0].minutes_rect.y = y;  leaders[0].minutes_rect.w = minutes_surface->w;  leaders[0].minutes_rect.h = minutes_surface->h;
    leaders[0].seconds_rect.x = x + 200;    leaders[0].seconds_rect.y = y;  leaders[0].seconds_rect.w = seconds_surface->w;  leaders[0].seconds_rect.h = seconds_surface->h;

    SDL_FreeSurface(Name_surface);
    SDL_FreeSurface(minutes_surface);
    SDL_FreeSurface(seconds_surface);
    }

    double Time;
    int i = 1;
    for (; i < 21 && !feof(file); i++)
    {
        fscanf(file, "Name: %s Time: %lf\n", Name, &Time);
        snprintf(minutes, sizeof(char) * 10, "%d", (int)Time / 60);
        snprintf(seconds, sizeof(char) * 10, "%d", (int)Time % 60);
        printf("%s\t %s\t %s\n", Name, minutes, seconds);

        SDL_Surface* Name_surface = TTF_RenderText_Blended(font, Name, Black);
        SDL_Surface* minutes_surface = TTF_RenderText_Blended(font, minutes, Black);
        SDL_Surface* seconds_surface = TTF_RenderText_Blended(font, seconds, Black);

        leaders[i].Name = SDL_CreateTextureFromSurface(renderer, Name_surface);
        leaders[i].minutes = SDL_CreateTextureFromSurface(renderer, minutes_surface);
        leaders[i].seconds = SDL_CreateTextureFromSurface(renderer, seconds_surface);

        leaders[i].Name_rect.x = x;             leaders[i].Name_rect.y = y + i * 30;     leaders[i].Name_rect.w = Name_surface->w;        leaders[i].Name_rect.h = Name_surface->h;
        leaders[i].minutes_rect.x = x + 130;    leaders[i].minutes_rect.y = y + i * 30;  leaders[i].minutes_rect.w = minutes_surface->w;  leaders[i].minutes_rect.h = minutes_surface->h;
        leaders[i].seconds_rect.x = x + 200;    leaders[i].seconds_rect.y = y + i * 30;  leaders[i].seconds_rect.w = seconds_surface->w;  leaders[i].seconds_rect.h = seconds_surface->h;

        SDL_FreeSurface(Name_surface);
        SDL_FreeSurface(minutes_surface);
        SDL_FreeSurface(seconds_surface);
    }

    for (; i < 21; i++)
    {
        leaders[i].Name = NULL;
        leaders[i].minutes = NULL;
        leaders[i].seconds = NULL;
    }

    TTF_CloseFont(font);
    fclose(file);

    return leaders;
}

void PrintLeaders(SDL_Renderer* renderer, leader* leaders)
{
    for (int i = 0; i < 21 && leaders[i].Name; i++)
    {
        SDL_RenderCopy(renderer, leaders[i].Name, NULL, &leaders[i].Name_rect);
        SDL_RenderCopy(renderer, leaders[i].minutes, NULL, &leaders[i].minutes_rect);
        SDL_RenderCopy(renderer, leaders[i].seconds, NULL, &leaders[i].seconds_rect);
        printf("NIGGERS!!!\n");
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
    MenuItem items[3];
    CreateTextButton(&items[0], font, "Continue",   SCREEN_WIDTH/2 - 220, 200, 400, 80);
    CreateTextButton(&items[1], font, "Save",       SCREEN_WIDTH/2 - 220, 310, 400, 80);
    CreateTextButton(&items[2], font, "Main Menu",  SCREEN_WIDTH/2 - 200, 400, 400, 80);

    SDL_Color Black = {0, 0, 0, 255};
    SDL_Color Green = {0, 255, 0, 255};
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
                SDL_Color color = items[i].hovered ? red : Green;

                renderText(renderer, font, items[i].text, items[i].rect.x, items[i].rect.y, color, &Black);
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

void renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y, SDL_Color text_color, SDL_Color* outline_color) 
{
    if (outline_color != NULL)
    {
        TTF_SetFontOutline(font, 2);
        SDL_Surface* outline_surface = TTF_RenderText_Blended(font, text, *outline_color);
        SDL_Texture* outline_texture = SDL_CreateTextureFromSurface(renderer, outline_surface);
        SDL_Rect outline_rect = {x, y, outline_surface->w, outline_surface->h};
        SDL_RenderCopy(renderer, outline_texture, NULL, &outline_rect);

        SDL_FreeSurface(outline_surface);
        SDL_DestroyTexture(outline_texture);
    }
    
    TTF_SetFontOutline(font, 0);
    SDL_Surface* text_surface = TTF_RenderText_Blended(font, text, text_color);
    SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    SDL_Rect text_rect = {x, y, text_surface->w, text_surface->h};
    SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
    
    SDL_FreeSurface(text_surface);
    SDL_DestroyTexture(text_texture);
}