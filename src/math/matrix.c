#include "math/matrix.h"
#include <math.h>

#define MAT_ADD(result, a, b, size)                                           \
  do                                                                          \
    {                                                                         \
      for (int i = 0; i < (size); i++)                                        \
        {                                                                     \
          (result).m[i] = (a)->m[i] + (b)->m[i];                              \
        }                                                                     \
    }                                                                         \
  while (0)

#define MAT_SUB(result, a, b, size)                                           \
  do                                                                          \
    {                                                                         \
      for (int i = 0; i < (size); i++)                                        \
        {                                                                     \
          (result).m[i] = (a)->m[i] - (b)->m[i];                              \
        }                                                                     \
    }                                                                         \
  while (0)

#define MAT_MUL_SCALAR(result, m, s, size)                                    \
  do                                                                          \
    {                                                                         \
      for (int i = 0; i < (size); i++)                                        \
        {                                                                     \
          (result).m[i] = (m)->m[i] * (s);                                    \
        }                                                                     \
    }                                                                         \
  while (0)

#define MAT_DIV_SCALAR(result, m, s, size)                                    \
  do                                                                          \
    {                                                                         \
      for (int i = 0; i < (size); i++)                                        \
        {                                                                     \
          (result).m[i] = (m)->m[i] / (s);                                    \
        }                                                                     \
    }                                                                         \
  while (0)

#define GET_MINOR(row, col)                                                   \
  minor.m[0] = m->m[((row + 1) % 4) * 4 + ((col + 1) % 4)];                   \
  minor.m[1] = m->m[((row + 1) % 4) * 4 + ((col + 2) % 4)];                   \
  minor.m[2] = m->m[((row + 1) % 4) * 4 + ((col + 3) % 4)];                   \
  minor.m[3] = m->m[((row + 2) % 4) * 4 + ((col + 1) % 4)];                   \
  minor.m[4] = m->m[((row + 2) % 4) * 4 + ((col + 2) % 4)];                   \
  minor.m[5] = m->m[((row + 2) % 4) * 4 + ((col + 3) % 4)];                   \
  minor.m[6] = m->m[((row + 3) % 4) * 4 + ((col + 1) % 4)];                   \
  minor.m[7] = m->m[((row + 3) % 4) * 4 + ((col + 2) % 4)];                   \
  minor.m[8] = m->m[((row + 3) % 4) * 4 + ((col + 3) % 4)];

Mat2x2f_t
mat2x2f_zero (void)
{
  return (Mat2x2f_t){ 0 };
}

Mat3x3f_t
mat3x3f_zero (void)
{
  return (Mat3x3f_t){ 0 };
}

Mat3x4f_t
mat3x4f_zero (void)
{
  return (Mat3x4f_t){ 0 };
}

Mat4x3f_t
mat4x3f_zero (void)
{
  return (Mat4x3f_t){ 0 };
}

Mat4x4f_t
mat4x4f_zero (void)
{
  return (Mat4x4f_t){ 0 };
}

Mat2x2f_t
mat2x2f_add (const Mat2x2f_t *a, const Mat2x2f_t *b)
{
  Mat2x2f_t result;
  MAT_ADD (result, a, b, 4);
  return result;
}

Mat3x3f_t
mat3x3f_add (const Mat3x3f_t *a, const Mat3x3f_t *b)
{
  Mat3x3f_t result;
  MAT_ADD (result, a, b, 9);
  return result;
}

Mat3x4f_t
mat3x4f_add (const Mat3x4f_t *a, const Mat3x4f_t *b)
{
  Mat3x4f_t result;
  MAT_ADD (result, a, b, 12);
  return result;
}

Mat4x3f_t
mat4x3f_add (const Mat4x3f_t *a, const Mat4x3f_t *b)
{
  Mat4x3f_t result;
  MAT_ADD (result, a, b, 12);
  return result;
}

Mat4x4f_t
mat4x4f_add (const Mat4x4f_t *a, const Mat4x4f_t *b)
{
  Mat4x4f_t result;
  MAT_ADD (result, a, b, 16);
  return result;
}

Mat2x2f_t
mat2x2f_sub (const Mat2x2f_t *a, const Mat2x2f_t *b)
{
  Mat2x2f_t result;
  MAT_SUB (result, a, b, 4);
  return result;
}

Mat3x3f_t
mat3x3f_sub (const Mat3x3f_t *a, const Mat3x3f_t *b)
{
  Mat3x3f_t result;
  MAT_SUB (result, a, b, 9);
  return result;
}

Mat3x4f_t
mat3x4f_sub (const Mat3x4f_t *a, const Mat3x4f_t *b)
{
  Mat3x4f_t result;
  MAT_SUB (result, a, b, 12);
  return result;
}

Mat4x3f_t
mat4x3f_sub (const Mat4x3f_t *a, const Mat4x3f_t *b)
{
  Mat4x3f_t result;
  MAT_SUB (result, a, b, 12);
  return result;
}

Mat4x4f_t
mat4x4f_sub (const Mat4x4f_t *a, const Mat4x4f_t *b)
{
  Mat4x4f_t result;
  MAT_SUB (result, a, b, 16);
  return result;
}

Mat2x2f_t
mat2x2f_mul (const Mat2x2f_t *a, const Mat2x2f_t *b)
{
  Mat2x2f_t result;

  /* clang-format off */
  result.m[0] = a->m[0] * b->m[0] + a->m[1] * b->m[2];
  result.m[1] = a->m[0] * b->m[1] + a->m[1] * b->m[3];
  result.m[2] = a->m[2] * b->m[0] + a->m[3] * b->m[2];
  result.m[3] = a->m[2] * b->m[1] + a->m[3] * b->m[3];
  /* clang-format on */

  return result;
}

Mat3x3f_t
mat3x3f_mul (const Mat3x3f_t *a, const Mat3x3f_t *b)
{
  Mat3x3f_t result;

  /* clang-format off */
  result.m[0] = a->m[0] * b->m[0] + a->m[1] * b->m[3] + a->m[2] * b->m[6];
  result.m[1] = a->m[0] * b->m[1] + a->m[1] * b->m[4] + a->m[2] * b->m[7];
  result.m[2] = a->m[0] * b->m[2] + a->m[1] * b->m[5] + a->m[2] * b->m[8];

  result.m[3] = a->m[3] * b->m[0] + a->m[4] * b->m[3] + a->m[5] * b->m[6];
  result.m[4] = a->m[3] * b->m[1] + a->m[4] * b->m[4] + a->m[5] * b->m[7];
  result.m[5] = a->m[3] * b->m[2] + a->m[4] * b->m[5] + a->m[5] * b->m[8];

  result.m[6] = a->m[6] * b->m[0] + a->m[7] * b->m[3] + a->m[8] * b->m[6];
  result.m[7] = a->m[6] * b->m[1] + a->m[7] * b->m[4] + a->m[8] * b->m[7];
  result.m[8] = a->m[6] * b->m[2] + a->m[7] * b->m[5] + a->m[8] * b->m[8];
  /* clang-format on */

  return result;
}

Mat4x4f_t
mat4x4f_mul (const Mat4x4f_t *a, const Mat4x4f_t *b)
{
  Mat4x4f_t result;

  /* clang-format off */
  result.m[0]  = a->m[0]*b->m[0]  + a->m[1]*b->m[4]  + a->m[2]*b->m[8]   + a->m[3]*b->m[12];
  result.m[1]  = a->m[0]*b->m[1]  + a->m[1]*b->m[5]  + a->m[2]*b->m[9]   + a->m[3]*b->m[13];
  result.m[2]  = a->m[0]*b->m[2]  + a->m[1]*b->m[6]  + a->m[2]*b->m[10]  + a->m[3]*b->m[14];
  result.m[3]  = a->m[0]*b->m[3]  + a->m[1]*b->m[7]  + a->m[2]*b->m[11]  + a->m[3]*b->m[15];

  result.m[4]  = a->m[4]*b->m[0]  + a->m[5]*b->m[4]  + a->m[6]*b->m[8]   + a->m[7]*b->m[12];
  result.m[5]  = a->m[4]*b->m[1]  + a->m[5]*b->m[5]  + a->m[6]*b->m[9]   + a->m[7]*b->m[13];
  result.m[6]  = a->m[4]*b->m[2]  + a->m[5]*b->m[6]  + a->m[6]*b->m[10]  + a->m[7]*b->m[14];
  result.m[7]  = a->m[4]*b->m[3]  + a->m[5]*b->m[7]  + a->m[6]*b->m[11]  + a->m[7]*b->m[15];

  result.m[8]  = a->m[8]*b->m[0]  + a->m[9]*b->m[4]  + a->m[10]*b->m[8]  + a->m[11]*b->m[12];
  result.m[9]  = a->m[8]*b->m[1]  + a->m[9]*b->m[5]  + a->m[10]*b->m[9]  + a->m[11]*b->m[13];
  result.m[10] = a->m[8]*b->m[2]  + a->m[9]*b->m[6]  + a->m[10]*b->m[10] + a->m[11]*b->m[14];
  result.m[11] = a->m[8]*b->m[3]  + a->m[9]*b->m[7]  + a->m[10]*b->m[11] + a->m[11]*b->m[15];

  result.m[12] = a->m[12]*b->m[0] + a->m[13]*b->m[4] + a->m[14]*b->m[8]  + a->m[15]*b->m[12];
  result.m[13] = a->m[12]*b->m[1] + a->m[13]*b->m[5] + a->m[14]*b->m[9]  + a->m[15]*b->m[13];
  result.m[14] = a->m[12]*b->m[2] + a->m[13]*b->m[6] + a->m[14]*b->m[10] + a->m[15]*b->m[14];
  result.m[15] = a->m[12]*b->m[3] + a->m[13]*b->m[7] + a->m[14]*b->m[11] + a->m[15]*b->m[15];
  /* clang-format on */

  return result;
}

Mat3x4f_t
mat3x3f_mul_mat3x4f (const Mat3x3f_t *a, const Mat3x4f_t *b)
{
  Mat3x4f_t result;

  /* clang-format off */
  result.m[0]  = a->m[0]*b->m[0]  + a->m[1]*b->m[4]  + a->m[2]*b->m[8];
  result.m[1]  = a->m[0]*b->m[1]  + a->m[1]*b->m[5]  + a->m[2]*b->m[9];
  result.m[2]  = a->m[0]*b->m[2]  + a->m[1]*b->m[6]  + a->m[2]*b->m[10];
  result.m[3]  = a->m[0]*b->m[3]  + a->m[1]*b->m[7]  + a->m[2]*b->m[11];

  result.m[4]  = a->m[3]*b->m[0]  + a->m[4]*b->m[4]  + a->m[5]*b->m[8];
  result.m[5]  = a->m[3]*b->m[1]  + a->m[4]*b->m[5]  + a->m[5]*b->m[9];
  result.m[6]  = a->m[3]*b->m[2]  + a->m[4]*b->m[6]  + a->m[5]*b->m[10];
  result.m[7]  = a->m[3]*b->m[3]  + a->m[4]*b->m[7]  + a->m[5]*b->m[11];

  result.m[8]  = a->m[6]*b->m[0]  + a->m[7]*b->m[4]  + a->m[8]*b->m[8];
  result.m[9]  = a->m[6]*b->m[1]  + a->m[7]*b->m[5]  + a->m[8]*b->m[9];
  result.m[10] = a->m[6]*b->m[2]  + a->m[7]*b->m[6]  + a->m[8]*b->m[10];
  result.m[11] = a->m[6]*b->m[3]  + a->m[7]*b->m[7]  + a->m[8]*b->m[11];
  /* clang-format on */

  return result;
}

Mat3x4f_t
mat3x4f_mul_mat4x4f (const Mat3x4f_t *a, const Mat4x4f_t *b)
{
  Mat3x4f_t result;

  /* clang-format off */
  result.m[0]  = a->m[0]*b->m[0]  + a->m[1]*b->m[4]  + a->m[2]*b->m[8]   + a->m[3]*b->m[12];
  result.m[1]  = a->m[0]*b->m[1]  + a->m[1]*b->m[5]  + a->m[2]*b->m[9]   + a->m[3]*b->m[13];
  result.m[2]  = a->m[0]*b->m[2]  + a->m[1]*b->m[6]  + a->m[2]*b->m[10]  + a->m[3]*b->m[14];
  result.m[3]  = a->m[0]*b->m[3]  + a->m[1]*b->m[7]  + a->m[2]*b->m[11]  + a->m[3]*b->m[15];

  result.m[4]  = a->m[4]*b->m[0]  + a->m[5]*b->m[4]  + a->m[6]*b->m[8]   + a->m[7]*b->m[12];
  result.m[5]  = a->m[4]*b->m[1]  + a->m[5]*b->m[5]  + a->m[6]*b->m[9]   + a->m[7]*b->m[13];
  result.m[6]  = a->m[4]*b->m[2]  + a->m[5]*b->m[6]  + a->m[6]*b->m[10]  + a->m[7]*b->m[14];
  result.m[7]  = a->m[4]*b->m[3]  + a->m[5]*b->m[7]  + a->m[6]*b->m[11]  + a->m[7]*b->m[15];

  result.m[8]  = a->m[8]*b->m[0]  + a->m[9]*b->m[4]  + a->m[10]*b->m[8]  + a->m[11]*b->m[12];
  result.m[9]  = a->m[8]*b->m[1]  + a->m[9]*b->m[5]  + a->m[10]*b->m[9]  + a->m[11]*b->m[13];
  result.m[10] = a->m[8]*b->m[2]  + a->m[9]*b->m[6]  + a->m[10]*b->m[10] + a->m[11]*b->m[14];
  result.m[11] = a->m[8]*b->m[3]  + a->m[9]*b->m[7]  + a->m[10]*b->m[11] + a->m[11]*b->m[15];
  /* clang-format on */

  return result;
}

Mat4x4f_t
mat4x3f_mul_mat3x4f (const Mat4x3f_t *a, const Mat3x4f_t *b)
{
  Mat4x4f_t result;

  /* clang-format off */
  result.m[0]  = a->m[0]*b->m[0]  + a->m[1]*b->m[4]  + a->m[2]*b->m[8];
  result.m[1]  = a->m[0]*b->m[1]  + a->m[1]*b->m[5]  + a->m[2]*b->m[9];
  result.m[2]  = a->m[0]*b->m[2]  + a->m[1]*b->m[6]  + a->m[2]*b->m[10];
  result.m[3]  = a->m[0]*b->m[3]  + a->m[1]*b->m[7]  + a->m[2]*b->m[11];

  result.m[4]  = a->m[3]*b->m[0]  + a->m[4]*b->m[4]  + a->m[5]*b->m[8];
  result.m[5]  = a->m[3]*b->m[1]  + a->m[4]*b->m[5]  + a->m[5]*b->m[9];
  result.m[6]  = a->m[3]*b->m[2]  + a->m[4]*b->m[6]  + a->m[5]*b->m[10];
  result.m[7]  = a->m[3]*b->m[3]  + a->m[4]*b->m[7]  + a->m[5]*b->m[11];

  result.m[8]  = a->m[6]*b->m[0]  + a->m[7]*b->m[4]  + a->m[8]*b->m[8];
  result.m[9]  = a->m[6]*b->m[1]  + a->m[7]*b->m[5]  + a->m[8]*b->m[9];
  result.m[10] = a->m[6]*b->m[2]  + a->m[7]*b->m[6]  + a->m[8]*b->m[10];
  result.m[11] = a->m[6]*b->m[3]  + a->m[7]*b->m[7]  + a->m[8]*b->m[11];

  result.m[12] = a->m[9]*b->m[0]  + a->m[10]*b->m[4] + a->m[11]*b->m[8];
  result.m[13] = a->m[9]*b->m[1]  + a->m[10]*b->m[5] + a->m[11]*b->m[9];
  result.m[14] = a->m[9]*b->m[2]  + a->m[10]*b->m[6] + a->m[11]*b->m[10];
  result.m[15] = a->m[9]*b->m[3]  + a->m[10]*b->m[7] + a->m[11]*b->m[11];
  /* clang-format on */

  return result;
}

Mat4x3f_t
mat4x4f_mul_mat4x3f (const Mat4x4f_t *a, const Mat4x3f_t *b)
{
  Mat4x3f_t result;

  /* clang-format off */
  result.m[0]   = a->m[0]*b->m[0]  + a->m[1]*b->m[3]  + a->m[2]*b->m[6]  + a->m[3]*b->m[9];
  result.m[1]   = a->m[0]*b->m[1]  + a->m[1]*b->m[4]  + a->m[2]*b->m[7]  + a->m[3]*b->m[10];
  result.m[2]   = a->m[0]*b->m[2]  + a->m[1]*b->m[5]  + a->m[2]*b->m[8]  + a->m[3]*b->m[11];

  result.m[3]   = a->m[4]*b->m[0]  + a->m[5]*b->m[3]  + a->m[6]*b->m[6]  + a->m[7]*b->m[9];
  result.m[4]   = a->m[4]*b->m[1]  + a->m[5]*b->m[4]  + a->m[6]*b->m[7]  + a->m[7]*b->m[10];
  result.m[5]   = a->m[4]*b->m[2]  + a->m[5]*b->m[5]  + a->m[6]*b->m[8]  + a->m[7]*b->m[11];

  result.m[6]   = a->m[8]*b->m[0]  + a->m[9]*b->m[3]  + a->m[10]*b->m[6] + a->m[11]*b->m[9];
  result.m[7]   = a->m[8]*b->m[1]  + a->m[9]*b->m[4]  + a->m[10]*b->m[7] + a->m[11]*b->m[10];
  result.m[8]   = a->m[8]*b->m[2]  + a->m[9]*b->m[5]  + a->m[10]*b->m[8] + a->m[11]*b->m[11];

  result.m[9]   = a->m[12]*b->m[0] + a->m[13]*b->m[3] + a->m[14]*b->m[6] + a->m[15]*b->m[9];
  result.m[10]  = a->m[12]*b->m[1] + a->m[13]*b->m[4] + a->m[14]*b->m[7] + a->m[15]*b->m[10];
  result.m[11]  = a->m[12]*b->m[2] + a->m[13]*b->m[5] + a->m[14]*b->m[8] + a->m[15]*b->m[11];
  /* clang-format on */

  return result;
}

Vec2f_t
mat2x2f_mul_vec2f (const Mat2x2f_t *m, const Vec2f_t *v)
{
  Vec2f_t result;

  result.x = m->m[0] * v->x + m->m[1] * v->y;
  result.y = m->m[2] * v->x + m->m[3] * v->y;

  return result;
}

Vec3f_t
mat3x3f_mul_vec3f (const Mat3x3f_t *m, const Vec3f_t *v)
{
  Vec3f_t result;

  /* clang-format off */
  result.x = m->m[0] * v->x + m->m[1] * v->y + m->m[2] * v->z;
  result.y = m->m[3] * v->x + m->m[4] * v->y + m->m[5] * v->z;
  result.z = m->m[6] * v->x + m->m[7] * v->y + m->m[8] * v->z;
  /* clang-format on */

  return result;
}

Vec3f_t
mat3x4f_mul_vec4f (const Mat3x4f_t *m, const Vec4f_t *v)
{
  Vec3f_t result;

  /* clang-format off */
  result.x = m->m[0] * v->x + m->m[1] * v->y + m->m[2] * v->z + m->m[3]  * v->w;
  result.y = m->m[4] * v->x + m->m[5] * v->y + m->m[6] * v->z + m->m[7]  * v->w;
  result.z = m->m[8] * v->x + m->m[9] * v->y + m->m[10]* v->z + m->m[11] * v->w;
  /* clang-format on */

  return result;
}

Vec4f_t
mat4x3f_mul_vec3f (const Mat4x3f_t *m, const Vec3f_t *v)
{
  Vec4f_t result;

  /* clang-format off */
  result.x = m->m[0]  * v->x + m->m[1]  * v->y + m->m[2]  * v->z;
  result.y = m->m[3]  * v->x + m->m[4]  * v->y + m->m[5]  * v->z;
  result.z = m->m[6]  * v->x + m->m[7]  * v->y + m->m[8]  * v->z;
  result.w = m->m[9]  * v->x + m->m[10] * v->y + m->m[11] * v->z;
  /* clang-format on */

  return result;
}

Vec4f_t
mat4x4f_mul_vec4f (const Mat4x4f_t *m, const Vec4f_t *v)
{
  Vec4f_t result;

  /* clang-format off */
  result.x = m->m[0]  * v->x + m->m[1]  * v->y + m->m[2]  * v->z + m->m[3]  * v->w;
  result.y = m->m[4]  * v->x + m->m[5]  * v->y + m->m[6]  * v->z + m->m[7]  * v->w;
  result.z = m->m[8]  * v->x + m->m[9]  * v->y + m->m[10] * v->z + m->m[11] * v->w;
  result.w = m->m[12] * v->x + m->m[13] * v->y + m->m[14] * v->z + m->m[15] * v->w;
  /* clang-format on */

  return result;
}

Mat2x2f_t
mat2x2f_mul_scalar (const Mat2x2f_t *m, float s)
{
  Mat2x2f_t result;
  MAT_MUL_SCALAR (result, m, s, 4);
  return result;
}

Mat3x3f_t
mat3x3f_mul_scalar (const Mat3x3f_t *m, float s)
{
  Mat3x3f_t result;
  MAT_MUL_SCALAR (result, m, s, 9);
  return result;
}

Mat3x4f_t
mat3x4f_mul_scalar (const Mat3x4f_t *m, float s)
{
  Mat3x4f_t result;
  MAT_MUL_SCALAR (result, m, s, 12);
  return result;
}

Mat4x3f_t
mat4x3f_mul_scalar (const Mat4x3f_t *m, float s)
{
  Mat4x3f_t result;
  MAT_MUL_SCALAR (result, m, s, 12);
  return result;
}

Mat4x4f_t
mat4x4f_mul_scalar (const Mat4x4f_t *m, float s)
{
  Mat4x4f_t result;
  MAT_MUL_SCALAR (result, m, s, 16);
  return result;
}

Mat2x2f_t
mat2x2f_div_scalar (const Mat2x2f_t *m, float s)
{
  Mat2x2f_t result;
  MAT_DIV_SCALAR (result, m, s, 4);
  return result;
}

Mat3x3f_t
mat3x3f_div_scalar (const Mat3x3f_t *m, float s)
{
  Mat3x3f_t result;
  MAT_DIV_SCALAR (result, m, s, 9);
  return result;
}

Mat3x4f_t
mat3x4f_div_scalar (const Mat3x4f_t *m, float s)
{
  Mat3x4f_t result;
  MAT_DIV_SCALAR (result, m, s, 12);
  return result;
}

Mat4x3f_t
mat4x3f_div_scalar (const Mat4x3f_t *m, float s)
{
  Mat4x3f_t result;
  MAT_DIV_SCALAR (result, m, s, 12);
  return result;
}

Mat4x4f_t
mat4x4f_div_scalar (const Mat4x4f_t *m, float s)
{
  Mat4x4f_t result;
  MAT_DIV_SCALAR (result, m, s, 16);
  return result;
}

Mat2x2f_t
mat2x2f_transpose (const Mat2x2f_t *m)
{
  Mat2x2f_t result;
  result.m[0] = m->m[0];
  result.m[1] = m->m[2];
  result.m[2] = m->m[1];
  result.m[3] = m->m[3];
  return result;
}

Mat3x3f_t
mat3x3f_transpose (const Mat3x3f_t *m)
{
  Mat3x3f_t result;

  result.m[0] = m->m[0];
  result.m[1] = m->m[3];
  result.m[2] = m->m[6];

  result.m[3] = m->m[1];
  result.m[4] = m->m[4];
  result.m[5] = m->m[7];

  result.m[6] = m->m[2];
  result.m[7] = m->m[5];
  result.m[8] = m->m[8];

  return result;
}

Mat4x3f_t
mat3x4f_transpose (const Mat3x4f_t *m)
{
  Mat4x3f_t result;

  result.m[0] = m->m[0];
  result.m[1] = m->m[4];
  result.m[2] = m->m[8];
  result.m[3] = m->m[1];

  result.m[4] = m->m[5];
  result.m[5] = m->m[9];
  result.m[6] = m->m[2];
  result.m[7] = m->m[6];

  result.m[8] = m->m[10];
  result.m[9] = m->m[3];
  result.m[10] = m->m[7];
  result.m[11] = m->m[11];

  return result;
}

Mat3x4f_t
mat4x3f_transpose (const Mat4x3f_t *m)
{
  Mat3x4f_t result;

  result.m[0] = m->m[0];
  result.m[1] = m->m[3];
  result.m[2] = m->m[6];

  result.m[3] = m->m[9];
  result.m[4] = m->m[1];
  result.m[5] = m->m[4];

  result.m[6] = m->m[7];
  result.m[7] = m->m[10];
  result.m[8] = m->m[2];

  result.m[9] = m->m[5];
  result.m[10] = m->m[8];
  result.m[11] = m->m[11];

  return result;
}

Mat4x4f_t
mat4x4f_transpose (const Mat4x4f_t *m)
{
  Mat4x4f_t result;

  result.m[0] = m->m[0];
  result.m[1] = m->m[4];
  result.m[2] = m->m[8];
  result.m[3] = m->m[12];

  result.m[4] = m->m[1];
  result.m[5] = m->m[5];
  result.m[6] = m->m[9];
  result.m[7] = m->m[13];

  result.m[8] = m->m[2];
  result.m[9] = m->m[6];
  result.m[10] = m->m[10];
  result.m[11] = m->m[14];

  result.m[12] = m->m[3];
  result.m[13] = m->m[7];
  result.m[14] = m->m[11];
  result.m[15] = m->m[15];

  return result;
}

float
mat2x2f_det (const Mat2x2f_t *m)
{
  return m->m[0] * m->m[3] - m->m[1] * m->m[2];
}

float
mat3x3f_det (const Mat3x3f_t *m)
{
  /* clang-format off */
  return  m->m[0] * (m->m[4] * m->m[8] - m->m[5] * m->m[7])
         -m->m[1] * (m->m[3] * m->m[8] - m->m[5] * m->m[6])
         +m->m[2] * (m->m[3] * m->m[7] - m->m[4] * m->m[6]);
  /* clang-format on */
}

float
mat4x4f_det (const Mat4x4f_t *m)
{
  float det;
  Mat3x3f_t minor;

  GET_MINOR (0, 0);
  float c0 = mat3x3f_det (&minor);

  GET_MINOR (0, 1);
  float c1 = -mat3x3f_det (&minor);

  GET_MINOR (0, 2);
  float c2 = mat3x3f_det (&minor);

  GET_MINOR (0, 3);
  float c3 = -mat3x3f_det (&minor);

  det = m->m[0] * c0 + m->m[1] * c1 + m->m[2] * c2 + m->m[3] * c3;

  return det;
}

/* Inveres matrix */
bool
mat2x2f_inv (const Mat2x2f_t *m, Mat2x2f_t *out)
{
  float det = mat2x2f_det (m);

  if (fabs (det) < 1e-6)
    {
      return false;
    }

  float inv_det = 1.0f / det;

  /* clang-format off */
  out->m[0] =  m->m[3] * inv_det;
  out->m[1] = -m->m[1] * inv_det;
  out->m[2] = -m->m[2] * inv_det;
  out->m[3] =  m->m[0] * inv_det;
  /* clang-format on */

  return true;
}

bool
mat3x3f_inv (const Mat3x3f_t *m, Mat3x3f_t *out)
{
  float det = mat3x3f_det (m);

  if (fabs (det) < 1e-6f)
    {
      return false;
    }

  float inv_det = 1.0f / det;

  /* clang-format off */
  out->m[0] =  (m->m[4]*m->m[8] - m->m[5]*m->m[7]) * inv_det;
  out->m[1] = -(m->m[1]*m->m[8] - m->m[2]*m->m[7]) * inv_det;
  out->m[2] =  (m->m[1]*m->m[5] - m->m[2]*m->m[4]) * inv_det;

  out->m[3] = -(m->m[3]*m->m[8] - m->m[5]*m->m[6]) * inv_det;
  out->m[4] =  (m->m[0]*m->m[8] - m->m[2]*m->m[6]) * inv_det;
  out->m[5] = -(m->m[0]*m->m[5] - m->m[2]*m->m[3]) * inv_det;

  out->m[6] =  (m->m[3]*m->m[7] - m->m[4]*m->m[6]) * inv_det;
  out->m[7] = -(m->m[0]*m->m[7] - m->m[1]*m->m[6]) * inv_det;
  out->m[8] =  (m->m[0]*m->m[4] - m->m[1]*m->m[3]) * inv_det;
  /* clang-format on */

  return true;
}

bool
mat4x4f_inv (const Mat4x4f_t *m, Mat4x4f_t *out)
{
  float det;
  Mat3x3f_t minor;

  GET_MINOR (0, 0);
  float c0 = mat3x3f_det (&minor);
  GET_MINOR (0, 1);
  float c1 = -mat3x3f_det (&minor);
  GET_MINOR (0, 2);
  float c2 = mat3x3f_det (&minor);
  GET_MINOR (0, 3);
  float c3 = -mat3x3f_det (&minor);

  det = m->m[0] * c0 + m->m[1] * c1 + m->m[2] * c2 + m->m[3] * c3;
  if (fabs (det) < 1e-6f)
    return false;

  float inv_det = 1.0f / det;

  for (int row = 0; row < 4; ++row)
    {
      for (int col = 0; col < 4; ++col)
        {
          GET_MINOR (row, col);
          float cofactor = mat3x3f_det (&minor);
          if ((row + col) % 2 != 0)
            cofactor = -cofactor;
          out->m[col * 4 + row] = cofactor * inv_det;
        }
    }

  return true;
}

Mat2x2f_t
mat2x2f_identity (void)
{
  /* clang-format off */
  return (Mat2x2f_t) {
    {
      1, 0,
      0, 1
    }
  };
  /* clang-format on */
}

Mat3x3f_t
mat3x3f_identity (void)
{
  /* clang-format off */
  return (Mat3x3f_t) {
    {
      1, 0, 0,
      0, 1, 0,
      0, 0, 1
    }
  };
  /* clang-format on */
}

Mat4x4f_t
mat4x4f_identity (void)
{
  /* clang-format off */
  return (Mat4x4f_t) {
    {
      1, 0, 0, 0,
      0, 1, 0, 0,
      0, 0, 1, 0,
      0, 0, 0, 1
    }
  };
  /* clang-format on */
}

Mat4x4f_t
mat4x4f_translate (const Mat4x4f_t *m, Vec3f_t v)
{
  /* clang-format off */
  Mat4x4f_t T = { .m = {
    1.0f, 0.0f, 0.0f, v.x,
    0.0f, 1.0f, 0.0f, v.y,
    0.0f, 0.0f, 1.0f, v.z,
    0.0f, 0.0f, 0.0f, 1.0f
  }};
  /* clang-format on */

  return mat4x4f_mul (m, &T);
}

Mat4x4f_t
mat4x4f_scale (const Mat4x4f_t *m, Vec3f_t v)
{
  /* clang-format off */
  Mat4x4f_t S = { .m = {
    v.x,  0.0f, 0.0f, 0.0f,
    0.0f, v.y,  0.0f, 0.0f,
    0.0f, 0.0f, v.z,  0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
  }};
  /* clang-format on */

  return mat4x4f_mul (m, &S);
}

Mat4x4f_t
mat4x4f_rotation (const Mat4x4f_t *m, Vec3f_t v)
{
  /* clang-format off */
  Mat4x4f_t Rx = { .m = {
    1.0f, 0.0f,       0.0f,        0.0f,
    0.0f, cosf(v.x), -sinf(v.x),   0.0f,
    0.0f, sinf(v.x),  cosf(v.x),   0.0f,
    0.0f, 0.0f,       0.0f,        1.0f
  }};

  Mat4x4f_t Ry = { .m = {
    cosf(v.y),  0.0f,  sinf(v.y),  0.0f,
    0.0f,       1.0f,  0.0f,       0.0f,
   -sinf(v.y),  0.0f,  cosf(v.y),  0.0f,
    0.0f,       0.0f,  0.0f,       1.0f
  }};

  Mat4x4f_t Rz = { .m = {
    cosf(v.z), -sinf(v.z),  0.0f, 0.0f,
    sinf(v.z),  cosf(v.z),  0.0f, 0.0f,
    0.0f,       0.0f,       1.0f, 0.0f,
    0.0f,       0.0f,       0.0f, 1.0f
  }};
  /* clang-format on */

  Mat4x4f_t R = mat4x4f_mul (&Rz, &Ry);
  R = mat4x4f_mul (&R, &Rx);

  return mat4x4f_mul (m, &R);
}

Mat4x4f_t
mat4x4f_lookat (Vec3f_t eye, Vec3f_t target, Vec3f_t up)
{
  Vec3f_t forward = vec3f_normalize (vec3f_sub (target, eye));
  Vec3f_t right = vec3f_normalize (vec3f_cross (forward, up));
  Vec3f_t true_up = vec3f_cross (right, forward);

  /* clang-format off */
  return (Mat4x4f_t){ .m = {
     right.x,                 true_up.x,                -forward.x,               0,
     right.y,                 true_up.y,                -forward.y,               0,
     right.z,                 true_up.z,                -forward.z,               0,
    -vec3f_dot(right, eye),  -vec3f_dot(true_up, eye),  -vec3f_dot(forward, eye), 1
  }};
  /* clang-format on */
}

Mat4x4f_t
mat4x4f_perspective (float angle, float ratio, float near, float far)
{
  float f = 1.0f / tanf (angle * 0.5f);

  /* clang-format off */
  return (Mat4x4f_t) {.m = {
    f / ratio,  0.0f, 0.0f,                             0.0f,
    0.0f,       f,    0.0f,                             0.0f,
    0.0f,       0.0f, (far + near) / (near - far),     -1.0f,
    0.0f,       0.0f, (2 * far * near) / (near - far),  0.0f
  }};
  /* clang-format on */
}

Mat4x4f_t
mat4x4f_frustum (float l, float r, float t, float b, float n, float f)
{
  /* clang-format off */
  return (Mat4x4f_t) {.m = {
    (2 * n) / (r - l), 0,                     (r + l) / (r - l),  0,
    0,                 (2 * n) / (t - b),     (t + b) / (t - b),  0,
    0,                 0,                    -(f + n) / (f - n),  (-2 * f * n) / (f - n),
    0,                 0,                    -1,                  0
  }};
  /* clang-format on */
}
