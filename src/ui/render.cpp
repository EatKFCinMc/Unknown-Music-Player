#include <iostream>

#include "common.h"
#include "../cover/showCover.h"
#include "../event/globalVar.h"
#include "../logger/log.h"


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

void drawList() {

}

void frame_render() {
    drawBorder();
    drawLeftBox();
    drawList();
}

void cover_render() {
    displayCover(songPath);
}

void metadata_render() {
    std::string space;
    for (size_t i = 0; i < lb_width_inner; i++)
        space += ' ';
    printAt(3, title_row, space);
    printAt(3, artist_row, space);
    printAt(3, album_row, space);

    std::string temp_title = title.substr(0, title.length() > lb_width_inner ? lb_width_inner : title.length());
    std::string temp_artist = artist.substr(0, artist.length() > lb_width_inner ? lb_width_inner : artist.length());
    std::string temp_album = album.substr(0, album.length() > lb_width_inner ? lb_width_inner : album.length());
    printAt(3, title_row, temp_title);
    printAt(3, artist_row, temp_artist);
    printAt(3, album_row, temp_album);
    logger("Title length: " + std::to_string(title.length()) + "\n"
        + "Title length after scrapping: " + std::to_string(title.length()) + "\n"
        + "lb_width_inner - 1 : " + std::to_string(lb_width_inner)
        );
}

void full_render() {
    clearScr();
    print_instruction();
    frame_render();
    cover_render();
    metadata_render();
}