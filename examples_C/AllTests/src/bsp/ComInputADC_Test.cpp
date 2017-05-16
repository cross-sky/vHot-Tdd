#include <stdio.h>
#include <stdarg.h>
#include "CppUTest/TestHarness.h"

extern "C"
{
#include "ComInputADC.h"
#include "ComInputADCSpy.h"
}

TEST_GROUP(ComInputADC)
{
void setup()
{
	ComInputADC_create();
	ComInputADCSpy_crate();
}

void teardown()
{
	ComInputADC_destory();
}
};

TEST(ComInputADC, convert5Times)
{
	uint8_t i;
	
	for (i = 0; i < 5; i ++)
	{
		ComInputADC_process();
	}

	ComInputADC_process();

	for (i = 0; i < ADCIN_MAX_ENUM; i++)
	{
		LONGS_EQUAL(4 + i, ComInputADC_getADCValue(i));
	}
}
