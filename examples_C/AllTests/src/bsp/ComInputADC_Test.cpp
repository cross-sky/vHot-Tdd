#include <stdio.h>
#include <stdarg.h>
#include "CppUTest/TestHarness.h"

extern "C"
{
#include "ComInputADC.h"
#include "ComInputADCSpy.h"
#include "adc_filter.h"
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
		ComInputADC_setHardFlagFun();
	}

	ComInputADC_process();

	for (i = 0; i < ADCIN_MAX_ENUM; i++)
	{
		LONGS_EQUAL(4 + i + ADCSPY_TESTVALUE, ComInputADC_getADCValue(i));
	}

}

TEST(ComInputADC, testADCValue)
{
	//1985 means 250
	uint16_t searchValue = 1985,index;
	int16_t realValue;
	index=uADCSearchData5K(searchValue);
	realValue = iADCTemperCalc5K(index,searchValue);
	LONGS_EQUAL(250, realValue);
	printf("realValue is %d\r\n", realValue);
}
