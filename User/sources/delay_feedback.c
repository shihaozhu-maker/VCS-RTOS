#include "delay_feedback.h"

int16_t Distance_History1[1000] = {0};		// 存放历史位移数据
int16_t Distance_History2[1000] = {0};

Delay_Feedback_Struct Delay_Feedback_Controller1 = 
{
	.status = 0,
	.History_data = Distance_History1,
	.Current_Index = 0,
	.g = 0.0f,
	.tau_ms = 0
};

Delay_Feedback_Struct Delay_Feedback_Controller2 = 
{
	.status = 0,
	.History_data = Distance_History2,
	.Current_Index = 0,
	.g = 0.0f,
	.tau_ms = 0
};

void Delay_Feedback_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	
	// 时基TimeBase 168/4*2/84=1M
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);
	TIM_TimeBaseInitStructure.TIM_Prescaler = 84;
	TIM_TimeBaseInitStructure.TIM_Period = UPDATE_ITTIME*1000;
	TIM_TimeBaseInit(HISTORY_UPDATE_TIMER, &TIM_TimeBaseInitStructure);
	
	NVIC_InitTypeDef NVIC_InitTypeStructure;
	NVIC_InitTypeStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitTypeStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitTypeStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitTypeStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitTypeStructure);
	TIM_ITConfig(HISTORY_UPDATE_TIMER, TIM_IT_Update, ENABLE);
	
	TIM_Cmd(HISTORY_UPDATE_TIMER, ENABLE);
}


 void Distance_History_Update(Delay_Feedback_Struct* delay_comtroller, int16_t data)
{
	if((++(delay_comtroller->Current_Index))>=1000)
		delay_comtroller->Current_Index = 0;
	
	delay_comtroller->History_data[delay_comtroller->Current_Index]=data;
}

int16_t Distance_History_Get(Delay_Feedback_Struct* delay_comtroller, uint16_t time_ms)
{ 
	if(time_ms>=1000)
		return 0;
	
	uint16_t History_Index;
	if((delay_comtroller->Current_Index-time_ms)>=0)
		History_Index = delay_comtroller->Current_Index-time_ms;
	else
		History_Index = 1000+delay_comtroller->Current_Index-time_ms;
	return delay_comtroller->History_data[History_Index];
}

void Change_Feedback_Parameter(Delay_Feedback_Struct* delay_comtroller, float32_t g, uint16_t tau_ms)
{
	if(g>9999)
		g=9999;
	else if(g<0)
		g=0;
	
	if(tau_ms>32767)
		tau_ms=0;
	else if(tau_ms>9999)
		tau_ms=9999;
	
	delay_comtroller->tau_ms = tau_ms;
	delay_comtroller->g = g;
}

float32_t Calculat_Feedback_Voltage(Delay_Feedback_Struct* delay_comtroller)
{
	float32_t feedback_voltage;
	feedback_voltage = delay_comtroller->g * Distance_History_Get(delay_comtroller,delay_comtroller->tau_ms) * 0.000001l / 3;
	return feedback_voltage;
}



