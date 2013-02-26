#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <math.h>

#define PI 3.14159265359

static inline float deg_to_rad(float deg)
{
	return (deg / 180) * PI;
}

static inline float rad_to_deg(float rad)
{
	return (rad / PI) * 180;
}

struct vec2i {
	int x;
	int y;
};

struct vec3i {
	int x;
	int y;
	int z;
};

struct vec2 {
	float x;
	float y;
};

struct vec3 {
	float x;
	float y;
	float z;
};

struct vec4 {
	float x;
	float y;
	float z;
	float w;
};

static inline struct vec2i vec2i_add(struct vec2i *a, struct vec2i *b)
{
	struct vec2i c;

	c.x = a->x + b->x;
	c.y = a->y + b->y;

	return c;
}

static inline struct vec2i vec2i_bub(struct vec2i *a, struct vec2i *b)
{
	struct vec2i c;

	c.x = a->x - b->x;
	c.y = a->y - b->y;

	return c;
}

static inline struct vec2i vec2i_neg(struct vec2i *v)
{
	struct vec2i n;

	n.x = -v->x;
	n.y = -v->y;

	return n;
}

static inline float vec2i_dot(struct vec2i *a, struct vec2i *b)
{
	return a->x * b->x + a->y * b->y;
}

static inline float vec2i_mag(struct vec2i *v)
{
	return sqrtf(v->x * v->x + v->y * v->y);
}

static inline struct vec2i *vec2i_zero(struct vec2i *v)
{
	v->x = 0;
	v->y = 0;

	return v;
}

static inline struct vec3i vec3i_add(struct vec3i *a, struct vec3i *b)
{
	struct vec3i c;

	c.x = a->x + b->x;
	c.y = a->y + b->y;
	c.z = a->z + b->z;

	return c;
}

static inline struct vec3i vec3i_sub(struct vec3i *a, struct vec3i *b)
{
	struct vec3i c;

	c.x = a->x - b->x;
	c.y = a->y - b->y;
	c.z = a->z - b->z;

	return c;
}

static inline struct vec3i vec3i_neg(struct vec3i *v)
{
	struct vec3i n;

	n.x = -v->x;
	n.y = -v->y;
	n.z = -v->z;

	return n;
}

static inline float vec3i_dot(struct vec3i *a, struct vec3i *b)
{
	return a->x * b->x + a->y * b->y + a->z * b->z;
}

static inline float vec3i_mag(struct vec3i *v)
{
	return sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
}

static inline struct vec3i vec3i_zero(struct vec3i *v)
{
	*v = (struct vec3i) {
	0, 0, 0};

	return *v;
}

static inline struct vec2 vec2_add(struct vec2 *a, struct vec2 *b)
{
	struct vec2 c;

	c.x = a->x + b->x;
	c.y = a->y + b->y;

	return c;
}

static inline struct vec2 vec2_sub(struct vec2 *a, struct vec2 *b)
{
	struct vec2 c;

	c.x = a->x - b->x;
	c.y = a->y - b->y;

	return c;
}

static inline struct vec2 vec2_neg(struct vec2 *v)
{
	struct vec2 n;

	n.x = -v->x;
	n.y = -v->y;

	return n;
}

static inline float vec2_dot(struct vec2 *a, struct vec2 *b)
{
	return a->x * b->x + a->y * b->y;
}

static inline float vec2_mag(struct vec2 *v)
{
	return sqrtf(v->x * v->x + v->y * v->y);
}

static inline struct vec2 *vec2_zero(struct vec2 *v)
{
	v->x = 0.0f;
	v->y = 0.0f;

	return v;
}

static inline struct vec2 vec2_norm(struct vec2 *v)
{
	struct vec2 n;
	float m = vec2_mag(v);

	n.x = v->x / m;
	n.y = v->y / m;

	return n;
}

static inline struct vec3 vec3_add(struct vec3 *a, struct vec3 *b)
{
	struct vec3 c;

	c.x = a->x + b->x;
	c.y = a->y + b->y;
	c.z = a->z + b->z;

	return c;
}

static inline struct vec3 vec3_sub(struct vec3 *a, struct vec3 *b)
{
	struct vec3 c;

	c.x = a->x - b->x;
	c.y = a->y - b->y;
	c.z = a->z - b->z;

	return c;
}

static inline struct vec3 vec3_neg(struct vec3 *v)
{
	struct vec3 n;

	n.x = -v->x;
	n.y = -v->y;
	n.z = -v->z;

	return n;
}

static inline float vec3_dot(struct vec3 *a, struct vec3 *b)
{
	return a->x * b->x + a->y * b->y + a->z * b->z;
}

static inline struct vec3 vec3_cross(struct vec3 *a, struct vec3 *b)
{
	struct vec3 c;

	c.x = a->y * b->z - a->z * b->y;
	c.y = a->z * b->x - a->x * b->z;
	c.z = a->x * b->y - a->y * b->x;

	return c;
}

static inline float vec3_mag(struct vec3 *v)
{
	return sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
}

static inline struct vec3 *vec3_zero(struct vec3 *v)
{
	v->x = 0.0f;
	v->y = 0.0f;
	v->z = 0.0f;

	return v;
}

static inline struct vec3 vec3_norm(struct vec3 *v)
{
	struct vec3 n;
	float m = vec3_mag(v);

	n.x = v->x / m;
	n.y = v->y / m;
	n.z = v->z / m;

	return n;
}

static inline struct vec4 vec4_add(struct vec4 *a, struct vec4 *b)
{
	struct vec4 c;

	c.x = a->x + b->x;
	c.y = a->y + b->y;
	c.z = a->z + b->z;
	c.w = a->w + b->w;

	return c;
}

static inline struct vec4 vec4_sub(struct vec4 *a, struct vec4 *b)
{
	struct vec4 c;

	c.x = a->x - b->x;
	c.y = a->y - b->y;
	c.z = a->z - b->z;
	c.w = a->w - b->w;

	return c;
}

static inline struct vec4 vec4_neg(struct vec4 *v)
{
	struct vec4 n;

	n.x = -v->x;
	n.y = -v->y;
	n.z = -v->z;
	n.w = -v->w;

	return n;
}

static inline float vec4_dot(struct vec4 *a, struct vec4 *b)
{
	return a->x * b->x + a->y * b->y + a->z * b->z + a->w * b->w;
}

static inline float vec4_mag(struct vec4 *v)
{
	return sqrtf(v->x * v->x + v->y * v->y + v->z * v->z + v->w * v->w);
}

static inline struct vec4 *vec4_zero(struct vec4 *v)
{
	v->x = 0.0f;
	v->y = 0.0f;
	v->z = 0.0f;
	v->w = 0.0f;

	return v;
}

static inline struct vec4 vec4_norm(struct vec4 *v)
{
	struct vec4 n;
	float m = vec4_mag(v);

	n.x = v->x / m;
	n.y = v->y / m;
	n.z = v->z / m;
	n.w = v->w / m;

	return n;
}

#endif
