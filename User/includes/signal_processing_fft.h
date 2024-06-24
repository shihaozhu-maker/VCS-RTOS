#ifndef __SIGNAL_PROCESSING_FFT_H
#define __SIGNAL_PROCESSING_FFT_H

#include "stm32f4xx.h"
#include "stdlib.h"
#include "arm_math.h"
#include "usart.h"

#define DATA_LENGTH							1024
#define SAMPLE_RATE_FFT 				200
#define SAMPLE_TIME_MS					1000/SAMPLE_RATE_FFT


void FFT_Init(void);
void Calculate_fft(void);
void FFT_Data_Update(float32_t data);


#endif


