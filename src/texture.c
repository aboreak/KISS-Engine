#include <GL/glew.h>
#include <GL/gl.h>

#include "texture.h"
#include "program.h"
#include "image.h"
#include "common.h"

struct texture * texture_new(const char *file_name)
{
	struct image *image;
	if (!(image = image_new(file_name)))
		if (!(image = image_new("res/no_texture.bmp")))
			return NULL;

	struct texture *texture = NEW(struct texture);
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &texture->id);
	glBindTexture(GL_TEXTURE_2D, texture->id);
	if (image->format == BGR_24)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width,
			image->height, 0, GL_BGR, GL_UNSIGNED_BYTE,
			image->data);
	else if (image->format == BGRA_32)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width,
			image->height, 0, GL_BGRA, GL_UNSIGNED_BYTE,
			image->data);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	image_delete(&image);
	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}

void texture_delete(struct texture **texture)
{
	glDeleteTextures(1, &(*texture)->id);
	*texture = NULL;
}

void texture_bind(struct texture *texture)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->id);
	GLint texture_uniform = glGetUniformLocation(g_program, "tex");
	glUniform1i(texture_uniform, 0);
}
