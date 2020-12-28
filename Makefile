CC = clang
CFLAGS = -g -Wall -Wextra -pedantic -std=c11 -O2 -DINFO -lm -lpthread -lpng

.PHONY: clean

compile: *.c
	$(CC) $(CFLAGS) -o mandelbrot main.c mandelbrot.c image.c colors.c utils.c

run: mandelbrot
	./mandelbrot

clean:
	rm -rf ./mandelbrot

all: compile
