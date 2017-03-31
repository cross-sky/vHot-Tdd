#include <stddef.h>
#include "Stack_a.h"
#include "assert.h"
#include "mem.h"

//#define T Stack_T
#define  T Stack_T

typedef struct elem{
	void *x;
	struct elem *link;
} elem;

typedef struct Stack_struct{
	int16_t count;
	elem *head;
};

T Stack_new(void)
{
	T stk;

	NEW(stk);
	stk->count = 0;
	stk->head = NULL;
	return stk;
}

int16_t Stack_empty(T stk)
{
	assert(stk);
	return stk->count == 0;
}

void Stack_push(T stk, void *x)
{
	struct elem *t;
	
	assert(stk);
	NEW(t);
	t->x = x;
	t->link = stk->head;
	stk->head = t;
	stk->count++;
}

void* Stack_pop(T stk)
{
	void *x;
	elem *t;

	assert(stk);
	assert(stk->count > 0);

	t = stk->head;
	stk->head = t->link;
	stk->count--;
	x = t->x;
	FREE(t);
	return x;
}

//free stk
void Stack_free(T *stk)
{
	elem *t, *u;

	assert(stk && *stk);
	for (t = (*stk)->head; t; t = u)
	{
		u = t->link;
		FREE(t);
	}
	FREE(*stk);
}