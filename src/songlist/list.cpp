#include "list.h"

#include <filesystem>

#include "../event/events.h"

#include "../logger/log.h"

Playlist::Playlist(std::string dirPath) {
    if (!dirPath.empty()) {
        loadFromPath(dirPath);
    }
}

void Playlist::loadFromPath(std::string dirPath) {
    if (!std::filesystem::exists(dirPath)) return;
    if (std::filesystem::is_directory(dirPath)) {
        for (const auto &entry : std::filesystem::directory_iterator(dirPath)) {
            Song *s = new Song(entry.path().string());
            if (s->is_available()) {
                playList.push_back(s);
                amount++;
            }
        }
    } else {
        Song *s = new Song(dirPath);
        if (s->is_available()) {
            playList.push_back(s);
            amount++;
        }
    }
}

void Playlist::playFromList () {
    if (playList.empty()) return;

    playList.front()->play();
    if (TERMINATE) return;

    while (true) {
        if (SONG_END) {
            SONG_END = false;
            playNext();
        } else if (NEXT) {
            NEXT = false;
            playNext();
        } else if (PREV) {
            PREV = false;
            playPrev();
        }

        if (TERMINATE) break;
    }
}

void Playlist::shuffle () {
    current = 0;
}


void Playlist::playNext () {
    if (!playList.empty()) {
        auto temp = playList.front();
        playList.pop_front();
        playList.push_back(temp);
        logger(("Playing next. Current song list length: " + std::to_string(playList.size())).c_str());

        playList.front()->play();
    }
}


void Playlist::playPrev () {
    if (!playList.empty()) {
        auto temp = playList.back();
        playList.pop_back();
        playList.push_front(temp);
        logger(("Playing prev. Current song list length: " + std::to_string(playList.size())).c_str());

        playList.front()->play();
    }
}


void Playlist::addSong (Song *s) {
    if (s->is_available()) {
        playList.push_back(s);
    }
}