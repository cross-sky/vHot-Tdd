#ifndef __COMINPUT_H
#define __COMINPUT_H

#include "commdata.h"

#define MAX_ADI_CONVERT_COUNT 5

typedef struct _ComInputStruct{
	uint8_t count;
	uint8_t maxCount;
	uint8_t runTime;
	uint8_t maxRunTime;
	uint8_t convertFlag;
	uint8_t waitTime;
	uint8_t maxWaitTime;
	pvNormalFun hardFun;
	pvNormalFun aveFun;
	pu8NormalFun hardFlagFun;
	pvNormalFun clearHardFlagFun;
}ComInput_T, *P_ComInput;

void ComInput_Create(void);
void ComInput_Destory(P_ComInput comInput);
void ComInput_Process(P_ComInput comInput);

#endif

