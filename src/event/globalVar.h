#ifndef GLOBALVAR_H
#define GLOBALVAR_H

#include <string>

extern std::string title;
extern std::string artist;
extern std::string album;
extern float total_time;
extern float current_time;

// terminal data
extern size_t term_height;
extern size_t term_width;
extern size_t theme_color;
extern std::string term_name;

// layout

// left box
extern size_t lb_width;



// flag
extern bool cover_drawing;

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

#endif //GLOBALVAR_H