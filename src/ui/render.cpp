#include <filesystem>
#include <iostream>
#include <bits/locale_facets_nonio.h>

#include "common.h"
#include "../cover/showCover.h"
#include "../event/globalVar.h"
#include "../logger/log.h"


void draw_frame() {
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


void draw_leftBox() {
    if (term_width < 3 || term_height < 2 || lb_width >= term_width)
        return;

    printf("\033[%d;%luH%s", 1, lb_width, horiDown);
    for (size_t i = 2; i < term_height; i++)
        printf("\033[%lu;%luH%s", i, lb_width, vertiLine);
    printf("\033[%lu;%luH%s", term_height, lb_width, horiUp);
}

void draw_listHeader() {
    printAt(rb_pos_inner + rb_width_inner / 4 * 3, 1, horiDown);
    printAt(rb_pos_inner, 2, "Title");
    printAt(rb_pos_inner + rb_width_inner / 4 * 3, 2, vertiLine);
    printAt(rb_pos_inner + rb_width_inner / 4 * 3 + 2, 2, "Artist");
    std::string temp;
    for (size_t i = lb_width_inner; i < term_width - 4; i++) temp += horiLine;
    printAt(rb_pos_inner - 1, 3, temp);
    printAt(rb_pos_inner - 2, 3, vertiRight);
    printAt(term_width, 3, vertiLeft);
    printAt(rb_pos_inner + rb_width_inner / 4 * 3, 3, horiUp);
}

void clean_list() {
    std::string space;
    for (size_t i = 0; i < rb_width_inner; i++)
        space += ' ';
    for (size_t i = 0; i < rb_height - 2; i++) {
        printAt(rb_pos_inner, i + rb_pos_height, space);
    }
}

void draw_list() {
    for (size_t i = 0; i < (list_ptr->size() > rb_height ? rb_height : list_ptr->size()); i++) {
        std::string title_t = list_ptr->at(i)->getTitle();
        std::string temp_title = title_t.substr(0, title_t.length() > rb_width_inner ? rb_width_inner : title_t.length());
        printAt(rb_pos_inner, i + rb_pos_height, temp_title);
    }

    // std::string title_t = list_ptr->at(0)->getTitle();
    // std::string temp_title = title_t.substr(0, title_t.length() > rb_width_inner ? rb_width_inner : title_t.length());
    // printAt(rb_pos_inner, 2, temp_title);
}

void draw_metadata() {
    std::string space;
    for (size_t i = 0; i < lb_width_inner; i++)
        space += ' ';

    std::string temp_title = title.substr(0, title.length() > lb_width_inner ? lb_width_inner : title.length());
    std::string temp_artist = artist.substr(0, artist.length() > lb_width_inner ? lb_width_inner : artist.length());
    std::string temp_album = album.substr(0, album.length() > lb_width_inner ? lb_width_inner : album.length());
    if (title_row < term_height) {
        printAt(3, title_row, space);
        printAt(3, title_row, temp_title);
    }
    if (artist_row < term_height) {
        printAt(3, artist_row, space);
        printAt(3, artist_row, temp_artist);
    }
    if (album_row < term_height) {
        printAt(3, album_row, space);
        printAt(3, album_row, temp_album);
    }
}

void frame_render() {
    draw_frame();
    draw_leftBox();
    draw_listHeader();
}

void cover_render() {
    displayCover(songPath);
}

void metadata_render() {
    draw_metadata();
}

void list_render() {
    clean_list();
    draw_list();
}

void full_render() {
    clearScr();
    print_instruction();
    frame_render();
    cover_render();
    list_render();
    metadata_render();
}