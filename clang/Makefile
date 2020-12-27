CC = clang
CFLAGS = -g -Wall -Wextra -pedantic -std=c11 -O2 -DDEBUG

.PHONY: clean
clean:
	rm -rf ./mandelbrot

compile: *.c
	$(CC) $(CFLAGS) -o mandelbrot main.c image.c colors.c utils.c

run: mandelbrot
	./mandelbrot

all: compile run
