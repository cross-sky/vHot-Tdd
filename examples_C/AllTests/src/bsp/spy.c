#include "comm.h"

static uint8_t* addr;

uint8_t* SPY_getDstAddress(void)
{
	return addr;
}

void vuart2DmaTxDataEnable(uint16_t len, uint8_t *address)
{
	printf((char*)address);
}

void vuart3DmaTxDataEnable(uint16_t len, uint8_t *address)
{
	addr = address;
}


