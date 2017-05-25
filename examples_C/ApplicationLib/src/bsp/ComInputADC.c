#include "comm.h"
#include "adc_filter.h"

#define ADC1_DR_Address    ((uint32_t)0x4001244C)
#define ADCx_DMA1_IRQn     DMA1_Channel1_IRQn


#define  NTCPINCs GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3
#define  NTCPINAs GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7
//1.头文件设置adc 2.adc数组初始化 3.adc规则组设置 4.转换成实际数据

//channel 序号以硬件ntc00-09所使用的引脚为准
uint8_t tabChannel[ADCIN_MAX_ENUM]={
	ADC_Channel_10,ADC_Channel_11,ADC_Channel_12,ADC_Channel_13,
	ADC_Channel_0, ADC_Channel_1, ADC_Channel_4, ADC_Channel_5,
	ADC_Channel_6, ADC_Channel_7
};

//-----------------
static uint16_t S_tempData[MAX_ADI_CONVERT_COUNT][ADCIN_MAX_ENUM];

static uint16_t S_aveData[ADCIN_MAX_ENUM];
static uint16_t S_relData[ADCIN_MAX_ENUM];

static uint8_t S_hardFlag = DONE;

static ComInput_T S_comInputAdc;

void ComInputADC_setData(void)
{
	
}

static void convertADCData(void)
{
	uint8_t i;
	uint16_t searchValue,index;
	for (i = 0; i < ADCIN_MAX_ENUM; i++)
	{
		searchValue = S_aveData[i];
		index=uADCSearchData10K(searchValue);
		S_relData[i] = iADCTemperCalc10K(index,searchValue);
	}
}

void ComInputADC_hardFun(void)
{
	ComInputADC_setData();
	//isr set hard flag = done
	ComInputADC_startSimpling();
}

void ComInputADC_aveFun(void)
{
	uint8_t i, j;
	uint16_t values;
	for (i = 0; i < ADCIN_MAX_ENUM ; i++)
	{
		values = 0;
		for (j = 0; j < MAX_ADI_CONVERT_COUNT; j ++)
		{
			values += S_tempData[j][i];
		}
		S_aveData[i] = values / MAX_ADI_CONVERT_COUNT;
	}

	//convert to real adc value
	convertADCData();
}

uint16_t ComInputADC_printAdc(char* dst, uint16_t maxSize)
{
	uint16_t len= 0;
	uint8_t i;
	for (i = 0; i < ADCIN_MAX_ENUM; i++)
	{
		len += snprintf(dst + len, maxSize - len,"%d,", S_relData[i]);
	}
	len += snprintf(dst + len, maxSize - len,"VA-%d,", Valve_getTotalSteps(VALVE_TYPE_MAINA));
	len += snprintf(dst + len, maxSize - len,"VB-%d,", Valve_getTotalSteps(VALVE_TYPE_SUBB));
	//
	len += snprintf(dst + len, maxSize - len,"DR-%d,", RTCom3_getCount());
	len  += snprintf(dst + len, maxSize - len, "\r\n");
	return len;
}


uint8_t ComInputADC_getHardFlagFun(void)
{
	return S_hardFlag;
}
void ComInputADC_clearHardFlagFun(void)
{
	S_hardFlag = UNDONE;
}

void ComInputADC_setHardFlagFun(void)
{
	S_hardFlag = DONE;
}

void ComInputADC_create(void)
{
	S_comInputAdc.count = 0;
	S_comInputAdc.maxCount = MAX_ADI_CONVERT_COUNT;
	S_comInputAdc.runTime = 0;
	S_comInputAdc.maxRunTime = 10;
	S_comInputAdc.convertFlag = UNDONE;
	S_comInputAdc.waitTime = 0;
	S_comInputAdc.maxWaitTime = 3;
	S_comInputAdc.hardFun = ComInputADC_hardFun;
	S_comInputAdc.aveFun = ComInputADC_aveFun;
	S_comInputAdc.hardFlagFun = ComInputADC_getHardFlagFun;
	S_comInputAdc.clearHardFlagFun = ComInputADC_clearHardFlagFun;
}
void ComInputADC_destory(void)
{

}
void ComInputADC_process(void)
{
	ComInput_Process(&S_comInputAdc);
}

uint16_t* ComInputADC_getTempDataAddr(void)
{
	uint8_t count;
	count = S_comInputAdc.count;
	return &S_tempData[count][0];
}

P_ComInput ComInputADC_getBaseAddr(void)
{
	return &S_comInputAdc;
}

uint16_t ComInputADC_getADCValue(uint8_t adc)
{
	return S_aveData[adc];
}

void ComInputADC_ADCDmaChannel1_ISR(void)
{
	if (DMA_GetITStatus(DMA1_IT_TC1) != RESET)
	{
		DMA_ClearITPendingBit(DMA1_IT_GL1);
		DMA_ClearITPendingBit(DMA1_IT_TC1);//清除全局中断
		ADC_SoftwareStartConvCmd(ADC1, DISABLE);
		ComInputADC_setHardFlagFun();
	}
}

uint16_t* uINPGetADCaddress(void)
{
	return &S_tempData[S_comInputAdc.count][0];
}

void ComInputADC_startSimpling(void)
{
	//1.重新设置规则组?；2.清除转换完成；3.dma设置；4.adc软启动
	DMA_Cmd(DMA1_Channel1, DISABLE);
	DMA1_Channel1->CMAR=(uint32_t)uINPGetADCaddress();
	DMA1_Channel1->CNDTR =ADCIN_MAX_ENUM;	//这里大意了
	DMA_Cmd(DMA1_Channel1, ENABLE);
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}


static void adcHWInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	uint8_t name;
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_ClearFlag(DMA1_FLAG_TC1);
	NVIC_InitStructure.NVIC_IRQChannel=ADCx_DMA1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=10;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//手动开CLK吧，也就2个,注意开ADC的时钟树
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1
		, ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div4);//<14MHz
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);

	//按定义的adcINput定义的数量为准(一般adc使用GPIOA和GPIOC)

	GPIO_InitStructure.GPIO_Pin = NTCPINAs;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = NTCPINCs;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* DMA1 channel1 configuration ----------------------------------------------*/
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)uINPGetADCaddress();
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = ADCIN_MAX_ENUM;	//
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	//
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;	//
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);

	//使能DMA传输完成中断
	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC,ENABLE);

	/* ADC1 configuration ------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = ADCIN_MAX_ENUM;
	ADC_Init(ADC1, &ADC_InitStructure);

	//cdmax-1 个外部adc，1个内部adc
	for (name= 0; name < ADCIN_MAX_ENUM; name++)
	{
		//  /* ADC1 regular channel14 configuration */ 
		//ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_55Cycles5);
		//1. rank start at 1
		ADC_RegularChannelConfig(ADC1, tabChannel[name], name+1, ADC_SampleTime_239Cycles5);
	}

	//need to enable
	ADC_TempSensorVrefintCmd(ENABLE);
	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);

	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);

	/* Enable ADC1 reset calibration register */   
	ADC_ResetCalibration(ADC1);
	/* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC1));

	/* Start ADC1 calibration */
	ADC_StartCalibration(ADC1);
	/* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADC1));

	/* Enable DMA1 channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);

	/* Start ADC1 Software Conversion */ 
	//ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void ComInputADC_Init(void)
{
	ComInputADC_create();
	adcHWInit();
}
