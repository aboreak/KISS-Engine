#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include "matrix.h"

struct array;
struct stack;
struct model;
struct scene_node;

struct scene_node {
	struct vec3		translation;
	struct vec3		rotation;
	struct vec3		center;
	struct mat4		to_world_matrix;
	struct array *		children;
	struct scene_node *	parent;
	struct model *		model;
};

struct scene_manager {
	struct scene_node *	root;
	struct stack *		matrix_stack;
	struct light_manager *	light_mgr;
	struct camera *		camera;
};

struct scene_node * scene_node_new();

void scene_node_delete(struct scene_node **node);

void scene_node_translate(struct scene_node *node, float x, float y, float z);

void scene_node_rotate(struct scene_node *node, float pitch, float yaw,
	float roll);

void scene_node_add_child(struct scene_node *node, struct scene_node *child);

void scene_node_set_parent(struct scene_node *node, struct scene_node *parent);

void scene_node_set_model(struct scene_node *node, struct model *model);

struct mat4 scene_node_matrix(struct scene_node *node);

void scene_node_remove_child(struct scene_node *node, struct scene_node *child);

struct scene_manager * scene_manager_new();

void scene_manager_delete(struct scene_manager **scene);

void scene_manager_draw(struct scene_manager *scene);

inline void scene_manager_add_node(struct scene_manager *scene,
	struct scene_node *node);

inline struct light_manager * scene_manager_get_light_manager(
	struct scene_manager *scene);

inline void scene_manager_add_light(struct scene_manager *scene, unsigned type);

inline void scene_manager_set_camera_position(struct scene_manager *scene,
	float x, float y, float z);

inline void scene_manager_set_camera_target(struct scene_manager *scene,
	float x, float y, float z);

inline void scene_manager_set_camera_up(struct scene_manager *scene,float x,
	float y, float z);

#endif
