#include <iostream>
#include <unistd.h>
#include <sys/ioctl.h>
#include <filesystem>
#include <fcntl.h>
#include <thread>
#include <oneapi/tbb/info.h>

#include "ui/common.h"
#include "ui/frame.h"

#include "songlist/song.h"
#include "songlist/list.h"

#include "keyboard/keyboard.h"

#include "event/events.h"
#include "event/globalVar.h"

#include "logger/log.h"


int tall, wide;

void getWinSize(int &row, int &col) {
    winsize w{};
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    row = w.ws_row;
    col = w.ws_col;
    term_height = w.ws_row;
    term_width = w.ws_col;
}


void init_globalVar() {
    title = "";
    artist = "";
    album = "";
    total_time = 0;
    current_time = 0;
    term_height = 0;
    term_width = 0;
    theme_color = 0;
}


void init(const std::string &dir) {
    init_globalVar();
    getWinSize(tall, wide);
    log_init();
    std::thread keyboard_thread(keyboard_listener);
    keyboard_thread.detach();

    int col, row;
    std::string s = "Just Listen";
    enterAltScr();
    clearScr();
    hideCursor();

    getWinSize(row, col);
    printAt(col / 2 - s.length() / 2, row / 2, s, 96, true);
    refreshBuffer();

    draw_border();
    printAt(2, 2, "Controls: 'p' or space = Play/Pause, 'q' = Quit, Arrow = next / prev");
    if (std::filesystem::exists(dir)) {
        Playlist playlist(dir);
        playlist.playFromList();
    } else {
        printAt(2, 3, "Error: No such file or directory");
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

    clearScr();
    exitAltScr();
    return 0;
}