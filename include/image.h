#ifndef __IMAGE_H__
#define __IMAGE_H__

enum {
	BGRA_32,
	BGR_24,
	BGRA_16,
	BGR_15,
	BW_8,
	BW_4,
	BW_1
};

struct image {
	unsigned char *data;
	unsigned int width;
	unsigned int height;
	unsigned int format;
};

struct image *image_new(const char *filename);

void image_delete(struct image **img);

#endif
