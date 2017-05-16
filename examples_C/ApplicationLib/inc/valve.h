#ifndef __VALVE_H
#define __VALVE_H

#include "commdata.h"
#include "rtdata.h"

typedef enum{
	VALVE_INIT,
	VALVE_RUN,
	VALVE_CLOSE,
	VALVE_BACK,		//反方向(减少)
	VALVE_HOLD,		//不运行
	VALVE_ADD,	//运行方向 1正方向(增加)
	VALVE_USED,
	VALVE_UNUSED,
}VALVESTATE_ENUM;

typedef enum{
	VALVE_MAINA,
	VALVE_SUBB,
	VALVE_MAX,
}VALVEKINDLE_ENUM;

typedef struct EventValve_T{
	Event_T event;	//type valve, id valvekind
	int16_t code;	//run steps
	VALVESTATE_ENUM state; //init run close
}EventValve_T, *P_EventValve;

typedef struct {
	DONE_ENUM isDone;
	int16_t totalSteps;
	int16_t curSteps;
	int16_t runSteps;
	VALVESTATE_ENUM runDirect;
	VALVESTATE_ENUM isUsed;
}ValveStatus_T, *P_ValveStatus;

void Valve_createEvent(void);
bool Valve_popEvent(P_EventValve event);
bool Valve_getEvent(P_EventValve event);
bool Valve_pushEvent(P_EventValve event);
uint16_t Valve_lenEvent(void);
void Valve_destoryEvent(void);
void Valve_ProcessEvent(VALVEKINDLE_ENUM valveKind);
uint16_t Valve_getState(VALVEKINDLE_ENUM valveKind);
#endif

