#include <GL/glew.h>
#include <GL/gl.h>

#include "mesh.h"
#include "array.h"
#include "material.h"
#include "image.h"
#include "common.h"

#define LINE_LENGTH 64

static void parse_face_token(char *token, struct vec3i *f)
{
	f->x = atoi(token);

	char *c = strchr(token, '/');
	if (c == NULL)
		return;
	
	c++;
	if (*c == '/') {
		c++;
		f->z = atoi(c);
		return;
	}

	f->y = atoi(c);
	c = strchr(c, '/');
	if (c == NULL)
		return;

	c++;
	f->z = atoi(c);

	return;
}

static void parse_file(FILE *file, struct mesh *mesh, struct array *vertices,
	struct array *texcoords, struct array *normals, struct array *faces)
{
	char line[LINE_LENGTH];
	while (fgets(line, LINE_LENGTH, file)) {
		char *token;
		token = strtok(line, " ");
		if (strcmp(token, "mtllib") == 0) {
			token = strtok(NULL, " \n\r");
			mesh->material_file_name = malloc(strlen(token));
			strcpy(mesh->material_file_name, token);
		} else if (strcmp(token, "v") == 0) {
			struct vec3 v;
			token = strtok(NULL, " ");
			if (token)
				v.x = atof(token);
			token = strtok(NULL, " ");
			if (token)
				v.y = atof(token);
			token = strtok(NULL, " ");
			if (token)
				v.z = atof(token);
			array_push_back(vertices, &v);
		} else if (strcmp(token, "vt") == 0) {
			struct vec2 vt;
			token = strtok(NULL, " ");
			if (token)
				vt.x = atof(token);
			token = strtok(NULL, " ");
			if (token)
				vt.y = atof(token);
			array_push_back(texcoords, &vt);
		} else if (strcmp(token, "vn") == 0) {
			struct vec3 vn;
			token = strtok(NULL, " ");
			if (token)
				vn.x = atof(token);
			token = strtok(NULL, " ");
			if (token)
				vn.y = atof(token);
			token = strtok(NULL, " ");
			if (token)
				vn.z = atof(token);
			array_push_back(normals, &vn);
		} else if (strcmp(token, "f") == 0) {
			struct vec3i f1, f2, f3;
			token = strtok(NULL, " ");
			if (token)
				parse_face_token(token, &f1);
			token = strtok(NULL, " ");
			if (token)
				parse_face_token(token, &f2);
			token = strtok(NULL, " ");
			if (token)
				parse_face_token(token, &f3);
			array_push_back(faces, &f1);
			array_push_back(faces, &f2);
			array_push_back(faces, &f3);
		}
	}
}

static void calculate_center(struct mesh *mesh, struct array *vertices)
{
	mesh->center = (struct vec3) { 0, 0, 0 };
	struct vec3 *v;

	size_t i;
	for (i = 0; i < vertices->len; i++) {
		v = (struct vec3*) array_get_ptr(vertices, i);
		mesh->center.x += v->x;
		mesh->center.y += v->y;
		mesh->center.z += v->z;
	}

	mesh->center.x /= vertices->len;
	mesh->center.y /= vertices->len;
	mesh->center.z /= vertices->len;
}

static void create_indexed_data(struct mesh *mesh, struct array *vertices,
	struct array *texcoords, struct array *normals, struct array *faces)
{
	size_t i;
	for (i = 0; i < faces->len; i++) {
		if (vertices->len) {
			if (mesh->indexed_vertices == NULL)
				mesh->indexed_vertices = array_new(struct vec3);
			int idx = array_get(faces, i, struct vec3i).x - 1;
			struct vec3 v = array_get(vertices, idx, struct vec3);
			array_push_back(mesh->indexed_vertices, &v);
		}
		if (texcoords->len) {
			if (mesh->indexed_texcoords == NULL)
				mesh->indexed_texcoords =
					array_new(struct vec2);
			int idx = array_get(faces, i, struct vec3i).y - 1;
			struct vec2 vt = array_get(texcoords, idx, struct vec2);
			array_push_back(mesh->indexed_texcoords, &vt);
		}
		if (normals->len) {
			if (mesh->indexed_normals == NULL)
				mesh->indexed_normals = array_new(struct vec3);
			int idx = array_get(faces, i, struct vec3i).z - 1;
			struct vec3 vn = array_get(normals, idx, struct vec3);
			array_push_back(mesh->indexed_normals, &vn);
		}
	}

	array_delete(&vertices);
	array_delete(&texcoords);
	array_delete(&normals);
	array_delete(&faces);
}

struct mesh * mesh_new(const char *file_name)
{
	FILE *file = fopen(file_name, "r");
	if (file == NULL) {
		fprintf(stderr, "%s doesn't exist\n", file_name);
		return NULL;
	}
	printf("Loading %s...\n", file_name);

	struct mesh *mesh = NEW(struct mesh);
	mesh->indexed_vertices = NULL;
	mesh->indexed_texcoords = NULL;
	mesh->indexed_normals = NULL;

	struct array *vertices = array_new(struct vec3);
	struct array *texcoords = array_new(struct vec2);
	struct array *normals = array_new(struct vec3);
	struct array *faces = array_new(struct vec3i);
	
	parse_file(file, mesh, vertices, texcoords, normals, faces);
	fclose(file);

	calculate_center(mesh, vertices);
	
	create_indexed_data(mesh, vertices, texcoords, normals, faces);

	return mesh;
}

void mesh_delete(struct mesh **mesh)
{
	if ((*mesh)->indexed_vertices);
		array_delete(&(*mesh)->indexed_vertices);
	if ((*mesh)->indexed_texcoords);
		array_delete(&(*mesh)->indexed_texcoords);
	if ((*mesh)->indexed_normals);
		array_delete(&(*mesh)->indexed_normals);

	FREE(*mesh);
}

unsigned int mesh_get_vertex_count(struct mesh *mesh)
{
	return mesh->indexed_vertices->len;
}
