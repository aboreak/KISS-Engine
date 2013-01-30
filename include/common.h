#ifndef __COMMON_H__
#define __COMMON_H__

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

#define NEW(x) ((x*)malloc(sizeof(x)))

#define NEW2(x) x = malloc(sizeof(*(x)));

#define NEW0(x) x = calloc(1, sizeof(*(x)));

#define NNEW(x,y) ((x*) malloc(sizeof(x) * y))

#define FREE(x) if (x != NULL) free(x);\
		x = NULL;

#define MIN(x,y) ((x) < (y) ? (x) : (y))
#define MAX(x,y) ((x) > (y) ? (x) : (y))
#define ABS(x) ((x > 0) ? x : -x)

#define LEN(x) (sizeof(x)/sizeof(x[0]))

#define KB (1024)
#define MB (1048576)
#define GB (1073741824)

#ifndef TRUE
#define TRUE (1)
#endif

#ifndef FALSE
#define FALSE (!TRUE)
#endif

extern const size_t POINTER_SIZE;
extern const size_t UNSIGNED_SIZE;

static inline void memshift(void *to, void *from, unsigned esize)
{
	void *p;

	esize = (to > from) ? -esize : esize;

	for (p = to; p != from; p += esize)
		memcpy(p, p + esize, esize);
}

#endif
