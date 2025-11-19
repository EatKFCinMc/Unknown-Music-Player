SRC = ./src
BUILD = ./build

SONGLIST = songlist-song songlist-list
KEYBOARD = keyboard-keyboard
UI = ui-common ui-frame
EVENT = event-events event-globalVar
LOGGER = logger-log
PIC = pic-loadPic

CXXFLAGS=$(shell pkg-config --cflags chafa glib-2.0 taglib)
LDFLAGS=$(shell pkg-config --libs chafa glib-2.0 taglib)

FLAGS = -Wall -ldl -lpthread -lm -ltag -std=c++20 -I ./include $(CXXFLAGS) $(LDFLAGS)

ump: main $(SONGLIST) $(KEYBOARD) $(UI) $(EVENT) $(LOGGER) $(PIC)
	g++ -o ./ump $(BUILD)/*.o $(FLAGS)

main: $(SRC)/main.cpp
	g++ -c $(SRC)/main.cpp -o $(BUILD)/main.o $(FLAGS)

ui-common: $(SRC)/ui/common.cpp
	g++ -c $(SRC)/ui/common.cpp -o $(BUILD)/common.o $(FLAGS)

ui-frame: $(SRC)/ui/frame.cpp
	g++ -c $(SRC)/ui/frame.cpp -o $(BUILD)/frame.o $(FLAGS)

keyboard-keyboard: $(SRC)/keyboard/keyboard.cpp
	g++ -c $(SRC)/keyboard/keyboard.cpp -o $(BUILD)/keyboard.o $(FLAGS)

songlist-song: $(SRC)/songlist/song.cpp
	g++ -c $(SRC)/songlist/song.cpp -o $(BUILD)/song.o $(FLAGS)

songlist-list: $(SRC)/songlist/list.cpp
	g++ -c $(SRC)/songlist/list.cpp -o $(BUILD)/list.o $(FLAGS)

event-events: $(SRC)/event/events.cpp
	g++ -c $(SRC)/event/events.cpp -o $(BUILD)/events.o $(FLAGS)

event-globalVar: $(SRC)/event/globalVar.cpp
	g++ -c $(SRC)/event/globalVar.cpp -o $(BUILD)/globalVar.o $(FLAGS)

logger-log: $(SRC)/logger/log.cpp
	g++ -c $(SRC)/logger/log.cpp -o $(BUILD)/log.o $(FLAGS)

pic-loadPic: $(SRC)/pic/loadPic.cpp
	g++ -c $(SRC)/pic/loadPic.cpp -o $(BUILD)/loadPic.o $(FLAGS)

clean:
	rm -f $(BUILD)/*.o ump