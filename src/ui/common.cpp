#include <stdio.h>
#include <string>

void enterAltScr() { printf("\033[?1049h"); }

void exitAltScr() { printf("\033[?1049l"); }

void clearScr() { printf("\033[2J\033[H"); }

void printAt(int col, int row, const std::string &s, int color = 0) {
    printf("\033[%dm\033[%d;%dH%s\033[0m", color, row, col, s.c_str());
}

void refreshBuffer() { fflush(stdout); }

void hideCursor() { printf("\033[?25l"); }

void showCursor() { printf("\033[?25h"); }