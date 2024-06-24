#ifndef __DRIVER_H
#define __DRIVER_H

#include "stm32f4xx.h"
#include "arm_math.h"

#define ENCODER_RESOLUTION 				1

// coil motor driver1
#define DRIVER1_AI_GPIOPORT						GPIOA
#define DRIVER1_AI_GPIOPIN						GPIO_Pin_4
#define DRIVER1_AI_GPIORCC						RCC_AHB1Periph_GPIOA

#define DRIVER1_ENCODER_A_GPIOPORT		GPIOA
#define DRIVER1_ENCODER_A_GPIOPIN			GPIO_Pin_8
#define DRIVER1_ENCODER_A_PINSOURCE		GPIO_PinSource8
#define DRIVER1_ENCODER_A_GPIORCC			RCC_AHB1Periph_GPIOA

#define DRIVER1_ENCODER_B_GPIOPORT		GPIOA
#define DRIVER1_ENCODER_B_GPIOPIN			GPIO_Pin_9
#define DRIVER1_ENCODER_B_PINSOURCE		GPIO_PinSource9
#define DRIVER1_ENCODER_B_GPIORCC			RCC_AHB1Periph_GPIOA

#define DRIVER1_ENCODER_Z_GPIOPORT		GPIOA
#define DRIVER1_ENCODER_Z_GPIOPIN			GPIO_Pin_10
#define DRIVER1_ENCODER_Z_GPIORCC			RCC_AHB1Periph_GPIOA

#define DRIVER1_ENCODER_TIMER					TIM1
#define DRIVER1_ENCODER_TIMER_RCC			RCC_APB2Periph_TIM1
#define DRIVER1_ENCODER_TIMER_PERIOD	0xffff



// coil motor driver2
#define DRIVER2_AI_GPIOPORT						GPIOA
#define DRIVER2_AI_GPIOPIN						GPIO_Pin_5
#define DRIVER2_AI_GPIORCC						RCC_AHB1Periph_GPIOA

#define DRIVER2_ENCODER_A_GPIOPORT		GPIOB
#define DRIVER2_ENCODER_A_GPIOPIN			GPIO_Pin_6
#define DRIVER2_ENCODER_A_PINSOURCE		GPIO_PinSource6
#define DRIVER2_ENCODER_A_GPIORCC			RCC_AHB1Periph_GPIOB

#define DRIVER2_ENCODER_B_GPIOPORT		GPIOB
#define DRIVER2_ENCODER_B_GPIOPIN			GPIO_Pin_7
#define DRIVER2_ENCODER_B_PINSOURCE		GPIO_PinSource7
#define DRIVER2_ENCODER_B_GPIORCC			RCC_AHB1Periph_GPIOB

#define DRIVER2_ENCODER_Z_GPIOPORT		GPIOB
#define DRIVER2_ENCODER_Z_GPIOPIN			GPIO_Pin_8
#define DRIVER2_ENCODER_Z_GPIORCC			RCC_AHB1Periph_GPIOB

#define DRIVER2_ENCODER_TIMER					TIM4
#define DRIVER2_ENCODER_TIMER_RCC			RCC_APB1Periph_TIM4
#define DRIVER2_ENCODER_TIMER_PERIOD	0xffff

// DAC
#define REFERENCE_VOLTAGE						0x7FF

/*ADC电压输出=设定值-VOLTAGE_OUTPUT_BIAS*/
#define VOLTAGE_OUTPUT_BIAS					1.65l
#define VOLTAGE_OUTPUT_STANDARD			1				// 输出电压经过标定

// TIMER
#define DRIVER_TIMER								TIM2
#define DRIVER_TIMER_RCC						RCC_APB1Periph_TIM2	
#define DRIVER_CONTROL_PERIOD				(int)(DRIVER_UPDATE_TIME*84000000)	

#define DRIVER_UPDATE_TIME					0.0005f		// 0.5ms进行一次DAC输出（控制循环时间）
#define SINWAVE_DATA_LENGTH					2000			// 波形数据最大长度
#define SINWAVE_MIN_FREQUENCY				1
#define SINWAVE_MAX_FREQUENCY				20

#define SWEEP_UPDATE_TIME						2					// sweep控制输出时间，以控制循环时间为单位，如2表示两个控制循环输出一次sweep信号

#define VOLTAGE_MAX									5l					// 标定后可实现的输出
#define VOLTAGE_MIN									-5l				

#define VOLTAGE_OUT_MAX							7.5l				// 控制板实际可输出模拟电压限制
#define VOLTAGE_OUT_MIN							-6.2l

#define DISPLACE_MAX								12.0l				// 最大控制位移 mm
#define DISPLACE_MIN								-12.0l			// 最小控制位移 mm

#define SWEEP_FREQUENCY_MAX					99.9f
#define SWEEP_TIME_MAX							999

typedef struct
{
	// 电压控制
	float32_t (*Sinwave_data1)[SINWAVE_DATA_LENGTH+1];	// sin波形数据 row1:time		row2:sin_data		table[0][0]数组有效位数
	float32_t (*Sinwave_data2)[SINWAVE_DATA_LENGTH+1];
	uint16_t sindata_number;														// 有效波形数据个数
	float32_t current_frequency;												// 当前sin频率（>1）
	float32_t current_amplitude;												// 当前sin幅值（<8）
	uint8_t current_data_table;													// 当前DAC转换的波形数组 1 or 2
	uint16_t current_sindata_index ;										// 当前DAC转换的波形数据索引
}Sinwave_Struct;

// 扫频结构体
typedef struct
{
	float frequency_begin;		// 扫频频率范围 单位Hz
	float frequency_end;

	float amplitude;					// 扫频幅值
	uint32_t time;								// 扫频时间 单位s
	uint32_t interval_time;				// 扫频间隔时间
	uint8_t output_enable;		// 扫频信号输出使能
	
	uint8_t mode_option;			// 1为电压模式（默认），2为位移模式
}Sweep_Structure;


typedef struct
{
	uint8_t ID;														// 1 or 2
	uint8_t Status;												// 0设备未就绪 1驱动器模拟输出模式 2驱动器正弦输出 3位置控制模式 4正弦位置模式 5编码器模式 6电压chirp模式 7位置chirp模式
	
	Sinwave_Struct* Sinwave;
	
	// 模拟电压输出
	uint16_t DAC_Value;										// AN模式下DAC输出数值
	float32_t Output_Voltage; 						// AN模式下DAC输出电压
	float32_t frequency_force;						// 力控正弦模式的电压频率
	float32_t amplitude_force;						// 力控正弦模式的电压幅值
	
	// 位置PID输出
	float32_t Target_Displacement;					// DIS模式下的目标位置
	float32_t frequency_displacement;				// 位移正弦模式的位移频率
	float32_t amplitude_displacement;				// 位移正弦模式的位移幅值
	
	// 编码器
	uint8_t Resolution;
	uint16_t Counter_Value;
	int32_t Distance;
	
	// 扫频数据
	Sweep_Structure* Sweep_wave;
}Driver_Struct;


void Driver_Init(void);

void Driver_Voltage_Output(Driver_Struct* driver, float32_t voltage);
void Driver_Voltage_Output_0(Driver_Struct* driver);
void Driver_DAC_Output(Driver_Struct* driver, uint16_t DAC_value);

void Driver_Switch_CurrentVoltage(Driver_Struct* driver, float32_t voltage);
void Driver_Switch_TargetDisplacement(Driver_Struct* driver, float32_t target);
void Driver_TargetDisplacement_0(Driver_Struct* driver);

void Driver_Switch_Sinwave(float32_t frequency, float32_t amplitude, Driver_Struct* driver);
void Driver_Sinwave_Outout_Start(Driver_Struct* driver);
void Driver_Sinwave_Output_Stop(Driver_Struct* driver);

void Driver_Encoder_Start(Driver_Struct* driver);
void Get_Driver_Encoder_Value(Driver_Struct* driver);
void Clear_Driver_Encoder_Value(Driver_Struct* driver);

// 下列函数会对驱动器结构体中的正弦数组进行更新
void Return_to_AN_SIN_Mode(Driver_Struct* driver);
void Return_to_DIS_SIN_Mode(Driver_Struct* driver);
void Return_to_Chirp_Mode(Driver_Struct* driver);

void Update_Chairp_data(Driver_Struct* driver);		// 更新chairp数据
void Change_Sweep_Parameter(Driver_Struct* driver, float amp, float frequency_begin, float frequency_end, uint32_t time);


#endif




