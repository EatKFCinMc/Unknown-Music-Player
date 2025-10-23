#include <unistd.h>
#include <cstdio>
#include <fcntl.h>
#include <string>
#include <termios.h>
#include "miniaudio/miniaudio.h"

#include "../ui/common.h"

int kbhit() {
    termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}


int getch() {
    int ch;
    termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

void playing(std::string filePath) {
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
    printAt(2, 3, "Playing: " + filePath);
    refreshBuffer();

    ma_sound_start(&sound);

    bool flagPlaying = true;
    ma_uint64 cursor = ma_sound_get_length_in_pcm_frames(&sound, &cursor), total = ma_sound_get_length_in_pcm_frames(&sound, &total);
    while (cursor <= total) {
        if (kbhit()) {
            char c = getch();
            if (c == 'p') {
                if (flagPlaying) {
                    ma_sound_stop(&sound);
                    flagPlaying = false;
                } else {
                    ma_sound_start(&sound);
                    flagPlaying = true;
                }
            }
            else if (c == 'q') break;
        }
    }

    ma_sound_uninit(&sound);
    ma_engine_uninit(&engine);
}