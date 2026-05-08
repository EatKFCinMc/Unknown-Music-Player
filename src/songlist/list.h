#ifndef LIST_H
#define LIST_H
#include <deque>
#include <string>

#include "song.h"

bool is_music_file(std::string filePath);

class Playlist {
    std::deque<Song*> playList;
    size_t amount = 0;
    size_t current = 0;
public:
    explicit Playlist (std::string dirPath = "");
    void loadFromPath (std::string dirPath);
    void playFromList ();
    void shuffle ();
    void playNext ();
    void playPrev ();
    void addSong (Song *s);
    Song* getFront();
    Song* at(size_t index);
    size_t size();
};

#endif //LIST_H