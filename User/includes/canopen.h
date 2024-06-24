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

// 对象字典
#define AI_CONTROL_MODE					0x201B
#define STATUS_WORD							0x6041
#define CONTROL_WORD						0x6040
#define MODE_OF_OPERATION				0x6060
#define MODE_OF_OPERATION_SHOW	0x6061
#define TARGET_TORQUE						0x6071
#define TORQUE_SLOPE						0x6087

// 音圈电机参数
#define RATED_TORQUE						15
 
typedef struct		
{
	uint8_t Status;
}Canopen_Structure;

void Canopen_Init(void);
void Run_NMT(void);

void Read_SDO(uint16_t index, uint8_t sub_index, uint8_t id);																							// 使用SDO报文读取对象字典
void Write_SDO(uint16_t index, uint8_t sub_index, uint32_t data, uint8_t data_byte, uint8_t id);					// 使用SDO报文写入对象字典

void Config_RPDO2(uint8_t id);																																			// 重映射RPDO2
void Write_RPDO1(uint32_t data_l, uint32_t data_h, uint8_t data_byte, uint8_t id);												// 向RPDO1写入数据
void Write_RPDO2(uint32_t data_l, uint32_t data_h, uint8_t data_byte, uint8_t id);												// 向RPDO2写入数据

void Enable_Driver(uint8_t id);												// 驱动器使能
void Disable_Driver(uint8_t id);											// 驱动器禁能
void Profile_Torque_Mode_Config(uint8_t id);					// 转矩轮廓模式配置
void Target_Torque_Set(float torque, uint8_t id);			// 设置目标转矩
void AI_Mode_Config(uint8_t mode, uint16_t id);										// 切换AI控制模式

#endif



