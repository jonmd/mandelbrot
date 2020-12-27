#include "colors.h"

/*
 *
 * RGB-HSV and HSV-RGB implementations from StackOverflow:
 *
 * https://stackoverflow.com/questions/3018313/algorithm-to-convert-rgb-to-hsv-and-hsv-to-rgb-in-range-0-255-for-both
 *
 */


#define BYTE (255.0)


rgb_t hsv_to_rgb(const hsv_t hsv) {
  hsv_norm_t in = norm_hsv(hsv);
  rgb_norm_t out;

  double hh, p, q, t, ff;
  long i;

  if (in.s <= 0.0) {       // < is bogus, just shuts up warnings
    out.r = in.v;
    out.g = in.v;
    out.b = in.v;
    return quant_rgb(out);
  }

  hh = in.h;
  if (hh >= 360.0) {
    hh = 0.0;
  }
  hh /= 60.0;

  i = (long) hh;
  ff = hh - i;

  p = in.v * (1.0 - in.s);
  q = in.v * (1.0 - (in.s * ff));
  t = in.v * (1.0 - (in.s * (1.0 - ff)));

  switch (i) {
    case 0:
      out.r = in.v;
      out.g = t;
      out.b = p;
      break;
    case 1:
      out.r = q;
      out.g = in.v;
      out.b = p;
      break;
    case 2:
      out.r = p;
      out.g = in.v;
      out.b = t;
      break;
    case 3:
      out.r = p;
      out.g = q;
      out.b = in.v;
      break;
    case 4:
      out.r = t;
      out.g = p;
      out.b = in.v;
      break;
    case 5:
    default:
      out.r = in.v;
      out.g = p;
      out.b = q;
      break;
  }

  return quant_rgb(out);
}


hsv_t rgb_to_hsv(const rgb_t rgb) {
  rgb_norm_t in = norm_rgb(hsv);
  hsv_norm_t out;

  double min, max, delta;

  min = in.r < in.g ? in.r : in.g;
  min = min  < in.b ? min  : in.b;

  max = in.r > in.g ? in.r : in.g;
  max = max  > in.b ? max  : in.b;

  out.v = max;
  delta = max - min;
  if (delta < 0.00001)
  {
      out.s = 0.0;
      out.h = 0.0; // undefined, maybe nan?
      return quant_hsv(out);
  }

  if (max > 0.0 ) { // NOTE: if Max is == 0, this divide would cause a crash
      out.s = (delta / max);                  // s
  } else {
      // if max is 0, then r = g = b = 0
      // s = 0, h is undefined
      out.s = 0.0;
      out.h = 0.0; // undefined, maybe nan?
      return quant_hsv(out);
  }

  if (in.r >= max) {                           // > is bogus, just keeps compiler happy
      out.h = ( in.g - in.b ) / delta;        // between yellow & magenta
  } else if (in.g >= max) {
      out.h = 2.0 + ( in.b - in.r ) / delta;  // between cyan & yellow
  } else {
      out.h = 4.0 + ( in.r - in.g ) / delta;  // between magenta & cyan
  }

  out.h *= 60.0;                              // degrees

  if (out.h < 0.0) {
      out.h += 360.0;
  }

  return quant_hsv(out);
}


extern rgb_norm_t norm_rgb(const rgb_t in)
{
  rgb_norm_t out;
  out.r = ((double) in.r) / BYTE;
  out.g = ((double) in.g) / BYTE;
  out.b = ((double) in.b) / BYTE;
  return out;
}


extern rgb_t quant_rgb(const rgb_norm_t in)
{
  rgb_t out;
  out.r = (uint8_t) round(in.r * BYTE);
  out.g = (uint8_t) round(in.g * BYTE);
  out.b = (uint8_t) round(in.b * BYTE);
  return out;
}


extern hsv_norm_t norm_hsv(const hsv_t in)
{
  hsv_norm_t out;
  out.h = (360.0 * (double) in.h) / BYTE;
  out.s = ((double) in.s) / BYTE;
  out.v = ((double) in.v) / BYTE;
  return out;
}


extern hsv_t quant_hsv(const hsv_norm_t in)
{
  hsv_t out;
  out.h = (uint8_t) round(in.h * BYTE / 360.0);
  out.s = (uint8_t) round(in.s * BYTE);
  out.v = (uint8_t) round(in.v * BYTE);
  return out;
}
