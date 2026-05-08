#ifndef GLOBALVAR_H
#define GLOBALVAR_H

#include <string>
#include <mutex>
#include "../songlist/list.h"

// general data
extern std::string title;
extern std::string artist;
extern std::string album;
extern std::string songPath;
extern float total_time;
extern float current_time;
extern Playlist* list_ptr;

// terminal data
extern size_t term_height;
extern size_t term_width;
extern size_t theme_color;
extern std::string term_name;

// layout
extern size_t list_num;
// cover position and size
extern size_t cover_x;
extern size_t cover_y;
extern size_t cover_height;
extern size_t cover_width;
// metadata position
extern size_t title_row;
extern size_t artist_row;
extern size_t album_row;
// left box
extern size_t lb_width;
extern size_t lb_width_inner;

// flags
extern bool cover_drawing;

// event flags
extern bool TERMINATE;
extern bool KBHIT_PAUSE;
extern bool PAUSE;
extern bool NEXT;
extern bool PREV;
extern bool VOLUP;
extern bool VOLDOWN;
extern bool LOGGER_DISABLED;
extern bool WINDOW_CHANGE;
extern bool SONG_END;
extern bool SONG_UPDATE;

// characters

inline const char* downRight = "┌";
inline const char* upRight = "└";
inline const char* downLeft = "┐";
inline const char* upLeft = "┘";
inline const char* horiLine = "─";
inline const char* vertiLine = "│";
inline const char* vertiLeft = "┤";
inline const char* vertiRight = "├";
inline const char* horiDown = "┬";
inline const char* horiUp = "┴";
inline const char* cross = "┼";

void init_globalVar();
void get_term_name();
void reload_layoutVar();
void reload_metadata();

#endif //GLOBALVAR_H