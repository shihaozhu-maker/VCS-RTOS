#include "delay.h"

/*  
 * ���ö�ʱ��10  Timer 10 ������ʱ
*/
void delay_init(void)
{
	
	RCC_APB2PeriphClockCmd(DELAY_TIMER_RCC, ENABLE);
	
	// ʱ��TimeBase 168/2*2=168M
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);
	TIM_TimeBaseInitStructure.TIM_Period = DELAY_US_PERIOD;
	TIM_TimeBaseInit(DELAY_TIMER, &TIM_TimeBaseInitStructure);
	
	TIM_Cmd(DELAY_TIMER, ENABLE);
}	

/*
 * us��ʱ����
 * SysTick->LOADΪ24λ�Ĵ��� ��ʱnus<2^24/(168/8)=798,915
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
 * ms��ʱ����
 * ͨ��ѭ������delay_usʵ������ʱ������ʱ
*/
void delay_ms(uint16_t nms)
{	 	 
	for(uint16_t i=0;i<nms;i++)
		delay_us(1000);
} 

