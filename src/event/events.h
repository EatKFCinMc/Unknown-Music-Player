#ifndef EVENTS_H
#define EVENTS_H

#define TERMINATE 1;
#define KBHIT_PAUSE 2;
#define PAUSE 3;
#define NEXT 4;
#define PREV 5;
#define VOLUP 6;
#define VOLDOWN 7;
#define LOGGER_DISABLED 8;
#define WINDOW_CHANGE 9;
#define SONG_END 10;
#define SONG_UPDATE 11;
#define BAR_UPDATE 12;

void event_listener();

#endif //EVENTS_H