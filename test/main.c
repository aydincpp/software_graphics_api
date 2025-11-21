#include "matrix_test.h"

int
main (void)
{
  // matrix tests
  test_mat2x2f_add ();
  test_mat3x3f_add ();
  test_mat3x4f_add ();
  test_mat4x3f_add ();
  test_mat4x4f_add ();

  test_mat2x2f_sub ();
  test_mat3x3f_sub ();
  test_mat3x4f_sub ();
  test_mat4x4f_sub ();

  test_mat2x2f_mul ();
  test_mat3x3f_mul ();
  test_mat4x4f_mul ();

  test_mat3x3f_mul_mat3x4f ();
  test_mat3x4f_mul_mat4x4f ();
  test_mat4x3f_mul_mat3x4f ();
  test_mat4x4f_mul_mat4x3f ();

  test_mat2x2f_mul_vec2f ();
  test_mat3x3f_mul_vec3f ();
  test_mat3x4f_mul_vec4f ();
  test_mat4x3f_mul_vec3f ();
  test_mat4x4f_mul_vec4f ();

  test_mat2x2f_mul_scalar ();
  test_mat3x3f_mul_scalar ();
  test_mat3x4f_mul_scalar ();
  test_mat4x3f_mul_scalar ();
  test_mat4x4f_mul_scalar ();

  test_mat2x2f_div_scalar ();
  test_mat3x3f_div_scalar ();
  test_mat3x4f_div_scalar ();
  test_mat4x3f_div_scalar ();
  test_mat4x4f_div_scalar ();

  test_mat2x2f_transpose ();
  test_mat3x3f_transpose ();
  test_mat3x4f_transpose ();
  test_mat4x3f_transpose ();
  test_mat4x4f_transpose ();

  test_mat2x2f_det ();
  test_mat3x3f_det ();
  test_mat4x4f_det ();

  test_mat2x2f_inv ();
  test_mat3x3f_inv ();
  test_mat4x4f_inv ();

  return 0;
}
