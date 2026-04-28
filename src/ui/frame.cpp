#include <iostream>

#include "../event/globalVar.h"


void drawBorder() {
    if (term_width < 2 || term_height < 2)
        return;

    printf("\033[%zum\033[%d;%dH%s", theme_color, 1, 1, downRight);
    for (size_t i = 0; i < term_width - 2; i++) std::cout<<horiLine;
    std::cout<<downLeft;
    for (size_t i = 1; i < term_height - 1; i++) {
        printf("\033[%zu;%dH%s", 1 + i, 1, vertiLine);
        printf("\033[%zu;%zuH%s", 1 + i, term_width, vertiLine);
    }
    printf("\033[%zu;%dH%s", term_height, 1, upRight);
    for (size_t i = 0; i < term_width - 2; i++) std::cout<<horiLine;
    std::cout<<upLeft;
    std::cout<<"\033[0m";
}


void drawLeftBox() {
    if (term_width < 3 || term_height < 2 || lb_width + 1 >= term_width)
        return;

    printf("\033[%d;%luH%s", 1, lb_width + 1, horiDown);
    for (size_t i = 2; i < term_height; i++)
        printf("\033[%lu;%luH%s", i, lb_width + 1, vertiLine);
    printf("\033[%lu;%luH%s", term_height, lb_width + 1, horiUp);
}
