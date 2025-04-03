// #ifndef PLAYERACTIONS
// #define PLAYERACTIONS


// #include "visualise.h"
// #include "checkers and board/StructsAndEnum.h"


// // Нужна для того чтобы регистрировать координаты нажатия игрока во время игры в шашки, конца игры и 
// Choice get_mouse_click(int* x, int* y, Game* game, Board* board)
// {
//     SDL_Event event;

//     while (true)
//     {
//         // Проверяем события в очереди
//         while (SDL_PollEvent(&event)) 
//         {
//             switch (event.type)
//             {
//             case SDL_QUIT:  // Обработка закрытия окна
//                 app_cleanup(game, EXIT_SUCCESS);
//                 break;  

//             case SDL_KEYDOWN:  // Обработка нажатий клавиш
//                 switch (event.key.keysym.scancode)
//                 {
//                 case SDL_SCANCODE_ESCAPE:  // Закрытие по ESC
//                     app_cleanup(game, EXIT_SUCCESS);
//                     break;
//                 case SDL_SCANCODE_RETURN:
//                     return ENTER;
//                 default:
//                     break;
//                 }
//                 break;

//             case SDL_MOUSEBUTTONDOWN:
//                 if (event.button.button == SDL_BUTTON_LEFT) // ЛКМ
//                 {            
//                     *x = event.button.x;
//                     *y = event.button.y;
//                 }
//                 // Выходим после первого обнаруженного клика
//                 return MOUSE_LEFT;
//                 break;
//             }
//         }
//     }
//     return 0;
// }

// #endif