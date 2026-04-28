#include <iostream>
#include <unistd.h>
#include <sys/ioctl.h>
#include <filesystem>
#include <fcntl.h>
#include <thread>

#include "ui/common.h"
#include "ui/frame.h"

#include "songlist/song.h"
#include "songlist/list.h"

#include "keyboard/keyboard.h"

#include "event/events.h"
#include "event/globalVar.h"

#include "logger/log.h"

#include "cover/showCover.h"


int tall, wide;

void getWinSize() {
    winsize w{};
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    term_height = w.ws_row;
    term_width = w.ws_col;
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


void init_window() {
    enterAltScr();
    clearScr();
    hideCursor();

    drawBorder();
    drawLeftBox();

    refreshBuffer();
}


void init(const std::string &dir = "") {
    std::ios::sync_with_stdio(true);
    log_init();
    init_globalVar();
    init_window();

    std::thread keyboard_thread(keyboard_listener);
    keyboard_thread.detach();


    if (std::filesystem::is_regular_file(dir))
        displayCover(2, 2, dir);

    printAt(lb_width + 3, 2, "Controls: 'p' or space = Play/Pause, 'q' = Quit, Arrow = next / prev");
    if (std::filesystem::exists(dir)) {
        Playlist playlist(dir);
        playlist.playFromList();
    } else {
        printAt(lb_width + 3, 3, "Error: No such file or directory");
        sleep(3);
    }

}


int main(int argc, char *argv[]) {
    std::string fileStr;
    if (argc == 2) fileStr = argv[1];
    else {
        printf("Usage: ump <audio_file_path>\n");
        return 0;
    }

    init(fileStr);

    showCursor();
    exitAltScr();
    return 0;
}
