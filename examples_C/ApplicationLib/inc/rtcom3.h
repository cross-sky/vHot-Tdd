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
	uint8_t p0Sethz;	//�趨Ƶ��
	uint8_t p1RunHz;	//����Ƶ��
	uint8_t p2State;	//�豸����״̬
	uint8_t p3IH;		//ѹ����������Чֵ������λ
	uint8_t p4IL;		//ѹ����������ЧֵС����λ
	uint8_t p5UH;		//ĸ�ߵ�ѹ������λ
	uint8_t p6UL;		//ĸ�ߵ�ѹС����λ
	uint8_t p7Err1;		//����1��0������1���ϣ�
	uint8_t p8Err2;		//
	uint8_t p9Err3;		//
	uint8_t p10Temper;	// IPM�¶ȣ�value = real + 100
	uint8_t p11compVer;	// ѹ��������
	uint8_t p12SoftVer;	// ��������汾��
	uint8_t p13Iuu;		// U�����ֵ
	uint8_t p14Ivv;		// V�����ֵ
	uint8_t p15Err4;	//	
	uint8_t p16PFCT;	//	PFCģ���¶�
	uint8_t p17U220;	//	������Դ��Чֵ�� value = real / 2
	uint8_t p18I220H;	//	����������Чֵ������λ
	uint8_t p19I220L;	//  ����������ЧֵС����λ
	uint8_t p20KWH;		//	���빦��������λ *10
	uint8_t p21KWL;		//	���빦��С����λ	*10
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

