#ifndef __EVENT_H
#define __EVENT_H

#include "commdata.h"

typedef enum{
	FUN_TYPE=1
}EVENTTYPE;

typedef struct _EventStruct{
	EVENTTYPE eventType;
	uint16_t eventId;
}Event_T, *P_Event;

/**
void Event_create(P_Event event, void* dataAddr, uint16_t dataSize);
bool Event_pop(Queue_T que, void* dstAddr);
bool Event_push(Queue_T que, void* dataAddr, uint16_t dataLen);
uint16_t Event_length(Queue_T que);
void Event_destory(Queue_T que);
**/
#endif
