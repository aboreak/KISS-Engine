#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "vector.h"
#include "algorithm.h"

struct mat2 {
	struct vec2 x;
	struct vec2 y;
};

struct mat3 {
	struct vec3 x;
	struct vec3 y;
	struct vec3 z;
};

struct mat4 {
	struct vec4 x;
	struct vec4 y;
	struct vec4 z;
	struct vec4 w;
};

static inline struct mat2 mat2_zero()
{
	struct mat2 tmp;

	tmp.x = (struct vec2) {
	0, 0};
	tmp.y = (struct vec2) {
	0, 0};

	return tmp;
}

static inline struct mat2 mat2_rotate(float angle)
{
	struct mat2 tmp;

	tmp.x = (struct vec2) {
	cos(angle), -sin(angle)};
	tmp.y = (struct vec2) {
	sin(angle), cos(angle)};

	return tmp;
}

static inline float mat2_determinant(struct mat2 *mat)
{
	return mat->x.x * mat->y.y - mat->x.y * mat->y.x;
}

static inline struct mat2 mat2_from_mat3(struct mat3 *mat, unsigned int x,
					 unsigned int y)
{
	struct mat2 tmp;
	if (x >= 2 || y >= 2)
		return tmp;

	float *m = (float *)mat;
	tmp.x.x = m[x + y * 3];
	tmp.x.y = m[x + 1 + y * 3];
	tmp.y.x = m[x + 4 + y * 3];
	tmp.y.y = m[x + 5 + y * 3];

	return tmp;
}

static inline struct mat2 mat2_from_mat4(struct mat4 *mat, unsigned int x,
					 unsigned int y)
{
	struct mat2 tmp;
	if (x >= 3 || y >= 3)
		return tmp;

	float *m = (float *)mat;
	tmp.x.x = m[x + y * 4];
	tmp.x.y = m[x + 1 + y * 4];
	tmp.y.x = m[x + 4 + y * 4];
	tmp.y.y = m[x + 5 + y * 4];

	return tmp;
}

static inline struct mat3 mat3_zero()
{
	struct mat3 tmp;

	tmp.x = (struct vec3) {
	0, 0, 0};
	tmp.y = (struct vec3) {
	0, 0, 0};
	tmp.z = (struct vec3) {
	0, 0, 0};

	return tmp;
}

static inline struct mat3 mat3_multf(struct mat3 *mat, float w)
{
	struct mat3 tmp = *mat;

	tmp.x.x *= w;
	tmp.x.y *= w;
	tmp.x.z *= w;
	tmp.y.x *= w;
	tmp.y.y *= w;
	tmp.y.z *= w;
	tmp.z.x *= w;
	tmp.z.y *= w;
	tmp.z.z *= w;

	return tmp;
}

static inline struct mat3 mat3_divf(struct mat3 *mat, float w)
{
	struct mat3 tmp = *mat;

	tmp.x.x /= w;
	tmp.x.y /= w;
	tmp.x.z /= w;
	tmp.y.x /= w;
	tmp.y.y /= w;
	tmp.y.z /= w;
	tmp.z.x /= w;
	tmp.z.y /= w;
	tmp.z.z /= w;

	return tmp;
}

static inline struct mat3 mat3_mult(struct mat3 *m, struct mat3 *n)
{
	struct mat3 tmp;

	tmp.x.x = m->x.x * n->x.x + m->x.y * n->y.x + m->x.z * n->z.x;
	tmp.x.y = m->x.x * n->x.y + m->x.y * n->y.y + m->x.z * n->z.y;
	tmp.x.z = m->x.x * n->x.z + m->x.y * n->y.z + m->x.z * n->z.z;
	tmp.y.x = m->y.x * n->x.x + m->y.y * n->y.x + m->y.z * n->z.x;
	tmp.y.y = m->y.x * n->x.y + m->y.y * n->y.y + m->y.z * n->z.y;
	tmp.y.z = m->y.x * n->x.z + m->y.y * n->y.z + m->y.z * n->z.z;
	tmp.z.x = m->z.x * n->x.x + m->z.y * n->y.x + m->z.z * n->z.x;
	tmp.z.y = m->z.x * n->x.y + m->z.y * n->y.y + m->z.z * n->z.y;
	tmp.z.z = m->z.x * n->x.z + m->z.y * n->y.z + m->z.z * n->z.z;

	return tmp;
}

static inline struct mat3 mat3_rotate_x(float angle)
{
	struct mat3 tmp;

	tmp.x.x = 1.0;
	tmp.x.y = 0.0;
	tmp.x.z = 0.0;
	tmp.y.x = 0.0;
	tmp.y.y = cos(angle);
	tmp.y.z = sin(angle);
	tmp.z.x = 0.0;
	tmp.z.y = -sin(angle);
	tmp.z.z = cos(angle);

	return tmp;
}

static inline struct mat3 mat3_rotate_y(float angle)
{
	struct mat3 tmp;

	tmp.x.x = cos(angle);
	tmp.x.y = 0.0;
	tmp.x.z = -sin(angle);
	tmp.y.x = 0.0;
	tmp.y.y = 1.0;
	tmp.y.z = 0.0;
	tmp.z.x = sin(angle);
	tmp.z.y = 0.0;
	tmp.z.z = cos(angle);

	return tmp;
}

static inline struct mat3 mat3_rotate_z(float angle)
{
	struct mat3 tmp;

	tmp.x.x = cos(angle);
	tmp.x.y = sin(angle);
	tmp.x.z = 0.0;
	tmp.y.x = -sin(angle);
	tmp.y.y = cos(angle);
	tmp.y.z = 0.0f;
	tmp.z.x = 0.0;
	tmp.z.y = 0.0;
	tmp.z.z = 1.0;

	return tmp;
}

static inline struct mat3 mat3_rotate_vec3(struct vec3 *e)
{
	struct mat3 tmp;

	tmp.x.x = cos(e->y) * cos(e->z) + sin(e->y) * sin(e->x) * sin(e->z);
	tmp.x.y = sin(e->z) * cos(e->x);
	tmp.x.z = -sin(e->y) * cos(e->z) + cos(e->y) * sin(e->x) * sin(e->z);

	tmp.y.x = -cos(e->y) * sin(e->z) + sin(e->y) * sin(e->x) * cos(e->z);
	tmp.y.y = cos(e->z) * cos(e->x);
	tmp.y.z = sin(e->z) * sin(e->y) + cos(e->y) * sin(e->x) * cos(e->z);

	tmp.z.x = sin(e->y) * cos(e->x);
	tmp.z.y = -sin(e->x);
	tmp.z.z = cos(e->y) * cos(e->x);

	return tmp;
}

static inline struct mat3 mat3_transpose(struct mat3 *mat)
{
	struct mat3 tmp = *mat;

	swapf(&tmp.x.y, &tmp.y.x);
	swapf(&tmp.x.z, &tmp.z.x);
	swapf(&tmp.y.z, &tmp.z.y);

	return tmp;
}

static inline struct mat2 mat3_delete_col_row(struct mat3 *mat,
					      unsigned int col,
					      unsigned int row)
{
	struct mat2 tmp;
	float *m = (float *)&tmp;
	float *n = (float *)mat;

	size_t i, j, k;
	k = 0;
	for (j = 0; j < 3; j++) {
		if (j == row)
			continue;
		for (i = 0; i < 3; i++) {
			if (i == col)
				continue;
			m[k] = n[i + j * 3];
			k++;
		}
	}

	return tmp;
}

static inline float mat3_minor(struct mat3 *mat, unsigned int col,
			       unsigned int row)
{
	struct mat2 submat = mat3_delete_col_row(mat, col, row);

	return mat2_determinant(&submat);
}

static inline float mat3_cofactor(struct mat3 *mat, unsigned int col,
				  unsigned int row)
{
	float cofactor = mat3_minor(mat, col, row);

	if ((col + row) % 2 == 0)
		return cofactor;

	return -cofactor;
}

static inline float mat3_determinant(struct mat3 *mat)
{
	return mat->x.x * mat3_minor(mat, 0, 0) -
	    mat->x.y * mat3_minor(mat, 1, 0) + mat->x.z * mat3_minor(mat, 2, 0);
}

static inline struct mat3 mat3_adjoint(struct mat3 *mat)
{
	struct mat3 tmp;

	tmp.x.x = mat3_cofactor(mat, 0, 0);
	tmp.x.y = mat3_cofactor(mat, 1, 0);
	tmp.x.z = mat3_cofactor(mat, 2, 0);
	tmp.y.x = mat3_cofactor(mat, 0, 1);
	tmp.y.y = mat3_cofactor(mat, 1, 1);
	tmp.y.z = mat3_cofactor(mat, 2, 1);
	tmp.z.x = mat3_cofactor(mat, 0, 2);
	tmp.z.y = mat3_cofactor(mat, 1, 2);
	tmp.z.z = mat3_cofactor(mat, 2, 2);

	tmp = mat3_transpose(&tmp);

	return tmp;
}

static inline struct mat3 mat3_inverse(struct mat3 *mat)
{
	struct mat3 tmp = mat3_adjoint(mat);
	float det = mat3_determinant(mat);

	return mat3_divf(&tmp, det);
}

static inline struct mat3 mat3_from_mat4(struct mat4 *mat, unsigned int x,
					 unsigned int y)
{
	struct mat3 tmp;
	if (x >= 2 || y >= 2)
		return tmp;

	float *m = (float *)mat;
	tmp.x.x = m[x + y * 4];
	tmp.x.y = m[x + 1 + y * 4];
	tmp.x.z = m[x + 2 + y * 4];
	tmp.y.x = m[x + 4 + y * 4];
	tmp.y.y = m[x + 5 + y * 4];
	tmp.y.z = m[x + 6 + y * 4];
	tmp.z.x = m[x + 8 + y * 4];
	tmp.z.y = m[x + 9 + y * 4];
	tmp.z.z = m[x + 10 + y * 4];

	return tmp;
}

static inline struct vec3 vec3_mult_mat3(struct vec3 *vec, struct mat3 *mat)
{
	struct vec3 res;

	res.x = vec->x * mat->x.x + vec->y * mat->y.x + vec->z * mat->z.x;
	res.y = vec->x * mat->x.y + vec->y * mat->y.y + vec->z * mat->z.y;
	res.z = vec->x * mat->x.z + vec->y * mat->y.z + vec->z * mat->z.z;

	return res;
}

static inline struct vec3 vec3_rotate_y(struct vec3 *vec, float angle)
{
	struct mat3 mat;

	mat = mat3_rotate_y(angle);

	return vec3_mult_mat3(vec, &mat);
}

static inline struct vec3 vec3_rotate_x(struct vec3 *vec, float angle)
{
	struct mat3 mat;

	mat = mat3_rotate_x(angle);

	return vec3_mult_mat3(vec, &mat);
}

static inline struct mat4 mat4_zero()
{
	struct mat4 tmp;

	tmp.x = (struct vec4) {
	0, 0, 0, 0};
	tmp.y = (struct vec4) {
	0, 0, 0, 0};
	tmp.z = (struct vec4) {
	0, 0, 0, 0};
	tmp.w = (struct vec4) {
	0, 0, 0, 0};

	return tmp;
}

static inline struct mat4 mat4_multf(struct mat4 *mat, float s)
{
	struct mat4 tmp;

	tmp.x.x *= s;
	tmp.x.y *= s;
	tmp.x.z *= s;
	tmp.x.w *= s;
	tmp.y.x *= s;
	tmp.y.y *= s;
	tmp.y.z *= s;
	tmp.y.w *= s;
	tmp.z.x *= s;
	tmp.z.y *= s;
	tmp.z.z *= s;
	tmp.z.w *= s;
	tmp.w.x *= s;
	tmp.w.y *= s;
	tmp.w.z *= s;
	tmp.w.w *= s;

	return tmp;
}

static inline struct mat4 mat4_divf(struct mat4 *mat, float s)
{
	struct mat4 tmp;

	tmp.x.x /= s;
	tmp.x.y /= s;
	tmp.x.z /= s;
	tmp.x.w /= s;
	tmp.y.x /= s;
	tmp.y.y /= s;
	tmp.y.z /= s;
	tmp.y.w /= s;
	tmp.z.x /= s;
	tmp.z.y /= s;
	tmp.z.z /= s;
	tmp.z.w /= s;
	tmp.w.x /= s;
	tmp.w.y /= s;
	tmp.w.z /= s;
	tmp.w.w /= s;

	return tmp;
}

static inline struct mat4 mat4_mult(struct mat4 *a, struct mat4 *b)
{
	struct mat4 c;

	c.x.x = a->x.x * b->x.x + a->x.y * b->y.x + a->x.z * b->z.x +
	    a->x.w * b->w.x;
	c.x.y = a->x.x * b->x.y + a->x.y * b->y.y + a->x.z * b->z.y +
	    a->x.w * b->w.y;
	c.x.z = a->x.x * b->x.z + a->x.y * b->y.z + a->x.z * b->z.z +
	    a->x.w * b->w.z;
	c.x.w = a->x.x * b->x.w + a->x.y * b->y.w + a->x.z * b->z.w +
	    a->x.w * b->w.w;

	c.y.x = a->y.x * b->x.x + a->y.y * b->y.x + a->y.z * b->z.x +
	    a->y.w * b->w.x;
	c.y.y = a->y.x * b->x.y + a->y.y * b->y.y + a->y.z * b->z.y +
	    a->y.w * b->w.y;
	c.y.z = a->y.x * b->x.z + a->y.y * b->y.z + a->y.z * b->z.z +
	    a->y.w * b->w.z;
	c.y.w = a->y.x * b->x.w + a->y.y * b->y.w + a->y.z * b->z.w +
	    a->y.w * b->w.w;

	c.z.x = a->z.x * b->x.x + a->z.y * b->y.x + a->z.z * b->z.x +
	    a->z.w * b->w.x;
	c.z.y = a->z.x * b->x.y + a->z.y * b->y.y + a->z.z * b->z.y +
	    a->z.w * b->w.y;
	c.z.z = a->z.x * b->x.z + a->z.y * b->y.z + a->z.z * b->z.z +
	    a->z.w * b->w.z;
	c.z.w = a->z.x * b->x.w + a->z.y * b->y.w + a->z.z * b->z.w +
	    a->z.w * b->w.w;

	c.w.x = a->w.x * b->x.x + a->w.y * b->y.x + a->w.z * b->z.x +
	    a->w.w * b->w.x;
	c.w.y = a->w.x * b->x.y + a->w.y * b->y.y + a->w.z * b->z.y +
	    a->w.w * b->w.y;
	c.w.z = a->w.x * b->x.z + a->w.y * b->y.z + a->w.z * b->z.z +
	    a->w.w * b->w.z;
	c.w.w = a->w.x * b->x.w + a->w.y * b->y.w + a->w.z * b->z.w +
	    a->w.w * b->w.w;

	return c;
}

static inline struct mat4 mat4_identity()
{
	struct mat4 tmp;

	tmp.x = (struct vec4) {
	1, 0, 0, 0};
	tmp.y = (struct vec4) {
	0, 1, 0, 0};
	tmp.z = (struct vec4) {
	0, 0, 1, 0};
	tmp.w = (struct vec4) {
	0, 0, 0, 1};

	return tmp;
}

static inline void mat4_translate(struct mat4 *mat, float x, float y, float z)
{
	mat->w.x += x;
	mat->w.y += y;
	mat->w.z += z;
}

static inline struct mat4 mat4_translate_vec3(struct vec3 *v)
{
	struct mat4 tmp;

	tmp.x = (struct vec4) {
	1, 0, 0, 0};
	tmp.y = (struct vec4) {
	0, 1, 0, 0};
	tmp.z = (struct vec4) {
	0, 0, 1, 0};
	tmp.w = (struct vec4) {
	v->x, v->y, v->z, 1};

	return tmp;
}

static inline struct mat4 mat4_rotate_x(float angle)
{
	struct mat4 tmp;

	tmp.x.x = 1.0;
	tmp.x.y = 0.0;
	tmp.x.z = 0.0;
	tmp.x.w = 0.0;

	tmp.y.x = 0.0;
	tmp.y.y = cos(angle);
	tmp.y.z = sin(angle);
	tmp.y.w = 0.0;

	tmp.z.x = 0.0;
	tmp.z.y = -sin(angle);
	tmp.z.z = cos(angle);
	tmp.z.w = 0.0;

	tmp.w.x = 0.0;
	tmp.w.y = 0.0;
	tmp.w.z = 0.0;
	tmp.w.w = 1.0;

	return tmp;
}

static inline struct mat4 mat4_rotate_y(float angle)
{
	struct mat4 tmp;

	tmp.x.x = cos(angle);
	tmp.x.y = 0.0;
	tmp.x.z = -sin(angle);
	tmp.x.w = 0.0;

	tmp.y.x = 0.0;
	tmp.y.y = 1.0;
	tmp.y.z = 0.0;
	tmp.y.w = 0.0;

	tmp.z.x = sin(angle);
	tmp.z.y = 0.0;
	tmp.z.z = cos(angle);
	tmp.z.w = 0.0;

	tmp.w.x = 0.0;
	tmp.w.y = 0.0;
	tmp.w.z = 0.0;
	tmp.w.w = 1.0;

	return tmp;
}

static inline struct mat4 mat4_rotate_z(float angle)
{
	struct mat4 tmp;

	tmp.x.x = cos(angle);
	tmp.x.y = sin(angle);
	tmp.x.z = 0.0;
	tmp.x.w = 0.0;

	tmp.y.x = -sin(angle);
	tmp.y.y = cos(angle);
	tmp.y.z = 0.0;
	tmp.y.w = 0.0;

	tmp.z.x = 0.0;
	tmp.z.y = 0.0;
	tmp.z.z = 1.0;
	tmp.z.w = 0.0;

	tmp.w.x = 0.0;
	tmp.w.y = 0.0;
	tmp.w.z = 0.0;
	tmp.w.w = 1.0;

	return tmp;
}

static inline struct mat4 mat4_rotate(float angle, float x, float y, float z)
{
	struct mat4 tmp;

	tmp.x = (struct vec4) {
	x *x * (1 - cos(angle)) + cos(angle),
		    x * y * (1 - cos(angle)) + z * sin(angle),
		    x * z * (1 - cos(angle)) - y * sin(angle), 0};

	tmp.y = (struct vec4) {
	x *y * (1 - cos(angle)) - z * sin(angle),
		    y * y * (1 - cos(angle)) + cos(angle),
		    z * y * (1 - cos(angle)) + x * sin(angle), 0};

	tmp.z = (struct vec4) {
	x *z * (1 - cos(angle)) + y * sin(angle),
		    y * z * (1 - cos(angle)) - x * sin(angle),
		    z * z * (1 - cos(angle)) + cos(angle), 0};

	return tmp;
}

static inline struct mat4 mat4_rotate_vec3(struct vec3 *e)
{
	struct mat4 tmp;

	tmp.x.x = cos(e->y) * cos(e->z) + sin(e->y) * sin(e->x) * sin(e->z);
	tmp.x.y = sin(e->z) * cos(e->x);
	tmp.x.z = -sin(e->y) * cos(e->z) + cos(e->y) * sin(e->x) * sin(e->z);

	tmp.y.x = -cos(e->y) * sin(e->z) + sin(e->y) * sin(e->x) * cos(e->z);
	tmp.y.y = cos(e->z) * cos(e->x);
	tmp.y.z = sin(e->z) * sin(e->y) + cos(e->y) * sin(e->x) * cos(e->z);

	tmp.z.x = sin(e->y) * cos(e->x);
	tmp.z.y = -sin(e->x);
	tmp.z.z = cos(e->y) * cos(e->x);

	return tmp;
}

static inline struct mat4 mat4_transpose(struct mat4 *mat)
{
	struct mat4 tmp;

	swapf(&tmp.x.y, &tmp.y.x);
	swapf(&tmp.x.z, &tmp.z.x);
	swapf(&tmp.x.w, &tmp.w.x);
	swapf(&tmp.y.z, &tmp.z.y);
	swapf(&tmp.y.w, &tmp.w.y);
	swapf(&tmp.z.w, &tmp.w.z);

	return tmp;
}

static inline struct mat3 mat4_delete_col_row(struct mat4 *mat,
					      unsigned int col,
					      unsigned int row)
{
	struct mat3 tmp;
	float *m = (float *)&tmp;
	float *n = (float *)mat;

	size_t i, j, k;
	k = 0;
	for (j = 0; j < 4; j++) {
		if (j == row)
			continue;
		for (i = 0; i < 4; i++) {
			if (i == col)
				continue;
			m[k] = n[i + j * 4];
			k++;
		}
	}

	return tmp;

}

static inline float mat4_minor(struct mat4 *mat, unsigned int col,
			       unsigned int row)
{
	struct mat3 tmp = mat4_delete_col_row(mat, col, row);
	return mat3_determinant(&tmp);
}

static inline float mat4_cofactor(struct mat4 *mat, unsigned int col,
				  unsigned int row)
{
	float cofactor = mat4_minor(mat, col, row);
	if ((col + row) % 2 == 0)
		return cofactor;
	return -cofactor;
}

static inline float mat4_determinant(struct mat4 *mat)
{
	return mat->x.x * mat4_minor(mat, 0, 0) -
	    mat->x.y * mat4_minor(mat, 1, 0) +
	    mat->x.z * mat4_minor(mat, 2, 0) - mat->x.w * mat4_minor(mat, 3, 0);
}

static inline struct mat4 mat4_adjoint(struct mat4 *mat)
{
	struct mat4 tmp;

	tmp.x.x = mat4_cofactor(mat, 0, 0);
	tmp.x.y = mat4_cofactor(mat, 1, 0);
	tmp.x.z = mat4_cofactor(mat, 2, 0);
	tmp.x.w = mat4_cofactor(mat, 3, 0);

	tmp.y.x = mat4_cofactor(mat, 0, 1);
	tmp.y.y = mat4_cofactor(mat, 1, 1);
	tmp.y.z = mat4_cofactor(mat, 2, 1);
	tmp.y.w = mat4_cofactor(mat, 3, 1);

	tmp.z.x = mat4_cofactor(mat, 0, 2);
	tmp.z.y = mat4_cofactor(mat, 1, 2);
	tmp.z.z = mat4_cofactor(mat, 2, 2);
	tmp.z.w = mat4_cofactor(mat, 3, 2);

	tmp.w.x = mat4_cofactor(mat, 0, 3);
	tmp.w.y = mat4_cofactor(mat, 1, 3);
	tmp.w.z = mat4_cofactor(mat, 2, 3);
	tmp.w.w = mat4_cofactor(mat, 3, 3);

	tmp = mat4_transpose(&tmp);

	return tmp;
}

static inline struct mat4 mat4_inverse(struct mat4 *mat)
{
	struct mat4 tmp = mat4_adjoint(mat);
	float det = mat4_determinant(mat);

	return mat4_divf(&tmp, det);
}

static inline struct vec4 vec4_mult_mat4(struct vec4 *vec, struct mat4 *mat)
{
	struct vec4 result;

	result.x = vec->x * mat->x.x + vec->y * mat->y.x + vec->z * mat->z.x +
	    vec->w * mat->w.x;
	result.y = vec->x * mat->x.y + vec->y * mat->y.y + vec->z * mat->z.y +
	    vec->w * mat->w.y;
	result.z = vec->x * mat->x.z + vec->y * mat->y.z + vec->z * mat->z.z +
	    vec->w * mat->w.z;
	result.w = vec->x * mat->x.w + vec->y * mat->y.w + vec->z * mat->z.w +
	    vec->w * mat->w.w;

	return result;
}

static inline void mat4_ortho(struct mat4 *mat, float zoom_x, float zoom_y,
			      float z_near, float z_far)
{
	float d = -(z_far + z_near) / (z_far - z_near);
	float e = -2 / (z_far - z_near);

	mat->x = (struct vec4) {
	zoom_x, 0, 0, 0};
	mat->y = (struct vec4) {
	0, zoom_y, 0, 0};
	mat->z = (struct vec4) {
	0, 0, e, 0};
	mat->w = (struct vec4) {
	0, 0, d, 1};
}

static inline void mat4_perspective(struct mat4 *mat, float fov,
				    float aspect_ratio, float z_near,
				    float z_far)
{
	fov = deg_to_rad(fov);

	float zoom = 1 / tan(fov / 2);
	float d = -(z_far + z_near) / (z_far - z_near);
	float e = -2 * z_near * z_far / (z_far - z_near);

	mat->x = (struct vec4) {
	zoom, 0, 0, 0};
	mat->y = (struct vec4) {
	0, zoom * aspect_ratio, 0, 0};
	mat->z = (struct vec4) {
	0, 0, d, -1};
	mat->w = (struct vec4) {
	0, 0, e, 0};
}

static inline struct mat4 mat4_look_at(struct vec3 *pos, struct vec3 *target,
				       struct vec3 *up)
{
	struct vec3 z = vec3_sub(target, pos);
	struct vec3 zdir = vec3_norm(&z);
	struct vec3 x = vec3_cross(up, &zdir);
	struct vec3 xdir = vec3_norm(&x);
	struct vec3 ydir = vec3_cross(&zdir, &xdir);

	struct mat4 orientation;
	orientation.x = (struct vec4) {
	xdir.x, xdir.y, xdir.z, 0};
	orientation.y = (struct vec4) {
	ydir.x, ydir.y, ydir.z, 0};
	orientation.z = (struct vec4) {
	zdir.x, zdir.y, zdir.z, 0};
	orientation.w = (struct vec4) {
	0, 0, 0, 1};

	struct mat4 translation;
	translation.x = (struct vec4) {
	1, 0, 0, 0};
	translation.y = (struct vec4) {
	0, 1, 0, 0};
	translation.z = (struct vec4) {
	0, 0, 1, 0};
	translation.w = (struct vec4) {
	-pos->x, -pos->y, -pos->z, 1};

	return mat4_mult(&orientation, &translation);
}

#endif
