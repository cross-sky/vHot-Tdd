
#include <stdio.h>
#include <stdarg.h>
#include "CppUTest/TestHarness.h"

extern "C"
{
#include "comm.h"
}

static Event_T S_srcFunEvent;
static Event_T S_dstFunEvent;

static uint16_t S_eventType;
static uint16_t S_eventId;

static Fundata_T S_fundata;

static void initFunevent(P_Event event, uint16_t eventType, uint16_t eventId)
{
	event->eventType = (EVENTTYPE)eventType;
	event->eventId = eventId;
}
static void checkEvent(void)
{
	LONGS_EQUAL(S_dstFunEvent.eventId, S_srcFunEvent.eventId);
	LONGS_EQUAL(S_dstFunEvent.eventType, S_srcFunEvent.eventType);
}
TEST_GROUP(Funstate)
{
void setup()
{
	Funstate_createEvent();
	initFunevent(&S_dstFunEvent, 0, 0);

	Fundata_create(&S_fundata);
}

void teardown()
{
	Funstate_destoryEvent();

	Fundata_destory(&S_fundata);
}
};

TEST(Funstate, PushAndPopEvent)
{
	S_eventType = 8;
	S_eventId = 4;
	initFunevent(&S_srcFunEvent, S_eventType, S_eventId);
	Funstate_pushEvent(&S_srcFunEvent);
	Funstate_popEvent(&S_dstFunEvent);
	checkEvent();
}

TEST(Funstate, preFunexitToOn)
{
	S_eventType = FUN_TYPE;
	S_eventId = SIG_FUN_ON;
	initFunevent(&S_srcFunEvent, S_eventType, S_eventId);
	Funstate_pushEvent(&S_srcFunEvent);

	Funstate_processEvent();
	LONGS_EQUAL(FUN_STATE_INIT, Fundata_getFunChangeState());

	Funstate_processEvent();
	LONGS_EQUAL(FUN_STATE_RUN, Fundata_getFunChangeState());

	Funstate_processEvent();
	LONGS_EQUAL(FUN_STATE_RUN, Fundata_getFunChangeState());

	Funstate_processEvent();
	LONGS_EQUAL(FUN_STATE_RUN, Fundata_getFunChangeState());
}
