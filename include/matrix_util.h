#ifndef __MATRIX_UTIL_H__
#define __MATRIX_UTIL_H__

#include "common.h"

struct mat3;
struct mat4;

static void print_mat3(struct mat3 *mat)
{
	float *arr;

	arr = (float *) mat;

	unsigned i;
	for (i = 0; i < 3; i++)
		printf("%f %f %f\n", arr[0+i*3], arr[1+i*3], arr[2+i*3]);
	printf("\n");
}

static void print_mat4(struct mat4 *mat)
{
	float *arr;

	arr = (float *) mat;

	unsigned i;
	for (i = 0; i < 4; i++)
		printf("%f %f %f %f\n", arr[0+i*4], arr[1+i*4], arr[2+i*4], 
			arr[3+i*4]);
	printf("\n");
}

#endif
