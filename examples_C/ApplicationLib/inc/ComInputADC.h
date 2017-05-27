#ifndef __COMINPUTADC_H
#define __COMINPUTADC_H

#include "ComInput.h"

#define AirOutTemperMax100 1000		//110¶È

typedef enum{
	ADCIN0_AIN,		//comp in T
	ADCIN1_MEVA,	//middle evaporate
	ADCIN2,
	ADCIN3,
	ADCIN4,
	ADCIN5,
	ADCIN6,
	ADCIN7,
	ADCIN8,
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
int16_t ADC_getSuperHeat(void);

void ADC_setRealData(ADC_ENUM adcIn, int16_t data);
#endif

