#ifndef __PID_H
#define __PID_H

#include "stm32f4xx.h"

#define PID_TIMER							TIM9
#define PID_TIMER_RCC					RCC_APB2Periph_TIM9
#define FILTER								1

// λ��ʽpid
typedef struct 
{
	float Target;									//�����趨ֵ
	float ActualValue;						//����ʵ��ֵ
	float Err;										//����ƫ��ֵ
	float Err_last;								//������һ��ƫ��ֵ
	float Integral;								//����ֵ
	float Kp, Ki, Kd;							//������������֡�΢��ϵ��
	float Output;									//PID���������ֵ
	float Output_last;
	
	uint32_t Sampling_time_ms;		//����ʱ��ms
}PID_Postype_Structure;

// ����ʽpid
typedef struct
{
	float Target;									//�����趨ֵ
	float ActualValue;						//����ʵ��ֵ
	float Err;										//����ƫ��ֵ
	float Err_last;								//������һ��ƫ��ֵ
	float Err_last_last;					//��������һ��ƫ��ֵ
	float Kp, Ki, Kd;							//������������֡�΢��ϵ��
	float Output;									//PID���������ֵ
	float Output_last;						
	
	uint32_t Sampling_time_ms;		//����ʱ��ms
}PID_Inctype_Structure;


void PID_Init(void);
float PID_Postype_Calculate(float target, float current_value, PID_Postype_Structure *pid);
float PID_Inctype_Calculate(float target, float current_value, PID_Inctype_Structure *pid);


void PID_Pos_Clear_Value(PID_Postype_Structure *pid);
void PID_Inc_Clear_Value(PID_Inctype_Structure *pid);


#endif




