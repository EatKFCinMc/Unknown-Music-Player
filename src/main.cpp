#include <iostream>
#include <unistd.h>
#include <sys/ioctl.h>
#include <filesystem>
#include <fcntl.h>

#include "ui/common.h"

#include "songlist/song.h"
#include "songlist/list.h"


int tall, wide;

void getWinSize(int &row, int &col) {
    winsize w{};
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    row = w.ws_row;
    col = w.ws_col;
}


int main(int argc, char *argv[]) {
    getWinSize(tall, wide);
    std::string fileStr;
    if (argc == 2) fileStr = argv[1];
    else {
        printf("Usage: ump <audio_file_path>\n");
        return 0;
    }
    // else fileStr = "./test/Alea jacta est! (xi Remix) - BlackY.mp3";

    int col, row;
    std::string s = "Just Listen";
    enterAltScr();
    clearScr();
    hideCursor();

    getWinSize(row, col);
    printAt(col / 2 - s.length() / 2, row / 2, s, 96, true);
    refreshBuffer();

    drawBox(tall, wide, 1, 1);
    printAt(2, 2, "Controls: 'p' = Play/Pause, 'q' = Quit");
    if (std::filesystem::is_directory(fileStr)) {
        Playlist playlist(fileStr);
        playlist.playFromList();
    } else {
        Song song(fileStr);
        song.play();
    }

    clearScr();
    s = "Exiting in 2 seconds...";
    printAt(col / 2 - s.length() / 2, row / 2, s, 94, true);
    refreshBuffer();
    sleep(2);

    exitAltScr();
    return 0;
}