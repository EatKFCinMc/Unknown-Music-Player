#include "events.h"

#include <thread>

#include "globalVar.h"


void event_listener() {
    while (!TERMINATE) {
        if (KBHIT_PAUSE) {}
        if (PAUSE) {}
        if (NEXT) {}
        if (PREV) {}
        if (VOLUP) {}
        if (VOLDOWN) {}

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
