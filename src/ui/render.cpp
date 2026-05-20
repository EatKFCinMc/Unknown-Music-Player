#include <filesystem>
#include <iostream>
#include <bits/locale_facets_nonio.h>

#include "common.h"
#include "showCover.h"
#include "globalVar.h"
#include "log.h"


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
    for (size_t i = 0; i < list_height; i++) {
        printAt(rb_pos_inner, i + rb_pos_height, space);
    }
}

void draw_list() {
    for (size_t i = 0; i < (list_ptr->size() > list_height ? list_height : list_ptr->size()); i++) {
        std::string title_t = list_ptr->at(i)->getTitle();
        std::string temp_title = title_t.substr(0, title_t.length() > list_title_len ? list_title_len : title_t.length());
        printAt(rb_pos_inner, i + rb_pos_height, temp_title);

        std::string artist_t = list_ptr->at(i)->getArtist();
        std::string temp_artist = artist_t.substr(0, artist_t.length() > list_artist_len ? list_artist_len : artist_t.length());
        printAt(rb_pos_inner + list_title_len + 3, i + rb_pos_height, temp_artist);
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

std::string milisec_to_min_string(size_t milisec) {
    size_t sec = milisec / 1000;
    size_t min = sec / 60;
    sec = sec % 60;
    std::string res = std::to_string(min) + ":" + std::to_string(sec);
    return res;
}


std::string sec_to_min_string(double sec) {
    size_t min = sec / 60;
    size_t sec_t = static_cast<int>(sec) % 60;
    std::string sec_str = std::to_string(sec_t);
    if (sec_str.length() == 1) sec_str = "0" + sec_str;
    std::string res = std::to_string(min) + ":" + sec_str;
    return res;
}

void draw_bar_frame() {
    printAt(1, lb_bar_pos, vertiRight);
    std::string line_t;
    for (size_t i = 0; i < lb_width_inner + 2; i++)
        line_t += horiLine;
    printAt(2, lb_bar_pos, line_t);
    printAt(lb_width, lb_bar_pos, vertiLeft);
}

void draw_bar() {
    std::string space;
    for (size_t i = 0; i < lb_bar_len; i++)
        space += ' ';
    printAt(4, lb_bar_pos + 2, space);
    printAt(4, lb_bar_pos + 3, space);

    size_t progress_chunk = static_cast<size_t>(current_time / total_time * static_cast<int>(lb_bar_len)) ? 0 : !total_time;
    std::string line_t;
    for (size_t i = 0; i < progress_chunk; i++)
        line_t += barThick;
    for (size_t i = 0; i < lb_bar_len - progress_chunk; i++)
        line_t += barEmpty;
    printAt(4, lb_bar_pos + 2, line_t);

    std::string curr_time_t = sec_to_min_string(current_time);
    std::string total_time_t = sec_to_min_string(total_time);
    printAt(4, lb_bar_pos + 3, curr_time_t);
    printAt(lb_width - 2 - total_time_t.length(), lb_bar_pos + 3, total_time_t);
}


void frame_render() {
    logger("frame_render initiated");
    draw_frame();
    draw_leftBox();
    draw_listHeader();
    draw_bar_frame();
}

void cover_render() {
    logger("cover_render initiated");
    displayCover(songPath);
}

void songinfo_render() {
    logger("songinfo_render initiated");
    draw_metadata();
    draw_bar();
}

void list_render() {
    logger("list_render initiated");
    clean_list();
    draw_list();
}

void full_render() {
    logger("full_render initiated");
    clearScr();
    print_instruction();
    frame_render();
    cover_render();
    list_render();
    songinfo_render();
}
