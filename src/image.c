#include "image.h"
#include "common.h"
#include "algorithm.h"
#include "array.h"

enum {
	IMAGE_UNKNOWN = 0,
	IMAGE_TGA,
	IMAGE_BMP
};

static unsigned char get_image_type(FILE * file)
{
	if (fseek(file, -18, SEEK_END) == -1)
		return 0;
	size_t start = ftell(file);
	if (fseek(file, 0, SEEK_END) == -1)
		return 0;
	size_t end = ftell(file);
	size_t sz = end - start;

	if (fseek(file, -18, SEEK_END) == -1)
		return 0;
	char buf[sz];
	if (fread(buf, 1, sz, file) < sz)
		return 0;
	if (strcmp(buf, "TRUEVISION-XFILE.") == 0) {
		printf("It's a TGA file!\n");
		return IMAGE_TGA;
	}

	if (fseek(file, 0, SEEK_SET) == -1)
		return 0;
	if (fread(buf, 1, 2, file) < 2)
		return 0;
	if (memcmp(buf, "BM", 2) == 0) {
		printf("It's a BMP file!\n");
		return IMAGE_BMP;
	}

	fprintf(stderr, "The image file type is not supported\n");

	return 0;
}

static void load_tga(struct image *img, FILE * file)
{
	img->width = 0;
	img->height = 0;

	if (fseek(file, 0xC, SEEK_SET) == -1)
		return;
	if (fread(&img->width, 2, 1, file) < 1)
		return;
	if (fseek(file, 0xE, SEEK_SET) == -1)
		return;
	if (fread(&img->height, 2, 1, file) < 1)
		return;

	printf("image dimension: %ux%u pixels\n", img->width, img->height);

	if (fseek(file, 0x10, SEEK_SET) == -1)
		return;
	unsigned char num_bits = fgetc(file);
	if (num_bits == EOF)
		return;
	else if (num_bits == 32) {
		img->format = BGRA_32;
		printf("Color depth: %hu-bit\n", num_bits);
	} else if (num_bits == 24)
		img->format = BGR_24;
	else if (num_bits == 16)
		img->format = BGRA_16;
	else if (num_bits == 15)
		img->format = BGR_15;
	else if (num_bits == 8)
		img->format = BW_8;

	if (fseek(file, 0x12, SEEK_SET) == -1)
		return;
	size_t data_size = 0;
	unsigned int image_length = img->width * img->height;
	if (img->format == BGRA_32) {
		data_size = image_length * 4;
		img->data = (unsigned char *)malloc(data_size);
		if (fread(img->data, 1, data_size, file) < data_size)
			return;
	} else if (img->format == BGR_24) {
		data_size = image_length * 3;
		img->data = (unsigned char *)malloc(data_size);
		if (fread(img->data, 1, data_size, file) < data_size)
			return;
	} else if (img->format == BW_8) {
		data_size = image_length;
		img->data = (unsigned char *)malloc(data_size);
		if (fread(img->data, 1, data_size, file) < data_size)
			return;
	}
	printf("Pixel data size: %.2f KB\n", data_size / 1024.0f);

	fclose(file);
}

static void load_bmp(struct image *img, FILE * file)
{
	unsigned int data_pos = 0;
	if (fseek(file, 0xA, SEEK_SET) == -1)
		return;
	if (fread(&data_pos, 4, 1, file) < 1)
		return;

	img->width = 0;
	img->height = 0;
	if (fseek(file, 0x12, SEEK_SET) == -1)
		return;
	if (fread(&img->width, 4, 1, file) < 1)
		return;
	if (fseek(file, 0x16, SEEK_SET) == -1)
		return;
	if (fread(&img->height, 4, 1, file) < 1)
		return;
	printf("image dimension: %ux%u pixels\n", img->width, img->height);

	unsigned short num_bits = 0;
	if (fseek(file, 0x1C, SEEK_SET) == -1)
		return;
	if (fread(&num_bits, 2, 1, file) < 1)
		return;
	if (num_bits == 32)
		img->format = BGRA_32;
	else if (num_bits == 24)
		img->format = BGR_24;
	else if (num_bits == 16)
		img->format = BGRA_16;
	else if (num_bits == 8)
		img->format = BW_8;
	else if (num_bits == 4)
		img->format = BW_4;
	else if (num_bits == 1)
		img->format = BW_1;
	printf("Color depth: %hu-bit\n", num_bits);

	unsigned int compression_method = -1;
	if (fseek(file, 0x1E, SEEK_SET) == -1)
		return;
	if (fread(&compression_method, 4, 1, file) < 1)
		return;

	size_t data_size = 0;
	if (fseek(file, 0x22, SEEK_SET) == -1)
		return;
	if (fread(&data_size, 4, 1, file) < 1)
		return;
	img->data = malloc(data_size);
	printf("Pixel data size: %.2f KB\n", data_size / 1024.0f);

	if (compression_method == 0) {
		if (fseek(file, data_pos, SEEK_SET) == -1)
			return;
		if (fread(img->data, data_size, 1, file) < 1)
			return;
	} else {
		free(img->data);
		return;
	}

	fclose(file);
}

struct image *image_new(const char *filename)
{
	FILE *file = fopen(filename, "r");
	if (!file) {
		fprintf(stderr, "%s doesn't exist\n", filename);
		return NULL;
	}

	printf("Loading %s...\n", filename);

	struct image *img = NEW(struct image);
	char image_type = get_image_type(file);
	if (image_type == IMAGE_TGA)
		load_tga(img, file);
	else if (image_type == IMAGE_BMP)
		load_bmp(img, file);

	return img;
}

void image_delete(struct image **img)
{
	free((*img)->data);
	free(*img);
	*img = NULL;
}
