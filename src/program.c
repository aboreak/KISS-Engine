#include <GL/glew.h>
#include <GL/gl.h>

#include "program.h"
#include "array.h"
#include "common.h"

GLuint g_program = 0;

static GLuint create_program(struct array *shaders)
{
	GLuint tmp = glCreateProgram();

	unsigned i;
	for (i = 0; i < shaders->len; i++)
		glAttachShader(tmp, array_get(shaders, i, GLuint));

	glLinkProgram(tmp);

	GLint status;
	glGetProgramiv(tmp, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		GLint info_log_length;
		glGetProgramiv(tmp, GL_INFO_LOG_LENGTH, &info_log_length);

		GLchar *info_log_str = (GLchar *) malloc(info_log_length + 1);
		glGetProgramInfoLog(tmp, info_log_length, NULL, info_log_str);
		fprintf(stderr, "Linker failure: %s\n", info_log_str);
		free(info_log_str);
	}

	for (i = 0; i < shaders->len; i++)
		glDetachShader(tmp, array_get(shaders, i, GLuint));

	return tmp;
}

static GLuint create_shader(GLenum type, FILE * file)
{
	fseek(file, 0, SEEK_END);
	size_t fsize = ftell(file);
	fseek(file, 0, SEEK_SET);
	GLchar *str = (GLchar *) malloc(sizeof(GLchar) * fsize);
	size_t nread = fread(str, 1, fsize, file);
	if (nread != fsize) {
		perror("Error occured while reading bytes from file to string");
		return 0;
	}
	str[nread] = '\0';

	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, (const GLchar **)&str, NULL);
	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		GLint info_log_length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);

		GLchar *info_log_str = (GLchar *) malloc(info_log_length + 1);
		glGetShaderInfoLog(shader, info_log_length, NULL, info_log_str);

		const char *type_str = NULL;
		switch (type) {
		case GL_VERTEX_SHADER:
			type_str = "vertex";
			break;
		case GL_FRAGMENT_SHADER:
			type_str = "fragment";
			break;
		}

		fprintf(stderr, "Compile failure in %s shader:\n%s\n", type_str,
			info_log_str);
		free(info_log_str);
		return 0;
	}

	return shader;
}

void program_startup(const char *vshader_file_name,
		     const char *fshader_file_name)
{
	FILE *vshader_file = fopen(vshader_file_name, "r");
	if (!vshader_file) {
		fprintf(stderr, "%s doesn't exist\n", vshader_file_name);
		return;
	}

	FILE *fshader_file = fopen(fshader_file_name, "r");
	if (!fshader_file) {
		fprintf(stderr, "%s doesn't exist\n", fshader_file_name);
		return;
	}

	struct array *shaders = array_new(GLuint);
	if (!shaders) {
		perror("Out of memory: Could not allocate space for struct"
		       "array");
		return;
	}

	GLuint vshader = create_shader(GL_VERTEX_SHADER, vshader_file);
	GLuint fshader = create_shader(GL_FRAGMENT_SHADER, fshader_file);
	array_push_back(shaders, &vshader);
	array_push_back(shaders, &fshader);

	g_program = create_program(shaders);

	unsigned i;
	for (i = 0; i < shaders->len; i++)
		glDeleteShader(array_get(shaders, i, GLuint));
	array_delete(&shaders);
	fclose(vshader_file);
	fclose(fshader_file);
}

void program_shutdown()
{
	glDeleteProgram(g_program);
	g_program = 0;
}
