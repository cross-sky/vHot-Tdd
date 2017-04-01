
#include <stdio.h>
#include <stdarg.h>
#include "CppUTest/TestHarness.h"

extern "C"
{
#include "ComInput.h"
}

static uint8_t hardFlag = UNDONE;
static ComInput_T comInput;


static void hardFun(void)
{
	printf("hardfun\r\n");
	hardFlag = DONE;
}

static void aveFun(void)
{
	printf("avefun\r\n");
}

static uint8_t hardFlagFun(void)
{
	return hardFlag;
}
static void clearHardFlagFun(void)
{
	static uint8_t i = 0;
	printf("clear hard flag count:%d.\r\n", i++);
	hardFlag = UNDONE;
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
}

TEST_GROUP(ComInput)
{
void setup()
{
	ComInput_Create();
}

void teardown()
{
	ComInput_Destory(&comInput);
}
};

TEST(ComInput, run5Wait10)
{
	
}
