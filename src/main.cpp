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
#include "config.h"

int tall, wide;
Playlist playlist;

void init_window() {
    enterAltScr();
    clearScr();
    hideCursor();

    full_render();

    refreshBuffer();
}


void init() {
    std::ios::sync_with_stdio(true);
    log_init();
    init_globalVar();
    playlist.loadFromPath(root_dir);
    list_ptr = &playlist;
    reload_metadata();
    init_window();
    init_keyboard();

    std::thread keyboard_thread(keyboard_listener);
    std::thread event_thread(event_listener);

    playlist.playFromList();

    // if (std::filesystem::exists(dir)) {
    //     playlist.playFromList();
    // } else {
    //     printAt(term_width / 2 - 16, term_height / 2, "Error: No such file or directory", true);
    //     sleep(3);
    // }

    keyboard_thread.join();
    event_thread.join();

    write_to_config();
    showCursor();
    exitAltScr();
}


int main(int argc, char *argv[]) {
    // loading root_dir from config
    load_from_config();
    std::string fileStr;

    if (argc == 2) {
        fileStr = argv[1];
        if (!std::filesystem::exists(fileStr)) {
            printf("Illgal path or path not exist\n");
            return 0;
        }
        root_dir = fileStr;
    }
    else if (root_dir.empty()) {
        printf("Usage: ump <audio_file_or_dir_path>\n");
        return 0;
    }

    if (!std::filesystem::exists(root_dir)) {
        printf("Previous saved folder can't be found\n"
               "Please initiate again.\n");
        root_dir = "";
        write_to_config();
        return 0;
    }

    init();
    return 0;
}
