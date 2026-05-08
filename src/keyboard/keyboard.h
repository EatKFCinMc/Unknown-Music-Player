#ifndef KEYBOARD_H
#define KEYBOARD_H

void init_keyboard();
int kbhit();
int getch();
void keyboard_listener();
void arrow_input();

#endif //KEYBOARD_H