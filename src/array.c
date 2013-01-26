#include "array.h"
#include "common.h"

static void array_extend(struct array *ary);

struct array * array_new_real(size_t esize)
{
	struct array *ary;

	NEW2(ary);
	ary->data  = NULL;
	ary->esize = esize;
	ary->len   = 0;
	ary->cap   = 0;

	return ary;
}

void array_delete(struct array **ary)
{
	FREE((*ary)->data);
	FREE(*ary);
}

inline void array_put(struct array *ary, size_t i, void *data)
{
	memcpy(ary->data + ary->esize * i, data, ary->esize);
}

inline void * array_get_ptr(struct array *ary, size_t i)
{
	return ary->data + i * ary->esize;
}

void array_push_back(struct array *ary, void *data)
{
	size_t esize = ary->esize;
	size_t len = ary->len;
	void *dst;

	if (ary->data == NULL) {
		ary->cap = 1;
		ary->data = malloc(esize);
	}

	if (len == ary->cap)
		array_extend(ary);

	dst = ary->data + len * esize;
	memcpy(dst, data, esize);
	ary->len++;
}

void array_pop(struct array *ary, size_t i)
{
	size_t esize = ary->esize;
	void *dst = ary->data + i * esize;
	void *last = ary->data + ary->len * esize;

	memcpy(dst, dst + esize, last-dst-esize);
	ary->len--;
}

inline void array_pop_front(struct array *ary)
{
	size_t esize = ary->esize;

	memcpy(ary->data, ary->data + esize, esize);
	ary->len--;
}

inline void array_pop_back(struct array *ary)
{
	ary->len--;
}

inline size_t array_size(struct array *ary)
{
	return ary->len * ary->esize;
}

void array_print(struct array *ary)
{
	printf("data: %p\nesize: %zu\nlen: %zu\ncap: %zu\n", ary->data,
		ary->esize, ary->len, ary->cap);
}

static void array_extend(struct array *ary)
{
	void *new_data;
	size_t new_cap;

	new_cap = ary->cap * 2;
	new_data = malloc(new_cap * ary->esize);
	memcpy(new_data, ary->data, ary->cap * ary->esize);
	free(ary->data);
	ary->data = new_data;
	ary->cap = new_cap;
}
