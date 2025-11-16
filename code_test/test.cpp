#include <iostream>
#include <termios.h>
#include <unistd.h>

int getKey() {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

int main() {
    std::cout << "Press keys (q to quit):\n";
    while (true) {
        int c = getKey();
        if (c == 'q') break;

        if (c == 27) { // Escape sequence start
            if (getKey() == '[') {
                switch (getKey()) {
                    case 'A': std::cout << "UP\n"; break;
                    case 'B': std::cout << "DOWN\n"; break;
                    case 'C': std::cout << "RIGHT\n"; break;
                    case 'D': std::cout << "LEFT\n"; break;
                }
            }
        } else {
            std::cout << "You pressed: " << (char)c << " (" << c << ")\n";
        }
    }
    return 0;
}
