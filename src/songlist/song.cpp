#define MINIAUDIO_IMPLEMENTATION
#include <taglib/tag.h>
#include <taglib/fileref.h>
#include "miniaudio/miniaudio.h"

#include <string>
#include <string_view>
#include <filesystem>
#include <thread>

#include "song.h"

#include "globalVar.h"
#include "events.h"

#include "log.h"

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
        title = metaData.tag()->title().to8Bit(true);
        artist = metaData.tag()->artist().to8Bit(true);
        album = metaData.tag()->album().to8Bit(true);
        return true;
    }
    return false;
}


void Song::play() {
    if (!available) return;

    ma_result result;
    ma_engine engine;

    result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS)
        return;

    ma_sound sound;
    result = ma_sound_init_from_file(&engine, filePath.c_str(), 0, NULL, NULL, &sound);
    if (result != MA_SUCCESS) {
        ma_engine_uninit(&engine);
        return;
    }

    float time_t;
    ma_sound_get_length_in_seconds(&sound, &time_t);
    song_len = time_t;
    time_t = 0;
    song_cursor = 0;
    BAR_UPDATE = true;

    size_t framerate = ma_engine_get_sample_rate(&engine);
    ma_sound_start(&sound);

    bool flagPlaying = true;
    ma_uint64 cursor;
    ma_uint64 total_frame;
    ma_sound_get_cursor_in_pcm_frames(&sound, &cursor);
    ma_sound_get_length_in_pcm_frames(&sound, &total_frame);
    logger("Media engine initialization complete.\nCursor: "
        + std::to_string(cursor) + "\nTotal: " + std::to_string(total_frame));

    while (cursor < total_frame) {
        if (PAUSE) {
            if (flagPlaying) {
                ma_sound_stop(&sound);
                flagPlaying = false;
                PAUSE = !PAUSE;
            } else {
                ma_sound_start(&sound);
                flagPlaying = true;
                PAUSE = !PAUSE;
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
        ma_sound_get_cursor_in_pcm_frames(&sound, &cursor);
        song_cursor = static_cast<double>(cursor) / static_cast<double>(framerate);
        if (song_cursor - time_t >= 1) {
            BAR_UPDATE = true;
            time_t = static_cast<float>(song_cursor);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    ma_sound_uninit(&sound);
    ma_engine_uninit(&engine);
    if (cursor >= total_frame)
        SONG_END = true;
}

bool Song::is_available() const {
    return available;
}

std::string Song::getTitle() const {
    return title;
}

std::string Song::getArtist() const {
    return artist;
}

std::string Song::getAlbum() const {
    return album;
}
std::string Song::getSongPath() const {
    return filePath;
}
