#include <stdio.h>
#include <stdarg.h>
#include "CppUTest/TestHarness.h"

extern "C"
{
#include "Queue.h"

}
#define dataSize 10

int src=1200;
int dst=0;
Queue_T _queue;

uint16_t dataBuf[QUEUELENGTH][dataSize];

TEST_GROUP(QueueS)
{
void setup()
{
	Queue_create(&_queue, (void*)dataBuf, sizeof(dataBuf) / QUEUELENGTH);
	src=1200;
}

void teardown()
{
	Queue_destory(&_queue);
	src = 0;
	dst = 0;
}
};

TEST(QueueS, pushAndPop)
{
	src = 1000;
	Queue_push(&_queue, &src, sizeof(int));
	src = 1100;
	Queue_push(&_queue, &src, sizeof(int));
	Queue_pop(&_queue, &dst);
	LONGS_EQUAL(dst, 1000);
	Queue_pop(&_queue, &dst);
	LONGS_EQUAL(dst, 1100);
}


TEST(QueueS, fullThenPush)
{
	uint16_t i;
	bool result;
	for (i = 0; i < QUEUELENGTH; i++)
	{
		Queue_push(&_queue, &src, sizeof(int));
	}
	result = Queue_push(&_queue, &src, sizeof(int));
	LONGS_EQUAL(false, result);
}

TEST(QueueS, nullThenPop)
{
	bool result;
	result = Queue_pop(&_queue, &dst);
	LONGS_EQUAL(false, result);
}

