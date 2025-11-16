#include <iostream>
#include <unistd.h>
#include <sys/ioctl.h>
#include <filesystem>
#include <fcntl.h>
#include <thread>

#include "ui/common.h"

#include "songlist/song.h"
#include "songlist/list.h"

#include "keyboard/keyboard.h"

#include "event/events.h"

#include "logger/log.h"


int tall, wide;

void getWinSize(int &row, int &col) {
    winsize w{};
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    row = w.ws_row;
    col = w.ws_col;
}


int main(int argc, char *argv[]) {
    std::string fileStr;
    if (argc == 2) fileStr = argv[1];
    else {
        printf("Usage: ump <audio_file_path>\n");
        return 0;
    }

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

    drawBox(tall, wide, 1, 1, 96);
    printAt(2, 2, "Controls: 'p' = Play/Pause, 'q' = Quit, Arrow = next / prev");
    if (std::filesystem::exists(fileStr)) {
        Playlist playlist(fileStr);
        // std::thread event_thread(event_listener);
        // event_thread.detach();
        playlist.playFromList();
    } else {
        printAt(2, 3, "Error: No such file or directory");
        sleep(3);
    }

    clearScr();
    exitAltScr();
    return 0;
}