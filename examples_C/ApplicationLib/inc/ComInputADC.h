#ifndef __COMINPUTADC_H
#define __COMINPUTADC_H

#include "ComInput.h"

#define AirOutTemperMax100 1000		//110��

//1-������ 2-������3-���£�4-��ˮ��5-��ˮ��6-�廻������7-�������룬8-����������9-�̹��룬10-�̹ܳ�
//11-������ PT1-����ѹ����PT2-����ѹ��
typedef enum{
	ADCIN0_AIN,		//comp in T
	ADCIN1_MEVA,	//middle evaporate
	ADCIN2_ENV,
	ADCIN3_WOUT,
	ADCIN4_WIN,
	ADCIN5,
	ADCIN6,
	ADCIN7_ECONIN,
	ADCIN8_ECONOUT,
	ADCIN9,
	ADCIN11_AOUT, //pc5
	ADCPT1_L,	//pb0
	ADCPT2_H,	//pb1
	ADCIN_MAX_ENUM
}ADC_ENUM;

void ComInputADC_create(void);
void ComInputADC_destory(void);
void ComInputADC_process(void);

void ComInputADC_hardFun(void);
void ComInputADC_aveFun(void);
uint8_t ComInputADC_getHardFlagFun(void);
void ComInputADC_clearHardFlagFun(void);
void ComInputADC_startSimpling(void);

uint16_t ComInputADC_getADCValue(uint8_t adc);
uint16_t* ComInputADC_getTempDataAddr(void);

P_ComInput ComInputADC_getBaseAddr(void);

uint16_t ComInputADC_printAdc(char* dst, uint16_t maxSize);
void ComInputADC_setHardFlagFun(void);
void ComInputADC_Init(void);
void ComInputADC_ADCDmaChannel1_ISR(void);

int16_t ADC_getAOut(void);
int16_t ADC_getAIN(void);
int16_t ADC_getMEva(void);
int16_t ADC_getWIN(void);
int16_t ADC_getWOUT(void);
int16_t ADC_getSuperHeat(void);
void ADC_setSuperHeat(int16_t value);
int16_t ADC_getAINSaturation(void);

void ADC_setRealData(ADC_ENUM adcIn, int16_t data);
#endif

