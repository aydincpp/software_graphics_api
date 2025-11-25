#ifndef MATRIX_H
#define MATRIX_H

#include "math/vector.h"
#include <stdbool.h>

#ifndef M2x2_AT
#define M2x2_AT(m, r, c) ((m)->m[(r) * 2 + (c)])
#endif

#ifndef M3x3_AT
#define M3x3_AT(m, r, c) ((m)->m[(r) * 3 + (c)])
#endif

#ifndef M3x4_AT
#define M3x4_AT(m, r, c) ((m)->m[(r) * 4 + (c)])
#endif

#ifndef M4x3_AT
#define M4x3_AT(m, r, c) ((m)->m[(r) * 3 + (c)])
#endif

#ifndef M4x4_AT
#define M4x4_AT(m, r, c) ((m)->m[(r) * 4 + (c)])
#endif

typedef struct
{
  float m[4];
} Mat2x2f_t;

typedef struct
{
  float m[9];
} Mat3x3f_t;

typedef struct
{
  float m[12];
} Mat3x4f_t;

typedef struct
{
  float m[12];
} Mat4x3f_t;

typedef struct
{
  float m[16];
} Mat4x4f_t;

/*
 * --------------------------------------------------------------------
 * Zero matrix creation
 * Returns a matrix with all elements set to zero.
 * --------------------------------------------------------------------
 */

/**
 * @brief Returns a 2x2 zero matrix.
 */
Mat2x2f_t mat2x2f_zero (void);

/**
 * @brief Returns a 3x3 zero matrix.
 */
Mat3x3f_t mat3x3f_zero (void);

/**
 * @brief Returns a 3x4 zero matrix.
 */
Mat3x4f_t mat3x4f_zero (void);

/**
 * @brief Returns a 4x3 zero matrix.
 */
Mat4x3f_t mat4x3f_zero (void);

/**
 * @brief Returns a 4x4 zero matrix.
 */
Mat4x4f_t mat4x4f_zero (void);

/*
 * --------------------------------------------------------------------
 * Matrix addition
 * Returns a matrix resulting from element-wise addition of two input
 * matrices of the same size.
 * --------------------------------------------------------------------
 */

/**
 * @brief Adds two 2x2 matrices.
 */
Mat2x2f_t mat2x2f_add (const Mat2x2f_t *a, const Mat2x2f_t *b);

/**
 * @brief Adds two 3x3 matrices.
 */
Mat3x3f_t mat3x3f_add (const Mat3x3f_t *a, const Mat3x3f_t *b);

/**
 * @brief Adds two 3x4 matrices.
 */
Mat3x4f_t mat3x4f_add (const Mat3x4f_t *a, const Mat3x4f_t *b);

/**
 * @brief Adds two 4x3 matrices.
 */
Mat4x3f_t mat4x3f_add (const Mat4x3f_t *a, const Mat4x3f_t *b);

/**
 * @brief Adds two 4x4 matrices.
 */
Mat4x4f_t mat4x4f_add (const Mat4x4f_t *a, const Mat4x4f_t *b);

/*
 * --------------------------------------------------------------------
 * Matrix subtraction
 * Returns a matrix resulting from element-wise subtraction of the second
 * matrix from the first matrix of the same size.
 * --------------------------------------------------------------------
 */

/**
 * @brief Subtracts two 2x2 matrices.
 */
Mat2x2f_t mat2x2f_sub (const Mat2x2f_t *a, const Mat2x2f_t *b);

/**
 * @brief Subtracts two 3x3 matrices.
 */
Mat3x3f_t mat3x3f_sub (const Mat3x3f_t *a, const Mat3x3f_t *b);

/**
 * @brief Subtracts two 3x4 matrices.
 */
Mat3x4f_t mat3x4f_sub (const Mat3x4f_t *a, const Mat3x4f_t *b);

/**
 * @brief Subtracts two 4x3 matrices.
 */
Mat4x3f_t mat4x3f_sub (const Mat4x3f_t *a, const Mat4x3f_t *b);

/**
 * @brief Subtracts two 4x4 matrices.
 */
Mat4x4f_t mat4x4f_sub (const Mat4x4f_t *a, const Mat4x4f_t *b);

/*
 * --------------------------------------------------------------------
 * Matrix multiplication
 * Returns the product of two square matrices of the same dimension.
 * --------------------------------------------------------------------
 */

/**
 * @brief Multiplies two 2x2 matrices.
 */
Mat2x2f_t mat2x2f_mul (const Mat2x2f_t *a, const Mat2x2f_t *b);

/**
 * @brief Multiplies two 3x3 matrices.
 */
Mat3x3f_t mat3x3f_mul (const Mat3x3f_t *a, const Mat3x3f_t *b);

/**
 * @brief Multiplies two 4x4 matrices.
 */
Mat4x4f_t mat4x4f_mul (const Mat4x4f_t *a, const Mat4x4f_t *b);

/*
 * --------------------------------------------------------------------
 * Matrix mixed-dimension multiplication
 * Returns the product of matrices with compatible but different dimensions.
 * --------------------------------------------------------------------
 */

/**
 * @brief Multiplies a 3x3 matrix by a 3x4 matrix.
 */
Mat3x4f_t mat3x3f_mul_mat3x4f (const Mat3x3f_t *a, const Mat3x4f_t *b);

/**
 * @brief Multiplies a 3x4 matrix by a 4x4 matrix.
 */
Mat3x4f_t mat3x4f_mul_mat4x4f (const Mat3x4f_t *a, const Mat4x4f_t *b);

/**
 * @brief Multiplies a 4x3 matrix by a 3x4 matrix.
 */
Mat4x4f_t mat4x3f_mul_mat3x4f (const Mat4x3f_t *a, const Mat3x4f_t *b);

/**
 * @brief Multiplies a 4x4 matrix by a 4x3 matrix.
 */
Mat4x3f_t mat4x4f_mul_mat4x3f (const Mat4x4f_t *a, const Mat4x3f_t *b);

/*
 * --------------------------------------------------------------------
 * Matrix-vector multiplication
 * Returns the product of a matrix and a vector, transforming the vector
 * by the matrix.
 * --------------------------------------------------------------------
 */

/**
 * @brief Multiplies a 2x2 matrix by a 2D vector.
 */
Vec2f_t mat2x2f_mul_vec2f (const Mat2x2f_t *m, const Vec2f_t *v);

/**
 * @brief Multiplies a 3x3 matrix by a 3D vector.
 */
Vec3f_t mat3x3f_mul_vec3f (const Mat3x3f_t *m, const Vec3f_t *v);

/**
 * @brief Multiplies a 3x4 matrix by a 4D vector, returns a 3D vector.
 */
Vec3f_t mat3x4f_mul_vec4f (const Mat3x4f_t *m, const Vec4f_t *v);

/**
 * @brief Multiplies a 4x3 matrix by a 3D vector, returns a 4D vector.
 */
Vec4f_t mat4x3f_mul_vec3f (const Mat4x3f_t *m, const Vec3f_t *v);

/**
 * @brief Multiplies a 4x4 matrix by a 4D vector.
 */
Vec4f_t mat4x4f_mul_vec4f (const Mat4x4f_t *m, const Vec4f_t *v);

/*
 * --------------------------------------------------------------------
 * Scalar multiplication
 * Returns a matrix where each element is multiplied by the given scalar.
 * --------------------------------------------------------------------
 */

/**
 * @brief Multiplies each element of a 2x2 matrix by a scalar.
 */
Mat2x2f_t mat2x2f_mul_scalar (const Mat2x2f_t *m, float s);

/**
 * @brief Multiplies each element of a 3x3 matrix by a scalar.
 */
Mat3x3f_t mat3x3f_mul_scalar (const Mat3x3f_t *m, float s);

/**
 * @brief Multiplies each element of a 3x4 matrix by a scalar.
 */
Mat3x4f_t mat3x4f_mul_scalar (const Mat3x4f_t *m, float s);

/**
 * @brief Multiplies each element of a 4x3 matrix by a scalar.
 */
Mat4x3f_t mat4x3f_mul_scalar (const Mat4x3f_t *m, float s);

/**
 * @brief Multiplies each element of a 4x4 matrix by a scalar.
 */
Mat4x4f_t mat4x4f_mul_scalar (const Mat4x4f_t *m, float s);

/*
 * --------------------------------------------------------------------
 * Scalar division
 * Returns a matrix where each element is divided by the given scalar.
 * --------------------------------------------------------------------
 */

/**
 * @brief Divides each element of a 2x2 matrix by a scalar.
 */
Mat2x2f_t mat2x2f_div_scalar (const Mat2x2f_t *m, float s);

/**
 * @brief Divides each element of a 3x3 matrix by a scalar.
 */
Mat3x3f_t mat3x3f_div_scalar (const Mat3x3f_t *m, float s);

/**
 * @brief Divides each element of a 3x4 matrix by a scalar.
 */
Mat3x4f_t mat3x4f_div_scalar (const Mat3x4f_t *m, float s);

/**
 * @brief Divides each element of a 4x3 matrix by a scalar.
 */
Mat4x3f_t mat4x3f_div_scalar (const Mat4x3f_t *m, float s);

/**
 * @brief Divides each element of a 4x4 matrix by a scalar.
 */
Mat4x4f_t mat4x4f_div_scalar (const Mat4x4f_t *m, float s);

/*
 * --------------------------------------------------------------------
 * Matrix transpose
 * Returns the transpose of the input matrix, swapping rows and columns.
 * --------------------------------------------------------------------
 */

/**
 * @brief Transposes a 2x2 matrix.
 */
Mat2x2f_t mat2x2f_transpose (const Mat2x2f_t *m);

/**
 * @brief Transposes a 3x3 matrix.
 */
Mat3x3f_t mat3x3f_transpose (const Mat3x3f_t *m);

/**
 * @brief Transposes a 3x4 matrix, returning a 4x3 matrix.
 */
Mat4x3f_t mat3x4f_transpose (const Mat3x4f_t *m);

/**
 * @brief Transposes a 4x3 matrix, returning a 3x4 matrix.
 */
Mat3x4f_t mat4x3f_transpose (const Mat4x3f_t *m);

/**
 * @brief Transposes a 4x4 matrix.
 */
Mat4x4f_t mat4x4f_transpose (const Mat4x4f_t *m);

/*
 * --------------------------------------------------------------------
 * Determinant
 * Computes the determinant of the input matrix.
 * --------------------------------------------------------------------
 */

/**
 * @brief Calculates the determinant of a 2x2 matrix.
 */
float mat2x2f_det (const Mat2x2f_t *m);

/**
 * @brief Calculates the determinant of a 3x3 matrix.
 */
float mat3x3f_det (const Mat3x3f_t *m);

/**
 * @brief Calculates the determinant of a 4x4 matrix.
 */
float mat4x4f_det (const Mat4x4f_t *m);

/*
 * --------------------------------------------------------------------
 * Inverse matrix
 * Computes the inverse of the input matrix if it exists.
 * Returns true if inversion is successful, false if matrix is singular.
 * --------------------------------------------------------------------
 */

/**
 * @brief Computes the inverse of a 2x2 matrix.
 * @param m Pointer to the input matrix.
 * @param out Pointer to the output matrix where the inverse will be stored.
 * @return true if the matrix is invertible, false otherwise.
 */
bool mat2x2f_inv (const Mat2x2f_t *m, Mat2x2f_t *out);

/**
 * @brief Computes the inverse of a 3x3 matrix.
 * @param m Pointer to the input matrix.
 * @param out Pointer to the output matrix where the inverse will be stored.
 * @return true if the matrix is invertible, false otherwise.
 */
bool mat3x3f_inv (const Mat3x3f_t *m, Mat3x3f_t *out);

/**
 * @brief Computes the inverse of a 4x4 matrix.
 * @param m Pointer to the input matrix.
 * @param out Pointer to the output matrix where the inverse will be stored.
 * @return true if the matrix is invertible, false otherwise.
 */
bool mat4x4f_inv (const Mat4x4f_t *m, Mat4x4f_t *out);

/*
 * --------------------------------------------------------------------
 * Identity matrix
 * Returns an identity matrix of the specified size.
 * --------------------------------------------------------------------
 */

/**
 * @brief Returns a 2x2 identity matrix.
 */
Mat2x2f_t mat2x2f_identity (void);

/**
 * @brief Returns a 3x3 identity matrix.
 */
Mat3x3f_t mat3x3f_identity (void);

/**
 * @brief Returns a 4x4 identity matrix.
 */
Mat4x4f_t mat4x4f_identity (void);

/*
 * --------------------------------------------------------------------
 *  Transformation matrices
 *  Returns a matrix for translation, rotation or scaling transforms
 * --------------------------------------------------------------------
 */

/**
 * @brief Returns a 4x4 translate matrix.
 */
Mat4x4f_t mat4x4f_translate (const Mat4x4f_t *m, Vec3f_t v);

/**
 * @brief Returns a 4x4 scale matrix.
 */
Mat4x4f_t mat4x4f_scale (const Mat4x4f_t *m, Vec3f_t v);

/**
 * @brief Returns a 4x4 rotation matrix matrix for x-axis
 */
Mat4x4f_t mat4x4f_rotation (const Mat4x4f_t *m, Vec3f_t v);

/**
 * @brief Creates a view matrix for a camera.
 * @param eye Position of the camera.
 * @param target Point the camera is looking at.
 * @param up Up direction vector.
 * @return View matrix.
 */
Mat4x4f_t mat4x4f_lookat (Vec3f_t eye, Vec3f_t target, Vec3f_t up);

/**
 * @brief Creates a symmetric perspective projection matrix.
 * @param angle Vertical field of view in radians.
 * @param ratio Aspect ratio (width / height).
 * @param near Near clipping plane distance.
 * @param far Far clipping plane distance.
 * @return Perspective projection matrix.
 */
Mat4x4f_t mat4x4f_perspective (float angle, float ratio, float near,
                               float far);

/**
 * @brief Creates a perspective projection matrix with arbitrary frustum
 * bounds.
 * @param l Left boundary on the near clipping plane.
 * @param r Right boundary on the near clipping plane.
 * @param t Top boundary on the near clipping plane.
 * @param b Bottom boundary on the near clipping plane.
 * @param n Near clipping plane distance.
 * @param f Far clipping plane distance.
 * @return Projection matrix with specified frustum.
 */
Mat4x4f_t mat4x4f_frustum (float l, float r, float t, float b, float n,
                           float f);

#endif /* MATRIX_H */
