#ifndef __COMINPUTADC_H
#define __COMINPUTADC_H
#include "ComInput.h"

typedef enum{
	ADC0,
	ADC1,
	ADC2,
	ADC3,
	ADC4,
	ADC5,
	ADC6,
	ADC7,
	ADC8,
	ADC9,
	ADC10,
	ADC_MAX_ENUM
};

void ComInputADC_create(void);
void ComInputADC_destory(void);
void ComInputADC_process(void);

void ComInputADC_hardFun(void);
void ComInputADC_aveFun(void);
uint8_t ComInputADC_getHardFlagFun(void);
void ComInputADC_clearHardFlagFun(void);

uint16_t ComInputADC_getADCValue(uint8_t adc);
uint16_t* ComInputADC_getTempDataAddr(void);

P_ComInput ComInputADC_getBaseAddr(void);

void ComInputADC_setData(void);

#endif

