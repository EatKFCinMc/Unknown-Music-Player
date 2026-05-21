#include "keyboard.h"

#include <unistd.h>
#include <cstdio>
#include <fcntl.h>
#include <termios.h>
#include <thread>

#include "miniaudio/miniaudio.h"

#include "events.h"
#include "log.h"
#include "globalVar.h"

// int kbhit() {
//     termios oldt, newt;
//     int ch;
//     int oldf;
//
//     tcgetattr(STDIN_FILENO, &oldt);
//     newt = oldt;
//     newt.c_lflag &= ~(ICANON | ECHO);
//     tcsetattr(STDIN_FILENO, TCSANOW, &newt);
//
//     oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
//     fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
//
//     ch = getchar();
//     // int result = read(STDIN_FILENO, &ch, 1);
//
//     tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
//     fcntl(STDIN_FILENO, F_SETFL, oldf);
//
//     if (ch != EOF) {
//     // if (result == 1) {
//         ungetc(ch, stdin);
//         return 1;
//     }
//     return 0;
// }
//
//
// int getch() {
//     int ch;
//     termios oldt, newt;
//     tcgetattr(STDIN_FILENO, &oldt);
//     newt = oldt;
//     newt.c_lflag &= ~(ICANON | ECHO);
//     tcsetattr(STDIN_FILENO, TCSANOW, &newt);
//     ch = getchar();
//     tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
//     return ch;
// }


static termios oldt;
static int oldf;

void init_keyboard() {
    termios newt;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);

    atexit([]() {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        fcntl(STDIN_FILENO, F_SETFL, oldf);
    });
}

int kbhit() {
    int ch;

    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    clearerr(stdin);
    return 0;
}

int getch() {
    return getchar();
}

void keyboard_listener() {
    while (!TERMINATE) {
        if (cover_drawing) continue;
        if (kbhit()) {
            char c = getch();
            switch (c){
                case 'q':
                    TERMINATE = true;
                    break;
                case 'p':
                case ' ':
                    PAUSE = !PAUSE;
                    break;
                case 27:
                    arrow_input();
                    break;
                default:
                    break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void arrow_input() {
    if (kbhit()) {
        char first = getch();
        if (first == '[') {
            if (kbhit()) {
                char second = getch();
                switch (second) {
                    case 'A': // Up arrow key pressed
                        break;
                    case 'B': // Down arrow key pressed
                        break;
                    case 'C': // Right arrow key pressed
                        NEXT = true;
                        break;
                    case 'D': // Left arrow key pressed
                        PREV = true;
                        break;
                    default:
                        break;
                }
            }
        }
    }
}