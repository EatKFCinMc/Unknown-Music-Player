#include "globalVar.h"

#include <unistd.h>
#include <sys/ioctl.h>

#include "../logger/log.h"

// song info
std::string title = "";
std::string artist = "";
std::string album = "";
float total_time = 0;
float current_time = 0;

// terminal data
size_t term_height = 0;
size_t term_width = 0;
size_t theme_color = 0;
std::string term_name = "";

// layout
// left box
size_t lb_width = 0;
size_t lb_width_inner = 0;

// flags
bool cover_drawing = false;

// event flags
bool TERMINATE = false;
bool KBHIT_PAUSE = false;
bool PAUSE = false;
bool NEXT = false;
bool PREV = false;
bool VOLUP = false;
bool VOLDOWN = false;
bool LOGGER_DISABLED = false;
bool SONG_END = false;
bool SONG_UPDATE = false;

void getWinSize() {
    winsize w{};
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    term_height = w.ws_row;
    term_width = w.ws_col;
}

void get_term_name() {
    const char *term = std::getenv("TERM");
    term_name = term == nullptr ? "" : term;
}

void init_globalVar() {
    title = "";
    artist = "";
    album = "";
    total_time = 0;
    current_time = 0;
    getWinSize();
    theme_color = 0;
    get_term_name();
    lb_width = term_width / 3;

    std::string msg = std::string("Global variables initialization complete:\n") +
    "term_height = " + std::to_string(term_height) + "\n" +
    "term_width = " + std::to_string(term_width) + "\n" +
    "term_name = " + term_name + "\n" +
    "lb_width = " + std::to_string(lb_width);
    logger(msg);
}