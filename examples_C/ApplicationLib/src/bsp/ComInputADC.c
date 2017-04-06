#include "comm.h"

static uint16_t S_tempData[MAX_ADC_CONVERT_COUNT][ADC_MAX_ENUM];

static uint16_t S_aveData[ADC_MAX_ENUM];

static uint8_t S_hardFlag = DONE;

static ComInput_T S_comInputAdc;

void ComInputADC_setData(void)
{

}

void ComInputADC_hardFun(void)
{
	S_hardFlag = DONE;
	ComInputADC_setData();
	//dma set hard flag = done
}

void ComInputADC_aveFun(void)
{
	uint8_t i, j;
	uint16_t values;
	for (i = 0; i < ADC_MAX_ENUM ; i++)
	{
		values = 0;
		for (j = 0; j < MAX_ADC_CONVERT_COUNT; j ++)
		{
			values += S_tempData[j][i];
		}
		S_aveData[i] = values / MAX_ADC_CONVERT_COUNT;
	}
}
uint8_t ComInputADC_hardFlagFun(void)
{
	return S_hardFlag;
}
void ComInputADC_clearHardFlagFun(void)
{
	S_hardFlag = UNDONE;
}

void ComInputADC_create(void)
{
	S_comInputAdc.count = 0;
	S_comInputAdc.maxCount = MAX_ADC_CONVERT_COUNT;
	S_comInputAdc.runTime = 0;
	S_comInputAdc.maxRunTime = 10;
	S_comInputAdc.convertFlag = UNDONE;
	S_comInputAdc.waitTime = 0;
	S_comInputAdc.maxWaitTime = 3;
	S_comInputAdc.hardFun = ComInputADC_hardFun;
	S_comInputAdc.aveFun = ComInputADC_aveFun;
	S_comInputAdc.hardFlagFun = ComInputADC_hardFlagFun;
	S_comInputAdc.clearHardFlagFun = ComInputADC_clearHardFlagFun;
}
void ComInputADC_destory(void)
{

}
void ComInputADC_process(void)
{
	ComInput_Process(&S_comInputAdc);
}

uint16_t* ComInputADC_getDmaCountAddr(void)
{
	uint8_t count;
	count = S_comInputAdc.count;
	return &S_tempData[count][0];
}

P_ComInput ComInputADC_getbaseAddr(void)
{
	return &S_comInputAdc;
}

uint16_t ComInputADC_getADCValue(uint8_t adc)
{
	return S_aveData[adc];
}
