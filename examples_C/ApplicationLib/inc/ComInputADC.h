#ifndef __COMINPUTADC_H
#define __COMINPUTADC_H

#include "ComInput.h"

typedef enum{
	ADCIN0,
	ADCIN1,
	ADCIN2,
	ADCIN3,
	ADCIN4,
	ADCIN5,
	ADCIN6,
	ADCIN7,
	ADCIN8,
	ADCIN9,
//	ADCIN10,
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
#endif

