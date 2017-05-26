
#include <stdio.h>
#include <stdarg.h>
#include "CppUTest/TestHarness.h"

extern "C"
{
#include "ComInput.h"
}

static uint8_t hardFlag = DONE;
static ComInput_T comInput;


static void hardFun(void)
{
	printf("test hard done\r\n");
	hardFlag = DONE;
}

static void aveFun(void)
{
	printf("test avefun\r\n");
}

static uint8_t hardFlagFun(void)
{
	return hardFlag;
}
static void clearHardFlagFun(void)
{
	static uint8_t i = 0;
	printf("test clear hard flag count:%d.\r\n", i++);
	hardFlag = UNDONE;
}

static void checkCount(uint8_t i)
{
	LONGS_EQUAL(i, comInput.count);
}
static void checkRunTime(uint8_t i)
{
	LONGS_EQUAL(i, comInput.runTime);
}
static void initComInput(void)
{
	comInput.count = 0;
	comInput.maxCount = 5;
	comInput.runTime = 0;
	comInput.maxRunTime = 10;
	comInput.convertFlag = UNDONE;
	comInput.waitTime = 0;
	comInput.maxWaitTime = 3;
	comInput.hardFun = hardFun;
	comInput.aveFun = aveFun;
	comInput.hardFlagFun = hardFlagFun;
	comInput.clearHardFlagFun = clearHardFlagFun;
}

TEST_GROUP(ComInput)
{
void setup()
{
	initComInput();
	ComInput_Create();
}

void teardown()
{
	ComInput_Destory(&comInput);
}
};

TEST(ComInput, run5Wait10)
{
	uint8_t i;
	//5 times convert
	for (i = 1; i <= 5; i++)
	{
		ComInput_Process(&comInput);
		checkCount(i);
		checkRunTime(i);
	}
	
	//then convert done, run ave fun
	ComInput_Process(&comInput);
	LONGS_EQUAL(DONE, comInput.convertFlag);

	//wait max runtime
	for (i = 7; i < 10; i++)
	{
		ComInput_Process(&comInput);
		checkRunTime(i);
	}
	//after wait,again convert
	ComInput_Process(&comInput);
	LONGS_EQUAL(UNDONE, comInput.convertFlag);
}
