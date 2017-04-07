
#include <stdio.h>
#include <stdarg.h>
#include "CppUTest/TestHarness.h"

extern "C"
{
#include "ComInputDI.h"
#include "ComInputDISpy.h"
}

TEST_GROUP(ComInputDI)
{
void setup()
{
	ComInputDI_create();
	ComInputDISpy_crate();
}

void teardown()
{
	ComInputDI_destory();
}
};

TEST(ComInputDI, convert5Times)
{
	uint8_t i;
	
	for (i = 0; i < 5; i ++)
	{
		ComInputDI_process();
	}

	ComInputDI_process();

	for (i = 0; i < DI_MAX_ENUM; i++)
	{
		LONGS_EQUAL(1, ComInputDI_getDIValue(i));
	}
}
