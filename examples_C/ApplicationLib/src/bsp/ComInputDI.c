#include "comm.h"

static uint16_t S_tempData[MAX_ADI_CONVERT_COUNT];
static uint16_t S_aveData;
static uint8_t S_hardFlag = DONE;
static ComInput_T S_comInputDI;

void ComInputDI_create(void)
{
	S_comInputDI.count = 0;
	S_comInputDI.maxCount = MAX_ADI_CONVERT_COUNT;
	S_comInputDI.runTime = 0;
	S_comInputDI.maxRunTime = 10;
	S_comInputDI.convertFlag = UNDONE;
	S_comInputDI.waitTime = 0;
	S_comInputDI.maxWaitTime = 3;
	S_comInputDI.hardFun = ComInputDI_hardFun;
	S_comInputDI.aveFun = ComInputDI_aveFun;
	S_comInputDI.hardFlagFun = ComInputDI_getHardFlagFun;
	S_comInputDI.clearHardFlagFun = ComInputDI_clearHardFlagFun;
}
void ComInputDI_destory(void)
{

}
void ComInputDI_process(void)
{
	ComInput_Process(&S_comInputDI);
}

void ComInputDI_hardFun(void)
{
	//S_hardFlag = DONE;
	ComInputDI_setDIData();
}
void ComInputDI_aveFun(void)
{
	uint8_t i, j, count;
	for (j = 0; j < DI_MAX_ENUM; j++)
	{
		count = 0;
		for (i = 0; i < MAX_ADI_CONVERT_COUNT; i++)
		{
			count += (S_tempData[i] >> j) & 0x01;
		}
		//有4次闭合，确认正常
		S_aveData |= (count >> 2) << j;
	}
	
}
uint8_t ComInputDI_getHardFlagFun(void)
{
	return S_hardFlag;
}
void ComInputDI_clearHardFlagFun(void)
{
	S_hardFlag = UNDONE;
}

void ComInputDI_setHardFlagFun(void)
{
	S_hardFlag = DONE;
}

uint16_t* ComInputDI_getTempDataAddr(void)
{
	uint8_t count;
	count = S_comInputDI.count;
	return &S_tempData[count];
}

P_ComInput ComInputDI_getBaseAddr(void)
{
	return &S_comInputDI;
}

uint16_t ComInputDI_getDIValue(uint8_t diNumber)
{
	return (S_aveData >> diNumber) & 0x01;
}

void ComInputDI_setDIData(void)
{

}