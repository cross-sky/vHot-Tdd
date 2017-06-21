
#include <stdio.h>
#include <stdarg.h>
#include "CppUTest/TestHarness.h"

extern "C"
{
#include "valve.h"
#include "ComInputADC.h"
}

EventValve_T src, dst;
uint16_t runcode = 30;

TEST_GROUP(ValveTest)
{
void setup()
{
	Valve_createEvent();
	src.event.eventType = VALVE_TYPE;
	src.event.eventId = VALVE_TYPE_MAINA;
	src.code = runcode;
	src.state = VALVE_RUN;
}

void teardown()
{
	Valve_destoryEvent();
	memset(&dst, 0, sizeof(EventValve_T));
}
};

TEST(ValveTest, pushAndPop)
{
	Valve_pushEvent(&src);
	Valve_getEvent(&dst);
	LONGS_EQUAL(VALVE_TYPE, dst.event.eventType);
	LONGS_EQUAL(VALVE_RUN, dst.state);
}

TEST(ValveTest, processEvent)
{
	uint8_t i, j = 5;
	Valve_pushEvent(&src);
	for (i = 0; i<= runcode + 10; i++)
	{
		Valve_ProcessEvent(VALVE_TYPE_MAINA);
		LONGS_EQUAL(UNDONE, Valve_getState(VALVE_TYPE_MAINA));
	}
	Valve_ProcessEvent(VALVE_TYPE_MAINA);
	LONGS_EQUAL(DONE, Valve_getState(VALVE_TYPE_MAINA));
}

TEST(ValveTest, CalcValve)
{
	ADC_setRealData(ADCIN11_AOUT, 800);
	ADC_setRealData(ADCIN0_AIN, 100);
	ADC_setRealData(ADCPT1_L, 30);
	ValveCalc_calcValveMain(VALVE_TYPE_MAINA);

	Valve_popEvent(&dst);
	LONGS_EQUAL(VALVE_STEPS_ONECE, dst.code); //first forward

	ValveCalc_calcValveMain(VALVE_TYPE_MAINA);
	Valve_popEvent(&dst);
	LONGS_EQUAL(VALVE_STEPS_ONECE, dst.code);//again forward

	ADC_setRealData(ADCPT1_L, 60);	//100-40 = 40 < 50
	ValveCalc_calcValveMain(VALVE_TYPE_MAINA);
	Valve_popEvent(&dst);
	LONGS_EQUAL(-VALVE_STEPS_ONECE/2, dst.code);//then back


}

