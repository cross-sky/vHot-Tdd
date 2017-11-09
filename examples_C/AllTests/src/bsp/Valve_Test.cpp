
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

TEST(ValveTest, CalcMainValve)
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

TEST(ValveTest, CalcSubValve)
{
	//env=40, econin=400, econout=460; economizerT=50;
	ADC_setRealData(ADCIN2_ENV, 40);
	ADC_setRealData(ADCIN7_ECONIN, 400);
	ADC_setRealData(ADCIN8_ECONOUT, 460);
	ValveCalc_calcValveSub(VALVE_TYPE_SUBB);

	Valve_popEvent(&dst);
	LONGS_EQUAL(VALVE_STEPS_ONECE/2, dst.code); //first forward

	ADC_setRealData(ADCIN8_ECONOUT, 440);
	ValveCalc_calcValveSub(VALVE_TYPE_SUBB);

	Valve_popEvent(&dst);
	LONGS_EQUAL(-VALVE_STEPS_ONECE/4, dst.code); //second back

	ADC_setRealData(ADCIN2_ENV, 51);
	ValveCalc_calcValveSub(VALVE_TYPE_SUBB);
	Valve_popEvent(&dst);
	LONGS_EQUAL(30, dst.code); // env set to 51, close sub valve

	dst.code = 0;
	ValveCalc_calcValveSub(VALVE_TYPE_SUBB);
	Valve_popEvent(&dst);
	LONGS_EQUAL(0, dst.code);
}

