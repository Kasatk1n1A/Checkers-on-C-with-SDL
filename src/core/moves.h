#ifndef MOVES_H
#define MOVES_H

void showCaptureOptions(bool** board);
bool** canCapture(CH_Type** board, bool isWhiteTurn);
bool CanContinue(CH_Type** board, int x, int y, Player player);
bool CanCheckerAttack(CH_Type** board, int x, int y, bool isWhite);
void executeCaptureMove(Window* window, CH_Type** board, bool** attack_board, Player player, Board* CheckersBoard);

bool isCheckerBlocked(CH_Type** board, int x, int y);
void executeRegularMove(Window* window, CH_Type** board, Player player, Board* CheckersBoard);
bool canCheckerMove(int x1, int y1, int x2, int y2, bool isKing, Player color, CH_Type** board);

bool CheckerCoordinates(Window* window, int* x, int* y);
void performCapture(CH_Type** board, int fromX, int fromY, int toX, int toY);

#endif