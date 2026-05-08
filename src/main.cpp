#include <iostream>
#include <filesystem>
#include <fcntl.h>
#include <thread>

#include "ui/common.h"
#include "ui/render.h"

#include "songlist/song.h"
#include "songlist/list.h"

#include "keyboard/keyboard.h"

#include "event/events.h"
#include "event/globalVar.h"

#include "logger/log.h"

#include "cover/showCover.h"


int tall, wide;
Playlist playlist;

void init_window() {
    enterAltScr();
    clearScr();
    hideCursor();

    full_render();

    refreshBuffer();
}


void init(const std::string &dir = "") {
    std::ios::sync_with_stdio(true);
    log_init();
    init_globalVar();
    playlist.loadFromPath(dir);
    list_ptr = &playlist;
    init_window();
    init_keyboard();

    std::thread keyboard_thread(keyboard_listener);
    keyboard_thread.detach();
    std::thread event_thread(event_listener);
    event_thread.detach();

    if (std::filesystem::exists(dir)) {
        playlist.playFromList();
    } else {
        printAt(term_width / 2 - 16, term_height / 2, "Error: No such file or directory", true);
        sleep(3);
    }

    showCursor();
    exitAltScr();
}


int main(int argc, char *argv[]) {
    std::string fileStr;
    if (argc == 2) fileStr = argv[1];
    else {
        printf("Usage: ump <audio_file_path>\n");
        return 0;
    }

    init(fileStr);
    return 0;
}
