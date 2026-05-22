#ifndef EVENTS_H
#define EVENTS_H
#include <deque>

#define SONG_UPDATE_EVENT 1

extern std::deque<size_t> event_bus;

void event_listener();

#endif //EVENTS_H