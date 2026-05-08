#ifndef SONG_H
#define SONG_H

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
    [[nodiscard]] bool is_available() const;
    [[nodiscard]] std::string getTitle() const;
    [[nodiscard]] std::string getArtist() const;
    [[nodiscard]] std::string getAlbum() const;
    [[nodiscard]] std::string getSongPath() const;
};

#endif //SONG_H
