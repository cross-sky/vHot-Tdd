
#include <stdio.h>
#include <stdarg.h>
#include "CppUTest/TestHarness.h"

extern "C"
{
#include "Stack_a.h"
}

Stack_T _stk;

TEST_GROUP(Stack_a)
{
	
void setup()
{
	_stk = Stack_new();
}

void teardown()
{
	Stack_free(&_stk);
}
};

TEST(Stack_a, pushAndPop)
{
	int dst = 100;
	int *test = NULL;
	Stack_push(_stk, &dst);
	test = (int*)Stack_pop(_stk);
	LONGS_EQUAL(dst, *test);
}
