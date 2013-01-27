#include <string.h>
#include <math.h>
#include <stdio.h>
#include <SDL/SDL.h>

#include "software_renderer.h"

void draw_pixel(SDL_Surface *surface, int x, int y, unsigned int color)
{
	memcpy(surface->pixels + (x + surface->w * y) * sizeof(color), &color,
	       sizeof(color));
}

void draw_line(SDL_Surface *surface, struct vec2i *v1, struct vec2i *v2,
	       unsigned int color)
{
	struct vec2i *min, *max;
	float slope;
	int dx;
	int dy;

	dx = v2->x - v1->x;
	dy = v2->y - v1->y;

	if (abs(dx) > abs(dy)) {
		if (v1->x > v2->x) {
			min = v2;
			max = v1;
		} else {
			min = v1;
			max = v2;
		}
		slope = (float) dy / dx;
		for (int x = min->x; x < max->x; x++) {
			int y = min->y + ((int) (slope * (x - min->x)));
			draw_pixel(surface, x, y, color);
		}
	} else {
		if (v1->y > v2->y) {
			min = v2;
			max = v1;
		} else {
			min = v1;
			max = v2;
		}
		slope = (float) dx / dy;
		for (int y = min->y; y < max->y; y++) {
			int x = v1->x + ((int) (slope * (y - min->y)));
			draw_pixel(surface, x, y, color);
		}

	}
}
