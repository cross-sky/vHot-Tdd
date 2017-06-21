#include "comm.h"

typedef struct _driverRec{
	uint8_t setHz;
	uint8_t runHz;
	uint8_t status;
	int8_t	ipmTemp;
	uint8_t err[4];
	uint16_t dcI;
	uint16_t dcU;
	int8_t fpcTemp;
	uint8_t versoft;
	uint8_t verComp;
	uint16_t acU;
	uint16_t acI;
	uint16_t power;
}DriverRecData_T;

typedef struct _DriverData{
	RTCom3Data1Value txValue;
	DriverRecData_T rxValue;
}DriverData_T;

static DriverData_T S_driverData;
static DriverData_T* P_driverData = &S_driverData;

typedef struct _MData{
	uint16_t* realADC;
	
	RalayVData_T* rvData;
	DriverData_T* driverData;
}MData_T;

static void initDriverData(void)
{
	S_driverData.txValue.p1Set = 0;
	S_driverData.txValue.p2run = 0;
	S_driverData.txValue.p3Code = 0;
	S_driverData.txValue.p4Upspeed = 2;
	S_driverData.txValue.p5DownSpeed = 2;
	S_driverData.txValue.p6Null = 0;
}

void MainData_txSetHz(uint8_t hz)
{
	if (hz > 90)
	{
		return;
	}
	S_driverData.txValue.p1Set = hz;
	//@@@@@ÒªÉèÖÃP2
}
uint8_t MainData_txGetHz(void)
{
	return S_driverData.txValue.p1Set;
}

void MainData_txSetCode(uint8_t code)
{
	S_driverData.txValue.p3Code = code & 0x01;
}
uint8_t MainData_txGetCode(void)
{
	return S_driverData.txValue.p3Code;
}

void MainData_txSetUpspeed(uint8_t upspeed)
{
	S_driverData.txValue.p4Upspeed = upspeed;
}
uint8_t MainData_txGetUpspeed(void)
{
	return S_driverData.txValue.p4Upspeed;
}

void MainData_txSetDownspeed(uint8_t downspeed)
{
	S_driverData.txValue.p5DownSpeed = downspeed;
}
uint8_t MainData_txGetDownspeed(void)
{
	return S_driverData.txValue.p5DownSpeed;
}

uint16_t MainData_getPower(void)
{
	return P_driverData->rxValue.power;
}

void MainData_rxConvert(P_RTCom3RFrame1 rec1)
{
	DriverRecData_T* dst = &S_driverData.rxValue;
//	S_driverData.txValue.p2run = rec1->data.p1RunHz;
	dst->runHz = rec1->data.p1RunHz;
	dst->status = rec1->data.p2State;
	dst->dcI = rec1->data.p3IH * 10 + rec1->data.p4IL / 10;
	dst->dcU = (rec1->data.p5UH << 8) + rec1->data.p6UL;
	dst->ipmTemp = rec1->data.p10Temper - 100;
	dst->err[0] = rec1->data.p7Err1;
	dst->err[1] = rec1->data.p8Err2;
	dst->err[2] = rec1->data.p9Err3;
	dst->err[3] = rec1->data.p15Err4;
	dst->fpcTemp = rec1->data.p16PFCT - 100;
	dst->versoft = rec1->data.p12SoftVer;
	dst->verComp = rec1->data.p11compVer;
	dst->acU = rec1->data.p17U220 << 1;
	dst->acI = rec1->data.p18I220H * 10 + rec1->data.p19I220L / 10;
	dst->power = ((rec1->data.p20KWH << 8) + rec1->data.p21KWL) * 10;
}

uint8_t MainData_rxDrGetStatus(void)
{
	return S_driverData.rxValue.status;
}

void MainData_initPara(void)
{
	initDriverData();
}

uint16_t MainData_printDriverMessage(char* dst, uint16_t maxSize)
{
	uint16_t len = 0;

	len += snprintf(dst + len, maxSize - len,"shz-%d,", MainData_txGetHz());
	len += snprintf(dst + len, maxSize - len,"rhz-%d,", P_driverData->rxValue.runHz);
	len += snprintf(dst + len, maxSize - len,"T-%d,", P_driverData->rxValue.ipmTemp);
	len += snprintf(dst + len, maxSize - len,"U-%d,", P_driverData->rxValue.dcU);
	len += snprintf(dst + len, maxSize - len,"I-%d,", P_driverData->rxValue.dcI);
	len += snprintf(dst + len, maxSize - len,"%d,", P_driverData->rxValue.err[0]);
	len += snprintf(dst + len, maxSize - len,"%d,", P_driverData->rxValue.err[1]);
	len += snprintf(dst + len, maxSize - len,"%d,", P_driverData->rxValue.err[2]);
	len += snprintf(dst + len, maxSize - len,"%d,", P_driverData->rxValue.err[3]);
	len += snprintf(dst + len, maxSize - len,"p-%d,", P_driverData->rxValue.power);

	return len;
}

