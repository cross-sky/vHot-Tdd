#include "ComInputDISpy.h"

P_ComInput _comInputDI;

static uint8_t getCount(void)
{
	return _comInputDI->count;
}

static void initComInputDI(void)
{
	_comInputDI->maxRunTime = 10;
}

void ComInputDI_setDIData(void)
{
	uint16_t *addr;
	addr = ComInputDI_getTempDataAddr();
	*addr = 0x07;
}

void ComInputDISpy_crate(void)
{
	_comInputDI = ComInputDI_getBaseAddr();
	initComInputDI();
}

