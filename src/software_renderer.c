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
 * draw_line2 - Bresenham's line drawing function (one octane only)
 * @surface:	target surface to draw onto
 * @v:		positions of the line vertices
 * @color:	color of the line
 */
void draw_line2(SDL_Surface *surface, struct vec2 v[2], unsigned int color)
{
	int dx = v[1].x - v[0].x;
	int dy = v[1].y - v[0].y;
	float error = 0, derror = fabs((float) dy / dx);
	
	int x, y = v[0].y;
	for (x = v[0].x; x < v[1].x; x++) {
		draw_pixel(surface, x, y, color);
		error += derror;
		if (error >= 0.5) {
			y++;
			error -= 1.0;
		}
	}
}

/**
 * draw_line3 - Bresenham's line drawing function (all octanes)
 * @surface:	target surface to draw onto
 * @v:		positions of the line vertices
 * @color:	color of the line
 */

void draw_line3(SDL_Surface *surface, struct vec2i v[2], unsigned int color)
{
	int dx = abs(v[0].x - v[1].x);
	int dy = abs(v[0].y - v[1].y);
	int sx = (v[0].x < v[1].x) ? 1 : -1;
	int sy = (v[0].y < v[1].y) ? 1 : -1;
	int err = dx - dy;
	int e2;

	for (;;) {
		draw_pixel(surface, v[0].x, v[0].y, color);
		if (v[0].x == v[1].x && v[0].y == v[1].y)
			break;
		e2 = err * 2;
		if (e2 > -dy) {
			err -= dy;
			v[0].x += sx;
		}
		if (e2 < dx) {
			err += dx;
			v[0].y += sy;
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
	float tdx, tdy;
	float sdx, sdy;
	float tslope, sslope;

	if (tall->v[0].y > tall->v[1].y)
		swap(&tall->v[0], &tall->v[1], sizeof(struct vec2i));
	if (shrt->v[0].y > shrt->v[1].y)
		swap(&shrt->v[0], &shrt->v[1], sizeof(struct vec2i));
	tdx = tall->v[1].x - tall->v[0].x;
	tdy = tall->v[1].y - tall->v[0].y;
	sdx = shrt->v[1].x - shrt->v[0].x;
	sdy = shrt->v[1].y - shrt->v[0].y;
	tslope =  tdx / tdy;
	sslope =  sdx / sdy;

	if (tall->v[0].x == shrt->v[0].x &&
	    tall->v[0].y == shrt->v[0].y) {
		for (float y = shrt->v[0].y; y <= shrt->v[1].y; y++) {
			float sx = shrt->v[0].x + sslope * (y - shrt->v[0].y);
			float tx = tall->v[0].x + tslope * (y - tall->v[0].y);
			float min, max;
			if (sx < tx) {
				min = sx;
				max = tx;
			} else {
				min = tx;
				max = sx;
			}
			for (float x = min; x < max; x++) {
				draw_pixel(surface, x, y, color);
			}
		}
	} else if (tall->v[1].x == shrt->v[1].x &&
		   tall->v[1].y == shrt->v[1].y) {
		for (float y = shrt->v[1].y; y >= shrt->v[0].y; y--) {
			float sx = shrt->v[0].x + sslope * (y - shrt->v[0].y);
			float tx = tall->v[0].x + tslope * (y - tall->v[0].y);
			float min, max;
			if (sx < tx) {
				min = sx;
				max = tx;
			} else {
				min = tx;
				max = sx;
			}
			for (float x = min; x < max; x++) {
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

void draw_circle(SDL_Surface *surface, struct vec2 v[2], float radius,
		 unsigned int color)
{
	
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
 * renderer_clear - clear the screen
 * @rndr:	the renderer
 */
void renderer_clear(struct renderer *rndr)
{
	int len = rndr->screen->w * rndr->screen->h * sizeof(unsigned int);
	memset(rndr->screen->pixels, 0, len);
}

/**
 * renderer_display - update the screen with the latest framebuffer
 * @rndr:	the renderer
 */
void renderer_display(struct renderer *rndr)
{
	SDL_Flip(rndr->screen);
}

static void map_line_to_viewport(struct renderer *rndr, struct vec2 v[2])
{
	v[0].x = mapf(v[0].x, rndr->viewport.left, rndr->viewport.right,
		      0, rndr->screen->w);
	v[0].y = mapf(v[0].y, rndr->viewport.top, rndr->viewport.bottom,
		      0, rndr->screen->h);
	v[1].x = mapf(v[1].x, rndr->viewport.left, rndr->viewport.right,
		      0, rndr->screen->w);
	v[1].y = mapf(v[1].y, rndr->viewport.top, rndr->viewport.bottom,
		      0, rndr->screen->h);
}

static void map_triangle_to_viewport(struct renderer *rndr, struct vec2 v[3])
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
}

static void map_rect_to_viewport(struct renderer *rndr, struct vec2 v[4])
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
	map_line_to_viewport(rndr, v);
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
	map_triangle_to_viewport(rndr, v);
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
	map_rect_to_viewport(rndr, v);
	draw_rect(rndr->screen, v, color);
}

void renderer_draw_line3d(struct renderer *rndr, struct vec3 v[2],
			  unsigned int color)
{
	float zscale[2] = {-1 / v[0].z, -1 / v[1].z};

	v[0].x *= zscale[0];
	v[0].y *= zscale[0];
	v[1].x *= zscale[1];
	v[1].y *= zscale[1];

	struct vec2 tv[2] = { {v[0].x, v[0].y}, {v[1].x, v[1].y} };
	map_line_to_viewport(rndr, tv);
	draw_line(rndr->screen, tv, color);
}

void renderer_draw_triangle3d(struct renderer *rndr, struct vec3 v[3],
			  unsigned int color)
{
	float zscale[3] = {-1 / v[0].z, -1 / v[1].z, -1 / v[2].z};

	v[0].x *= zscale[0];
	v[0].y *= zscale[0];
	v[1].x *= zscale[1];
	v[1].y *= zscale[1];
	v[2].x *= zscale[2];
	v[2].y *= zscale[2];

	struct vec2 tv[3] = { {v[0].x, v[0].y},
			      {v[1].x, v[1].y},
			      {v[2].x, v[2].y} };
	map_triangle_to_viewport(rndr, tv);
	draw_triangle(rndr->screen, tv, color);
}

void renderer_draw_rect3d(struct renderer *rndr, struct vec3 v[4],
			  unsigned int color)
{
	float zscale[4] = {-1 / v[0].z, -1 / v[1].z, -1 / v[2].z, -1 / v[3].z};

	v[0].x *= zscale[0];
	v[0].y *= zscale[0];
	v[1].x *= zscale[1];
	v[1].y *= zscale[1];
	v[2].x *= zscale[2];
	v[2].y *= zscale[2];
	v[3].x *= zscale[3];
	v[3].y *= zscale[3];

	struct vec2 tv[4] = { {v[0].x, v[0].y},
			      {v[1].x, v[1].y},
			      {v[2].x, v[2].y},
			      {v[3].x, v[3].y} };
	map_rect_to_viewport(rndr, tv);
	draw_rect(rndr->screen, tv, color);
}

void renderer_draw_cube(struct renderer *rndr, struct vec3 v, float w, float h,
			float d)
{
	struct vec3 front[4]	= {{v.x,	v.y,		v.z},
				   {v.x + w,	v.y,		v.z},
				   {v.x + w,	v.y + h,	v.z},
				   {v.x,	v.y + h,	v.z} };
	struct vec3 back[4]	= {{v.x,	v.y,		v.z - d},
				   {v.x + w,	v.y,		v.z - d},
				   {v.x + w,	v.y + h,	v.z - d},
				   {v.x,	v.y + h,	v.z - d}};
	struct vec3 left[4]	= {{v.x,	v.y,		v.z - d},
				   {v.x,	v.y,		v.z},
				   {v.x,	v.y + h,	v.z},
				   {v.x,	v.y + h,	v.z - d}};
	struct vec3 right[4]	= {{v.x + w,	v.y,		v.z},
				   {v.x + w,	v.y,		v.z - d},
				   {v.x + w, 	v.y + h,	v.z - d},
				   {v.x + w, 	v.y + h,	v.z}};
	struct vec3 top[4]	= {{v.x,	v.y,		v.z},
				   {v.x + w,	v.y,		v.z},
				   {v.x + w, 	v.y,		v.z - d},
				   {v.x,	v.y,		v.z - d}};
	struct vec3 bottom[4]	= {{v.x,	v.y + h,	v.z},
				   {v.x + w,	v.y + h,	v.z},
				   {v.x + w, 	v.y + h,	v.z - d},
				   {v.x,	v.y + h,	v.z - d}};
	renderer_draw_rect3d(rndr, back, 0xff00ff00);
	renderer_draw_rect3d(rndr, top, 0xffff00ff);
	renderer_draw_rect3d(rndr, left, 0xff0000ff);
	renderer_draw_rect3d(rndr, right, 0xffffff00);
	renderer_draw_rect3d(rndr, bottom, 0xffff00ff);
	renderer_draw_rect3d(rndr, front, 0xffff0000);
}
