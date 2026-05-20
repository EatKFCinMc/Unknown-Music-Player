#include "events.h"

#include <thread>

#include "globalVar.h"
#include "render.h"
#include "log.h"

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
        }
        if (SONG_UPDATE) {
            logger("SONG_UPDATE received");
            reload_metadata();
            songinfo_render();
            list_render();
            cover_render();
            SONG_UPDATE = false;
        }
        if (BAR_UPDATE) {
            logger("BAR_UPDATE received");
            draw_bar();
            BAR_UPDATE = false;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
