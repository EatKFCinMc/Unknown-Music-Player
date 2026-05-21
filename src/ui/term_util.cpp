#include "term_util.h"

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <chrono>
#include <cstring>
#include <iostream>
#include <string>

#include "globalVar.h"


void updateWinSize() {
    winsize w{};
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    term_height = w.ws_row;
    term_width = w.ws_col;
}

void get_term_name() {
    const char *term = std::getenv("TERM");
    term_name = term == nullptr ? "" : term;
}

bool detect_kitty_support(int timeout_ms) {
    if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO))
        return false;

    termios old_termios{};
    if (tcgetattr(STDIN_FILENO, &old_termios) != 0)
        return false;

    termios raw = old_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;

    if (tcsetattr(STDIN_FILENO, TCSANOW, &raw) != 0)
        return false;

    int old_flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, old_flags | O_NONBLOCK);

    const char* query =
        "\033_Gi=31,s=1,v=1,a=q,t=d,f=24;AAAA\033\\"
        "\033[c";

    write(STDOUT_FILENO, query, std::strlen(query));
    fsync(STDOUT_FILENO);

    std::string response;
    char buffer[256];

    auto start = std::chrono::steady_clock::now();

    while (true) {
        ssize_t n = read(STDIN_FILENO, buffer, sizeof(buffer));
        if (n > 0) {
            response.append(buffer, buffer + n);

            if (response.find("\033_G") != std::string::npos) {
                tcsetattr(STDIN_FILENO, TCSANOW, &old_termios);
                fcntl(STDIN_FILENO, F_SETFL, old_flags);
                return true;
            }

            if (response.find("\033[?") != std::string::npos &&
                response.find('c') != std::string::npos) {
                tcsetattr(STDIN_FILENO, TCSANOW, &old_termios);
                fcntl(STDIN_FILENO, F_SETFL, old_flags);
                return false;
            }
        }

        auto now = std::chrono::steady_clock::now();
        auto elapsed =
            std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();

        if (elapsed > timeout_ms)
            break;

        usleep(1000);
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &old_termios);
    fcntl(STDIN_FILENO, F_SETFL, old_flags);
    return false;
}