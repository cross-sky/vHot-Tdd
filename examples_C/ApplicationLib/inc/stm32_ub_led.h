#ifndef __STM32_UB_LED_H
#define	__STM32_UB_LED_H

#include "commdata.h"
#include "stm32f10x.h"

typedef enum
{
	LED1 = 0,
//	LED3 = 1,
//	LED13 = 2,
	LED_ANZ 
}LED_NAME_t;


typedef enum {
	LED_OFF = 0,     // LED EIN
	LED_ON = 1			// LED AUS
}LED_STATUS_t;

typedef struct  
{
	LED_NAME_t		LED_NAME;
	GPIO_TypeDef*	LED_PORT;
	const uint16_t	LED_PIN;
	const uint32_t	LED_CLK;
	LED_STATUS_t	LED_INIT;
}LED_t;

void UB_Led_Off(LED_NAME_t led_name);
void UB_Led_On(LED_NAME_t led_name);
void UB_Led_Toggle(LED_NAME_t led_name);
void UB_Led_Switch(LED_NAME_t led_name, LED_STATUS_t status);
void UB_LED_Init(void);
void LED1on(void);
// 带参宏，可以像内联函数一样使用;
// #define LED1(a)	if (a)	\
// 					GPIO_SetBits(GPIOC,GPIO_Pin_1);\
// 					else		\
// 					GPIO_ResetBits(GPIOC,GPIO_Pin_1)
// 
// extern void LedGpioConfig(void);
// extern void LEDdisplay(u8 LightChannel);
//  void LED1on(void);


#endif /* __LED_H */


