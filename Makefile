CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -Ivendor -O2

SRC = $(wildcard src/*.c)

LIBS = $(shell pkg-config --libs raylib)

TARGET = build/app

all:
	mkdir -p build

	# Generate compile_commands.json
	bear -- $(CC) $(SRC) $(CFLAGS) $(LIBS) -o $(TARGET)

run: all
	./$(TARGET)

clean:
	rm -rf build
