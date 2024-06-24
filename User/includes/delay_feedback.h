#ifndef __DELAY_FEEDBACK_H
#define __DELAY_FEEDBACK_H

#include "stm32f4xx.h"
#include "encoder.h"
#include "arm_math.h"

#define POSITIVE_FEEDBACK_1				0
#define POSITIVE_FEEDBACK_2				1

#define HISTORY_UPDATE_TIMER			TIM7				// 专门用于更新用于时滞反馈的历史数据
#define UPDATE_ITTIME							1						// 历史数据更新时间ms

typedef struct
{
	uint8_t status;		// 0关闭 1开启
	int16_t *History_data;
	uint16_t Current_Index;
	float32_t g;
	uint16_t tau_ms;
}Delay_Feedback_Struct;

void Delay_Feedback_Init(void);

void Distance_History_Update(Delay_Feedback_Struct* delay_comtroller, int16_t data);						// 更新历史数据
int16_t Distance_History_Get(Delay_Feedback_Struct* delay_comtroller, uint16_t time_ms);				// 获取历史数据

void Change_Feedback_Parameter(Delay_Feedback_Struct* delay_comtroller, float32_t g, uint16_t tau_ms);
float32_t Calculat_Feedback_Voltage(Delay_Feedback_Struct* delay_comtroller);



#endif





