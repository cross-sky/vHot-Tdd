#include "comm.h"

#define RTLEN 40

//创建全局队列
static Queue_T S_sendQue;
static uint8_t S_sendData[QUEUELENGTH][RTLEN];

//临时存储帧发送1
static uint8_t S_tsend1[RTLEN];

static uint8_t S_tDmaSend1[RTLEN];

static RTRCFlag_T S_rcFlag;

void RTCom3_initRCflag(P_RTRCFlag rcFlag)
{
	rcFlag->event.eventType = UART_TYPE;
	rcFlag->event.eventId = RTCOM3;
	rcFlag->recFlag = DONE;
	rcFlag->sendCount = 0;
	rcFlag->sendCountMax = 3;
}

static uint8_t* getSend1FrameHeaderAddr(void)
{
	return S_tsend1;
}

static uint8_t* getSend1FrameDataAddr(void)
{
	return S_tsend1 + sizeof(RTHeader_T);
}

void RTCom3_createEvent(void)
{
	Queue_create(&S_sendQue, S_sendData, RTLEN);
	RTCom3_initRCflag(&S_rcFlag);
}
bool RTCom3_popEvent(void* dataAddr)
{
	return Queue_pop(&S_sendQue, dataAddr);
}
bool RTCom3_pushEvent(void* dataAddr, uint16_t dataLen)
{
	return Queue_push(&S_sendQue, dataAddr, dataLen);
}
uint16_t RTCom3_lengthEvent(void)
{
	return Queue_length(&S_sendQue);
}
void RTCom3_destoryEvent(void)
{
	Queue_destory(&S_sendQue);
}

uint8_t RTCom3_checkXrr(void* addr, uint8_t len)
{
	uint8_t result = 0;
	uint8_t* src = (uint8_t*)addr;
	while(len > 0)
	{
		result ^= *src++;
		len--;
	}
	return result;
}


void RTCom3_SendCM1(uint8_t vHz)
{
	//add header, data
	P_RTHeader frameHeader;
	P_RTCom3SFrame1 frameData;
	uint8_t xrrLen;
	xrrLen = sizeof(RTHeader_T) + sizeof(RTCom3SFrame1_T);

	frameHeader = (P_RTHeader)getSend1FrameHeaderAddr();
	frameData = (P_RTCom3SFrame1)getSend1FrameDataAddr();

	frameHeader->event.eventType = UART_TYPE;
	frameHeader->event.eventId = RTCOM3;
	frameHeader->len = sizeof(RTCom3SFrame1_T);

	frameData->header.header1 = 0x7e;
	frameData->header.header2 = 0x7e;
	frameData->header.dstAddr = 0xc0;
	frameData->header.srcAddr = 0x00;
	frameData->header.frameCode = 0x11;
	frameData->header.funCode = 0x83;
	frameData->header.len = 8;

	frameData->data.p1Set = vHz;
	frameData->data.p2run = 0;
	frameData->data.p3Code = 0;
	frameData->data.p4Upspeed = 0x0a;
	frameData->data.p5DownSpeed = 0x0a;
	frameData->data.p6Null = 0x00;

	//check err
	frameData->crr = RTCom3_checkXrr(frameData, frameHeader->len - 1);

	//push to queue
	RTCom3_pushEvent(frameHeader, xrrLen);
}

static bool sendNext(P_RTRCFlag rcFlag)
{
	if (rcFlag->sendCount >= rcFlag->sendCountMax)
	{
		//recv err
		//reset rcvflag done
		rcFlag->recFlag = DONE;
		return true;
	}
	if (rcFlag->recFlag == DONE)
	{
		return true;
	}else
	{
		//not receive
		rcFlag->sendCount++;
		return false;
	}
	if (RTCom3_lengthEvent() == 0)
	{
		return false;
	}
}

void RTCom3_SendProcess(P_RTRCFlag rcFlag)
{
	assert(rcFlag);

	if (!sendNext(rcFlag))
	{
		//resend
		//set dma addr at S_tDmaSend1
		return;
	}
	
	//pop next queue
	//set dma addr count, 1.cast to RTHeader_T,obtain len, uartx,
	//2.get data Frame addr,set dma addr, len
	//3.set rcFlag->recFlag = UNDONE;
	//reset sendCount
	RTCom3_popEvent(S_tDmaSend1);
	rcFlag->sendCount = 0;
}

void RTCom3_setRecFlag(P_Event event)
{
	if (event->eventId == RTCOM3)
	{
		S_rcFlag.recFlag = DONE;
	}
}
