#include "visualise.h"



void text_cleanup(Text* text, int N)
{
    // Удаление всех текстовых элементов
    for (int i = 0; i < N; i++)
    {
        // Удаление текстуры текста
        if (text[i].image)
            SDL_DestroyTexture(text[i].image);
        
        // Закрытие шрифта
        if (text[i].font)
            TTF_CloseFont(text[i].font);
    }
    
    // Освобождение массива текстов
    free(text);
}

bool load_text(Game* game, char* text, int r, int g, int b, int a, int x, int y, int size)
{

}