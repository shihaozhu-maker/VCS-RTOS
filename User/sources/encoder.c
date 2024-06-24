#include "encoder.h"
static void Encoder1_Init(void);
static void Encoder2_Init(void);
static void Encoder_ADC_Init(void);
static void Encoder_Timer_Init(void);

Encoder_Struct Encoder1 = {
	.ID = 1,
	.Status = 0,
	.Magnetic_Field = 0,
	.Counter_Value = 0,
	.Distance = 0,
	.Resolution = Resolution_AS5304
};

Encoder_Struct Encoder2 = {
	.ID = 2,
	.Status = 0,
	.Magnetic_Field = 0,
	.Counter_Value = 0,
	.Distance = 0,
	.Resolution = Resolution_AS5304
}; 


uint16_t Encoder_ADC_Data[2] = {0,0};

void Encoder_Init(void)
{
	Encoder1_Init();
	Encoder2_Init();
	Encoder_ADC_Init();
	
	// Encoder_Timer_Init();
}


static void Encoder1_Init(void)
{
	//  初始化GPIO
	RCC_AHB1PeriphClockCmd(ENCODER1_A_GPIORCC|ENCODER1_B_GPIORCC|ENCODER1_AO_GPIORCC|ENCODER1_INDEX_GPIORCC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = ENCODER1_A_GPIOPIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_Init(ENCODER1_A_GPIOPORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(ENCODER1_A_GPIOPORT, ENCODER1_A_PINSOURCE, GPIO_AF_TIM5);
	
	GPIO_InitStructure.GPIO_Pin = ENCODER1_B_GPIOPIN;
	GPIO_Init(ENCODER1_B_GPIOPORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(ENCODER1_B_GPIOPORT, ENCODER1_B_PINSOURCE, GPIO_AF_TIM5);
	
	GPIO_InitStructure.GPIO_Pin = ENCODER1_AO_GPIOPIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_Init(ENCODER1_AO_GPIOPORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = ENCODER1_INDEX_GPIOPIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(ENCODER1_INDEX_GPIOPORT, &GPIO_InitStructure);
	
	// 初始化TIMER 168/4*2 = 84M
	RCC_APB1PeriphClockCmd(ENCODER1_TIMER_RCC, ENABLE);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);
	TIM_TimeBaseInitStructure.TIM_Period = COUNTER_PERIOD;
	TIM_TimeBaseInit(ENCODER1_TIMER, &TIM_TimeBaseInitStructure);
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0x5;		// 84/2/8=5.25
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(ENCODER1_TIMER, &TIM_ICInitStructure);
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInit(ENCODER1_TIMER, &TIM_ICInitStructure);
	
	TIM_EncoderInterfaceConfig(ENCODER1_TIMER, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
	TIM_Cmd(ENCODER1_TIMER, ENABLE);
	
	Encoder1.Status = 1;
}

static void Encoder2_Init(void)
{
	//  初始化GPIO
	RCC_AHB1PeriphClockCmd(ENCODER2_A_GPIORCC|ENCODER2_B_GPIORCC|ENCODER2_AO_GPIORCC|ENCODER2_INDEX_GPIORCC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = ENCODER2_A_GPIOPIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_Init(ENCODER2_A_GPIOPORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(ENCODER2_A_GPIOPORT, ENCODER2_A_PINSOURCE, GPIO_AF_TIM3);
	
	GPIO_InitStructure.GPIO_Pin = ENCODER2_B_GPIOPIN;
	GPIO_Init(ENCODER2_B_GPIOPORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(ENCODER2_B_GPIOPORT, ENCODER2_B_PINSOURCE, GPIO_AF_TIM3);
	
	GPIO_InitStructure.GPIO_Pin = ENCODER2_AO_GPIOPIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_Init(ENCODER2_AO_GPIOPORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = ENCODER2_INDEX_GPIOPIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(ENCODER2_INDEX_GPIOPORT, &GPIO_InitStructure);
	
	// 初始化TIMER 168/4*2 = 84M
	RCC_APB1PeriphClockCmd(ENCODER2_TIMER_RCC, ENABLE);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);
	TIM_TimeBaseInitStructure.TIM_Period = COUNTER_PERIOD;
	TIM_TimeBaseInit(ENCODER2_TIMER, &TIM_TimeBaseInitStructure);
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0x5;		// 84/2/8=5.25
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(ENCODER2_TIMER, &TIM_ICInitStructure);
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInit(ENCODER2_TIMER, &TIM_ICInitStructure);
	
	TIM_EncoderInterfaceConfig(ENCODER2_TIMER, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
	TIM_Cmd(ENCODER2_TIMER, ENABLE);
	
	Encoder2.Status = 1;
}

static void Encoder_ADC_Init(void)
{
	// 初始化ADC
	RCC_APB2PeriphClockCmd(ENCODER_ADC_RCC, ENABLE);
	
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_NbrOfConversion = Encoder1.Status + Encoder2.Status;
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_8b;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_Init(ENCODER_ADC, &ADC_InitStructure);
	
	if(Encoder1.Status)
	{
		ADC_RegularChannelConfig(ENCODER_ADC, ENCODER1_ADC_CHANNEL, 1, ADC_SampleTime_3Cycles);
		if(Encoder2.Status)
			ADC_RegularChannelConfig(ENCODER_ADC, ENCODER2_ADC_CHANNEL, 2, ADC_SampleTime_3Cycles);
	}
	else if(Encoder2.Status)
		ADC_RegularChannelConfig(ENCODER_ADC, ENCODER2_ADC_CHANNEL, 1, ADC_SampleTime_3Cycles);

	
	// 初始化DMA
	RCC_AHB1PeriphClockCmd(ENCODER_DMA_RCC, ENABLE);
	
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_BufferSize = Encoder1.Status+Encoder2.Status;
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	// 不使用FIFO阈值控制模式，数据传输到FIFO后直接保存到目标地址
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	// FIFO阈值
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	
	if(Encoder1.Status)
		DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)Encoder_ADC_Data;
	else
		DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&(Encoder_ADC_Data[1]);
	// 使用单次传输，不使用突发传输
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ENCODER_ADC->DR);
	// 使用单次传输，不使用突发传输
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	
	DMA_Init(ENCODER_DMA, &DMA_InitStructure);
	DMA_Cmd(ENCODER_DMA, ENABLE);
	
	ADC_DMACmd(ENCODER_ADC, ENABLE);
	ADC_Cmd(ENCODER_ADC, ENABLE);
}

static void Encoder_Timer_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	
	// 时基TimeBase 168/4*2/84=1M
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);
	TIM_TimeBaseInitStructure.TIM_Prescaler = 84;
	TIM_TimeBaseInitStructure.TIM_Period = ENCODER_TIMER_ITTIME*1000;
	TIM_TimeBaseInit(ENCODER_TIMER, &TIM_TimeBaseInitStructure);
	
	NVIC_InitTypeDef NVIC_InitTypeStructure;
	NVIC_InitTypeStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;
	NVIC_InitTypeStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitTypeStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitTypeStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitTypeStructure);
	TIM_ITConfig(ENCODER_TIMER, TIM_IT_Update, ENABLE);
	
	TIM_Cmd(ENCODER_TIMER, ENABLE);
}

/*更细磁场强度数据*/
void Encoder_Magnetic_Field(void)
{
	uint8_t A,B,Index;
	
//	ADC_SoftwareStartConv(ENCODER_ADC);
//	Encoder1.Magnetic_Field = Encoder_ADC_Data[0];
//	Encoder2.Magnetic_Field = Encoder_ADC_Data[1];
	
	
	if(Encoder1.Status)
	{
		Encoder1.Magnetic_Field = 100;
		A = GPIO_ReadInputDataBit(ENCODER1_A_GPIOPORT, ENCODER1_A_GPIOPIN);
		B = GPIO_ReadInputDataBit(ENCODER1_B_GPIOPORT, ENCODER1_B_GPIOPIN);
		Index = GPIO_ReadInputDataBit(ENCODER1_INDEX_GPIOPORT, ENCODER1_INDEX_GPIOPIN);
		if(Index&&(!A)&&(!B))
			Encoder1.Magnetic_Field = 0;		// 磁场强度过低
	}
	if(Encoder2.Status)
	{
		Encoder2.Magnetic_Field = 100;
		A = GPIO_ReadInputDataBit(ENCODER2_A_GPIOPORT, ENCODER2_A_GPIOPIN);
		B = GPIO_ReadInputDataBit(ENCODER2_B_GPIOPORT, ENCODER2_B_GPIOPIN);
		Index = GPIO_ReadInputDataBit(ENCODER2_INDEX_GPIOPORT, ENCODER2_INDEX_GPIOPIN);
		if(Index&&(!A)&&(!B))
			Encoder2.Magnetic_Field = 0;		// 磁场强度过低
	}
}

void Get_Encoder_Value(void)
{
	int16_t distance;
	if(Encoder1.Status)
	{
		Encoder1.Counter_Value = TIM_GetCounter(ENCODER1_TIMER);
		distance = Encoder1.Counter_Value;
		Encoder1.Distance = distance*Encoder1.Resolution;
	}
	if(Encoder2.Status)
	{
		Encoder2.Counter_Value = TIM_GetCounter(ENCODER2_TIMER);
		distance = Encoder2.Counter_Value;
		Encoder2.Distance = distance*Encoder2.Resolution;
	}
}

void Clear_Encoder_Value(Encoder_Struct* encoder)
{
	if((encoder->ID==1)&&(encoder->Status==1))
	{
		TIM_SetCounter(ENCODER1_TIMER, 0);
		encoder->Counter_Value = 0;
		encoder->Distance = 0;
	}
	if((encoder->ID==2)&&(encoder->Status==1))
	{
		TIM_SetCounter(ENCODER2_TIMER, 0);
		encoder->Counter_Value = 0;
		encoder->Distance = 0;
	}
}


