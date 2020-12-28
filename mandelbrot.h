#pragma once

// Required for nanosleep to work
#define _POSIX_C_SOURCE 199309L

#include <math.h>
#include <pthread.h>
#include <time.h>

#include "colors.h"
#include "image.h"
#include "utils.h"


typedef struct {
  int32_t width;
  int32_t iterations;
  int32_t threads;
  int32_t supersampling;
  int32_t progress;
  int32_t verbose;
  double x_min;
  double x_max;
  double y_min;
  double y_max;
  char * filename;
} args_t;


extern double x_min;
extern double x_max;
extern double y_min;
extern double y_max;

extern double x_range;
extern double y_range;

extern int32_t width;
extern int32_t height;

extern int32_t n_iterations;
extern int32_t n_threads;

extern int32_t supersampling;
extern int32_t show_progress;


extern void mandelbrot_init(const args_t * args);

extern image_t * mandelbrot_calculate();
