#pragma once

#include <math.h>
#include <stdint.h>

#include "utils.h"


typedef struct __attribute__((packed)) {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} rgb_t;


typedef struct __attribute__((packed)) {
    uint8_t h;
    uint8_t s;
    uint8_t v;
    uint8_t a;
} hsv_t;


union pixel {
    rgb_t rgb;
    hsv_t hsv;
    uint8_t grey;
    int32_t i32;
};


extern void colorize_init(const int32_t iterations);

extern hsv_t colorize(const int32_t iterations);

extern rgb_t hsv_to_rgb(const hsv_t hsv);

extern hsv_t rgb_to_hsv(const rgb_t rgb);

extern hsv_t hsv_lerp(const hsv_t hsv1, const hsv_t hsv2, double factor);
