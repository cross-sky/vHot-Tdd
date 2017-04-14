#ifndef __RTDATA_H
#define __RTDATA_H

#include "Event.h"

typedef struct _RTSendFlagStruct{
	Event_T event;
	uint8_t sendCount;
	uint8_t sendCountMax;
	uint8_t recFlag;
}RTRCFlag_T, *P_RTRCFlag;


void RTSend_createEvent(void);
bool RTSend_popEvent(P_Event funEvent);
bool RTSend_pushEvent(P_Event funEvent);
uint16_t RTSend_lengthEvent(void);
void RTSend_destoryEvent(void);

#endif

