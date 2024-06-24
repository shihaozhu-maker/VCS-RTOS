#ifndef __BSP_DELAY_H
#define __BSP_DELAY_H

#include "stm32f4xx.h"

#define system_frequency 				168
#define DELAY_TIMER							TIM10	
#define DELAY_TIMER_RCC					RCC_APB2Periph_TIM10
#define DELAY_US_PERIOD					168-1

void delay_init(void);
void delay_ms(uint16_t nms);
void delay_us(uint16_t nus);

#endif /* __BSP_DELAY_H */

