#include "comm.h"

#define RTLEN 40

//创建全局队列
static Queue_T S_recQue;
static uint8_t S_recData[QUEUELENGTH][RTLEN];

//临时存储帧发送1
#define RTCOM3_DMALEN 60
static uint8_t S_trec[RTCOM3_DMALEN];

#define RTCOM3_HEADER 0x7e

void RTCom3Rec_createEvent(void)
{
	Queue_create(&S_recQue, S_recData, RTLEN);
}
bool RTCom3Rec_popEvent(void* dataAddr)
{
	return Queue_pop(&S_recQue, dataAddr);
}
bool RTCom3Rec_pushEvent(void* dataAddr, uint16_t dataLen)
{
	return Queue_push(&S_recQue, dataAddr, dataLen);
}
uint16_t RTCom3Rec_lengthEvent(void)
{
	return Queue_length(&S_recQue);
}
void RTCom3Rec_destoryEvent(void)
{
	Queue_destory(&S_recQue);
}


bool RTCom3Rec_checkHeader(P_RTCom3DMA dmaAddr)
{
	uint8_t* addr = dmaAddr->addr;
	uint16_t len = dmaAddr->len;

	while(len > 0)
	{
		if (*addr++ == RTCOM3_HEADER && *addr == RTCOM3_HEADER)
		{
			dmaAddr->addr = addr - 1;
			dmaAddr->len = len;
			return true;
		}
		len--;
	}
	return false;
}


bool RTCom3Rec_checkFrame(P_RTCom3DMA dmaAddr)
{
	P_RTCom3RFrame1 rec = (P_RTCom3RFrame1)dmaAddr->addr;
	P_RTHeader eventHeader;
	uint16_t len = dmaAddr->len;
	uint16_t dataLen;
	uint8_t xrrResult = 0;

	//check dstAddr
	if (rec->header.dstAddr != 0x00)
	{
		return false;
	}

	//check srcAddr

	//check fun code
	if (rec->header.funCode != 0x03)
	{
		return false;
	}
	//.check xrr
	dataLen = sizeof(RTCom3RFrame1_T);
	if (len < dataLen)
	{
		return false;
	}
	xrrResult = RTCom3_checkXrr(rec, dataLen - 1);

	if (xrrResult != rec->crr)
	{
		return false;
	}

	//push data to queue
	//or update data
	//add header message
	//sizeof(RTHeader_T) = 12
	eventHeader = (P_RTHeader)(dmaAddr->addr - sizeof(RTHeader_T));
	eventHeader->event.eventType = UART_TYPE;
	eventHeader->event.eventId = RTCOM3;
	eventHeader->len = dataLen + sizeof(RTHeader_T);

	//check send flag, then set recflag


	RTCom3Rec_pushEvent(eventHeader, eventHeader->len);
	RTCom3_setRecFlag(&eventHeader->event);
	return true;
}

void RTCom3Rec_recProcess(P_RTRCFlag rcFlag)
{
	if (RTCom3Rec_lengthEvent() == 0)
	{
		return;
	}
	RTCom3Rec_popEvent(S_trec);

	//dataProcess
	//update data
	//check err

	//rcFlag->recFlag = DONE;
}
