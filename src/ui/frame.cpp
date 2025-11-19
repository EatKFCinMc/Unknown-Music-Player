#include <iostream>

#include "../event/globalVar.h"

const char* downRight = "┌";
const char* upRight = "└";
const char* downLeft = "┐";
const char* upLeft = "┘";
const char* horiLine = "─";
const char* vertiLine = "│";
const char* vertiLeft = "┤";
const char* vertiRight = "├";
const char* horiDown = "┬";
const char* horiUp = "┴";
const char* cross = "┼";

void draw_border() {
    printf("\033[%zum\033[%zu;%zuH%s", theme_color, term_height, term_width, downRight);
    for (int i = 0; i < term_width - 2; i++) std::cout<<horiLine;
    std::cout<<downLeft;
    for (int i = 1; i < term_height - 1; i++) {
        printf("\033[%d;%dH%s", 1 + i, 1, vertiLine);
        printf("\033[%d;%zuH%s", 1 + i, term_width, vertiLine);
    }
    printf("\033[%zu;%dH%s", term_height, 1, upRight);
    for (int i = 0; i < term_width - 2; i++) std::cout<<horiLine;
    std::cout<<upLeft;
    std::cout<<"\033[0m";
}