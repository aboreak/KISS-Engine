#include <string.h>
#include <math.h>
#include <stdio.h>
#include <SDL/SDL.h>

#include "software_renderer.h"
#include "algorithm.h"

/**
 * draw_pixel - put a pixel onto an SDL_Surface
 * @surface:	the target surface to draw onto
 * @x:		horizontal position of the pixel
 * @y:		vertical position of the pixel
 * @color:	color of the pixel
 */
void draw_pixel(SDL_Surface *surface, int x, int y, unsigned int color)
{
	void *ptr = surface->pixels + (x + surface->w * y) * sizeof(color);

	memcpy(ptr, &color, sizeof(color));
}

/**
 * draw_line - a very simple line drawing function
 * @surface:	target surface to draw onto
 * @v:		positions of the line vertices
 * @color:	color of the line
 */
void draw_line(SDL_Surface *surface, struct vec2i v[2], unsigned int color)
{
	struct vec2i *min, *max;
	float slope;
	int dx = v[1].x - v[0].x;
	int dy = v[1].y - v[0].y;

	if (abs(dx) > abs(dy)) {
		if (v[0].x > v[1].x) {
			min = &v[1];
			max = &v[0];
		} else {
			min = &v[0];
			max = &v[1];
		}
		slope = (float) dy / dx;
		for (int x = min->x; x <= max->x; x++) {
			int y = min->y + ((int) (slope * (x - min->x)));
			draw_pixel(surface, x, y, color);
		}
	} else {
		if (v[0].y > v[1].y) {
			min = &v[1];
			max = &v[0];
		} else {
			min = &v[0];
			max = &v[1];
		}
		slope = (float) dx / dy;
		for (int y = min->y; y <= max->y; y++) {
			int x = min->x + ((int) (slope * (y - min->y)));
			draw_pixel(surface, x, y, color);
		}

	}
}

/**
 * draw_triangle_span - draw the horizontal lines of the triangle
 * @surface:	target surface to draw onto
 * @tallest:	the tallest line in the triangle
 * @shorter:	one of the two shorter lines of the triangle
 * @color:	color of the triangle
 */
static void draw_triangle_span(SDL_Surface *surface, struct line *tallest,
			       struct line *shorter, unsigned int color)
{
	int tdx, tdy;
	int sdx, sdy;
	float tslope, sslope;

	if (tallest->v1.y > tallest->v2.y)
		swap(&tallest->v1, &tallest->v2, sizeof(struct vec2i));
	if (shorter->v1.y > shorter->v2.y)
		swap(&shorter->v1, &shorter->v2, sizeof(struct vec2i));
	tdx = tallest->v2.x - tallest->v1.x;
	tdy = tallest->v2.y - tallest->v1.y;
	sdx = shorter->v2.x - shorter->v1.x;
	sdy = shorter->v2.y - shorter->v1.y;
	tslope = (float) tdx / tdy;
	sslope = (float) sdx / sdy;

	if (tallest->v1.x == shorter->v1.x &&
	    tallest->v1.y == shorter->v1.y) {
		for (int y = shorter->v1.y; y <= shorter->v2.y; y++) {
			int sx = shorter->v1.x + sslope * (y - shorter->v1.y);
			int tx = tallest->v1.x + tslope * (y - tallest->v1.y);
			int min, max;
			if (sx < tx) {
				min = sx;
				max = tx;
			} else {
				min = tx;
				max = sx;
			}
			for (int x = min; x <= max; x++) {
				draw_pixel(surface, x, y, color);
			}
		}
	} else if (tallest->v2.x == shorter->v2.x &&
		   tallest->v2.y == shorter->v2.y) {
		for (int y = shorter->v2.y; y >= shorter->v1.y; y--) {
			int sx = shorter->v1.x + sslope * (y - shorter->v1.y);
			int tx = tallest->v1.x + tslope * (y - tallest->v1.y);
			int min, max;
			if (sx < tx) {
				min = sx;
				max = tx;
			} else {
				min = tx;
				max = sx;
			}
			for (int x = min; x <= max; x++) {
				draw_pixel(surface, x, y, color);
			}
		}
	}
}

/**
 * draw_triangle - simple triangle drawing function
 * @surface:	target surface to draw onto
 * @v:		three vertices of the triangle
 * @color:	color of the triangle
 *
 * This function works by finding the middle vertex of the triangle and split
 * the triangle into half: top and bottom. It then draws each part in separate
 * draw_triangle_span() function
 */
void draw_triangle(SDL_Surface *surface, struct vec2i v[3], unsigned int color)
{
	struct line lines[3];
	int tallest = 0;
	int tallest_height = 0;

	for (int i = 0; i < 3; i++) {
		lines[i] = (struct line) {v[i], v[(i+1) % 3]};
		int height = abs(lines[i].v1.y - lines[i].v2.y);
		if (tallest_height < height) {
			tallest_height = height;
			tallest = i;
		}
	}

	draw_triangle_span(surface, &lines[tallest], &lines[(tallest + 1) % 3],
			   color);
	draw_triangle_span(surface, &lines[tallest], &lines[(tallest + 2) % 3],
			   color);
}
