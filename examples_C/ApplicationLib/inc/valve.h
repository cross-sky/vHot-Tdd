#ifndef __VALVE_H
#define __VALVE_H

#include "commdata.h"
#include "rtdata.h"

#define VALVE_STEPS_ONECE	32	//默认每次运行最大步数

typedef enum{
	VALVE_INIT,
	VALVE_RUN,
	VALVE_CLOSE,
	VALVE_BACK,		//反方向(减少)
	VALVE_HOLD,		//不运行
	VALVE_ADD,	//运行方向 1正方向(增加)
	VALVE_USED,
	VALVE_UNUSED,
	DirectBack,		//反方向(减少)
	DirectHold,		//不运行
	DirectForward,	//运行方向 1正方向(增加)
}VALVESTATE_ENUM;

typedef enum{
	VALVE_TYPE_MAINA,
	VALVE_TYPE_SUBB,
	VALVE_TYPE_MAX,
}VALVEKINDLE_ENUM;

typedef struct EventValve_T{
	Event_T event;	//type valve, id valvekind
	int16_t code;	//run steps
	VALVESTATE_ENUM state; //init run close
}EventValve_T, *P_EventValve;

typedef struct {
	DONE_ENUM isDone;
	int16_t totalSteps;
	int16_t runSteps;
	VALVESTATE_ENUM isUsed;
	int16_t calcLastStep;	//previous一次步数
	int16_t calcDirection;//运行方向direct**
	int16_t calcCounts;//连续增加或减少次数
}ValveStatus_T, *P_ValveStatus;

void Valve_createEvent(void);
bool Valve_popEvent(P_EventValve event);
bool Valve_getEvent(P_EventValve event);
bool Valve_pushEvent(P_EventValve event);
uint16_t Valve_lenEvent(void);
void Valve_destoryEvent(void);
void Valve_ProcessEvent(VALVEKINDLE_ENUM valveKind);
uint16_t Valve_getState(VALVEKINDLE_ENUM valveKind);
void RVOUT_taskProcess(void);
void Valve_hwInit(void);


void RV_clearValveValue(VALVEKINDLE_ENUM valveKind);
int16_t Valve_getTotalSteps(VALVEKINDLE_ENUM valvekindle);
void Valve_setToStep(VALVEKINDLE_ENUM valveKindle, int16_t steps, VALVESTATE_ENUM state);

void ValveCalc_calcValveMain(VALVEKINDLE_ENUM valveKind);

void ValveCalc_task(void);
void ValveClac_closeClac(VALVEKINDLE_ENUM valveKind);
void ValveClac_startClac(VALVEKINDLE_ENUM valveKind);

#endif

