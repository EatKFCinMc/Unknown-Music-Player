#include "list.h"
#include <filesystem>


Playlist::Playlist(std::string dirPath) {
    if (!dirPath.empty()) {
        loadFromDir(dirPath);
    }
}

void Playlist::loadFromDir(std::string dirPath) {
    if (!std::filesystem::exists(dirPath)) return;
    for (const auto &entry : std::filesystem::directory_iterator(dirPath)) {
        if (entry.is_regular_file()) {
            Song s(entry.path().string());
            playList.push_back(s);
            amount++;
        }
    }
}

void Playlist::playFromList () {
    if (amount == 0) return;
    for (int i = 0; i < amount; i++) {
        playList[i].play();
    }
}

void Playlist::shuffle () {
    return;
}