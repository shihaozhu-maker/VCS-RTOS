#include "key.h"

KEY_Struct Key = {
	.Key11 = 0,
	.Key12 = 0,
	.Key13 = 0,
	.Key14 = 0,
	.Key21 = 0,
	.Key22 = 0,
	.Key23 = 0,
	.Key24 = 0
};

/*
 * Top_Device
 * 0		无选中设备
 * 1		Driver1
 * 2		Driver2
 * 3		Encoder1
 * 4		Encoder2
 * 5		Usart
 * 6		CAN
 * 7		保留
 * 8		保留
 * 9		DRC1
 * 10		DRC1_Config
 * 11		DRC2
 * 12		DRC2_Config
 * 13		Sweep1
 * 14		Sweep1_Config
 * 15		Sweep2
 * 16		Sweep2_Config
*/
volatile uint8_t Top_Device = 0;

/*
 * Encoder_Data_Update
 * 0		none
 * 1		update
*/
uint8_t Encoder_Data_Update = 0;


volatile Options_Struct Option = 
{
	.Normal_Option = 0,
	.Usart_DataTx_Option = 0,
	.DRC1_DataSource_Option = 0,
	.DRC2_DataSource_Option = 0,
	
	.Driver_Current_Status = 0,
	.Driver_CAN_Out = 0,
	.Sweep1_Mode = 1,
	.Sweep2_Mode = 1
};


void Key_Init(void)
{
	// GPIO初始化
	RCC_AHB1PeriphClockCmd(KEY11_GPIO_RCC|KEY12_GPIO_RCC|KEY13_GPIO_RCC|KEY14_GPIO_RCC|
												 KEY21_GPIO_RCC|KEY22_GPIO_RCC|KEY23_GPIO_RCC|KEY24_GPIO_RCC, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Pin = KEY11_GPIO_PIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_Init(KEY11_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = KEY12_GPIO_PIN;
	GPIO_Init(KEY12_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = KEY13_GPIO_PIN;
	GPIO_Init(KEY13_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = KEY14_GPIO_PIN;
	GPIO_Init(KEY14_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = KEY21_GPIO_PIN;
	GPIO_Init(KEY21_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = KEY22_GPIO_PIN;
	GPIO_Init(KEY22_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = KEY23_GPIO_PIN;
	GPIO_Init(KEY23_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = KEY24_GPIO_PIN;
	GPIO_Init(KEY24_GPIO_PORT, &GPIO_InitStructure);
	
	// 中断配置
	// NVIC
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 15;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_Init(&NVIC_InitStructure);
	
	// SYSCFG 中断线映射
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource4);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource5);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource0);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource1);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource6);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource7);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource8);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource9);
	
	// EXTI初始化
	EXTI_ClearITPendingBit(EXTI_Line4|EXTI_Line5|EXTI_Line0|EXTI_Line1|EXTI_Line6|EXTI_Line7|EXTI_Line8|EXTI_Line9);
	
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line4|EXTI_Line5|EXTI_Line0|EXTI_Line1|EXTI_Line6|EXTI_Line7|EXTI_Line8|EXTI_Line9;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);
}

// 按下返回1 未按下返回0
uint8_t Key_Is_Pressed(uint8_t key_number)
{
	uint8_t status  = 0;
	switch(key_number)
	{
		case 1:
		{
			if(!GPIO_ReadInputDataBit(KEY11_GPIO_PORT, KEY11_GPIO_PIN))
				status = 1;
			break;
		}
		case 2:
		{
			if(!GPIO_ReadInputDataBit(KEY12_GPIO_PORT, KEY12_GPIO_PIN))
				status = 1;
			break;
		}
		case 3:
		{
			if(!GPIO_ReadInputDataBit(KEY13_GPIO_PORT, KEY13_GPIO_PIN))
				status = 1;
			break;
		}
		case 4:
		{
			if(!GPIO_ReadInputDataBit(KEY14_GPIO_PORT, KEY14_GPIO_PIN))
				status = 1;
			break;
		}
		case 5:
		{
			if(!GPIO_ReadInputDataBit(KEY21_GPIO_PORT, KEY21_GPIO_PIN))
				status = 1;
			break;
		}
		case 6:
		{
			if(!GPIO_ReadInputDataBit(KEY22_GPIO_PORT, KEY22_GPIO_PIN))
				status = 1;
			break;
		}
		case 7:
		{
			if(!GPIO_ReadInputDataBit(KEY23_GPIO_PORT, KEY23_GPIO_PIN))
				status = 1;
			break;
		}
		case 8:
		{
			if(!GPIO_ReadInputDataBit(KEY24_GPIO_PORT, KEY24_GPIO_PIN))
				status = 1;
			break;
		}
		default:
			break;
	}
	return status;
}


extern Driver_Struct Driver1;
extern Driver_Struct Driver2;
extern Encoder_Struct Encoder1;
extern Encoder_Struct Encoder2;
extern Delay_Feedback_Struct Delay_Feedback_Controller1;
extern Delay_Feedback_Struct Delay_Feedback_Controller2;
extern Canopen_Structure Canopen;
extern PID_Postype_Structure pid1; 
extern PID_Postype_Structure pid2; 
extern PID_Inctype_Structure pid_inc1;

extern char Device_List[8][128];
extern char Driver1_AN[8][128];
extern char Driver1_SIN[8][128];
extern char Driver1_EN[8][128];
extern char Driver2_AN[8][128];
extern char Driver2_SIN[8][128];
extern char Driver2_EN[8][128];
extern char Encoder1_vm[8][128];
extern char Encoder2_vm[8][128];
extern Font Square_Brackets;

int before_canout1 = 0, before_canout2 = 0;


//  按键功能定义
void Key11_Function(void)
{
	Encoder_Data_Update = 0;
	if(Top_Device==0)
		Top_Device=1;
	else if(Top_Device<5)
		Top_Device=5;
	else if(Top_Device<9)
		Top_Device=9;
	else if(Top_Device<11)
		Top_Device=11;
	else if(Top_Device<13)
		Top_Device=13;
	else if(Top_Device<15)
		Top_Device=15;
	else
		Top_Device=0;
	
	Option.Normal_Option = 0;
	
	if((Top_Device==3)||(Top_Device==4))
		Encoder_Data_Update = 1;
}
void Key12_Function(void)
{
	if(Top_Device == 1)
	{
		switch(Driver1.Status)
		{
			case 1:
			{
				Return_to_AN_SIN_Mode(&Driver1);
				Driver_Sinwave_Outout_Start(&Driver1);		// 切换到模拟正弦模式
				Option.Normal_Option = 0;
				break;
			}
			case 2:
			{
				// 位置模式下需要采用模拟输出模式
				before_canout1=(Option.Driver_CAN_Out & 0x02);
				if(before_canout1)
				{
					AI_Mode_Config(3, 1);
					Option.Driver_CAN_Out &= (~0x02);
				}
				
				PID_Pos_Clear_Value(&pid1);
				Driver1.Status = 3;												// 切换到位置控制模式
				Driver1.Sinwave->current_sindata_index = 0;
				Option.Normal_Option = 0;
				break;
			}
			case 3:
			{
				Return_to_DIS_SIN_Mode(&Driver1);
				PID_Pos_Clear_Value(&pid1);
				Driver1.Status = 4;												// 切换到位置正弦模式
				Option.Normal_Option = 0;
				break;
			}
			case 4:
			{						
				if(before_canout1)
				{
					AI_Mode_Config(0, 1);
					Option.Driver_CAN_Out |= 0x02;
				}
				
				Driver_Encoder_Start(&Driver1);						// 切换到编码器模式
				Option.Normal_Option = 0;
				break;
			}
			case 5:
			{
				Driver1.Status = 1;												// 切换到AN模式
				Driver_Voltage_Output(&Driver1, Driver1.Output_Voltage);
				Option.Normal_Option = 0;
			}
		}
	}
	else if(Top_Device == 2)
	{
		switch(Driver2.Status)
		{
			case 1:
			{
				Return_to_AN_SIN_Mode(&Driver2);
				Driver_Sinwave_Outout_Start(&Driver2);		// 切换到正弦模式
				Option.Normal_Option = 0;
				break;
			}
			case 2:
			{
				// 位置模式下需要采用模拟输出模式
				before_canout2=(Option.Driver_CAN_Out & 0x01);
				if(before_canout2)
				{
					AI_Mode_Config(3, 2);
					Option.Driver_CAN_Out &= (~0x01);
				}
				
				PID_Inc_Clear_Value(&pid_inc1);
				Driver2.Status = 3;												// 切换到位置控制模式
				Driver2.Sinwave->current_sindata_index = 0;
				Option.Normal_Option = 0;
				break;
			}
			case 3:
			{
				Return_to_DIS_SIN_Mode(&Driver2);
				PID_Inc_Clear_Value(&pid_inc1);
				Driver2.Status = 4;												// 切换到正弦模式
				Option.Normal_Option = 0;
				break;
			}
			case 4:
			{
				if(before_canout2)
				{
					AI_Mode_Config(0, 2);
					Option.Driver_CAN_Out |= 0x01;
				}
				
				Driver_Encoder_Start(&Driver2);						// 切换到编码器模式
				Option.Normal_Option = 0;
				break;
			}
			case 5:
			{
				Driver2.Status = 1;												// 切换到AN模式
				Driver_Voltage_Output(&Driver2, Driver2.Output_Voltage);
				Option.Normal_Option = 0;
			}
		}
	}
	else if(Top_Device == 6)
	{
		switch(Canopen.Status)
		{
			case 1:
			{
				Canopen.Status = 2;
				break;
			}
			case 2:
			{
				Canopen.Status = 1;
				break;
			}
		}
	}
	else if((Top_Device == 9)||(Top_Device == 10))
	{
		switch(Delay_Feedback_Controller1.status)
		{
			case 0:
			{
				Delay_Feedback_Controller1.status=1;
				break;
			}
			case 1:
			{
				Delay_Feedback_Controller1.status=0;
				break;
			}
		}
	}
	else if((Top_Device == 11)||(Top_Device == 12))
	{
		switch(Delay_Feedback_Controller2.status)
		{
			case 0:
			{
				Delay_Feedback_Controller2.status=1;
				break;
			}
			case 1:
			{
				Delay_Feedback_Controller2.status=0;
				break;
			}
		}
	}
	else if((Top_Device == 13)||(Top_Device == 14))
	{
		switch(Driver1.Sweep_wave->output_enable)
		{
			case 0:
			{
				Driver1.Sweep_wave->output_enable=1;
				Return_to_Chirp_Mode(&Driver1);
				Option.Normal_Option = 0;
				Driver1.Status=6;
				break;
			}
			case 1:
			{
				Driver1.Sweep_wave->output_enable=0;
				Driver1.Status=1;
				Driver_Voltage_Output_0(&Driver1);
				break;
			}
		}
	}
	else if((Top_Device == 15)||(Top_Device == 16))
	{
		switch(Driver2.Sweep_wave->output_enable)
		{
			case 0:
			{
				Driver2.Sweep_wave->output_enable=1;
				Return_to_Chirp_Mode(&Driver2);
				Option.Normal_Option = 0;
				Driver2.Status=6;
				break;
			}
			case 1:
			{
				Driver2.Sweep_wave->output_enable=0;
				Driver2.Status=1;
				Driver_Voltage_Output_0(&Driver2);
				break;
			}
		}
	}
}

void Key13_Function(void)
{
	if(Top_Device==1)
	{
		if((Driver1.Status==1)||(Driver1.Status==2)||(Driver1.Status==3)||(Driver1.Status==4))
		{
			if(Option.Normal_Option==0)
			{
				Option.Normal_Option=1;
			}
			else if(Option.Normal_Option==1)
			{
				if((Driver1.Status==1)||(Driver1.Status==3))
				{
					Option.Normal_Option=0;
				}
				else if((Driver1.Status==2)||(Driver1.Status==4))
				{
					Option.Normal_Option=2;
				}
			}
			else if(Option.Normal_Option==2)
			{
				if((Driver1.Status==2)||(Driver1.Status==4))
				{
					Option.Normal_Option=0;
				}
			}
		}
	}	
	else if(Top_Device==2)
	{
		if((Driver2.Status==1)||(Driver2.Status==2)||(Driver2.Status==3)||(Driver2.Status==4))
		{
			if(Option.Normal_Option==0)
			{
				Option.Normal_Option=1;
			}
			else if(Option.Normal_Option==1)
			{
				if((Driver2.Status==1)||(Driver2.Status==3))
				{
					Option.Normal_Option=0;
				}
				if((Driver2.Status==2)||(Driver2.Status==4))
				{
					Option.Normal_Option=2;
				}
			}
			else if(Option.Normal_Option==2)
			{
				if((Driver2.Status==2)||(Driver2.Status==4))
				{
					Option.Normal_Option=0;
				}
			}
		}
	}
	else if(Top_Device==5)
	{
		if(Option.Usart_DataTx_Option==4)
			Option.Usart_DataTx_Option=0;
		else
			Option.Usart_DataTx_Option++;
	}
	else if(Top_Device == 6)
	{
		if(Canopen.Status == 1)		// 使能/禁能功能
		{
			if((Option.Driver_Current_Status&0x02)==0)
			{
				// 使能
				Config_RPDO2(1);
				//Profile_Torque_Mode_Config(1);
				Enable_Driver(1);
				if((Option.Driver_CAN_Out&0x02)==0) 
					AI_Mode_Config(3,1);
				else
					AI_Mode_Config(0,1);
				Option.Driver_Current_Status|=0x02;
			}
			else
			{
				// 禁能
				Disable_Driver(1);
				Option.Driver_Current_Status&=0xfd;
			}
		}
		else if(Canopen.Status == 2)	// CAN/AI控制输出
		{
			if((Option.Driver_CAN_Out&0x02)==0)
			{
				AI_Mode_Config(0, 1);
				Option.Driver_CAN_Out|=0x02;
			}
			else
			{
				AI_Mode_Config(3, 1);
				Option.Driver_CAN_Out&=0xfd;
			}
		}
	}
	else if(Top_Device==9)
	{
		if(Option.Normal_Option==0)
			Option.Normal_Option = 1;
		else if(Option.Normal_Option==1)
			Option.Normal_Option = 2;
		else
			Option.Normal_Option = 0;
	}
	else if(Top_Device==10)
	{
		if(Option.DRC1_DataSource_Option==0)
			Option.DRC1_DataSource_Option = 1;
		else if(Option.DRC1_DataSource_Option==1)
			Option.DRC1_DataSource_Option = 2;
		else if(Option.DRC1_DataSource_Option==2)
			Option.DRC1_DataSource_Option = 3;
		else if(Option.DRC1_DataSource_Option==3)
			Option.DRC1_DataSource_Option = 4;
		else
			Option.DRC1_DataSource_Option = 0;
		
	}
	else if(Top_Device==11)
	{
		if(Option.Normal_Option==0)
			Option.Normal_Option = 1;
		else if(Option.Normal_Option==1)
			Option.Normal_Option = 2;
		else
			Option.Normal_Option = 0;
	}
	else if(Top_Device==12)
	{
		if(Option.DRC2_DataSource_Option==0)
			Option.DRC2_DataSource_Option = 1;
		else if(Option.DRC2_DataSource_Option==1)
			Option.DRC2_DataSource_Option = 2;
		else if(Option.DRC2_DataSource_Option==2)
			Option.DRC2_DataSource_Option = 3;
		else if(Option.DRC2_DataSource_Option==3)
			Option.DRC2_DataSource_Option = 4;
		else
			Option.DRC2_DataSource_Option = 0;
	}
	else if(Top_Device==13)		// Sweep1
	{
		if(Option.Normal_Option == 0)
		{
			Driver1.Sweep_wave->output_enable = 0;		// 配置扫频需要先关闭扫频 这是被动关闭，只改变sweep的输出使能，并不改变驱动器当前模式  注意与按键KEY_12中关闭Sweep的操作区别
			Option.Normal_Option = 1;
			
			OLED_Update();
			OLED_Update_Data();
		}
		else if(Option.Normal_Option == 1)
			Option.Normal_Option = 2;
		else if(Option.Normal_Option == 2)
			Option.Normal_Option = 3;
		else if(Option.Normal_Option == 3)
			Option.Normal_Option = 4;
		else
		{
			Option.Normal_Option = 0;
			if(Driver1.Status == 6)										// 配置之前扫频打开，配置后自动打开
			{
				Driver1.Sweep_wave->output_enable = 1;
				Return_to_Chirp_Mode(&Driver1);
				
				OLED_Update();
				OLED_Update_Data();
			}
		}
	}
	else if(Top_Device==14)
	{
		if(Option.Sweep1_Mode == 1)
			Option.Sweep1_Mode = 2;
		else if(Option.Sweep1_Mode == 2)
			Option.Sweep1_Mode = 1;
		
		Driver1.Sweep_wave->mode_option = Option.Sweep1_Mode;	
	}
	/*Sweep2*/
	else if(Top_Device==15)
	{
		if(Option.Normal_Option == 0)
		{
			Driver2.Sweep_wave->output_enable = 0;		// 配置扫频需要先关闭扫频 这是被动关闭，只改变sweep的输出使能，并不改变驱动器当前模式  注意与按键KEY_12中关闭Sweep的操作区别
			Option.Normal_Option = 1;
			
			OLED_Update();
			OLED_Update_Data();
		}
		else if(Option.Normal_Option == 1)
			Option.Normal_Option = 2;
		else if(Option.Normal_Option == 2)
			Option.Normal_Option = 3;
		else if(Option.Normal_Option == 3)
			Option.Normal_Option = 4;
		else
		{
			Option.Normal_Option = 0;
			if(Driver2.Status == 6)										// 配置之前扫频打开，配置后自动打开
			{
				Driver2.Sweep_wave->output_enable = 1;
				Return_to_Chirp_Mode(&Driver2);
				
				OLED_Update();
				OLED_Update_Data();
			}
		}
	}
	else if(Top_Device==16)
	{
		if(Option.Sweep2_Mode == 1)
			Option.Sweep2_Mode = 2;
		else if(Option.Sweep2_Mode == 2)
			Option.Sweep2_Mode = 1;
		
		Driver2.Sweep_wave->mode_option = Option.Sweep2_Mode;
		OLED_Update_Cursor();
	}
}

void Key14_Function(void)
{
	switch(Top_Device)
	{
		case 1:
		{
			if((Driver1.Status==1)||(Driver1.Status==2))
				Driver_Voltage_Output_0(&Driver1);
			else if((Driver1.Status==3)||(Driver1.Status==4))
				Driver_TargetDisplacement_0(&Driver1);
			else if(Driver1.Status==5)
				Clear_Driver_Encoder_Value(&Driver1);
			break;
		}
		case 2:
		{
			if((Driver2.Status==1)||(Driver2.Status==2))
				Driver_Voltage_Output_0(&Driver2);
			else if((Driver2.Status==3)||(Driver2.Status==4))
				Driver_TargetDisplacement_0(&Driver2);
			else if(Driver2.Status==5)
				Clear_Driver_Encoder_Value(&Driver2);
			break;
		}
		case 3:
		{
			Encoder_Data_Update = 0;
			Clear_Encoder_Value(&Encoder1);
			Encoder_Data_Update = 1;
			break;
		}
		case 4:
		{
			Encoder_Data_Update = 0;
			Clear_Encoder_Value(&Encoder2);
			Encoder_Data_Update = 1;
			break;
		}
		case 5:
		{
			if(Option.Usart_DataTx_Option==1)
			{
				Clear_Driver_Encoder_Value(&Driver1);
			}
			else if(Option.Usart_DataTx_Option==2)
			{
				Clear_Driver_Encoder_Value(&Driver2);
			}
			else if(Option.Usart_DataTx_Option==3)
			{
				Clear_Encoder_Value(&Encoder1);
			}
			else if(Option.Usart_DataTx_Option==4)
			{
				Clear_Encoder_Value(&Encoder2);
			}
			break;
		}
		case 6:
		{
			if(Canopen.Status == 1)
			{
				if((Option.Driver_Current_Status&0x01)==0)
				{
					// 使能
					Config_RPDO2(2);
					//Profile_Torque_Mode_Config(2);
					Enable_Driver(2);
					if((Option.Driver_CAN_Out&0x01)==0)
						AI_Mode_Config(3, 2);
					else
						AI_Mode_Config(0, 2);
					Option.Driver_Current_Status|=0x01;
				}
				else
				{
					// 禁能
					Disable_Driver(2);
					Option.Driver_Current_Status&=0xfe;
				}
			}
			else if(Canopen.Status == 2)
			{
				if((Option.Driver_CAN_Out&0x01)==0)
				{
					AI_Mode_Config(0, 2);
					Option.Driver_CAN_Out|=0x01;
				}
				else
				{
					AI_Mode_Config(3, 2);
					Option.Driver_CAN_Out&=0xfe;
				}
			}
			break;
		}
		default:
			break;
	}
}

void Key21_Function(void)
{
	// 向上粗调
	if((Top_Device==1)&&(Driver1.Status==1)&&(Option.Normal_Option==1))				// 向上粗调电压
	{
		Driver_Switch_CurrentVoltage(&Driver1, (int32_t)(Driver1.Output_Voltage)+1);
	}
	else if((Top_Device==1)&&(Driver1.Status==2)&&(Option.Normal_Option==2))		// 向上粗调频率
	{
		Driver_Switch_Sinwave((int32_t)(Driver1.frequency_force)+1, Driver1.amplitude_force, &Driver1);
	}
	else if((Top_Device==1)&&(Driver1.Status==2)&&(Option.Normal_Option==1))		// 向上粗调幅值
	{
		Driver_Switch_Sinwave(Driver1.frequency_force, (int32_t)(Driver1.amplitude_force)+1, &Driver1);
	}
	else if((Top_Device==1)&&(Driver1.Status==3)&&(Option.Normal_Option==1))				// 向上粗调目标位置
	{
		Driver_Switch_TargetDisplacement(&Driver1, (int32_t)(Driver1.Target_Displacement)+1);
	}
	else if((Top_Device==1)&&(Driver1.Status==4)&&(Option.Normal_Option==2))				// 向上粗调位置频率
	{
		Driver_Switch_Sinwave((int32_t)(Driver1.frequency_displacement)+1, Driver1.amplitude_displacement, &Driver1);
	}
	else if((Top_Device==1)&&(Driver1.Status==4)&&(Option.Normal_Option==1))				// 向上粗调位置幅值
	{
		Driver_Switch_Sinwave(Driver1.frequency_displacement, (int32_t)(Driver1.amplitude_displacement)+1, &Driver1);
	}
	else if((Top_Device==2)&&(Driver2.Status==1)&&(Option.Normal_Option==1))				// 向上粗调电压
	{
		Driver_Switch_CurrentVoltage(&Driver2, (int32_t)(Driver2.Output_Voltage)+1);
	}
	else if((Top_Device==2)&&(Driver2.Status==2)&&(Option.Normal_Option==2))				// 向上粗调频率
	{
		Driver_Switch_Sinwave((int32_t)(Driver2.frequency_force)+1, Driver2.amplitude_force, &Driver2);
	}
	else if((Top_Device==2)&&(Driver2.Status==2)&&(Option.Normal_Option==1))				// 向上粗调幅值
	{
		Driver_Switch_Sinwave(Driver2.frequency_force, (int32_t)(Driver2.amplitude_force)+1, &Driver2);
	}
	else if((Top_Device==2)&&(Driver2.Status==3)&&(Option.Normal_Option==1))				// 向上粗调目标位置
	{
		Driver_Switch_TargetDisplacement(&Driver2, (int32_t)(Driver2.Target_Displacement)+1);
	}
	else if((Top_Device==2)&&(Driver2.Status==4)&&(Option.Normal_Option==2))				// 向上粗调位置频率
	{
		Driver_Switch_Sinwave((int32_t)(Driver2.frequency_displacement)+1, Driver2.amplitude_displacement, &Driver2);
	}
	else if((Top_Device==2)&&(Driver2.Status==4)&&(Option.Normal_Option==1))				// 向上粗调位置幅值
	{
		Driver_Switch_Sinwave(Driver2.frequency_displacement, (int32_t)(Driver2.amplitude_displacement)+1, &Driver2);
	}
	else if((Top_Device==9)&&(Option.Normal_Option==1))		// 向上粗调反馈增益1
	{
		Change_Feedback_Parameter(&Delay_Feedback_Controller1, Delay_Feedback_Controller1.g+10, Delay_Feedback_Controller1.tau_ms);
	}
	else if((Top_Device==9)&&(Option.Normal_Option==2))		// 向上粗调反馈时滞1
	{
		Change_Feedback_Parameter(&Delay_Feedback_Controller1, Delay_Feedback_Controller1.g, Delay_Feedback_Controller1.tau_ms+10);
	}
	else if((Top_Device==11)&&(Option.Normal_Option==1))		// 向上粗调反馈增益2
	{
		Change_Feedback_Parameter(&Delay_Feedback_Controller2, Delay_Feedback_Controller2.g+10, Delay_Feedback_Controller2.tau_ms);
	}
	else if((Top_Device==11)&&(Option.Normal_Option==2))		// 向上粗调反馈时滞2
	{
		Change_Feedback_Parameter(&Delay_Feedback_Controller2, Delay_Feedback_Controller2.g, Delay_Feedback_Controller2.tau_ms+10);
	}
	// Sweep1 参数向上粗调
	else if((Top_Device==13)&&(Option.Normal_Option==1))
	{
		Change_Sweep_Parameter(&Driver1, (int32_t)(Driver1.Sweep_wave->amplitude)+1.0l, Driver1.Sweep_wave->frequency_begin, Driver1.Sweep_wave->frequency_end, Driver1.Sweep_wave->time);	
	}
	else if((Top_Device==13)&&(Option.Normal_Option==2))
	{
		Change_Sweep_Parameter(&Driver1, Driver1.Sweep_wave->amplitude, Driver1.Sweep_wave->frequency_begin+1.0l, Driver1.Sweep_wave->frequency_end, Driver1.Sweep_wave->time);
	}
	else if((Top_Device==13)&&(Option.Normal_Option==3))
	{
		Change_Sweep_Parameter(&Driver1, Driver1.Sweep_wave->amplitude, Driver1.Sweep_wave->frequency_begin, Driver1.Sweep_wave->frequency_end+1.0l, Driver1.Sweep_wave->time);
	}
	else if((Top_Device==13)&&(Option.Normal_Option==4))
	{
		Change_Sweep_Parameter(&Driver1, Driver1.Sweep_wave->amplitude, Driver1.Sweep_wave->frequency_begin, Driver1.Sweep_wave->frequency_end, Driver1.Sweep_wave->time+100);
	}
	// Sweep2 参数向上粗调
	else if((Top_Device==15)&&(Option.Normal_Option==1))
	{
		Change_Sweep_Parameter(&Driver2, (uint32_t)(Driver2.Sweep_wave->amplitude)+1.0l, Driver2.Sweep_wave->frequency_begin, Driver2.Sweep_wave->frequency_end, Driver2.Sweep_wave->time);
	}
	else if((Top_Device==15)&&(Option.Normal_Option==2))
	{
		Change_Sweep_Parameter(&Driver2, Driver2.Sweep_wave->amplitude, Driver2.Sweep_wave->frequency_begin+1.0l, Driver2.Sweep_wave->frequency_end, Driver2.Sweep_wave->time);
	}
	else if((Top_Device==15)&&(Option.Normal_Option==3))
	{
		Change_Sweep_Parameter(&Driver2, Driver2.Sweep_wave->amplitude, Driver2.Sweep_wave->frequency_begin, Driver2.Sweep_wave->frequency_end+1.0l, Driver2.Sweep_wave->time);
	}
	else if((Top_Device==15)&&(Option.Normal_Option==4))
	{
		Change_Sweep_Parameter(&Driver2, Driver2.Sweep_wave->amplitude, Driver2.Sweep_wave->frequency_begin, Driver2.Sweep_wave->frequency_end, Driver2.Sweep_wave->time+100);
	}
	
	// 设备切换
	else if((Top_Device<5)&&(Top_Device>0))
	{
		Encoder_Data_Update = 0;
		Top_Device=1;
		Option.Normal_Option = 0;
	
		OLED_Update();
	}
	else if((Top_Device>4)&&(Top_Device<9))
	{
		Encoder_Data_Update = 0;
		Top_Device=5;
		Option.Normal_Option = 0;
	
		OLED_Update();
	}
	else if((Top_Device>8)&&(Top_Device<11))
	{
		Encoder_Data_Update = 0;
		Top_Device=9;
		Option.Normal_Option = 0;
	
		OLED_Update();
	}
	else if((Top_Device>10)&&(Top_Device<13))
	{
		Encoder_Data_Update = 0;
		Top_Device=11;
		Option.Normal_Option = 0;
	
		OLED_Update();
	}
	else if((Top_Device>12)&&(Top_Device<15))
	{
		Top_Device=13;
		Option.Normal_Option = 0;
	
		OLED_Update();
	}
	else if((Top_Device>14)&&(Top_Device<17))
	{
		Top_Device=15;
		Option.Normal_Option = 0;
	
		OLED_Update();
	}
}

void Key22_Function(void)
{
	// 向下粗调
	if((Top_Device==1)&&(Driver1.Status==1)&&(Option.Normal_Option==1))				// 向下粗调电压
	{
		Driver_Switch_CurrentVoltage(&Driver1, (int32_t)(Driver1.Output_Voltage)-1);
	}
	else if((Top_Device==1)&&(Driver1.Status==2)&&(Option.Normal_Option==2))		// 向下粗调频率
	{
		Driver_Switch_Sinwave((int32_t)(Driver1.frequency_force)-1, Driver1.amplitude_force, &Driver1);
	}
	else if((Top_Device==1)&&(Driver1.Status==2)&&(Option.Normal_Option==1))		// 向下粗调幅值
	{
		Driver_Switch_Sinwave(Driver1.frequency_force, (int32_t)(Driver1.amplitude_force)-1, &Driver1);
	}
	else if((Top_Device==1)&&(Driver1.Status==3)&&(Option.Normal_Option==1))				// 向下粗调目标位置
	{
		Driver_Switch_TargetDisplacement(&Driver1, (int32_t)(Driver1.Target_Displacement)-1);
	}
	else if((Top_Device==1)&&(Driver1.Status==4)&&(Option.Normal_Option==2))		// 向下粗调位置频率
	{
		Driver_Switch_Sinwave((int32_t)(Driver1.frequency_displacement)-1, Driver1.amplitude_displacement, &Driver1);
	}
	else if((Top_Device==1)&&(Driver1.Status==4)&&(Option.Normal_Option==1))		// 向下粗调位置幅值
	{
		Driver_Switch_Sinwave(Driver1.frequency_displacement, (int32_t)(Driver1.amplitude_displacement)-1, &Driver1);
	}
	else if((Top_Device==2)&&(Driver2.Status==1)&&(Option.Normal_Option==1))				// 向下粗调电压
	{
		Driver_Switch_CurrentVoltage(&Driver2, (int32_t)(Driver2.Output_Voltage)-1);
	}
	else if((Top_Device==2)&&(Driver2.Status==2)&&(Option.Normal_Option==2))				// 向下粗调频率
	{
		Driver_Switch_Sinwave((int32_t)(Driver2.frequency_force)-1, Driver2.amplitude_force, &Driver2);
	}
	else if((Top_Device==2)&&(Driver2.Status==2)&&(Option.Normal_Option==1))				// 向下粗调幅值
	{
		Driver_Switch_Sinwave(Driver2.frequency_force, (int32_t)(Driver2.amplitude_force)-1, &Driver2);
	}
	else if((Top_Device==2)&&(Driver2.Status==3)&&(Option.Normal_Option==1))				// 向下粗调目标位置
	{
		Driver_Switch_TargetDisplacement(&Driver2, (int32_t)(Driver2.Target_Displacement)-1);
	}
	else if((Top_Device==2)&&(Driver2.Status==4)&&(Option.Normal_Option==2))				// 向下粗调位置频率
	{
		Driver_Switch_Sinwave((int32_t)(Driver2.frequency_displacement)-1, Driver2.amplitude_displacement, &Driver2);
	}
	else if((Top_Device==2)&&(Driver2.Status==4)&&(Option.Normal_Option==1))				// 向下粗调位置幅值
	{
		Driver_Switch_Sinwave(Driver2.frequency_displacement, (int32_t)(Driver2.amplitude_displacement)-1, &Driver2);
	}
	else if((Top_Device==9)&&(Option.Normal_Option==1))		// 向下粗调反馈增益
	{
		Change_Feedback_Parameter(&Delay_Feedback_Controller1, Delay_Feedback_Controller1.g-10, Delay_Feedback_Controller1.tau_ms);
	}
	else if((Top_Device==9)&&(Option.Normal_Option==2))		// 向下粗调反馈时滞
	{
		Change_Feedback_Parameter(&Delay_Feedback_Controller1, Delay_Feedback_Controller1.g, Delay_Feedback_Controller1.tau_ms-10);
	}
	else if((Top_Device==11)&&(Option.Normal_Option==1))		// 向下粗调反馈增益
	{
		Change_Feedback_Parameter(&Delay_Feedback_Controller2, Delay_Feedback_Controller2.g-10, Delay_Feedback_Controller2.tau_ms);
	}
	else if((Top_Device==11)&&(Option.Normal_Option==2))		// 向下粗调反馈时滞
	{
		Change_Feedback_Parameter(&Delay_Feedback_Controller2, Delay_Feedback_Controller2.g, Delay_Feedback_Controller2.tau_ms-10);
	}
	// Sweep1 参数向下粗调
	else if((Top_Device==13)&&(Option.Normal_Option==1))
	{
		Change_Sweep_Parameter(&Driver1, (uint32_t)(Driver1.Sweep_wave->amplitude)-1.0l, Driver1.Sweep_wave->frequency_begin, Driver1.Sweep_wave->frequency_end, Driver1.Sweep_wave->time);
	}
	else if((Top_Device==13)&&(Option.Normal_Option==2))
	{
		Change_Sweep_Parameter(&Driver1, Driver1.Sweep_wave->amplitude, Driver1.Sweep_wave->frequency_begin-1.0l, Driver1.Sweep_wave->frequency_end, Driver1.Sweep_wave->time);
	}
	else if((Top_Device==13)&&(Option.Normal_Option==3))
	{
		Change_Sweep_Parameter(&Driver1, Driver1.Sweep_wave->amplitude, Driver1.Sweep_wave->frequency_begin, Driver1.Sweep_wave->frequency_end-1.0l, Driver1.Sweep_wave->time);
	}
	else if((Top_Device==13)&&(Option.Normal_Option==4))
	{
		Change_Sweep_Parameter(&Driver1, Driver1.Sweep_wave->amplitude, Driver1.Sweep_wave->frequency_begin, Driver1.Sweep_wave->frequency_end, Driver1.Sweep_wave->time-100);
	}
	// Sweep2 参数向下粗调
	else if((Top_Device==15)&&(Option.Normal_Option==1))
	{
		Change_Sweep_Parameter(&Driver2, (uint32_t)(Driver2.Sweep_wave->amplitude)-1.0l, Driver2.Sweep_wave->frequency_begin, Driver2.Sweep_wave->frequency_end, Driver2.Sweep_wave->time);
	}
	else if((Top_Device==15)&&(Option.Normal_Option==2))
	{
		Change_Sweep_Parameter(&Driver2, Driver2.Sweep_wave->amplitude, Driver2.Sweep_wave->frequency_begin-1.0l, Driver2.Sweep_wave->frequency_end, Driver2.Sweep_wave->time);
	}
	else if((Top_Device==15)&&(Option.Normal_Option==3))
	{
		Change_Sweep_Parameter(&Driver2, Driver2.Sweep_wave->amplitude, Driver2.Sweep_wave->frequency_begin, Driver2.Sweep_wave->frequency_end-1.0l, Driver2.Sweep_wave->time);
	}
	else if((Top_Device==15)&&(Option.Normal_Option==4))
	{
		Change_Sweep_Parameter(&Driver2, Driver2.Sweep_wave->amplitude, Driver2.Sweep_wave->frequency_begin, Driver2.Sweep_wave->frequency_end, Driver2.Sweep_wave->time-100);
	}
	
	// 切换设备
	else if((Top_Device<5)&&(Top_Device>0))
	{
		Encoder_Data_Update = 0;
		Top_Device=2;
		Option.Normal_Option = 0;
		
		OLED_Update();
	}
	else if((Top_Device>4)&&(Top_Device<9))
	{
		Encoder_Data_Update = 0;
		Top_Device=6;
		Option.Normal_Option = 0;
	
		OLED_Update();
	}
	else if((Top_Device>8)&&(Top_Device<11))
	{
		Encoder_Data_Update = 0;
		Top_Device=10;
		Option.Normal_Option = 0;
	
		OLED_Update();
	}
	else if((Top_Device>10)&&(Top_Device<13))
	{
		Encoder_Data_Update = 0;
		Top_Device=12;
		Option.Normal_Option = 0;
	
		OLED_Update();
	}
	else if((Top_Device>12)&&(Top_Device<15))
	{
		Top_Device=14;
		Option.Normal_Option = 0;
	
		OLED_Update();
	}
	else if((Top_Device>14)&&(Top_Device<17))
	{
		Top_Device=16;
		Option.Normal_Option = 0;
	
		OLED_Update();
	}
}

void Key23_Function(void)
{
	// 向上精调
	if((Top_Device==1)&&(Driver1.Status==1)&&(Option.Normal_Option==1))				// 向上精调电压
	{
		Driver_Switch_CurrentVoltage(&Driver1, (((int32_t)(Driver1.Output_Voltage*10))+1.0f)/10);
	}
	else if((Top_Device==1)&&(Driver1.Status==2)&&(Option.Normal_Option==2))		// 向上精调频率
	{
		Driver_Switch_Sinwave(((int32_t)(Driver1.frequency_force*10)+1.0f)/10, Driver1.amplitude_force, &Driver1);
	}
	else if((Top_Device==1)&&(Driver1.Status==2)&&(Option.Normal_Option==1))		// 向上精调幅值
	{
		Driver_Switch_Sinwave(Driver1.frequency_force, ((int32_t)(Driver1.amplitude_force*10)+1.0f)/10, &Driver1);
	}
	else if((Top_Device==1)&&(Driver1.Status==3)&&(Option.Normal_Option==1))				// 向上精调目标位置
	{
		Driver_Switch_TargetDisplacement(&Driver1, (((int32_t)(Driver1.Target_Displacement*10))+1.0f)/10);
	}
	else if((Top_Device==1)&&(Driver1.Status==4)&&(Option.Normal_Option==2))		// 向上精调频率
	{
		Driver_Switch_Sinwave(((int32_t)(Driver1.frequency_displacement*10)+1.0f)/10, Driver1.amplitude_displacement, &Driver1);
	}
	else if((Top_Device==1)&&(Driver1.Status==4)&&(Option.Normal_Option==1))		// 向上精调幅值
	{
		Driver_Switch_Sinwave(Driver1.frequency_displacement, ((int32_t)(Driver1.amplitude_displacement*10)+1.0f)/10, &Driver1);
	}
	else if((Top_Device==2)&&(Driver2.Status==1)&&(Option.Normal_Option==1))				// 向上精调电压
	{
		Driver_Switch_CurrentVoltage(&Driver2, (((int32_t)(Driver2.Output_Voltage*10))+1.0f)/10);
	}
	else if((Top_Device==2)&&(Driver2.Status==2)&&(Option.Normal_Option==2))		// 向上精调频率
	{
		Driver_Switch_Sinwave(((int32_t)(Driver2.frequency_force*10)+1.0f)/10, Driver2.amplitude_force, &Driver2);
	}
	else if((Top_Device==2)&&(Driver2.Status==2)&&(Option.Normal_Option==1))		// 向上精调幅值
	{
		Driver_Switch_Sinwave(Driver2.frequency_force, ((int32_t)(Driver2.amplitude_force*10)+1.0f)/10, &Driver2);
	}
	else if((Top_Device==2)&&(Driver2.Status==3)&&(Option.Normal_Option==1))				// 向上精调目标位置
	{
		Driver_Switch_TargetDisplacement(&Driver2, (((int32_t)(Driver2.Target_Displacement*10))+1.0f)/10);
	}
	else if((Top_Device==2)&&(Driver2.Status==4)&&(Option.Normal_Option==2))		// 向上精调位置频率
	{
		Driver_Switch_Sinwave(((int32_t)(Driver2.frequency_displacement*10)+1.0f)/10, Driver2.amplitude_displacement, &Driver2);
	}
	else if((Top_Device==2)&&(Driver2.Status==4)&&(Option.Normal_Option==1))		// 向上精调位置幅值
	{
		Driver_Switch_Sinwave(Driver2.frequency_displacement, ((int32_t)(Driver2.amplitude_displacement*10)+1.0f)/10, &Driver2);
	}
	else if((Top_Device==9)&&(Option.Normal_Option==1))		// 向上精调反馈增益
	{
		Change_Feedback_Parameter(&Delay_Feedback_Controller1, Delay_Feedback_Controller1.g+1, Delay_Feedback_Controller1.tau_ms);
	}
	else if((Top_Device==9)&&(Option.Normal_Option==2))		// 向上精调反馈时滞
	{
		Change_Feedback_Parameter(&Delay_Feedback_Controller1, Delay_Feedback_Controller1.g, Delay_Feedback_Controller1.tau_ms+1);
	}
	else if((Top_Device==11)&&(Option.Normal_Option==1))		// 向上精调反馈增益
	{
		Change_Feedback_Parameter(&Delay_Feedback_Controller2, Delay_Feedback_Controller2.g+1, Delay_Feedback_Controller2.tau_ms);
	}
	else if((Top_Device==11)&&(Option.Normal_Option==2))		// 向上精调反馈时滞
	{
		Change_Feedback_Parameter(&Delay_Feedback_Controller2, Delay_Feedback_Controller2.g, Delay_Feedback_Controller2.tau_ms+1);
	}
	// Sweep1 向上精调参数
	else if((Top_Device==13)&&(Option.Normal_Option==1))
	{
		Change_Sweep_Parameter(&Driver1, ((int32_t)(Driver1.Sweep_wave->amplitude*10)+1.0f)/10, Driver1.Sweep_wave->frequency_begin, Driver1.Sweep_wave->frequency_end, Driver1.Sweep_wave->time);
	}
	else if((Top_Device==13)&&(Option.Normal_Option==2))
	{
		Change_Sweep_Parameter(&Driver1, Driver1.Sweep_wave->amplitude, ((int32_t)(Driver1.Sweep_wave->frequency_begin*10)+1.0f)/10, Driver1.Sweep_wave->frequency_end, Driver1.Sweep_wave->time);
	}
	else if((Top_Device==13)&&(Option.Normal_Option==3))
	{
		Change_Sweep_Parameter(&Driver1, Driver1.Sweep_wave->amplitude, Driver1.Sweep_wave->frequency_begin, ((int32_t)(Driver1.Sweep_wave->frequency_end*10)+1.0f)/10, Driver1.Sweep_wave->time);
	}
	else if((Top_Device==13)&&(Option.Normal_Option==4))
	{
		Change_Sweep_Parameter(&Driver1, Driver1.Sweep_wave->amplitude, Driver1.Sweep_wave->frequency_begin, Driver1.Sweep_wave->frequency_end, Driver1.Sweep_wave->time+10);
	}
	// Sweep2 向上精调参数
	else if((Top_Device==15)&&(Option.Normal_Option==1))
	{
		Change_Sweep_Parameter(&Driver2, ((int32_t)(Driver2.Sweep_wave->amplitude*10)+1.0f)/10, Driver2.Sweep_wave->frequency_begin, Driver2.Sweep_wave->frequency_end, Driver2.Sweep_wave->time);
	}
	else if((Top_Device==15)&&(Option.Normal_Option==2))
	{
		Change_Sweep_Parameter(&Driver2, Driver2.Sweep_wave->amplitude, ((int32_t)(Driver2.Sweep_wave->frequency_begin*10)+1.0f)/10, Driver2.Sweep_wave->frequency_end, Driver2.Sweep_wave->time);
	}
	else if((Top_Device==15)&&(Option.Normal_Option==3))
	{
		Change_Sweep_Parameter(&Driver2, Driver2.Sweep_wave->amplitude, Driver2.Sweep_wave->frequency_begin, ((int32_t)(Driver2.Sweep_wave->frequency_end*10)+1.0f)/10, Driver2.Sweep_wave->time);
	}
	else if((Top_Device==15)&&(Option.Normal_Option==4))
	{
		Change_Sweep_Parameter(&Driver2, Driver2.Sweep_wave->amplitude, Driver2.Sweep_wave->frequency_begin, Driver2.Sweep_wave->frequency_end, Driver2.Sweep_wave->time+10);
	}
	
	// 切换设备
	else if((Top_Device<5)&&(Top_Device>0))
	{
		Encoder_Data_Update = 0;
		Top_Device=3;
		Option.Normal_Option = 0;
	
		OLED_Update();
		Encoder_Data_Update = 1;
	}
}

void Key24_Function(void)
{
	// 向下精调
	if((Top_Device==1)&&(Driver1.Status==1)&&(Option.Normal_Option==1))				// 向下精调电压
	{
		Driver_Switch_CurrentVoltage(&Driver1, (((int32_t)(Driver1.Output_Voltage*10))-1.0f)/10);
	}
	else if((Top_Device==1)&&(Driver1.Status==2)&&(Option.Normal_Option==2))		// 向下精调频率
	{
		Driver_Switch_Sinwave(((int32_t)(Driver1.frequency_force*10)-1.0f)/10, Driver1.amplitude_force, &Driver1);
	}
	else if((Top_Device==1)&&(Driver1.Status==2)&&(Option.Normal_Option==1))		// 向下精调幅值
	{
		Driver_Switch_Sinwave(Driver1.frequency_force, ((int32_t)(Driver1.amplitude_force*10)-1.0f)/10, &Driver1);
	}
	else if((Top_Device==1)&&(Driver1.Status==3)&&(Option.Normal_Option==1))				// 向下精调目标位置
	{
		Driver_Switch_TargetDisplacement(&Driver1, (((int32_t)(Driver1.Target_Displacement*10))-1.0f)/10);
	}
	else if((Top_Device==1)&&(Driver1.Status==4)&&(Option.Normal_Option==2))		// 向下精调位置频率
	{
		Driver_Switch_Sinwave(((int32_t)(Driver1.frequency_displacement*10)-1.0f)/10, Driver1.amplitude_displacement, &Driver1);
	}
	else if((Top_Device==1)&&(Driver1.Status==4)&&(Option.Normal_Option==1))		// 向下精调位置幅值
	{
		Driver_Switch_Sinwave(Driver1.frequency_displacement, ((int32_t)(Driver1.amplitude_displacement*10)-1.0f)/10, &Driver1);
	}
	else if((Top_Device==2)&&(Driver2.Status==1)&&(Option.Normal_Option==1))				// 向下精调电压
	{
		Driver_Switch_CurrentVoltage(&Driver2, (((int32_t)(Driver2.Output_Voltage*10))-1.0f)/10);
	}
	else if((Top_Device==2)&&(Driver2.Status==2)&&(Option.Normal_Option==2))		// 向下精调频率
	{
		Driver_Switch_Sinwave(((int32_t)(Driver2.frequency_force*10)-1.0f)/10, Driver2.amplitude_force, &Driver2);
	}
	else if((Top_Device==2)&&(Driver2.Status==2)&&(Option.Normal_Option==1))		// 向下精调幅值
	{
		Driver_Switch_Sinwave(Driver2.frequency_force, ((int32_t)(Driver2.amplitude_force*10)-1.0f)/10, &Driver2);
	}
	else if((Top_Device==2)&&(Driver2.Status==3)&&(Option.Normal_Option==1))				// 向下精调目标位置
	{
		Driver_Switch_TargetDisplacement(&Driver2, (((int32_t)(Driver2.Target_Displacement*10))-1.0f)/10);
	}
	else if((Top_Device==2)&&(Driver2.Status==4)&&(Option.Normal_Option==2))		// 向下精调位置频率
	{
		Driver_Switch_Sinwave(((int32_t)(Driver2.frequency_displacement*10)-1.0f)/10, Driver2.amplitude_displacement, &Driver2);
	}
	else if((Top_Device==2)&&(Driver2.Status==4)&&(Option.Normal_Option==1))		// 向下精调位置幅值
	{
		Driver_Switch_Sinwave(Driver2.frequency_displacement, ((int32_t)(Driver2.amplitude_displacement*10)-1.0f)/10, &Driver2);
	}
	else if((Top_Device==9)&&(Option.Normal_Option==1))		// 向下精调反馈增益
	{
		Change_Feedback_Parameter(&Delay_Feedback_Controller1, Delay_Feedback_Controller1.g-1, Delay_Feedback_Controller1.tau_ms);
	}
	else if((Top_Device==9)&&(Option.Normal_Option==2))		// 向下精调反馈时滞
	{
		Change_Feedback_Parameter(&Delay_Feedback_Controller1, Delay_Feedback_Controller1.g, Delay_Feedback_Controller1.tau_ms-1);
	}
	else if((Top_Device==11)&&(Option.Normal_Option==1))		// 向下精调反馈增益
	{
		Change_Feedback_Parameter(&Delay_Feedback_Controller2, Delay_Feedback_Controller2.g-1, Delay_Feedback_Controller2.tau_ms);
	}
	else if((Top_Device==11)&&(Option.Normal_Option==2))		// 向下精调反馈时滞
	{
		Change_Feedback_Parameter(&Delay_Feedback_Controller2, Delay_Feedback_Controller2.g, Delay_Feedback_Controller2.tau_ms-1);
	}
	// Sweep1 向下精调参数
	else if((Top_Device==13)&&(Option.Normal_Option==1))
	{
		Change_Sweep_Parameter(&Driver1, ((int32_t)(Driver1.Sweep_wave->amplitude*10)-1.0f)/10, Driver1.Sweep_wave->frequency_begin, Driver1.Sweep_wave->frequency_end, Driver1.Sweep_wave->time);
	}
	else if((Top_Device==13)&&(Option.Normal_Option==2))
	{
		Change_Sweep_Parameter(&Driver1, Driver1.Sweep_wave->amplitude, ((int32_t)(Driver1.Sweep_wave->frequency_begin*10)-1.0f)/10, Driver1.Sweep_wave->frequency_end, Driver1.Sweep_wave->time);
	}
	else if((Top_Device==13)&&(Option.Normal_Option==3))
	{
		Change_Sweep_Parameter(&Driver1, Driver1.Sweep_wave->amplitude, Driver1.Sweep_wave->frequency_begin, ((int32_t)(Driver1.Sweep_wave->frequency_end*10)-1.0f)/10, Driver1.Sweep_wave->time);
	}
	else if((Top_Device==13)&&(Option.Normal_Option==4))
	{
		Change_Sweep_Parameter(&Driver1, Driver1.Sweep_wave->amplitude, Driver1.Sweep_wave->frequency_begin, Driver1.Sweep_wave->frequency_end, Driver1.Sweep_wave->time-10);
	}
	// Sweep2 向下精调参数
	else if((Top_Device==15)&&(Option.Normal_Option==1))
	{
		Change_Sweep_Parameter(&Driver2, ((int32_t)(Driver2.Sweep_wave->amplitude*10)-1.0f)/10, Driver2.Sweep_wave->frequency_begin, Driver2.Sweep_wave->frequency_end, Driver2.Sweep_wave->time);
	}
	else if((Top_Device==15)&&(Option.Normal_Option==2))
	{
		Change_Sweep_Parameter(&Driver2, Driver2.Sweep_wave->amplitude, ((int32_t)(Driver2.Sweep_wave->frequency_begin*10)-1.0f)/10, Driver2.Sweep_wave->frequency_end, Driver2.Sweep_wave->time);
	}
	else if((Top_Device==15)&&(Option.Normal_Option==3))
	{
		Change_Sweep_Parameter(&Driver2, Driver2.Sweep_wave->amplitude, Driver2.Sweep_wave->frequency_begin, ((int32_t)(Driver2.Sweep_wave->frequency_end*10)-1.0f)/10, Driver2.Sweep_wave->time);
	}
	else if((Top_Device==15)&&(Option.Normal_Option==4))
	{
		Change_Sweep_Parameter(&Driver2, Driver2.Sweep_wave->amplitude, Driver2.Sweep_wave->frequency_begin, Driver2.Sweep_wave->frequency_end, Driver2.Sweep_wave->time-10);
	}
		
	//  切换设备
	else if((Top_Device<5)&&(Top_Device>0))
	{
		Encoder_Data_Update = 0;
		Top_Device=4;
		Option.Normal_Option = 0;
	
		OLED_Update();
		Encoder_Data_Update = 1;
	}
}

