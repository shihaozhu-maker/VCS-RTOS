#ifndef __OLED_H
#define __OLED_H

#include "stm32f4xx.h"
#include "soft_i2c.h"
#include "font.h"
#include "driver.h"
#include "encoder.h"
#include "delay_feedback.h"
#include "canopen.h"

#define OLED_DEV_ADDR 								0x78
#define OLED_WRITE										0x78
#define OLED_READ											0x79
#define OLED_MAX_TRIALS_NUMBER      	10			/*  设备检测的最大次数 如超过最大次数还没检测到设备则视为超时 */ 

#define OLED_EVENT_UPDATE							1<<0
#define OLED_EVENT_UPDATE_FLASH
#define OLED_EVENT_UPDATE_DATA				1<<1
#define OLED_EVENT_UPDATE_CURSOR			1<<2

/*
 * 选项（光标）结构体
 *
 * Normal_Option
 * 0		none
 * 1		amplitude
 * 2		fequency
 
 * Encoder_Data_Tx
 * 0		none
 * 1		Dr1
 * 2		Dr2
 * 3		En1
 * 4		En2
 
 * DRCx_DataSource_Option
 * 0		none
 * 1		Dr1
 * 2		Dr2
 * 3		En1
 * 4		En2
 
 * Driver_Current_Status
 * 0		ID1 disable		ID2 disable
 * 1		ID1 disable		ID2 enable
 * 2		ID1 enable		ID2 disable
 * 3		ID1 enable		ID2 enable
 
 * Driver_CAN_Out
 * 0		ID1 disable		ID2 disable
 * 1		ID1 disable		ID2 enable
 * 2		ID1 enable		ID2 disable
 * 3		ID1 enable		ID2 enable
*/

typedef struct
{
	uint8_t Normal_Option;
	
	uint8_t Usart_DataTx_Option;
	uint8_t DRC1_DataSource_Option;
	uint8_t DRC2_DataSource_Option;
	
	uint8_t Driver_Current_Status; 
	uint8_t Driver_CAN_Out;
	uint8_t Sweep1_Mode;
	uint8_t Sweep2_Mode;
}Options_Struct;

typedef struct
{
	uint8_t Status;
}OLED_Struct;	

uint8_t OLED_CheckDevice(void);
void OLED_WriteCmd(uint8_t OLED_Command);
void OLED_WriteData(uint8_t OLED_Data);
void OLED_Init(void);
void OLED_Display_Video_Memory(char Video_Memory[8][128], uint8_t x, uint8_t y);
void OLED_Show_Matrix_Pointer(unsigned char x, unsigned char y, Font_Pointer pointer, unsigned char N);
void OLED_Show_Matrix(unsigned char x, unsigned char y, Font* const caodetab, unsigned char N);										// 针对小尺寸字模
void OLED_Show_Number(unsigned char x, unsigned char y, uint8_t number);
void OLED_SetPos(uint8_t x , uint8_t y);

void OLED_Update(void);
void OLED_Update_Data(void);
uint32_t OLED_Data_Round(float32_t data);
void OLED_Update_Cursor(void);



#endif



