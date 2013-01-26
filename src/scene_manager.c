#include "scene_manager.h"
#include "array.h"
#include "stack.h"
#include "model.h"
#include "mesh.h"
#include "light_manager.h"
#include "camera.h"
#include "common.h"

static struct vec3 calculate_transformed_center(struct model *model)
{
	struct mat4 mat = model_matrix(model);
	struct vec4 center = {  model->mesh->center.x,
				model->mesh->center.y,
				model->mesh->center.z,
				1 };
	struct vec4 tcenter = vec4_mult_mat4(&center, &mat);

	return (struct vec3) { tcenter.x, tcenter.y, tcenter.z };
}

static void scene_node_pre_draw_children(struct array *children,
	struct scene_manager *scene);

static void scene_node_post_draw_children(struct array *children,
	struct scene_manager *scene);

static void scene_node_pre_draw(struct scene_node *node,
	struct scene_manager *scene)
{
	struct mat4 * top_matrix;
	struct mat4   local_matrix;

	if (node == scene->root) {
		stack_push(scene->matrix_stack, &node->to_world_matrix);
		scene_node_pre_draw_children(node->children, scene);
		return;
	}

	top_matrix 	      = (struct mat4 *) stack_top(scene->matrix_stack);
	local_matrix	      = scene_node_matrix(node);
	node->to_world_matrix = mat4_mult(&local_matrix, top_matrix);
	stack_push(scene->matrix_stack, &node->to_world_matrix);

	scene_node_pre_draw_children(node->children, scene);
}

static void scene_node_post_draw(struct scene_node *node,
	struct scene_manager *scene)
{
	scene_node_post_draw_children(node->children, scene);
	stack_pop(scene->matrix_stack);
}

static void scene_node_pre_draw_children(struct array *children,
	struct scene_manager *scene)
{
	struct scene_node *it;

	unsigned i;
	for (i = 0; i < children->len; i++) {
		it = array_get(children, i, struct scene_node*);
		if (it)
			scene_node_pre_draw(it, scene);
	}
}

static void scene_node_post_draw_children(struct array *children,
	struct scene_manager *scene)
{
	struct scene_node *it;

	unsigned i;
	for (i = 0; i < children->len; i++) {
		it = array_get(children, i, struct scene_node*);
		if (it)
			scene_node_post_draw(it, scene);
	}
}

static void scene_node_draw_children(struct array *children,
	struct scene_manager *scene);

static void scene_node_draw(struct scene_node *node,
	struct scene_manager *scene)
{
	if (node->model)
		model_draw_with_matrix(node->model, &node->to_world_matrix);

	scene_node_draw_children(node->children, scene);
}

static void scene_node_draw_children(struct array *children,
	struct scene_manager *scene)
{
	struct scene_node *it;

	unsigned i;
	for (i = 0; i < children->len; i++) {
		it = array_get(children, i, struct scene_node*);
		if (it)
			scene_node_draw(it, scene);
	}
}

struct scene_node * scene_node_new()
{
	struct scene_node * node = NEW(struct scene_node);

	node->children	      = array_new(struct scene_node*);
	node->parent	      = NULL;
	node->model	      = NULL;
	node->rotation 	      = (struct vec3) { 0, 0, 0 };
	node->translation     = (struct vec3) { 0, 0, 0 };
	node->center	      = (struct vec3) { 0, 0, 0 };
	node->to_world_matrix = mat4_identity();

	return node;
}

void scene_node_delete(struct scene_node **node)
{
	struct array *	    children;
	struct scene_node * child;

	children = (*node)->children;

	unsigned i;
	for (i = 0; i < children->len; i++) {
		child = array_get(children, i, struct scene_node*);
		scene_node_delete(&child);
	}
	array_delete(&(*node)->children);

	free(*node);
	*node = NULL;
}

void scene_node_add_child(struct scene_node *node, struct scene_node *child)
{
	array_push_back(node->children, &child);
}

void scene_node_set_parent(struct scene_node *node, struct scene_node *parent)
{
	if (node->parent)
		scene_node_remove_child(node->parent, node);

	node->parent = parent;
}

void scene_node_set_model(struct scene_node *node, struct model *model)
{
	node->model = model;
	node->center = calculate_transformed_center(model);
	node->center = vec3_add(&node->center, &node->translation);
}

void scene_node_rotate(struct scene_node *node, float pitch, float yaw,
	float roll)
{
	node->rotation.x += pitch;
	node->rotation.y += yaw;
	node->rotation.z += roll;
}

void scene_node_translate(struct scene_node *node, float x, float y, float z)
{
	node->translation.x += x;
	node->translation.y += y;
	node->translation.z += z;
	node->center.x += x;
	node->center.y += y;
	node->center.z += z;
}

struct mat4 scene_node_matrix(struct scene_node *node)
{
	struct mat4 translation_matrix;
	struct mat4 rotation_matrix;

	translation_matrix = mat4_translate_vec3(&node->translation);
	rotation_matrix	   = mat4_rotate_vec3(&node->rotation);

	return mat4_mult(&rotation_matrix, &translation_matrix);
}

void scene_node_remove_child(struct scene_node *node, struct scene_node *child)
{
	struct scene_node *it;

	unsigned i;
	for (i = 0; i < node->children->len; i++) {
		it = array_get(node->children, i, struct scene_node*);
		if (it == child)
			array_pop(node->children, i);
	}
}

struct scene_manager * scene_manager_new()
{
	struct scene_manager * scene = NEW(struct scene_manager);

	scene->root  	    = scene_node_new();
	scene->matrix_stack = stack_new();
	scene->light_mgr    = light_manager_new();
	scene->camera	    = camera_new();

	return scene;
}

void scene_manager_delete(struct scene_manager **scene)
{
	scene_node_delete(&(*scene)->root);
	stack_delete(&(*scene)->matrix_stack);
	light_manager_delete(&(*scene)->light_mgr);
	camera_delete(&(*scene)->camera);

	free(*scene);
	*scene = NULL;
}

void scene_manager_draw(struct scene_manager *scene)
{
	scene_node_pre_draw(scene->root, scene);
	scene_node_draw(scene->root, scene);
	scene_node_post_draw(scene->root, scene);
	light_manager_draw(scene->light_mgr);
	camera_use(scene->camera);
}

inline void scene_manager_add_node(struct scene_manager *scene, 
	struct scene_node *node)
{
	scene_node_add_child(scene->root, node);
}

inline struct light_manager * scene_manager_get_light_manager(
	struct scene_manager *scene)
{
	return scene->light_mgr;
}

inline void scene_manager_add_light(struct scene_manager *scene, unsigned type)
{
	light_manager_add_light(scene->light_mgr, type);
}

inline void scene_manager_set_camera_position(struct scene_manager *scene,
	float x, float y, float z)
{
	camera_set_position(scene->camera, x, y, z);
}

inline void scene_manager_set_camera_target(struct scene_manager *scene,
	float x, float y, float z)
{
	camera_set_target(scene->camera, x, y, z);
}

inline void scene_manager_set_camera_up(struct scene_manager *scene,float x,
	float y, float z)
{
	camera_set_up(scene->camera, x, y, z);
}
