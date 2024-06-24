#ifndef __KEY_H
#define __KEY_H

#include "stm32f4xx.h"
#include "driver.h"
#include "oled.h"
#include "delay.h"
#include "canopen.h"
#include "pid.h"


#define KEY11_GPIO_PORT					GPIOC
#define KEY11_GPIO_PIN					GPIO_Pin_4
#define KEY11_GPIO_RCC					RCC_AHB1Periph_GPIOC

#define KEY12_GPIO_PORT					GPIOC
#define KEY12_GPIO_PIN					GPIO_Pin_5
#define KEY12_GPIO_RCC					RCC_AHB1Periph_GPIOC

#define KEY13_GPIO_PORT					GPIOB
#define KEY13_GPIO_PIN					GPIO_Pin_0
#define KEY13_GPIO_RCC					RCC_AHB1Periph_GPIOB					

#define KEY14_GPIO_PORT					GPIOB
#define KEY14_GPIO_PIN					GPIO_Pin_1
#define KEY14_GPIO_RCC					RCC_AHB1Periph_GPIOB

#define KEY21_GPIO_PORT					GPIOC
#define KEY21_GPIO_PIN					GPIO_Pin_6
#define KEY21_GPIO_RCC					RCC_AHB1Periph_GPIOC

#define KEY22_GPIO_PORT					GPIOC
#define KEY22_GPIO_PIN					GPIO_Pin_7
#define KEY22_GPIO_RCC					RCC_AHB1Periph_GPIOC

#define KEY23_GPIO_PORT					GPIOC
#define KEY23_GPIO_PIN					GPIO_Pin_8
#define KEY23_GPIO_RCC					RCC_AHB1Periph_GPIOC

#define KEY24_GPIO_PORT					GPIOC
#define KEY24_GPIO_PIN					GPIO_Pin_9
#define KEY24_GPIO_RCC					RCC_AHB1Periph_GPIOC


#define KEY11_Event							1<<0
#define KEY12_Event							1<<1
#define KEY13_Event							1<<2
#define KEY14_Event							1<<3
#define KEY21_Event							1<<4
#define KEY22_Event							1<<5
#define KEY23_Event							1<<6
#define KEY24_Event							1<<7


typedef struct
{
	uint8_t Key11;
	uint8_t Key12;
	uint8_t Key13;
	uint8_t Key14;
	uint8_t Key21;
	uint8_t Key22;
	uint8_t Key23;
	uint8_t Key24;
}KEY_Struct;



void Key_Init(void);
uint8_t Key_Is_Pressed(uint8_t key_number);
void Key_Function(void);
void Key11_Function(void);
void Key12_Function(void);
void Key13_Function(void);
void Key14_Function(void);
void Key21_Function(void);
void Key22_Function(void);
void Key23_Function(void);
void Key24_Function(void);

#endif






