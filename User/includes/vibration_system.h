#ifndef __VIBRATION_SYSTEM_H
#define __VIBRATION_SYSTEM_H

#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "encoder.h"
#include "delay.h"
#include "led.h"
#include "usart.h"
#include "key.h"
#include "driver.h"
#include "oled.h"
#include "delay_feedback.h"
#include "canopen.h"
#include "pid.h"
#include "spi.h"

#include "signal_processing_fft.h"

void Vibration_System_Init(void);



#endif



