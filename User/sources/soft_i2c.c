#include "soft_i2c.h"

static void I2C_SDA_Write(uint8_t data);
static uint8_t I2C_SDA_Read(void);

static void I2C_SDA_Write(uint8_t data)
{
	if(data)
		SDA_High();
	else
		SDA_Low();
}

static uint8_t I2C_SDA_Read(void)
{
	return GPIO_ReadInputDataBit(I2C_SDA_GPIO_PORT, I2C_SDA_GPIO_PIN);
}



void I2C_Software_Init(void)
{
	RCC_AHB1PeriphClockCmd(I2C_SCL_GPIO_RCC|I2C_SDA_GPIO_RCC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Pin = I2C_SCL_GPIO_PIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_Init(I2C_SCL_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = I2C_SDA_GPIO_PIN;
	GPIO_Init(I2C_SDA_GPIO_PORT, &GPIO_InitStructure);
}

void I2C_Start(void)
{
	SDA_High();
	SCL_High();
	delay_us(4);
 	SDA_Low();
	delay_us(4);
	SCL_Low();
	SDA_High();
}

void I2C_Stop(void)
{
	SDA_Low();
	SCL_High();
 	delay_us(4);
	SDA_High();
	delay_us(4);
	SCL_Low();
	SDA_High();
}

uint8_t I2C_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	SDA_High(); 
	SCL_High();
	delay_us(1);	 
	while(I2C_SDA_Read())
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			SCL_Low();
			I2C_Stop();
			return 1;		// 超时
		}
	}
	SCL_Low();   
	return 0;  
}

void I2C_Ack(void)
{
	SDA_Low();
	SCL_High();
	delay_us(2);
	SCL_Low();
	SDA_High();
}

void I2C_NAck(void)
{
	SDA_High();
	SCL_High();
	delay_us(2);
	SCL_Low();
	SDA_High();
}

void I2C_Send_Byte(uint8_t txd)
{
	uint8_t index; 	    
  for(index=0;index<8;index++)
	{    
		I2C_SDA_Write((txd&0x80)>>7);
    txd<<=1; 	  
		delay_us(2);   
		SCL_High();
		delay_us(2); 
		SCL_Low();	
		delay_us(2);
	}
	SCL_Low();
	SDA_High();
}

uint8_t I2C_Read_Byte(void)
{
	uint8_t i;
	uint8_t value;
	
	/* 读到第1个bit为数据的bit7 */
	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		SCL_High();
		delay_us(2);
		if (I2C_SDA_Read())
		{
			value++;
		}
		SCL_Low();
		delay_us(2);
	}
	SCL_Low();
	SDA_High();
	return value;
}






