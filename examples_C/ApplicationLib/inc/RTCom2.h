#ifndef __RTCOM2_H
#define __RTCOM2_H

#include "commdata.h"

#define USART2_Rx				USART2
#define USART2_Rx_GPIO			GPIOA
#define USART2_Rx_CLK			RCC_APB1Periph_USART2
#define USART2_Rx_GPIO_CLK		RCC_APB2Periph_GPIOA
#define USART2_RxPin			GPIO_Pin_3

#define USART2_Tx				USART2
#define USART2_Tx_GPIO			GPIOA
#define USART2_Tx_CLK			RCC_APB1Periph_USART2
#define USART2_Tx_GPIO_CLK		RCC_APB2Periph_GPIOA
#define USART2_TxPin			GPIO_Pin_2
//这是一个双向寄存器，包含了TDR和RDR，对它读操作，读取的是RDR寄存器的值，对它的写操作，实际上是写到TDR寄存器的
#define USART2_Tx_DR_Base		0x40004404
#define DMA1_Ch_Usart2_Tx		DMA1_Channel7
#define DMA1_Ch_Usart2_Tx_IRQn	DMA1_Channel7_IRQn

#define USART2_Rx_DR_Base		0x40004404
#define DMA1_Ch_Usart2_Rx		DMA1_Channel6
#define DMA1_Ch_Usart2_Rx_IRQn	DMA1_Channel6_IRQn

void UART2_Init(void);
void vUart2DmaTxHandler_ISR(void);
void vUart2DmaRxHandle_ISR(void);
void Usart2IdlHandle_ISR(void);
void vuart2DmaTxDataEnable(uint16_t len, uint8_t *address);

void RTCom2Rec_createEvent(void);
void RTCom2Rec_destoryEvent(void);
void RTCom2Rec_recProcess(void);
bool RTCom2Rec_pushEvent(void* dataAddr, uint16_t dataLen);

bool RTCom2Rec_getPrintADCFlag(void);
void RTCom2_paraInit(void);
void RTCom2_task(void);
#endif

