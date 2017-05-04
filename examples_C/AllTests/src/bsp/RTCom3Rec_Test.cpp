
#include <stdio.h>
#include <stdarg.h>
#include "CppUTest/TestHarness.h"

extern "C"
{
#include "rtcom3rec.h"
}

#define DMADATA_LEN 60
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

TEST_GROUP(RTCom3RecTes)
{
void setup()
{
	RTCom3Rec_createEvent();
	createDmaData(S_dmaDataT + 16);
}

void teardown()
{
	RTCom3Rec_destoryEvent();
}
};

TEST(RTCom3RecTes, receiveDatas)
{
	//offset 16
	RTCom3DMA_T srdata;
	srdata.addr = S_dmaDataT + 16;
	srdata.len = sizeof(RTCom3RFrame1_T) + 16;

	RTCom3Rec_checkHeader(&srdata);
	RTCom3Rec_checkFrame(&srdata);

	LONGS_EQUAL(1, RTCom3Rec_lengthEvent());
}


