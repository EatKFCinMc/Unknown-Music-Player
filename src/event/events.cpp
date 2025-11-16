#include "events.h"

bool TERMINATE = false;
bool KBHIT_PAUSE = false;
bool PAUSE = false;
bool NEXT = false;
bool PREV = false;
bool VOLUP = false;
bool VOLDOWN = false;
bool LOGGER_DISABLED = false;
bool SONG_END = false;


void event_listener() {
    while (!TERMINATE) {
        if (KBHIT_PAUSE) {}
        if (PAUSE) {}
        if (NEXT) {}
        if (PREV) {}
        if (VOLUP) {}
        if (VOLDOWN) {}

    }
}
