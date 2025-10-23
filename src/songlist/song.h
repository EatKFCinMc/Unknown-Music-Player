#ifndef UNKNOWNMUSICPLAYER_SONG_H
#define UNKNOWNMUSICPLAYER_SONG_H

#include <string>

class Song {
    std::string title;
    std::string artist;
    std::string album;
    std::string filePath;
    bool available;
    bool inList = false;
public:
    explicit Song(std::string file, bool inlist = false);
    bool verifyPath();
    bool getMetaData();
    void play();
};

#endif //UNKNOWNMUSICPLAYER_SONG_H
