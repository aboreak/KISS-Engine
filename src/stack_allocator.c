#include "stack_allocator.h"
#include "common.h"

const size_t UNSIGNED_SIZE = sizeof(unsigned);

struct stack_allocator *g_stack_allocator = NULL;

struct stack_allocator * stack_allocator_new(unsigned cap)
{
	struct stack_allocator *stack = NEW(struct stack_allocator);
	stack->cap = cap;
	stack->num_chunks = 0;
	stack->free_size = cap;
	stack->data = malloc(stack->cap);
	stack->top = NULL;

	return stack;
}

struct stack_allocator * stack_allocator_new_n_real(unsigned esize,
	unsigned ecount)
{
	struct stack_allocator *stack = NEW(struct stack_allocator);
	stack->cap = (esize + UNSIGNED_SIZE) * ecount;
	stack->num_chunks = 0;
	stack->free_size = stack->cap;
	stack->data = malloc(stack->cap);
	stack->top = NULL;

	return stack;
}

void stack_allocator_delete()
{
	if (!g_stack_allocator)
		return;
	
	free(g_stack_allocator->data);
	free(g_stack_allocator);
	g_stack_allocator = NULL;
}

void stack_allocator_bind(struct stack_allocator *stack)
{
	if (stack)
		g_stack_allocator = stack;
}

unsigned char * stack_allocator_malloc_real(unsigned esize)
{
	if (!g_stack_allocator)
		return NULL;

	if (g_stack_allocator->free_size < esize + UNSIGNED_SIZE)
		return NULL;

	if (!g_stack_allocator->top)
		g_stack_allocator->top = g_stack_allocator->data;
	else
		g_stack_allocator->top = g_stack_allocator->top + 
			g_stack_allocator->top_size + UNSIGNED_SIZE;

	g_stack_allocator->top_size = esize;
	g_stack_allocator->num_chunks++;
	g_stack_allocator->free_size -= esize + UNSIGNED_SIZE;

	return g_stack_allocator->top;
}

unsigned char * stack_allocator_free()
{
	if (!g_stack_allocator)
		return NULL;

	if (!g_stack_allocator->top)
		return NULL;

	g_stack_allocator->free_size += g_stack_allocator->top_size +
		UNSIGNED_SIZE;
	g_stack_allocator->top -= g_stack_allocator->top_size + UNSIGNED_SIZE;
	g_stack_allocator->top_size = (unsigned) (*g_stack_allocator->top);
	g_stack_allocator->num_chunks--;
	if (g_stack_allocator->num_chunks == 0) {
		g_stack_allocator->top_size = 0;
		g_stack_allocator->top = NULL;
	}
	
	return g_stack_allocator->top;
}

inline void stack_allocator_clear()
{
	if (!g_stack_allocator)
		return;

	g_stack_allocator->num_chunks = 0;
	g_stack_allocator->top_size = 0;
	g_stack_allocator->top = NULL;
}
