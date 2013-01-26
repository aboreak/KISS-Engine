#include <GL/glew.h>
#include <GL/gl.h>

#include "model.h"
#include "mesh.h"
#include "material.h"
#include "texture.h"
#include "image.h"
#include "array.h"
#include "program.h"
#include "common.h"

static void create_buffer_object(struct model *model)
{
	glGenBuffers(3, model->id);

	glBindBuffer(GL_ARRAY_BUFFER, model->id[0]);
	struct array *vertices = model->mesh->indexed_vertices;
	if (vertices)
		glBufferData(GL_ARRAY_BUFFER, array_size(vertices),
			vertices->data, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, model->id[1]);
	struct array *normals = model->mesh->indexed_normals;
	if (normals)
		glBufferData(GL_ARRAY_BUFFER, array_size(normals),
			normals->data, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, model->id[2]);
	struct array *texcoords = model->mesh->indexed_texcoords;
	if (texcoords)
		glBufferData(GL_ARRAY_BUFFER, array_size(texcoords),
			texcoords->data, GL_STATIC_DRAW);
}

static void send_material_info(struct model *model)
{
	GLint ka_uniform = glGetUniformLocation(g_program, "material.ka");
	GLint kd_uniform = glGetUniformLocation(g_program, "material.kd");
	GLint ks_uniform = glGetUniformLocation(g_program, "material.ks");
	GLint shininess_uniform = glGetUniformLocation(g_program,
		"material.shininess");
	
	struct material *material = model->material;
	glUniform3fv(ka_uniform, 1, (GLfloat *) &material->ambient);
	glUniform3fv(kd_uniform, 1, (GLfloat *) &material->diffuse);
	glUniform3fv(ks_uniform, 1, (GLfloat *) &material->specular);
	glUniform1f(shininess_uniform, material->shininess);
}
static void send_mesh_info(struct model *model)
{
	GLint model_uniform = glGetUniformLocation(g_program, "model");
	struct mat4 mat = model_matrix(model);
	glUniformMatrix4fv(model_uniform, 1, GL_FALSE, (float*) &mat);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	if (model->id[0]) {
		glBindBuffer(GL_ARRAY_BUFFER, model->id[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}
	if (model->id[1]) {
		glBindBuffer(GL_ARRAY_BUFFER, model->id[1]);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}
	if (model->id[2]) {
		glBindBuffer(GL_ARRAY_BUFFER, model->id[2]);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}
	struct mesh *mesh = model->mesh;
	glDrawArrays(GL_TRIANGLES, 0, mesh->indexed_vertices->len);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

static void send_mesh_info_with_matrix(struct model *model, struct mat4 *mat)
{
	GLint model_uniform = glGetUniformLocation(g_program, "model");
	glUniformMatrix4fv(model_uniform, 1, GL_FALSE, (float*) mat);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	if (model->id[0]) {
		glBindBuffer(GL_ARRAY_BUFFER, model->id[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}
	if (model->id[1]) {
		glBindBuffer(GL_ARRAY_BUFFER, model->id[1]);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}
	if (model->id[2]) {
		glBindBuffer(GL_ARRAY_BUFFER, model->id[2]);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}
	struct mesh *mesh = model->mesh;
	glDrawArrays(GL_TRIANGLES, 0, mesh->indexed_vertices->len);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

static void calculate_transformed_center(struct model *model)
{
	struct mat4 mat = model_matrix(model);
	struct vec4 center = {  model->mesh->center.x,
				model->mesh->center.y,
				model->mesh->center.z,
				1 };
	
	struct vec4 tcenter = vec4_mult_mat4(&center, &mat);
	model->center =	(struct vec3) { tcenter.x, tcenter.y, tcenter.z };
}

struct model * model_new(const char *file_name)
{
	struct model *model = NEW(struct model);

	memset(model->id, 0, sizeof(unsigned) * 3);
	model->mesh = mesh_new(file_name);
	model->material = material_new(model->mesh->material_file_name);
	model->texture = texture_new(model->material->diffuse_map_file_name);
	model->rotation = (struct vec3) { 0.0f, 0.0f, 0.0f };
	model->translation = (struct vec3) { 0.0f, 0.0f, 0.0f };

	calculate_transformed_center(model);
	create_buffer_object(model);

	return model;
}

void model_delete(struct model **model)
{
	mesh_delete(&(*model)->mesh);
	material_delete(&(*model)->material);
	texture_delete(&(*model)->texture);
	free(*model);
	*model = NULL;
}

void model_draw(struct model *model)
{
	texture_bind(model->texture);
	send_material_info(model);
	send_mesh_info(model);
}

void model_draw_with_matrix(struct model *model, struct mat4 *mat)
{
	texture_bind(model->texture);
	send_material_info(model);
	send_mesh_info_with_matrix(model, mat);
}

void model_translate(struct model *model, float x, float y, float z)
{
	model->translation.x += x;
	model->translation.y += y;
	model->translation.z += z;
	model->center.x += x;
	model->center.y += y;
	model->center.z += z;
}

void model_rotate(struct model *model, float x, float y, float z)
{
	model->rotation.x += x;
	model->rotation.y += y;
	model->rotation.z += z;
}

struct mat4 model_matrix(struct model *model)
{
	struct mat4 mat;
	struct mat4 rotation_matrix;

	mat = mat4_translate_vec3(&model->translation);
	rotation_matrix = mat4_rotate_vec3(&model->rotation);
	mat = mat4_mult(&mat, &rotation_matrix);

	return mat;
}
