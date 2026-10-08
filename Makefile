CC = gcc
CFLAGS = -std=c99 -Wall -Wextra

all: build/lexer

build/lexer: src/lexer.c
	mkdir -p build
	$(CC) $(CFLAGS) src/lexer.c -o build/lexer

run: all
	./build/lexer tests/input1.c

clean:
	rm -rf build *.exe
