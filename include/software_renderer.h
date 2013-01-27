#ifndef __SOFTWARE_RENDERER_H__
#define __SOFTWARE_RENDERER_H__

#include "vector.h"

typedef struct SDL_Surface SDL_Surface;

struct line {
	struct vec2i v1;
	struct vec2i v2;
};

void draw_pixel(SDL_Surface *surface, int x, int y, unsigned int color);

void draw_pixels(SDL_Surface *surface, int x, int y, unsigned int color, int n);

void draw_line(SDL_Surface *surface, struct vec2i v[2], unsigned int color);

void draw_triangle(SDL_Surface *surface, struct vec2i v[3], unsigned int color);

#endif
