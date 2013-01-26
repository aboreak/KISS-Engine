#ifndef __MODEL_H__
#define __MODEL_H__

#include "matrix.h"

struct mesh;
struct material;
struct texture;

struct model {
	struct vec3		translation;
	struct vec3		rotation;
	struct vec3		center;
	unsigned		id[3];
	struct mesh *		mesh;
	struct material *	material;
	struct texture *	texture;
};

#define model_center(x) (x->center)

struct model * model_new(const char *filename);

void model_delete(struct model **model);

void model_draw(struct model *model);

void model_draw_with_matrix(struct model *model, struct mat4 *mat);

void model_translate(struct model *model, float x, float y, float z);

void model_rotate(struct model *model, float pitch, float yaw, float roll);

struct mat4 model_matrix(struct model *model);

#endif
