#include "comm.h"

static RalayVData_T S_rvData={0,0};

#define HC1_DS GPIO_Pin_6		//串行数据输入
#define HC1_OE GPIO_Pin_7		//输出有效（低电平）
#define HC1_STCP GPIO_Pin_8		//输出存储器锁存时钟线
#define HC1_SHCP GPIO_Pin_9		//数据输入时钟线　
#define HC1_PINS GPIO_Pin_9|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8

#define HC1_H(pin) GPIOC->BSRR=pin
#define HC1_L(pin) GPIOC->BRR=pin

#define IOPORT_MAX 1
const IOControl relaysOut[IOPORT_MAX]={
	{HC1_PINS, GPIOC ,RCC_APB2Periph_GPIOC},
	//{0,  VALVE_SUBB_PINS, GPIOB ,RCC_APB2Periph_GPIOB}
};

P_RalayVData RV_getDAddr(void)
{
	return &S_rvData;
}
//继电器bit位置，在中
//b7 b6  b5 b4 b3 b2 b1 b0 a7 a6 a5 a4 a3 a2 a1 a0
//k9 k10 /  v4 v3 v2 v1 /  k2 k3 k4 k5 k6 k7 k8 k1
//15 14  13 12 11 10 9  8  7  6  5  4  3  2  1  0
//r1 - r10
const uint8_t relayIndex[16]={
	0, 7, 6, 5, 4, 3, 2, 1,
	15,14,10,13,13,13, 13,13
};
//电子膨胀阀对应的通电相数 
const uint16_t S_tableValve[VALVE_MAX][8]={
	{0x1000,0x1800,	0x0800,	0x0c00,	0x0400,	0x0600,	0x0200,	0x1200},
	{0x8000,0xc000,	0x4000,	0x6000,	0x2000,	0x3000,	0x1000,	0x9000}
};
//电子膨胀阀当前拍数，0-8
uint8_t S_tableIndex[VALVE_MAX]={0,0};

//电子膨胀阀A的bit位置
#define ValveMainABits 0x1e00

//subB pb12-15
#define ValVeSubBBits 0xf000

void RV_setRelay(RELAY_ENUM relay, DONE_ENUM isDone)
{
	if (relay >= RELAY_MAX)
	{
		return;
	}
	if (DONE == isDone)
	{
		S_rvData.relaysAndValveMainA |= 1 << relayIndex[relay];
	}else{
		S_rvData.relaysAndValveMainA &= (~(1 << relayIndex[relay]));
	}
}

void RV_valveRunDirect(VALVESTATE_ENUM addOrSub, VALVEKINDLE_ENUM valveKind)
{
	switch(valveKind)
	{
	case VALVE_MAINA:
		{
			S_rvData.relaysAndValveMainA = (S_rvData.relaysAndValveMainA & (~ValveMainABits)) | S_tableValve[valveKind][S_tableIndex[valveKind]];
			break;
		}
	case VALVE_SUBB:
		{
			S_rvData.valveSubB = (S_rvData.valveSubB & (~ValVeSubBBits)) | S_tableValve[valveKind][S_tableIndex[valveKind]];
			break;
		}
	default:break;
	}

	assert(addOrSub == VALVE_ADD || addOrSub == VALVE_BACK);
	if (addOrSub == VALVE_ADD)
		S_tableIndex[valveKind]++;
	else
		S_tableIndex[valveKind]--;
	S_tableIndex[valveKind] &= 0x07;
}

static void delay1us(void)
{
	uint8_t i=5;
	i--;
}

static void vHC1DataOut(uint16_t dataOut)
{
	//RelaysAndValveMainA
	uint8_t i;
	HC1_L(HC1_SHCP);
	HC1_L(HC1_STCP);
	for(i=0;i<16;i++)
	{
		if ((dataOut<<i) & 0x8000)
		{
			HC1_H(HC1_DS);
		}
		else
			HC1_L(HC1_DS);

		HC1_L(HC1_SHCP);
		delay1us();

		HC1_H(HC1_SHCP);
		delay1us();
	}
	HC1_H(HC1_STCP);
}

void RV_hwInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	uint8_t name;

	//手动开CLK吧，也就几个
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	for (name=0; name<IOPORT_MAX; name++)
	{
		//如上手动开了，就1个
		//RCC_APB2PeriphClockCmd(controlCd4051[name].clk, ENABLE);
		GPIO_InitStructure.GPIO_Pin = relaysOut[name].pin;
		//这里设置为推挽输出，
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_Init(relaysOut[name].port, &GPIO_InitStructure);
	}

	HC1_L(HC1_OE);
	vHC1DataOut(0);
	//vHC1DataOut(0xffff);
	//上电初始化
}

void RV_Task4OutProcess(void)
{
	static uint16_t preRelayValveA=0,preValveB=0;
	P_RalayVData data= &S_rvData;

	if (preRelayValveA != data->relaysAndValveMainA)
	{
		vHC1DataOut(data->relaysAndValveMainA);
		preRelayValveA = data->relaysAndValveMainA;
	}
}



