#include <math.h>

#include "colors.h"
#include "utils.h"

/*
 *
 * RGB-HSV and HSV-RGB implementations from StackOverflow:
 *
 * https://stackoverflow.com/questions/3018313/algorithm-to-convert-rgb-to-hsv-and-hsv-to-rgb-in-range-0-255-for-both
 *
 */


typedef struct {
    double r; // 0.0 - 1.0
    double g; // 0.0 - 1.0
    double b; // 0.0 - 1.0
} rgb_norm_t;

typedef struct {
    double h; // 0.0 - 360.0
    double s; // 0.0 - 1.0
    double v; // 0.0 - 1.0
} hsv_norm_t;

rgb_norm_t norm_rgb(const rgb_t in);
rgb_t quant_rgb(const rgb_norm_t in);
hsv_norm_t norm_hsv(const hsv_t in);
hsv_t quant_hsv(const hsv_norm_t in);


#define BYTE (255.0)

const rgb_t RGB_DARK_BLUE = { 0, 0, 0x1f, 0 };
const rgb_t RGB_BLACK = { 0, 0, 0, 0 };
const rgb_t RGB_COLORS [] = {
  { 0x17, 0x35, 0x5f, 0x00 },
  { 0x20, 0x63, 0x9B, 0x00 },
  { 0x3C, 0xAE, 0xA3, 0x00 },
  { 0xF6, 0xD5, 0x5C, 0x00 },
  { 0xED, 0x55, 0x3B, 0x00 },
  { 0xBF, 0xA0, 0xFF, 0x00 },
};
const int32_t RGB_COLORS_COUNT = 6;


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
  rgb_norm_t in = norm_rgb(rgb);
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


hsv_t hsv_lerp(const hsv_t hsv1, const hsv_t hsv2, double factor)
{
  hsv_t out;

  out.h = (uint8_t) round(hsv2.h * factor + hsv1.h * (1.0 - factor));
  out.s = (uint8_t) round(hsv2.s * factor + hsv1.s * (1.0 - factor));
  out.v = (uint8_t) round(hsv2.v * factor + hsv1.v * (1.0 - factor));

  return out;
}


rgb_norm_t norm_rgb(const rgb_t in)
{
  rgb_norm_t out;
  out.r = ((double) in.r) / BYTE;
  out.g = ((double) in.g) / BYTE;
  out.b = ((double) in.b) / BYTE;
  return out;
}


rgb_t quant_rgb(const rgb_norm_t in)
{
  rgb_t out;
  out.r = (uint8_t) round(in.r * BYTE);
  out.g = (uint8_t) round(in.g * BYTE);
  out.b = (uint8_t) round(in.b * BYTE);
  return out;
}


hsv_norm_t norm_hsv(const hsv_t in)
{
  hsv_norm_t out;
  out.h = (360.0 * (double) in.h) / BYTE;
  out.s = ((double) in.s) / BYTE;
  out.v = ((double) in.v) / BYTE;
  return out;
}


hsv_t quant_hsv(const hsv_norm_t in)
{
  hsv_t out;
  out.h = (uint8_t) round(in.h * BYTE / 360.0);
  out.s = (uint8_t) round(in.s * BYTE);
  out.v = (uint8_t) round(in.v * BYTE);
  return out;
}


typedef struct {
  int32_t index;
  rgb_t color;
} color_step_t;

typedef struct {
  int32_t i0;
  int32_t i1;
  hsv_t c0;
  hsv_t c1;
} color_range_t;

color_range_t * color_gradient;
int32_t color_gradient_size = 0;
int32_t n_iterations = 0;

void _c_create_gradient(int32_t iterations);
void _c_prepare_gradients(color_step_t * steps, int32_t n_steps);


void colorize_init(int32_t iterations)
{
  n_iterations = iterations;
  _c_create_gradient(iterations);
}


hsv_t colorize(int32_t value)
{
  int32_t i_min = 0;
  int32_t i_max = color_gradient_size;
  int32_t i;

  double factor;
  hsv_t hsv;

  color_range_t * r;

  while (true)
  {
    i = floor((i_min + i_max) / 2.0);
    r = &color_gradient[i];

    if (r->i0 <= value && value < r->i1) {
      factor = (value - ((double) r->i0)) / (r->i1 - ((double) r->i0));
      hsv = hsv_lerp(r->c0, r->c1, factor);
      return hsv;
    } else if (value < r->i0) {
      i_max = i;
    } else if (value >= r->i1) {
      i_min = i;
    }
  }

  if (i == n_iterations) {
    return rgb_to_hsv(RGB_BLACK);
  } else {
    return rgb_to_hsv(RGB_DARK_BLUE);
  }
}


void _c_create_gradient(int32_t iterations)
{
  // Allocate for steps
  int32_t n_steps = 2 * iterations / 50 + 3;
  int32_t s = 0, i = 0, c = 0;
  int32_t i0, i1;
  rgb_t color;

  color_step_t * steps = (color_step_t *) mem_alloc(
    sizeof(color_step_t) * n_steps
  );

  color_step_t cs = { 0, RGB_DARK_BLUE };
  steps[s++] = cs;

  while (i + 51 < iterations)
  {
    i0 = i + 35;
    i1 = i + 50;
    color = RGB_COLORS[c];
    c = (c + 1) % RGB_COLORS_COUNT;
    color_step_t cs1 = { i0, color };
    steps[s++] = cs1;
    color_step_t cs2 = { i1, color };
    steps[s++] = cs2;
    i = i + 50;
  }

  color_step_t cs_e0 = { iterations - 1, RGB_BLACK };
  steps[s++] = cs_e0;
  color_step_t cs_e1 = { iterations + 1, RGB_BLACK };
  steps[s++] = cs_e1;

  _c_prepare_gradients(steps, n_steps);

  mem_free(steps);
}


void _c_prepare_gradients(color_step_t * steps, int32_t n_steps)
{
  int32_t i;
  color_step_t * prev;
  color_step_t * curr;

  color_gradient_size = n_steps - 1;
  color_gradient = (color_range_t *) mem_alloc(
    sizeof(color_range_t) * color_gradient_size
  );

  prev = &steps[0];
  for (i = 1; i < n_steps; i++)
  {
    curr = &steps[i];

    color_range_t range = {
      .i0 = prev->index,
      .i1 = curr->index,
      .c0 = rgb_to_hsv(prev->color),
      .c1 = rgb_to_hsv(curr->color),
    };
    color_gradient[i - 1] = range;

    prev = curr;
  }

  for (i = 0; i < color_gradient_size; i++)
  {
    color_range_t * range = &color_gradient[i];
    hsv_t c0 = range->c0;
    hsv_t c1 = range->c1;
    printf(
      "[%3d]  %3d – %3d  |  c0 = %3d,%3d,%3d  |  c1 = %3d,%3d,%3d\n",
      i, range->i0, range->i1,
      c0.h, c0.s, c0.v,
      c1.h, c1.s, c1.v
    );
  }

}
