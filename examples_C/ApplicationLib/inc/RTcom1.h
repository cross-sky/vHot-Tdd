#ifndef __UART1RR_H
#define __UART1RR_H

#include "commdata.h"


#define USART1_Rx				USART1
#define USART1_Rx_GPIO			GPIOA
#define USART1_Rx_CLK			RCC_APB2Periph_USART1
#define USART1_Rx_GPIO_CLK		RCC_APB2Periph_GPIOA
#define USART1_RxPin			GPIO_Pin_10

#define USART1_Tx				USART1
#define USART1_Tx_GPIO			GPIOA
#define USART1_Tx_CLK			RCC_APB2Periph_USART1
#define USART1_Tx_GPIO_CLK		RCC_APB2Periph_GPIOA
#define USART1_TxPin			GPIO_Pin_9
//����һ��˫��Ĵ�����������TDR��RDR����������������ȡ����RDR�Ĵ�����ֵ��������д������ʵ������д��TDR�Ĵ�����
#define USART1_Tx_DR_Base		0x40013804
#define DMA1_Ch_Usart1_Tx		DMA1_Channel4
#define DMA1_Ch_Usart1_Tx_IRQn	DMA1_Channel4_IRQn

#define USART1_Rx_DR_Base		0x40013804
#define DMA1_Ch_Usart1_Rx		DMA1_Channel5
#define DMA1_Ch_Usart1_Rx_IRQn	DMA1_Channel5_IRQn

void Uart1_DmaRxHandle_ISR(void);
void Uart1_DmaTxHandler_ISR(void);
void Usart1_IdlHandle_ISR(void);
void RTCom1_init(void);

#endif

