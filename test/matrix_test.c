#include "matrix_test.h"
#include "math/matrix.h"
#include <math.h>
#include <stdio.h>

static bool
float_eq (float a, float b, float eps)
{
  float diff = fabs (a - b);
  if (diff < eps)
    return true;

  float largest = (fabs (a) > fabs (b)) ? fabs (a) : fabs (b);
  return diff / largest < eps;
}

static bool
mat_eq (const float *a, const float *b, int size, float eps, const char *name)
{
  for (int i = 0; i < size; i++)
    {
      if (!float_eq (a[i], b[i], eps))
        {
          printf ("%s failed\n", name);
          return false;
        }
    }
  return true;
}

[[maybe_unused]]
static bool
vec2f_eq (const Vec2f_t *a, const Vec2f_t *b, float eps, const char *name)
{
  if (float_eq (a->x, b->x, eps) && float_eq (a->y, b->y, eps))
    {
      return true;
    }
  printf ("%s failed\n", name);
  return false;
}

[[maybe_unused]]
static bool
vec3f_eq (const Vec3f_t *a, const Vec3f_t *b, float eps, const char *name)
{
  if (float_eq (a->x, b->x, eps) && float_eq (a->y, b->y, eps)
      && float_eq (a->z, b->z, eps))
    {
      return true;
    }
  printf ("%s failed\n", name);
  return false;
}

[[maybe_unused]]
static bool
vec4f_eq (const Vec4f_t *a, const Vec4f_t *b, float eps, const char *name)
{
  if (float_eq (a->x, b->x, eps) && float_eq (a->y, b->y, eps)
      && float_eq (a->z, b->z, eps) && float_eq (a->w, b->w, eps))
    {
      return true;
    }
  printf ("%s failed\n", name);
  return false;
}

bool
test_mat2x2f_add (void)
{
  const float eps = 1e-5f;
  int size = 4;

  // clang-format off
  Mat2x2f_t a =         { .m = {
     1.5f, -2.3f,
     4.1f,  3.2f
  }};
  Mat2x2f_t b =         { .m = {
     0.5f, 1.3f,
    -1.1f, 2.8f
  }};
  Mat2x2f_t expected =  { .m = {
     2.0f, -1.0f,
     3.0f,  6.0f
  }};
  // clang-format on

  Mat2x2f_t result = mat2x2f_add (&a, &b);
  return mat_eq (expected.m, result.m, size, eps, __func__);
}

bool
test_mat3x3f_add (void)
{
  const float eps = 1e-5f;
  int size = 9;

  // clang-format off
  Mat3x3f_t a =         { .m = {
     1.1f, -2.2f,  3.3f,
     4.4f,  5.5f, -6.6f,
     7.7f, -8.8f,  9.9f
  }};
  Mat3x3f_t b =         { .m = {
     0.9f,  2.2f, -3.3f,
    -4.4f, -5.5f,  6.6f,
    -7.7f,  8.8f, -9.9f
  }};
  Mat3x3f_t expected =  { .m = {
     2.0f, 0.0f, 0.0f,
     0.0f, 0.0f, 0.0f,
     0.0f, 0.0f, 0.0f
  }};
  // clang-format on

  Mat3x3f_t result = mat3x3f_add (&a, &b);
  return mat_eq (expected.m, result.m, size, eps, __func__);
}

bool
test_mat3x4f_add (void)
{
  const float eps = 1e-5f;
  int size = 12;

  // clang-format off
  Mat3x4f_t a =         { .m = {
     1.0f, 2.0f,  3.0f,  4.0f,
     5.0f, 6.0f,  7.0f,  8.0f,
     9.0f, 10.0f, 11.0f, 12.0f
  }};
  Mat3x4f_t b =         { .m = {
     0.5f, -2.0f,  1.0f,  -4.0f,
    -5.0f,  6.0f, -7.0f,   8.0f,
     9.5f, -10.0f, 11.0f, -12.0f
  }};
  Mat3x4f_t expected =  { .m = {
     1.5f,  0.0f,  4.0f,  0.0f,
     0.0f,  12.0f, 0.0f,  16.0f,
     18.5f, 0.0f,  22.0f, 0.0f
  }};
  // clang-format on

  Mat3x4f_t result = mat3x4f_add (&a, &b);
  return mat_eq (expected.m, result.m, size, eps, __func__);
}

bool
test_mat4x3f_add (void)
{
  const float eps = 1e-5f;
  int size = 12;

  // clang-format off
  Mat4x3f_t a =         { .m = {
     1.0f, 2.0f, 3.0f,
     4.0f, 5.0f, 6.0f,
     7.0f, 8.0f, 9.0f,
     10.0f, 11.0f, 12.0f
  }};
  Mat4x3f_t b =         { .m = {
    -1.0f,  -2.0f,  -3.0f,
    -4.0f,  -5.0f,  -6.0f,
    -7.0f,  -8.0f,  -9.0f,
    -10.0f, -11.0f, -12.0f
  }};
  Mat4x3f_t expected =  { .m = {
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f
  }};
  // clang-format on

  Mat4x3f_t result = mat4x3f_add (&a, &b);
  return mat_eq (expected.m, result.m, size, eps, __func__);
}

bool
test_mat4x4f_add (void)
{
  const float eps = 1e-5f;
  int size = 16;

  // clang-format off
  Mat4x4f_t a =         { .m = {
     1.1f,  -2.2f,  3.3f,  -4.4f,
     5.5f,  -6.6f,  7.7f,  -8.8f,
     9.9f,  -10.1f, 11.1f, -12.2f,
     13.3f, -14.4f, 15.5f, -16.6f
  }};
  Mat4x4f_t b =         { .m = {
     0.9f,  2.2f,  -3.3f,  4.4f,
    -5.5f,  6.6f,  -7.7f,  8.8f,
    -9.9f,  10.1f, -11.1f, 12.2f,
    -13.3f, 14.4f, -15.5f, 16.6f
  }};
  Mat4x4f_t expected =  { .m = {
     2.0f, 0.0f, 0.0f, 0.0f,
     0.0f, 0.0f, 0.0f, 0.0f,
     0.0f, 0.0f, 0.0f, 0.0f,
     0.0f, 0.0f, 0.0f, 0.0f
  }};
  // clang-format on

  Mat4x4f_t result = mat4x4f_add (&a, &b);
  return mat_eq (expected.m, result.m, size, eps, __func__);
}

bool
test_mat2x2f_sub (void)
{
  const float eps = 1e-5f;
  int size = 4;

  // clang-format off
  Mat2x2f_t a =         { .m = {
     5.50f,  3.20f,
     1.10f, -2.40f
  }};
  Mat2x2f_t b =         { .m = {
     2.00f, -1.20f,
    -4.10f,  3.40f
  }};
  Mat2x2f_t expected =  { .m = {
     3.50f,  4.40f,
     5.20f, -5.80f
  }};
  // clang-format on

  Mat2x2f_t result = mat2x2f_sub (&a, &b);
  return mat_eq (expected.m, result.m, size, eps, __func__);
}

bool
test_mat3x3f_sub (void)
{
  const float eps = 1e-5f;
  int size = 9;

  // clang-format off
  Mat3x3f_t a =         { .m = {
     4.00f,  3.50f, -2.20f,
     7.10f, -1.30f,  8.80f,
    -5.50f,  2.20f,  9.90f
  }};
  Mat3x3f_t b =         { .m = {
     1.00f, -2.50f,  3.20f,
    -4.10f,  6.30f, -7.80f,
     5.50f, -1.20f, -9.90f
  }};
  Mat3x3f_t expected =  { .m = {
     3.00f,   6.00f, -5.40f,
     11.20f, -7.60f,  16.60f,
    -11.00f,  3.40f,  19.80f
  }};
  // clang-format on

  Mat3x3f_t result = mat3x3f_sub (&a, &b);
  return mat_eq (expected.m, result.m, size, eps, __func__);
}

bool
test_mat3x4f_sub (void)
{
  const float eps = 1e-5f;
  int size = 12;

  // clang-format off
  Mat3x4f_t a =         { .m = {
     10.00f, 5.00f, -3.00f,  2.50f,
    -1.20f,  8.00f,  7.70f, -9.90f,
     4.40f, -6.60f,  3.30f,  12.00f
  }};
  Mat3x4f_t b =         { .m = {
     2.00f, -1.00f,  1.00f, -4.50f,
    -3.80f,  2.00f, -7.70f,  9.90f,
     5.60f,  6.60f, -3.30f, -2.00f
  }};
  Mat3x4f_t expected =  { .m = {
     8.00f,  6.00f,  -4.00f,   7.00f,
     2.60f,  6.00f,   15.40f, -19.80f,
    -1.20f, -13.20f,  6.60f,   14.00f
  }};
  // clang-format on

  Mat3x4f_t result = mat3x4f_sub (&a, &b);
  return mat_eq (expected.m, result.m, size, eps, __func__);
}

bool
test_mat4x3f_sub (void)
{
  const float eps = 1e-5f;
  int size = 12;

  // clang-format off
  Mat4x3f_t a =         { .m = {
     4.00f,  3.00f,  2.00f,
     1.00f, -1.00f, -2.00f,
    -3.00f,  5.00f,  7.00f,
     10.00f, 11.00f, 12.00f
  }};
  Mat4x3f_t b =         { .m = {
     1.00f,  2.00f,  3.00f,
     4.00f,  5.00f,  6.00f,
     7.00f,  8.00f,  9.00f,
     10.00f, 11.00f, 12.00f
  }};
  Mat4x3f_t expected =  { .m = {
     3.00f,   1.00f, -1.00f,
    -3.00f,  -6.00f, -8.00f,
    -10.00f, -3.00f, -2.00f,
     0.00f,   0.00f,  0.00f
  }};
  // clang-format on

  Mat4x3f_t result = mat4x3f_sub (&a, &b);
  return mat_eq (expected.m, result.m, size, eps, __func__);
}

bool
test_mat4x4f_sub (void)
{
  const float eps = 1e-5f;
  int size = 16;

  // clang-format off
  Mat4x4f_t a =        { .m = {
     1.10f,  2.20f,  3.30f,  4.40f,
     5.50f,  6.60f,  7.70f,  8.80f,
     9.90f,  10.10f, 11.20f, 12.30f,
     13.40f, 14.50f, 15.60f, 16.70f
  }};
  Mat4x4f_t b =        { .m = {
     0.10f,  -2.20f,   1.30f,  -4.40f,
    -5.50f,   6.60f,  -7.70f,   8.80f,
    -9.90f,   10.10f, -11.20f,  12.30f,
     13.40f, -14.50f,  15.60f, -16.70f
  }};
  Mat4x4f_t expected = { .m = {
     1.00f,   4.40f,   2.00f,   8.80f,
     11.00f,  0.00f,   15.40f,  0.00f,
     19.80f,  0.00f,   22.40f,  0.00f,
     0.00f,   29.00f,  0.00f,   33.40f
  }};
  // clang-format on

  Mat4x4f_t result = mat4x4f_sub (&a, &b);
  return mat_eq (expected.m, result.m, size, eps, __func__);
}

bool
test_mat2x2f_mul (void)
{
  const float eps = 1e-5f;
  int size = 4;

  // clang-format off
  Mat2x2f_t a = { .m = {
     1.50f,  -2.00f,
     3.25f,   0.75f
  }};

  Mat2x2f_t b = { .m = {
    -1.00f,  4.00f,
     2.50f, -3.50f
  }};

  Mat2x2f_t expected = { .m = {
    -6.5f,   13.0f,
    -1.375f, 10.375f
  }};
  // clang-format on

  Mat2x2f_t result = mat2x2f_mul (&a, &b);
  return mat_eq (expected.m, result.m, size, eps, __func__);
}

bool
test_mat3x3f_mul (void)
{
  const float eps = 1e-5f;
  int size = 9;

  // clang-format off
  Mat3x3f_t a =         { .m = {
     1.20f, -2.50f,  3.10f,
     0.75f,  4.40f, -1.30f,
    -3.30f,  2.20f,  0.90f
  }};

  Mat3x3f_t b =         { .m = {
     2.00f, -1.10f,  0.50f,
    -3.20f,  4.40f, -2.20f,
     1.50f,  0.25f,  3.75f
  }};

  Mat3x3f_t expected =  { .m = {
     15.05f, -11.545f,  17.725f,
    -14.53f,  18.21f,  -14.18f,
    -12.29f,  13.535f, -3.115f,
  }};
  // clang-format on

  Mat3x3f_t result = mat3x3f_mul (&a, &b);
  return mat_eq (expected.m, result.m, size, eps, __func__);
}

bool
test_mat4x4f_mul (void)
{
  const float eps = 1e-5f;
  int size = 16;

  // clang-format off
  Mat4x4f_t a =         { .m = {
     1.00f,  2.00f,  3.00f,  4.00f,
    -1.50f,  0.75f,  2.20f, -3.30f,
     5.10f, -2.40f,  0.90f,  1.70f,
     0.25f,  3.30f, -4.40f,  2.00f
  }};

  Mat4x4f_t b =         { .m = {
     2.20f, -1.00f,  0.50f,  4.00f,
    -3.00f,  3.10f, -2.20f, -1.50f,
     1.50f,  2.00f,  1.10f,  0.75f,
    -2.20f,  0.40f, -3.30f,  1.25f
  }};

  Mat4x4f_t expected =  { .m = {
    -8.1f,    12.8f,  -13.8f,    8.25f,
     5.01f,   6.905f,  10.91f,  -9.6f,
     16.03f, -10.06f,  3.21f,    26.8f,
    -20.35f,  1.98f,  -18.575f, -4.75f
  }};
  // clang-format on

  Mat4x4f_t result = mat4x4f_mul (&a, &b);
  return mat_eq (expected.m, result.m, size, eps, __func__);
}

bool
test_mat3x3f_mul_mat3x4f (void)
{
  const float eps = 1e-5f;
  int size = 12;

  // clang-format off
  Mat3x3f_t a =         { .m = {
    3.1f, 2.5f, 5.4f,
    6.2f, 1.2f, 4.0f,
    2.0f, 5.0f, 0.0f
  }};

  Mat3x4f_t b =         { .m = {
     1.1f,  3.3f, -2.0f,  4.4f,
    -0.5f,  2.2f,  0.0f, -1.1f,
     7.7f, -3.3f,  1.5f,  0.0f,
  }};

  Mat3x4f_t expected =  { .m = {
     43.74f, -2.09f,  1.9f, 10.89f,
     37.02f,  9.9f,  -6.4f, 25.96f,
    -0.3f,    17.6f, -4.0f,   3.3f,
  }};
  // clang-format on

  Mat3x4f_t result = mat3x3f_mul_mat3x4f (&a, &b);
  return mat_eq (expected.m, result.m, size, eps, __func__);
}

bool
test_mat3x4f_mul_mat4x4f (void)
{
  const float eps = 1e-5f;
  int size = 12;

  // clang-format off
  Mat3x4f_t a =         { .m = {
     43.74f,  -2.09f,  1.9f, 10.89f,
     37.02f,   9.9f,  -6.4f, 25.96f,
    -0.3f,     17.6f, -4.0f, 3.3f
  }};

  Mat4x4f_t b =         { .m = {
    -8.1f,    12.8f,  -13.8f,    8.25f,
     5.01f,   6.905f,  10.91f,  -9.6f,
     16.03f, -10.06f,  3.21f,    26.8f,
    -20.35f,  1.98f,  -18.575f, -4.75f
  }};

  Mat3x4f_t expected =  { .m = {
    -555.9194f, 547.88875f, -822.59665f,  380.1115f,
    -881.141f,  658.0003f,  -905.618f,   -84.455f,
    -40.669f,   164.462f,    122.0185f,  -294.31f
  }};
  // clang-format on

  Mat3x4f_t result = mat3x4f_mul_mat4x4f (&a, &b);
  return mat_eq (expected.m, result.m, size, eps, __func__);
}

bool
test_mat4x3f_mul_mat3x4f (void)
{
  const float eps = 1e-5f;
  int size = 16;

  // clang-format off
  Mat4x3f_t a =         { .m = {
     10.5f, 3.5f,  4.2f,
    -0.3f,  1.2f,  8.8f,
     5.5f,  4.1f, -6.6f,
    -2.2f, -1.1f,  1.1f
  }};

  Mat3x4f_t b =         { .m = {
     4.4, 5.1,  5.9,   4.1,
     4.5, 4.2,  1.1,   5.5,
    -6,  -9.3, -4.85, -6.34
  }};

  Mat4x4f_t expected =  { .m = {
     36.75f,  29.19f,  45.43f,   35.672f,
    -48.72f, -78.33f, -43.13f,  -50.422f,
     82.25f,  106.65f, 68.97f,   86.944f,
    -21.23f, -26.07f, -19.525f, -22.044f
  }};
  // clang-format on

  Mat4x4f_t result = mat4x3f_mul_mat3x4f (&a, &b);
  return mat_eq (expected.m, result.m, size, eps, __func__);
}

bool
test_mat4x4f_mul_mat4x3f (void)
{
  const float eps = 1e-5f;
  int size = 12;

  // clang-format off
  Mat4x4f_t a =         { .m = {
     1.5f,   5.5f,  4.2f,  2.52f,
     5.54f,  3.7f,  6.66f, 1.5f,
    -6.56f, -4.54f, 8.2f,  5.9f,
     3.5f,   4.6f,  14.6f, 15.7f
  }};

  Mat4x3f_t b =         { .m = {
    1.5f,    5.41f,  85.54f,
    6.23f,  -35.56f, 14.5f,
    45.32f, -17.5f,  85.6f,
    12.2f,   18.0f,  5.0f
  }};

  Mat4x3f_t expected =  { .m = {
     257.603f, -215.605f,    580.18f,   351.4922f,
    -191.1506f, 1105.1376f,  405.4798f, 88.6528f,
     104.4476f, 887.12f,    -117.541f,  1694.35f
  }};
  // clang-format on

  Mat4x3f_t result = mat4x4f_mul_mat4x3f (&a, &b);
  return mat_eq (expected.m, result.m, size, eps, __func__);
}

bool
test_mat2x2f_mul_vec2f (void)
{
  const float eps = 1e-5f;

  // clang-format off
  Mat2x2f_t m =       { .m = { 2.0f, 3.0f, 1.0f, 4.0f } };
  Vec2f_t v =         { 5.0f, 6.0f };
  Vec2f_t expected =  { 28.0f, 29.0f };
  // clang-format on

  Vec2f_t result = mat2x2f_mul_vec2f (&m, &v);
  return vec2f_eq (&expected, &result, eps, __func__);
}

bool
test_mat3x3f_mul_vec3f (void)
{
  const float eps = 1e-5f;

  // clang-format off
  Mat3x3f_t m =       { .m = {
     1.2f, -3.0f,  4.5f,
     2.0f,  0.5f, -1.0f,
    -7.0f,  3.3f,  2.2f
  }};
  Vec3f_t v =         { 2.0f, -1.0f, 3.0f };
  Vec3f_t expected =  { 18.9f, 0.5f, -10.7f };
  // clang-format on

  Vec3f_t result = mat3x3f_mul_vec3f (&m, &v);
  return vec3f_eq (&expected, &result, eps, __func__);
}

bool
test_mat3x4f_mul_vec4f (void)
{
  const float eps = 1e-5f;

  // clang-format off
  Mat3x4f_t m =       { .m = {
     1.5f, -2.0f,  0.5f,  4.0f,
    -3.2f,  1.1f,  2.0f, -0.5f,
     0.0f,  3.3f, -1.2f,  2.2f
  }};

  Vec4f_t v =         { 2.0f, 1.0f, -3.0f, 0.5f };
  Vec3f_t expected =  { 1.5f, -11.55f, 8.0f };
  // clang-format on

  Vec3f_t result = mat3x4f_mul_vec4f (&m, &v);
  return vec3f_eq (&expected, &result, eps, __func__);
}

bool
test_mat4x3f_mul_vec3f (void)
{
  const float eps = 1e-5f;

  // clang-format off
  Mat4x3f_t m =       { .m = {
     1.0f, -2.0f,  3.0f,
     0.5f,  4.0f, -1.0f,
    -3.5f,  2.2f,  0.0f,
     6.0f, -1.0f,  2.5f
  }};
  Vec3f_t v =         { 2.0f, -1.0f, 0.5f };
  Vec4f_t expected =  { 5.5f, -3.5f, -9.2f, 14.25f };
  // clang-format on

  Vec4f_t result = mat4x3f_mul_vec3f (&m, &v);
  return vec4f_eq (&expected, &result, eps, __func__);
}

bool
test_mat4x4f_mul_vec4f (void)
{
  const float eps = 1e-5f;

  // clang-format off
  Mat4x4f_t m =       { .m = {
     1.0f,  2.0f, -1.0f,  0.5f,
    -3.0f,  0.0f,  4.0f, -2.0f,
     5.5f, -1.2f,  0.0f,  3.3f,
     2.0f,  1.0f,  1.0f, -1.0f
  }};
  Vec4f_t v =         { 1.0f, -2.0f, 0.5f, 3.0f };
  Vec4f_t expected =  { -2.0f, -7.0f, 17.8f, -2.5f };
  // clang-format on

  Vec4f_t result = mat4x4f_mul_vec4f (&m, &v);
  return vec4f_eq (&expected, &result, eps, __func__);
}

bool
test_mat2x2f_mul_scalar (void)
{
  const float eps = 1e-5f;

  // clang-format off
  Mat2x2f_t m =         { .m = {
    1.3f,  2.5f,
    3.4f, -4.3f
  }};

  Mat2x2f_t expected =  { .m = {
    2.6f,  5.0f,
    6.8f, -8.6f
  }};
  // clang-format on

  Mat2x2f_t result = mat2x2f_mul_scalar (&m, 2.0f);
  return mat_eq (expected.m, result.m, 4, eps, __func__);
}

bool
test_mat3x3f_mul_scalar (void)
{
  const float eps = 1e-5f;

  // clang-format off
  Mat3x3f_t m =         { .m = {
    1.0f, 3.0f, -1.5,
    0.0f, 6.0f, -4.0f
  }};

  Mat3x3f_t expected =  { .m = {
    2.0f, 6.0f, -3.0f,
    0.0f, 12.0f, -8.0f
  }};
  // clang-format on

  Mat3x3f_t result = mat3x3f_mul_scalar (&m, 2.0f);
  return mat_eq (expected.m, result.m, 9, eps, __func__);
}

bool
test_mat3x4f_mul_scalar (void)
{
  const float eps = 1e-5f;

  // clang-format off
  Mat3x4f_t m =         { .m = {
    1.0f,  3.0f, -1.5f, 2.0f,
    0.0f,  6.0f, -4.0f, 5.5f,
    1.5f, -2.5f,  3.0f, 0.0f
  }};

  Mat3x4f_t expected =  { .m = {
    2.0f,  6.0f,  -3.0f,  4.0f,
    0.0f,  12.0f, -8.0f,  11.0f,
    3.0f, -5.0f,   6.0f,  0.0f
  }};
  // clang-format on

  Mat3x4f_t result = mat3x4f_mul_scalar (&m, 2.0f);
  return mat_eq (expected.m, result.m, 12, eps, __func__);
}

bool
test_mat4x3f_mul_scalar (void)
{
  const float eps = 1e-5f;

  // clang-format off
  Mat4x3f_t m =         { .m = {
    1.0f,  3.0f, -1.5f,
    0.0f,  6.0f, -4.0f,
    1.5f, -2.5f,  3.0f,
    4.0f,  0.0f, -3.0f
  }};

  Mat4x3f_t expected =  { .m = {
    2.0f,  6.0f,  -3.0f,
    0.0f,  12.0f, -8.0f,
    3.0f, -5.0f,   6.0f,
    8.0f,  0.0f,  -6.0f
  }};
  // clang-format on

  Mat4x3f_t result = mat4x3f_mul_scalar (&m, 2.0f);
  return mat_eq (expected.m, result.m, 12, eps, __func__);
}

bool
test_mat4x4f_mul_scalar (void)
{
  const float eps = 1e-5f;

  // clang-format off
  Mat4x4f_t m =         { .m = {
    1.0f,  3.0f, -1.5f, 2.0f,
    0.0f,  6.0f, -4.0f, 5.5f,
    1.5f, -2.5f,  3.0f, 0.0f,
    4.0f,  0.0f, -3.0f, 7.0f
  }};

  Mat4x4f_t expected =  { .m = {
    2.0f,  6.0f,  -3.0f, 4.0f,
    0.0f,  12.0f, -8.0f, 11.0f,
    3.0f, -5.0f,   6.0f, 0.0f,
    8.0f,  0.0f,  -6.0f, 14.0f
  }};
  // clang-format on

  Mat4x4f_t result = mat4x4f_mul_scalar (&m, 2.0f);
  return mat_eq (expected.m, result.m, 16, eps, __func__);
}

bool
test_mat2x2f_div_scalar (void)
{
  const float eps = 1e-5f;

  // clang-format off
  Mat2x2f_t m =         { .m = {
    2.0f, 4.0f,
    6.0f, 8.0f
  }};

  Mat2x2f_t expected =  { .m = {
    1.0f, 2.0f,
    3.0f, 4.0f
  }};
  // clang-format on

  Mat2x2f_t result = mat2x2f_div_scalar (&m, 2.0f);
  return mat_eq (expected.m, result.m, 4, eps, __func__);
}

bool
test_mat3x3f_div_scalar (void)
{
  const float eps = 1e-5f;

  // clang-format off
  Mat3x3f_t m =         { .m = {
    2.0f,  6.0f,  -3.0f,
    0.0f,  12.0f, -8.0f,
    3.0f, -5.0f,   6.0f
  }};

  Mat3x3f_t expected =  { .m = {
    1.0f,  3.0f, -1.5f,
    0.0f,  6.0f, -4.0f,
    1.5f, -2.5f,  3.0f
  }};
  // clang-format on

  Mat3x3f_t result = mat3x3f_div_scalar (&m, 2.0f);
  return mat_eq (expected.m, result.m, 9, eps, __func__);
}

bool
test_mat3x4f_div_scalar (void)
{
  const float eps = 1e-5f;

  // clang-format off
  Mat3x4f_t m =         { .m = {
    2.0f,  6.0f,  -3.0f, 4.0f,
    0.0f,  12.0f, -8.0f, 11.0f,
    3.0f, -5.0f,   6.0f, 0.0f
  }};

  Mat3x4f_t expected =  { .m = {
    1.0f,  3.0f, -1.5f, 2.0f,
    0.0f,  6.0f, -4.0f, 5.5f,
    1.5f, -2.5f,  3.0f, 0.0f
  }};
  // clang-format on

  Mat3x4f_t result = mat3x4f_div_scalar (&m, 2.0f);
  return mat_eq (expected.m, result.m, 12, eps, __func__);
}

bool
test_mat4x3f_div_scalar (void)
{
  const float eps = 1e-5f;

  // clang-format off
  Mat4x3f_t m =         { .m = {
    2.0f,  6.0f,  -3.0f,
    0.0f,  12.0f, -8.0f,
    3.0f, -5.0f,   6.0f,
    8.0f,  0.0f,  -6.0f
  }};

  Mat4x3f_t expected =  { .m = {
    1.0f,  3.0f, -1.5f,
    0.0f,  6.0f, -4.0f,
    1.5f, -2.5f,  3.0f,
    4.0f,  0.0f, -3.0f
  }};
  // clang-format on

  Mat4x3f_t result = mat4x3f_div_scalar (&m, 2.0f);
  return mat_eq (expected.m, result.m, 12, eps, __func__);
}

bool
test_mat4x4f_div_scalar (void)
{
  const float eps = 1e-5f;

  // clang-format off
  Mat4x4f_t m = { .m = {
    2.0f,  6.0f,  -3.0f, 4.0f,
    0.0f,  12.0f, -8.0f, 11.0f,
    3.0f, -5.0f,   6.0f, 0.0f,
    8.0f,  0.0f,  -6.0f, 14.0f
  }};

  Mat4x4f_t expected = { .m = {
    1.0f, 3.0f, -1.5f, 2.0f,
    0.0f, 6.0f, -4.0f, 5.5f,
    1.5f, -2.5f, 3.0f, 0.0f,
    4.0f, 0.0f, -3.0f, 7.0f
  }};
  // clang-format on

  Mat4x4f_t result = mat4x4f_div_scalar (&m, 2.0f);
  return mat_eq (expected.m, result.m, 16, eps, __func__);
}

bool
test_mat2x2f_transpose (void)
{
  const float eps = 1e-5f;

  // clang-format off
  Mat2x2f_t m = { .m = {
    1.0f, 2.0f,
    3.0f, 4.0f
  }};

  Mat2x2f_t expected = { .m = {
    1.0f, 3.0f,
    2.0f, 4.0f
  }};
  // clang-format on

  Mat2x2f_t result = mat2x2f_transpose (&m);
  return mat_eq (expected.m, result.m, 4, eps, __func__);
}

bool
test_mat3x3f_transpose (void)
{
  const float eps = 1e-5f;

  // clang-format off
  Mat3x3f_t m = { .m = {
    1.0f, 2.0f, 3.0f,
    4.0f, 5.0f, 6.0f,
    7.0f, 8.0f, 9.0f
  }};

  Mat3x3f_t expected = { .m = {
    1.0f, 4.0f, 7.0f,
    2.0f, 5.0f, 8.0f,
    3.0f, 6.0f, 9.0f
  }};
  // clang-format on

  Mat3x3f_t result = mat3x3f_transpose (&m);
  return mat_eq (expected.m, result.m, 9, eps, __func__);
}

bool
test_mat3x4f_transpose (void)
{
  const float eps = 1e-5f;

  // clang-format off
  Mat3x4f_t m = { .m = {
    1.0f,  2.0f,  3.0f,  4.0f,
    5.0f,  6.0f,  7.0f,  8.0f,
    9.0f,  10.0f, 11.0f, 12.0f
  }};

  Mat4x3f_t expected = { .m = {
    1.0f, 5.0f, 9.0f,
    2.0f, 6.0f, 10.0f,
    3.0f, 7.0f, 11.0f,
    4.0f, 8.0f, 12.0f
  }};
  // clang-format on

  Mat4x3f_t result = mat3x4f_transpose (&m);
  return mat_eq (expected.m, result.m, 12, eps, __func__);
}

bool
test_mat4x3f_transpose (void)
{
  const float eps = 1e-5f;

  // clang-format off
  Mat4x3f_t m = { .m = {
    1.0f,  2.0f,  3.0f,
    4.0f,  5.0f,  6.0f,
    7.0f,  8.0f,  9.0f,
    10.0f, 11.0f, 12.0f
  }};

  Mat3x4f_t expected = { .m = {
    1.0f, 4.0f, 7.0f, 10.0f,
    2.0f, 5.0f, 8.0f, 11.0f,
    3.0f, 6.0f, 9.0f, 12.0f
  }};
  // clang-format on

  Mat3x4f_t result = mat4x3f_transpose (&m);
  return mat_eq (expected.m, result.m, 12, eps, __func__);
}

bool
test_mat4x4f_transpose (void)
{
  const float eps = 1e-5f;

  // clang-format off
  Mat4x4f_t m = { .m = {
    1.0f,   2.0f,  3.0f,  4.0f,
    5.0f,   6.0f,  7.0f,  8.0f,
    9.0f,   10.0f, 11.0f, 12.0f,
    13.0f,  14.0f, 15.0f, 16.0f
  }};

  Mat4x4f_t expected = { .m = {
    1.0f, 5.0f, 9.0f,  13.0f,
    2.0f, 6.0f, 10.0f, 14.0f,
    3.0f, 7.0f, 11.0f, 15.0f,
    4.0f, 8.0f, 12.0f, 16.0f
  }};
  // clang-format on

  Mat4x4f_t result = mat4x4f_transpose (&m);
  return mat_eq (expected.m, result.m, 16, eps, __func__);
}
