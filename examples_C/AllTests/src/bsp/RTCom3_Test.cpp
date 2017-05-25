
#include <stdio.h>
#include <stdarg.h>
#include "CppUTest/TestHarness.h"

extern "C"
{
#include "rtcom3.h"
#include "spy.h"
}

static uint8_t src[30];
static uint8_t dst[30];
static RTRCFlag_T rcFlag;

#define DMADATA_LEN 40
static uint8_t S_dmaDataT[DMADATA_LEN];

static void createDmaData(uint8_t* src)
{
	P_RTCom3RFrame1 fram = (P_RTCom3RFrame1)src;
	fram->header.header1 = 0x7e;
	fram->header.header2 = 0x7e;
	fram->header.dstAddr = 0x00;
	fram->header.srcAddr = 0xc0;
	fram->header.frameCode = 0x11;
	fram->header.len = sizeof(RTCom3_RData1Value_T) + 2;
	fram->header.funCode = 0x03;

	fram->data.p0Sethz = 0x1e;
	fram->data.p1RunHz = 0;
	fram->data.p2State = 1;
	fram->data.p3IH = 0;
	fram->data.p4IL = 0;
	fram->data.p5UH = 1;
	fram->data.p6UL = 0x6a;
	fram->data.p7Err1 = 0;
	fram->data.p8Err2 = 0;
	fram->data.p9Err3 = 0;
	fram->data.p10Temper = 0x88;
	fram->data.p11compVer = 0x01;
	fram->data.p12SoftVer = 1;
	fram->data.p13Iuu = 0xff;
	fram->data.p14Ivv = 0xff;
	fram->data.p15Err4 = 0;
	fram->data.p16PFCT = 0x64;
	fram->data.p17U220 = 0;
	fram->data.p18I220H = 0;
	fram->data.p19I220L = 0;
	fram->data.p20KWH = 0;
	fram->data.p21KWL = 0x14;
	fram->crr = 0x46;
}

void RTCom3_SendCM1(void)
{
	//add header, data
	P_RTCom3SFrame1 frameData;
	uint8_t xrrLen;
	xrrLen = sizeof(RTCom3SFrame1_T);
	frameData = (P_RTCom3SFrame1)dst;

	frameData->header.header1 = 0x7e;
	frameData->header.header2 = 0x7e;
	frameData->header.dstAddr = 0xc0;
	frameData->header.srcAddr = 0x00;
	frameData->header.frameCode = 0x11;
	frameData->header.funCode = 0x83;
	frameData->header.len = 8;

	frameData->data.p1Set = 30;
	frameData->data.p2run = 0;
	frameData->data.p3Code = 0;
	frameData->data.p4Upspeed = 0x0a;
	frameData->data.p5DownSpeed = 0x0a;
	frameData->data.p6Null = 0x00;

	//check err
	frameData->crr = RTCom3_checkXrr(frameData, xrrLen - 1);

	//enable dma tx
	vuart3DmaTxDataEnable(xrrLen, (uint8_t*)frameData);
}

TEST_GROUP(RTCom3)
{
void setup()
{
	RTCom3_createEvent();
	memset(dst, 0, 30);
	RTCom3_initRCflag(&rcFlag);
	createDmaData(S_dmaDataT);
}

void teardown()
{
	RTCom3_destoryEvent();
}
};

TEST(RTCom3, pushAndPop)
{
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
	uint8_t *addr;
	//xrr = sizeof(RTHeader_T) + sizeof(RTCom3SFrame1_T);
	xrr =  sizeof(RTCom3SFrame1_T);
	RTCom3_SendCM1();
	addr = SPY_getDstAddress();
	LONGS_EQUAL(0x44, addr[xrr - 1]);
}

TEST(RTCom3,SendWithoutRecv)
{
	uint8_t i;
	for (i = 0; i < 30; i++)
	{
		RTCom3_SendProcess(&rcFlag);
	}
	LONGS_EQUAL(29, rcFlag.sendCount);

	rcFlag.recFlag = DONE;
	RTCom3_SendProcess(&rcFlag);
	LONGS_EQUAL(0, rcFlag.sendCount);
}

TEST(RTCom3, checkRecFrame)
{
	uint8_t result;
	RTCom3_pushEvent(S_dmaDataT, 30);
	result = RTCom3_recProcess();
	LONGS_EQUAL(1, result);
	LONGS_EQUAL(0, RTCom3_lengthEvent());
}


