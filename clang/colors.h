#pragma once

#include <stdint.h>


struct rgba_st {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} __attribute__((packed));

typedef struct rgba_st rgba;


struct hsva_st {
    uint8_t h;
    uint8_t s;
    uint8_t v;
    uint8_t a;
} __attribute__((packed));

typedef struct hsva_st hsva;


union pixel {
    rgba rgb;
    rgba hsv;
    int32_t i32;
};


extern rgba hsv_to_rgb(const hsva hsv);


extern hsva rgb_to_hsv(const rgba rgb);
