#include <GL/glew.h>
#include <GL/gl.h>

#include "camera.h"
#include "matrix.h"
#include "program.h"
#include "common.h"

struct camera * camera_new()
{
	struct camera *cam = NEW(struct camera);
	cam->position	   = (struct vec3) { 0, 0, 0 };
	cam->target	   = (struct vec3) { 0, 0, 0 };
	cam->up		   = (struct vec3) { 0, 1, 0 };

	return cam;
}

void camera_use(struct camera *cam)
{
	struct mat4 mat = mat4_look_at(&cam->position, &cam->target,
		&cam->up);
	GLint uniform = glGetUniformLocation(g_program, "camera");
	glUniformMatrix4fv(uniform, 1, GL_FALSE, (GLfloat *) &mat);
}

void camera_delete(struct camera **cam)
{
	free(*cam);
	*cam = NULL;
}

inline void camera_set_position(struct camera *cam, float x, float y, float z)
{
	cam->position.x = -x;
	cam->position.y = -y;
	cam->position.z = -z;
}

inline void camera_set_target(struct camera *cam, float x, float y, float z)
{
	cam->target.x = -x;
	cam->target.y = -y;
	cam->target.z = -z;
}

inline void camera_set_up(struct camera *cam, float x, float y, float z)
{
	cam->up.x = x;
	cam->up.y = y;
	cam->up.z = z;
}
