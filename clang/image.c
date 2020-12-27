#include "image.h"
#include "utils.h"


union pixel image_get_pixel(
    const struct image * img,
    const int32_t x,
    const int32_t y
  )
{
  const int32_t w = img->width;
  return img->pixels[x * w + y];
}


void image_set_pixel(
    struct image * img,
    const int32_t x,
    const int32_t y,
    const union pixel pixel
  )
{
  const int32_t w = img->width;
  img->pixels[x * w + y] = pixel;
}


struct image * image_new(
    const int32_t width,
    const int32_t height
  )
{
  size_t size;
  size = sizeof(struct image);
  size = size + width * height * sizeof(union pixel);

  struct image * img = mem_alloc(size);

  return img;
}


void image_destroy(struct image * img)
{
  free(img);
}


struct image * image_downscale(const struct image * img)
{
  if (img == NULL) {
    critical("image_downscale() received NULL\n");
  } else {
    critical("image_downscale() not implemented\n");
  }

  return (struct image * ) NULL;
}
