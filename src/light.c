#include <GL/glew.h>
#include <GL/gl.h>

#include "light.h"
#include "array.h"
#include "program.h"
#include "matrix.h"
#include "common.h"

struct light *light_new(enum LightType type)
{
	struct light *light = NEW(struct light);

	light_set_translation(light, 0, 0, 0);
	light_set_rotation(light, 0, 0, 0);
	light_set_intensity(light, 1, 1, 1);
	light_set_spot_exponent(light, 32);
	light_set_spot_cutoff(light, PI / 8);
	light_set_type(light, type);

	return light;
}

inline void light_set_translation(struct light *light, float x, float y,
				  float z)
{
	light->translation.x = x;
	light->translation.y = y;
	light->translation.z = z;
}

inline void light_set_rotation(struct light *light, float pitch, float yaw,
			       float roll)
{
	light->rotation.x = pitch;
	light->rotation.y = yaw;
	light->rotation.z = roll;
}

inline void light_set_intensity(struct light *light, float r, float g, float b)
{
	light->intensity.x = r;
	light->intensity.y = g;
	light->intensity.z = b;
}

inline void light_set_spot_exponent(struct light *light, float spot_exponent)
{
	light->spot_exponent = spot_exponent;
}

inline void light_set_spot_cutoff(struct light *light, float spot_cutoff)
{
	light->spot_cutoff = spot_cutoff;
}

inline void light_set_type(struct light *light, unsigned int type)
{
	light->type = type;
}

inline void light_translate(struct light *light, float x, float y, float z)
{
	light->translation.x += x;
	light->translation.y += y;
	light->translation.z += z;
}

inline void light_rotate(struct light *light, float pitch, float yaw,
			 float roll)
{
	light->rotation.x += pitch;
	light->rotation.y += yaw;
	light->rotation.z += roll;
}

inline struct vec3 light_get_direction(struct light *light)
{
	struct vec3 default_direction = { 0, 0, -1 };
	struct mat3 rotation_matrix = mat3_rotate_vec3(&light->rotation);

	return vec3_mult_mat3(&default_direction, &rotation_matrix);
}
