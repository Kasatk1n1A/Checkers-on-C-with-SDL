# Функции
## SDL_CreateRenderer()
**SDL_CreateRenderer()** — это функция в библиотеке SDL (Simple DirectMedia Layer), которая используется для создания рендерера, связанного с указанным окном. Рендерер представляет собой абстракцию над графическими API и используется для отрисовки графики на экране.

**Синтаксис функции SDL_CreateRenderer:**

```
SDL_Renderer* SDL_CreateRenderer(SDL_Window* window, int index, Uint32 flags);
```

**Параметры функции:**

- `window` — окно, с которым будет связан рендерер.
- `index` — индекс устройства рендеринга. Обычно используется значение -1 для использования устройства рендеринга по умолчанию.
- `flags` — флаги, определяющие поведение рендерера. Это может быть комбинация следующих значений:
    - SDL_RENDERER_SOFTWARE — использовать программный рендерер.
    - SDL_RENDERER_ACCELERATED — использовать аппаратное ускорение рендеринга (по умолчанию).
    - SDL_RENDERER_PRESENTVSYNC — синхронизировать представление с вертикальной синхронизацией (V-Sync).

**Возвращаемое значение:**

- Если функция успешно создаёт рендерер, она возвращает указатель на созданный рендерер типа SDL_Renderer*.
- Если происходит ошибка, функция возвращает NULL, и можно использовать функцию SDL_GetError для получения дополнительной информации об ошибке.

**Пример использования SDL_CreateRenderer для создания рендерера:**

```
#include <SDL.h>

int main(int argc, char *argv[]) {
    SDL_Window* window = SDL_CreateWindow("My Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Error creating window: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        return 1;
    }
    // Здесь можно работать с рендерером
    SDL_DestroyRenderer(renderer); // Уничтожение рендерера
    SDL_DestroyWindow(window); // Уничтожение окна
    SDL_Quit(); // Завершение работы со всеми подсистемами SDL
    return 0;
}
```
## SDL_CreateWindow()
**SDL_CreateWindow()** — это функция в библиотеке SDL (Simple DirectMedia Layer), которая используется для создания нового окна.

**Синтаксис функции SDL_CreateWindow:**

```
SDL_Window* SDL_CreateWindow(const char* title, int x, int y, int w, int h, Uint32 flags);
```

**Параметры функции:**

- `title` — заголовок окна.
- `x`, `y` — координаты верхнего левого угла окна на экране. Обычно используются значения SDL_WINDOWPOS_UNDEFINED, чтобы позволить системе определить позицию автоматически.
- `w`, `h` — ширина и высота окна.
- `flags` — флаги, определяющие поведение окна. Это может быть комбинация следующих значений:
    - SDL_WINDOW_SHOWN — окно будет отображаться сразу после создания.
    - SDL_WINDOW_FULLSCREEN — окно будет полноэкранным.
    - SDL_WINDOW_BORDERLESS — окно будет без рамки.
    - SDL_WINDOW_RESIZABLE — окно будет изменяемым по размеру.
    - SDL_WINDOW_MINIMIZED — окно будет свёрнуто по умолчанию.
    - SDL_WINDOW_MAXIMIZED — окно будет развёрнуто на весь экран по умолчанию.
    - и другие.

**Возвращаемое значение:**

- Если функция успешно создаёт окно, она возвращает указатель на созданное окно типа SDL_Window*.
- Если происходит ошибка, функция возвращает NULL, и можно использовать функцию SDL_GetError для получения дополнительной информации об ошибке.

**Пример использования SDL_CreateWindow для создания окна:**

```
#include <SDL.h>

int main(int argc, char *argv[]) {
    SDL_Window* window = SDL_CreateWindow("My Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Error creating window: %s\n", SDL_GetError());
        return 1;
    }
    // Здесь можно работать с окном
    SDL_DestroyWindow(window); // Уничтожение окна
    SDL_Quit(); // Завершение работы со всеми подсистемами SDL
    return 0;
}
```
## SDL_Delay()
**SDL_Delay()** — это функция в библиотеке SDL (Simple DirectMedia Layer), которая используется для приостановки выполнения программы на заданное количество миллисекунд.

**Синтаксис функции SDL_Delay:**

```
void SDL_Delay(Uint32 ms);
```

**Параметр функции:**

- `ms` — количество миллисекунд, на которое нужно приостановить выполнение программы.

Эта функция не возвращает значения. Она просто приостанавливает выполнение программы на указанное время.

**Пример использования SDL_Delay для приостановки выполнения программы:**

```
#include <SDL.h>

int main(int argc, char *argv[]) {
    SDL_Window* window = SDL_CreateWindow("My Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Error creating window: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        return 1;
    }

    // Установка цвета очистки
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Красный цвет

    // Очистка рендерера
    SDL_RenderClear(renderer);

    // Отображение изменений на экране
    SDL_RenderPresent(renderer);

    // Приостановка выполнения программы на 2 секунды
    SDL_Delay(2000);

    SDL_DestroyRenderer(renderer); // Уничтожение рендерера
    SDL_DestroyWindow(window); // Уничтожение окна
    SDL_Quit(); // Завершение работы со всеми подсистемами SDL
    return 0;
}
```
## SDL_DestroyRenderer()
**SDL_DestroyRenderer()** — это функция в библиотеке SDL (Simple DirectMedia Layer), которая используется для уничтожения рендерера и освобождения связанных с ним ресурсов.

**Синтаксис функции SDL_DestroyRenderer:**

```
void SDL_DestroyRenderer(SDL_Renderer* renderer);
```

**Параметр функции:**

- `renderer` — указатель на рендерер, который нужно уничтожить.

Эта функция не возвращает значения. После вызова SDL_DestroyRenderer рендерер больше не может быть использован, и все ресурсы, связанные с ним, будут освобождены.

**Пример использования SDL_DestroyRenderer для уничтожения рендерера:**

```
#include <SDL.h>

int main(int argc, char *argv[]) {
    SDL_Window* window = SDL_CreateWindow("My Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Error creating window: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        return 1;
    }
    // Здесь можно работать с рендерером
    SDL_DestroyRenderer(renderer); // Уничтожение рендерера
    SDL_DestroyWindow(window); // Уничтожение окна
    SDL_Quit(); // Завершение работы со всеми подсистемами SDL
    return 0;
}
```
## SDL_DestroyWindow()
**SDL_DestroyWindow()** — это функция в библиотеке SDL (Simple DirectMedia Layer), которая используется для уничтожения окна и освобождения связанных с ним ресурсов.

**Синтаксис функции SDL_DestroyWindow:**

```
void SDL_DestroyWindow(SDL_Window* window);
```

**Параметр функции:**

- `window` — указатель на окно, которое нужно уничтожить.

Эта функция не возвращает значения. После вызова SDL_DestroyWindow окно больше не может быть использовано, и все ресурсы, связанные с ним, будут освобождены.

**Пример использования SDL_DestroyWindow для уничтожения окна:**

```
#include <SDL.h>

int main(int argc, char *argv[]) {
    SDL_Window* window = SDL_CreateWindow("My Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Error creating window: %s\n", SDL_GetError());
        return 1;
    }
    // Здесь можно работать с окном
    SDL_DestroyWindow(window); // Уничтожение окна
    SDL_Quit(); // Завершение работы со всеми подсистемами SDL
    return 0;
}
```

## SDL_Init()
**SDL_Init()** — это функция в библиотеке SDL (Simple DirectMedia Layer), которая используется для инициализации основных подсистем, необходимых для работы с SDL.

**Синтаксис функции SDL_Init:**

```
int SDL_Init(Uint32 flags);
```

**Параметр flags** — это битовая маска, которая указывает, какие подсистемы SDL нужно инициализировать. Например, можно инициализировать подсистему видео, аудио, таймер и т. д.

**Возвращаемое значение:**
- Если функция успешно инициализирует указанные подсистемы, она возвращает 0.
- Если происходит ошибка, функция возвращает -1, и можно использовать функцию SDL_GetError() для получения дополнительной информации об ошибке.
- 
**Пример использования SDL_Init для инициализации подсистемы видео:**

```
#include <SDL.h>

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }
    // Здесь можно использовать подсистему видео
    SDL_Quit(); // Завершение работы со всеми подсистемами SDL
    return 0;
}
```
`flags` может быть любым из следующих Или вместе взятых:

- [`SDL_INIT_TIMER`](https://wiki.libsdl.org/SDL2/SDL_INIT_TIMER): подсистема таймера
- [`SDL_INIT_AUDIO`](https://wiki.libsdl.org/SDL2/SDL_INIT_AUDIO): аудиоподсистема
- [`SDL_INIT_VIDEO`](https://wiki.libsdl.org/SDL2/SDL_INIT_VIDEO): видеоподсистема; автоматически инициализирует подсистему событий
- [`SDL_INIT_JOYSTICK`](https://wiki.libsdl.org/SDL2/SDL_INIT_JOYSTICK): подсистема джойстика; автоматически инициализирует подсистему событий
- [`SDL_INIT_HAPTIC`](https://wiki.libsdl.org/SDL2/SDL_INIT_HAPTIC): тактильная подсистема (силовая обратная связь)
- [`SDL_INIT_GAMECONTROLLER`](https://wiki.libsdl.org/SDL2/SDL_INIT_GAMECONTROLLER): подсистема контроллера; автоматически инициализирует подсистему джойстика
- [`SDL_INIT_EVENTS`](https://wiki.libsdl.org/SDL2/SDL_INIT_EVENTS): подсистема событий
- [`SDL_INIT_EVERYTHING`](https://wiki.libsdl.org/SDL2/SDL_INIT_EVERYTHING): все вышеперечисленные подсистемы
- [`SDL_INIT_NOPARACHUTE`](https://wiki.libsdl.org/SDL2/SDL_INIT_NOPARACHUTE): совместимость; этот флаг игнорируется
## SDL_InitSubSystem()
**SDL_InitSubSystem** — это функция в библиотеке SDL (Simple DirectMedia Layer), которая позволяет инициализировать подсистемы SDL по отдельности, вместо того чтобы инициализировать все подсистемы сразу с помощью функции SDL_Init.

**Синтаксис функции SDL_InitSubSystem:**

```
int SDL_InitSubSystem(Uint32 flags);
```

**Параметр flags** — это битовая маска, которая указывает, какие подсистемы SDL нужно инициализировать. Например, можно инициализировать только подсистему аудио или только подсистему графики.

**Возвращаемое значение:**

- Если функция успешно инициализирует указанные подсистемы, она возвращает 0.
- Если происходит ошибка, функция возвращает -1, и можно использовать функцию SDL_GetError для получения дополнительной информации об ошибке.

**Пример использования SDL_InitSubSystem для инициализации подсистемы аудио:**

```
#include <SDL.h>

int main(int argc, char *argv[]) {
    if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
        printf("Error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }
    // Здесь можно использовать подсистему аудио
    SDL_QuitSubSystem(SDL_INIT_AUDIO); // Завершение работы с подсистемой аудио
    SDL_Quit(); // Завершение работы со всеми подсистемами SDL
    return 0;
}
```

## SDL_GetError

Извлекает сообщение о последней ошибке, произошедшей в текущем потоке.
**Возвращаемое значение[![](https://wiki.libsdl.org/static_files/link.svg)](https://wiki.libsdl.org/SDL2/SDL_GetError#return-value)**

(const char \*) Возвращает сообщение с информацией о конкретной ошибке, которая произошла, или пустую строку, если с момента последнего вызова [SDL_ClearError](https://wiki.libsdl.org/SDL2/SDL_ClearError)() не было установлено сообщение об ошибке. Сообщение применимо только в том случае, если функция SDL сигнализировала об ошибке. Вы должны проверять возвращаемые значения вызовов функций SDL, чтобы определить, когда следует вызывать [SDL_GetError](https://wiki.libsdl.org/SDL2/SDL_GetError)().

**Замечания[![](https://wiki.libsdl.org/static_files/link.svg)](https://wiki.libsdl.org/SDL2/SDL_GetError#remarks)**

Перед вызовом [SDL_GetError](https://wiki.libsdl.org/SDL2/SDL_GetError)(). может возникнуть несколько ошибок. Возвращается только последняя ошибка.

Это сообщение применимо только в том случае, если функция SDL сигнализирует об ошибке. Вы должны проверять возвращаемые значения вызовов функций SDL, чтобы определить, когда следует вызывать [SDL_GetError](https://wiki.libsdl.org/SDL2/SDL_GetError)(). Вы _не должны_ использовать результаты [SDL_GetError](https://wiki.libsdl.org/SDL2/SDL_GetError)() для определения того, произошла ли ошибка! Иногда SDL устанавливает строку ошибки даже при успешном выполнении.

SDL _не_ очищает строку ошибок при успешных вызовах API. Вы _должны_ проверять возвращаемые значения в случае сбоя, прежде чем предполагать, что строка ошибок применима.

Строки ошибок задаются для каждого потока отдельно, поэтому ошибка, заданная в другом потоке, не повлияет на работу текущего потока.

Возвращаемая строка выделяется внутри приложения и не должна быть освобождена приложением.

## SDL_RenderClear()
**SDL_RenderClear()** — это функция в библиотеке SDL (Simple DirectMedia Layer), которая используется для очистки текущего рендерера, заполнив его текущим цветом очистки.

**Синтаксис функции SDL_RenderClear:**

```
int SDL_RenderClear(SDL_Renderer* renderer);
```

**Параметр функции:**

- `renderer` — указатель на рендерер, который нужно очистить.

**Возвращаемое значение:**

- Если функция успешно очищает рендерер, она возвращает 0.
- Если происходит ошибка, функция возвращает -1, и можно использовать функцию SDL_GetError для получения дополнительной информации об ошибке.

Перед вызовом SDL_RenderClear обычно устанавливается цвет очистки с помощью функции SDL_SetRenderDrawColor.

**Пример использования SDL_RenderClear для очистки рендерера:**

```
#include <SDL.h>

int main(int argc, char *argv[]) {
    SDL_Window* window = SDL_CreateWindow("My Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Error creating window: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        return 1;
    }

    // Установка цвета очистки
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Красный цвет

    // Очистка рендерера
    if (SDL_RenderClear(renderer) < 0) {
        printf("Error clearing renderer: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        return 1;
    }

    SDL_RenderPresent(renderer); // Представление изменений на экране

    SDL_DestroyRenderer(renderer); // Уничтожение рендерера
    SDL_DestroyWindow(window); // Уничтожение окна
    SDL_Quit(); // Завершение работы со всеми подсистемами SDL
    return 0;
}
```
## SDL_RenderPresent()
**SDL_RenderPresent()** — это функция в библиотеке SDL (Simple DirectMedia Layer), которая используется для отображения всех изменений, произведённых на рендерере, на экране.

**Синтаксис функции SDL_RenderPresent:**

```
int SDL_RenderPresent(SDL_Renderer* renderer);
```

**Параметр функции:**

- `renderer` — указатель на рендерер, изменения которого нужно отобразить.

**Возвращаемое значение:**

- Если функция успешно отображает изменения, она возвращает 0.
- Если происходит ошибка, функция возвращает -1, и можно использовать функцию SDL_GetError для получения дополнительной информации об ошибке.

Эта функция обычно вызывается после того, как все необходимые графические операции были выполнены на рендерере, чтобы обновить содержимое окна и сделать его видимым для пользователя.

**Пример использования SDL_RenderPresent для отображения изменений на экране:**

```
#include <SDL.h>

int main(int argc, char *argv[]) {
    SDL_Window* window = SDL_CreateWindow("My Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Error creating window: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        return 1;
    }

    // Установка цвета очистки
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Красный цвет

    // Очистка рендерера
    SDL_RenderClear(renderer);

    // Отображение изменений на экране
    SDL_RenderPresent(renderer);

    SDL_DestroyRenderer(renderer); // Уничтожение рендерера
    SDL_DestroyWindow(window); // Уничтожение окна
    SDL_Quit(); // Завершение работы со всеми подсистемами SDL
    return 0;
}
```
## SDL_Quit()
**SDL_Quit()** — это функция в библиотеке SDL (Simple DirectMedia Layer), которая используется для завершения работы со всеми подсистемами SDL и освобождения связанных с ними ресурсов.

**Синтаксис функции SDL_Quit:**

```
void SDL_Quit(void);
```

Эта функция не имеет параметров и не возвращает значения. После вызова SDL_Quit все подсистемы SDL будут корректно завершены, и все ресурсы будут освобождены.

**Пример использования SDL_Quit для завершения работы с SDL:**

```
#include <SDL.h>

int main(int argc, char *argv[]) {
    SDL_Window* window = SDL_CreateWindow("My Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Error creating window: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        return 1;
    }
    // Здесь можно работать с рендерером и окном
    SDL_DestroyRenderer(renderer); // Уничтожение рендерера
    SDL_DestroyWindow(window); // Уничтожение окна
    SDL_Quit(); // Завершение работы со всеми подсистемами SDL
    return 0;
}
```
# Структуры

## SDL_Render
**SDL_Renderer** — это структура в библиотеке SDL (Simple DirectMedia Layer), которая представляет собой абстракцию над графическими API, такими как OpenGL, Direct3D и другими. Она используется для отрисовки графики на экране.

Основные функции, связанные с SDL_Renderer:

1. **Создание рендерера:**
    
    ```
    SDL_Renderer* SDL_CreateRenderer(SDL_Window* window, int index, Uint32 flags);
    ```
    
    - `window` — окно, в котором будет отображаться графика.
    - `index` — индекс устройства рендеринга (обычно -1 для использования по умолчанию).
    - `flags` — флаги, определяющие поведение рендерера (например, SDL_RENDERER_ACCELERATED для аппаратного ускорения).
2. **Уничтожение рендерера:**
    
    ```
    void SDL_DestroyRenderer(SDL_Renderer* renderer);
    ```
    
3. **Настройка цвета очистки:**
    
    ```
    void SDL_SetRenderDrawColor(SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    ```
    
    - `r`, `g`, `b`, `a` — компоненты цвета (красный, зелёный, синий, альфа).
4. **Очистка рендерера:**
    
    ```
    int SDL_RenderClear(SDL_Renderer* renderer);
    ```
    
5. **Отрисовка объектов:**
    
    - Прямоугольники:
        
        ```
        int SDL_RenderFillRect(SDL_Renderer* renderer, const SDL_Rect* rect);
        int SDL_RenderDrawRect(SDL_Renderer* renderer, const SDL_Rect* rect);
        ```
        
    - Точки и линии:
        
        ```
        int SDL_RenderDrawPoint(SDL_Renderer* renderer, int x, int y);
        int SDL_RenderDrawLine(SDL_Renderer* renderer, int x1, int y1, int x2, int y2);
        ```
        
6. **Копирование текстуры на экран:**
    
    ```
    int SDL_RenderCopy(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_Rect* src, const SDL_Rect* dst);
    ```
    
    - `texture` — текстура, которую нужно отобразить.
    - `src` — исходный прямоугольник на текстуре.
    - `dst` — целевой прямоугольник на экране.
7. **Представление изменений на экране:**
    
    ```
    int SDL_RenderPresent(SDL_Renderer* renderer);
    ```

## SDL_Window
**SDL_Window** — это структура в библиотеке SDL (Simple DirectMedia Layer), которая представляет окно для отображения графики, текста и других элементов интерфейса.

Основные функции, связанные с SDL_Window:

1. **Создание окна:**
    
    ```
    SDL_Window* SDL_CreateWindow(const char* title, int x, int y, int w, int h, Uint32 flags);
    ```
    
    - `title` — заголовок окна.
    - `x`, `y` — координаты верхнего левого угла окна на экране.
    - `w`, `h` — ширина и высота окна.
    - `flags` — флаги, определяющие поведение окна (например, SDL_WINDOW_SHOWN для отображения окна после создания).
2. **Уничтожение окна:**
    
    ```
    void SDL_DestroyWindow(SDL_Window* window);
    ```
    
3. **Получение размеров окна:**
    
    ```
    int SDL_GetWindowSize(SDL_Window* window, int* w, int* h);
    ```
    
    - `w`, `h` — указатели на переменные, в которые будут записаны ширина и высота окна.
4. **Изменение размеров окна:**
    
    ```
    int SDL_SetWindowSize(SDL_Window* window, int w, int h);
    ```
    
    - `w`, `h` — новые ширина и высота окна.

# Макросы

## SDL_WINDOWPOS_CENTERED
**SDL_WINDOWPOS_CENTERED** — это специальное значение в библиотеке SDL (Simple DirectMedia Layer), которое используется при создании окна для указания того, что окно должно быть расположено по центру экрана.

Это значение может быть передано в качестве параметров `x` и `y` в функцию `SDL_CreateWindow`, чтобы система автоматически разместила окно по центру.

**Пример использования SDL_WINDOWPOS_CENTERED для создания окна:**

```
#include <SDL.h>

int main(int argc, char *argv[]) {
    SDL_Window* window = SDL_CreateWindow("My Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Error creating window: %s\n", SDL_GetError());
        return 1;
    }
    // Здесь можно работать с окном
    SDL_DestroyWindow(window); // Уничтожение окна
    SDL_Quit(); // Завершение работы со всеми подсистемами SDL
    return 0;
}
```