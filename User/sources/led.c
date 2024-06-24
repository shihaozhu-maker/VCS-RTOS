#include "led.h"

void LED_Init(void)
{
	RCC_AHB1PeriphClockCmd(LED_L1_RCCPORT|LED_L2_RCCPORT|LED_L3_RCCPORT|LED_L4_RCCPORT|LED_R1_RCCPORT|LED_R2_RCCPORT, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = LED_L1_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Fast_Speed;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(LED_L1_PORT, &GPIO_InitStruct);
	GPIO_SetBits(LED_L1_PORT, LED_L1_PIN);
	
	GPIO_InitStruct.GPIO_Pin = LED_L2_PIN;
	GPIO_Init(LED_L2_PORT, &GPIO_InitStruct);
	GPIO_SetBits(LED_L2_PORT, LED_L2_PIN);
	
	GPIO_InitStruct.GPIO_Pin = LED_L3_PIN;
	GPIO_Init(LED_L3_PORT, &GPIO_InitStruct);
	GPIO_SetBits(LED_L3_PORT, LED_L3_PIN);
	
	GPIO_InitStruct.GPIO_Pin = LED_L4_PIN;
	GPIO_Init(LED_L4_PORT, &GPIO_InitStruct);
	GPIO_SetBits(LED_L4_PORT, LED_L4_PIN);
	
	GPIO_InitStruct.GPIO_Pin = LED_R1_PIN;
	GPIO_Init(LED_R1_PORT, &GPIO_InitStruct);
	GPIO_SetBits(LED_R1_PORT, LED_R1_PIN);
	
	GPIO_InitStruct.GPIO_Pin = LED_R2_PIN;
	GPIO_Init(LED_R2_PORT, &GPIO_InitStruct);
	GPIO_SetBits(LED_R2_PORT, LED_R2_PIN);
}

void LED_On(uint8_t led)
{
	switch(led)
	{
		case 1:
			GPIO_ResetBits(LED_L1_PORT, LED_L1_PIN);
			break;
		case 2:
			GPIO_ResetBits(LED_L2_PORT, LED_L2_PIN);
			break;
		case 3:
			GPIO_ResetBits(LED_L3_PORT, LED_L3_PIN);
			break;
		case 4:
			GPIO_ResetBits(LED_L4_PORT, LED_L4_PIN);
			break;
		case 5:
			GPIO_ResetBits(LED_R1_PORT, LED_R1_PIN);
			break;
		case 6:
			GPIO_ResetBits(LED_R2_PORT, LED_R2_PIN);
			break;
		default:
			break;
	}
}

void LED_On_Mask(uint8_t led)
{
	uint8_t mask = led;
	if(mask&0x01)
		GPIO_ResetBits(LED_L1_PORT, LED_L1_PIN);
	if(mask&0x02)
		GPIO_ResetBits(LED_L2_PORT, LED_L2_PIN);
	if(mask&0x04)
		GPIO_ResetBits(LED_L3_PORT, LED_L3_PIN);
	if(mask&0x08)
		GPIO_ResetBits(LED_L4_PORT, LED_L4_PIN);
	if(mask&0x10)
		GPIO_ResetBits(LED_R1_PORT, LED_R1_PIN);
	if(mask&0x20)
		GPIO_ResetBits(LED_R2_PORT, LED_R2_PIN);
}


void LED_Off(uint8_t led)
{
	switch(led)
	{
		case 1:
			GPIO_SetBits(LED_L1_PORT, LED_L1_PIN);
			break;
		case 2:
			GPIO_SetBits(LED_L2_PORT, LED_L2_PIN);
			break;
		case 3:
			GPIO_SetBits(LED_L3_PORT, LED_L3_PIN);
			break;
		case 4:
			GPIO_SetBits(LED_L4_PORT, LED_L4_PIN);
			break;
		case 5:
			GPIO_SetBits(LED_R1_PORT, LED_R1_PIN);
			break;
		case 6:
			GPIO_SetBits(LED_R2_PORT, LED_R2_PIN);
			break;
		default:
			break;
	}
}

void LED_Off_Mask(uint8_t led)
{
	uint8_t mask = led;
	if(mask&0x01)
		GPIO_SetBits(LED_L1_PORT, LED_L1_PIN);
	if(mask&0x02)
		GPIO_SetBits(LED_L2_PORT, LED_L2_PIN);
	if(mask&0x04)
		GPIO_SetBits(LED_L3_PORT, LED_L3_PIN);
	if(mask&0x08)
		GPIO_SetBits(LED_L4_PORT, LED_L4_PIN);
	if(mask&0x10)
		GPIO_SetBits(LED_R1_PORT, LED_R1_PIN);
	if(mask&0x20)
		GPIO_SetBits(LED_R2_PORT, LED_R2_PIN);
}


void LED_OverTurn(uint8_t led)
{
	uint8_t current_status;
	switch(led)
	{
		case 1:
		{
			current_status = GPIO_ReadOutputDataBit(LED_L1_PORT, LED_L1_PIN);
			if(current_status)
				GPIO_WriteBit(LED_L1_PORT, LED_L1_PIN, Bit_RESET);
			else
				GPIO_WriteBit(LED_L1_PORT, LED_L1_PIN, Bit_SET);
			break;
		}
		case 2:
		{
			current_status = GPIO_ReadOutputDataBit(LED_L2_PORT, LED_L2_PIN);
			if(current_status)
				GPIO_WriteBit(LED_L2_PORT, LED_L2_PIN, Bit_RESET);
			else
				GPIO_WriteBit(LED_L2_PORT, LED_L2_PIN, Bit_SET);
			break;
		}
		case 3:
		{
			current_status = GPIO_ReadOutputDataBit(LED_L3_PORT, LED_L3_PIN);
			if(current_status)
				GPIO_WriteBit(LED_L3_PORT, LED_L3_PIN, Bit_RESET);
			else
				GPIO_WriteBit(LED_L3_PORT, LED_L3_PIN, Bit_SET);
			break;
		}
		case 4:
		{
			current_status = GPIO_ReadOutputDataBit(LED_L4_PORT, LED_L4_PIN);
			if(current_status)
				GPIO_WriteBit(LED_L4_PORT, LED_L4_PIN, Bit_RESET);
			else
				GPIO_WriteBit(LED_L4_PORT, LED_L4_PIN, Bit_SET);
			break;
		}
		case 5:
		{
			current_status = GPIO_ReadOutputDataBit(LED_R1_PORT, LED_R1_PIN);
			if(current_status)
				GPIO_WriteBit(LED_R1_PORT, LED_R1_PIN, Bit_RESET);
			else
				GPIO_WriteBit(LED_R1_PORT, LED_R1_PIN, Bit_SET);
			break;
		}
		case 6:
		{
			current_status = GPIO_ReadOutputDataBit(LED_R2_PORT, LED_R2_PIN);
			if(current_status)
				GPIO_WriteBit(LED_R2_PORT, LED_R2_PIN, Bit_RESET);
			else
				GPIO_WriteBit(LED_R2_PORT, LED_R2_PIN, Bit_SET);
			break;
		}
		default:
			break;
	}	
}

void LED_OverTurn_Mask(uint8_t led)
{
	uint8_t mask = led;
	if(mask&0x01)
		LED_OverTurn(1);
	if(mask&0x02)
		LED_OverTurn(2);
	if(mask&0x04)
		LED_OverTurn(3);
	if(mask&0x08)
		LED_OverTurn(4);
	if(mask&0x10)
		LED_OverTurn(5);
	if(mask&0x20)
		LED_OverTurn(6);
}

