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

## SDL_PollEvent()
**SDL_PollEvent()** — это функция в библиотеке SDL (Simple DirectMedia Layer), которая используется для обработки событий в цикле обработки событий программы. Она позволяет проверить наличие новых событий в очереди событий и, если событие доступно, извлечь его и обработать.

### Основные моменты использования `SDL_PollEvent`:

1. **Проверка событий**: `SDL_PollEvent` проверяет, есть ли новые события в очереди. Если событие доступно, оно копируется в структуру, на которую указывает аргумент функции, и возвращается значение `1`. Если событий нет, возвращается `0`.
    
2. **Цикл обработки событий**: обычно `SDL_PollEvent` используется в цикле, который непрерывно проверяет наличие событий и обрабатывает их по мере поступления. Это позволяет программе реагировать на пользовательский ввод и другие события в реальном времени.
    
3. **Структура события**: функция принимает указатель на структуру `SDL_Event`, которая описывает различные типы событий, такие как нажатие клавиш, движение мыши, закрытие окна и т. д.
    

### Пример использования `SDL_PollEvent`:

```
#include <SDL2/SDL.h>

int main(int argc, char *argv[]) {
    SDL_Event event;

    // Инициализация SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    // Создание окна
    SDL_Window *window = SDL_CreateWindow("SDL Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    if (!window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Основной цикл программы
    int running = 1;
    while (running) {
        // Проверка событий
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = 0;
                    break;
                // Обработка других событий
                default:
                    break;
            }
        }

        // Обновление экрана и другие действия
        // ...
    }

    // Освобождение ресурсов и завершение SDL
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
```

В этом примере:

- Инициализируется SDL и создаётся окно.
- В основном цикле программы используется `SDL_PollEvent` для проверки событий.
- При получении события `SDL_QUIT` (например, при закрытии окна) программа завершает работу.
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
## [SDL_Event](https://wiki.libsdl.org/SDL2/SDL_Event)
**SDL_Event** — это структура в библиотеке SDL, которая используется для представления различных типов событий, таких как нажатия клавиш, движения мыши, закрытие окна и другие. Эта структура является ключевым элементом для обработки событий в приложениях, использующих SDL.
### Синтаксис[![](https://wiki.libsdl.org/static_files/link.svg)](https://wiki.libsdl.org/SDL2/SDL_Event#syntax)

```
typedef union SDL_Event
{
    Uint32 type;                            /**< Event type, shared with all events */
    SDL_CommonEvent common;                 /**< Common event data */
    SDL_DisplayEvent display;               /**< Display event data */
    SDL_WindowEvent window;                 /**< Window event data */
    SDL_KeyboardEvent key;                  /**< Keyboard event data */
    SDL_TextEditingEvent edit;              /**< Text editing event data */
    SDL_TextEditingExtEvent editExt;        /**< Extended text editing event data */
    SDL_TextInputEvent text;                /**< Text input event data */
    SDL_MouseMotionEvent motion;            /**< Mouse motion event data */
    SDL_MouseButtonEvent button;            /**< Mouse button event data */
    SDL_MouseWheelEvent wheel;              /**< Mouse wheel event data */
    SDL_JoyAxisEvent jaxis;                 /**< Joystick axis event data */
    SDL_JoyBallEvent jball;                 /**< Joystick ball event data */
    SDL_JoyHatEvent jhat;                   /**< Joystick hat event data */
    SDL_JoyButtonEvent jbutton;             /**< Joystick button event data */
    SDL_JoyDeviceEvent jdevice;             /**< Joystick device change event data */
    SDL_JoyBatteryEvent jbattery;           /**< Joystick battery event data */
    SDL_ControllerAxisEvent caxis;          /**< Game Controller axis event data */
    SDL_ControllerButtonEvent cbutton;      /**< Game Controller button event data */
    SDL_ControllerDeviceEvent cdevice;      /**< Game Controller device event data */
    SDL_ControllerTouchpadEvent ctouchpad;  /**< Game Controller touchpad event data */
    SDL_ControllerSensorEvent csensor;      /**< Game Controller sensor event data */
    SDL_AudioDeviceEvent adevice;           /**< Audio device event data */
    SDL_SensorEvent sensor;                 /**< Sensor event data */
    SDL_QuitEvent quit;                     /**< Quit request event data */
    SDL_UserEvent user;                     /**< Custom event data */
    SDL_SysWMEvent syswm;                   /**< System dependent window event data */
    SDL_TouchFingerEvent tfinger;           /**< Touch finger event data */
    SDL_MultiGestureEvent mgesture;         /**< Gesture event data */
    SDL_DollarGestureEvent dgesture;        /**< Gesture event data */
    SDL_DropEvent drop;                     /**< Drag and drop event data */

    /* This is necessary for ABI compatibility between Visual C++ and GCC.
       Visual C++ will respect the push pack pragma and use 52 bytes (size of
       SDL_TextEditingEvent, the largest structure for 32-bit and 64-bit
       architectures) for this union, and GCC will use the alignment of the
       largest datatype within the union, which is 8 bytes on 64-bit
       architectures.

       So... we'll add padding to force the size to be 56 bytes for both.

       On architectures where pointers are 16 bytes, this needs rounding up to
       the next multiple of 16, 64, and on architectures where pointers are
       even larger the size of SDL_UserEvent will dominate as being 3 pointers.
    */
    Uint8 padding[sizeof(void *) <= 8 ? 56 : sizeof(void *) == 16 ? 64 : 3 * sizeof(void *)];
} SDL_Event;
```
### Основные моменты структуры `SDL_Event`:

1. **Типы событий**: структура `SDL_Event` содержит поле `type`, которое указывает на тип события. Возможные типы событий включают:
    
    - `SDL_QUIT`: событие закрытия окна.
    - `SDL_KEYDOWN` и `SDL_KEYUP`: события нажатия и отпускания клавиш.
    - `SDL_MOUSEMOTION`: событие движения мыши.
    - `SDL_MOUSEBUTTONDOWN` и `SDL_MOUSEBUTTONUP`: события нажатия и отпускания кнопок мыши.
    - `SDL_WINDOWEVENT`: события, связанные с окном, такие как изменение размера или перемещение.
2. **Поля структуры**: в зависимости от типа события, структура `SDL_Event` может содержать различные поля, которые предоставляют дополнительную информацию о событии. Например, для событий нажатия клавиш (`SDL_KEYDOWN` и `SDL_KEYUP`) структура содержит поля, указывающие на нажатую клавишу и модификаторы (например, Shift, Ctrl, Alt).
    
3. **Обработка событий**: в приложениях SDL обычно используется цикл обработки событий, в котором функция `SDL_PollEvent` или `SDL_WaitEvent` используется для получения событий из очереди событий. Полученное событие затем обрабатывается в зависимости от его типа.
    
### Пример использования `SDL_Event`:

```
#include <SDL2/SDL.h>

int main(int argc, char *argv[]) {
    SDL_Event event;

    // Инициализация SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    // Создание окна
    SDL_Window *window = SDL_CreateWindow("SDL Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    if (!window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Основной цикл программы
    int running = 1;
    while (running) {
        // Проверка событий
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = 0;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        running = 0;
                    }
                    break;
                // Обработка других событий
                default:
                    break;
            }
        }

        // Обновление экрана и другие действия
        // ...
    }

    // Освобождение ресурсов и завершение SDL
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
```

В этом примере:

- Инициализируется SDL и создаётся окно.
- В основном цикле программы используется `SDL_PollEvent` для проверки событий.
- При получении события `SDL_QUIT` или нажатии клавиши Escape программа завершает работу.

### Relationships between event types and union members[![](https://wiki.libsdl.org/static_files/link.svg)](https://wiki.libsdl.org/SDL2/SDL_Event#relationships-between-event-types-and-union-members)

|Event Type|Event Structure|SDL_Event Field|
|---|---|---|
|SDL_AUDIODEVICEADDED, SDL_AUDIODEVICEREMOVED|[SDL_AudioDeviceEvent](https://wiki.libsdl.org/SDL2/SDL_AudioDeviceEvent)|`adevice`|
|SDL_CONTROLLERAXISMOTION|[SDL_ControllerAxisEvent](https://wiki.libsdl.org/SDL2/SDL_ControllerAxisEvent)|`caxis`|
|SDL_CONTROLLERBUTTONDOWN, SDL_CONTROLLERBUTTONUP|[SDL_ControllerButtonEvent](https://wiki.libsdl.org/SDL2/SDL_ControllerButtonEvent)|`cbutton`|
|SDL_CONTROLLERDEVICEADDED, SDL_CONTROLLERDEVICEREMOVED, SDL_CONTROLLERDEVICEREMAPPED|[SDL_ControllerDeviceEvent](https://wiki.libsdl.org/SDL2/SDL_ControllerDeviceEvent)|`cdevice`|
|SDL_DOLLARGESTURE, SDL_DOLLARRECORD|[SDL_DollarGestureEvent](https://wiki.libsdl.org/SDL2/SDL_DollarGestureEvent)|`dgesture`|
|SDL_DROPFILE, SDL_DROPTEXT, SDL_DROPBEGIN, SDL_DROPCOMPLETE|[SDL_DropEvent](https://wiki.libsdl.org/SDL2/SDL_DropEvent)|`drop`|
|SDL_FINGERMOTION, SDL_FINGERDOWN, SDL_FINGERUP|[SDL_TouchFingerEvent](https://wiki.libsdl.org/SDL2/SDL_TouchFingerEvent)|`tfinger`|
|SDL_KEYDOWN, SDL_KEYUP|[SDL_KeyboardEvent](https://wiki.libsdl.org/SDL2/SDL_KeyboardEvent)|`key`|
|SDL_JOYAXISMOTION|[SDL_JoyAxisEvent](https://wiki.libsdl.org/SDL2/SDL_JoyAxisEvent)|`jaxis`|
|SDL_JOYBALLMOTION|[SDL_JoyBallEvent](https://wiki.libsdl.org/SDL2/SDL_JoyBallEvent)|`jball`|
|SDL_JOYHATMOTION|[SDL_JoyHatEvent](https://wiki.libsdl.org/SDL2/SDL_JoyHatEvent)|`jhat`|
|SDL_JOYBUTTONDOWN, SDL_JOYBUTTONUP|[SDL_JoyButtonEvent](https://wiki.libsdl.org/SDL2/SDL_JoyButtonEvent)|`jbutton`|
|SDL_JOYDEVICEADDED, SDL_JOYDEVICEREMOVED|[SDL_JoyDeviceEvent](https://wiki.libsdl.org/SDL2/SDL_JoyDeviceEvent)|`jdevice`|
|SDL_MOUSEMOTION|[SDL_MouseMotionEvent](https://wiki.libsdl.org/SDL2/SDL_MouseMotionEvent)|`motion`|
|SDL_MOUSEBUTTONDOWN, SDL_MOUSEBUTTONUP|[SDL_MouseButtonEvent](https://wiki.libsdl.org/SDL2/SDL_MouseButtonEvent)|`button`|
|SDL_MOUSEWHEEL|[SDL_MouseWheelEvent](https://wiki.libsdl.org/SDL2/SDL_MouseWheelEvent)|`wheel`|
|SDL_MULTIGESTURE|[SDL_MultiGestureEvent](https://wiki.libsdl.org/SDL2/SDL_MultiGestureEvent)|`mgesture`|
|SDL_QUIT|[SDL_QuitEvent](https://wiki.libsdl.org/SDL2/SDL_QuitEvent)|`quit`|
|SDL_SYSWMEVENT|[SDL_SysWMEvent](https://wiki.libsdl.org/SDL2/SDL_SysWMEvent)|`syswm`|
|SDL_TEXTEDITING|[SDL_TextEditingEvent](https://wiki.libsdl.org/SDL2/SDL_TextEditingEvent)|`edit`|
|SDL_TEXTEDITING_EXT|[SDL_TextEditingExtEvent](https://wiki.libsdl.org/SDL2/SDL_TextEditingExtEvent)|`editExt`|
|SDL_TEXTINPUT|[SDL_TextInputEvent](https://wiki.libsdl.org/SDL2/SDL_TextInputEvent)|`text`|
|SDL_USEREVENT|[SDL_UserEvent](https://wiki.libsdl.org/SDL2/SDL_UserEvent)|`user`|
|SDL_WINDOWEVENT|[SDL_WindowEvent](https://wiki.libsdl.org/SDL2/SDL_WindowEvent)|`window`|
|Other events|[SDL_CommonEvent](https://wiki.libsdl.org/SDL2/SDL_CommonEvent)|`common`|
## SDL_KeyboardEvent
**SDL_KeyboardEvent** — это структура в библиотеке SDL (Simple DirectMedia Layer), которая содержит информацию о событиях клавиатуры. Она используется для обработки событий, связанных с нажатием и отпусканием клавиш на клавиатуре.

Структура SDL_KeyboardEvent включает следующие поля:
```
typedef struct SDL_KeyboardEvent
{
    Uint32 type;        /**< SDL_KEYDOWN or SDL_KEYUP */
    Uint32 timestamp;   /**< In milliseconds, populated using SDL_GetTicks() */
    Uint32 windowID;    /**< The window with keyboard focus, if any */
    Uint8 state;        /**< SDL_PRESSED or SDL_RELEASED */
    Uint8 repeat;       /**< Non-zero if this is a key repeat */
    Uint8 padding2;
    Uint8 padding3;
    SDL_Keysym keysym;  /**< The key that was pressed or released */
} SDL_KeyboardEvent;
```
- **type**: тип события (например, SDL_KEYDOWN или SDL_KEYUP).
- **timestamp**: время, когда произошло событие.
- **windowID**: идентификатор окна, в котором произошло событие.
- **state**: состояние клавиши (нажата или отпущена).
- **keysym**: структура, содержащая информацию о нажатой клавише, включая её скан-код, символ и модификаторы.

Для работы с событиями клавиатуры в SDL необходимо обработать события в главном цикле программы, проверяя тип события и реагируя на него соответствующим образом. Например, можно отслеживать нажатия определённых клавиш и выполнять соответствующие действия в ответ на них.
## SDL_Keysym
**SDL_Keysym** — это структура в библиотеке SDL (Simple DirectMedia Layer), которая содержит информацию о конкретной клавише на клавиатуре. Она используется для представления нажатой клавиши и её атрибутов.

Структура SDL_Keysym включает следующие поля:
```
typedef struct SDL_Keysym
{
    SDL_Scancode scancode;      /**< SDL physical key code - see SDL_Scancode for details */
    SDL_Keycode sym;            /**< SDL virtual key code - see SDL_Keycode for details */
    Uint16 mod;                 /**< current key modifiers */
    Uint32 unused;
} SDL_Keysym;
```
- **scancode**: уникальный код, который идентифицирует физическую клавишу на клавиатуре. Этот код не зависит от раскладки клавиатуры или состояния модификаторов.
- **sym**: символ, соответствующий нажатой клавише. Это может быть ASCII-символ или один из специальных символов SDL (например, SDLK_LEFT для стрелки влево).
- **mod**: битовая маска, которая указывает на состояние модификаторов (Ctrl, Shift, Alt и т. д.) во время нажатия клавиши.
- **unused**: зарезервированное поле, которое в настоящее время не используется.

SDL_Keysym используется в структуре SDL_KeyboardEvent для предоставления подробной информации о нажатой клавише. Это позволяет разработчикам точно определять, какая клавиша была нажата, и реагировать на неё соответствующим образом в своих приложениях.
## SDL_Scancode
**SDL_Scancode** — это уникальный код, который идентифицирует конкретную физическую клавишу на клавиатуре в библиотеке SDL (Simple DirectMedia Layer). Этот код не зависит от раскладки клавиатуры или состояния модификаторов, таких как Ctrl, Shift или Alt.

SDL_Scancode используется для определения, какая клавиша была нажата, независимо от её текущей функции или символа, связанного с ней. Это особенно полезно при разработке игр и приложений, где необходимо точно отслеживать нажатия клавиш, независимо от языковых настроек или изменений в раскладке клавиатуры.

В структуре SDL_Keysym SDL_Scancode представлено полем scancode, которое содержит уникальный код клавиши. Это позволяет разработчикам точно идентифицировать нажатую клавишу и обрабатывать её соответствующим образом в своём приложении.

### Синтаксис
```
typedef enum SDL_Scancode
{
    SDL_SCANCODE_UNKNOWN = 0,

    /**
     *  \name Usage page 0x07
     *
     *  These values are from usage page 0x07 (USB keyboard page).
     */
    /* @{ */

    SDL_SCANCODE_A = 4,
    SDL_SCANCODE_B = 5,
    SDL_SCANCODE_C = 6,
    SDL_SCANCODE_D = 7,
    SDL_SCANCODE_E = 8,
    SDL_SCANCODE_F = 9,
    SDL_SCANCODE_G = 10,
    SDL_SCANCODE_H = 11,
    SDL_SCANCODE_I = 12,
    SDL_SCANCODE_J = 13,
    SDL_SCANCODE_K = 14,
    SDL_SCANCODE_L = 15,
    SDL_SCANCODE_M = 16,
    SDL_SCANCODE_N = 17,
    SDL_SCANCODE_O = 18,
    SDL_SCANCODE_P = 19,
    SDL_SCANCODE_Q = 20,
    SDL_SCANCODE_R = 21,
    SDL_SCANCODE_S = 22,
    SDL_SCANCODE_T = 23,
    SDL_SCANCODE_U = 24,
    SDL_SCANCODE_V = 25,
    SDL_SCANCODE_W = 26,
    SDL_SCANCODE_X = 27,
    SDL_SCANCODE_Y = 28,
    SDL_SCANCODE_Z = 29,

    SDL_SCANCODE_1 = 30,
    SDL_SCANCODE_2 = 31,
    SDL_SCANCODE_3 = 32,
    SDL_SCANCODE_4 = 33,
    SDL_SCANCODE_5 = 34,
    SDL_SCANCODE_6 = 35,
    SDL_SCANCODE_7 = 36,
    SDL_SCANCODE_8 = 37,
    SDL_SCANCODE_9 = 38,
    SDL_SCANCODE_0 = 39,

    SDL_SCANCODE_RETURN = 40,
    SDL_SCANCODE_ESCAPE = 41,
    SDL_SCANCODE_BACKSPACE = 42,
    SDL_SCANCODE_TAB = 43,
    SDL_SCANCODE_SPACE = 44,

    SDL_SCANCODE_MINUS = 45,
    SDL_SCANCODE_EQUALS = 46,
    SDL_SCANCODE_LEFTBRACKET = 47,
    SDL_SCANCODE_RIGHTBRACKET = 48,
    SDL_SCANCODE_BACKSLASH = 49, /**< Located at the lower left of the return
                                  *   key on ISO keyboards and at the right end
                                  *   of the QWERTY row on ANSI keyboards.
                                  *   Produces REVERSE SOLIDUS (backslash) and
                                  *   VERTICAL LINE in a US layout, REVERSE
                                  *   SOLIDUS and VERTICAL LINE in a UK Mac
                                  *   layout, NUMBER SIGN and TILDE in a UK
                                  *   Windows layout, DOLLAR SIGN and POUND SIGN
                                  *   in a Swiss German layout, NUMBER SIGN and
                                  *   APOSTROPHE in a German layout, GRAVE
                                  *   ACCENT and POUND SIGN in a French Mac
                                  *   layout, and ASTERISK and MICRO SIGN in a
                                  *   French Windows layout.
                                  */
    SDL_SCANCODE_NONUSHASH = 50, /**< ISO USB keyboards actually use this code
                                  *   instead of 49 for the same key, but all
                                  *   OSes I've seen treat the two codes
                                  *   identically. So, as an implementor, unless
                                  *   your keyboard generates both of those
                                  *   codes and your OS treats them differently,
                                  *   you should generate SDL_SCANCODE_BACKSLASH
                                  *   instead of this code. As a user, you
                                  *   should not rely on this code because SDL
                                  *   will never generate it with most (all?)
                                  *   keyboards.
                                  */
    SDL_SCANCODE_SEMICOLON = 51,
    SDL_SCANCODE_APOSTROPHE = 52,
    SDL_SCANCODE_GRAVE = 53, /**< Located in the top left corner (on both ANSI
                              *   and ISO keyboards). Produces GRAVE ACCENT and
                              *   TILDE in a US Windows layout and in US and UK
                              *   Mac layouts on ANSI keyboards, GRAVE ACCENT
                              *   and NOT SIGN in a UK Windows layout, SECTION
                              *   SIGN and PLUS-MINUS SIGN in US and UK Mac
                              *   layouts on ISO keyboards, SECTION SIGN and
                              *   DEGREE SIGN in a Swiss German layout (Mac:
                              *   only on ISO keyboards), CIRCUMFLEX ACCENT and
                              *   DEGREE SIGN in a German layout (Mac: only on
                              *   ISO keyboards), SUPERSCRIPT TWO and TILDE in a
                              *   French Windows layout, COMMERCIAL AT and
                              *   NUMBER SIGN in a French Mac layout on ISO
                              *   keyboards, and LESS-THAN SIGN and GREATER-THAN
                              *   SIGN in a Swiss German, German, or French Mac
                              *   layout on ANSI keyboards.
                              */
    SDL_SCANCODE_COMMA = 54,
    SDL_SCANCODE_PERIOD = 55,
    SDL_SCANCODE_SLASH = 56,

    SDL_SCANCODE_CAPSLOCK = 57,

    SDL_SCANCODE_F1 = 58,
    SDL_SCANCODE_F2 = 59,
    SDL_SCANCODE_F3 = 60,
    SDL_SCANCODE_F4 = 61,
    SDL_SCANCODE_F5 = 62,
    SDL_SCANCODE_F6 = 63,
    SDL_SCANCODE_F7 = 64,
    SDL_SCANCODE_F8 = 65,
    SDL_SCANCODE_F9 = 66,
    SDL_SCANCODE_F10 = 67,
    SDL_SCANCODE_F11 = 68,
    SDL_SCANCODE_F12 = 69,

    SDL_SCANCODE_PRINTSCREEN = 70,
    SDL_SCANCODE_SCROLLLOCK = 71,
    SDL_SCANCODE_PAUSE = 72,
    SDL_SCANCODE_INSERT = 73, /**< insert on PC, help on some Mac keyboards (but
                                   does send code 73, not 117) */
    SDL_SCANCODE_HOME = 74,
    SDL_SCANCODE_PAGEUP = 75,
    SDL_SCANCODE_DELETE = 76,
    SDL_SCANCODE_END = 77,
    SDL_SCANCODE_PAGEDOWN = 78,
    SDL_SCANCODE_RIGHT = 79,
    SDL_SCANCODE_LEFT = 80,
    SDL_SCANCODE_DOWN = 81,
    SDL_SCANCODE_UP = 82,

    SDL_SCANCODE_NUMLOCKCLEAR = 83, /**< num lock on PC, clear on Mac keyboards
                                     */
    SDL_SCANCODE_KP_DIVIDE = 84,
    SDL_SCANCODE_KP_MULTIPLY = 85,
    SDL_SCANCODE_KP_MINUS = 86,
    SDL_SCANCODE_KP_PLUS = 87,
    SDL_SCANCODE_KP_ENTER = 88,
    SDL_SCANCODE_KP_1 = 89,
    SDL_SCANCODE_KP_2 = 90,
    SDL_SCANCODE_KP_3 = 91,
    SDL_SCANCODE_KP_4 = 92,
    SDL_SCANCODE_KP_5 = 93,
    SDL_SCANCODE_KP_6 = 94,
    SDL_SCANCODE_KP_7 = 95,
    SDL_SCANCODE_KP_8 = 96,
    SDL_SCANCODE_KP_9 = 97,
    SDL_SCANCODE_KP_0 = 98,
    SDL_SCANCODE_KP_PERIOD = 99,

    SDL_SCANCODE_NONUSBACKSLASH = 100, /**< This is the additional key that ISO
                                        *   keyboards have over ANSI ones,
                                        *   located between left shift and Y.
                                        *   Produces GRAVE ACCENT and TILDE in a
                                        *   US or UK Mac layout, REVERSE SOLIDUS
                                        *   (backslash) and VERTICAL LINE in a
                                        *   US or UK Windows layout, and
                                        *   LESS-THAN SIGN and GREATER-THAN SIGN
                                        *   in a Swiss German, German, or French
                                        *   layout. */
    SDL_SCANCODE_APPLICATION = 101, /**< windows contextual menu, compose */
    SDL_SCANCODE_POWER = 102, /**< The USB document says this is a status flag,
                               *   not a physical key - but some Mac keyboards
                               *   do have a power key. */
    SDL_SCANCODE_KP_EQUALS = 103,
    SDL_SCANCODE_F13 = 104,
    SDL_SCANCODE_F14 = 105,
    SDL_SCANCODE_F15 = 106,
    SDL_SCANCODE_F16 = 107,
    SDL_SCANCODE_F17 = 108,
    SDL_SCANCODE_F18 = 109,
    SDL_SCANCODE_F19 = 110,
    SDL_SCANCODE_F20 = 111,
    SDL_SCANCODE_F21 = 112,
    SDL_SCANCODE_F22 = 113,
    SDL_SCANCODE_F23 = 114,
    SDL_SCANCODE_F24 = 115,
    SDL_SCANCODE_EXECUTE = 116,
    SDL_SCANCODE_HELP = 117,    /**< AL Integrated Help Center */
    SDL_SCANCODE_MENU = 118,    /**< Menu (show menu) */
    SDL_SCANCODE_SELECT = 119,
    SDL_SCANCODE_STOP = 120,    /**< AC Stop */
    SDL_SCANCODE_AGAIN = 121,   /**< AC Redo/Repeat */
    SDL_SCANCODE_UNDO = 122,    /**< AC Undo */
    SDL_SCANCODE_CUT = 123,     /**< AC Cut */
    SDL_SCANCODE_COPY = 124,    /**< AC Copy */
    SDL_SCANCODE_PASTE = 125,   /**< AC Paste */
    SDL_SCANCODE_FIND = 126,    /**< AC Find */
    SDL_SCANCODE_MUTE = 127,
    SDL_SCANCODE_VOLUMEUP = 128,
    SDL_SCANCODE_VOLUMEDOWN = 129,
/* not sure whether there's a reason to enable these */
/*     SDL_SCANCODE_LOCKINGCAPSLOCK = 130,  */
/*     SDL_SCANCODE_LOCKINGNUMLOCK = 131, */
/*     SDL_SCANCODE_LOCKINGSCROLLLOCK = 132, */
    SDL_SCANCODE_KP_COMMA = 133,
    SDL_SCANCODE_KP_EQUALSAS400 = 134,

    SDL_SCANCODE_INTERNATIONAL1 = 135, /**< used on Asian keyboards, see
                                            footnotes in USB doc */
    SDL_SCANCODE_INTERNATIONAL2 = 136,
    SDL_SCANCODE_INTERNATIONAL3 = 137, /**< Yen */
    SDL_SCANCODE_INTERNATIONAL4 = 138,
    SDL_SCANCODE_INTERNATIONAL5 = 139,
    SDL_SCANCODE_INTERNATIONAL6 = 140,
    SDL_SCANCODE_INTERNATIONAL7 = 141,
    SDL_SCANCODE_INTERNATIONAL8 = 142,
    SDL_SCANCODE_INTERNATIONAL9 = 143,
    SDL_SCANCODE_LANG1 = 144, /**< Hangul/English toggle */
    SDL_SCANCODE_LANG2 = 145, /**< Hanja conversion */
    SDL_SCANCODE_LANG3 = 146, /**< Katakana */
    SDL_SCANCODE_LANG4 = 147, /**< Hiragana */
    SDL_SCANCODE_LANG5 = 148, /**< Zenkaku/Hankaku */
    SDL_SCANCODE_LANG6 = 149, /**< reserved */
    SDL_SCANCODE_LANG7 = 150, /**< reserved */
    SDL_SCANCODE_LANG8 = 151, /**< reserved */
    SDL_SCANCODE_LANG9 = 152, /**< reserved */

    SDL_SCANCODE_ALTERASE = 153,    /**< Erase-Eaze */
    SDL_SCANCODE_SYSREQ = 154,
    SDL_SCANCODE_CANCEL = 155,      /**< AC Cancel */
    SDL_SCANCODE_CLEAR = 156,
    SDL_SCANCODE_PRIOR = 157,
    SDL_SCANCODE_RETURN2 = 158,
    SDL_SCANCODE_SEPARATOR = 159,
    SDL_SCANCODE_OUT = 160,
    SDL_SCANCODE_OPER = 161,
    SDL_SCANCODE_CLEARAGAIN = 162,
    SDL_SCANCODE_CRSEL = 163,
    SDL_SCANCODE_EXSEL = 164,

    SDL_SCANCODE_KP_00 = 176,
    SDL_SCANCODE_KP_000 = 177,
    SDL_SCANCODE_THOUSANDSSEPARATOR = 178,
    SDL_SCANCODE_DECIMALSEPARATOR = 179,
    SDL_SCANCODE_CURRENCYUNIT = 180,
    SDL_SCANCODE_CURRENCYSUBUNIT = 181,
    SDL_SCANCODE_KP_LEFTPAREN = 182,
    SDL_SCANCODE_KP_RIGHTPAREN = 183,
    SDL_SCANCODE_KP_LEFTBRACE = 184,
    SDL_SCANCODE_KP_RIGHTBRACE = 185,
    SDL_SCANCODE_KP_TAB = 186,
    SDL_SCANCODE_KP_BACKSPACE = 187,
    SDL_SCANCODE_KP_A = 188,
    SDL_SCANCODE_KP_B = 189,
    SDL_SCANCODE_KP_C = 190,
    SDL_SCANCODE_KP_D = 191,
    SDL_SCANCODE_KP_E = 192,
    SDL_SCANCODE_KP_F = 193,
    SDL_SCANCODE_KP_XOR = 194,
    SDL_SCANCODE_KP_POWER = 195,
    SDL_SCANCODE_KP_PERCENT = 196,
    SDL_SCANCODE_KP_LESS = 197,
    SDL_SCANCODE_KP_GREATER = 198,
    SDL_SCANCODE_KP_AMPERSAND = 199,
    SDL_SCANCODE_KP_DBLAMPERSAND = 200,
    SDL_SCANCODE_KP_VERTICALBAR = 201,
    SDL_SCANCODE_KP_DBLVERTICALBAR = 202,
    SDL_SCANCODE_KP_COLON = 203,
    SDL_SCANCODE_KP_HASH = 204,
    SDL_SCANCODE_KP_SPACE = 205,
    SDL_SCANCODE_KP_AT = 206,
    SDL_SCANCODE_KP_EXCLAM = 207,
    SDL_SCANCODE_KP_MEMSTORE = 208,
    SDL_SCANCODE_KP_MEMRECALL = 209,
    SDL_SCANCODE_KP_MEMCLEAR = 210,
    SDL_SCANCODE_KP_MEMADD = 211,
    SDL_SCANCODE_KP_MEMSUBTRACT = 212,
    SDL_SCANCODE_KP_MEMMULTIPLY = 213,
    SDL_SCANCODE_KP_MEMDIVIDE = 214,
    SDL_SCANCODE_KP_PLUSMINUS = 215,
    SDL_SCANCODE_KP_CLEAR = 216,
    SDL_SCANCODE_KP_CLEARENTRY = 217,
    SDL_SCANCODE_KP_BINARY = 218,
    SDL_SCANCODE_KP_OCTAL = 219,
    SDL_SCANCODE_KP_DECIMAL = 220,
    SDL_SCANCODE_KP_HEXADECIMAL = 221,

    SDL_SCANCODE_LCTRL = 224,
    SDL_SCANCODE_LSHIFT = 225,
    SDL_SCANCODE_LALT = 226, /**< alt, option */
    SDL_SCANCODE_LGUI = 227, /**< windows, command (apple), meta */
    SDL_SCANCODE_RCTRL = 228,
    SDL_SCANCODE_RSHIFT = 229,
    SDL_SCANCODE_RALT = 230, /**< alt gr, option */
    SDL_SCANCODE_RGUI = 231, /**< windows, command (apple), meta */

    SDL_SCANCODE_MODE = 257,    /**< I'm not sure if this is really not covered
                                 *   by any of the above, but since there's a
                                 *   special KMOD_MODE for it I'm adding it here
                                 */

    /* @} *//* Usage page 0x07 */

    /**
     *  \name Usage page 0x0C
     *
     *  These values are mapped from usage page 0x0C (USB consumer page).
     *  See https://usb.org/sites/default/files/hut1_2.pdf
     *
     *  There are way more keys in the spec than we can represent in the
     *  current scancode range, so pick the ones that commonly come up in
     *  real world usage.
     */
    /* @{ */

    SDL_SCANCODE_AUDIONEXT = 258,
    SDL_SCANCODE_AUDIOPREV = 259,
    SDL_SCANCODE_AUDIOSTOP = 260,
    SDL_SCANCODE_AUDIOPLAY = 261,
    SDL_SCANCODE_AUDIOMUTE = 262,
    SDL_SCANCODE_MEDIASELECT = 263,
    SDL_SCANCODE_WWW = 264,             /**< AL Internet Browser */
    SDL_SCANCODE_MAIL = 265,
    SDL_SCANCODE_CALCULATOR = 266,      /**< AL Calculator */
    SDL_SCANCODE_COMPUTER = 267,
    SDL_SCANCODE_AC_SEARCH = 268,       /**< AC Search */
    SDL_SCANCODE_AC_HOME = 269,         /**< AC Home */
    SDL_SCANCODE_AC_BACK = 270,         /**< AC Back */
    SDL_SCANCODE_AC_FORWARD = 271,      /**< AC Forward */
    SDL_SCANCODE_AC_STOP = 272,         /**< AC Stop */
    SDL_SCANCODE_AC_REFRESH = 273,      /**< AC Refresh */
    SDL_SCANCODE_AC_BOOKMARKS = 274,    /**< AC Bookmarks */

    /* @} *//* Usage page 0x0C */

    /**
     *  \name Walther keys
     *
     *  These are values that Christian Walther added (for mac keyboard?).
     */
    /* @{ */

    SDL_SCANCODE_BRIGHTNESSDOWN = 275,
    SDL_SCANCODE_BRIGHTNESSUP = 276,
    SDL_SCANCODE_DISPLAYSWITCH = 277, /**< display mirroring/dual display
                                           switch, video mode switch */
    SDL_SCANCODE_KBDILLUMTOGGLE = 278,
    SDL_SCANCODE_KBDILLUMDOWN = 279,
    SDL_SCANCODE_KBDILLUMUP = 280,
    SDL_SCANCODE_EJECT = 281,
    SDL_SCANCODE_SLEEP = 282,           /**< SC System Sleep */

    SDL_SCANCODE_APP1 = 283,
    SDL_SCANCODE_APP2 = 284,

    /* @} *//* Walther keys */

    /**
     *  \name Usage page 0x0C (additional media keys)
     *
     *  These values are mapped from usage page 0x0C (USB consumer page).
     */
    /* @{ */

    SDL_SCANCODE_AUDIOREWIND = 285,
    SDL_SCANCODE_AUDIOFASTFORWARD = 286,

    /* @} *//* Usage page 0x0C (additional media keys) */

    /**
     *  \name Mobile keys
     *
     *  These are values that are often used on mobile phones.
     */
    /* @{ */

    SDL_SCANCODE_SOFTLEFT = 287, /**< Usually situated below the display on phones and
                                      used as a multi-function feature key for selecting
                                      a software defined function shown on the bottom left
                                      of the display. */
    SDL_SCANCODE_SOFTRIGHT = 288, /**< Usually situated below the display on phones and
                                       used as a multi-function feature key for selecting
                                       a software defined function shown on the bottom right
                                       of the display. */
    SDL_SCANCODE_CALL = 289, /**< Used for accepting phone calls. */
    SDL_SCANCODE_ENDCALL = 290, /**< Used for rejecting phone calls. */

    /* @} *//* Mobile keys */

    /* Add any other keys here. */

    SDL_NUM_SCANCODES = 512 /**< not a key, just marks the number of scancodes
                                 for array bounds */
} SDL_Scancode;
```
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