#ifndef __TEXTURE_H__
#define __TEXTURE_H__

struct texture {
	unsigned id;
};

struct texture * texture_new(const char *filename);

void texture_delete(struct texture **texture);

void texture_bind(struct texture *texture);

#endif
