#ifndef __RTCOM3_H
#define __RTCOM3_H

#include "rtdata.h"

typedef enum{
	RTCOM3,
}RTCOM;

#define RS485_UART3				USART3
#define RS485_GPIO				GPIOB
#define RS485_UART3_CLK			RCC_APB1Periph_USART3
#define RS485_GPIO_CLK			RCC_APB2Periph_GPIOB

#define RS485_RxPin				GPIO_Pin_11
#define RS485_TxPin				GPIO_Pin_10	//GPIOB
#define RS485_RePin				GPIO_Pin_8	//GPIOA

//����һ��˫��Ĵ�����������TDR��RDR����������������ȡ����RDR�Ĵ�����ֵ��������д������ʵ������д��TDR�Ĵ�����
#define USART3_Tx_DR_Base		0x40004804
#define DMA1_Ch_Usart3_Tx		DMA1_Channel2
#define DMA1_Ch_Usart3_Tx_IRQn	DMA1_Channel2_IRQn

#define USART3_Rx_DR_Base		0x40004804
#define DMA1_Ch_Usart3_Rx		DMA1_Channel3
#define DMA1_Ch_Usart3_Rx_IRQn	DMA1_Channel3_IRQn


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
	uint8_t len;
	uint8_t funCode;
}RTCom3_Data1Header;

typedef struct _RTCom3Send1{
	RTCom3_Data1Header header;
	RTCom3Data1Value data;
	uint8_t crr;
}RTCom3SFrame1_T, *P_RTCom3SFrame1;

typedef struct {
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
}RTCom3RFrame1_T, *P_RTCom3RFrame1;

void RTCom3_createEvent(void);
bool RTCom3_popEvent(void* dataAddr);
bool RTCom3_pushEvent(void* dataAddr, uint16_t dataLen);
uint16_t RTCom3_lengthEvent(void);
void RTCom3_destoryEvent(void);

void RTCom3_SendCM1(void);

void RTCom3_SendProcess(P_RTRCFlag rcFlag);
void RTCom3_initRCflag(P_RTRCFlag rcFlag);

uint8_t RTCom3_checkXrr(void* addr, uint8_t len);
void RTCom3_setRecFlag(void);

void UART3_Init(void);
void Uart3_DmaTxHandler_ISR(void);
void Uart3_DmaRxHandle_ISR(void);
void Usart3_IdlHandle_ISR(void);
void vuart3DmaTxDataEnable(uint16_t len, uint8_t *address);

bool RTCom3_recProcess(void);
uint8_t RTCom3_getCount(void);
void UART3_RTProcess(void);

uint8_t* RTCom3_getRS485Data(void);

#endif

