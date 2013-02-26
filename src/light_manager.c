#include <GL/glew.h>
#include <GL/gl.h>

#include "light_manager.h"
#include "light.h"
#include "array.h"
#include "program.h"
#include "common.h"

struct light_manager *light_manager_new()
{
	struct light_manager *light_mgr = NEW(struct light_manager);

	light_mgr->lights = array_new(struct light *);

	return light_mgr;
}

void light_manager_delete(struct light_manager **light_mgr)
{
	array_delete(&(*light_mgr)->lights);
	free(*light_mgr);
	*light_mgr = NULL;
}

unsigned light_manager_add_light(struct light_manager *light_mgr, unsigned type)
{
	struct array *lights;
	struct light *light;

	lights = light_mgr->lights;
	light = light_new(type);

	if (lights->len < MAX_LIGHTS)
		array_push_back(lights, &light);

	return lights->len - 1;
}

struct light *light_manager_get_light(struct light_manager *light_mgr,
				      unsigned i)
{
	if (light_mgr->lights->len <= i)
		return NULL;

	return array_get(light_mgr->lights, i, struct light *);
}

void light_manager_draw(struct light_manager *light_mgr)
{
	struct array *lights;
	struct light *light;
	char str[32];
	char id_str[4];
	unsigned len;
	GLint translation_uniform;
	GLint direction_uniform;
	GLint intensity_uniform;
	GLint spot_exponent_uniform;
	GLint spot_cutoff_uniform;
	GLint type_uniform;

	lights = light_mgr->lights;

	unsigned i;
	for (i = 0; i < lights->len; i++) {
		light = array_get(lights, i, struct light *);

		strcpy(str, "light[");
		sprintf(id_str, "%d", (int)i);
		strcat(str, id_str);
		len = strlen(str);
		strcat(str, "].translation");
		translation_uniform = glGetUniformLocation(g_program, str);
		str[len] = '\0';
		strcat(str, "].direction");
		direction_uniform = glGetUniformLocation(g_program, str);
		str[len] = '\0';
		strcat(str, "].intensity");
		intensity_uniform = glGetUniformLocation(g_program, str);
		str[len] = '\0';
		strcat(str, "].spot_exponent");
		spot_exponent_uniform = glGetUniformLocation(g_program, str);
		str[len] = '\0';
		strcat(str, "].spot_cutoff");
		spot_cutoff_uniform = glGetUniformLocation(g_program, str);
		str[len] = '\0';
		strcat(str, "].type");
		type_uniform = glGetUniformLocation(g_program, str);
		str[len] = '\0';

		struct vec3 direction = light_get_direction(light);

		glUniform3fv(translation_uniform, 1,
			     (GLfloat *) & light->translation);
		glUniform3fv(direction_uniform, 1, (GLfloat *) & direction);
		glUniform3fv(intensity_uniform, 1,
			     (GLfloat *) & light->intensity);
		glUniform1f(spot_exponent_uniform, light->spot_exponent);
		glUniform1f(spot_cutoff_uniform, light->spot_cutoff);
		glUniform1i(type_uniform, light->type);

		GLint light_num_uniform = glGetUniformLocation(g_program,
							       "light_num");
		glUniform1i(light_num_uniform, lights->len);
	}
}
