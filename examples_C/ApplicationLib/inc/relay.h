#ifndef __RELAY_H
#define __RELAY_H

#include "commdata.h"

typedef enum{
	RELAY1_PUMB,
	RELAY2_VALVE4,
	RELAY3_COMPHEAT,
	RELAY4_CHAHEAT,
	RELAY5_ASSHEAT,
	RELAY6_WAThEAT,
	RELAY7_FUN,
	RELAY_MAX
}RELAY_ENUM;

typedef struct RalayVData_T{
	uint16_t relaysAndValveMainA;	//relay µÍÎ»£¬ valveA ¸ßÎ»(12-15)
	uint16_t valveSubB;
}RalayVData_T, *P_RalayVData;

P_RalayVData RV_getDAddr(void);
void RV_setRelay(RELAY_ENUM relay, DONE_ENUM isDone);

#endif

