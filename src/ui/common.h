#ifndef UNKNOWNMUSICPLAYER_COMMON_H
#define UNKNOWNMUSICPLAYER_COMMON_H

#include <string>

void enterAltScr();
void exitAltScr();
void clearScr();
void clearLine(int row, int col, int len);
void printAt(int col, int row, const std::string &s, int color = 0, bool border = false);
void drawBox(int tall, int wide, int ori_col, int ori_row, int color = 0);
void refreshBuffer();
void hideCursor();
void showCursor();

#endif