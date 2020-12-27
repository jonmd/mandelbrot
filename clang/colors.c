#include "colors.h"


rgba hsv_to_rgb(const hsva hsv) {
  rgba rgb;

  rgb.r = hsv.h;

  return rgb;
}


hsva rgb_to_hsv(const rgba rgb) {
  hsva hsv;

  hsv.h = rgb.r;

  return hsv;
}
