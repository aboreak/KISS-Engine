#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "vector.h"

#define MAX_LIGHTS 8

struct array;

extern struct array *g_lights;

enum LightType {
	POINT = 0,
	DIRECTIONAL,
	SPOT,
	AMBIENT
};

struct light {
	struct vec3 translation;
	struct vec3 rotation;
	struct vec3 intensity;
	float spot_exponent;
	float spot_cutoff;
	float constant_attenuation;
	float linear_attenuation;
	float quadratic_attenuation;
	unsigned type;
};

struct light *light_new(unsigned type);

void light_delete(struct light **light);

inline void light_set_translation(struct light *light, float x, float y,
				  float z);

inline void light_set_rotation(struct light *light, float pitch, float yaw,
			       float roll);

inline void light_set_intensity(struct light *light, float r, float g, float b);

inline void light_set_spot_exponent(struct light *light, float spot_exponent);

inline void light_set_spot_cutoff(struct light *light, float spot_cutoff);

inline void light_translate(struct light *light, float x, float y, float z);

inline void light_rotate(struct light *light, float pitch, float yaw,
			 float roll);

inline void light_set_type(struct light *light, unsigned int type);

inline struct vec3 light_get_direction(struct light *light);

#endif
