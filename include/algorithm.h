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

static inline void swapd(double *a, double *b)
{
	double tmp = *a;
	*a = *b;
	*b = tmp;
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

static inline double mapd(double val, double min1, double max1, double min2,
			  double max2)
{
	return (val - min1) / (max1 - min1) * (max2 - min2) + min2;
}

static inline float mapf(float val, float min1, float max1, float min2,
			 float max2)
{
	return (val - min1) / (max1 - min1) * (max2 - min2) + min2;
}

static inline int mapi(int val, int min1, int max1, int min2, int max2)
{
	return (val - min1) / (max1 - min1) * (max2 - min2) + min2;
}

static inline double clampd(double val, double min, double max)
{
	return (val < min) ? min :
			    (val > max) ? max : val;
}

static inline float clampf(float val, float min, float max)
{
	return (val < min) ? min :
			     (val > max) ? max : val;
}

static inline int clampi(int val, int min, int max)
{
	return (val < min) ? min :
			     (val > max) ? max : val;
}

#endif
