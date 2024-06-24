#ifndef __PID_H
#define __PID_H

#include "stm32f4xx.h"

#define PID_TIMER							TIM9
#define PID_TIMER_RCC					RCC_APB2Periph_TIM9
#define FILTER								1

// 位置式pid
typedef struct 
{
	float Target;									//定义设定值
	float ActualValue;						//定义实际值
	float Err;										//定义偏差值
	float Err_last;								//定义上一个偏差值
	float Integral;								//积分值
	float Kp, Ki, Kd;							//定义比例、积分、微分系数
	float Output;									//PID控制器输出值
	float Output_last;
	
	uint32_t Sampling_time_ms;		//采样时间ms
}PID_Postype_Structure;

// 增量式pid
typedef struct
{
	float Target;									//定义设定值
	float ActualValue;						//定义实际值
	float Err;										//定义偏差值
	float Err_last;								//定义上一个偏差值
	float Err_last_last;					//定义上上一个偏差值
	float Kp, Ki, Kd;							//定义比例、积分、微分系数
	float Output;									//PID控制器输出值
	float Output_last;						
	
	uint32_t Sampling_time_ms;		//采样时间ms
}PID_Inctype_Structure;


void PID_Init(void);
float PID_Postype_Calculate(float target, float current_value, PID_Postype_Structure *pid);
float PID_Inctype_Calculate(float target, float current_value, PID_Inctype_Structure *pid);


void PID_Pos_Clear_Value(PID_Postype_Structure *pid);
void PID_Inc_Clear_Value(PID_Inctype_Structure *pid);


#endif




