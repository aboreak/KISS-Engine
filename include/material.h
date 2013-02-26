#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "vector.h"

struct material {
	struct vec3 ambient;
	struct vec3 diffuse;
	struct vec3 specular;
	float shininess;
	char *diffuse_map_file_name;
};

struct material *material_new(const char *file_name);

void material_delete(struct material **material);

#endif
