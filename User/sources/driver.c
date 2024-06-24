#include "driver.h"

const float32_t Standard_Voltage_Table[101] = {
-4.87872340425532,-4.77400000000000,-4.67234042553192,-4.56800000000000,-4.46800000000000,
-4.36734693877551,-4.26600000000000,-4.16458333333333,-4.06200000000000,-3.96200000000000,
-3.85957446808511,-3.75686274509804,-3.65434782608696,-3.55098039215686,-3.45200000000000,
-3.35200000000000,-3.25200000000000,-3.14893617021277,-3.04705882352941,-2.94693877551020,
-2.84600000000000,-2.74600000000000,-2.64489795918367,-2.54400000000000,-2.44166666666667,
-2.34230769230769,-2.24400000000000,-2.13913043478261,-2.03600000000000,-1.93469387755102,
-1.83125000000000,-1.73000000000000,-1.62857142857143,-1.52653061224490,-1.42291666666667,
-1.31875000000000,-1.21960784313725,-1.11111111111111,-1.01176470588235,-0.904347826086956,
-0.807692307692308,-0.706122448979591,-0.598148148148148,-0.505555555555556,-0.404081632653061,
-0.315789473684211,-0.222641509433962,-0.120833333333333,-0.0506172839506168,0.0102272727272729,
0.0670454545454546,0.141176470588235,0.241666666666666,0.344000000000000,0.444897959183673,
0.546938775510204,0.648000000000000,0.748979591836735,0.851020408163265,0.950980392156862,
1.05208333333333,1.15400000000000,1.25510204081633,1.35600000000000,1.45600000000000,
1.55833333333333,1.65882352941176,1.76041666666667,1.86200000000000,1.96326530612245,
2.06530612244898,2.16470588235294,2.26666666666667,2.36800000000000,2.46666666666667,
2.56875000000000,2.66730769230769,2.76734693877551,2.86938775510204,2.97000000000000,
3.07142857142857,3.17346938775510,3.27551020408163,3.37600000000000,3.47916666666667,
3.57843137254902,3.67647058823529,3.77755102040816,3.87959183673469,3.98333333333333,
4.08235294117647,4.18367346938776,4.28750000000000,4.38627450980392,4.48958333333333,
4.59000000000000,4.69000000000000,4.79183673469388,4.89200000000000,4.99787234042553,5.09423076923077
};

static void Driver1_Init(void);
static void Driver2_Init(void);
static void Driver_Timer_Init(void);
static void Driver1_Encoder_Init(void);
static void Driver2_Encoder_Init(void);
static void Driver_Generate_Sinwave(float32_t frequency, float32_t amplitude, Driver_Struct* driver, uint8_t time_update_flag);

// driver1 ���������������
float32_t data11[2][SINWAVE_DATA_LENGTH+1] = {{0},{0}};
float32_t data12[2][SINWAVE_DATA_LENGTH+1] = {{0},{0}};
// driver2 ���������������
float32_t data21[2][SINWAVE_DATA_LENGTH+1] = {{0},{0}};
float32_t data22[2][SINWAVE_DATA_LENGTH+1] = {{0},{0}};


Sweep_Structure Sweep1 = {
	.frequency_begin = 0.0,
	.frequency_end = 0.0,
	.amplitude = 0.0,
	.time = 0,
	.interval_time = 20,
	.output_enable = 0,
	.mode_option = 1
};

Sweep_Structure Sweep2 = {
	.frequency_begin = 0.0,
	.frequency_end = 0.0,
	.amplitude = 0.0,
	.time = 0,
	.interval_time = 20,
	.output_enable = 0,
	.mode_option = 1
};

Sinwave_Struct Sinewave1 = {
	.Sinwave_data1 = data11,
	.Sinwave_data2 = data12,
	.sindata_number = 0,
	.current_frequency = 0,
	.current_amplitude = 0,
	.current_data_table = 1,
	.current_sindata_index = 0
};

Sinwave_Struct Sinewave2 = {
	.Sinwave_data1 = data21,
	.Sinwave_data2 = data22,
	.sindata_number = 0,
	.current_frequency = 0,
	.current_amplitude = 0,
	.current_data_table = 1,
	.current_sindata_index = 0
};

Driver_Struct Driver1 = {
	.ID = 1,
	.Status = 0,
	.DAC_Value = 0,
	.Output_Voltage = 0, 
	.Resolution = ENCODER_RESOLUTION,
	.Counter_Value = 0,
	.Distance = 0,
	.Sinwave = &Sinewave1,
	.frequency_force = 0,
	.amplitude_force = 0,
	
	.Target_Displacement = 0,
	.frequency_displacement = 0,
	.amplitude_displacement = 0,
	
	.Sweep_wave = &Sweep1
};

Driver_Struct Driver2 = {
	.ID = 2,
	.Status = 0,
	.DAC_Value = 0,
	.Output_Voltage = 0, 
	.Resolution = ENCODER_RESOLUTION,
	.Counter_Value = 0,
	.Distance = 0,
	.Sinwave = &Sinewave2,
	.frequency_force = 0,
	.amplitude_force = 0,
	
	.Target_Displacement = 0,
	.frequency_displacement = 0,
	.amplitude_displacement = 0,
	
	.Sweep_wave = &Sweep2
};

/*******************************************��������ʼ��*******************************************/
void Driver_Init(void)
{
	// ��ʼ��DAC���������ʱ��
	Driver_Timer_Init();
	
	// ��������ʼ��
	Driver1_Encoder_Init();
	Driver2_Encoder_Init();
	
	// ��������ʼ��
	Driver1_Init();
	Driver2_Init();
	
	// ģ�����ģʽ
	Driver1.Status = 1;
	Driver2.Status = 1;
	
	Driver_Voltage_Output_0(&Driver1);
	Driver_Voltage_Output_0(&Driver2);
	
	// ������Ҳ�������ʱ������
	uint16_t i = 0;
	for(i=1;i<=SINWAVE_DATA_LENGTH;i++)
	{
		Sinewave1.Sinwave_data1[0][i] = (i-1)*DRIVER_UPDATE_TIME;
		Sinewave1.Sinwave_data2[0][i] = (i-1)*DRIVER_UPDATE_TIME;
		Sinewave2.Sinwave_data1[0][i] = (i-1)*DRIVER_UPDATE_TIME;
		Sinewave2.Sinwave_data2[0][i] = (i-1)*DRIVER_UPDATE_TIME;
	}
}

/*******************************************������1��ʼ��*******************************************/
static void Driver1_Init(void)
{
	// GPIO
	RCC_AHB1PeriphClockCmd(DRIVER1_AI_GPIORCC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_Pin = DRIVER1_AI_GPIOPIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(DRIVER1_AI_GPIOPORT, &GPIO_InitStructure);
	
	// DAC
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	
	DAC_InitTypeDef DAC_InitStructure;
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_T2_TRGO;			// TIMER2����
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_Init(DAC_Channel_1, &DAC_InitStructure);
	
	DAC_Cmd(DAC_Channel_1, ENABLE);
}

/*******************************************������2��ʼ��*******************************************/
static void Driver2_Init(void)
{
	// GPIO
	RCC_AHB1PeriphClockCmd(DRIVER2_AI_GPIORCC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_Pin = DRIVER2_AI_GPIOPIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(DRIVER2_AI_GPIOPORT, &GPIO_InitStructure);
	
	// DAC
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	
	DAC_InitTypeDef DAC_InitStructure;
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_T2_TRGO;			// TIMER2����
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_Init(DAC_Channel_2, &DAC_InitStructure);
	
	DAC_Cmd(DAC_Channel_2, ENABLE);
}

/*******************************************������1��ʼ��*******************************************/
static void Driver1_Encoder_Init(void)
{
	// GPIO
	RCC_AHB1PeriphClockCmd(DRIVER1_ENCODER_A_GPIORCC|DRIVER1_ENCODER_B_GPIORCC|DRIVER1_ENCODER_Z_GPIORCC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = DRIVER1_ENCODER_A_GPIOPIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_Init(DRIVER1_ENCODER_A_GPIOPORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(DRIVER1_ENCODER_A_GPIOPORT, DRIVER1_ENCODER_A_PINSOURCE, GPIO_AF_TIM1);
	
	GPIO_InitStructure.GPIO_Pin = DRIVER1_ENCODER_B_GPIOPIN;
	GPIO_Init(DRIVER1_ENCODER_B_GPIOPORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(DRIVER1_ENCODER_B_GPIOPORT, DRIVER1_ENCODER_B_PINSOURCE, GPIO_AF_TIM1);
	
	GPIO_InitStructure.GPIO_Pin = DRIVER1_ENCODER_Z_GPIOPIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(DRIVER1_ENCODER_Z_GPIOPORT, &GPIO_InitStructure);
	
	// TIMER
	RCC_APB2PeriphClockCmd(DRIVER1_ENCODER_TIMER_RCC, ENABLE);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);
	TIM_TimeBaseInitStructure.TIM_Period = DRIVER1_ENCODER_TIMER_PERIOD;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1;		// 2��Ƶ
	TIM_TimeBaseInit(DRIVER1_ENCODER_TIMER, &TIM_TimeBaseInitStructure);
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 5;		// 84/2/8=5.25
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(DRIVER1_ENCODER_TIMER, &TIM_ICInitStructure);
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInit(DRIVER1_ENCODER_TIMER, &TIM_ICInitStructure);
	
	TIM_EncoderInterfaceConfig(DRIVER1_ENCODER_TIMER, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
	TIM_Cmd(DRIVER1_ENCODER_TIMER, ENABLE);
}

/*******************************************������2��ʼ��*******************************************/
static void Driver2_Encoder_Init(void)
{
	// GPIO
	RCC_AHB1PeriphClockCmd(DRIVER2_ENCODER_A_GPIORCC|DRIVER2_ENCODER_B_GPIORCC|DRIVER2_ENCODER_Z_GPIORCC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = DRIVER2_ENCODER_A_GPIOPIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_Init(DRIVER2_ENCODER_A_GPIOPORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(DRIVER2_ENCODER_A_GPIOPORT, DRIVER2_ENCODER_A_PINSOURCE, GPIO_AF_TIM4);
	
	GPIO_InitStructure.GPIO_Pin = DRIVER2_ENCODER_B_GPIOPIN;
	GPIO_Init(DRIVER2_ENCODER_B_GPIOPORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(DRIVER2_ENCODER_B_GPIOPORT, DRIVER2_ENCODER_B_PINSOURCE, GPIO_AF_TIM4);
	
	GPIO_InitStructure.GPIO_Pin = DRIVER2_ENCODER_Z_GPIOPIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(DRIVER2_ENCODER_Z_GPIOPORT, &GPIO_InitStructure);
	
	// TIMER
	RCC_APB1PeriphClockCmd(DRIVER2_ENCODER_TIMER_RCC, ENABLE);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);
	TIM_TimeBaseInitStructure.TIM_Period = DRIVER2_ENCODER_TIMER_PERIOD;
	TIM_TimeBaseInit(DRIVER2_ENCODER_TIMER, &TIM_TimeBaseInitStructure);
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 5;		// 84/2/8=5.25
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(DRIVER2_ENCODER_TIMER, &TIM_ICInitStructure);
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInit(DRIVER2_ENCODER_TIMER, &TIM_ICInitStructure);
	
	TIM_EncoderInterfaceConfig(DRIVER2_ENCODER_TIMER, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
	TIM_Cmd(DRIVER2_ENCODER_TIMER, ENABLE);
}

/*******************************************DAC����ʱ�ӳ�ʼ��*******************************************/
static void Driver_Timer_Init(void)
{
	RCC_APB1PeriphClockCmd(DRIVER_TIMER_RCC, ENABLE);
	
	// ʱ��TimeBase 168/4*2=84M
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);
	TIM_TimeBaseInitStructure.TIM_Period = DRIVER_CONTROL_PERIOD;
	TIM_TimeBaseInit(DRIVER_TIMER, &TIM_TimeBaseInitStructure);
	
	// DAC����
	TIM_SelectOutputTrigger(DRIVER_TIMER, TIM_TRGOSource_Update);
	
	// ��������ж�
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
	TIM_ITConfig(DRIVER_TIMER, TIM_IT_Update, ENABLE);
	
	TIM_Cmd(DRIVER_TIMER, ENABLE);
}

/*******************************************�����������ѹ*******************************************/
// voltage1,voltage2�ֱ�Ϊ�����������1,2�ĵ�ѹ
// ADCģ���ѹ		��������ѹ
// 3.3						-8
// 3.3/2					0
// 0							8
void Driver_Voltage_Output(Driver_Struct* driver, float32_t voltage)
{
	uint16_t DAC_value;
	float32_t DAC_vlotage;
	
	float32_t Standard_vlotage;
	uint8_t index_min;
	uint8_t index_max;

	if((driver->Status == 1)||(driver->Status == 2)||(driver->Status == 3)||(driver->Status == 4)||(driver->Status == 6))
	{
		if(VOLTAGE_OUTPUT_STANDARD)		// �����ѹ�궨
		{
			Standard_vlotage = ((voltage+5)*10);
			if(Standard_vlotage>0)
				index_min = (uint8_t)Standard_vlotage;
			else
				index_min = (uint8_t)(-Standard_vlotage);
			
			// ��֤index_min��index_max�ڱ궨�����±귶Χ��
			if(index_min>99)
				index_min=99;
			index_max = index_min+1;
		
			Standard_vlotage = Standard_Voltage_Table[index_min]+(Standard_Voltage_Table[index_max]-Standard_Voltage_Table[index_min])*(voltage-(index_min*0.1l-5))/0.1l;
		}
		else
			Standard_vlotage = voltage;
		
		
		if(Standard_vlotage>VOLTAGE_OUT_MAX)
			Standard_vlotage = VOLTAGE_OUT_MAX;
		else if(Standard_vlotage<VOLTAGE_OUT_MIN)
			Standard_vlotage=VOLTAGE_OUT_MIN;
		
		DAC_vlotage = -Standard_vlotage*3.3f/16+3.3f/2+VOLTAGE_OUTPUT_BIAS*3.3l/16;
		DAC_value = (uint16_t)(DAC_vlotage*4095/3.3f);
		
		if(driver->ID == 1)
			DAC_SetChannel1Data(DAC_Align_12b_R, DAC_value);
		else if(driver->ID == 2)
			DAC_SetChannel2Data(DAC_Align_12b_R, DAC_value);
	}
}

/*******************************************�����������ѹ��DAC��ֵ��*******************************************/
void Driver_DAC_Output(Driver_Struct* driver, uint16_t DAC_value)
{
	if((driver->Status == 1)||(driver->Status == 2)||(driver->Status == 3))
	{
		if(DAC_value>0xfff)
			DAC_value = 0xfff;
		
		if(driver->ID == 1)
			DAC_SetChannel1Data(DAC_Align_12b_R, DAC_value);
		else if(driver->ID == 2)
			DAC_SetChannel2Data(DAC_Align_12b_R, DAC_value);
	}
}

/*******************************************�л�ģ���ѹ���*******************************************/
void Driver_Switch_CurrentVoltage(Driver_Struct* driver, float32_t voltage)
{
	if((driver->Status == 1)||(driver->Status == 5))
	{
		uint16_t DAC_value;
		float32_t DAC_vlotage;
		
		if(voltage>VOLTAGE_MAX)
			voltage = VOLTAGE_MAX;
		else if(voltage<VOLTAGE_MIN)
			voltage=VOLTAGE_MIN;
			
		DAC_vlotage = -voltage*3.3f/16+3.3f/2+VOLTAGE_OUTPUT_BIAS*3.3l/16;
		DAC_value = (uint16_t)(DAC_vlotage*4095/3.3f);
			
		driver->Output_Voltage = voltage;
		driver->DAC_Value = DAC_value;
	}
}

/*******************************************�л�λ�ÿ���Ŀ��λ��*******************************************/
void Driver_Switch_TargetDisplacement(Driver_Struct* driver, float32_t target)
{
	if((driver->Status == 3)||(driver->Status == 5))
	{
		if(target>DISPLACE_MAX)
			target = DISPLACE_MAX;
		else if(target<DISPLACE_MIN)
			target=DISPLACE_MIN;
		driver->Target_Displacement = target;
	}
}


/*******************************************���������0��ѹ*******************************************/
/* 
 * �����ǰ��ģʽ1��ģ�����������ֱ�����0��ѹ
 * �����ǰ��ģʽ2���������������������Ϊģʽ1�������0��ѹ
 * 
 * �ú�����ʵ��ģʽ1��ģʽ2�µ������ѹ����
*/
void Driver_Voltage_Output_0(Driver_Struct* driver)
{
	if((driver->Status == 1)||(driver->Status == 5))
		Driver_Switch_CurrentVoltage(driver, 0);
	else if(driver->Status == 2)
		Driver_Switch_Sinwave(driver->Sinwave->current_frequency, 0, driver);
}

/*******************************************λ�ÿ���Ŀ��λ������Ϊ0*******************************************/
void Driver_TargetDisplacement_0(Driver_Struct* driver)
{
	if((driver->Status == 3)||(driver->Status == 5))
		Driver_Switch_TargetDisplacement(driver, 0);
	else if(driver->Status == 4)
		Driver_Switch_Sinwave(driver->Sinwave->current_frequency, 0, driver);
}

/*******************************************����/���� ���Ҳ�������*******************************************/
/*
 * ���Sinwave_data����
*/
static void Driver_Generate_Sinwave(float32_t frequency, float32_t amplitude, Driver_Struct* driver, uint8_t time_update_falg)
{
	if(time_update_falg)
	{
		uint16_t i = 0;
		if(driver->ID == 1)
		{
			for(i=1;i<=SINWAVE_DATA_LENGTH;i++)
			{
				Sinewave1.Sinwave_data1[0][i] = (i-1)*DRIVER_UPDATE_TIME;
				Sinewave1.Sinwave_data2[0][i] = (i-1)*DRIVER_UPDATE_TIME;
			}
		} else if(driver->ID == 2)
		{
			for(i=1;i<=SINWAVE_DATA_LENGTH;i++)
			{
				Sinewave2.Sinwave_data1[0][i] = (i-1)*DRIVER_UPDATE_TIME;
				Sinewave2.Sinwave_data2[0][i] = (i-1)*DRIVER_UPDATE_TIME;
			}
		}
		
	}
	
	uint16_t number;
	if(frequency==0)
		number = 1;
	else
		number = (uint16_t)(1/frequency/DRIVER_UPDATE_TIME);
	
	uint16_t i;
	float32_t rad;
	if(driver->Sinwave->current_data_table == 1)
	{
		for(i=1;i<=number;i++)
		{
			if(frequency==0)
				driver->Sinwave->Sinwave_data2[1][i] = amplitude;
			else
			{
				rad = 2*PI*frequency*driver->Sinwave->Sinwave_data2[0][i];
				driver->Sinwave->Sinwave_data2[1][i] = arm_sin_f32(rad)*amplitude;
			}
		}
		driver->Sinwave->Sinwave_data2[0][0] = number;
	}
	else if(driver->Sinwave->current_data_table == 2)
	{
		for(i=1;i<=number;i++)
		{
			if(frequency==0)
				driver->Sinwave->Sinwave_data1[1][i] = amplitude;
			else
			{
				rad = 2*PI*frequency*driver->Sinwave->Sinwave_data1[0][i];
				driver->Sinwave->Sinwave_data1[1][i] = arm_sin_f32(rad)*amplitude;
			}
		}
		driver->Sinwave->Sinwave_data1[0][0] = number;
	}
}
/*******************************************����Chirp��������*******************************************/
// ��䲨������   ���chirp���ε�time������ݣ���1s���ݵ�timeΪ0
// zeroΪ���0��־
static void Driver_Generate_Chirpwave(Driver_Struct* driver, uint32_t time, uint8_t zero)
{
	uint16_t i;
	float32_t rad;
	float t=time;
	float f_begin = driver->Sweep_wave->frequency_begin;
	float f_end = driver->Sweep_wave->frequency_end;
	float amplitude = driver->Sweep_wave->amplitude;
	float k;						// Ƶ�ʱ仯б��
	if(driver->Sweep_wave->time!=0)
		k = (f_end-f_begin)/driver->Sweep_wave->time;
	else
		k=0;
	float delta_t = DRIVER_UPDATE_TIME*SWEEP_UPDATE_TIME;					// sweep�����������
	uint16_t num_data = 1/(DRIVER_UPDATE_TIME*SWEEP_UPDATE_TIME);
		
	if(driver->Sinwave->current_data_table == 1)
	{
		for(i=1;i<num_data+1;i++)
		{
			if(zero)
				driver->Sinwave->Sinwave_data2[0][i] = 0;
			else
			{
				rad = 2*PI*(f_begin*t+0.5l*k*t*t);
				driver->Sinwave->Sinwave_data2[0][i] = arm_sin_f32(rad)*amplitude;
				t += delta_t;
			}
		}
		driver->Sinwave->Sinwave_data2[0][0] = num_data;			// ��Ч���ݸ���
		for(i=1;i<num_data+1;i++)
		{
			if(zero)
				driver->Sinwave->Sinwave_data2[1][i] = 0;
			else
			{
				rad = 2*PI*(f_begin*t+0.5l*k*t*t);
				driver->Sinwave->Sinwave_data2[1][i] = arm_sin_f32(rad)*amplitude;
				t += delta_t;
			}
		}
		driver->Sinwave->Sinwave_data2[1][0] = time+1;			// ��ŵ�ǰ�������ʼchirp����ʱ��
	}
	else if(driver->Sinwave->current_data_table == 2)
	{
		for(i=1;i<num_data+1;i++)
		{
			if(zero)
				driver->Sinwave->Sinwave_data1[0][i] = 0;
			else
			{
				rad = 2*PI*(f_begin*t+0.5l*k*t*t);
				driver->Sinwave->Sinwave_data1[0][i] = arm_sin_f32(rad)*amplitude;
				t += delta_t;
			}
		}
		driver->Sinwave->Sinwave_data1[0][0] = num_data;
		for(i=1;i<num_data+1;i++)
		{
			if(zero)
				driver->Sinwave->Sinwave_data1[1][i] = 0;
			else
			{
				rad = 2*PI*(f_begin*t+0.5l*k*t*t);
				driver->Sinwave->Sinwave_data1[1][i] = arm_sin_f32(rad)*amplitude;
				t += delta_t;
			}
		}
		driver->Sinwave->Sinwave_data1[1][0] = time+1;		// ��ŵ�ǰchirp��ʱ��
	}
}

/*******************************************�л����Ҳ������*******************************************/
void Driver_Switch_Sinwave(float32_t frequency, float32_t amplitude, Driver_Struct* driver)
{
	if(frequency>SINWAVE_MAX_FREQUENCY)
		frequency = SINWAVE_MAX_FREQUENCY;
	else if(frequency<SINWAVE_MIN_FREQUENCY)
		frequency = 0;
	
	// �޷�
	if(driver->Status == 2)		// ���ҵ�ѹ���ģʽ
	{
		if(amplitude>VOLTAGE_MAX)
			amplitude=VOLTAGE_MAX;
		if(amplitude<(-VOLTAGE_MAX))
			amplitude=-VOLTAGE_MAX;
	}
	else if(driver->Status == 4)		// ����λ�ƿ���ģʽ
	{
		if(amplitude>DISPLACE_MAX)
			amplitude=DISPLACE_MAX;
		if(amplitude<DISPLACE_MIN)
			amplitude=DISPLACE_MIN;
	}
	
	Driver_Generate_Sinwave(frequency, amplitude, driver, 0);
	
	
	if((driver->Status == 2)||(driver->Status == 4))
	{
		while(driver->Sinwave->current_sindata_index != 0);	
		
		driver->Sinwave->current_amplitude = amplitude;
		driver->Sinwave->current_frequency = frequency;
		if(driver->Sinwave->current_data_table == 1)
		{
			driver->Sinwave->current_data_table = 2;
			driver->Sinwave->sindata_number = driver->Sinwave->Sinwave_data2[0][0];
		}
		else if(driver->Sinwave->current_data_table == 2)
		{
			driver->Sinwave->current_data_table = 1;
			driver->Sinwave->sindata_number = driver->Sinwave->Sinwave_data1[0][0];
		}
		driver->Sinwave->current_sindata_index = 0;
		
		
		if(driver->Status == 2)	// ��ǰ�������ҵ�ѹ���ģʽ
		{
			driver->amplitude_force = amplitude;
			driver->frequency_force = frequency;
		}
		else if(driver->Status == 4)	// ��ǰ��������λ�����ģʽ
		{
			driver->amplitude_displacement = amplitude;
			driver->frequency_displacement = frequency;
		}
	}
}

/*******************************************�л�������ģ���ѹ���ģʽ*******************************************/
void Return_to_AN_SIN_Mode(Driver_Struct* driver)
{
	Driver_Generate_Sinwave(driver->frequency_force, driver->amplitude_force, driver, 1);
	
	driver->Sinwave->current_amplitude = driver->amplitude_force;
	driver->Sinwave->current_frequency = driver->frequency_force;
	if(driver->Sinwave->current_data_table == 1)
	{
		driver->Sinwave->current_data_table = 2;
		driver->Sinwave->sindata_number = driver->Sinwave->Sinwave_data2[0][0];
	}
	else
	{
		driver->Sinwave->current_data_table = 1;
		driver->Sinwave->sindata_number = driver->Sinwave->Sinwave_data1[0][0];
	}
	driver->Sinwave->current_sindata_index = 0;
}
/*******************************************�л�������λ�ÿ���ģʽ*******************************************/
void Return_to_DIS_SIN_Mode(Driver_Struct* driver)
{
	Driver_Generate_Sinwave(driver->frequency_displacement, driver->amplitude_displacement, driver, 1);
	
	driver->Sinwave->current_amplitude = driver->amplitude_displacement;
	driver->Sinwave->current_frequency = driver->frequency_displacement;
	if(driver->Sinwave->current_data_table == 1)
	{
		driver->Sinwave->current_data_table = 2;
		driver->Sinwave->sindata_number = driver->Sinwave->Sinwave_data2[0][0];
	}
	else
	{
		driver->Sinwave->current_data_table = 1;
		driver->Sinwave->sindata_number = driver->Sinwave->Sinwave_data1[0][0];
	}
	driver->Sinwave->current_sindata_index = 0;
}

/*******************************************�л���Chirpģʽ**********************************/
/*ɨƵ����׼��*/
void Return_to_Chirp_Mode(Driver_Struct* driver)
{
	if(driver->Sweep_wave->time==0)
		Driver_Generate_Chirpwave(driver, 0, 1);
	else
		Driver_Generate_Chirpwave(driver, 0, 0);			// ���Chirp���εĵ�0s����

	driver->Sinwave->current_amplitude = driver->Sweep_wave->amplitude;
	driver->Sinwave->current_frequency = -1;
	if(driver->Sinwave->current_data_table == 1)
	{
		driver->Sinwave->current_data_table = 2;
		driver->Sinwave->sindata_number = SINWAVE_DATA_LENGTH*2;
	}
	else
	{
		driver->Sinwave->current_data_table = 1;
		driver->Sinwave->sindata_number = SINWAVE_DATA_LENGTH*2;
	}
	driver->Sinwave->current_sindata_index = 0;
	
	if(driver->Sweep_wave->time<=2)
		if(driver->Sweep_wave->interval_time == 0)
		{
			if(driver->Sweep_wave->time==0)
				Driver_Generate_Chirpwave(driver, 0, 1);
			else
				Driver_Generate_Chirpwave(driver, 0, 0);
		}
		else
			Driver_Generate_Chirpwave(driver, 2, 1);	// ���0������ɨƵ������
	else
		Driver_Generate_Chirpwave(driver, 2, 0);		// ���Chirp���εĵ�2s����
}

/*******************************************DAC������Ҳ���*******************************************/
void Driver_Sinwave_Outout_Start(Driver_Struct* driver)
{
	driver->Status = 2;
}

/*******************************************DACֹͣ������Ҳ���*******************************************/
void Driver_Sinwave_Output_Stop(Driver_Struct* driver)
{
	Driver_Voltage_Output_0(driver);
}

/*******************************************�л���������ģʽ*******************************************/
void Driver_Encoder_Start(Driver_Struct* driver)
{
	driver->Status = 5;
	// ������ģʽ�л�������ģʽʱ������������������
	driver->Sinwave->current_sindata_index = 0;
}

/*******************************************��ȡ����������*******************************************/
void Get_Driver_Encoder_Value(Driver_Struct* driver)
{
	int16_t distance;
	if((driver->ID==1)&&(driver->Status == 3))
	{
		Driver1.Counter_Value = TIM_GetCounter(DRIVER1_ENCODER_TIMER);
		distance = Driver1.Counter_Value;
		Driver1.Distance = distance * Driver1.Resolution;
	}
	if((driver->ID==2)&&(driver->Status == 3))
	{
		Driver2.Counter_Value = TIM_GetCounter(DRIVER2_ENCODER_TIMER);
		distance = Driver2.Counter_Value;
		Driver2.Distance = distance * Driver2.Resolution;
	}
}

/*******************************************��������������*******************************************/
void Clear_Driver_Encoder_Value(Driver_Struct* driver)
{
	if((driver->ID==1)&&(driver->Status != 0))
	{
		TIM_SetCounter(DRIVER1_ENCODER_TIMER, 0);
		Driver1.Counter_Value = 0;
		Driver1.Distance = 0;
	}
	if((driver->ID==2)&&(driver->Status != 0))
	{
		TIM_SetCounter(DRIVER2_ENCODER_TIMER, 0);
		Driver2.Counter_Value = 0;
		Driver2.Distance = 0;
	}
}


/*******************************************����chairp����*******************************************/
void Update_Chairp_data(Driver_Struct* driver)
{
	uint32_t time;
	uint8_t zero_falg = 0;
	
	if(driver->Sinwave->current_data_table==1)
		time = driver->Sinwave->Sinwave_data1[1][0]+1;
	else
		time = driver->Sinwave->Sinwave_data2[1][0]+1;
	
	if(time+1 > driver->Sweep_wave->time)		// ���ݶν���
	{
		if((time-(driver->Sweep_wave->time)-((driver->Sweep_wave->time)%2))+1 > driver->Sweep_wave->interval_time)	// ���жν���
			time = 0;
		else
			zero_falg = 1;
	}
		
	Driver_Generate_Chirpwave(driver, time, zero_falg);
}
/*****************************************�޸�Sweep����***********************************************/
void Change_Sweep_Parameter(Driver_Struct* driver, float amp, float frequency_begin, float frequency_end, uint32_t time)
{
	if(frequency_begin>SWEEP_FREQUENCY_MAX)
		frequency_begin = SWEEP_FREQUENCY_MAX;
	else if(frequency_begin<0)
		frequency_begin = 0;
	
	if(frequency_end>SWEEP_FREQUENCY_MAX)
		frequency_end = SWEEP_FREQUENCY_MAX;
	else if(frequency_end<0)
		frequency_end = 0;
	
	if(time>0xefffffff)
		time = 0;
	else if(time>SWEEP_TIME_MAX)
		time = SWEEP_TIME_MAX;
	
	if(driver->Sweep_wave->mode_option==1)
	{
		if(amp > VOLTAGE_MAX)
			amp = VOLTAGE_MAX;
		else if(amp<0)
			amp = 0;
	}
	else if(driver->Sweep_wave->mode_option==2)
	{
		if(amp > DISPLACE_MAX)
			amp = DISPLACE_MAX;
		else if(amp<0)
			amp = 0;
	}
	
	if(driver->Sweep_wave->output_enable == 0)
	{
		driver->Sweep_wave->amplitude = amp;
		driver->Sweep_wave->frequency_begin = frequency_begin;
		driver->Sweep_wave->frequency_end = frequency_end;
		driver->Sweep_wave->time = time;
	}
}







