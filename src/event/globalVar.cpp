#include "globalVar.h"

#include <unistd.h>
#include <sys/ioctl.h>
#include <csignal>
#include <cstring>

#include "../logger/log.h"

// song info
std::string title;
std::string artist;
std::string album;
std::string songPath;
float total_time = 0;
float current_time = 0;
Playlist* list_ptr = nullptr;

// terminal data
size_t term_height = 0;
size_t term_width = 0;
size_t theme_color = 0;
std::string term_name;

// layout
size_t list_num;
// cover position and size
size_t cover_x;
size_t cover_y;
size_t cover_height;
size_t cover_width;
// metadata position
size_t title_row;
size_t artist_row;
size_t album_row;
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
bool WINDOW_CHANGE = false;
bool SONG_END = false;
bool SONG_UPDATE = false;


void sigwinch_handler(int sig) {
    WINDOW_CHANGE = true;
}

void sigint_handler(int sig) {
    TERMINATE = true;
}

void updateWinSize() {
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
    list_ptr = nullptr;
    theme_color = 0;
    get_term_name();
    reload_layoutVar();

    std::string msg = std::string("Global variables initialization complete:\n") +
    "term_height = " + std::to_string(term_height) + "\n" +
    "term_width = " + std::to_string(term_width) + "\n" +
    "term_name = " + term_name + "\n" +
    "lb_width = " + std::to_string(lb_width);
    logger(msg);

    struct sigaction sa;
    std::memset(&sa, 0, sizeof(sa));
    sa.sa_handler = sigwinch_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGWINCH, &sa, nullptr);

    struct sigaction sb;
    std::memset(&sb, 0, sizeof(sb));
    sb.sa_handler = sigint_handler;
    sigemptyset(&sb.sa_mask);
    sb.sa_flags = 0;
    sigaction(SIGINT, &sb, nullptr);
}

void reload_layoutVar() {
    updateWinSize();

    lb_width = term_width / 3;
    lb_width_inner = lb_width - 2;
    cover_width = lb_width > 1 ? lb_width - 1 : 1;
    cover_height = std::min(cover_width / 2, term_height > 2 ? term_height - 2 : 1UL);
    cover_x = 2;
    cover_y = 2;
    title_row = cover_height + 2;
    artist_row = cover_height + 3;
    album_row = cover_height + 4;
}

void reload_metadata() {
    title = list_ptr->getFront()->getTitle();
    artist = list_ptr->getFront()->getArtist();
    album = list_ptr->getFront()->getAlbum();
    songPath = list_ptr->getFront()->getSongPath();
}
