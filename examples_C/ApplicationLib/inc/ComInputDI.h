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
//硬件读取数值,设置硬件转换完成。
void ComInputDI_hardFun(void);
//读取到的值求平均数
void ComInputDI_aveFun(void);
//获取硬件转换标志函数
uint8_t ComInputDI_getHardFlagFun(void);
//清除硬件转换标志函数
void ComInputDI_clearHardFlagFun(void);
//获取相应数字输入值
uint16_t ComInputDI_getDIValue(uint8_t adc);
//根据转换次数，获取相应的临时存储地址值
uint16_t* ComInputDI_getTempDataAddr(void);
//获取基础转换参数地址
P_ComInput ComInputDI_getBaseAddr(void);
//用于测试设置值
void ComInputDI_setDIData(void);

void ComInputDI_setHardFlagFun(void);

#endif
