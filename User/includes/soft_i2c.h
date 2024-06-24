#ifndef __SOFT_I2C_H
#define __SOFT_I2C_H

#include "stm32f4xx.h"
#include "delay.h"

#define I2C_SCL_GPIO_PORT			GPIOB
#define I2C_SCL_GPIO_PIN			GPIO_Pin_10
#define I2C_SCL_GPIO_RCC			RCC_AHB1Periph_GPIOB

#define I2C_SDA_GPIO_PORT			GPIOB
#define I2C_SDA_GPIO_PIN			GPIO_Pin_11
#define I2C_SDA_GPIO_RCC			RCC_AHB1Periph_GPIOB



#define SCL_High()						GPIO_SetBits(I2C_SCL_GPIO_PORT, I2C_SCL_GPIO_PIN)
#define SCL_Low()							GPIO_ResetBits(I2C_SCL_GPIO_PORT, I2C_SCL_GPIO_PIN)

#define SDA_High()						GPIO_SetBits(I2C_SDA_GPIO_PORT, I2C_SDA_GPIO_PIN)
#define SDA_Low()						GPIO_ResetBits(I2C_SDA_GPIO_PORT, I2C_SDA_GPIO_PIN)



void I2C_Software_Init(void);
void I2C_Start(void);
void I2C_Stop(void);
uint8_t I2C_Wait_Ack(void);
void I2C_Ack(void);
void I2C_NAck(void);
void I2C_Send_Byte(uint8_t txd);
uint8_t I2C_Read_Byte(void);

#endif




