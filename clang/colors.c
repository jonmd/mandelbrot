#include "colors.h"


#define BYTE_MAX 255


rgb_t hsv_to_rgb(const hsv_t hsv) {
  rgb_t rgb;

  rgb.r = hsv.h;

  return rgb;
}


hsv_t rgb_to_hsv(const rgb_t rgb) {
  hsv_t hsv;

  hsv.h = rgb.r;

  return hsv;
}
