#ifndef COMMON_H
#define COMMON_H

#include <string>

void enterAltScr();
void exitAltScr();
void clearScr();
void clearLine(int row, int col, int len);
void printAt(int col, int row, const std::string &s, int color = 0, bool border = false);
void refreshBuffer();
void hideCursor();
void showCursor();

#endif //COMMON_H