#include "pool_allocator.h"
#include "common.h"

const size_t POINTER_SIZE = sizeof(unsigned char*);

struct pool_allocator *g_pool_allocator = NULL;

struct pool_allocator * pool_allocator_new_real(unsigned esize, unsigned ecount)
{
	unsigned char *addr = NULL;
	size_t chunk_size;

	struct pool_allocator *pool = NEW(struct pool_allocator);
	pool->esize	= esize;
	pool->ecount	= ecount;
	pool->data	= NNEW(unsigned char*, ecount);
	pool->front	= NULL;

	chunk_size = esize + POINTER_SIZE;
	
	unsigned i = ecount;
	while (i--) {
		pool->data[i] = malloc(chunk_size);
		memcpy(pool->data[i], &addr, POINTER_SIZE);
		addr = pool->data[i];
		if (i == 0)
			break;
	}
	pool->front = pool->data[0];

	return pool;
}

void pool_allocator_delete()
{
	if (!g_pool_allocator)
		return;

	int i;
	for (i = 0; i < g_pool_allocator->ecount; i++)
		free(g_pool_allocator->data[i]);
	free(g_pool_allocator->data);
	free(g_pool_allocator);
	g_pool_allocator = NULL;
}

void pool_allocator_bind(struct pool_allocator *pool)
{
	if (pool)
		g_pool_allocator = pool;
}

void * pool_allocator_malloc()
{
	unsigned char *	addr;
	void *		ret;

	if (!g_pool_allocator)
		return NULL;
	
	ret = g_pool_allocator->front;
	
	addr = 0;
	memcpy(&addr, g_pool_allocator->front, POINTER_SIZE);
	if (!addr)
		return NULL;
	g_pool_allocator->front = (unsigned char*) addr;

	return ret + POINTER_SIZE;
}

void pool_allocator_free(void *data)
{
	unsigned char *	next;

	if (!g_pool_allocator)
		return;

	if (data) {
		memset(data, 0, g_pool_allocator->esize);
		next = g_pool_allocator->front;
		g_pool_allocator->front = data - POINTER_SIZE;
		memcpy(g_pool_allocator->front, next, POINTER_SIZE);
	}
}
