#ifndef MATH_UTILS_H
#define MATH_UTILS_H

/* Define PI with enough digits */
#define PI 3.14159265358979323846

/* Convert degrees to radians */
static inline float
deg_to_rad (float deg)
{
  return deg * (PI / 180.0f);
}

/* Convert radians to degrees */
static inline float
rad_to_deg (float rad)
{
  return rad * (180.0f / PI);
}

/* Get absolute value of a float */
static inline float
math_absf (float x)
{
  return (x < 0.0f) ? -x : x;
}

/* Get absolute value of an int */
static inline int
math_absi (int x)
{
  return (x < 0) ? -x : x;
}

/* Get the sign of a float (-1, 0, or 1) */
static inline float
math_signf (float x)
{
  return (x > 0.0f) - (x < 0.0f);
}

/* Get the sign of an int (-1, 0, or 1) */
static inline int
math_signi (int x)
{
  return (x > 0) - (x < 0);
}

/* Round float to nearest int */
static inline int
math_roundf_to_int (float x)
{
  if (x >= 0.0f)
    {
      return (int)(x + 0.5f);
    }
  else
    {
      return (int)(x - 0.5f);
    }
}

/* Round float to nearest float value */
static inline float
math_roundf_to_float (float x)
{
  return (float)math_roundf_to_int (x);
}

/* Floor float to int (round down) */
static inline int
math_floorf_to_int (float x)
{
  int i = (int)x;
  return (x < 0 && x != i) ? (i - 1) : i;
}

/* Ceil float to int (round up) */
static inline int
math_ceilf_to_int (float x)
{
  int i = (int)x;
  return (x > 0 && x != i) ? (i + 1) : i;
}

/* Floor float to float */
static inline float
math_floorf_to_float (float x)
{
  return (float)math_floorf_to_int (x);
}

/* Ceil float to float */
static inline float
math_ceilf_to_float (float x)
{
  return (float)math_ceilf_to_int (x);
}

static inline float
fmin2f (float a, float b)
{
  return (a < b) ? a : b;
}

static inline float
fmax2f (float a, float b)
{
  return (a > b) ? a : b;
}

static inline float
fmin3f (float a, float b, float c)
{
  float t = (a < b) ? a : b;
  return (t < c) ? t : c;
}

static inline float
fmax3f (float a, float b, float c)
{
  float t = (a > b) ? a : b;
  return (t > c) ? t : c;
}

static inline int
min2i (int a, int b) {
  return (a < b) ? a : b;
}

static inline int
max2i (int a, int b) {
  return (a > b) ? a : b;
}

static inline int
min3i (int a, int b, int c)
{
  int t = (a < b) ? a : b;
  return (t < c) ? t : c;
}

static inline int
max3i (int a, int b, int c)
{
  int t = (a > b) ? a : b;
  return (t > c) ? t : c;
}

/* Clamp float x between min and max */
static inline float
clampf (float x, float min, float max)
{
  if (x < min)
    return min;
  if (x > max)
    return max;
  return x;
}

/* Clamp int x between min and max */
static inline int
clampi (int x, int min, int max)
{
  if (x < min)
    return min;
  if (x > max)
    return max;
  return x;
}

/* Linear interpolation between a and b by t (float) */
static inline float
lerpf (float a, float b, float t)
{
  return a + t * (b - a);
}

/* Linear interpolation between a and b by t (int) */
static inline int
lerpi (int a, int b, float t)
{
  return a + t * (b - a);
}

#endif /* MATH_UTILS_H */
