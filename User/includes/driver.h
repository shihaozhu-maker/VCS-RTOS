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

/*ADC��ѹ���=�趨ֵ-VOLTAGE_OUTPUT_BIAS*/
#define VOLTAGE_OUTPUT_BIAS					1.65l
#define VOLTAGE_OUTPUT_STANDARD			1				// �����ѹ�����궨

// TIMER
#define DRIVER_TIMER								TIM2
#define DRIVER_TIMER_RCC						RCC_APB1Periph_TIM2	
#define DRIVER_CONTROL_PERIOD				(int)(DRIVER_UPDATE_TIME*84000000)	

#define DRIVER_UPDATE_TIME					0.0005f		// 0.5ms����һ��DAC���������ѭ��ʱ�䣩
#define SINWAVE_DATA_LENGTH					2000			// ����������󳤶�
#define SINWAVE_MIN_FREQUENCY				1
#define SINWAVE_MAX_FREQUENCY				20

#define SWEEP_UPDATE_TIME						2					// sweep�������ʱ�䣬�Կ���ѭ��ʱ��Ϊ��λ����2��ʾ��������ѭ�����һ��sweep�ź�

#define VOLTAGE_MAX									5l					// �궨���ʵ�ֵ����
#define VOLTAGE_MIN									-5l				

#define VOLTAGE_OUT_MAX							7.5l				// ���ư�ʵ�ʿ����ģ���ѹ����
#define VOLTAGE_OUT_MIN							-6.2l

#define DISPLACE_MAX								12.0l				// ������λ�� mm
#define DISPLACE_MIN								-12.0l			// ��С����λ�� mm

#define SWEEP_FREQUENCY_MAX					99.9f
#define SWEEP_TIME_MAX							999

typedef struct
{
	// ��ѹ����
	float32_t (*Sinwave_data1)[SINWAVE_DATA_LENGTH+1];	// sin�������� row1:time		row2:sin_data		table[0][0]������Чλ��
	float32_t (*Sinwave_data2)[SINWAVE_DATA_LENGTH+1];
	uint16_t sindata_number;														// ��Ч�������ݸ���
	float32_t current_frequency;												// ��ǰsinƵ�ʣ�>1��
	float32_t current_amplitude;												// ��ǰsin��ֵ��<8��
	uint8_t current_data_table;													// ��ǰDACת���Ĳ������� 1 or 2
	uint16_t current_sindata_index ;										// ��ǰDACת���Ĳ�����������
}Sinwave_Struct;

// ɨƵ�ṹ��
typedef struct
{
	float frequency_begin;		// ɨƵƵ�ʷ�Χ ��λHz
	float frequency_end;

	float amplitude;					// ɨƵ��ֵ
	uint32_t time;								// ɨƵʱ�� ��λs
	uint32_t interval_time;				// ɨƵ���ʱ��
	uint8_t output_enable;		// ɨƵ�ź����ʹ��
	
	uint8_t mode_option;			// 1Ϊ��ѹģʽ��Ĭ�ϣ���2Ϊλ��ģʽ
}Sweep_Structure;


typedef struct
{
	uint8_t ID;														// 1 or 2
	uint8_t Status;												// 0�豸δ���� 1������ģ�����ģʽ 2������������� 3λ�ÿ���ģʽ 4����λ��ģʽ 5������ģʽ 6��ѹchirpģʽ 7λ��chirpģʽ
	
	Sinwave_Struct* Sinwave;
	
	// ģ���ѹ���
	uint16_t DAC_Value;										// ANģʽ��DAC�����ֵ
	float32_t Output_Voltage; 						// ANģʽ��DAC�����ѹ
	float32_t frequency_force;						// ��������ģʽ�ĵ�ѹƵ��
	float32_t amplitude_force;						// ��������ģʽ�ĵ�ѹ��ֵ
	
	// λ��PID���
	float32_t Target_Displacement;					// DISģʽ�µ�Ŀ��λ��
	float32_t frequency_displacement;				// λ������ģʽ��λ��Ƶ��
	float32_t amplitude_displacement;				// λ������ģʽ��λ�Ʒ�ֵ
	
	// ������
	uint8_t Resolution;
	uint16_t Counter_Value;
	int32_t Distance;
	
	// ɨƵ����
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

// ���к�������������ṹ���е�����������и���
void Return_to_AN_SIN_Mode(Driver_Struct* driver);
void Return_to_DIS_SIN_Mode(Driver_Struct* driver);
void Return_to_Chirp_Mode(Driver_Struct* driver);

void Update_Chairp_data(Driver_Struct* driver);		// ����chairp����
void Change_Sweep_Parameter(Driver_Struct* driver, float amp, float frequency_begin, float frequency_end, uint32_t time);


#endif




