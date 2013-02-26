#ifndef __LIGHT_MANAGER_H__
#define __LIGHT_MANAGER_H__

#define MAX_LIGHTS 8

struct array;

struct light_manager {
	struct array *lights;
};

struct light_manager *light_manager_new();

void light_manager_delete(struct light_manager **light_mgr);

unsigned light_manager_add_light(struct light_manager *light_mgr,
				 unsigned type);

void light_manager_draw(struct light_manager *light_mgr);

struct light *light_manager_get_light(struct light_manager *light_mgr,
				      unsigned i);

#endif
