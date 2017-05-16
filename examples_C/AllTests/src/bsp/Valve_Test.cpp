
#include <stdio.h>
#include <stdarg.h>
#include "CppUTest/TestHarness.h"

extern "C"
{
#include "valve.h"
}

EventValve_T src, dst;
uint16_t runcode = 30;

TEST_GROUP(ValveTest)
{
void setup()
{
	Valve_createEvent();
	src.event.eventType = VALVE_TYPE;
	src.event.eventId = VALVE_MAINA;
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
	uint8_t i;
	Valve_pushEvent(&src);
	for (i = 0; i<= runcode + 10; i++)
	{
		Valve_ProcessEvent(VALVE_MAINA);
		LONGS_EQUAL(UNDONE, Valve_getState(VALVE_MAINA));
	}
	Valve_ProcessEvent(VALVE_MAINA);
	LONGS_EQUAL(DONE, Valve_getState(VALVE_MAINA));
}
