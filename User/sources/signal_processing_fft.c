#include "signal_processing_fft.h"

float32_t* fft_data;
float32_t* fft_magnitudes;
static int FFT_Current_Index = 0;

void FFT_Init(void)
{
	fft_data = (float32_t*)malloc(4*DATA_LENGTH+64);
	fft_magnitudes = (float32_t*)malloc(4*DATA_LENGTH+64);
	
	if(fft_data)
			memset(fft_data, 0, 4 * DATA_LENGTH);
	else 
		while(1);
	
	if(fft_magnitudes)
			memset(fft_magnitudes, 0, 4 * DATA_LENGTH);
	else 
		while(1);
}

void Calculate_fft()
{
	float32_t fft_out[DATA_LENGTH*2];
	float32_t fft_in[DATA_LENGTH];
	memcpy(fft_in, fft_data, 4*DATA_LENGTH);
	
	
	// 实例化FFT结构体
	arm_rfft_fast_instance_f32 S;

	// 初始化FFT实例，指定FFT的大小
	arm_rfft_fast_init_f32(&S, DATA_LENGTH);


	arm_rfft_fast_f32(&S, fft_in, fft_out, 0);

	// 计算输出复数的幅度
	arm_cmplx_mag_f32(fft_out, fft_magnitudes, DATA_LENGTH);

	// 寻找幅度最大值及其索引
	float32_t max_value;
	uint32_t max_index;
	arm_max_f32(fft_magnitudes, DATA_LENGTH / 2, &max_value, &max_index);  // 只考虑到Nyquist频率
	max_value/=(DATA_LENGTH/2);

	// 计算最大幅度对应的频率
	float32_t frequency = (float32_t)max_index * SAMPLE_RATE_FFT / DATA_LENGTH;
	if(max_index==0)
		max_value/=2;
	
	// 串口打印fft结果
	//printf("Max amplitude at frequency: %f Hz with value: %f\n", frequency, max_value);
}


void FFT_Data_Update(float32_t data)
{
	if((++(FFT_Current_Index))>=DATA_LENGTH)
	{
		FFT_Current_Index = 0;
	}
	
	fft_data[FFT_Current_Index]=data;
}








