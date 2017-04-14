
#include <stdio.h>
#include <stdarg.h>
#include "CppUTest/TestHarness.h"

extern "C"
{
#include "rtcom3.h"
}

static uint8_t src[30];
static uint8_t dst[30];
static RTRCFlag_T rcFlag;

TEST_GROUP(RTCom3)
{
void setup()
{
	RTCom3_createEvent();
	memset(dst, 0, 30);
	RTCom3_initRCflag(&rcFlag);
}

void teardown()
{
	RTCom3_destoryEvent();
}
};

TEST(RTCom3, pushAndPop)
{
	//RTCom3Send1_T rtcom3s1;
	src[5] = 3;
	RTCom3_pushEvent(&src, sizeof(RTCom3SFrame1_T));
	RTCom3_popEvent((uint8_t*)dst);
	LONGS_EQUAL(dst[5], src[5]);
}

TEST(RTCom3,createSendData)
{
	//1.create a send data
	//add data to queue
	//check data
	uint8_t xrr;
	xrr = sizeof(RTHeader_T) + sizeof(RTCom3SFrame1_T);
	RTCom3_SendCM1(30);
	RTCom3_popEvent(dst);
	LONGS_EQUAL(0x44, dst[xrr - 1]);
}

TEST(RTCom3,SendWithoutRecv)
{
	uint8_t i;
	RTCom3_SendCM1(30);
	RTCom3_SendCM1(30);
	for (i = 0; i < 3; i++)
	{
		RTCom3_SendProcess(&rcFlag);
		rcFlag.recFlag = UNDONE;
	}
	LONGS_EQUAL(1, RTCom3_lengthEvent());
	//recv err, and send next que
	RTCom3_SendProcess(&rcFlag);
	LONGS_EQUAL(1, RTCom3_lengthEvent());
}

