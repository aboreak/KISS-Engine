#ifndef __POOL_ALLOCATOR_H__
#define __POOL_ALLOCATOR_H__

struct pool_allocator;

extern struct pool_allocator *g_pool_allocator;

struct pool_allocator {
	unsigned char **data;
	unsigned char *front;
	unsigned esize;
	unsigned ecount;
};

#define pool_allocator_new(x,y) (pool_allocator_new_real(sizeof(x), y))

struct pool_allocator *pool_allocator_new_real(unsigned esize, unsigned ecount);

void pool_allocator_delete();

void pool_allocator_bind(struct pool_allocator *pool);

void *pool_allocator_malloc();

void pool_allocator_free(void *data);

#endif
