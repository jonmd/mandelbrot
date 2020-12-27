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


typedef struct {
    double r; // 0 - 1
    double g; // 0 - 1
    double b; // 0 - 1
} rgb_norm_t;


typedef struct {
    double h; // 0 - 360
    double s; // 0 - 1
    double v; // 0 - 1
} hsv_norm_t;


union pixel {
    rgb_t rgb;
    hsv_t hsv;
    uint8_t grey;
    int32_t i32;
};


extern rgb_t hsv_to_rgb(const hsv_t hsv);


extern hsv_t rgb_to_hsv(const rgb_t rgb);


extern rgb_norm_t norm_rgb(const rgb_t in);


extern rgb_t quant_rgb(const rgb_norm_t in);


extern hsv_norm_t norm_hsv(const hsv_t in);


extern hsv_t quant_hsv(const hsv_norm_t in);
