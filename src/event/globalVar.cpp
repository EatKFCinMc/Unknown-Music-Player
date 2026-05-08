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
size_t lb_width;
size_t lb_width_inner;
// right box
size_t rb_width;
size_t rb_width_inner;
size_t rb_pos;
size_t rb_pos_inner;
size_t rb_height;
size_t rb_pos_height;

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
    total_time = 0;
    current_time = 0;
    list_ptr = nullptr;
    theme_color = 0;
    get_term_name();
    reload_layoutVar();

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

    // left box
    lb_width = term_width / 3 + 1;
    lb_width_inner = lb_width - 4;
    // right box
    rb_width = term_width - lb_width + 1;
    rb_width_inner = rb_width - 4;
    rb_pos = lb_width;
    rb_pos_inner = lb_width + 2;
    rb_height = term_height - 2;
    rb_pos_height = 4;
    // others
    cover_width = lb_width > 1 ? lb_width - 2 : 1;
    cover_height = std::min(cover_width / 2, term_height > 2 ? term_height - 2 : 1UL);
    cover_x = 2;
    cover_y = 2;
    title_row = cover_height + 2;
    artist_row = cover_height + 3;
    album_row = cover_height + 4;

    std::string msg = std::string("Layout variables reloaded:\n") +
    "term_height = " + std::to_string(term_height) + "\n" +
    "term_width = " + std::to_string(term_width) + "\n" +
    "term_name = " + term_name + "\n" +
    "lb_width = " + std::to_string(lb_width) + "\n" +
    "lb_width_inner = " + std::to_string(lb_width_inner) + "\n" +
    "rb_width = " + std::to_string(rb_width) + "\n" +
    "rb_width_inner = " + std::to_string(rb_width_inner) + "\n" +
    "rb_pos = " + std::to_string(rb_pos) + "\n" +
    "rb_pos_inner = " + std::to_string(rb_pos_inner) + "\n" +
    "rb_height = " + std::to_string(rb_height) + "\n" +
    "cover_width = " + std::to_string(cover_width) + "\n" +
    "cover_height = " + std::to_string(cover_height) + "\n" +
    "cover_x = " + std::to_string(cover_x) + "\n" +
    "cover_y = " + std::to_string(cover_y) + "\n" +
    "title_row = " + std::to_string(title_row) + "\n" +
    "artist_row = " + std::to_string(artist_row) + "\n" +
    "album_row = " + std::to_string(album_row);
    logger(msg);
}

void reload_metadata() {
    title = list_ptr->getFront()->getTitle();
    artist = list_ptr->getFront()->getArtist();
    album = list_ptr->getFront()->getAlbum();
    songPath = list_ptr->getFront()->getSongPath();

    std::string msg = std::string("Metadata reloaded:\n") +
    "title = " + title + "\n" +
    "artist = " + artist + "\n" +
    "album = " + album + "\n" +
    "songPath = " + songPath;
    logger(msg);
}
