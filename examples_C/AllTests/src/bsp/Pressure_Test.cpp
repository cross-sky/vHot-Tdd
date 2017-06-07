
#include <stdio.h>
#include <stdarg.h>
#include "CppUTest/TestHarness.h"

extern "C"
{
#include "pressure.h"
}

TEST_GROUP(PRESSURE_TEST)
{
void setup()
{

}

void teardown()
{

}
};

//low pressure test, adc=2081, mean t = 80
TEST(PRESSURE_TEST, TESTLPressure)
{
	uint16_t adc = 2081, temper = 80, index;
	uint16_t dst;
	index=uPresLSearchData(adc);
	dst = iPresLCalc(index,adc);
	LONGS_EQUAL(temper, dst);
}

//low pressure test, adc=2081, mean t = 80
TEST(PRESSURE_TEST, TESTHPressure)
{
	uint16_t adc = 1136, temper = 80, index;
	uint16_t dst;
	index=uPresHSearchData(adc);
	dst = iPresHCalc(index,adc);
	LONGS_EQUAL(temper, dst);

	adc = 1147;
	index=uPresHSearchData(adc);
	dst = iPresHCalc(index,adc);
	LONGS_EQUAL(85, dst);
}
