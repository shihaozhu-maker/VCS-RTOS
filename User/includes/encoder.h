#ifndef __ENCODER_H
#define __ENCODER_H

#include "stm32f4xx.h"

#define COUNTER_PERIOD				0xffff		// 16bit
#define COUNTER_PSC						0


// encoder1
#define ENCODER1_A_GPIOPORT				GPIOA
#define ENCODER1_A_GPIOPIN				GPIO_Pin_0
#define ENCODER1_A_PINSOURCE			GPIO_PinSource0
#define ENCODER1_A_GPIORCC				RCC_AHB1Periph_GPIOA

#define ENCODER1_B_GPIOPORT				GPIOA
#define ENCODER1_B_GPIOPIN				GPIO_Pin_1
#define ENCODER1_B_PINSOURCE			GPIO_PinSource1
#define ENCODER1_B_GPIORCC				RCC_AHB1Periph_GPIOA

#define ENCODER1_AO_GPIOPORT			GPIOC
#define ENCODER1_AO_GPIOPIN				GPIO_Pin_1
#define ENCODER1_AO_PINSOURCE			GPIO_PinSource1
#define ENCODER1_AO_GPIORCC				RCC_AHB1Periph_GPIOC

#define ENCODER1_INDEX_GPIOPORT		GPIOC
#define ENCODER1_INDEX_GPIOPIN		GPIO_Pin_0
#define ENCODER1_INDEX_GPIORCC		RCC_AHB1Periph_GPIOC

#define ENCODER1_TIMER						TIM5
#define ENCODER1_TIMER_RCC				RCC_APB1Periph_TIM5


// encoder2
#define ENCODER2_A_GPIOPORT				GPIOA
#define ENCODER2_A_GPIOPIN				GPIO_Pin_6
#define ENCODER2_A_PINSOURCE			GPIO_PinSource6
#define ENCODER2_A_GPIORCC				RCC_AHB1Periph_GPIOA

#define ENCODER2_B_GPIOPORT				GPIOA
#define ENCODER2_B_GPIOPIN				GPIO_Pin_7
#define ENCODER2_B_PINSOURCE			GPIO_PinSource7
#define ENCODER2_B_GPIORCC				RCC_AHB1Periph_GPIOA

#define ENCODER2_AO_GPIOPORT			GPIOC
#define ENCODER2_AO_GPIOPIN				GPIO_Pin_3
#define ENCODER2_AO_PINSOURCE			GPIO_PinSource3
#define ENCODER2_AO_GPIORCC				RCC_AHB1Periph_GPIOC

#define ENCODER2_INDEX_GPIOPORT		GPIOC
#define ENCODER2_INDEX_GPIOPIN		GPIO_Pin_2
#define ENCODER2_INDEX_GPIORCC		RCC_AHB1Periph_GPIOC

#define ENCODER2_TIMER						TIM3
#define ENCODER2_TIMER_RCC				RCC_APB1Periph_TIM3

// ADC
#define ENCODER_ADC								ADC1	
#define ENCODER_ADC_RCC						RCC_APB2Periph_ADC1
#define ENCODER1_ADC_CHANNEL			ADC_Channel_11
#define ENCODER2_ADC_CHANNEL			ADC_Channel_13

// DMA
#define ENCODER_DMA								DMA2_Stream0
#define ENCODER_DMA_RCC						RCC_AHB1Periph_DMA2

// Timer
#define ENCODER_TIMER							TIM6
#define ENCODER_TIMER_RCC					RCC_APB1Periph_TIM6



#define ENCODER_TIMER_ITTIME			5			// 中断时间（ms）

#define ENCODER_OLED_REFRESH			10		// OLED显示更新的中断周期数
#define ENCODER_USART_REFRESH			1			// 串口发送数据的中断周期数
#define ENCODER_RSSI_REFRESH			20		// 信号强度数据更新的中断周期数
#define ENCODER_DATA_REFRESH			1			// 更新编码器数据的中断周期数

typedef struct
{
	uint8_t ID;
	uint8_t Status;
	uint16_t Magnetic_Field;
	uint16_t Counter_Value;
	int16_t Distance;
	uint8_t Resolution;
}Encoder_Struct;

#define Resolution_AS5304					25		// um

void Encoder_Init(void);
void Encoder_Magnetic_Field(void);
void Get_Encoder_Value(void);
void Clear_Encoder_Value(Encoder_Struct* encoder);


#endif





