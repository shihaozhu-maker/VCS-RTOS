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
	
	
	// ʵ����FFT�ṹ��
	arm_rfft_fast_instance_f32 S;

	// ��ʼ��FFTʵ����ָ��FFT�Ĵ�С
	arm_rfft_fast_init_f32(&S, DATA_LENGTH);


	arm_rfft_fast_f32(&S, fft_in, fft_out, 0);

	// ������������ķ���
	arm_cmplx_mag_f32(fft_out, fft_magnitudes, DATA_LENGTH);

	// Ѱ�ҷ������ֵ��������
	float32_t max_value;
	uint32_t max_index;
	arm_max_f32(fft_magnitudes, DATA_LENGTH / 2, &max_value, &max_index);  // ֻ���ǵ�NyquistƵ��
	max_value/=(DATA_LENGTH/2);

	// ���������ȶ�Ӧ��Ƶ��
	float32_t frequency = (float32_t)max_index * SAMPLE_RATE_FFT / DATA_LENGTH;
	if(max_index==0)
		max_value/=2;
	
	// ���ڴ�ӡfft���
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








