#ifndef COMMON_H
#define COMMON_H

#include <string>

void enterAltScr();
void exitAltScr();
void clearScr();
void clearLine(int row, int col, int len);
void printAt(size_t col, size_t row, const std::string &s, bool border = false, size_t bg_color = 0, size_t txt_color = 0);
void refreshBuffer();
void hideCursor();
void showCursor();
void print_instruction();

#endif //COMMON_H