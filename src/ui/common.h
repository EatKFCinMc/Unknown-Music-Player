//
// Created by Empty on 10/19/25.
//

#ifndef UNKNOWNMUSICPLAYER_COMMON_H
#define UNKNOWNMUSICPLAYER_COMMON_H

void enterAltScr();
void exitAltScr();
void clearScr();
void printAt(int col, int row, const std::string &s, int color = 0);
void refreshBuffer();
void hideCursor();
void showCursor();

#endif //UNKNOWNMUSICPLAYER_COMMON_H