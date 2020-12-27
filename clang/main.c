#include "mandelbrot.h"

#include <argp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


enum parameter_keys {
  WIDTH = 'w',
  ITERATIONS = 'i',
  THREADS = 't',
  VERBOSE = 'v',
  SUPERSAMPLING = 's',
  PROGRESS = 'p',
  XMIN_KEY = 0x00100000,
  XMAX_KEY = 0x00100001,
  YMIN_KEY = 0x00100002,
  YMAX_KEY = 0x00100003,
};

const char * argp_program_version = "mandelbrot v0.1";
static char doc [] = "Draw the Mandelbrot set a selected region.";

static char args_doc [] = "IMAGE.png";

static struct argp_option options [] = {
  {"width", WIDTH, "WIDTH", 0, "Set output image width in pixels [default: 300]", -1},
  {"iterations", ITERATIONS, "N", 0, "Number of iterations per pixel [default: 100]", -1},
  {"supersampling", SUPERSAMPLING, 0, 0, "Sample with a factor 2x2 [default: no]", -1},
  {"threads", THREADS, "NTHREADS", 0, "Set number of threads", -1},
  {"progress", PROGRESS, 0, 0, "Show progress [default: no]", -1},
  {"xmin", XMIN_KEY, "F", 0, "Minimum X [default: -2.5]", -1},
  {"xmax", XMAX_KEY, "F", 0, "Maximum X [default:  1.0]", -1},
  {"ymin", YMIN_KEY, "F", 0, "Minimum Y [default: -1.0]", -1},
  {"ymax", YMAX_KEY, "F", 0, "Maximum Y [default:  1.0]", -1},
  {"verbose", VERBOSE, 0, 0, "Print more", -1},
  { 0 },
};

static error_t parse_opt(int32_t key, char * arg, struct argp_state * state) {
  args_t *args = state->input;

  switch (key) {
    case WIDTH:
      args->width = atoi(arg);
      if (args->width < 16) {
        critical("Provide an integer to --width higher or equal to 16\n");
        argp_usage(state);
      }
      break;

    case ITERATIONS:
      args->iterations = atoi(arg);
      if (args->iterations < 1) {
        critical("Provide an integer to --iterations higher or equal to 1\n");
        argp_usage(state);
      }
      break;

    case THREADS:
      args->threads = atoi(arg);
      if (args->threads < 1) {
        critical("Provide an integer to --threads higher or equal to 1\n");
        argp_usage(state);
      }
      break;

    case SUPERSAMPLING:
      args->supersampling = 1;
      break;

    case PROGRESS:
      args->progress = 1;
      break;

    case VERBOSE:
      args->verbose = 1;
      break;

    case XMIN_KEY:
      args->x_min = strtod(arg, NULL);
      break;

    case XMAX_KEY:
      args->x_max = strtod(arg, NULL);
      break;

    case YMIN_KEY:
      args->y_min = strtod(arg, NULL);
      break;

    case YMAX_KEY:
      args->y_max = strtod(arg, NULL);
      break;

    case ARGP_KEY_ARG:
      if (state->arg_num >= 1) {
        // Only one filename should be provided
        argp_usage(state);
      }
      args->filename = arg;
      break;

    case ARGP_KEY_END:
      if (state->arg_num != 1) {
        // Provide exactly one output image file
        argp_usage(state);
      }
      break;

    default:
      return ARGP_ERR_UNKNOWN;
    }

  return 0;
}



int main(int argc, char ** argv) {
  // Default argument values
  args_t arguments;
  arguments.width = 300;
  arguments.iterations = 100;
  arguments.threads = 1;
  arguments.supersampling = 0;
  arguments.progress = 0;
  arguments.verbose = 0;
  arguments.x_min = -2.5;
  arguments.x_max = 1.0;
  arguments.y_min = -1.0;
  arguments.y_max = 1.0;
  arguments.filename = NULL;

  // Parse arguments
  static struct argp argp = { options, parse_opt, args_doc, doc, NULL, NULL, NULL };
  argp_parse(&argp, argc, argv, 0, 0, &arguments);

  // Mandelbrot calculation
  mandelbrot_init(&arguments);
  image_t * img = mandelbrot_calculate();

  /*
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      printf("%3d", image_get_pixel(img, x, y).i32);
    }
    printf("\n");
  }
  */

  // Scale down to half resolution (1/4 pixels if supersampling was enabled)
  if (arguments.supersampling) {
    image_t * old_img = img;
    img = image_downscale(old_img);
    image_destroy(old_img);
  }

  // Convert to RGB
  {
    image_t * old_img = img;
    img = image_hsv_to_rgb(old_img);
    image_destroy(old_img);
  }

  // Write PNG
  image_write_png(img, arguments.filename);

  return 0;
}
