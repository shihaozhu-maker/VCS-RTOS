#ifndef __LED_H
#define __LED_H

#include "stm32f4xx.h"

// led1
#define LED_L1_PORT			GPIOC
#define LED_L1_PIN			GPIO_Pin_15
#define LED_L1_RCCPORT	RCC_AHB1Periph_GPIOC

// led2
#define LED_L2_PORT			GPIOC
#define LED_L2_PIN			GPIO_Pin_14
#define LED_L2_RCCPORT	RCC_AHB1Periph_GPIOC

// led3
#define LED_L3_PORT			GPIOC
#define LED_L3_PIN			GPIO_Pin_13
#define LED_L3_RCCPORT	RCC_AHB1Periph_GPIOC

// led4
#define LED_L4_PORT			GPIOB
#define LED_L4_PIN			GPIO_Pin_9
#define LED_L4_RCCPORT	RCC_AHB1Periph_GPIOB

// led5
#define LED_R1_PORT			GPIOC
#define LED_R1_PIN			GPIO_Pin_10
#define LED_R1_RCCPORT	RCC_AHB1Periph_GPIOC

// led6
#define LED_R2_PORT			GPIOA
#define LED_R2_PIN			GPIO_Pin_15
#define LED_R2_RCCPORT	RCC_AHB1Periph_GPIOA


void LED_Init(void);
void LED_On(uint8_t led);
void LED_On_Mask(uint8_t led);
void LED_Off(uint8_t led);
void LED_Off_Mask(uint8_t led);
void LED_OverTurn(uint8_t led);
void LED_OverTurn_Mask(uint8_t led);



#endif

