#include "comm.h"

#define  Tx2BUF_MAX 300
#define Rx2BUF_MAX 15

static uint8_t TxBuff[Tx2BUF_MAX]={0};
static uint8_t RxBuff[Rx2BUF_MAX]={0};

#define Bounds 9600

typedef enum{
	BIT_FUNCODE = 2,
	BIT_FUNVALUE,
}BIT_ENUMS;

typedef enum{
	REC_CODE_ADC = 0,
	REC_CODE_RELAY,
	REC_CODE_VALVE,
	REC_CODE_RETURN,
	REC_CODE_MAX
}REC_CODE_ENUM;

//创建全局队列
static Queue_T S_recQue;
static uint8_t S_recData[QUEUELENGTH][Rx2BUF_MAX];

static uint8_t S_trec[Rx2BUF_MAX];
static uint8_t S_txcod4[8];

static uint8_t S_setPrintADCFlag=0;
static void recCode1(uint8_t value);
static void recCode4(uint8_t value);

static pvNormalFunU8 S_tabHandle[REC_CODE_MAX]={
	recCode1,recCode4,recCode4,recCode4,
};

static uint8_t* puartGetRTxAddress(void)
{
	return RxBuff;
}

void RTCom2_paraInit(void)
{
	S_setPrintADCFlag = 0;
	RTCom2Rec_createEvent();
}

void UART2_Init(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	DMA_InitTypeDef		DMA_InitStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;
	USART_InitTypeDef	USART_InitStructure;

	//必须让uart1初始化在前，然后才能初始化uart2
	//否则开APB2-AFIO，PORT，APB1-uart，AHB-DMA
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|USART2_Tx_GPIO_CLK,ENABLE);
	RCC_APB1PeriphClockCmd(USART2_Tx_CLK,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);

	// Configure USART_Rx as input floating 
	GPIO_InitStructure.GPIO_Pin = USART2_RxPin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART2_Rx_GPIO,&GPIO_InitStructure);

	// Configure USART_Tx as alternate function push-pull 
	GPIO_InitStructure.GPIO_Pin = USART2_TxPin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(USART2_Tx_GPIO,&GPIO_InitStructure);

	DMA_DeInit(DMA1_Ch_Usart2_Tx);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)USART2_Tx_DR_Base;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&TxBuff;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = Tx2BUF_MAX;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

	DMA_Init(DMA1_Ch_Usart2_Tx,&DMA_InitStructure);
	DMA_ITConfig(DMA1_Ch_Usart2_Tx,DMA_IT_TC,ENABLE);
	DMA_ITConfig(DMA1_Ch_Usart2_Tx,DMA_IT_TE,ENABLE);
	USART_DMACmd(USART2_Tx, USART_DMAReq_Tx, ENABLE);
	DMA_Cmd(DMA1_Ch_Usart2_Tx,DISABLE);

	DMA_DeInit(DMA1_Ch_Usart2_Rx);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)USART2_Rx_DR_Base;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)puartGetRTxAddress();
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = Rx2BUF_MAX;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;

	DMA_Init(DMA1_Ch_Usart2_Rx,&DMA_InitStructure);
	DMA_ITConfig(DMA1_Ch_Usart2_Rx,DMA_IT_TC,ENABLE);//transmit complete
	DMA_ITConfig(DMA1_Ch_Usart2_Rx,DMA_IT_TE,ENABLE);//transmit enable
	USART_DMACmd(USART2_Rx, USART_DMAReq_Rx, ENABLE);
	DMA_Cmd(DMA1_Ch_Usart2_Rx,ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//Enable DMA Channel4 Interrupt 
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Ch_Usart2_Tx_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//Enable DMA Channel5 Interrupt 
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Ch_Usart2_Rx_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_InitStructure.USART_BaudRate = Bounds;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2_Tx, &USART_InitStructure);

	//空闲中断
	USART_ITConfig(USART2_Tx, USART_IT_IDLE , ENABLE);//
	USART_Cmd(USART2_Tx, ENABLE);
	USART_ClearFlag(USART2_Tx, USART_FLAG_TC);     //* 清发送外城标志，

	RTCom2_paraInit();
}

void vUart2DmaTxHandler_ISR(void)
{
	DMA_ClearITPendingBit(DMA1_IT_TC7|DMA1_IT_TE7);
	DMA_Cmd(DMA1_Ch_Usart2_Tx, DISABLE);
	//vUart2_setTxStateOn();
}

void vUart2DmaRxHandle_ISR(void)
{
	DMA_ClearITPendingBit(DMA1_IT_TC6|DMA1_IT_TE6);
	DMA_Cmd(DMA1_Ch_Usart2_Rx, DISABLE);//关闭DMA,防止处理其间有数据, rx
	DMA1_Ch_Usart2_Rx->CNDTR =Rx2BUF_MAX;	//
	DMA1_Ch_Usart2_Rx->CMAR =(uint32_t)puartGetRTxAddress();
	DMA_Cmd(DMA1_Ch_Usart2_Rx, ENABLE);
}

void vuart2DmaTxDataEnable(uint16_t len, uint8_t *address)
{
	DMA_Cmd(DMA1_Ch_Usart2_Tx,DISABLE);
	DMA1_Ch_Usart2_Tx->CNDTR =len;	//
	//DMA1_Ch_Usart2_Tx->CMAR =(uint32_t)&TxBuff;
	DMA1_Ch_Usart2_Tx->CMAR =(uint32_t)address;
	DMA_Cmd(DMA1_Ch_Usart2_Tx,ENABLE);
}


void Usart2IdlHandle_ISR(void)
{
	uint16_t DataLen;
	uint8_t i = 0;
	if (USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
	{
		DMA_Cmd(DMA1_Ch_Usart2_Rx, DISABLE);
		DataLen = Rx2BUF_MAX - DMA_GetCurrDataCounter(DMA1_Ch_Usart2_Rx);
		//
		if (DataLen >0)
		{
			//RT_uartRxPush((uint8_t)DataLen, &uart2RxProcess);
			while(DataLen > 0)
			{
				if (RxBuff[i] == 0xca && RxBuff[i+1] == 0xac)
				{
					break;
				}
				i++;
				DataLen--;
			}
			if (DataLen>0)
			{
				//memcpy(TxBuff, &RxBuff[i], DataLen);	
				//vuart2DmaTxDataEnable(DataLen, &RxBuff[i]);
				RTCom2Rec_pushEvent(&RxBuff[i], DataLen);
			}
		}
		DMA_ClearFlag(DMA1_FLAG_GL6|DMA1_FLAG_TC6|DMA1_FLAG_TE6|DMA1_FLAG_HT6);//清标志
		DMA1_Ch_Usart2_Rx->CNDTR =Rx2BUF_MAX;	//
		DMA1_Ch_Usart2_Rx->CMAR =(uint32_t)puartGetRTxAddress();
		DMA_Cmd(DMA1_Ch_Usart2_Rx, ENABLE);//处理完,重开DMA
		//读SR后读DR清除Idle
		DataLen = USART2->SR;
		DataLen = USART2->DR;
	}
	if (USART_GetITStatus(USART2, USART_IT_PE | USART_IT_FE | USART_IT_NE) != RESET)
	{
		USART_ClearITPendingBit(USART2, USART_IT_PE | USART_IT_FE | USART_IT_NE);
	}
	USART_ClearITPendingBit(USART2, USART_IT_TC);
	USART_ClearITPendingBit(USART2, USART_IT_IDLE);
}

//---------------------
void RTCom2Rec_createEvent(void)
{
	Queue_create(&S_recQue, S_recData, Rx2BUF_MAX);
}
bool RTCom2Rec_popEvent(void* dataAddr)
{
	return Queue_pop(&S_recQue, dataAddr);
}
bool RTCom2Rec_pushEvent(void* dataAddr, uint16_t dataLen)
{
	return Queue_push(&S_recQue, dataAddr, dataLen);
}
uint16_t RTCom2Rec_lengthEvent(void)
{
	return Queue_length(&S_recQue);
}
void RTCom2Rec_destoryEvent(void)
{
	Queue_destory(&S_recQue);
}

//---------
void RTCom2Rec_recProcess(void)
{
	uint8_t funcode;
	if (RTCom2Rec_lengthEvent() == 0)
	{
		return;
	}
	RTCom2Rec_popEvent(S_trec);
	funcode = S_trec[BIT_FUNCODE]&0x0f;
	if ( funcode < REC_CODE_MAX)
	{
		S_tabHandle[funcode](S_trec[BIT_FUNVALUE]);
	}
}

static void recCode4(uint8_t value)
{
	value;
	memcpy(S_txcod4, S_trec, 5);
	vuart2DmaTxDataEnable(5, S_txcod4);
}

bool RTCom2Rec_getPrintADCFlag(void)
{
	return S_setPrintADCFlag;
}

static void recCode1(uint8_t value)
{
	S_setPrintADCFlag = value & 0x01;
}

static void task1RepeatPrintADC(void)
{
	uint16_t datalen;
	if (S_setPrintADCFlag)
	{
		//snprintf
		//datalen = sprintf_s((char*)TxBuff,Tx2BUF_MAX,"adc!");
		datalen = snprintf((char*)TxBuff,Tx2BUF_MAX,"adc!");
		vuart2DmaTxDataEnable(datalen, TxBuff);
	}
}

void RTCom2_task(void)
{
	RTCom2Rec_recProcess();
	task1RepeatPrintADC();
}

