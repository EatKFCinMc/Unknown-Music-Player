SRC = ./src
BUILD = ./build

CFLAGS = -Wall -ldl -lpthread -lm

ump: main ui-common
	g++ -o ./ump $(BUILD)/main.o $(BUILD)/ui/common.o $(CFLAGS)

main: $(SRC)/main.cpp
	g++ -c $(SRC)/main.cpp -o $(BUILD)/main.o $(CFLAGS)

ui-common: $(SRC)/ui/common.cpp
	g++ -c $(SRC)/ui/common.cpp -o $(BUILD)/ui/common.o $(CFLAGS)