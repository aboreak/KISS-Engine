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
void draw_line(SDL_Surface *surface, struct vec2 v[2], unsigned int color)
{
	struct vec2 *min, *max;
	float slope;
	float dx = v[1].x - v[0].x;
	float dy = v[1].y - v[0].y;

	if (fabs(dx) > fabs(dy)) {
		if (v[0].x > v[1].x) {
			min = &v[1];
			max = &v[0];
		} else {
			min = &v[0];
			max = &v[1];
		}
		slope = dy / dx;
		for (float x = min->x; x < max->x; x++) {
			float y = min->y + slope * (x - min->x);
			draw_pixel(surface, (int) x, (int) y, color);
		}
	} else {
		if (v[0].y > v[1].y) {
			min = &v[1];
			max = &v[0];
		} else {
			min = &v[0];
			max = &v[1];
		}
		slope = dx / dy;
		for (float y = min->y; y <= max->y; y++) {
			float x = min->x + slope * (y - min->y);
			draw_pixel(surface, (int) x, (int) y, color);
		}

	}
}

/**
 * draw_triangle_span - draw the horizontal lines of the triangle
 * @surface:	target surface to draw onto
 * @tall:	the tall line in the triangle
 * @shrt:	one of the two shrt lines of the triangle
 * @color:	color of the triangle
 */
static void draw_triangle_span(SDL_Surface *surface, struct line *tall,
			       struct line *shrt, unsigned int color)
{
	int tdx, tdy;
	int sdx, sdy;
	float tslope, sslope;

	if (tall->v[0].y > tall->v[1].y)
		swap(&tall->v[0], &tall->v[1], sizeof(struct vec2i));
	if (shrt->v[0].y > shrt->v[1].y)
		swap(&shrt->v[0], &shrt->v[1], sizeof(struct vec2i));
	tdx = tall->v[1].x - tall->v[0].x;
	tdy = tall->v[1].y - tall->v[0].y;
	sdx = shrt->v[1].x - shrt->v[0].x;
	sdy = shrt->v[1].y - shrt->v[0].y;
	tslope = (float) tdx / tdy;
	sslope = (float) sdx / sdy;

	if (tall->v[0].x == shrt->v[0].x &&
	    tall->v[0].y == shrt->v[0].y) {
		for (int y = shrt->v[0].y; y < shrt->v[1].y; y++) {
			int sx = shrt->v[0].x + sslope * (y - shrt->v[0].y);
			int tx = tall->v[0].x + tslope * (y - tall->v[0].y);
			int min, max;
			if (sx < tx) {
				min = sx;
				max = tx;
			} else {
				min = tx;
				max = sx;
			}
			for (int x = min; x < max; x++) {
				draw_pixel(surface, x, y, color);
			}
		}
	} else if (tall->v[1].x == shrt->v[1].x &&
		   tall->v[1].y == shrt->v[1].y) {
		for (int y = shrt->v[1].y; y > shrt->v[0].y; y--) {
			int sx = shrt->v[0].x + sslope * (y - shrt->v[0].y);
			int tx = tall->v[0].x + tslope * (y - tall->v[0].y);
			int min, max;
			if (sx < tx) {
				min = sx;
				max = tx;
			} else {
				min = tx;
				max = sx;
			}
			for (int x = min; x < max; x++) {
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
void draw_triangle(SDL_Surface *surface, struct vec2 v[3], unsigned int color)
{
	struct line lines[3];
	int tall = 0;
	int tall_height = 0;

	for (int i = 0; i < 3; i++) {
		lines[i] = (struct line) {{v[i], v[(i+1) % 3]}};
		int height = abs(lines[i].v[0].y - lines[i].v[1].y);
		if (tall_height < height) {
			tall_height = height;
			tall = i;
		}
	}

	draw_triangle_span(surface, &lines[tall], &lines[(tall + 1) % 3],
			   color);
	draw_triangle_span(surface, &lines[tall], &lines[(tall + 2) % 3],
			   color);
}

/**
 * draw_rect - simple rectangle drawing function
 * @surface:	target surface to draw onto
 * @v:		three vertices of the triangle
 * @color:	color of the triangle
 */
void draw_rect(SDL_Surface *surface, struct vec2 v[4], unsigned int color)
{
	struct vec2 v2[3] = {v[0], v[1], v[2]};
	draw_triangle(surface, v2, color);

	v2[1] = v[2];
	v2[2] = v[3];
	draw_triangle(surface, v2, color);
}

/**
 * renderer_new - creates an initialized struct renderer
 * @width:	the width of the screen
 * @height:	the height of the screen
 */
struct renderer * renderer_new(int width, int height)
{
	struct renderer *rndr = NEW(struct renderer);
	if (rndr == NULL) {
		fprintf(stderr, "malloc() on struct renderer failed");
		return NULL;
	}
	rndr->screen = SDL_SetVideoMode(width, height, 32, SDL_SWSURFACE);
	if (rndr->screen == NULL) {
		fprintf(stderr, "SDL_SetVideoMode was unsuccessful: %s\n",
			SDL_GetError());
		return NULL;
	}
	rndr->viewport.left = -1;
	rndr->viewport.right = 1;
	rndr->viewport.top = -1;
	rndr->viewport.bottom = 1;

	return rndr;
}

/**
 * renderer_set_viewport - set the dimension of the viewport similar to OpenGL
 * @rndr:	the renderer
 * @viewport:	the desired dimension the viewport
 */
void renderer_set_viewport(struct renderer *rndr, struct rect *viewport)
{
	rndr->viewport = *viewport;
}

/**
 * renderer_display - update the screen with the latest framebuffer
 * @rndr:	the renderer
 */
void renderer_display(struct renderer *rndr)
{
	SDL_Flip(rndr->screen);
}

/**
 * renderer_draw_line - the standard line drawing function
 * @rndr:	the renderer
 * @v:		vertices of the line
 * @color:	color of the line
 */
void renderer_draw_line(struct renderer *rndr, struct vec2 v[2],
			unsigned int color)
{
	v[0].x = mapf(v[0].x, rndr->viewport.left, rndr->viewport.right,
		      0, rndr->screen->w);
	v[0].y = mapf(v[0].y, rndr->viewport.top, rndr->viewport.bottom,
		      0, rndr->screen->h);
	v[1].x = mapf(v[1].x, rndr->viewport.left, rndr->viewport.right,
		      0, rndr->screen->w);
	v[1].y = mapf(v[1].y, rndr->viewport.top, rndr->viewport.bottom,
		      0, rndr->screen->h);

	draw_line(rndr->screen, v, color);
}

/**
 * renderer_draw_triangle - standard triangle drawing function
 * @rndr:	the renderer
 * @v:		vertices of the triangle
 * @color:	color of the triangle
 */
void renderer_draw_triangle(struct renderer *rndr, struct vec2 v[3],
			    unsigned int color)
{
	v[0].x = mapf(v[0].x, rndr->viewport.left, rndr->viewport.right,
		      0, rndr->screen->w);
	v[0].y = mapf(v[0].y, rndr->viewport.top, rndr->viewport.bottom,
		      0, rndr->screen->h);
	v[1].x = mapf(v[1].x, rndr->viewport.left, rndr->viewport.right,
		      0, rndr->screen->w);
	v[1].y = mapf(v[1].y, rndr->viewport.top, rndr->viewport.bottom,
		      0, rndr->screen->h);
	v[2].x = mapf(v[2].x, rndr->viewport.left, rndr->viewport.right,
		      0, rndr->screen->w);
	v[2].y = mapf(v[2].y, rndr->viewport.top, rndr->viewport.bottom,
		      0, rndr->screen->h);
	
	draw_triangle(rndr->screen, v, color);
}

/**
 * renderer_draw_rect - standard rectangle drawing function
 * @rndr:	the renderer
 * @v:		vertices of the rectangle
 * @color:	color of the rectangle
 */
void renderer_draw_rect(struct renderer *rndr, struct vec2 v[4],
			    unsigned int color)
{
	v[0].x = mapf(v[0].x, rndr->viewport.left, rndr->viewport.right,
		      0, rndr->screen->w);
	v[0].y = mapf(v[0].y, rndr->viewport.top, rndr->viewport.bottom,
		      0, rndr->screen->h);
	v[1].x = mapf(v[1].x, rndr->viewport.left, rndr->viewport.right,
		      0, rndr->screen->w);
	v[1].y = mapf(v[1].y, rndr->viewport.top, rndr->viewport.bottom,
		      0, rndr->screen->h);
	v[2].x = mapf(v[2].x, rndr->viewport.left, rndr->viewport.right,
		      0, rndr->screen->w);
	v[2].y = mapf(v[2].y, rndr->viewport.top, rndr->viewport.bottom,
		      0, rndr->screen->h);
	v[3].x = mapf(v[3].x, rndr->viewport.left, rndr->viewport.right,
		      0, rndr->screen->w);
	v[3].y = mapf(v[3].y, rndr->viewport.top, rndr->viewport.bottom,
		      0, rndr->screen->h);
	
	draw_rect(rndr->screen, v, color);
}
