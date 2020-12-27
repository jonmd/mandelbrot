#include "mandelbrot.h"

#include <stdio.h>


int main(const int argc, const char ** argv) {
  printf("argc = %d\n", argc);
  for (int32_t i = 0; i < argc; i++) {
    printf("argv[%d] = %s\n", i, argv[i]);
  }

  size_t size = sizeof(union pixel);
  printf("main.c %zu\n", size);

  return 0;
}
