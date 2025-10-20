#include <iostream>
#include <unistd.h>
#include <sys/ioctl.h>
#include <filesystem>
#include <termios.h>
#include <fcntl.h>


#include "./ui/common.h"

#ifndef MINIAUDIO_IMPLEMENTATION
#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio/miniaudio.h>
#endif
#define MA_DEBUG_OUTPUT


void getWinSize(int &row, int &col) {
    winsize w{};
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    row = w.ws_row;
    col = w.ws_col;
}


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


void musicPlay(std::string fileName) {
    if (!std::filesystem::exists(fileName)) {
        printf("File not found: %s\n", fileName.c_str());
        return;
    }

    ma_result result;
    ma_engine engine;

    result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS) {
        return;
    }

    ma_sound sound;
    result = ma_sound_init_from_file(&engine, fileName.c_str(), 0, NULL, NULL, &sound);
    if (result != MA_SUCCESS) {
        return;
    }
    printAt(1, 1, "Controls: 'p' = Play/Pause, 'q' = Quit");
    printAt(1, 2, "Playing: " + fileName);
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

    printf("\nComplete!\n");

    ma_sound_uninit(&sound);
    ma_engine_uninit(&engine);

}

int main(int argc, char *argv[]) {
    std::string fileStr;
    if (argc == 2) fileStr = argv[1];
    else {
        printf("Usage: ump <audio_file_path>\n");
        return 0;
    }
    // else fileStr = "./test/Alea jacta est! (xi Remix) - BlackY.mp3";

    int col, row;
    std::string s = "Just Listen";
    enterAltScr();
    clearScr();
    hideCursor();

    getWinSize(row, col);
    printAt(col / 2 - s.length() / 2, row / 2, s, 96);
    refreshBuffer();

    musicPlay(fileStr);

    clearScr();
    s = "Exiting in 2 seconds...";
    printAt(col / 2 - s.length() / 2, row / 2, s, 94);
    refreshBuffer();
    sleep(2);

    exitAltScr();
    return 0;
}