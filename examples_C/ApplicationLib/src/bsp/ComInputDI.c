#include "comm.h"

#define DIO_MAX 1
#define DIPins GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8
#define DIGPIO	GPIOB
#define DIIO_SHIFT 6

static uint16_t S_tempData[MAX_ADI_CONVERT_COUNT];
static uint16_t S_aveData;
static uint8_t S_hardFlag = DONE;
static ComInput_T S_comInputDI;

const IOControl Di_IoPin[DIO_MAX]={
	{DIPins, GPIOB ,RCC_APB2Periph_GPIOB},
};

void ComInputDI_create(void)
{
	S_comInputDI.count = 0;
	S_comInputDI.maxCount = MAX_ADI_CONVERT_COUNT;
	S_comInputDI.runTime = 0;
	S_comInputDI.maxRunTime = 10;
	S_comInputDI.convertFlag = UNDONE;
	S_comInputDI.waitTime = 0;
	S_comInputDI.maxWaitTime = 3;
	S_comInputDI.hardFun = ComInputDI_hardFun;
	S_comInputDI.aveFun = ComInputDI_aveFun;
	S_comInputDI.hardFlagFun = ComInputDI_getHardFlagFun;
	S_comInputDI.clearHardFlagFun = ComInputDI_clearHardFlagFun;
}
void ComInputDI_destory(void)
{

}
void ComInputDI_process(void)
{
	ComInput_Process(&S_comInputDI);
}

void ComInputDI_hardFun(void)
{
	//S_hardFlag = DONE;
	ComInputDI_setDIData();
}
void ComInputDI_aveFun(void)
{
	uint8_t i, j, count;
	uint16_t tdata = 0;
	for (j = 0; j < DI_MAX_ENUM; j++)
	{
		count = 0;
		for (i = 0; i < MAX_ADI_CONVERT_COUNT; i++)
		{
			count += (S_tempData[i] >> j) & 0x01;
		}
		//有4次闭合，确认正常
		tdata |= (count >> 2) << j;
	}
	S_aveData = tdata;
}
uint8_t ComInputDI_getHardFlagFun(void)
{
	return S_hardFlag;
}
void ComInputDI_clearHardFlagFun(void)
{
	S_hardFlag = UNDONE;
}

void ComInputDI_setHardFlagFun(void)
{
	S_hardFlag = DONE;
}

uint16_t* ComInputDI_getTempDataAddr(void)
{
	uint8_t count;
	count = S_comInputDI.count;
	return &S_tempData[count];
}

P_ComInput ComInputDI_getBaseAddr(void)
{
	return &S_comInputDI;
}

uint16_t ComInputDI_getDIValue(uint8_t diNumber)
{
	return (S_aveData >> diNumber) & 0x01;
}

void ComInputDI_setDIData(void)
{
	uint16_t *addr;
	S_hardFlag = DONE;
	addr = ComInputDI_getTempDataAddr();
	*addr = (DIGPIO->IDR >> DIIO_SHIFT) & 0x07;
}


void ComInputDI_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	uint8_t name;

	//手动开CLK吧，也就几个
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	for (name=0; name<DIO_MAX; name++)
	{
		GPIO_InitStructure.GPIO_Pin = Di_IoPin[name].pin;
		//这里设置为推挽输出，GPIO_Mode_IN_FLOATING
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(Di_IoPin[name].port, &GPIO_InitStructure);
	}

	ComInputDI_create();
}

uint16_t ComInputDI_getAveData(void)
{
	return S_aveData;
}

void ComInputADC_DIProcess(void)
{
	ComInputADC_process();
	ComInputDI_process();
}

