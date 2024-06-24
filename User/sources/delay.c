#include "delay.h"

/*  
 * 配置定时器10  Timer 10 用于延时
*/
void delay_init(void)
{
	
	RCC_APB2PeriphClockCmd(DELAY_TIMER_RCC, ENABLE);
	
	// 时基TimeBase 168/2*2=168M
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);
	TIM_TimeBaseInitStructure.TIM_Period = DELAY_US_PERIOD;
	TIM_TimeBaseInit(DELAY_TIMER, &TIM_TimeBaseInitStructure);
	
	TIM_Cmd(DELAY_TIMER, ENABLE);
}	

/*
 * us延时函数
 * SysTick->LOAD为24位寄存器 延时nus<2^24/(168/8)=798,915
*/
void delay_1us(void)
{		
	TIM_SetCounter(DELAY_TIMER, 5);
	while(TIM_GetCounter(DELAY_TIMER)>5);
}


void delay_us(uint16_t nns)
{		
	for(uint16_t i=0;i<nns;i++)
		delay_1us();
}

/*
 * ms延时函数
 * 通过循环调用delay_us实现无限时长的延时
*/
void delay_ms(uint16_t nms)
{	 	 
	for(uint16_t i=0;i<nms;i++)
		delay_us(1000);
} 

