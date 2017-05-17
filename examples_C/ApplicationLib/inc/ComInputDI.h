#ifndef __COMINPUTDI_H
#define __COMINPUTDI_H

#include "ComInput.h"

typedef enum{
	DI_LOW,
	DI_HIGH,
	DI_WATER,
	DI_MAX_ENUM
}DI_ENUM;

void ComInputDI_create(void);
void ComInputDI_destory(void);
void ComInputDI_process(void);
//Ӳ����ȡ��ֵ,����Ӳ��ת����ɡ�
void ComInputDI_hardFun(void);
//��ȡ����ֵ��ƽ����
void ComInputDI_aveFun(void);
//��ȡӲ��ת����־����
uint8_t ComInputDI_getHardFlagFun(void);
//���Ӳ��ת����־����
void ComInputDI_clearHardFlagFun(void);
//��ȡ��Ӧ��������ֵ
uint16_t ComInputDI_getDIValue(uint8_t adc);
//����ת����������ȡ��Ӧ����ʱ�洢��ֵַ
uint16_t* ComInputDI_getTempDataAddr(void);
//��ȡ����ת��������ַ
P_ComInput ComInputDI_getBaseAddr(void);
//���ڲ�������ֵ
void ComInputDI_setDIData(void);

void ComInputDI_setHardFlagFun(void);

#endif
