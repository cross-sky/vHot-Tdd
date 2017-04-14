#ifndef __RTCOM3_H
#define __RTCOM3_H

#include "rtdata.h"

typedef enum{
	RTCOM3,
}RTCOM;


typedef struct _RTHeader{
	Event_T event;
	uint16_t len;
}RTHeader_T, *P_RTHeader;

typedef struct _RTCom3DMA{
	uint8_t *addr;
	uint16_t len;
}RTCom3DMA_T,*P_RTCom3DMA;

typedef struct _RTCom3Data1Value{
	uint8_t p1Set;
	uint8_t p2run;
	uint8_t p3Code;
	uint8_t p4Upspeed;
	uint8_t p5DownSpeed;
	uint8_t p6Null;
}RTCom3Data1Value;

typedef struct _RTCom3_Data1Header{
	uint8_t header1;
	uint8_t header2;
	uint8_t dstAddr;
	uint8_t srcAddr;
	uint8_t frameCode;
	uint8_t funCode;
	uint8_t len;
}RTCom3_Data1Header;

typedef struct _RTCom3Send1{
	RTCom3_Data1Header header;
	RTCom3Data1Value data;
	uint8_t crr;
}RTCom3SFrame1_T, *P_RTCom3SFrame1;

typedef struct _RTCom3Recv1{
	uint8_t p0Sethz;	//设定频率
	uint8_t p1RunHz;	//运行频率
	uint8_t p2State;	//设备运行状态
	uint8_t p3IH;		//压缩机电流有效值整数高位
	uint8_t p4IL;		//压缩机电流有效值小数低位
	uint8_t p5UH;		//母线电压整数高位
	uint8_t p6UL;		//母线电压小数低位
	uint8_t p7Err1;		//错误1，0正常，1故障，
	uint8_t p8Err2;		//
	uint8_t p9Err3;		//
	uint8_t p10Temper;	// IPM温度，value = real + 100
	uint8_t p11compVer;	// 压缩机代号
	uint8_t p12SoftVer;	// 驱动软件版本号
	uint8_t p13Iuu;		// U相电流值
	uint8_t p14Ivv;		// V相电流值
	uint8_t p15Err4;	//	
	uint8_t p16PFCT;	//	PFC模块温度
	uint8_t p17U220;	//	交流电源有效值， value = real / 2
	uint8_t p18I220H;	//	交流电流有效值整数高位
	uint8_t p19I220L;	//  交流电流有效值小数低位
	uint8_t p20KWH;		//	输入功率整数高位 *10
	uint8_t p21KWL;		//	输入功率小数地位	*10
}RTCom3_RData1Value_T, *P_RTCom3_RData1Value;

typedef struct _RTCom3Rec1{
	RTCom3_Data1Header header;
	RTCom3_RData1Value_T data;
	uint8_t crr;
}RTCom3RFrame1_R, *P_RTCom3RFrame1;

void RTCom3_createEvent(void);
bool RTCom3_popEvent(void* dataAddr);
bool RTCom3_pushEvent(void* dataAddr, uint16_t dataLen);
uint16_t RTCom3_lengthEvent(void);
void RTCom3_destoryEvent(void);

void RTCom3_SendCM1(uint8_t vHz);

void RTCom3_SendProcess(P_RTRCFlag rcFlag);
void RTCom3_initRCflag(P_RTRCFlag rcFlag);



#endif

