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

