#ifndef COMMON_H
#define COMMON_H

#include <string>

void enterAltScr();
void exitAltScr();
void clearScr();
void clearLine(int row, int col, int len);
void printAt(size_t col, size_t row, const std::string &s, size_t color = 0, bool border = false);
void refreshBuffer();
void hideCursor();
void showCursor();

#endif //COMMON_H