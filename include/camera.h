#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "vector.h"

struct camera {
	struct vec3 position;
	struct vec3 target;
	struct vec3 up;
};

struct camera *camera_new();

void camera_delete(struct camera **cam);

void camera_use(struct camera *cam);

inline void camera_set_position(struct camera *cam, float x, float y, float z);

inline void camera_set_target(struct camera *cam, float x, float y, float z);

inline void camera_set_up(struct camera *cam, float x, float y, float z);

#endif
