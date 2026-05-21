#include "common.h"

#include <cstdio>
#include <iostream>

#include "globalVar.h"

using namespace std;


void enterAltScr() { printf("\033[?1049h"); }

void exitAltScr() { printf("\033[?1049l"); }

void clearScr() { printf("\033[2J\033[H"); }

void clearLine(int row, int col, int len) {
    printf("\033[%d;%dH", row, col);
    for (int i = 0; i < len; i++) printf(" ");
}

void printAt(size_t col, size_t row, const std::string &s, bool border, size_t bg_color, size_t txt_color) {
    if (bg_color) printf("\033[%lum", bg_color);
    if (txt_color) printf("\033[%lum", txt_color);
    printf("\033[%lu;%luH%s", row, col, s.c_str());

	if (border) {
		int len = s.length();
        printf("\033[%lu;%luH%s", row - 1, col - 1, downRight);
        for (int i = 0; i < len; i++) cout<<horiLine;
        cout<<downLeft;
        printf("\033[%lu;%luH%s", row, col - 1, vertiLine);
        printf("\033[%lu;%luH%s", row, col + len, vertiLine);
        printf("\033[%lu;%luH%s", row + 1, col - 1, upRight);
        for (int i = 0; i < len; i++) cout<<horiLine;
        cout<<upLeft;
    }

	printf("\033[0m");
}

void refreshBuffer() { fflush(stdout); }

void hideCursor() { printf("\033[?25l"); }

void showCursor() { printf("\033[?25h"); }

void print_instruction() {
    if (term_width > 70)
        printAt(term_width/2 - 34, 1, "Controls: 'p' or space = Play/Pause, 'q' = Exit, Arrow = next / prev"); // len=68
    else if (term_width > 46)
        printAt(term_width/2 - 22, 1, "p/space: play/pause q: exit Arrow: next/prev"); // len=44
}