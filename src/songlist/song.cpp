#define MINIAUDIO_IMPLEMENTATION
#include <taglib/tag.h>
#include <taglib/fileref.h>
#include "miniaudio/miniaudio.h"

#include <string>
#include <string_view>
#include <filesystem>

#include "song.h"

#include "../ui/common.h"

#include "../event/events.h"


std::string suffixes[] = {".mp3", ".wav", ".flac", ".aac", ".ogg", ".m4a"};

Song::Song(std::string file, bool inlist) {
    filePath = file;
    inList = inlist;
    available = false;
    if (verifyPath()) {
        getMetaData();
    }
}


bool Song::verifyPath() {
    if (filePath.empty()) return false;
    if (!std::filesystem::exists(filePath)) return false;
    for (auto suf : suffixes) {
        if (filePath.ends_with(suf)) {
            available = true;
            return true;
        }
    }
    return false;
}


bool Song::getMetaData() {
    TagLib::FileRef metaData(filePath.c_str());
    if (!metaData.isNull() && metaData.tag()) {
        title = metaData.tag()->title().to8Bit();
        artist = metaData.tag()->artist().to8Bit();
        album = metaData.tag()->album().to8Bit();
        return true;
    }
    return false;
}


void Song::play() {
    if (!available) return;

    ma_result result;
    ma_engine engine;

    result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS) {
        return;
    }

    ma_sound sound;
    result = ma_sound_init_from_file(&engine, filePath.c_str(), 0, NULL, NULL, &sound);
    if (result != MA_SUCCESS) {
        return;
    }
    printAt(2, 3, "Playing: " + title);
    refreshBuffer();

    ma_sound_start(&sound);

    bool flagPlaying = true;
    ma_uint64 cursor = ma_sound_get_length_in_pcm_frames(&sound, &cursor);
    ma_uint64 total = ma_sound_get_length_in_pcm_frames(&sound, &total);
    while (cursor <= total) {
        if (PAUSE) {
            if (flagPlaying) {
                ma_sound_stop(&sound);
                flagPlaying = false;
            } else {
                ma_sound_start(&sound);
                flagPlaying = true;
            }
        }
        if (NEXT || PREV) {
            ma_sound_stop(&sound);
            break;
        }
        if (TERMINATE) {
            ma_sound_stop(&sound);
            break;
        }
    }

    ma_sound_uninit(&sound);
    ma_engine_uninit(&engine);
    clearLine(3, 2, title.length() + 9);
    refreshBuffer();
}

bool Song::is_available() {
    return available;
}

