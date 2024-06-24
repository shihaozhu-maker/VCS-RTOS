#ifndef __SPI_H
#define __SPI_H

#include "stm32f4xx.h"

#define SPI_NSS_GPIO_PORT			GPIOB
#define SPI_NSS_GPIORCC				RCC_AHB1Periph_GPIOB
#define SPI_NSS_GPIO_PIN			GPIO_Pin_12
#define SPI_NSS_GPIO_PINSOURCE		GPIO_PinSource12

#define SPI_SCK_GPIO_PORT			GPIOB
#define SPI_SCK_GPIORCC				RCC_AHB1Periph_GPIOB
#define SPI_SCK_GPIO_PIN			GPIO_Pin_13
#define SPI_SCK_GPIO_PINSOURCE		GPIO_PinSource13

#define SPI_MISO_GPIO_PORT			GPIOB
#define SPI_MISO_GPIORCC			RCC_AHB1Periph_GPIOB
#define SPI_MISO_GPIO_PIN			GPIO_Pin_14
#define SPI_MISO_GPIO_PINSOURCE		GPIO_PinSource14

#define SPI_MOSI_GPIO_PORT			GPIOB
#define SPI_MOSI_GPIORCC			RCC_AHB1Periph_GPIOB
#define SPI_MOSI_GPIO_PIN			GPIO_Pin_15
#define SPI_MOSI_GPIO_PINSOURCE		GPIO_PinSource15


#define SPIX			SPI2
#define SPIX_RCC		RCC_APB1Periph_SPI2 

void spi_Init(void);




#endif







