#pragma once

#include <stdint.h>
#include "colors.h"


enum image_mode {
    IMAGE_MODE_GREYSCALE = 0,
    IMAGE_MODE_RGB = 1,
    IMAGE_MODE_HSV = 2,
};


typedef struct {
    int32_t width;
    int32_t height;
    int32_t mode;
    union pixel pixels[];  // Will allocate the size of the pixels
} image_t;



extern union pixel image_get_pixel(const image_t * img, const int32_t x, const int32_t y);


extern void image_set_pixel(image_t * img, const int32_t x, const int32_t y, const union pixel pixel);


extern image_t * image_new(const int32_t width, const int32_t height, const int32_t mode);


extern void image_destroy(image_t * img);


extern image_t * image_downscale(const image_t * img);


extern int32_t image_write_png(const image_t * img);
