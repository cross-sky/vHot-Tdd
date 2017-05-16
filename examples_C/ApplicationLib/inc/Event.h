#ifndef __EVENT_H
#define __EVENT_H

#include "commdata.h"

typedef enum{
	FUN_TYPE=1,
	UART_TYPE,
	VALVE_TYPE,
}EVENTTYPE;

typedef struct _EventStruct{
	EVENTTYPE eventType;
	uint16_t eventId;
}Event_T, *P_Event;

#endif
