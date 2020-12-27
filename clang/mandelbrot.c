#include "mandelbrot.h"


double x_min = -2.5;
double x_max =  1.0;
double y_min = -1.0;
double y_max =  1.0;

double x_range;
double y_range;

int32_t width;
int32_t height;

int32_t n_iterations;
int32_t n_threads;

int32_t supersampling = 0;
int32_t show_progress;


void mandelbrot_init(const args_t * args)
{
  x_min = args->x_min;
  x_max = args->x_max;
  y_min = args->y_min;
  y_max = args->y_max;

  x_range = x_max - x_min;
  y_range = y_max - y_min;

  width = args->width;
  height = round(
    ((double) width) * ((double) y_range / (double) x_range)
  );

  n_iterations = args->iterations;
  n_threads = args->threads;

  show_progress = args->progress;

  supersampling = args->supersampling;

  // Print arguments
  if (args->verbose) {
    printf("[mandelbrot][init] width = %dpx\n", width);
    printf("[mandelbrot][init] height = %dpx\n", height);
    printf("[mandelbrot][init] supersampling = %d\n", supersampling);
    printf("[mandelbrot][init] iterations = %d\n", n_iterations);
    printf("[mandelbrot][init] threads = %d\n", n_threads);
    printf("[mandelbrot][init] x_min = %15.12f\n", x_min);
    printf("[mandelbrot][init] x_max = %15.12f\n", x_max);
    printf("[mandelbrot][init] y_min = %15.12f\n", y_min);
    printf("[mandelbrot][init] y_max = %15.12f\n", y_max);
    printf("[mandelbrot][init] x_range = %15.12f\n", x_range);
    printf("[mandelbrot][init] y_range = %15.12f\n", y_range);
    printf("[mandelbrot][init] filename = %s\n", args->filename);
  }

  if (supersampling) {
    width = width * 2;
    height = height * 2;
  }
}


// The next image row
image_t * img;
int32_t img_next_row;
pthread_mutex_t lock;


int32_t get_next_row()
{
  int32_t row = -1;
  pthread_mutex_lock(&lock);

  if (img_next_row < height) {
    row = img_next_row++;
  }

  pthread_mutex_unlock(&lock);
  return row;
}


int32_t get_progress()
{
  int32_t row = 0;

  pthread_mutex_lock(&lock);

  row = img_next_row;

  pthread_mutex_unlock(&lock);

  return row;
}

void * mandelbrot_thread(void * ptr);
void * mandelbrot_progress_thread(void * ptr);


image_t * mandelbrot_calculate()
{
  // Create a new image
  image_t * working_image = image_new(width, height, IMAGE_MODE_HSV);
  img = working_image;

  // Init lock
  if (pthread_mutex_init(&lock, NULL) != 0) {
    critical("Failed to initialize mutex\n");
    exit(1);
  }

  // Create threads
  pthread_t progress;
  pthread_t workers[n_threads];
  int32_t worker_ids[n_threads];

  if (show_progress) {
    pthread_create(&progress, NULL, mandelbrot_progress_thread, NULL);
  }

  for (int32_t i = 0; i < n_threads; i++) {
    worker_ids[i] = i;
    pthread_create(&(workers[i]), NULL, mandelbrot_thread, NULL);
  }

  for (int32_t i = 0; i < n_threads; i++) {
    pthread_join(workers[i], NULL);
  }

  if (show_progress) {
    pthread_join(progress, NULL);
  }

  pthread_mutex_destroy(&lock);
  img = NULL;

  return working_image;
}


void * mandelbrot_progress_thread()
{
  struct timespec time;
  time.tv_sec = 0;
  time.tv_nsec = 100000000;

  char a = '.';
  char b = '#';

  int32_t row = 0;
  int32_t percentage = 0;
  int32_t progress = 0;

  printf("\n");
  for (int i = 0; i < 25; i++) {
    printf("%c", a);
  }

  while (percentage < 100)
  {
    row = get_progress();
    percentage = round((double) row * 100.0) / ((double) height);
    progress = round(((double) percentage) / 4.0);

    printf("\r");
    for (int i = 0; i < 25; i++) {
      printf("%c", i <= progress ? b : a);
    }
    printf(" %3d %% complete ", percentage);

    if (percentage == 100) {
      printf("\n\n");
    }

    fflush(stdout);

    nanosleep(&time, NULL);
  }

  return NULL;
}



double px_to_coordinate(const int32_t px);
double py_to_coordinate(const int32_t py);
int32_t m_solve(const double cx, const double cy);
int32_t m_process_pixel(const int32_t px, const int32_t py);


void * mandelbrot_thread()
{
  int32_t py;
  int32_t px;
  int32_t i;

  while ((py = get_next_row()) >= 0)
  {
    i = 0;
    for (px = 0; px < width; px++)
    {
      int32_t i_current = m_process_pixel(px, py);
      if (i_current > i) {
        i = i_current;
      }
    }
    debug("py = %d, max[i] = %d\n", py, i);
  }

  return NULL;
}


double px_to_coordinate(const int32_t px)
{
  const double p = ((double) px) + 0.5;
  return x_min + (p / ((double) width)) * x_range;
}


double py_to_coordinate(const int32_t py)
{
  const double h = (double) height;
  const double p = ((double) py) + 0.5;
  return y_min + ((h - p) / h) * y_range;
}


int32_t m_solve(const double cx, const double cy)
{
  const int32_t max = n_iterations;
  double x2, y2, x_temp;

  // Initial z = (0, 0)
  double x = 0;
  double y = 0;

  for (int32_t i = 0; i < max; i++)
  {
    // Check if outside radius of two
    x2 = x * x;
    y2 = y * y;
    if ((x2 + y2) > 4)
    {
      return i;
    }
    // Not outside – go to next value
    x_temp = x2 - y2 + cx;
    y = 2 * x * y + cy;
    x = x_temp;
  }

  return max;
}


int32_t m_process_pixel(const int32_t px, const int32_t py)
{
  // Convert from pixel to domain
  double x = px_to_coordinate(px);
  double y = py_to_coordinate(py);
  // "Solve" Mandelbrot for point
  int32_t iterations = m_solve(x, y);
  debug("p[%d, %d] = C[%.8f, %.8f] = %d\n", px, py, x, y, iterations);
  // Set pixel iteration color
  img->pixels[py * width + px].i32 = iterations;
  // Return iterations
  return iterations;
}
