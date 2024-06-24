#ifndef __CAN_OPEN_H
#define __CAN_OPEN_H

#include "stm32f4xx.h"
#include "delay.h"

#define CAN_RX_GPIO							GPIOA
#define CAN_RX_GPIOPIN					GPIO_Pin_11
#define CAN_RX_GPIOPINSOURCE		GPIO_PinSource11
#define CAN_RX_GPIORCC					RCC_AHB1Periph_GPIOA

#define CAN_TX_GPIO							GPIOA
#define CAN_TX_GPIOPIN					GPIO_Pin_12
#define CAN_TX_GPIOPINSOURCE		GPIO_PinSource12
#define CAN_TX_GPIORCC					RCC_AHB1Periph_GPIOA

#define CANOPEN_CAN							CAN1
#define CANOPEN_CAN_RCC					RCC_APB1Periph_CAN1

// �����ֵ�
#define AI_CONTROL_MODE					0x201B
#define STATUS_WORD							0x6041
#define CONTROL_WORD						0x6040
#define MODE_OF_OPERATION				0x6060
#define MODE_OF_OPERATION_SHOW	0x6061
#define TARGET_TORQUE						0x6071
#define TORQUE_SLOPE						0x6087

// ��Ȧ�������
#define RATED_TORQUE						15
 
typedef struct		
{
	uint8_t Status;
}Canopen_Structure;

void Canopen_Init(void);
void Run_NMT(void);

void Read_SDO(uint16_t index, uint8_t sub_index, uint8_t id);																							// ʹ��SDO���Ķ�ȡ�����ֵ�
void Write_SDO(uint16_t index, uint8_t sub_index, uint32_t data, uint8_t data_byte, uint8_t id);					// ʹ��SDO����д������ֵ�

void Config_RPDO2(uint8_t id);																																			// ��ӳ��RPDO2
void Write_RPDO1(uint32_t data_l, uint32_t data_h, uint8_t data_byte, uint8_t id);												// ��RPDO1д������
void Write_RPDO2(uint32_t data_l, uint32_t data_h, uint8_t data_byte, uint8_t id);												// ��RPDO2д������

void Enable_Driver(uint8_t id);												// ������ʹ��
void Disable_Driver(uint8_t id);											// ����������
void Profile_Torque_Mode_Config(uint8_t id);					// ת������ģʽ����
void Target_Torque_Set(float torque, uint8_t id);			// ����Ŀ��ת��
void AI_Mode_Config(uint8_t mode, uint16_t id);										// �л�AI����ģʽ

#endif



