#ifndef __USART_H
#define __USART_H

#include "stm32f4xx.h"
#include "stdio.h"

#define USART_TX_GPIO_PORT				GPIOA
#define USART_TX_GPIORCC					RCC_AHB1Periph_GPIOA
#define USART_TX_GPIO_PIN					GPIO_Pin_2
#define USART_TX_GPIO_PINSOURCE		GPIO_PinSource2

#define USART_RX_GPIO_PORT				GPIOA
#define USART_RX_GPIORCC					RCC_AHB1Periph_GPIOA
#define USART_RX_GPIO_PIN					GPIO_Pin_3
#define USART_RX_GPIO_PINSOURCE		GPIO_PinSource3

#define USART_										USART2
#define USART_RCC									RCC_APB1Periph_USART2
#define USART_BAUDRATE           	115200




void Usart_Init(void);
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);



#endif





