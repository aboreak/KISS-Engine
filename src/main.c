#include <stdbool.h>
#include <stdio.h>
#include <SDL/SDL.h>

#include "software_renderer.h"
#include "algorithm.h"

int main(int argc, char *argv[])
{
	struct renderer *renderer;
	bool running;

	SDL_Init(SDL_INIT_EVERYTHING);
	renderer = renderer_new(640, 480);
	running = true;

	while (running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				running = false;
			renderer_clear(renderer);
			float x = event.motion.x;
			float y = event.motion.y;
			float mx = mapf(x, 0, 640, -1, 1);
			float my = mapf(y, 0, 480, -1, 1);
			struct vec3 v = { mx, my, -1 };
			renderer_draw_cube(renderer, v, 0.5, 0.5, 0.5);
		}
		renderer_display(renderer);
	}

	SDL_Quit();

	return 0;
}

/*
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "model.h"
#include "program.h"
#include "camera.h"
#include "light.h"
#include "array.h"
#include "map.h"
#include "scene_manager.h"

int main(int argc, char *argv[])
{
	const SDL_VideoInfo *info = NULL;
	int width = 0;
	int height = 0;
	int bpp = 0;
	int flags = 0;
	
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Video initialization failed: %s\n",
			SDL_GetError());
		SDL_Quit();
		exit(1);
	}

	info = SDL_GetVideoInfo();
	if (!info) {
		fprintf(stderr, "Video query failed: %s\n", SDL_GetError());
		SDL_Quit();
		exit(1);
	}

	width = 640;
	height = 480;
	bpp = info->vfmt->BitsPerPixel;
	flags = SDL_OPENGL;
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	if (SDL_SetVideoMode(width, height, bpp, flags) == 0) {
		fprintf(stderr, "Video mode set failed: %s\n", SDL_GetError());
		SDL_Quit();
		exit(1);
	}

	glewInit();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_TEXTURE_2D);

	struct model *key   = model_new("res/Key_B_02.obj");
	struct model *floor = model_new("res/test.obj");
	program_startup("src/vertex.shader2", "src/fragment.shader2");

	struct mat4 projection = mat4_identity();
	mat4_perspective(&projection, 90.0f, (1.0f * width) / height, 1.0f,
		1000.0f);
	GLint projection_uniform = glGetUniformLocation(g_program,
		"projection");
	
	struct scene_manager *scene   = scene_manager_new();
	struct scene_node *key_node   = scene_node_new();
	struct scene_node *floor_node = scene_node_new();
	scene_node_set_model(key_node, key);
	scene_node_set_model(floor_node, floor);
	scene_manager_add_node(scene, key_node);
	scene_node_translate(key_node, 0, 0, -5);
	scene_node_add_child(key_node, floor_node);
	scene_node_translate(floor_node, 0, -1, 0);
	scene_manager_add_light(scene, POINT);
		
	struct vec3 target = key_node->center;
	scene_manager_set_camera_target(scene, target.x, target.y, target.z);

	float angle = 0.0f;
	int running = 1;
	while (running) {
		SDL_Event event;
		while (SDL_PollEvent(&event))
			if (event.type == SDL_QUIT)
				running = 0;
		angle += 0.01f;

		glUseProgram(g_program);
		glUniformMatrix4fv(projection_uniform, 1, GL_FALSE, 
			(GLfloat *) &projection);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		scene_node_rotate(key_node, 0, 0.01, 0);
		scene_node_rotate(floor_node, 0, 0, 0.01);
		scene_manager_draw(scene);

		SDL_GL_SwapBuffers();
		SDL_Delay(8);
	}

	SDL_Quit();

	return 0;
}
*/
