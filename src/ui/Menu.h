#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "../core/game_rules.h"
#include "../utils/sdl_utils.h"
#include "../../include/core/ui_types.h"
#include "../../include/core/Board_types.h"
#include "texture_manager.h"
#include "../core/saves.h"

typedef struct
{
    SDL_Texture* Name;
    SDL_Texture* minutes;
    SDL_Texture* seconds;
    SDL_Rect Name_rect;
    SDL_Rect minutes_rect;
    SDL_Rect seconds_rect;
} leader;

typedef struct 
{
    SDL_Texture* Name;
    SDL_Rect rect;
    bool hovered;
} save;

typedef struct 
{
    SDL_Texture** text_arr;
    SDL_Rect* rect_arr;
} about;

typedef struct 
{
    SDL_Texture* out_texture;
    SDL_Texture* inner_texture_disable;
    SDL_Texture* inner_texture_enable;
    SDL_Rect out_rect;
    SDL_Rect inner_rect;
    bool hovered;
} Button;

void FreeButtons(Button* buttons, int count);
void renderButton(SDL_Renderer* renderer, Button* buttons);
void CreateTextButton1(SDL_Renderer* renderer, Button* buttons, TTF_Font* font, const char* text, int x, int y);

void LoseMenu(Window* window);
void WinMenu(Window* window, GameInfo info);
int WinMenu_InputText(Window* window, GameInfo info);

int count_paragraphs(void);
void free_saves(save* saves);
void LoadGame(Window* window);
char* read_save(int name_count);
void LoadGame_InputText(Window* window);
save* read_saves(SDL_Renderer* renderer, int page);
void print_saves(SDL_Renderer* renderer, save* saves);
int delete_line_from_file(const char *filename, int line_to_delete);

void SaveGame(Window* window, Board* CheckersBoard, GameInfo info);
void SaveGame_InputText(Window* window, Board* CheckersBoard, GameInfo info);

void SetDifficult(Window* window);
void showMainMenu(Window* window);

void ShowMiniMenu(Window* window, Board* CheckersBoard, GameInfo info);

void free_about(about* About);
void ShowAbout(Window* window);
about* CreateAboutText(SDL_Renderer* renderer);

void free_leaders(leader* leaders);
void ShowLeaderBoard(Window* window);
void PrintLeaders(SDL_Renderer* renderer, leader* leaders);
leader* read_leaders(SDL_Renderer* renderer, int type, int x, int y);

void CreateTextButton(MenuItem* item, TTF_Font* font, const char* text, int x, int y, int w, int h);
void renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y, SDL_Color text_color, SDL_Color *outline_color);

#endif