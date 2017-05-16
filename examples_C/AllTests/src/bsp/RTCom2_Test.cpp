
#include <stdio.h>
#include <stdarg.h>
#include "CppUTest/TestHarness.h"

extern "C"
{
#include "comm.h"
}

#define bit_funcode 2
#define bit_value 3

static uint8_t src[30]={
	0xca, 0xca, 0xf0, 0x01, 0xaa	
};

TEST_GROUP(RTCOM2_TEST)
{
void setup()
{
	RTCom2Rec_createEvent();
	RTCom2_paraInit();
}

void teardown()
{
	RTCom2Rec_destoryEvent();
}
};

TEST(RTCOM2_TEST, recFuncode0xf1)
{
	RTCom2Rec_pushEvent(&src, 5);
	RTCom2Rec_recProcess();
	LONGS_EQUAL(1, RTCom2Rec_getPrintADCFlag());
}
