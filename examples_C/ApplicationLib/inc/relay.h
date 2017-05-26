#ifndef __RELAY_H
#define __RELAY_H

#include "commdata.h"
#include "valve.h"


typedef enum{
	RELAY1_PUMB,
	RELAY2_VALVE4,
	RELAY3_COMPHEAT,
	RELAY4_CHAHEAT,
	RELAY5_ASSHEAT,
	RELAY6_WAThEAT,
	RELAY7_FUNH,
	RELAY8_FUNL,
	RELAY_MAX
}RELAY_ENUM;

typedef struct  
{
	uint16_t		pin;
	GPIO_TypeDef*	port;
	uint32_t		clk;
}IOControl;

typedef struct RalayVData_T{
	uint16_t relaysAndValveMainA;	//relay µÍÎ»£¬ valveA ¸ßÎ»(12-15)
	uint16_t valveSubB;
}RalayVData_T, *P_RalayVData;

P_RalayVData RV_getDAddr(void);
void RV_setRelay(RELAY_ENUM relay, DONE_ENUM isDone);

void RV_valveRunDirect(VALVESTATE_ENUM addOrSub, VALVEKINDLE_ENUM valveKind);

void RV_Task4OutProcess(void);
void RV_hwInit(void);

#endif

