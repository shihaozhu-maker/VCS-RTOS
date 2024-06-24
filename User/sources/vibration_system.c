#include "vibration_system.h"

extern KEY_Struct Key;
extern char Welcome[8][128];
extern Driver_Struct Driver1;
extern Driver_Struct Driver2;
extern uint8_t Top_Device;
extern uint8_t Encoder_Data_Update;
extern uint8_t Refresh_Oled;
extern Encoder_Struct Encoder1;
extern Encoder_Struct Encoder2;
extern Options_Struct Option;
extern PID_Postype_Structure pid1; 
extern PID_Inctype_Structure pid_inc1;
extern uint8_t chirp_update_flag1;
extern uint8_t chirp_update_flag2;


void Vibration_System_Init(void)
{
	FFT_Init();
		
	Interrupt_Init();
	delay_init();

	I2C_Software_Init();
	OLED_Init();
	OLED_Display_Video_Memory(Welcome, 0, 0);
	
	Driver_Init();
	Encoder_Init();
	Key_Init();
	LED_Init();
	Usart_Init();
	spi_Init();		// 与imx6ull通信
	//Delay_Feedback_Init();
	PID_Init();
	
	Canopen_Init();
	// 开启网络管理
	Run_NMT();
	
	
	// 默认开启CAN控制
	Config_RPDO2(1);
	Profile_Torque_Mode_Config(1);
	AI_Mode_Config(0,1);
	Enable_Driver(1);
	
	// id2电机配置 用不到可以注释 节约初始化时间
	Config_RPDO2(2);
	Profile_Torque_Mode_Config(2);
	AI_Mode_Config(0,2);
	Enable_Driver(2);
	
	
	Option.Driver_Current_Status|=0x03;
	Option.Driver_CAN_Out |= 0x03;
					
	// 电机回零
	Target_Torque_Set(0,1);
	Target_Torque_Set(0,2);
	

// 配置pid参数
	pid1.Kp = 2.5;
	pid1.Ki = 0.012;
	pid1.Kd = 14;
	
	
	pid_inc1.Kp = 3.2;
	pid_inc1.Ki = 0.01;
	pid_inc1.Kd = 10;
	
	
	delay_ms(1000);
	Clear_Encoder_Value(&Encoder1);
	Clear_Encoder_Value(&Encoder2);
	Clear_Driver_Encoder_Value(&Driver1);
	Clear_Driver_Encoder_Value(&Driver2);
	OLED_Update();
	
	// 开启串口发送
	//Option.Usart_DataTx_Option = 3;
}






