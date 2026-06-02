#include "list.h"

#include <filesystem>
#include <vector>

#include "globalVar.h"
#include "events.h"
#include "log.h"

struct load_unit {
    std::string path;
    Song* song;
};

Playlist::Playlist(std::string dir_path) {
    if (!dir_path.empty()) {
        loadFromPath(dir_path);
    }
}

void Playlist::loadFromPath(std::string dir_path) {
    if (!std::filesystem::exists(dir_path)) return;

    if (std::filesystem::is_directory(dir_path)) {
        std::vector<load_unit> load_list;
        for (const auto &entry : std::filesystem::recursive_directory_iterator(dir_path))
            load_list.push_back(load_unit(entry.path().string(), nullptr));

        #pragma omp parallel for schedule(dynamic, 8)
        for (size_t i = 0; i < load_list.size(); i++)
            load_list[i].song = new Song(load_list[i].path);

        for (const auto &entry : load_list) {
            if (entry.song == nullptr) continue;
            if (!entry.song->is_available()) continue;
            playList.push_back(entry.song);
            amount++;
        }

    } else {
        Song *s = new Song(dir_path);
        if (s->is_available()) {
            playList.push_back(s);
            amount++;
        }
    }
}

void Playlist::playFromList () {
    if (playList.empty()) return;

    event_bus.push_front(SONG_UPDATE_EVENT);
    // SONG_UPDATE = true;
    playList.front()->play();
    if (TERMINATE) return;

    while (true) {
        if (SONG_END || NEXT) {
            NEXT = false;
            SONG_END = false;
            playNext();
        } else if (PREV) {
            PREV = false;
            SONG_END = false;
            playPrev();
        }

        if (TERMINATE) break;
    }
}

void Playlist::shuffle () {
}


void Playlist::playNext () {
    if (!playList.empty()) {
        auto temp = playList.front();
        playList.pop_front();
        playList.push_back(temp);
        event_bus.push_front(SONG_UPDATE_EVENT);
        // SONG_UPDATE = true;
        logger("Playing next. Current song list length: " + std::to_string(playList.size()));

        playList.front()->play();
    }
}


void Playlist::playPrev () {
    if (!playList.empty()) {
        auto temp = playList.back();
        playList.pop_back();
        playList.push_front(temp);
        event_bus.push_front(SONG_UPDATE_EVENT);
        // SONG_UPDATE = true;
        logger("Playing prev. Current song list length: " + std::to_string(playList.size()));

        playList.front()->play();
    }
}


void Playlist::addSong (Song *s) {
    if (s->is_available()) {
        playList.push_back(s);
    }
}

Song* Playlist::getFront() {
    if (!playList.empty())
        return playList.front();
    return nullptr;
}

Song* Playlist::get_curr() {
    if (!playList.empty())
        return playList[list_cursor];
    return nullptr;
}

Song* Playlist::at(size_t index) {
    if (index >= playList.size())
        return nullptr;
    return playList[index];
}

size_t Playlist::size() {
    return playList.size();
}