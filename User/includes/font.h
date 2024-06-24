#ifndef __CODETAB_H
#define __CODETAB_H

#include "stm32f4xx.h"

typedef struct
{
	uint8_t Font_data[32];
	uint32_t Font_Width;
	uint32_t Font_Hight;
}Font;

typedef struct
{
	uint8_t* Font_data;
	uint32_t Font_Width;
	uint32_t Font_Hight;
	uint32_t data_number;
}Font_Pointer;



#endif /* __CODETAB_H */



