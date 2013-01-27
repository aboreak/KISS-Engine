#ifndef __ALGORITHM_H__
#define __ALGORITHM_H__

#include "common.h"

static inline void swap(void *a, void *b, size_t sz)
{
	void *tmp = malloc(sz);
	memcpy(tmp, a, sz);
	memcpy(a, b, sz);
	memcpy(b, tmp, sz);
}

static inline void swapf(float *a, float *b)
{
	float tmp = *a;
	*a = *b;
	*b = tmp;
}

static inline void swapi(int *a, int *b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

#endif
