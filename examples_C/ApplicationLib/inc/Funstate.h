#ifndef __FUNSTATE_H
#define __FUNSTATE_H

#include "Queue.h"
#include "Event.h"



typedef enum{
	FUN_STATE_EXIT,
	FUN_STATE_INIT,
	FUN_STATE_RUN,
	FUN_STATE_NULL,
	FUN_STATE_MAX
}STATEFUN_ENUM;

typedef enum{
	SIG_FUN_OFF,
	SIG_FUN_ON,
	SIG_FUN_MAX
}FUN_ENUM;

typedef STATEFUN_ENUM (*pRunFunctions)(void);

typedef struct _pRunFunStruct{
	pRunFunctions init;
	pRunFunctions run;
	pRunFunctions exit;
}RunFun_T, *P_RunFun;

typedef struct _FundataStruct{
	DONE_ENUM isChange;
	STATEFUN_ENUM curFunState;
	FUN_ENUM curFun;
	FUN_ENUM preFun;
}Fundata_T, *P_Fundata;

void Funstate_createEvent(void);
bool Funstate_popEvent(P_Event funEvent);
bool Funstate_pushEvent(P_Event funEvent);
uint16_t Funstate_lengthEvent(void);
void Funstate_destoryEvent(void);

void Fundata_create(P_Fundata fundata);
void Fundata_destory(P_Fundata fundata);
bool Funstate_processEvent(void);

FUN_ENUM Fundata_getCurFunSig(void);
STATEFUN_ENUM Fundata_getFunChangeState(void);

#endif

