#include "ComInputADCSpy.h"

P_ComInput _comInputAdc;

static uint8_t getCount(void)
{
	return _comInputAdc->count;
}

static void initComInputAdc(void)
{
	_comInputAdc->maxRunTime = 10;
}

void ComInputADC_setData(void)
{
	uint16_t *addr;
	uint8_t i,count;
	addr = ComInputADC_getTempDataAddr();
	count = getCount();
	count <<= 1;
	for (i = 0; i < ADC_MAX_ENUM; i++)
	{
		*(addr + i) = count + i;
	}

}

void ComInputADCSpy_crate(void)
{
	_comInputAdc = ComInputADC_getBaseAddr();
	initComInputAdc();
}

