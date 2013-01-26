#ifndef __STACK_H__
#define __STACK_H__

struct stack {
	int count;
	struct elem {
		void *		ptr;
		struct elem *	prev;
	} *head;
};

struct stack * stack_new();

void stack_delete(struct stack **stack);

void stack_push(struct stack *stack, void *data);

void * stack_pop(struct stack *stack);

inline void * stack_top(struct stack *stk);

void stack_clear(struct stack *stack);

#endif
