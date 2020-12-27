#pragma once

#include <stdint.h>


struct rgba_st {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} __attribute__((packed));

typedef struct rgba_st rgb_t;


struct hsva_st {
    uint8_t h;
    uint8_t s;
    uint8_t v;
    uint8_t a;
} __attribute__((packed));

typedef struct hsva_st hsv_t;


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
