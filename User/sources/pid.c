#include "pid.h"

static void PID_Timer_Init(void);
static float Filer(float current, float last);

PID_Postype_Structure pid1; 
PID_Postype_Structure pid2; 

PID_Inctype_Structure pid_inc1;
PID_Inctype_Structure pid_inc2;

void PID_Init()
{
	pid1.Target=0.0;
	pid1.ActualValue=0.0;
	pid1.Err=0.0;
	pid1.Err_last=0.0;
	pid1.Output=0.0;
	pid1.Output_last=0.0;
	pid1.Integral=0.0;
	pid1.Kp=0.25;
	pid1.Ki=0.002;
	pid1.Kd=1;
	pid1.Sampling_time_ms = 1;
	
	pid2.Target=0.0;
	pid2.ActualValue=0.0;
	pid2.Err=0.0;
	pid2.Err_last=0.0;
	pid2.Output=0.0;
	pid2.Output_last=0.0;
	pid2.Integral=0.0;
	pid2.Kp=0.25;
	pid2.Ki=0.002;
	pid2.Kd=1;
	pid2.Sampling_time_ms = 1;

	pid_inc1.Target = 0.0;
	pid_inc1.ActualValue = 0.0;
	pid_inc1.Err = 0.0;
	pid_inc1.Err_last = 0.0;
	pid_inc1.Err_last_last = 0.0;
	pid_inc1.Output = 0.0;
	pid_inc1.Output_last = 0.0;
	pid_inc1.Kp = 0.25;
	pid_inc1.Ki = 0.002;
	pid_inc1.Kd = 1;
	pid_inc1.Sampling_time_ms = 1;
	
	pid_inc2.Target = 0.0;
	pid_inc2.ActualValue = 0.0;
	pid_inc2.Err = 0.0;
	pid_inc2.Err_last = 0.0;
	pid_inc2.Err_last_last = 0.0;
	pid_inc2.Output = 0.0;
	pid_inc2.Output_last = 0.0;
	pid_inc2.Kp = 0.25;
	pid_inc2.Ki = 0.002;
	pid_inc2.Kd = 1;
	pid_inc2.Sampling_time_ms = 1;
}

static void PID_Timer_Init(void)
{
	RCC_APB2PeriphClockCmd(PID_TIMER_RCC, ENABLE);
	
	// 定时器9时钟频率168M
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInit_Structure;
	TIM_TimeBaseStructInit(&TIM_TimeBaseInit_Structure);
	TIM_TimeBaseInit_Structure.TIM_Prescaler = 168;
	TIM_TimeBaseInit_Structure.TIM_Period = 1000;		// 1ms
	TIM_TimeBaseInit(PID_TIMER, &TIM_TimeBaseInit_Structure);
	
	NVIC_InitTypeDef NVIC_InitTypeStructure;
	NVIC_InitTypeStructure.NVIC_IRQChannel = TIM1_BRK_TIM9_IRQn;
	NVIC_InitTypeStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitTypeStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitTypeStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitTypeStructure);
	TIM_ClearITPendingBit(PID_TIMER, TIM_IT_Update);
	TIM_ITConfig(PID_TIMER, TIM_IT_Update, ENABLE);
	
	TIM_Cmd(PID_TIMER, ENABLE);
}

// 计算PID输出值
float PID_Postype_Calculate(float target, float current_value, PID_Postype_Structure *pid)
{
	pid->Target = target;
	pid->ActualValue = current_value;
	pid->Err = pid->Target - pid->ActualValue;
	pid->Integral += pid->Err;
	
	pid->Output = pid->Kp*pid->Err + pid->Ki*pid->Integral + pid->Kd*(pid->Err-pid->Err_last);
	
	// 输出限幅
	if(pid->Output>10.0f)
		pid->Output = 10.0f;
	else if(pid->Output<-10.0f)
		pid->Output = -10.0f;
	
	//滤波
	if(FILTER)
	{
		pid->Output = Filer(pid->Output,pid->Output_last);
	}
	
	
	pid->Err_last = pid->Err;
	pid->Output_last = pid->Output;
	
	return pid->Output;
}



float PID_Inctype_Calculate(float target, float current_value, PID_Inctype_Structure *pid)
{
	float pidout;
	pid->Target = target;
	pid->ActualValue = current_value;
	pid->Err = pid->Target - pid->ActualValue;
	
	pid->Output = pid->Output_last + pid->Kp*(pid->Err-pid->Err_last) + pid->Ki*pid->Err + pid->Kd*(pid->Err-2*pid->Err_last+pid->Err_last_last);
	
	// 输出限幅
	if(pid->Output>10.0f)
		pid->Output = 10.0f;
	else if(pid->Output<-10.0f)
		pid->Output = -10.0f;
	
	//滤波
	if(FILTER)
	{
		pid->Output = Filer(pid->Output,pid->Output_last);
	}
	
	pid->Output_last = pid->Output;
	pid->Err_last_last = pid->Err_last;
	pid->Err_last = pid->Err;
	
	return pid->Output;
}

// PID积分值清零
void PID_Pos_Clear_Value(PID_Postype_Structure *pid)
{
	pid->Integral = 0;
	pid->Err_last = 0;
}

void PID_Inc_Clear_Value(PID_Inctype_Structure *pid)
{
	pid->Output_last = 0;
	pid->Err_last = 0;
	pid->Err_last_last = 0;
}

// 对pid输出进行滤波，避免pid输出高频控制信号（噪声）
float Filer(float current, float last)
{
	float par = 0.7l;
	float out;
	out = par*current+(1-par)*last;
	return out;
}





