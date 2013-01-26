#include "stack.h"
#include "common.h"

struct stack * stack_new()
{
	struct stack *stk;

	NEW2(stk);
	stk->count = 0;
	stk->head  = NULL;

	return stk;
}

void stack_delete(struct stack **stk)
{
	struct elem *d, *e;

	assert(stk && *stk);
	for (e = (*stk)->head; e; e = d) {
		d = e->prev;
		FREE(e);
	}
}

void stack_push(struct stack *stk, void *ptr)
{
	struct elem *e;

	assert(stk);
	NEW2(e);
	e->ptr = ptr;
	e->prev = stk->head;
	stk->head = e;
	stk->count++;
}

void * stack_pop(struct stack *stk)
{
	void *ptr;
	struct elem *e;

	assert(stk);
	assert(stk->count > 0);
	e = stk->head;
	stk->head = e->prev;
	stk->count--;
	ptr = e->ptr;
	FREE(e);

	return ptr;
}

inline void * stack_top(struct stack *stk)
{
	return stk->head->ptr;
}
