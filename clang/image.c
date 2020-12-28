#include <stdio.h>
#include <math.h>
#include <png.h>

#include "image.h"
#include "utils.h"


union pixel image_get_pixel(
    const image_t * img,
    const int32_t x,
    const int32_t y
  )
{
  const int32_t w = img->width;
  return img->pixels[y * w + x];
}


void image_set_pixel(
    image_t * img,
    const int32_t x,
    const int32_t y,
    union pixel pixel
  )
{
  const int32_t w = img->width;
  img->pixels[y * w + x] = pixel;
}


image_t * image_new(
    const int32_t width,
    const int32_t height,
    const int32_t mode
  )
{
  size_t size;
  size = sizeof(image_t);
  size = size + width * height * sizeof(union pixel);

  image_t * img = mem_alloc(size);

  img->width = width;
  img->height = height;
  img->mode = mode;

  return img;
}


void image_destroy(image_t * img)
{
  free(img);
}


hsv_t _downscale_average(hsv_t * hsv, int32_t n);


image_t * image_downscale(const image_t * img)
{
  if (img == NULL) {
    critical("image_downscale() received NULL\n");
    return NULL;
  }

  int32_t width = img->width / 2;
  int32_t height = img->height / 2;

  image_t * out = image_new(width, height, IMAGE_MODE_HSV);

  hsv_t hsv[4];
  union pixel px;

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      hsv[0] = image_get_pixel(img, x * 2 + 0, y * 2 + 0).hsv;
      hsv[1] = image_get_pixel(img, x * 2 + 1, y * 2 + 0).hsv;
      hsv[2] = image_get_pixel(img, x * 2 + 0, y * 2 + 1).hsv;
      hsv[3] = image_get_pixel(img, x * 2 + 1, y * 2 + 1).hsv;

      px.hsv = _downscale_average(&hsv[0], 4);

      image_set_pixel(out, x, y, px);
    }
  }

  return (image_t * ) out;
}


hsv_t _downscale_average(hsv_t * hsv, int32_t n)
{
  const double f = 1 / (double) n;
  double h = 0.0, s = 0.0, v = 0.0;

  for (int32_t i = 0; i < n; i++)
  {
    h += hsv[i].h * f;
    s += hsv[i].s * f;
    v += hsv[i].v * f;
  }

  hsv_t out = { round(h), round(s), round(v), 0 };

  return out;
}



image_t * image_hsv_to_rgb(const image_t * img)
{
  if (img == NULL) {
    critical("image_hsv_to_rgb() received NULL\n");
    return (image_t * ) NULL;
  }

  int32_t width = img->width;
  int32_t height = img->height;

  image_t * out = image_new(width, height, IMAGE_MODE_RGB);
  hsv_t hsv;
  union pixel px;

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      hsv = image_get_pixel(img, x, y).hsv;
      px.rgb = hsv_to_rgb(hsv);
      image_set_pixel(out, x, y, px);
    }
  }

  return out;
}


int32_t image_write_png(const image_t * img, const char * filename) {
  if (img == NULL) {
    critical("image_write_png() received NULL image\n");
    return 1;
  }

  if (filename == NULL) {
    critical("image_write_png() received NULL filename\n");
    return 1;
  }

  int status = -1;

  FILE *fp;

  png_structp png_ptr = NULL;
  png_infop info_ptr = NULL;

  png_byte ** row_pointers = NULL;

  size_t width = (size_t) img->width;
  size_t height = (size_t) img->height;

  size_t x, y;

  int32_t pixel_size = 3;
  int32_t depth = 8;

  fp = fopen(filename, "wb");
  if (!fp) {
    critical("failed to open file '%s' in write mode\n", filename);
    return 2;
  }

  png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (png_ptr == NULL) {
      goto png_create_write_struct_failed;
  }

  info_ptr = png_create_info_struct(png_ptr);
  if (info_ptr == NULL) {
      goto png_create_info_struct_failed;
  }

  // Set up error handling for PNG
  if (setjmp(png_jmpbuf(png_ptr))) {
    goto png_failure;
  }

  // Set image attributes
  png_set_IHDR(
    png_ptr,
    info_ptr,
    width,
    height,
    depth,
    PNG_COLOR_TYPE_RGB,
    PNG_INTERLACE_NONE,
    PNG_COMPRESSION_TYPE_DEFAULT,
    PNG_FILTER_TYPE_DEFAULT
  );

  // Fill data
  row_pointers = png_malloc(png_ptr, height * sizeof (png_byte *));
  for (y = 0; y < height; y++) {
    png_byte *row = png_malloc(png_ptr, sizeof (uint8_t) * width * pixel_size);
    row_pointers[y] = row;
    for (x = 0; x < width; x++) {
      rgb_t px = image_get_pixel(img, x, y).rgb;
      *row++ = px.r;
      *row++ = px.g;
      *row++ = px.b;
    }
  }

  // Write the image data to fp
  png_init_io (png_ptr, fp);
  png_set_rows (png_ptr, info_ptr, row_pointers);
  png_write_png (png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

  status = 0;

  // Close fp and clean up stuff

png_failure:
png_create_info_struct_failed:
  png_destroy_write_struct (&png_ptr, &info_ptr);
png_create_write_struct_failed:
  fclose (fp);

  return status;
}
