#ifndef __SOFTWARE_RENDERER_H__
#define __SOFTWARE_RENDERER_H__

#include "vector.h"

typedef struct SDL_Surface SDL_Surface;

struct line {
	struct vec2 v[2];
};

void draw_pixel(SDL_Surface *surface, int x, int y, unsigned int color);

void draw_pixels(SDL_Surface *surface, int x, int y, unsigned int color, int n);

void draw_line(SDL_Surface *surface, struct vec2 v[2], unsigned int color);

void draw_triangle(SDL_Surface *surface, struct vec2 v[3], unsigned int color);

void draw_rect(SDL_Surface *surface, struct vec2 v[4], unsigned int color);

struct rect {
	float left;
	float top;
	float right;
	float bottom;
};

struct renderer {
	SDL_Surface *screen;
	struct rect viewport;
};

struct renderer * renderer_new(int width, int height);

void renderer_set_viewport(struct renderer *rndr, struct rect *viewport);

void renderer_display();

void renderer_draw_line(struct renderer *rndr, struct vec2 v[2],
			unsigned int color);

void renderer_draw_triangle(struct renderer *rndr, struct vec2 v[3],
			    unsigned int color);
void renderer_draw_rect(struct renderer *rndr, struct vec2 v[4],
			    unsigned int color);

#endif
