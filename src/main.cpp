#include <iostream>
#include <filesystem>
#include <fcntl.h>
#include <thread>
#include <unistd.h>

#include "common.h"
#include "render.h"
#include "list.h"
#include "keyboard.h"
#include "events.h"
#include "globalVar.h"
#include "log.h"

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
    reload_metadata();
    init_window();
    init_keyboard();

    std::thread keyboard_thread(keyboard_listener);
    std::thread event_thread(event_listener);

    if (std::filesystem::exists(dir)) {
        playlist.playFromList();
    } else {
        printAt(term_width / 2 - 16, term_height / 2, "Error: No such file or directory", true);
        sleep(3);
    }

    keyboard_thread.join();
    event_thread.join();

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
