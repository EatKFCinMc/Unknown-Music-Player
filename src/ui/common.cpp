#include "common.h"

#include <stdio.h>

const char* downRight = "┏";
const char* upRight = "┗";
const char* downLeft = "┓";
const char* upLeft = "┛";
const char* horiLine = "━";
const char* vertiLine = "┃";


void enterAltScr() { printf("\033[?1049h"); }

void exitAltScr() { printf("\033[?1049l"); }

void clearScr() { printf("\033[2J\033[H"); }

void clearLine(int row, int col, int len) {
    printf("\033[%d;%dH", row, col);
    for (int i = 0; i < len; i++) printf(" ");
}

void printAt(int col, int row, const std::string &s, int color, bool border) {
    printf("\033[%dm\033[%d;%dH%s\033[0m", color, row, col, s.c_str());

	if (border) {
		int len = s.length();
        printf("\033[%dm\033[%d;%dH%s", color, row - 1, col - 1, downRight);
        for (int i = 0; i < len; i++) printf(horiLine);
        printf(downLeft);
        printf("\033[%d;%dH%s", row, col - 1, vertiLine);
        printf("\033[%d;%dH%s", row, col + len, vertiLine);
        printf("\033[%d;%dH%s", row + 1, col - 1, upRight);
        for (int i = 0; i < len; i++) printf(horiLine);
        printf(upLeft);
		printf("\033[0m");
    }
}

void drawBox(int tall, int wide, int ori_col, int ori_row, int color){
    printf("\033[%dm\033[%d;%dH%s", color, ori_row, ori_col, downRight);
    for (int i = 0; i < wide - 2; i++) printf(horiLine);
    printf(downLeft);
    for (int i = 1; i < tall - 1; i++) {
        printf("\033[%d;%dH%s", ori_row + i, ori_col, vertiLine);
        printf("\033[%d;%dH%s", ori_row + i, ori_col + wide - 1, vertiLine);
    }
    printf("\033[%d;%dH%s", ori_row + tall - 1, ori_col, upRight);
    for (int i = 0; i < wide - 2; i++) printf(horiLine);
    printf(upLeft);
    printf("\033[0m");
}

void refreshBuffer() { fflush(stdout); }

void hideCursor() { printf("\033[?25l"); }

void showCursor() { printf("\033[?25h"); }