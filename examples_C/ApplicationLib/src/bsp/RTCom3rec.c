#include "comm.h"

#define RTCOM3_HEADER 0x7e

bool RTCom3_checkRecHeader(P_RTCom3DMA dmaAddr)
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

bool RTCom3_checkRecFrame(void* addr)
{
	//
}
