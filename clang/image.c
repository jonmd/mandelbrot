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
    const union pixel pixel
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


image_t * image_downscale(const image_t * img)
{
  if (img == NULL) {
    critical("image_downscale() received NULL\n");
  } else {
    critical("image_downscale() not implemented\n");
  }

  return (image_t * ) NULL;
}


int32_t image_write_png(const image_t * img) {
  if (img == NULL) {
    critical("image_write_png() received NULL\n");
  } else {
    critical("image_write_png() not implemented (img.width = %d)\n", img->width);
  }

  return 0;
}
