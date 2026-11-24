CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -Ivendor -O2

SRC = $(wildcard src/*.c)

LIBS = $(shell pkg-config --libs raylib) -lm

TARGET = build/app

all:
	mkdir -p build

	# Generate compile_commands.json
	bear -- $(CC) $(SRC) $(CFLAGS) $(LIBS) -o $(TARGET)

run: all
	./$(TARGET)

debug:
	$(CC) $(SRC) $(CFLAGS) $(LIBS) \
		-g -O0 -DDEBUG \
		-fsanitize=address,undefined \
		-fno-omit-frame-pointer \
		-o $(TARGET)
	./$(TARGET)

clean:
	rm -rf build
