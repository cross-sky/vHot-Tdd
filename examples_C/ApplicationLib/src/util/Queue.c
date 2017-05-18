#include "comm.h"
#include <string.h>

void Queue_create(P_Queue que, void* dataAddr, uint16_t dataSize)
{
	uint8_t i;
	uint16_t datref=0;
	que->in = 0;
	que->out = 0;
	que->qLen = 0;
	que->maxSize = QUEUELENGTH;
	for(i = 0; i < QUEUELENGTH; i++)
	{
		que->data.buff[i] = (uint8_t*)dataAddr + datref;
		datref += dataSize;
	}
}
bool Queue_pop(P_Queue que, void* dstAddr)
{
	if(Queue_length(que) <= 0)
	{
		return false;
	}
	memcpy(dstAddr, que->data.buff[que->out], que->data.len[que->out]);
	//que->out = (que->out+1)%que->maxSize;
	//que->out++;
	if (++que->out >=  que->maxSize)
	{
		que->out = 0;
	}
	que->qLen--;
	return true;
}

bool Queue_get(P_Queue que, void* dstAddr)
{
	if(Queue_length(que) <= 0)
	{
		return false;
	}
	memcpy(dstAddr, que->data.buff[que->out], que->data.len[que->out]);
	return true;
}

static bool isFull(P_Queue que)
{
	return que->qLen >= que->maxSize;
}
bool Queue_push(P_Queue que, void* dataAddr, uint16_t dataLen)
{
	if (isFull(que))
	{
		return false;
	}
	assert(dataLen);
	assert(dataAddr);

	memcpy(que->data.buff[que->in], dataAddr, dataLen);
	que->data.len[que->in] = dataLen;
	//que->in++;
	//que->in = (que->in+1)%que->maxSize;
	if (++que->in >=  que->maxSize)
	{
		que->in = 0;
	}
	que->qLen++;
	return true;
}
uint16_t Queue_length(P_Queue que)
{
	return que->qLen;
}
void Queue_destory(P_Queue que)
{
	que->in = 0;
	que->out = 0;
	que->qLen = 0;
}


