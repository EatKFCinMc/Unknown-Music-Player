#ifndef UNKNOWNMUSICPLAYER_LIST_H
#define UNKNOWNMUSICPLAYER_LIST_H
#include <vector>
#include <string>

#include "song.h"

class Playlist {
    std::pmr::vector<Song> playList;
    int amount = 0;
    int current = 0;
public:
    explicit Playlist (std::string dirPath = "");
    void loadFromDir (std::string dirPath);
    void playFromList ();
    void shuffle ();
};

#endif //UNKNOWNMUSICPLAYER_LIST_H