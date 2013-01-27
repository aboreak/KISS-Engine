#ifndef __SOFTWARE_RENDERER_H__
#define __SOFTWARE_RENDERER_H__

#include "vector.h"

typedef struct SDL_Surface SDL_Surface;

void draw_pixel(SDL_Surface *surface, int x, int y, unsigned int color);

void draw_line(SDL_Surface *surface, struct vec2i *v1, struct vec2i *v2,
	       unsigned int color);

#endif
