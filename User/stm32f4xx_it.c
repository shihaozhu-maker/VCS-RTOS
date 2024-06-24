/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
//#include "main.h"

void Interrupt_Init(void)
{
	// FreeRTOS要求所有的位都需要是抢占优先级
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
}

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */

// SVC中断用于进入第一个任务，在port.c文件中实现了vPortSVCHandler
//void SVC_Handler(void)
//{
//}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
// SVC中断用于任务切换，在port.c文件中实现了xPortPendSVHandler
//void PendSV_Handler(void)
//{
//}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
/**********************************************************/
/*from main.h*/
//static __IO uint32_t uwTimingDelay;

//void TimingDelay_Decrement(void)
//{
//  if (uwTimingDelay != 0x00)
//  { 
//    uwTimingDelay--;
//  }
//}
/**********************************************************/
#include "FreeRTOS.h"
void xPortSysTickHandler(void);

void SysTick_Handler(void)
{
		#if (INCLUDE_xTaskGetSchedulerState  == 1 )
      if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
      {
    #endif  /* INCLUDE_xTaskGetSchedulerState */  
	
        xPortSysTickHandler();
	
    #if (INCLUDE_xTaskGetSchedulerState  == 1 )
      }
    #endif  /* INCLUDE_xTaskGetSchedulerState */
	
  //TimingDelay_Decrement();
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/**************************************  EXTI4_IRQHandler KEY11 优先级 2 0 **************************************/
#include "key.h"
#include "event_groups.h"
extern EventGroupHandle_t KEY_EventGroup;
void EXTI4_IRQHandler(void)
{
	delay_ms(10);
	BaseType_t xHigherPriorityTaskWoken, xResult=pdFAIL;
	if(Key_Is_Pressed(1))
	{
		xHigherPriorityTaskWoken = pdFALSE; 
		xResult = xEventGroupSetBitsFromISR(KEY_EventGroup, KEY11_Event, &xHigherPriorityTaskWoken);
	}
	EXTI_ClearITPendingBit(EXTI_Line4);
	if(xResult != pdFAIL)
		portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

/**************************************  EXTI0_IRQHandler KEY13 优先级 2 0 **************************************/
void EXTI0_IRQHandler(void)
{
	delay_ms(10);
	BaseType_t xHigherPriorityTaskWoken, xResult=pdFAIL;
	if(Key_Is_Pressed(3))
	{
		xHigherPriorityTaskWoken = pdFALSE; 
		xResult = xEventGroupSetBitsFromISR(KEY_EventGroup, KEY13_Event, &xHigherPriorityTaskWoken);
	}
	EXTI_ClearITPendingBit(EXTI_Line0);
	if(xResult != pdFAIL)
		portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

/**************************************  EXTI1_IRQHandler KEY14 优先级 2 0 **************************************/
void EXTI1_IRQHandler(void)
{
	delay_ms(10);
	BaseType_t xHigherPriorityTaskWoken, xResult=pdFAIL;
	if(Key_Is_Pressed(4))
	{
		xHigherPriorityTaskWoken = pdFALSE; 
		xResult = xEventGroupSetBitsFromISR(KEY_EventGroup, KEY14_Event, &xHigherPriorityTaskWoken);
	}
	EXTI_ClearITPendingBit(EXTI_Line1);
	if(xResult != pdFAIL)
		portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

/**************************************  EXTI9_5_IRQHandler KEY12 KEY21 KEY22 KEY23 KEY24 优先级 2 0  **************************************/
void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line5))
	{
		delay_ms(10);
		BaseType_t xHigherPriorityTaskWoken, xResult=pdFAIL;
		if(Key_Is_Pressed(2))
		{
			xHigherPriorityTaskWoken = pdFALSE; 
			xResult = xEventGroupSetBitsFromISR(KEY_EventGroup, KEY12_Event, &xHigherPriorityTaskWoken);
		}
		EXTI_ClearITPendingBit(EXTI_Line5);
		if(xResult != pdFAIL)
			portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
	}
	else if(EXTI_GetITStatus(EXTI_Line6))
	{
		delay_ms(10);
		BaseType_t xHigherPriorityTaskWoken, xResult=pdFAIL;
		if(Key_Is_Pressed(5))
		{
			xHigherPriorityTaskWoken = pdFALSE; 
			xResult = xEventGroupSetBitsFromISR(KEY_EventGroup, KEY21_Event, &xHigherPriorityTaskWoken);
		}
		EXTI_ClearITPendingBit(EXTI_Line6);
		if(xResult != pdFAIL)
			portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
	}
	else if(EXTI_GetITStatus(EXTI_Line7))
	{
		delay_ms(10);
		BaseType_t xHigherPriorityTaskWoken, xResult=pdFAIL;
		if(Key_Is_Pressed(6))
		{
			xHigherPriorityTaskWoken = pdFALSE; 
			xResult = xEventGroupSetBitsFromISR(KEY_EventGroup, KEY22_Event, &xHigherPriorityTaskWoken);
		}
		EXTI_ClearITPendingBit(EXTI_Line7);
		if(xResult != pdFAIL)
			portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
	}
	else if(EXTI_GetITStatus(EXTI_Line8))
	{
		delay_ms(10);
		BaseType_t xHigherPriorityTaskWoken, xResult=pdFAIL;
		if(Key_Is_Pressed(7))
		{
			xHigherPriorityTaskWoken = pdFALSE; 
			xResult = xEventGroupSetBitsFromISR(KEY_EventGroup, KEY23_Event, &xHigherPriorityTaskWoken);
		}
		EXTI_ClearITPendingBit(EXTI_Line8);
		if(xResult != pdFAIL)
			portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
	}
	else if(EXTI_GetITStatus(EXTI_Line9))
	{
		delay_ms(10);
		BaseType_t xHigherPriorityTaskWoken, xResult=pdFAIL;
		if(Key_Is_Pressed(8))
		{
			xHigherPriorityTaskWoken = pdFALSE; 
			xResult = xEventGroupSetBitsFromISR(KEY_EventGroup, KEY24_Event, &xHigherPriorityTaskWoken);
		}
		EXTI_ClearITPendingBit(EXTI_Line9);
		if(xResult != pdFAIL)
				portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
	}
}

/**************************************  TIM2_IRQHandler 控制输出 优先级 0 0  **************************************/
#include "driver.h"
#include "canopen.h"
#include "pid.h"
#include "encoder.h"
#include "usart.h"

extern Driver_Struct Driver1;
extern Driver_Struct Driver2;
extern Delay_Feedback_Struct Delay_Feedback_Controller1;
extern Delay_Feedback_Struct Delay_Feedback_Controller2;
extern Options_Struct Option;
extern Encoder_Struct Encoder1;
extern Encoder_Struct Encoder2;
extern PID_Postype_Structure pid1; 
extern PID_Postype_Structure pid2; 
extern PID_Inctype_Structure pid_inc1;
extern PID_Inctype_Structure pid_inc2;

// 当前最新更新的输出电压值
float32_t current_voltage1;
float32_t current_voltage2;
float32_t Feedback_Output1;
float32_t Feedback_Output2;

float32_t Current_DisTarget1;
float32_t Current_DisTarget2;

float32_t pid_output1;
float32_t pid_output2;

float32_t Current_Output1;
float32_t Current_Output2;

uint8_t chirp_update_flag1 = 0;
uint8_t chirp_update_flag2 = 0;

uint8_t chirp_out_flag1 = 0;
uint8_t chirp_out_flag2 = 0;

void TIM2_IRQHandler(void)
{
	BaseType_t xHigherPriorityTaskWoken1, xResult1=pdFAIL;
	BaseType_t xHigherPriorityTaskWoken2, xResult2=pdFAIL;
	
	extern TaskHandle_t Chairp_Task_Handle;
	
	// 更新模拟输出 0.5ms执行一次
	if(TIM_GetFlagStatus(DRIVER_TIMER, TIM_FLAG_Update))
	{
		// 计算时滞反馈数据
		if(Delay_Feedback_Controller1.status==1)
			Feedback_Output1 = Calculat_Feedback_Voltage(&Delay_Feedback_Controller1);
		else
			Feedback_Output1 = 0;
		
		if(Delay_Feedback_Controller2.status==1)
			Feedback_Output2 = Calculat_Feedback_Voltage(&Delay_Feedback_Controller2);
		else
			Feedback_Output2 = 0;
		
		
		if(POSITIVE_FEEDBACK_1)
			;
		else
			Feedback_Output1 = -Feedback_Output1;
		if(POSITIVE_FEEDBACK_2)
			;
		else
			Feedback_Output2 = -Feedback_Output2;
				
			
		/********Driver1********/
		if(Driver1.Status == 1)
		{
			current_voltage1 = Driver1.Output_Voltage-Feedback_Output1;
			if((Option.Driver_CAN_Out&0x02)==0)
				Driver_Voltage_Output(&Driver1, current_voltage1);
			else
				Target_Torque_Set(current_voltage1*3,1);
			
			Current_Output1 = current_voltage1;
		}
		else if(Driver1.Status == 2)
		{
			if(Driver1.Sinwave->current_data_table == 1)
			{
				current_voltage1 = Driver1.Sinwave->Sinwave_data1[1][Driver1.Sinwave->current_sindata_index+1]-Feedback_Output1;
				if((Option.Driver_CAN_Out&0x02)==0)
					Driver_Voltage_Output(&Driver1, current_voltage1);
				else
					Target_Torque_Set((current_voltage1)*3,1);
			}
			else if(Driver1.Sinwave->current_data_table == 2)
			{
				current_voltage1 = Driver1.Sinwave->Sinwave_data2[1][Driver1.Sinwave->current_sindata_index+1]-Feedback_Output1;
				if((Option.Driver_CAN_Out&0x02)==0)
					Driver_Voltage_Output(&Driver1, current_voltage1);
				else
					Target_Torque_Set((current_voltage1)*3,1);
			}
			
			Driver1.Sinwave->current_sindata_index ++;
			if(Driver1.Sinwave->current_sindata_index>=Driver1.Sinwave->sindata_number)
				Driver1.Sinwave->current_sindata_index=0;
			
			Current_Output1 = current_voltage1;
		}
		else if(Driver1.Status == 3)
		{
			Current_DisTarget1 = Driver1.Target_Displacement;
			Get_Encoder_Value();		// 获取最新的位置数据
			//pid_output1 = PID_Postype_Calculate(Current_DisTarget1, Encoder1.Distance/1000.0f, &pid1);
			pid_output1 = PID_Inctype_Calculate(Current_DisTarget1, Encoder1.Distance/1000.0f, &pid_inc1);		// 增量式
			if((Option.Driver_CAN_Out&0x02)!=0)
				Target_Torque_Set((-pid_output1)*6,1);
			else
				Driver_Voltage_Output(&Driver1, (-pid_output1)*2);
			
			Current_Output1 = Current_DisTarget1;
		}
		else if(Driver1.Status == 4)
		{
			if(Driver1.Sinwave->current_data_table == 1)
			{
				Current_DisTarget1 = Driver1.Sinwave->Sinwave_data1[1][Driver1.Sinwave->current_sindata_index+1];
				Get_Encoder_Value();
				// pid_output1 = PID_Postype_Calculate(Current_DisTarget1, Encoder1.Distance/1000.0f, &pid1);
				pid_output1 = PID_Inctype_Calculate(Current_DisTarget1, Encoder1.Distance/1000.0f, &pid_inc1);		// 增量式
				if((Option.Driver_CAN_Out&0x02)!=0)
					Target_Torque_Set((-pid_output1)*6,1);
				else
					Driver_Voltage_Output(&Driver1, (-pid_output1)*2);
			}
			else if(Driver1.Sinwave->current_data_table == 2)
			{
				Current_DisTarget1 = Driver1.Sinwave->Sinwave_data2[1][Driver1.Sinwave->current_sindata_index+1];
				Get_Encoder_Value();
				// pid_output1 = PID_Postype_Calculate(Current_DisTarget1, Encoder1.Distance/1000.0f, &pid1);
				pid_output1 = PID_Inctype_Calculate(Current_DisTarget1, Encoder1.Distance/1000.0f, &pid_inc1);		// 增量式
				if((Option.Driver_CAN_Out&0x02)!=0)
					Target_Torque_Set((-pid_output1)*6,1);
				else
					Driver_Voltage_Output(&Driver1, (-pid_output1)*2);
			}
			
			Driver1.Sinwave->current_sindata_index ++;
			if(Driver1.Sinwave->current_sindata_index>=Driver1.Sinwave->sindata_number)
				Driver1.Sinwave->current_sindata_index=0;
			
			Current_Output1 = Current_DisTarget1;
		}
		else if(Driver1.Status == 5)
		{
			if((Option.Driver_CAN_Out&0x02)==0)
				Driver_Voltage_Output(&Driver1, 0);
			else
				Target_Torque_Set(0,1);
			Current_Output1 = 0;
			pid_output1 = 0;
		}
		else if((Driver1.Status == 6) && (Driver1.Sweep_wave->output_enable == 1))		// 电压chirp模式
		{
			chirp_out_flag1++;
			if(chirp_out_flag1==SWEEP_UPDATE_TIME)
			{
				uint16_t data_num;
					
				if(Driver1.Sinwave->current_data_table == 1)
				{
					data_num = Driver1.Sinwave->Sinwave_data1[0][0];
					// 二维数组全部用来存放数据
					if(Driver1.Sinwave->current_sindata_index<data_num)
					{
						if(Driver1.Sweep_wave->mode_option==1)
							current_voltage1 = Driver1.Sinwave->Sinwave_data1[0][Driver1.Sinwave->current_sindata_index+1];
						else if(Driver1.Sweep_wave->mode_option==2)
						{
							Get_Encoder_Value();
							pid_output1 = PID_Inctype_Calculate(Driver1.Sinwave->Sinwave_data1[0][Driver1.Sinwave->current_sindata_index+1], Encoder1.Distance/1000.0f, &pid_inc1);
							current_voltage1 = (-pid_output1)*2;
						}
					}
					else
					{
						if(Driver1.Sweep_wave->mode_option==1)
							current_voltage1 = Driver1.Sinwave->Sinwave_data1[1][Driver1.Sinwave->current_sindata_index-data_num+1];
						else if(Driver1.Sweep_wave->mode_option==2)
						{
							Get_Encoder_Value();
							pid_output1 = PID_Inctype_Calculate(Driver1.Sinwave->Sinwave_data1[1][Driver1.Sinwave->current_sindata_index-data_num+1], Encoder1.Distance/1000.0f, &pid_inc1);
							current_voltage1 = (-pid_output1)*2;
						}
					}
					
					if((Option.Driver_CAN_Out&0x02)==0)
						Driver_Voltage_Output(&Driver1, current_voltage1);
					else
						Target_Torque_Set((current_voltage1)*3,1);
				}
				else if(Driver1.Sinwave->current_data_table == 2)
				{
					data_num = Driver1.Sinwave->Sinwave_data2[0][0];
					if(Driver1.Sinwave->current_sindata_index<data_num)
					{
						if(Driver1.Sweep_wave->mode_option == 1)
							current_voltage1 = Driver1.Sinwave->Sinwave_data2[0][Driver1.Sinwave->current_sindata_index+1];
						else if(Driver1.Sweep_wave->mode_option == 2)
						{
							Get_Encoder_Value();
							pid_output1 = PID_Inctype_Calculate(Driver1.Sinwave->Sinwave_data2[0][Driver1.Sinwave->current_sindata_index+1], Encoder1.Distance/1000.0f, &pid_inc1);
							current_voltage1 = (-pid_output1)*2;
						}
					}
					else
					{
						if(Driver1.Sweep_wave->mode_option == 1)
							current_voltage1 = Driver1.Sinwave->Sinwave_data2[1][Driver1.Sinwave->current_sindata_index-data_num+1];
						else if(Driver1.Sweep_wave->mode_option == 2)
						{
							Get_Encoder_Value();
							pid_output1 = PID_Inctype_Calculate(Driver1.Sinwave->Sinwave_data2[1][Driver1.Sinwave->current_sindata_index-data_num+1], Encoder1.Distance/1000.0f, &pid_inc1);
							current_voltage1 = (-pid_output1)*2;
						}
					}
					
					if((Option.Driver_CAN_Out&0x02)==0)
						Driver_Voltage_Output(&Driver1, current_voltage1);
					else
						Target_Torque_Set((current_voltage1)*3,1);
				}
				Driver1.Sinwave->current_sindata_index ++;
				
				// 只输出了Sinwave_datax[0]，但是已经完成了输出
				if(Driver1.Sinwave->current_sindata_index==data_num)
				{
					if(Driver1.Sinwave->current_data_table == 1)
					{
						if(Driver1.Sinwave->Sinwave_data1[1][0] == Driver1.Sweep_wave->time || Driver1.Sinwave->Sinwave_data1[1][0] == Driver1.Sweep_wave->time + ((Driver1.Sweep_wave->time)%2 + Driver1.Sweep_wave->interval_time))
							Driver1.Sinwave->current_sindata_index=2*data_num;
					}
					else if(Driver1.Sinwave->current_data_table == 2)
					{
						if(Driver1.Sinwave->Sinwave_data2[1][0] == Driver1.Sweep_wave->time || Driver1.Sinwave->Sinwave_data2[1][0] == Driver1.Sweep_wave->time + ((Driver1.Sweep_wave->time)%2 + Driver1.Sweep_wave->interval_time))
							Driver1.Sinwave->current_sindata_index=2*data_num;
					}
				}
					
				if(Driver1.Sinwave->current_sindata_index>=2*data_num)
				{
					//while(chirp_update_flag1);		// chirp数据没有完成更新
					
					if(Driver1.Sinwave->current_data_table==1)
						Driver1.Sinwave->current_data_table=2;
					else
						Driver1.Sinwave->current_data_table=1;
					
					Driver1.Sinwave->current_sindata_index=0;
					
					chirp_update_flag1 = 1; 	// 当前的chirp数据已用完，需要更新新的数据
					
					xHigherPriorityTaskWoken1 = pdFALSE; 
					xResult1 = xTaskNotifyFromISR(Chairp_Task_Handle, 1, eSetBits , &xHigherPriorityTaskWoken1);
				}
				
				Current_Output1 = current_voltage1;
				chirp_out_flag1=0;
			}
		}
		
		/********Driver2********/
		if(Driver2.Status == 1)
		{
			current_voltage2 = Driver2.Output_Voltage-Feedback_Output2;
			if((Option.Driver_CAN_Out&0x01)==0)
				Driver_Voltage_Output(&Driver2, current_voltage2);
			else
				Target_Torque_Set(current_voltage2*3,2);
			
			Current_Output2 = current_voltage2;
		}
		else if(Driver2.Status == 2)
		{
			if(Driver2.Sinwave->current_data_table == 1)
			{
				current_voltage2 = Driver2.Sinwave->Sinwave_data1[1][Driver2.Sinwave->current_sindata_index+1]-Feedback_Output2;
				if((Option.Driver_CAN_Out&0x01)==0)
					Driver_Voltage_Output(&Driver2, current_voltage2);
				else
					Target_Torque_Set(current_voltage2*3,2);
			}		
			else if(Driver2.Sinwave->current_data_table == 2)
			{
				current_voltage2 = Driver2.Sinwave->Sinwave_data2[1][Driver2.Sinwave->current_sindata_index+1]-Feedback_Output2;
				if((Option.Driver_CAN_Out&0x01)==0)
					Driver_Voltage_Output(&Driver2, current_voltage2);
				else
					Target_Torque_Set(current_voltage2*3,2);
			}
			
			Driver2.Sinwave->current_sindata_index ++;
			if(Driver2.Sinwave->current_sindata_index>=Driver2.Sinwave->sindata_number)
				Driver2.Sinwave->current_sindata_index=0;
			
			Current_Output2 = current_voltage2;
		}
		else if(Driver2.Status == 3)
		{
			Current_DisTarget2 = Driver2.Target_Displacement;
			
			Get_Encoder_Value();
//				pid_output2 = PID_Postype_Calculate(Current_DisTarget2, Encoder2.Distance/1000.0f, &pid2);
			pid_output2 = PID_Inctype_Calculate(Current_DisTarget2, Encoder2.Distance/1000.0f, &pid_inc2);		// 增量式PID
			
			if((Option.Driver_CAN_Out&0x01)!=0)
				Target_Torque_Set((-pid_output2)*6,2);
			else
				Driver_Voltage_Output(&Driver2, (-pid_output2)*2);
			
			Current_Output2 = Current_DisTarget2;
		}
		else if(Driver2.Status == 4)
		{
			if(Driver2.Sinwave->current_data_table == 1)
			{
				Current_DisTarget2 = Driver2.Sinwave->Sinwave_data1[1][Driver2.Sinwave->current_sindata_index+1];
				Get_Encoder_Value();
//					pid_output2 = PID_Postype_Calculate(Current_DisTarget2, Encoder2.Distance/1000.0f, &pid2);
				pid_output2 = PID_Inctype_Calculate(Current_DisTarget2, Encoder2.Distance/1000.0f, &pid_inc2);
				if((Option.Driver_CAN_Out&0x01)!=0)
					Target_Torque_Set((-pid_output2)*6,2);
				else
					Driver_Voltage_Output(&Driver2, (-pid_output2)*2);
			}
			else if(Driver2.Sinwave->current_data_table == 2)
			{
				Current_DisTarget2 = Driver2.Sinwave->Sinwave_data2[1][Driver2.Sinwave->current_sindata_index+1];
				Get_Encoder_Value();
//					pid_output2 = PID_Postype_Calculate(Current_DisTarget2, Encoder2.Distance/1000.0f, &pid2);
				pid_output2 = PID_Inctype_Calculate(Current_DisTarget2, Encoder2.Distance/1000.0f, &pid_inc2);
				if((Option.Driver_CAN_Out&0x01)!=0)
					Target_Torque_Set((-pid_output2)*6,2);
				else
					Driver_Voltage_Output(&Driver2, (-pid_output2)*2);
			}
			Driver2.Sinwave->current_sindata_index ++;
			if(Driver2.Sinwave->current_sindata_index>=Driver2.Sinwave->sindata_number)
				Driver2.Sinwave->current_sindata_index=0;
			
			Current_Output2 = Current_DisTarget2;
		}
		else if(Driver2.Status == 5)
		{
			if((Option.Driver_CAN_Out&0x01)==0)
				Driver_Voltage_Output(&Driver2, 0);
			else
				Target_Torque_Set(0,2);
			Current_Output2 = 0;
			pid_output2 = 0;
		}
		else if((Driver2.Status == 6) && (Driver2.Sweep_wave->output_enable == 1))		// 电压chirp模式
		{
			chirp_out_flag2++;
			if(chirp_out_flag2==SWEEP_UPDATE_TIME)
			{
				uint16_t data_num;
					
				if(Driver2.Sinwave->current_data_table == 1)
				{
					data_num = Driver2.Sinwave->Sinwave_data1[0][0];
					// 二维数组全部用来存放数据
					if(Driver2.Sinwave->current_sindata_index<data_num)
					{
						if(Driver2.Sweep_wave->mode_option == 1)
							current_voltage2 = Driver2.Sinwave->Sinwave_data1[0][Driver2.Sinwave->current_sindata_index+1];
						else if(Driver2.Sweep_wave->mode_option == 2)
						{
							Get_Encoder_Value();
							pid_output2 = PID_Inctype_Calculate(Driver2.Sinwave->Sinwave_data1[0][Driver2.Sinwave->current_sindata_index+1], Encoder1.Distance/1000.0f, &pid_inc2);
							current_voltage2 = (-pid_output2)*2;
						}
					}
					else
					{
						if(Driver2.Sweep_wave->mode_option == 1)
							current_voltage2 = Driver2.Sinwave->Sinwave_data1[1][Driver2.Sinwave->current_sindata_index-data_num+1];
						else if(Driver2.Sweep_wave->mode_option == 2)
						{
							Get_Encoder_Value();
							pid_output2 = PID_Inctype_Calculate(Driver2.Sinwave->Sinwave_data1[1][Driver2.Sinwave->current_sindata_index-data_num+1], Encoder1.Distance/1000.0f, &pid_inc2);
							current_voltage2 = (-pid_output2)*2;
						}
					}
					
					if((Option.Driver_CAN_Out&0x01)==0)
						Driver_Voltage_Output(&Driver2, current_voltage2);
					else
						Target_Torque_Set((current_voltage2)*3,2);
				}
				else if(Driver2.Sinwave->current_data_table == 2)
				{
					data_num = Driver2.Sinwave->Sinwave_data2[0][0];
					if(Driver2.Sinwave->current_sindata_index<data_num)
					{
						if(Driver2.Sweep_wave->mode_option == 1)
							current_voltage2 = Driver2.Sinwave->Sinwave_data2[0][Driver2.Sinwave->current_sindata_index+1];
						else if(Driver2.Sweep_wave->mode_option == 2)
						{
							Get_Encoder_Value();
							pid_output2 = PID_Inctype_Calculate(Driver2.Sinwave->Sinwave_data2[0][Driver2.Sinwave->current_sindata_index+1], Encoder1.Distance/1000.0f, &pid_inc2);
							current_voltage2 = (-pid_output2)*2;
						}
					}
					else
					{
						if(Driver2.Sweep_wave->mode_option == 1)
							current_voltage2 = Driver2.Sinwave->Sinwave_data2[1][Driver2.Sinwave->current_sindata_index-data_num+1];
						else if(Driver2.Sweep_wave->mode_option == 2)
						{
							Get_Encoder_Value();
							pid_output2 = PID_Inctype_Calculate(Driver2.Sinwave->Sinwave_data2[1][Driver2.Sinwave->current_sindata_index-data_num+1], Encoder1.Distance/1000.0f, &pid_inc2);
							current_voltage2 = (-pid_output2)*2;
						}
					}
					
					if((Option.Driver_CAN_Out&0x01)==0)
						Driver_Voltage_Output(&Driver2, current_voltage2);
					else
						Target_Torque_Set((current_voltage2)*3,2);
				}
				Driver2.Sinwave->current_sindata_index ++;
				
				// 只输出了Sinwave_datax[0]，但是已经完成了输出
				if(Driver2.Sinwave->current_sindata_index==data_num)
				{
					if(Driver2.Sinwave->current_data_table == 1)
					{
						if(Driver2.Sinwave->Sinwave_data1[1][0] == Driver2.Sweep_wave->time || Driver2.Sinwave->Sinwave_data1[1][0] == Driver2.Sweep_wave->time + ((Driver2.Sweep_wave->time)%2 + Driver2.Sweep_wave->interval_time))
							Driver2.Sinwave->current_sindata_index=2*data_num;
					}
					else if(Driver2.Sinwave->current_data_table == 2)
					{
						if(Driver2.Sinwave->Sinwave_data2[1][0] == Driver2.Sweep_wave->time || Driver2.Sinwave->Sinwave_data2[1][0] == Driver2.Sweep_wave->time + ((Driver2.Sweep_wave->time)%2 + Driver2.Sweep_wave->interval_time))
							Driver2.Sinwave->current_sindata_index=2*data_num;
					}
				}
					
				if(Driver2.Sinwave->current_sindata_index>=2*data_num)
				{
					//while(chirp_update_flag2);		// chirp数据没有完成更新
					
					if(Driver2.Sinwave->current_data_table==1)
						Driver2.Sinwave->current_data_table=2;
					else
						Driver2.Sinwave->current_data_table=1;
					
					Driver2.Sinwave->current_sindata_index=0;
					
					chirp_update_flag2 = 1; 	// 当前的chirp数据已用完，需要更新新的数据
					
					xHigherPriorityTaskWoken2 = pdFALSE; 
					xResult2 = xTaskNotifyFromISR(Chairp_Task_Handle, 2, eSetBits , &xHigherPriorityTaskWoken2);
				}
				
				Current_Output2 = current_voltage2;
				chirp_out_flag2 = 0;
			}
		}
	}
	
	TIM_ClearFlag(DRIVER_TIMER, TIM_FLAG_Update);
	
	if(xResult1 != pdFAIL)
						portYIELD_FROM_ISR( xHigherPriorityTaskWoken1 );
	if(xResult2 != pdFAIL)
						portYIELD_FROM_ISR( xHigherPriorityTaskWoken2 );
}

/**************************************  TIM7_IRQHandler 更新历史数组 优先级 0 1  **************************************/
float pid_output;

void TIM7_IRQHandler(void)
{
	if(TIM_GetITStatus(HISTORY_UPDATE_TIMER, TIM_IT_Update))
	{
		switch(Option.DRC1_DataSource_Option)
		{
			case 0:
			{
				Distance_History_Update(&Delay_Feedback_Controller1, 0);
				break;
			}
			case 1:
			{
				Distance_History_Update(&Delay_Feedback_Controller1, 0);
				break;
			}
			case 2:
			{
				Get_Driver_Encoder_Value(&Driver2);
				Distance_History_Update(&Delay_Feedback_Controller1, Driver2.Distance);
				break;
			}
			case 3:
			{
				Get_Encoder_Value();
				Distance_History_Update(&Delay_Feedback_Controller1, Encoder1.Distance);
				break;
			}
			case 4:
			{
				Get_Encoder_Value();
				Distance_History_Update(&Delay_Feedback_Controller1, Encoder2.Distance);
				break;
			}
		}
		
		switch(Option.DRC2_DataSource_Option)
		{
			case 0:
			{
				Distance_History_Update(&Delay_Feedback_Controller2, 0);
				break;
			}
			case 1:
			{
				Get_Driver_Encoder_Value(&Driver1);
				Distance_History_Update(&Delay_Feedback_Controller2, Driver1.Distance);
				break;
			}
			case 2:
			{
				Distance_History_Update(&Delay_Feedback_Controller2, 0);
				break;
			}
			case 3:
			{
				Get_Encoder_Value();
				Distance_History_Update(&Delay_Feedback_Controller2, Encoder1.Distance);
				break;
			}
			case 4:
			{
				Get_Encoder_Value();
				Distance_History_Update(&Delay_Feedback_Controller2, Encoder2.Distance);
				break;
			}
		}
		TIM_ClearITPendingBit(HISTORY_UPDATE_TIMER, TIM_IT_Update);
	}
}

extern uint8_t SDO_Rxbuffer[9];

/**************************************  CAN1_RX0_IRQHandler CAN接收中断 优先级 1 0  **************************************/
void CAN1_RX0_IRQHandler(void)
{
	if(CAN_GetITStatus(CANOPEN_CAN, CAN_IT_FMP0))		// 从fifo读出数据后，FMP0会自动减小，因此不需要清除中断标志位
	{
		CanRxMsg RxMessage;
		CAN_Receive(CANOPEN_CAN, CAN_FIFO0, &RxMessage);
		
		
		for(int i=0;i<2;i++)
			Usart_SendByte(USART_, (RxMessage.StdId>>(1-i)*8)&0xff);		// 发送ID
		
		for(int i=0;i<RxMessage.DLC;i++)
			Usart_SendByte(USART_, RxMessage.Data[i]);		// CAN数据通过串口发送至上位机
		
		
		SDO_Rxbuffer[0] = 1;		// 接收标志
		for(int i=1; i<=RxMessage.DLC;i++)
			SDO_Rxbuffer[i] = RxMessage.Data[i-1];
		for(int i=RxMessage.DLC+1; i<=8;i++)		// 空余位填充0
			SDO_Rxbuffer[i] = 0;
	}
}

/**************************************  USART2_IRQHandler USART接收中断 优先级 1 0  **************************************/
uint8_t usart_queue_data[16] = {0};		// [0] 存放数据长度
#include "queue.h"
#include "task.h"
extern QueueHandle_t USART_RX_Queue;

void USART2_IRQHandler(void)
{
	if(USART_GetFlagStatus(USART_, USART_FLAG_RXNE))
	{
		uint8_t ucTemp;
		if(USART_GetITStatus(USART_,USART_IT_RXNE)!=RESET)
		{
			ucTemp = USART_ReceiveData(USART_);
			usart_queue_data[0]++;
			usart_queue_data[usart_queue_data[0]] = ucTemp;
			if(ucTemp=='E')
			{
				BaseType_t xHigherPriorityTaskWoken;
				xHigherPriorityTaskWoken = pdFALSE;
				xQueueSendFromISR(USART_RX_Queue, usart_queue_data, &xHigherPriorityTaskWoken);
				usart_queue_data[0]=0;
				if(xHigherPriorityTaskWoken)
						portYIELD_FROM_ISR( xHigherPriorityTaskWoken);
						//taskYIELD_FROM_ISR(); 
			} 
		}
	}
}


void SPI2_IRQHandler(void)
{
	if (SPI_I2S_GetITStatus(SPI2, SPI_I2S_IT_RXNE) == SET)
	{
		uint16_t data = SPI_I2S_ReceiveData(SPI2);		// 读数据清除中断标志
//		printf("data from spi2 = %d\n",data);
		
		Get_Encoder_Value();
		SPI_I2S_SendData(SPI2, Encoder2.Distance);
		printf("%f\n",(float32_t)Encoder2.Distance/1000.0f);
		
//		while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) == SET);
//		SPI_I2S_SendData(SPI2, (char)(distance>>8));
//		printf("data[0] = %d\n",(char)(distance>>8));
		
//		while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) == SET);
//		SPI_I2S_SendData(SPI2, (char)distance);
//		printf("data[1] = %d\n",(char)distance);
		
	}

}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
