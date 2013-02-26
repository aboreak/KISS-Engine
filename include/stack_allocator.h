#ifndef __STACK_ALLOCATOR_H__
#define __STACK_ALLOCATOR_H__

struct stack_allocator;

extern struct stack_allocator *g_stack_allocator;

struct stack_allocator {
	unsigned char *data;
	unsigned char *top;
	unsigned cap;
	unsigned num_chunks;
	unsigned free_size;
	unsigned top_size;
};

#define stack_allocator_new_n(x,y) (stack_allocator_new_n_real(sizeof(x), y))
#define stack_allocator_malloc(y) (stack_allocator_malloc_real(sizeof(y)))

struct stack_allocator *stack_allocator_new(unsigned cap);

struct stack_allocator *stack_allocator_new_n_real(unsigned esize,
						   unsigned ecount);

void stack_allocator_delete();

void stack_allocator_bind(struct stack_allocator *stack);

unsigned char *stack_allocator_malloc_real(unsigned esize);

unsigned char *stack_allocator_free();

inline void stack_allocator_clear();

#endif
