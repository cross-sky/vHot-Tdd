
#include <stdio.h>
#include <stdarg.h>
#include "CppUTest/TestHarness.h"

extern "C"
{
#include "relay.h"
}

static P_RalayVData s_rvData;

TEST_GROUP(RelayTest)
{
void setup()
{
	s_rvData = RV_getDAddr();
	s_rvData->relaysAndValveMainA = 0;
	s_rvData->valveSubB = 0;
}

void teardown()
{
	s_rvData->relaysAndValveMainA = 0;
	s_rvData->valveSubB = 0;
}
};

TEST(RelayTest, relayOnOff)
{
	RV_setRelay(RELAY1_PUMB, DONE);
	LONGS_EQUAL(1, s_rvData->relaysAndValveMainA);
	RV_setRelay(RELAY1_PUMB, UNDONE);
	LONGS_EQUAL(0, s_rvData->relaysAndValveMainA);
}
