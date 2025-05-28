#include "Menu.h"

#define IMAGE_FLAGS IMG_INIT_PNG
#define SCREEN_WIDTH 1400
#define SCREEN_HEIGHT 923
#define FONT_SIZE 72  // Увеличенный размер шрифта
#define FPS 60

void showMainMenu(Window* window)   //исправлены утечки
{
    SDL_Renderer* renderer = window->renderer;

    // Загрузка шрифта с увеличенным размером
    TTF_Font* font = TTF_OpenFont("assets/fonts/minecraft.ttf", FONT_SIZE);
    if (!font) 
    {
        fprintf(stderr, "Failed to load font: %s", TTF_GetError());
        Game_cleanup(window, EXIT_FAILURE);
    }

    //Создание кнопок меню    
    Button* buttons = (Button*)malloc(sizeof(Button) * 5);
    if (!buttons)
    {
        fprintf(stderr, "Fail with memory!");
        TTF_CloseFont(font);
        Game_cleanup(window, EXIT_FAILURE);
    }
    CreateTextButton(renderer, &buttons[0], font, "New game",   SCREEN_WIDTH/2, 200);
    CreateTextButton(renderer, &buttons[1], font, "Load game",  SCREEN_WIDTH/2, 300);
    CreateTextButton(renderer, &buttons[2], font, "Leaderboard",SCREEN_WIDTH/2, 400);
    CreateTextButton(renderer, &buttons[3], font, "About",      SCREEN_WIDTH/2, 500);
    CreateTextButton(renderer, &buttons[4], font, "Quit",       SCREEN_WIDTH/2, 600);
    
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
                TTF_CloseFont(font);
                FreeButtons(buttons, 5);
                Game_cleanup(window, EXIT_SUCCESS);
                break;

            case SDL_MOUSEMOTION:
                int x = event.motion.x;
                int y = event.motion.y;

                for (int i = 0; i < 5; i++) 
                {
                    buttons[i].hovered = (x >= buttons[i].out_rect.x && x <= buttons[i].out_rect.x + buttons[i].out_rect.w && 
                                          y >= buttons[i].out_rect.y && y <= buttons[i].out_rect.y + buttons[i].out_rect.h);
                }
                break;
                
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    for (int i = 0; i < 5; i++) {
                        if (buttons[i].hovered) {
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
            renderButton(renderer, &buttons[i]);

        SDL_RenderPresent(renderer);

        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < 1000/FPS)
            SDL_Delay(1000/FPS - frameTime);
    }

    TTF_CloseFont(font);
    FreeButtons(buttons, 5);

    switch (selectedItem)
    {
        case 0: SetDifficult(window); break;
        case 1: LoadGame(window); break;
        case 2: ShowLeaderBoard(window); break;
        case 3: ShowAbout(window); break;
        case 4: Game_cleanup(window, EXIT_SUCCESS); break;
        default: Game_cleanup(window, EXIT_FAILURE); break;
    }
}

void SetDifficult(Window* window)   //исправлены утечки
{
    SDL_Renderer* renderer = window->renderer;

    // Загрузка шрифта с увеличенным размером
    TTF_Font* font = TTF_OpenFont("assets/fonts/minecraft.ttf", FONT_SIZE);
    if (!font) 
    {
        fprintf(stderr, "Failed to load font: %s", TTF_GetError());
        Game_cleanup(window, EXIT_FAILURE);
    }

    //Создание кнопок меню
    Button* buttons = (Button*)malloc(sizeof(Button) * 4);
        if (!buttons)
    {
        fprintf(stderr, "Fail with memory!");
        TTF_CloseFont(font);
        Game_cleanup(window, EXIT_FAILURE);
    }
    CreateTextButton(renderer, &buttons[0], font, "Baby",          SCREEN_WIDTH/2, 200);
    CreateTextButton(renderer, &buttons[1], font, "Grandfather",   SCREEN_WIDTH/2, 300);
    CreateTextButton(renderer, &buttons[2], font, "GOD",           SCREEN_WIDTH/2, 400);
    CreateTextButton(renderer, &buttons[3], font, "Back",          SCREEN_WIDTH/2, 500);

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
                TTF_CloseFont(font);
                FreeButtons(buttons, 4);
                Game_cleanup(window, EXIT_SUCCESS);
                break;

            case SDL_MOUSEMOTION:
                int x = event.motion.x;
                int y = event.motion.y;

                for (int i = 0; i < 4; i++) 
                {
                    buttons[i].hovered = (x >= buttons[i].out_rect.x && x <= buttons[i].out_rect.x + buttons[i].out_rect.w && 
                                          y >= buttons[i].out_rect.y && y <= buttons[i].out_rect.y + buttons[i].out_rect.h);
                }
                break;
                
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    for (int i = 0; i < 4; i++) {
                        if (buttons[i].hovered) {
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
            renderButton(renderer, &buttons[i]);

        SDL_RenderPresent(renderer);

        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < 1000/FPS)
        SDL_Delay(1000/FPS - frameTime);
    }

    TTF_CloseFont(font);
    FreeButtons(buttons, 5);

    switch (selectedItem)
    {
        case 0: 
            checkers(window, NULL, 1, 0, 0);
            break;
        case 1: 
            checkers(window, NULL, 3, 0, 0);
            break;
        case 2: 
            checkers(window, NULL, 7, 0, 0);
            break;
        case 3: 
            showMainMenu(window);
            break;
        default: 
            Game_cleanup(window, EXIT_FAILURE);
            break;
    }
}

void ShowMiniMenu(Window* window, Board* CheckersBoard, GameInfo info)  //исправлены утечки
{
    SDL_Renderer* renderer = window->renderer;

    // Загрузка шрифта с увеличенным размером
    TTF_Font* font = TTF_OpenFont("assets/fonts/minecraft.ttf", FONT_SIZE);
    if (!font) 
    {
        fprintf(stderr, "Failed to load font: %s", TTF_GetError());
        Game_cleanup(window, EXIT_FAILURE);
    }

    Button* buttons = (Button*)malloc(sizeof(Button) * 3);
    if (!buttons)
    {
        fprintf(stderr, "Fail with memory!");
        TTF_CloseFont(font);
        Game_cleanup(window, EXIT_FAILURE);
    }
    CreateTextButton(renderer, &buttons[0], font, "Continue",  SCREEN_WIDTH/2, 200);
    CreateTextButton(renderer, &buttons[1], font, "Save",      SCREEN_WIDTH/2, 300);
    CreateTextButton(renderer, &buttons[2], font, "Main Menu", SCREEN_WIDTH/2, 400);

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
                TTF_CloseFont(font);
                FreeButtons(buttons, 3);
                board_cleanup_SDL(CheckersBoard);
                Game_cleanup(window, EXIT_SUCCESS);
                break;
            case SDL_MOUSEMOTION:
                int x = event.motion.x;
                int y = event.motion.y;

                for (int i = 0; i < 3; i++) 
                {
                    buttons[i].hovered = (x >= buttons[i].out_rect.x && x <= buttons[i].out_rect.x + buttons[i].out_rect.w && 
                                          y >= buttons[i].out_rect.y && y <= buttons[i].out_rect.y + buttons[i].out_rect.h);
                }
                break;
                
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    for (int i = 0; i < 3; i++) {
                        if (buttons[i].hovered) {
                            selectedItem = i;
                            running = false;
                            break;
                        }
                    }
                }
                break;
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, window->background, NULL, NULL);

        for (int i = 0; i < 3; i++) 
            renderButton(renderer, &buttons[i]);

        SDL_RenderPresent(renderer);

        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < 1000/FPS)
        SDL_Delay(1000/FPS - frameTime);
    }

    TTF_CloseFont(font);
    FreeButtons(buttons, 3);
    
    switch (selectedItem)
    {
        case 0: 
            CH_Type** board = CheckersBoard->board;
            CheckersBoard->board = NULL;
            board_cleanup_SDL(CheckersBoard);
            checkers(window, board, info.difficult, info.Time, info.player);
            break;
        case 1: 
            SaveGame(window, CheckersBoard, info);
            break;
        case 2: 
            board_cleanup_SDL(CheckersBoard);
            showMainMenu(window);
            break;
        default: 
            Game_cleanup(window, EXIT_FAILURE);
            break;
    }
}

//----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$

void SaveGame(Window* window, Board* CheckersBoard, GameInfo info)  //исправлены утечки
{
    SDL_Renderer* renderer = window->renderer;

    SDL_Color red = {255, 0, 0, 255};
    SDL_Color Black = {0, 0, 0, 255};

    // Загрузка шрифта с увеличенным размером
    TTF_Font* font1 = TTF_OpenFont("assets/fonts/minecraft.ttf", FONT_SIZE);
    TTF_Font* font2 = TTF_OpenFont("assets/fonts/freesansbold.ttf", 80);
    TTF_Font* font3 = TTF_OpenFont("assets/fonts/minecraft.ttf", 100);
    if (!font1 || !font2 || !font3) 
    {
        fprintf(stderr, "Failed to load font: %s", TTF_GetError());
        Game_cleanup(window, EXIT_FAILURE);
    }

    // Пункты меню с увеличенными размерами и отступами
    MenuItem* items = (MenuItem*)malloc(sizeof(MenuItem) * 2);

    SDL_Texture* button = Create_colored_rect(renderer, 500, 80, 255, 255, 255, 255);
    items[0].rect.x = SCREEN_WIDTH/2 - 250; items[0].rect.y = 400;
    items[0].rect.w = 500;  items[0].rect.h = 80;

    SDL_Texture* button_outline = Create_colored_rect(renderer, 410, 90, 0, 0, 0, 255);
    items[1].rect.x = SCREEN_WIDTH/2 - 255; items[1].rect.y = 395;
    items[1].rect.w = 510;  items[1].rect.h = 90;

    Button* buttons = (Button*)malloc(sizeof(Button));
    if (!buttons)
    {
        fprintf(stderr, "Fail with memory!");
        TTF_CloseFont(font1);
        TTF_CloseFont(font2);
        TTF_CloseFont(font3);
        Game_cleanup(window, EXIT_FAILURE);
    }
    CreateTextButton(renderer, buttons, font1, "Menu",  SCREEN_WIDTH/2, 600);

    //Создание надписи о просьбе ввода
    TTF_SetFontOutline(font3, 2);
    SDL_Surface* outline_surface = TTF_RenderText_Blended(font3, "Input save's name", Black);
    TTF_SetFontOutline(font3, 0);
    SDL_Surface* text_surface = TTF_RenderText_Blended(font3, "Input save's name", red);
    
    SDL_Texture* outline_texture = SDL_CreateTextureFromSurface(renderer, outline_surface);
    SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);

    SDL_Rect outline_rect = {SCREEN_WIDTH/2 - outline_surface->w / 2, 200, outline_surface->w, outline_surface->h};
    SDL_Rect text_rect = {SCREEN_WIDTH/2 - text_surface->w / 2, 200, text_surface->w, text_surface->h};

    SDL_FreeSurface(outline_surface);
    SDL_FreeSurface(text_surface);

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
                free(items);
                TTF_CloseFont(font1);
                TTF_CloseFont(font2);
                TTF_CloseFont(font3);
                FreeButtons(buttons, 1);
                SDL_DestroyTexture(text_texture);
                SDL_DestroyTexture(outline_texture);
                board_cleanup_SDL(CheckersBoard);
                Game_cleanup(window, EXIT_SUCCESS);
                break;

            case SDL_MOUSEMOTION:
                int x = event.motion.x;
                int y = event.motion.y;

                for (int i = 0; i < 2; i++) 
                {
                    items[i].hovered = (x >= items[i].rect.x && x <= items[i].rect.x + items[i].rect.w && 
                                        y >= items[i].rect.y && y <= items[i].rect.y + items[i].rect.h);
                }
                buttons->hovered = (x >= buttons->out_rect.x && x <= buttons->out_rect.x + buttons->out_rect.w && 
                                    y >= buttons->out_rect.y && y <= buttons->out_rect.y + buttons->out_rect.h);
                break;
                
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) 
                {
                    for (int i = 0; i < 2; i++) 
                    {
                        if (items[i].hovered) 
                        {
                            selectedItem = i;
                            running = false;
                            break;
                        }
                    }
                    if (buttons->hovered)
                    {
                        selectedItem = 2;
                        running = false;
                        break;
                    }
                }
                break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, window->background, NULL, NULL);

        renderButton(renderer, buttons);
        SDL_RenderCopy(renderer, outline_texture, NULL, &outline_rect);
        SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
        
        SDL_RenderCopy(renderer, button_outline, NULL, &items[1].rect);
        SDL_RenderCopy(renderer, button, NULL, &items[0].rect);
        renderText(renderer, font2, "Your name", items[0].rect.x, items[0].rect.y, Black, NULL);

        SDL_RenderPresent(renderer);

        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < 1000/FPS)
        SDL_Delay(1000/FPS - frameTime);
    }

    free(items);
    TTF_CloseFont(font1);
    TTF_CloseFont(font2);
    TTF_CloseFont(font3);
    FreeButtons(buttons, 1);
    SDL_DestroyTexture(text_texture);
    SDL_DestroyTexture(outline_texture);

    switch (selectedItem)
    {
        case 0: 
        case 1: SaveGame_InputText(window, CheckersBoard, info); break;
        case 2: ShowMiniMenu(window, CheckersBoard, info); break;
        default: 
            board_cleanup_SDL(CheckersBoard);
            Game_cleanup(window, EXIT_FAILURE); break;
    }
}

void SaveGame_InputText(Window* window, Board* CheckersBoard, GameInfo info)    //исправлены утечки
{
    SDL_Renderer* renderer = window->renderer;
    CH_Type** board = CheckersBoard->board;

    SDL_Color Black = {0, 0, 0, 255};
    SDL_Color red = {255, 0, 0, 255};

    // Загрузка шрифта с увеличенным размером
    TTF_Font* font1 = TTF_OpenFont("assets/fonts/minecraft.ttf", FONT_SIZE);
    TTF_Font* font2 = TTF_OpenFont("assets/fonts/freesansbold.ttf", 80);
    TTF_Font* font3 = TTF_OpenFont("assets/fonts/minecraft.ttf", 100);
    if (!font1 || !font2 || !font3) 
    {
        fprintf(stderr, "Failed to load font: %s", TTF_GetError());
        return;
    }

    // Пункты меню с увеличенными размерами и отступами
    MenuItem* items = (MenuItem*)malloc(sizeof(MenuItem) * 2);
    if (!items)
    {
        fprintf(stderr, "Fail with memory!");
        TTF_CloseFont(font1);
        TTF_CloseFont(font2);
        TTF_CloseFont(font3);
        Game_cleanup(window, EXIT_FAILURE);
    }

    SDL_Texture* button = Create_colored_rect(renderer, 400, 80, 255, 255, 255, 255);
    items[0].rect.x = SCREEN_WIDTH/2 - 250; items[0].rect.y = 400;
    items[0].rect.w = 500;  items[0].rect.h = 80;

    SDL_Texture* button_outline = Create_colored_rect(renderer, 410, 90, 0, 255, 0, 255);
    items[1].rect.x = SCREEN_WIDTH/2 - 255; items[1].rect.y = 395;
    items[1].rect.w = 510;  items[1].rect.h = 90;

    if (!button || !button_outline)
    {
        fprintf(stderr, "Error creating Texture: %s\n", IMG_GetError());
        TTF_CloseFont(font1);
        TTF_CloseFont(font2);
        TTF_CloseFont(font3);
        Game_cleanup(window, EXIT_FAILURE);
    }

    Button* buttons = (Button*)malloc(sizeof(Button));
    if (!buttons)
    {
        fprintf(stderr, "Fail with memory!");
        TTF_CloseFont(font1);
        TTF_CloseFont(font2);
        TTF_CloseFont(font3);
        Game_cleanup(window, EXIT_FAILURE);
    }
    CreateTextButton(renderer, buttons, font1, "Menu",  SCREEN_WIDTH/2, 600);

    //Создание надписи о просьбе ввода
    TTF_SetFontOutline(font3, 2);
    SDL_Surface* outline_surface = TTF_RenderText_Blended(font3, "Input save's name", Black);
    TTF_SetFontOutline(font3, 0);
    SDL_Surface* text_surface = TTF_RenderText_Blended(font3, "Input save's name", red);
    
    SDL_Texture* outline_texture = SDL_CreateTextureFromSurface(renderer, outline_surface);
    SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);

    SDL_Rect outline_rect = {SCREEN_WIDTH/2 - outline_surface->w / 2, 200, outline_surface->w, outline_surface->h};
    SDL_Rect text_rect = {SCREEN_WIDTH/2 - text_surface->w / 2, 200, text_surface->w, text_surface->h};

    SDL_FreeSurface(outline_surface);
    SDL_FreeSurface(text_surface);

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
                free(items);
                TTF_CloseFont(font1);
                TTF_CloseFont(font2);
                TTF_CloseFont(font3);
                FreeButtons(buttons, 1);
                SDL_DestroyTexture(text_texture);
                SDL_DestroyTexture(outline_texture);
                board_cleanup_SDL(CheckersBoard);
                Game_cleanup(window, EXIT_SUCCESS);
                break;

            case SDL_MOUSEMOTION:
                int x = event.motion.x;
                int y = event.motion.y;

                buttons->hovered = (x >= buttons->out_rect.x && x <= buttons->out_rect.x + buttons->out_rect.w && 
                                    y >= buttons->out_rect.y && y <= buttons->out_rect.y + buttons->out_rect.h);
                break;
                
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                    if (buttons->hovered)
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
                    SDL_StopTextInput();    //Нужно добавить сюда запись в список рекордов
                    SaveGame_file(input_text, board, info);
                    ShowMiniMenu(window, CheckersBoard, info);
                    break;

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
        
        renderButton(renderer, buttons);
        SDL_RenderCopy(renderer, outline_texture, NULL, &outline_rect);
        SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);

        SDL_RenderCopy(renderer, button_outline, NULL, &items[1].rect);
        SDL_RenderCopy(renderer, button, NULL, &items[0].rect);

        if (strlen(input_text) > 0)
            renderText(renderer, font2, input_text, items[0].rect.x, items[0].rect.y, Black, NULL);

        SDL_RenderPresent(renderer);

        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < 1000/FPS)
        SDL_Delay(1000/FPS - frameTime);
    }

    free(items);
    TTF_CloseFont(font1);
    TTF_CloseFont(font2);
    TTF_CloseFont(font3);
    FreeButtons(buttons, 1);
    SDL_DestroyTexture(text_texture);
    SDL_DestroyTexture(outline_texture);

    switch (selectedItem)
    {
        case 2: 
            SDL_StopTextInput();
            ShowMiniMenu(window, CheckersBoard, info);
            break;
        default: Game_cleanup(window, EXIT_FAILURE); break;
    }
}

//----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$

void LoadGame(Window* window)   //утечки исправлены
{
    SDL_Renderer* renderer = window->renderer;

    // Загрузка шрифта с увеличенным размером
    TTF_Font* font1 = TTF_OpenFont("assets/fonts/minecraft.ttf", FONT_SIZE);
    if (!font1) 
    {
        fprintf(stderr, "Failed to load font: %s", TTF_GetError());
        return;
    }

    // Пункты меню с увеличенными размерами и отступами
    Button* buttons = (Button*)malloc(sizeof(Button) * 3);
    CreateTextButton(renderer, &buttons[0], font1, "Main menu", SCREEN_WIDTH/2,       800);
    CreateTextButton(renderer, &buttons[1], font1, "Prev",      SCREEN_WIDTH/2 - 290, 400);
    CreateTextButton(renderer, &buttons[2], font1, "Next",      SCREEN_WIDTH/2 + 280, 400);

    //Создание фона для столбца сохранений
    SDL_Rect inner_square_rect = {575, 100, 250, 630};
    SDL_Rect outer_square_rect = {570, 95, 260, 640};

    int total_pages = count_paragraphs() / 20 + 1;
    int selectedItem = -1;
    bool running = true;
    int save_choice;
    int page = 1;

    save* Saves_name = read_saves(renderer, page);

    while (running) 
    {
        Uint32 frameStart = SDL_GetTicks();

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type) 
            {
            case SDL_QUIT:
                TTF_CloseFont(font1);
                free_saves(Saves_name);
                FreeButtons(buttons, 3);
                Game_cleanup(window, EXIT_SUCCESS);
                break;

            case SDL_MOUSEMOTION:
                int x = event.motion.x;
                int y = event.motion.y;

                for (int i = 0; i < 3; i++)
                    buttons[i].hovered = (x >= buttons[i].out_rect.x && x <= buttons[i].out_rect.x + buttons[i].out_rect.w && 
                                           y >= buttons[i].out_rect.y && y <= buttons[i].out_rect.y + buttons[i].out_rect.h);

                for (int i = 0; i < 20 && Saves_name[i].Name; i++) 
                    Saves_name[i].hovered = (x >= Saves_name[i].rect.x && x <= Saves_name[i].rect.x + Saves_name[i].rect.w && 
                                             y >= Saves_name[i].rect.y && y <= Saves_name[i].rect.y + Saves_name[i].rect.h);
                break;
                
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    if (buttons[0].hovered) 
                    {
                        selectedItem = 0;
                        running = false;
                    }
                    if (buttons[1].hovered && page != 1)
                    {
                        page--;
                        free_saves(Saves_name);
                        Saves_name = read_saves(renderer, page);
                    }
                    if (buttons[2].hovered && page != total_pages) 
                    {
                        page++;
                        free_saves(Saves_name);
                        Saves_name = read_saves(renderer, page);
                    }
                    for (int i = 1; i < 21; i++)
                    {
                        if (Saves_name[i].hovered)
                        {
                            running = false;
                            selectedItem = 1;
                            save_choice = i;
                        }
                    }
                }
                break;
            }
        }
        
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, window->background, NULL, NULL);
        
        for (int i = 0; i < 3; i++)
            renderButton(renderer, &buttons[i]);
    
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &outer_square_rect);
        
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &inner_square_rect);

        print_saves(renderer, Saves_name);
        
        SDL_RenderPresent(renderer);

        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < 1000/FPS)
        SDL_Delay(1000/FPS - frameTime);
    }

    TTF_CloseFont(font1);
    free_saves(Saves_name);
    FreeButtons(buttons, 3);
    
    switch (selectedItem)
    {
        case 0: showMainMenu(window); break;
        case 1: //выбор сохранения
            char* Save_name = read_save((page - 1) * 20 + save_choice);
            load_from_save(window, Save_name);
            break;
        default: Game_cleanup(window, EXIT_FAILURE); break;
    }
}

//----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$

void LoseMenu(Window* window)   //исправлены утечки
{
    SDL_Renderer* renderer = window->renderer;

    SDL_Color Black = {0, 0, 0, 255};
    SDL_Color red = {255, 0, 0, 255};

    // Загрузка шрифта с увеличенным размером
    TTF_Font* font1 = TTF_OpenFont("assets/fonts/minecraft.ttf", FONT_SIZE);
    TTF_Font* font2 = TTF_OpenFont("assets/fonts/minecraft.ttf", 100);
    if (!font1 || !font2) 
    {
        fprintf(stderr, "Failed to load font: %s", TTF_GetError());
        return;
    }

    // Пункты меню с увеличенными размерами и отступами
    Button* buttons = (Button*)malloc(sizeof(Button) * 1);
    CreateTextButton(renderer, buttons, font1, "Main menu", SCREEN_WIDTH/2, 500);

    //Создание надписи о проигрыше
    TTF_SetFontOutline(font2, 2);
    SDL_Surface* outline_surface = TTF_RenderText_Blended(font2, "YOU LOOOOSEEEER!!!", Black);
    TTF_SetFontOutline(font2, 0);
    SDL_Surface* text_surface = TTF_RenderText_Blended(font2, "YOU LOOOOSEEEER!!!", red);
    
    SDL_Texture* outline_texture = SDL_CreateTextureFromSurface(renderer, outline_surface);
    SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);

    SDL_Rect outline_rect = {SCREEN_WIDTH/2 - outline_surface->w / 2, 200, outline_surface->w, outline_surface->h};
    SDL_Rect text_rect = {SCREEN_WIDTH/2 - text_surface->w / 2, 200, text_surface->w, text_surface->h};

    SDL_FreeSurface(outline_surface);
    SDL_FreeSurface(text_surface);

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
                TTF_CloseFont(font1);
                TTF_CloseFont(font2);
                FreeButtons(buttons, 1);
                SDL_DestroyTexture(text_texture);
                SDL_DestroyTexture(outline_texture);
                Game_cleanup(window, EXIT_SUCCESS);
                break;

            case SDL_MOUSEMOTION:
                int x = event.motion.x;
                int y = event.motion.y;

                buttons->hovered = (x >= buttons->out_rect.x && x <= buttons->out_rect.x + buttons->out_rect.w && 
                                    y >= buttons->out_rect.y && y <= buttons->out_rect.y + buttons->out_rect.h);
                break;
                
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                        if (buttons->hovered) {
                            selectedItem = 0;
                            running = false;
                            break;
                        }
                break;
            
            default:
                break;
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, window->background, NULL, NULL);

        renderButton(renderer, buttons);
        SDL_RenderCopy(renderer, outline_texture, NULL, &outline_rect);
        SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);

        SDL_RenderPresent(renderer);

        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < 1000/FPS)
        SDL_Delay(1000/FPS - frameTime);
    }

    TTF_CloseFont(font1);
    TTF_CloseFont(font2);
    FreeButtons(buttons, 1);
    SDL_DestroyTexture(text_texture);
    SDL_DestroyTexture(outline_texture);

    switch (selectedItem)
    {
        case 0: showMainMenu(window); break;
        default: Game_cleanup(window, EXIT_FAILURE); break;
    }
}

void WinMenu(Window* window, GameInfo info) //исправлены утечки
{
    SDL_Renderer* renderer = window->renderer;

    SDL_Color Black = {0, 0, 0, 255};
    SDL_Color red = {255, 0, 0, 255};
    SDL_Color Grey = {178, 178, 178, 255};

    // Загрузка шрифта с увеличенным размером
    TTF_Font* font1 = TTF_OpenFont("assets/fonts/minecraft.ttf", FONT_SIZE);
    TTF_Font* font2 = TTF_OpenFont("assets/fonts/freesansbold.ttf", 80);
    TTF_Font* font3 = TTF_OpenFont("assets/fonts/minecraft.ttf", 100);
    if (!font1 || !font2 || !font3) 
    {
        fprintf(stderr, "Failed to load font: %s", TTF_GetError());
        return;
    }

    // Пункты меню с увеличенными размерами и отступами
    MenuItem* items = (MenuItem*)malloc(sizeof(MenuItem) * 2);

    SDL_Rect inner_button_rect = {SCREEN_WIDTH/2 - 250, 400, 500, 80};
    SDL_Rect outer_button_rect = {SCREEN_WIDTH/2 - 255, 395, 510, 90};
    items[0].rect = inner_button_rect;
    items[1].rect = outer_button_rect;

    Button* buttons = (Button*)malloc(sizeof(Button));
    CreateTextButton(renderer, buttons, font1, "Main menu", SCREEN_WIDTH/2, 600);

    //Создание надписи о победе
    TTF_SetFontOutline(font3, 2);
    SDL_Surface* outline_surface = TTF_RenderText_Blended(font3, "YOU WIIIIIIIIN!!!", Black);
    TTF_SetFontOutline(font3, 0);
    SDL_Surface* text_surface = TTF_RenderText_Blended(font3, "YOU WIIIIIIIIN!!!", red);
    SDL_Surface* Your_name_surface = TTF_RenderText_Blended(font2, "Your name", Grey);

    SDL_Texture* outline_texture = SDL_CreateTextureFromSurface(renderer, outline_surface);
    SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    SDL_Texture* Your_name_texture = SDL_CreateTextureFromSurface(renderer, Your_name_surface);

    SDL_Rect outline_rect = {SCREEN_WIDTH/2 - outline_surface->w / 2, 200, outline_surface->w, outline_surface->h};
    SDL_Rect text_rect = {SCREEN_WIDTH/2 - text_surface->w / 2, 200, text_surface->w, text_surface->h};

    SDL_FreeSurface(outline_surface);
    SDL_FreeSurface(text_surface);
    SDL_FreeSurface(Your_name_surface);

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
                free(items);
                TTF_CloseFont(font1);
                TTF_CloseFont(font2);
                TTF_CloseFont(font3);
                FreeButtons(buttons, 1);
                SDL_DestroyTexture(text_texture);
                SDL_DestroyTexture(outline_texture);
                SDL_DestroyTexture(Your_name_texture);
                Game_cleanup(window, EXIT_SUCCESS);
                break;

            case SDL_MOUSEMOTION:
                int x = event.motion.x;
                int y = event.motion.y;

                for (int i = 0; i < 2; i++) 
                    items[i].hovered = (x >= items[i].rect.x && x <= items[i].rect.x + items[i].rect.w && 
                                        y >= items[i].rect.y && y <= items[i].rect.y + items[i].rect.h);
                                        
                buttons->hovered = (x >= buttons->out_rect.x && x <= buttons->out_rect.x + buttons->out_rect.w && 
                                    y >= buttons->out_rect.y && y <= buttons->out_rect.y + buttons->out_rect.h);
                break;
                
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    for (int i = 0; i < 2; i++)
                    {
                        if (items[i].hovered)
                        {
                            selectedItem = i;
                            running = false;
                            break;
                        }
                    }
                    if (buttons->hovered)
                    {
                        selectedItem = 2;
                        running = false;
                        break;
                    }
                }
                break;
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, window->background, NULL, NULL);

        renderButton(renderer, buttons);

        SDL_RenderCopy(renderer, outline_texture, NULL, &outline_rect);
        SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
                
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &items[1].rect);
        
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &items[0].rect);

        SDL_RenderCopy(renderer, Your_name_texture, NULL, &items[0].rect);

        SDL_RenderPresent(renderer);

        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < 1000/FPS)
        SDL_Delay(1000/FPS - frameTime);
    }

    free(items);
    TTF_CloseFont(font1);
    TTF_CloseFont(font2);
    TTF_CloseFont(font3);
    FreeButtons(buttons, 1);
    SDL_DestroyTexture(text_texture);
    SDL_DestroyTexture(outline_texture);
    SDL_DestroyTexture(Your_name_texture);

    switch (selectedItem)
    {
        case 0:
        case 1:
            if (WinMenu_InputText(window, info)) 
                showMainMenu(window);
            break;
        case 2: showMainMenu(window); break;
        default: 
            Game_cleanup(window, EXIT_SUCCESS);
            break;
    }
}

int WinMenu_InputText(Window* window, GameInfo info) //исправлены утечки
{
    SDL_Renderer* renderer = window->renderer;

    SDL_Color Black = {0, 0, 0, 255};
    SDL_Color red = {255, 0, 0, 255};

    // Загрузка шрифта с увеличенным размером
    TTF_Font* font1 = TTF_OpenFont("assets/fonts/minecraft.ttf", FONT_SIZE);
    TTF_Font* font2 = TTF_OpenFont("assets/fonts/freesansbold.ttf", 80);
    TTF_Font* font3 = TTF_OpenFont("assets/fonts/minecraft.ttf", 100);
    if (!font1 || !font2 || !font3) 
    {
        fprintf(stderr, "Failed to load font: %s", TTF_GetError());
        return 0;
    }

    // Пункты меню с увеличенными размерами и отступами
    MenuItem* items = (MenuItem*)malloc(sizeof(MenuItem) * 2);

    SDL_Rect inner_button_rect = {SCREEN_WIDTH/2 - 250, 400, 500, 80};
    SDL_Rect outer_button_rect = {SCREEN_WIDTH/2 - 255, 395, 510, 90};
    items[0].rect = inner_button_rect;
    items[1].rect = outer_button_rect;

    Button* buttons = (Button*)malloc(sizeof(Button));
    CreateTextButton(renderer, buttons, font1, "Main menu", SCREEN_WIDTH/2, 600);

//-----------
    //Создание надписи о победе
    TTF_SetFontOutline(font3, 2);
    SDL_Surface* outline_surface = TTF_RenderText_Blended(font3, "YOU WIIIIIIIIN!!!", Black);
    TTF_SetFontOutline(font3, 0);
    SDL_Surface* text_surface = TTF_RenderText_Blended(font3, "YOU WIIIIIIIIN!!!", red);

    SDL_Texture* outline_texture = SDL_CreateTextureFromSurface(renderer, outline_surface);
    SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);

    SDL_Rect outline_rect = {SCREEN_WIDTH/2 - outline_surface->w / 2, 200, outline_surface->w, outline_surface->h};
    SDL_Rect text_rect = {SCREEN_WIDTH/2 - text_surface->w / 2, 200, text_surface->w, text_surface->h};

    SDL_FreeSurface(outline_surface);
    SDL_FreeSurface(text_surface);
//-----------

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
                free(items);
                TTF_CloseFont(font1);
                TTF_CloseFont(font2);
                TTF_CloseFont(font3);
                FreeButtons(buttons, 1);
                SDL_DestroyTexture(outline_texture);
                SDL_DestroyTexture(text_texture);
                Game_cleanup(window, EXIT_SUCCESS);
                break;

            case SDL_MOUSEMOTION:
                int x = event.motion.x;
                int y = event.motion.y;

                buttons->hovered = (x >= buttons->out_rect.x && x <= buttons->out_rect.x + buttons->out_rect.w && 
                                    y >= buttons->out_rect.y && y <= buttons->out_rect.y + buttons->out_rect.h);
                break;
                
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                    if (buttons->hovered)
                    {
                        selectedItem = 1;
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
                    selectedItem = 1;
                    running = false;
                    SaveForLeaderBoard(input_text, info);
                    SDL_StopTextInput();    //Нужно добавить сюда запись в список рекордов
                    break;

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

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, window->background, NULL, NULL);
        
        renderButton(renderer, buttons);
        SDL_RenderCopy(renderer, outline_texture, NULL, &outline_rect);
        SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &items[1].rect);
        
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &items[0].rect);

        if (strlen(input_text) > 0)
            renderText(renderer, font2, input_text, items[0].rect.x, items[0].rect.y, Black, NULL);

        SDL_RenderPresent(renderer);

        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < 1000/FPS)
        SDL_Delay(1000/FPS - frameTime);
    }

    free(items);
    TTF_CloseFont(font1);
    TTF_CloseFont(font2);
    TTF_CloseFont(font3);
    FreeButtons(buttons, 1);
    SDL_DestroyTexture(outline_texture);
    SDL_DestroyTexture(text_texture);

    switch (selectedItem)
    {
        case 1: showMainMenu(window); break;
        default: Game_cleanup(window, EXIT_FAILURE); break;
    }

    return 1;
}

//----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$

void ShowAbout(Window* window)  //исправлены утечки
{
    SDL_Renderer* renderer = window->renderer;

    // Загрузка шрифта с увеличенным размером
    TTF_Font* font = TTF_OpenFont("assets/fonts/minecraft.ttf", FONT_SIZE);
    if (!font) 
    {
        fprintf(stderr, "Failed to load font: %s", TTF_GetError());
        return;
    }

    // Пункты меню с увеличенными размерами и отступами
    MenuItem* items = (MenuItem*)malloc(sizeof(MenuItem) * 2);
    items[1].rect.x = 195; items[1].rect.y = 195; items[1].rect.h = 610; items[1].rect.w = 1010;
    items[2].rect.x = 200; items[2].rect.y = 200; items[2].rect.h = 600; items[2].rect.w = 1000;

    Button* button = (Button*)malloc(sizeof(Button));
    CreateTextButton(renderer, button, font, "Main menu", SCREEN_WIDTH/2, 820);

    about* About = CreateAboutText(renderer);

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
                free(items);
                FreeButtons(button, 1);
                free_about(About);
                TTF_CloseFont(font);
                Game_cleanup(window, EXIT_SUCCESS);
                break;

            case SDL_MOUSEMOTION:
                int x = event.motion.x;
                int y = event.motion.y;

                button->hovered = (x >= button->out_rect.x && x <= button->out_rect.x + button->out_rect.w && 
                                   y >= button->out_rect.y && y <= button->out_rect.y + button->out_rect.h);

                break;
                
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                    if (button->hovered) {
                        selectedItem = 0;
                        running = false;
                        break;
                    }
                break;
            }
        }
        
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, window->background, NULL, NULL);
        
        renderButton(renderer, button);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &items[1].rect);
        
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &items[2].rect);

        for (int i = 0; i < 20; i++)
            if (About->text_arr[i] != NULL)
                SDL_RenderCopy(renderer, About->text_arr[i], NULL, &About->rect_arr[i]);

        SDL_RenderPresent(renderer);

        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < 1000/FPS)
        SDL_Delay(1000/FPS - frameTime);
    }

    free(items);
    FreeButtons(button, 1);
    free_about(About);
    TTF_CloseFont(font);

    switch (selectedItem)
    {
        case 0: showMainMenu(window); break;
        default: Game_cleanup(window, EXIT_FAILURE); break;
    }
}

void free_about(about* About)
{
    for (int i = 0; i < 20; i++)
        SDL_DestroyTexture(About->text_arr[i]);
    free(About->rect_arr);
    free(About->text_arr);
    free(About);
}

about* CreateAboutText(SDL_Renderer* renderer)
{
    SDL_Color Black = {0, 0, 0, 255};
    TTF_Font* font = TTF_OpenFont("assets/fonts/freesansbold.ttf", 30);

    about* About = (about*)malloc(sizeof(about));

    About->rect_arr = (SDL_Rect*)malloc(sizeof(SDL_Rect) * 20);
    About->text_arr = (SDL_Texture**)malloc(sizeof(SDL_Texture*) * 20);
    for (int i = 0; i < 20; i++)
    {
        About->text_arr[i] = NULL;
        About->rect_arr[i].x = 200; About->rect_arr[i].y = 200 + i * 30;
    }

    SDL_Surface* text = TTF_RenderText_Blended(font, "The work was performed by 1st year students of Peter the Great", Black);
    About->rect_arr[0].w = text->w; About->rect_arr[0].h = text->h;
    About->text_arr[0] = SDL_CreateTextureFromSurface(renderer, text);
    SDL_FreeSurface(text);

    text = TTF_RenderText_Blended(font, "St. Petersburg University, Institute of Computer Science and", Black);
    About->rect_arr[1].w = text->w; About->rect_arr[1].h = text->h;
    About->text_arr[1] = SDL_CreateTextureFromSurface(renderer, text);
    SDL_FreeSurface(text);

    text = TTF_RenderText_Blended(font, "Cybersecurity, Department of Information Security, Lena Golovach", Black);
    About->rect_arr[2].w = text->w; About->rect_arr[2].h = text->h;
    About->text_arr[2] = SDL_CreateTextureFromSurface(renderer, text);
    SDL_FreeSurface(text);

    text = TTF_RenderText_Blended(font, "and Panki HOY!", Black);
    About->rect_arr[3].w = text->w; About->rect_arr[3].h = text->h;
    About->text_arr[3] = SDL_CreateTextureFromSurface(renderer, text);
    SDL_FreeSurface(text);

    TTF_CloseFont(font);

    return About;
}

//----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$

void ShowLeaderBoard(Window* window)    //исправлены утечки
{
    SDL_Renderer* renderer = window->renderer;

    SDL_Color Black = {0, 0, 0, 255};
    SDL_Color Green = {0, 255, 0, 255};

    // Загрузка шрифта с увеличенным размером
    TTF_Font* font = TTF_OpenFont("assets/fonts/minecraft.ttf", FONT_SIZE);
    if (!font) 
    {
        fprintf(stderr, "Failed to load font: %s", TTF_GetError());
        return;
    }

    // Пункты меню с увеличенными размерами и отступами
    MenuItem* items = (MenuItem*)malloc(sizeof(MenuItem) * 6);

    items[0].rect.x = 100;  items[0].rect.y = 100;  items[0].rect.h = 630; items[0].rect.w = 250;
    items[1].rect.x = 575;  items[1].rect.y = 100;  items[1].rect.h = 630; items[1].rect.w = 250;
    items[2].rect.x = 1050; items[2].rect.y = 100;  items[2].rect.h = 630; items[2].rect.w = 250;

    items[3].rect.x = 95;   items[3].rect.y = 95;   items[3].rect.h = 640; items[3].rect.w = 260;
    items[4].rect.x = 570;  items[4].rect.y = 95;   items[4].rect.h = 640; items[4].rect.w = 260;
    items[5].rect.x = 1045; items[5].rect.y = 95;   items[5].rect.h = 640; items[5].rect.w = 260;

    SDL_Rect inner_rect[3] = {items[0].rect, items[1].rect, items[2].rect};
    SDL_Rect outer_rect[3] = {items[3].rect, items[4].rect, items[5].rect};

    leader* easyLeaders = read_leaders(renderer, 1, items[1].rect.x, items[1].rect.y);
    leader* mediumLeaders = read_leaders(renderer, 2, items[2].rect.x, items[2].rect.y);
    leader* hardLeaders = read_leaders(renderer, 3, items[3].rect.x, items[3].rect.y);

    Button* buttons = (Button*)malloc(sizeof(Button));
    CreateTextButton(renderer, buttons, font, "Main menu", SCREEN_WIDTH/2, 800);

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
                free(items);
                TTF_CloseFont(font);
                FreeButtons(buttons, 1);
                free_leaders(easyLeaders);
                free_leaders(mediumLeaders);
                free_leaders(hardLeaders);
                Game_cleanup(window, EXIT_SUCCESS);
                break;

            case SDL_MOUSEMOTION:
                int x = event.motion.x;
                int y = event.motion.y;

                buttons->hovered = (x >= buttons->out_rect.x && x <= buttons->out_rect.x + buttons->out_rect.w && 
                                    y >= buttons->out_rect.y && y <= buttons->out_rect.y + buttons->out_rect.h);

                break;
                
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                    if (buttons->hovered) {
                        selectedItem = 0;
                        running = false;
                        break;
                    }
                break;
            }
        }
        
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, window->background, NULL, NULL);

        renderButton(renderer, buttons);
        renderText(renderer, font, "EASY", items[0].rect.x, items[0].rect.y - 80, Green, &Black);
        renderText(renderer, font, "MIDL", items[1].rect.x, items[1].rect.y - 80, Green, &Black);
        renderText(renderer, font, "HARD", items[2].rect.x, items[2].rect.y - 80, Green, &Black);
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRects(renderer, outer_rect, 3);
        
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRects(renderer, inner_rect, 3);

        PrintLeaders(renderer, easyLeaders);
        PrintLeaders(renderer, mediumLeaders);
        PrintLeaders(renderer, hardLeaders);
        
        SDL_RenderPresent(renderer);

        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < 1000/FPS)
        SDL_Delay(1000/FPS - frameTime);
    }

    free(items);
    TTF_CloseFont(font);
    FreeButtons(buttons, 1);
    free_leaders(easyLeaders);
    free_leaders(mediumLeaders);
    free_leaders(hardLeaders);

    switch (selectedItem)
    {
        case 0: showMainMenu(window); return;
        default: Game_cleanup(window, EXIT_FAILURE); break;
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
    }
}

//----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$----------$$$$$$$$$$

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

void CreateTextButton(SDL_Renderer* renderer, Button* buttons, TTF_Font* font, const char* text, int x, int y)
{
    SDL_Color Green = {0, 255, 0, 255};
    SDL_Color Black = {0, 0, 0, 255};
    SDL_Color red = {255, 0, 0, 255};

    TTF_SetFontOutline(font, 2);
    SDL_Surface* out_surface = TTF_RenderText_Blended(font, text, Black);

    TTF_SetFontOutline(font, 0);
    SDL_Surface* inner_surface_disable = TTF_RenderText_Blended(font, text, Green);
    SDL_Surface* inner_surface_enable = TTF_RenderText_Blended(font, text, red);

    buttons->out_texture = SDL_CreateTextureFromSurface(renderer, out_surface);
    buttons->inner_texture_disable = SDL_CreateTextureFromSurface(renderer, inner_surface_disable);
    buttons->inner_texture_enable = SDL_CreateTextureFromSurface(renderer, inner_surface_enable);

    // получение координат rect для обводки
    buttons->out_rect.w = out_surface->w;
    buttons->out_rect.h = out_surface->h;
    buttons->out_rect.x = x - (buttons->out_rect.w) / 2;
    buttons->out_rect.y = y;

    buttons->inner_rect.w = inner_surface_disable->w;
    buttons->inner_rect.h = inner_surface_disable->h;
    buttons->inner_rect.x = x - (buttons->inner_rect.w) / 2;
    buttons->inner_rect.y = y;

    buttons->hovered = false;

    SDL_FreeSurface(out_surface);
    SDL_FreeSurface(inner_surface_disable);
    SDL_FreeSurface(inner_surface_enable);
}

void FreeButtons(Button* buttons, int count)
{
    for (int i = 0; i < count; i++)
    {
        SDL_DestroyTexture(buttons->out_texture);
        SDL_DestroyTexture(buttons->inner_texture_disable);
        SDL_DestroyTexture(buttons->inner_texture_enable);
    }
    free(buttons);
}

void renderButton(SDL_Renderer* renderer, Button* buttons)
{
    SDL_RenderCopy(renderer, buttons->out_texture, NULL, &buttons->out_rect);
    
    if (buttons->hovered)
        SDL_RenderCopy(renderer, buttons->inner_texture_enable, NULL, &buttons->inner_rect);
    else
        SDL_RenderCopy(renderer, buttons->inner_texture_disable, NULL, &buttons->inner_rect);
}