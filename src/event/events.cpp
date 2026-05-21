#include "events.h"

#include <thread>
#include <oneapi/tbb/profiling.h>

#include "globalVar.h"
#include "render.h"
#include "log.h"

std::deque<size_t> event_bus;

void event_listener() {
    while (!TERMINATE) {
        if (KBHIT_PAUSE) {
            logger("KBHIT_PAUSE received");

        }
        if (PAUSE) {
            logger("PAUSE received");

        }
        if (NEXT) {
            logger("NEXT received");

        }
        if (PREV) {
            logger("PREV received");

        }
        if (VOLUP) {
            logger("VOLUP received");

        }
        if (VOLDOWN) {
            logger("VOLDOWN received");
        }
        if (WINDOW_CHANGE) {
            logger("WINDOW_CHANGE received");
            reload_layoutVar();
            full_render();
            WINDOW_CHANGE = false;
            continue;
        }
        // if (SONG_UPDATE) {
        //     logger("SONG_UPDATE received");
        //     reload_metadata();
        //     songinfo_render();
        //     list_render();
        //     cover_render();
        //     SONG_UPDATE = false;
        //     continue;
        // }
        if (BAR_UPDATE) {
            logger("BAR_UPDATE received");
            draw_bar();
            BAR_UPDATE = false;
            continue;
        }

        if (!event_bus.empty()) {
            size_t event = event_bus.front();
            event_bus.pop_front();

            switch (event) {
                case SONG_UPDATE_EVENT:
                    logger("SONG_UPDATE received");
                    std::string temp_title = title;
                    std::string temp_artist = artist;
                    std::string temp_album = album;
                    reload_metadata();
                    if (temp_title == title &&
                        temp_artist == artist &&
                        temp_album == album)
                        continue;
                    songinfo_render();
                    list_render();
                    cover_render();
                    // SONG_UPDATE = false;
                    continue;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
