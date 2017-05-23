#include "comm.h"

#define RTLEN 40

//创建全局队列
static Queue_T S_sendQue;
static uint8_t S_sendData[QUEUELENGTH][RTLEN];

//临时存储帧发送1
static uint8_t S_tsend1[RTLEN];

static uint8_t S_tDmaSend1[RTLEN];

#define Bounds 9600
#define Rx3BUF_MAX 40

//Pa8 re pin
#define RS485EN_H(pin) GPIOA->BSRR=pin
#define RS485EN_L(pin) GPIOA->BRR=pin

static uint8_t RxBuff[Rx3BUF_MAX]={0};

static RTRCFlag_T S_rcFlag;

void RTCom3_initRCflag(P_RTRCFlag rcFlag)
{
	rcFlag->event.eventType = UART_TYPE;
	rcFlag->event.eventId = RTCOM3;
	rcFlag->recFlag = DONE;
	rcFlag->sendCount = 0;
	rcFlag->sendCountMax = 3;
}

static uint8_t* getSend1FrameHeaderAddr(void)
{
	return S_tsend1;
}

static uint8_t* getSend1FrameDataAddr(void)
{
	return S_tsend1 + sizeof(RTHeader_T);
}

void RTCom3_createEvent(void)
{
	Queue_create(&S_sendQue, S_sendData, RTLEN);
	RTCom3_initRCflag(&S_rcFlag);
}
bool RTCom3_popEvent(void* dataAddr)
{
	return Queue_pop(&S_sendQue, dataAddr);
}
bool RTCom3_pushEvent(void* dataAddr, uint16_t dataLen)
{
	return Queue_push(&S_sendQue, dataAddr, dataLen);
}
uint16_t RTCom3_lengthEvent(void)
{
	return Queue_length(&S_sendQue);
}
void RTCom3_destoryEvent(void)
{
	Queue_destory(&S_sendQue);
}

uint8_t RTCom3_checkXrr(void* addr, uint8_t len)
{
	uint8_t result = 0;
	uint8_t* src = (uint8_t*)addr;
	while(len > 0)
	{
		result ^= *src++;
		len--;
	}
	return result;
}


void RTCom3_SendCM1(uint8_t vHz)
{
	//add header, data
	P_RTHeader frameHeader;
	P_RTCom3SFrame1 frameData;
	uint8_t xrrLen;
	xrrLen = sizeof(RTHeader_T) + sizeof(RTCom3SFrame1_T);

	frameHeader = (P_RTHeader)getSend1FrameHeaderAddr();
	frameData = (P_RTCom3SFrame1)getSend1FrameDataAddr();

	frameHeader->event.eventType = UART_TYPE;
	frameHeader->event.eventId = RTCOM3;
	frameHeader->len = sizeof(RTCom3SFrame1_T);

	frameData->header.header1 = 0x7e;
	frameData->header.header2 = 0x7e;
	frameData->header.dstAddr = 0xc0;
	frameData->header.srcAddr = 0x00;
	frameData->header.frameCode = 0x11;
	frameData->header.funCode = 0x83;
	frameData->header.len = 8;

	frameData->data.p1Set = vHz;
	frameData->data.p2run = 0;
	frameData->data.p3Code = 0;
	frameData->data.p4Upspeed = 0x0a;
	frameData->data.p5DownSpeed = 0x0a;
	frameData->data.p6Null = 0x00;

	//check err
	frameData->crr = RTCom3_checkXrr(frameData, frameHeader->len - 1);

	//push to queue
	RTCom3_pushEvent(frameHeader, xrrLen);
}

static bool sendNext(P_RTRCFlag rcFlag)
{
	if (rcFlag->sendCount >= rcFlag->sendCountMax)
	{
		//recv err
		//reset rcvflag done
		rcFlag->recFlag = DONE;
		return true;
	}
	if (rcFlag->recFlag == DONE)
	{
		return true;
	}else
	{
		//not receive
		rcFlag->sendCount++;
		return false;
	}
	//if (RTCom3_lengthEvent() == 0)
	//{
	//	return false;
	//}
}

void RTCom3_SendProcess(P_RTRCFlag rcFlag)
{
	assert(rcFlag);

	if (!sendNext(rcFlag))
	{
		//resend
		//set dma addr at S_tDmaSend1
		return;
	}

	if (RTCom3_lengthEvent() == 0)
	{
		return ;
	}
	
	//pop next queue
	//set dma addr count, 1.cast to RTHeader_T,obtain len, uartx,
	//2.get data Frame addr,set dma addr, len
	//3.set rcFlag->recFlag = UNDONE;
	//reset sendCount
	RTCom3_popEvent(S_tDmaSend1);
	rcFlag->sendCount = 0;
}

void RTCom3_setRecFlag(P_Event event)
{
	if (event->eventId == RTCOM3)
	{
		S_rcFlag.recFlag = DONE;
	}
}

//**********************************
//only send

static uint8_t* puartGetRTxAddress(void)
{
	return RxBuff;
}

void UART3_Init(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	DMA_InitTypeDef		DMA_InitStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;
	USART_InitTypeDef	USART_InitStructure;

	//必须让uart1初始化在前，然后才能初始化uart2
	//否则开APB2-AFIO，PORT，APB1-uart，AHB-DMA
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RS485_GPIO_CLK,ENABLE);
	RCC_APB1PeriphClockCmd(RS485_UART3_CLK,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);

	// Configure USART_Rx as input floating 
	GPIO_InitStructure.GPIO_Pin = RS485_RxPin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(RS485_GPIO, &GPIO_InitStructure);

	// Configure USART_Tx as alternate function push-pull 
	GPIO_InitStructure.GPIO_Pin = RS485_TxPin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(RS485_GPIO,&GPIO_InitStructure);

	//configure rx485 re pa.10@@@@
	GPIO_InitStructure.GPIO_Pin = RS485_RePin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	DMA_DeInit(DMA1_Ch_Usart3_Tx);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&RS485_UART3->DR;//(uint32_t)USART3_Tx_DR_Base;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&S_tDmaSend1;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = RTLEN;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

	DMA_Init(DMA1_Ch_Usart3_Tx,&DMA_InitStructure);
	DMA_ITConfig(DMA1_Ch_Usart3_Tx,DMA_IT_TC,ENABLE);
	DMA_ITConfig(DMA1_Ch_Usart3_Tx,DMA_IT_TE,ENABLE);
	USART_DMACmd(RS485_UART3, USART_DMAReq_Tx, ENABLE);
	DMA_Cmd(DMA1_Ch_Usart3_Tx,DISABLE);

	DMA_DeInit(DMA1_Ch_Usart3_Rx);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&RS485_UART3->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)puartGetRTxAddress();
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = Rx3BUF_MAX;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;

	DMA_Init(DMA1_Ch_Usart3_Rx,&DMA_InitStructure);
	DMA_ITConfig(DMA1_Ch_Usart3_Rx,DMA_IT_TC,ENABLE);//transmit complete
	DMA_ITConfig(DMA1_Ch_Usart3_Rx,DMA_IT_TE,ENABLE);//transmit enable
	USART_DMACmd(RS485_UART3, USART_DMAReq_Rx, ENABLE);
	DMA_Cmd(DMA1_Ch_Usart3_Rx,ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//Enable DMA Channel2 Interrupt 
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Ch_Usart3_Tx_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//Enable DMA Channel3 Interrupt 
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Ch_Usart3_Rx_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_InitStructure.USART_BaudRate = Bounds;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(RS485_UART3, &USART_InitStructure);

	//空闲中断
	USART_ITConfig(RS485_UART3, USART_IT_IDLE , ENABLE);//
	USART_Cmd(RS485_UART3, ENABLE);
	USART_ClearFlag(RS485_UART3, USART_FLAG_TC);     //* 清发送外城标志，

	//RTCom2_paraInit();

	RS485EN_H(RS485_RePin);

	//set to rx mode,set to low
	RS485EN_L(RS485_RePin);
}

void Uart3_DmaTxHandler_ISR(void)
{
	DMA_ClearITPendingBit(DMA1_IT_TC2|DMA1_IT_TE2);
	DMA_Cmd(DMA1_Ch_Usart3_Tx, DISABLE);
	
	//set back to rec mode ,re = 0
	RS485EN_L(RS485_RePin);
	//vUart2_setTxStateOn();
}

void Uart3_DmaRxHandle_ISR(void)
{
	DMA_ClearITPendingBit(DMA1_IT_TC3|DMA1_IT_TE3);
	DMA_Cmd(DMA1_Ch_Usart3_Rx, DISABLE);//关闭DMA,防止处理其间有数据, rx
	DMA1_Ch_Usart3_Rx->CNDTR =Rx3BUF_MAX;	//
	DMA1_Ch_Usart3_Rx->CMAR =(uint32_t)puartGetRTxAddress();
	DMA_Cmd(DMA1_Ch_Usart3_Rx, ENABLE);
}

void vuart3DmaTxDataEnable(uint16_t len, uint8_t *address)
{
	//before enable tx,set rePin to high
	RS485EN_H(RS485_RePin);

	DMA_Cmd(DMA1_Ch_Usart3_Tx,DISABLE);
	DMA1_Ch_Usart3_Tx->CNDTR =len;	//
	DMA1_Ch_Usart3_Tx->CMAR =(uint32_t)address;

	DMA_Cmd(DMA1_Ch_Usart3_Tx,ENABLE);
}

void Usart3_IdlHandle_ISR(void)
{
	uint16_t DataLen;
	uint8_t i = 0;
	//if (1)
	if (USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
	{
		DMA_Cmd(DMA1_Ch_Usart3_Rx, DISABLE);
		DataLen = Rx3BUF_MAX - DMA_GetCurrDataCounter(DMA1_Ch_Usart3_Rx);
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
				memcpy(S_tDmaSend1, &RxBuff[i], DataLen);	
				vuart3DmaTxDataEnable(DataLen, &RxBuff[i]);
				//RTCom2Rec_pushEvent(&RxBuff[i], DataLen);
			}
		}

		DMA_ClearFlag(DMA1_FLAG_GL3|DMA1_FLAG_TC3|DMA1_FLAG_TE3|DMA1_FLAG_HT3);//清标志
		DMA1_Ch_Usart3_Rx->CNDTR =Rx3BUF_MAX;	//
		DMA1_Ch_Usart3_Rx->CMAR =(uint32_t)puartGetRTxAddress();
		DMA_Cmd(DMA1_Ch_Usart3_Rx, ENABLE);//处理完,重开DMA
		//读SR后读DR清除Idle
		DataLen = USART3->SR;
		DataLen = USART3->DR;	
	}

	if (USART_GetITStatus(USART3, USART_IT_PE | USART_IT_FE | USART_IT_NE) != RESET)
	{
		USART_ClearITPendingBit(USART3, USART_IT_PE | USART_IT_FE | USART_IT_NE);
	}
	USART_ClearITPendingBit(USART3, USART_IT_TC);
	USART_ClearITPendingBit(USART3, USART_IT_IDLE);
}


