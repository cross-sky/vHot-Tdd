#include "comm.h"

static RalayVData_T S_rvData={0,0};

P_RalayVData RV_getDAddr(void)
{
	return &S_rvData;
}
//继电器bit位置，在中
//b7 b6  b5 b4 b3 b2 b1 b0 a7 a6 a5 a4 a3 a2 a1 a0
//k9 k10 /  v4 v3 v2 v1 /  k2 k3 k4 k5 k6 k7 k8 k1
//15 14  13 12 11 10 9  8  7  6  5  4  3  2  1  0
//r1 - r10
const uint8_t relayIndex[16]={
	0, 7, 6, 5, 4, 3, 2, 1,
	15,14,10,13,13,13, 13,13
};
//电子膨胀阀对应的通电相数 
const uint16_t S_tableValve[VALVE_MAX][8]={
	{0x1000,0x1800,	0x0800,	0x0c00,	0x0400,	0x0600,	0x0200,	0x1200},
	{0x8000,0xc000,	0x4000,	0x6000,	0x2000,	0x3000,	0x1000,	0x9000}
};
//电子膨胀阀当前拍数，0-8
uint8_t S_tableIndex[VALVE_MAX]={0,0};

//电子膨胀阀A的bit位置
#define ValveMainABits 0x1e00

//subB pb12-15
#define ValVeSubBBits 0xf000

void RV_setRelay(RELAY_ENUM relay, DONE_ENUM isDone)
{
	if (relay >= RELAY_MAX)
	{
		return;
	}
	if (DONE == isDone)
	{
		S_rvData.relaysAndValveMainA |= 1 << relayIndex[relay];
	}else{
		S_rvData.relaysAndValveMainA &= 0 << relayIndex[relay];
	}
}

void RV_valveRunDirect(VALVESTATE_ENUM addOrSub, VALVEKINDLE_ENUM valveKind)
{
	switch(valveKind)
	{
	case VALVE_MAINA:
		{
			S_rvData.relaysAndValveMainA = (S_rvData.relaysAndValveMainA & (~ValveMainABits)) | S_tableValve[valveKind][S_tableIndex[valveKind]];
			break;
		}
	case VALVE_SUBB:
		{
			S_rvData.valveSubB = (S_rvData.valveSubB & (~ValVeSubBBits)) | S_tableValve[valveKind][S_tableIndex[valveKind]];
			break;
		}
	default:break;
	}

	assert(addOrSub == VALVE_ADD || addOrSub == VALVE_BACK);
	if (addOrSub == VALVE_ADD)
		S_tableIndex[valveKind]++;
	else
		S_tableIndex[valveKind]--;
	S_tableIndex[valveKind] &= 0x07;
}
