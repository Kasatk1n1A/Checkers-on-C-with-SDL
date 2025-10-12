#include "game_rules.h"

static struct NIGGER{
    CH_Type** tmp_board;
    GameInfo* info;
};

static bool thread_finish = false;
static SDL_mutex* mutex;

int _bot_make_move_(void* data){
    struct NIGGER* POOP = (struct NIGGER*)data;
    bot_make_move(POOP->tmp_board, POOP->info->difficult, POOP->info->player);
    SDL_LockMutex(mutex);
    thread_finish = true;
    SDL_UnlockMutex(mutex);
    return 0;
}

int checkers(Window* window, CH_Type** board, int difficult, double Time, Player color)
{
    Board* CheckersBoard = (Board*)malloc(sizeof(Board));
    
    LoadBoardTextures(CheckersBoard, window);

    if (board == NULL)
        CheckersBoard->board = add_board();
    else
        CheckersBoard->board = board;    
    
    Uint32 start, end;
    GameInfo info;
    info.Time = Time;
    info.difficult = difficult;
    info.player = color;

    renderBoardFrame(window, CheckersBoard);
    // основной цикл игры
    while (true)
    {
        out_board(CheckersBoard->board);
        if (info.player == RED)
        {
            // Ход бота
            thread_finish = false;
            mutex = SDL_CreateMutex();

            struct NIGGER POOP;
            POOP.tmp_board = add_board();
            CopyBoard(CheckersBoard->board, POOP.tmp_board);
            POOP.info = &info;

            printf("Red turn.\n");
            SDL_Thread* thread = SDL_CreateThread(_bot_make_move_, "bot turn", (void*)(&POOP));
            SDL_DetachThread(thread);

            bool running = true;
            while (running){
                SDL_LockMutex(mutex);
                if (thread_finish)
                    running = false;
                SDL_UnlockMutex(mutex);
                renderBoardFrame(window, CheckersBoard);
            }

            CopyBoard(POOP.tmp_board, CheckersBoard->board);
            SDL_DestroyMutex(mutex);
            // bot_make_move(tmp_board, info.difficult, RED);
            
        }
        else
        {
            printf("White turn.\n");
            start = SDL_GetTicks();
            printf("start time%lf\n", (double)start);

            //проверка на необходимость атаки
            bool** attack_board = canCapture(CheckersBoard->board, info.player == WHITE ? true : false);
            if (attack_board) {
                executeCaptureMove(window, CheckersBoard->board, attack_board, CheckersBoard, info);
                freeBoard((void**)attack_board);
            }
            else    //Обычный ход
                executeRegularMove(window, CheckersBoard->board, info, CheckersBoard);
    
            end = SDL_GetTicks();
            printf("end time%lf\n", (double)end);
            info.Time += (end - start) / 1000.0;
            printf("White's move time: %.3f sec (Total: %.3f sec)\n", 
                ((double)(end - start)) / CLOCKS_PER_SEC, info.Time);
        }

        renderBoardFrame(window, CheckersBoard);
        
        if (Win_Check(CheckersBoard->board, info.player))
        {
            info.player == WHITE ? WinMenu(window, info) : LoseMenu(window);
            board_cleanup_SDL(CheckersBoard);
            break;
        }

        info.player = info.player == WHITE ? RED : WHITE;
    }

    showMainMenu(window);
    return 0;
}

int bot_vs_bot(Window* window, int bot1_difficulty, int bot2_difficulty)
{
    Board* CheckersBoard = (Board*)malloc(sizeof(Board));
    
    LoadBoardTextures(CheckersBoard, window);

    CheckersBoard->board = add_board();
    
    Uint32 start, end;
    double White_Time = 0, Red_Time = 0;
    Player player = WHITE;
    FILE* file = fopen("Bot vs bot statistic.txt", "a");

    CH_Type** tmp_board = add_board();
    CopyBoard(CheckersBoard->board, tmp_board);
    int moves_without_captures = 0;

    renderBoardFrame(window, CheckersBoard);
    // основной цикл игры
    while (true)
    {
        if (player == RED)
        {
            // Ход бота
            start = SDL_GetTicks();
            bot_make_move(CheckersBoard->board, bot1_difficulty, RED);
            end = SDL_GetTicks();
            Red_Time += (end - start) / 1000.0;
        }
        else
        {
            start = SDL_GetTicks();
            bot_make_move(CheckersBoard->board, bot2_difficulty, WHITE);
            end = SDL_GetTicks();
            White_Time += (end - start) / 1000.0;
        }

        renderBoardFrame(window, CheckersBoard);
        
        if (was_capture(CheckersBoard->board, tmp_board))
            moves_without_captures = 0;
        else
            moves_without_captures++;

        if (moves_without_captures == 50)
        {
            fprintf(file, "RED Diff: %d; White diff: %d\n", bot1_difficulty, bot2_difficulty);
            fprintf(file, "Nobody\n");
            fprintf(file, "Red time: %.3f sec; White Time: %.3f sec\n\n", Red_Time, White_Time);
            break;
        }

        if (Win_Check(CheckersBoard->board, player))
        {
            fprintf(file, "RED Diff: %d; White diff: %d\n", bot1_difficulty, bot2_difficulty);
            fprintf(file, player == WHITE ? "WHITE\n" : "RED\n");
            fprintf(file, "Red time: %.3f sec; White Time: %.3f sec\n\n", Red_Time, White_Time);
            break;
        }
        
        CopyBoard(CheckersBoard->board, tmp_board);
        player = player == WHITE ? RED : WHITE;
    }

    board_cleanup_SDL(CheckersBoard);
    freeBoard((void**)tmp_board);
    fclose(file);
    // board_cleanup_SDL(CheckersBoard);

    return 0;
}

bool was_capture(CH_Type** current_board, CH_Type** previous_board) 
{
    int current_count = 0;
    int previous_count = 0;

    // Подсчитываем количество фигур на текущей доске
    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
            if (current_board[y][x] != EMPTY)
                current_count++;

    // Подсчитываем количество фигур на предыдущей доске
    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
            if (previous_board[y][x] != EMPTY)
                previous_count++;

    // Если количество фигур уменьшилось - было съедение
    return current_count < previous_count;
}

bool Win_Check(CH_Type** board, Player player)
{
    CH_Type enemy_pawn = (player == WHITE) ? RED_PAWN : WHITE_PAWN;
    CH_Type enemy_king = (player == WHITE) ? RED_KING : WHITE_KING;

    bool EnemyHasCheckers = false;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if (board[i][j] == enemy_pawn || board[i][j] == enemy_king)
                EnemyHasCheckers = true;
    
    if (!EnemyHasCheckers)
    {
        printf("Enemy hasn`t checkers\n");
        return true;
    }

    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if (board[i][j] == enemy_pawn || board[i][j] == enemy_king)
            {
                printf("y: %d x: %d\n", i, j);
                if (!isCheckerBlocked(board, j, i))
                    return false;
            }

    printf("All enemy`s checkers blocked\n");
    return true;
}