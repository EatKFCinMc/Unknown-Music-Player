#include <iostream>
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

    displayCover(2, 2, "/home/Empty/Project/unknownMusicPlayer/test/Alea jacta est! (xi Remix) - BlackY.mp3");

    // printAt(lb_width + 3, 2, "Controls: 'p' or space = Play/Pause, 'q' = Quit, Arrow = next / prev");
    print_instruction();
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
