#pragma once

#include <stdint.h>
#include "colors.h"


struct image {
    int32_t width;
    int32_t height;
    union pixel pixels[]; // Hack! Will allocate the size of the pixels
};



extern union pixel image_get_pixel(const struct image * img, const int32_t x, const int32_t y);


extern void image_set_pixel(struct image * img, const int32_t x, const int32_t y, const union pixel pixel);


extern struct image * image_new(const int32_t width, const int32_t height);


extern void image_destroy(struct image * img);


extern struct image * image_downscale(const struct image * img);
