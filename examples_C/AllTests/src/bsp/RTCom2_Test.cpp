
#include <stdio.h>
#include <stdarg.h>
#include "CppUTest/TestHarness.h"

extern "C"
{
#include "comm.h"
}

#define bit_funcode 2
#define bit_value 3

static uint8_t baseData[8]={
	0xca, 0xca, 0xf0, 0x01, 0xaa
};

static uint8_t src[30]={
};

TEST_GROUP(RTCOM2_TEST)
{
void setup()
{
	RTCom2Rec_createEvent();
	RTCom2_paraInit();
	memcpy(src, baseData, 5);
}

void teardown()
{
	RTCom2Rec_destoryEvent();
}
};

TEST(RTCOM2_TEST, recFuncode0xf0)
{
	RTCom2Rec_pushEvent(&src, 5);
	RTCom2_task();
	LONGS_EQUAL(1, RTCom2Rec_getPrintADCFlag());
}

TEST(RTCOM2_TEST, recFuncode0xf1)
{
	src[bit_funcode] = 0xf1;
	src[bit_value] = 0x00;
	RTCom2Rec_pushEvent(&src, 5);
	RTCom2_task();
	//LONGS_EQUAL(1, RTCom2Rec_getPrintADCFlag());

	src[bit_funcode] = 0xf2;
	src[bit_value] = 0x01;
	src[bit_value+1] = 0x1f;
	RTCom2Rec_pushEvent(&src, 5);
	RTCom2_task();
}

