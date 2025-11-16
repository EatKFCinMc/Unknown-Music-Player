#ifndef EVENTS_H
#define EVENTS_H

extern bool TERMINATE;
extern bool KBHIT_PAUSE;
extern bool PAUSE;
extern bool NEXT;
extern bool PREV;
extern bool VOLUP;
extern bool VOLDOWN;
extern bool LOGGER_DISABLED;
extern bool WINDOW_CHANGE;
extern bool SONG_END;

void event_listener();

#endif //EVENTS_H