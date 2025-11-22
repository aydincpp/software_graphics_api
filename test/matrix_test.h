#ifndef MATRIX_TEST_H
#define MATRIX_TEST_H

#include <stdbool.h>

bool test_mat2x2f_add (void);
bool test_mat3x3f_add (void);
bool test_mat3x4f_add (void);
bool test_mat4x3f_add (void);
bool test_mat4x4f_add (void);

bool test_mat2x2f_sub (void);
bool test_mat3x3f_sub (void);
bool test_mat3x4f_sub (void);
bool test_mat4x3f_sub (void);
bool test_mat4x4f_sub (void);

bool test_mat2x2f_mul (void);
bool test_mat3x3f_mul (void);
bool test_mat4x4f_mul (void);

bool test_mat3x3f_mul_mat3x4f (void);
bool test_mat3x4f_mul_mat4x4f (void);
bool test_mat4x3f_mul_mat3x4f (void);
bool test_mat4x4f_mul_mat4x3f (void);

bool test_mat2x2f_mul_vec2f (void);
bool test_mat3x3f_mul_vec3f (void);
bool test_mat3x4f_mul_vec4f (void);
bool test_mat4x3f_mul_vec3f (void);
bool test_mat4x4f_mul_vec4f (void);

bool test_mat2x2f_mul_scalar (void);
bool test_mat3x3f_mul_scalar (void);
bool test_mat3x4f_mul_scalar (void);
bool test_mat4x3f_mul_scalar (void);
bool test_mat4x4f_mul_scalar (void);

bool test_mat2x2f_div_scalar (void);
bool test_mat3x3f_div_scalar (void);
bool test_mat3x4f_div_scalar (void);
bool test_mat4x3f_div_scalar (void);
bool test_mat4x4f_div_scalar (void);

bool test_mat2x2f_transpose (void);
bool test_mat3x3f_transpose (void);
bool test_mat3x4f_transpose (void);
bool test_mat4x3f_transpose (void);
bool test_mat4x4f_transpose (void);

bool test_mat2x2f_det (void);
bool test_mat3x3f_det (void);
bool test_mat4x4f_det (void);

bool test_mat2x2f_inv (void);
bool test_mat3x3f_inv (void);
bool test_mat4x4f_inv (void);

#endif
