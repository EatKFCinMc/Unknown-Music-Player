SRC = ./src
BUILD = ./build
SONGLIST = songlist-song songlist-list
KEYBOARD = keyboard-keyboard
UI = ui-common

CFLAGS = -Wall -ldl -lpthread -lm -ltag -std=c++20 -I ./include

ump: main $(SONGLIST) $(KEYBOARD) $(UI)
	g++ -o ./ump $(BUILD)/*.o $(CFLAGS)

main: $(SRC)/main.cpp
	g++ -c $(SRC)/main.cpp -o $(BUILD)/main.o $(CFLAGS)

ui-common: $(SRC)/ui/common.cpp
	g++ -c $(SRC)/ui/common.cpp -o $(BUILD)/common.o $(CFLAGS)

keyboard-keyboard: $(SRC)/keyboard/keyboard.cpp
	g++ -c $(SRC)/keyboard/keyboard.cpp -o $(BUILD)/keyboard.o $(CFLAGS)

songlist-song: $(SRC)/songlist/song.cpp
	g++ -c $(SRC)/songlist/song.cpp -o $(BUILD)/song.o $(CFLAGS)

songlist-list: $(SRC)/songlist/list.cpp
	g++ -c $(SRC)/songlist/list.cpp -o $(BUILD)/list.o $(CFLAGS)

clean:
	rm -f $(BUILD)/*.o ump