#ifndef __MESH_H__
#define __MESH_H__

#include "vector.h"

struct array;

struct mesh {
	struct array *	indexed_vertices;
	struct array *	indexed_texcoords;
	struct array *	indexed_normals;
	struct vec3	center;
	char *		material_file_name;
};

struct mesh * mesh_new(const char *filename);

void mesh_delete(struct mesh **m);

unsigned int mesh_get_vertex_count(struct mesh *m);

#endif
