#ifndef __ARRAY_H__
#define __ARRAY_H__

#include <stdlib.h>

struct array {
	void *data;
	size_t esize;
	size_t len;
	size_t cap;
};

#define array_new(x) (array_new_real(sizeof(x)))

#define array_get(x,y,z) (*((z*)(array_get_ptr(x, y))))

#define array_len(x) ((x)->len)

struct array *array_new_real(size_t esize);

void array_delete(struct array **ary);

inline void array_put(struct array *ary, size_t i, void *data);

inline void *array_get_ptr(struct array *ary, size_t i);

void array_push_back(struct array *ary, void *data);

void array_pop(struct array *ary, size_t i);

inline void array_pop_front(struct array *ary);

inline void array_pop_back(struct array *ary);

inline size_t array_size(struct array *ary);

void array_print(struct array *ary);

#endif
