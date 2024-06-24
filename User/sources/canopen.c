#include "canopen.h"

Canopen_Structure Canopen = 
{
	.Status = 0
};

uint8_t SDO_Rxbuffer[9]={0};		// SDO接收数据缓冲区 SDO_Rxbuffer[0]为标志位，1代表接受到新的SDO回复数据


void Canopen_Init(void)
{
	uint8_t CAN_Status = 0;
	// GPIO初始化
	RCC_AHB1PeriphClockCmd(CAN_RX_GPIORCC||CAN_TX_GPIORCC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = CAN_RX_GPIOPIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_Init(CAN_RX_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = CAN_TX_GPIOPIN;
	GPIO_Init(CAN_RX_GPIO, &GPIO_InitStructure);
	
	// GPIO复用
	GPIO_PinAFConfig(CAN_RX_GPIO, CAN_RX_GPIOPINSOURCE, GPIO_AF_CAN1);
	GPIO_PinAFConfig(CAN_TX_GPIO, CAN_TX_GPIOPINSOURCE, GPIO_AF_CAN1);
	
	// CAN初始化
	RCC_APB1PeriphClockCmd(CANOPEN_CAN_RCC, ENABLE);
	
	CAN_InitTypeDef CAN_InitStructure;
	CAN_InitStructure.CAN_ABOM = DISABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	// APB1 42MHz
	// 波特率 = 42M/(1+8+3)/14=250000
	CAN_InitStructure.CAN_SJW = CAN_SJW_4tq;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_8tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;
	CAN_InitStructure.CAN_Prescaler = 14;
	
	CAN_Status = CAN_Init(CANOPEN_CAN, &CAN_InitStructure);
	if(CAN_Status)
		Canopen.Status = 1;
	
	// 配置CAN报文过滤器
	CAN_FilterInitTypeDef CAN_FilterInitStructure;
	CAN_FilterInitStructure.CAN_FilterNumber = 0;					     							// 选择过滤器0-13
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;		 			// 掩码模式 or 列表模式
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;	 			// 过滤器位数 16bit or 32bit
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;				   	 					// 过滤器标识高16bit
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;					 							// 过滤器标识低16bit
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;				 					// 过滤器掩码高16bit 0不关心 1必须匹配
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;				 						// 过滤器掩码低16bit 0不关心 1必须匹配
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;		// 选择使用该过滤器的fifo
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;									// 使能过滤器
	
	CAN_FilterInit(&CAN_FilterInitStructure);

	
	// 中断初始化
	NVIC_InitTypeDef NVIC_InitTypeStructure;
	NVIC_InitTypeStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
	NVIC_InitTypeStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitTypeStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitTypeStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitTypeStructure);
	
	CAN_ITConfig(CANOPEN_CAN, CAN_IT_FMP0, ENABLE);
}

// 总线设备启动NMT网络服务
void Run_NMT(void)
{
	CanTxMsg TxMessage;	
	TxMessage.StdId = 0x00;
	TxMessage.ExtId = 0x01;
	TxMessage.RTR = CAN_RTR_Data;
	TxMessage.IDE = CAN_Id_Standard;
	TxMessage.DLC = 2;										// 数据段长度 0-8（字节）
	TxMessage.Data[0] = 0x01;
	TxMessage.Data[1] = 0x00;
	CAN_Transmit(CANOPEN_CAN, &TxMessage);
	
	delay_ms(5);													//指令执行、状态更新需要占用时间
}

void Read_SDO(uint16_t index, uint8_t sub_index, uint8_t id)
{
	SDO_Rxbuffer[0]=0;
	
	CanTxMsg TxMessage;	
	TxMessage.StdId = 0x600+id;
	TxMessage.ExtId = 0x01;
	TxMessage.RTR = CAN_RTR_Data;
	TxMessage.IDE = CAN_Id_Standard;
	TxMessage.DLC = 8;										// 数据段长度 0-8（字节）
	TxMessage.Data[0] = 0x40;
	TxMessage.Data[1] = index&0xff;
	TxMessage.Data[2] = (index>>8)&0xff;
	TxMessage.Data[3] = sub_index;
	TxMessage.Data[4] = 0x00;
	TxMessage.Data[5] = 0x00;
	TxMessage.Data[6] = 0x00;
	TxMessage.Data[7] = 0x00;
	CAN_Transmit(CANOPEN_CAN, &TxMessage);
	delay_ms(5);
}

void Write_SDO(uint16_t index, uint8_t sub_index, uint32_t data, uint8_t data_byte, uint8_t id)
{
	SDO_Rxbuffer[0]=0;		// 每次发送SDO指令前都要清理SDO回复缓冲区
	
	CanTxMsg TxMessage;	
	TxMessage.StdId = 0x600+id;
	TxMessage.ExtId = 0x01;
	TxMessage.RTR = CAN_RTR_Data;
	TxMessage.IDE = CAN_Id_Standard;
	TxMessage.DLC = 8;										// 数据段长度 0-8（字节）
	switch(data_byte)
	{
		case 1:
		{
			TxMessage.Data[0] = 0x2f;
			break;
		}
		case 2:
		{
			TxMessage.Data[0] = 0x2b;
			break;
		}
		case 3:
		{
			TxMessage.Data[0] = 0x27;
			break;
		}
		case 4:
		{
			TxMessage.Data[0] = 0x23;
			break;
		}
		default:
		{
			TxMessage.Data[0] = 0x23;
			break;
		}
	}
	TxMessage.Data[1] = index&0xff;
	TxMessage.Data[2] = (index>>8)&0xff;
	TxMessage.Data[3] = sub_index;
	TxMessage.Data[4] = data&0xff;
	TxMessage.Data[5] = (data>>8)&0xff;
	TxMessage.Data[6] = (data>>16)&0xff;
	TxMessage.Data[7] = (data>>24)&0xff;
	CAN_Transmit(CANOPEN_CAN, &TxMessage);
	delay_ms(5);
}

// 发送数据到RPDO1
void Write_RPDO1(uint32_t data_l, uint32_t data_h, uint8_t data_byte, uint8_t id)
{
	CanTxMsg TxMessage;	
	TxMessage.StdId = 0x200+id;
	TxMessage.ExtId = 0x01;
	TxMessage.RTR = CAN_RTR_Data;
	TxMessage.IDE = CAN_Id_Standard;
	TxMessage.DLC = data_byte;										// 数据段长度 0-8（字节）
	TxMessage.Data[0] = data_l&0xff;
	TxMessage.Data[1] = (data_l>>8)&0xff;
	TxMessage.Data[2] = (data_l>>16)&0xff;
	TxMessage.Data[3] = (data_l>>24)&0xff;
	TxMessage.Data[4] = data_h&0xff;
	TxMessage.Data[5] = (data_h>>8)&0xff;
	TxMessage.Data[6] = (data_h>>16)&0xff;
	TxMessage.Data[7] = (data_h>>24)&0xff;
	
	CAN_Transmit(CANOPEN_CAN, &TxMessage);
}

// 重映射RPDO2
void Config_RPDO2(uint8_t id)
{
	Write_SDO(0x1601, 0x00, 1, 1, id);
	Write_SDO(0x1601, 0x01, 0x60710010, 4, id);
	Write_SDO(0x1401, 0x02, 0xff, 1, id);
}

// 发送数据到RPDO2
void Write_RPDO2(uint32_t data_l, uint32_t data_h, uint8_t data_byte, uint8_t id)
{
	CanTxMsg TxMessage;	
	TxMessage.StdId = 0x300+id;
	TxMessage.ExtId = 0x01;
	TxMessage.RTR = CAN_RTR_Data;
	TxMessage.IDE = CAN_Id_Standard;
	TxMessage.DLC = data_byte;										// 数据段长度 0-8（字节）
	TxMessage.Data[0] = data_l&0xff;
	TxMessage.Data[1] = (data_l>>8)&0xff;
	TxMessage.Data[2] = (data_l>>16)&0xff;
	TxMessage.Data[3] = (data_l>>24)&0xff;
	TxMessage.Data[4] = data_h&0xff;
	TxMessage.Data[5] = (data_h>>8)&0xff;
	TxMessage.Data[6] = (data_h>>16)&0xff;
	TxMessage.Data[7] = (data_h>>24)&0xff;
	
	CAN_Transmit(CANOPEN_CAN, &TxMessage);
}

void Enable_Driver(uint8_t id)
{
	/* 使能前状态为 05 81 4B 41 60 00 40 02 00 00 */
	/* 使能后状态为 05 81 4B 41 60 00 37 06 00 00 */
	int i=0;
	while(i<255)
	{
		i++;
		Read_SDO(STATUS_WORD, 0x00, id);
		if(SDO_Rxbuffer[0]==1)
		{
			if(SDO_Rxbuffer[2]==0x41&&SDO_Rxbuffer[3]==0x60)
			{
				if((SDO_Rxbuffer[5]&0x6f)==0x27)		// 已使能
					break;
			}
			else
				continue;
		}
		else
			continue;
		
		Write_RPDO1(0x06,0x00,2,id);
		delay_ms(5);
		Write_RPDO1(0x07,0x00,2,id);
		delay_ms(5);
		Write_RPDO1(0x0f,0x00,2,id);
		delay_ms(30);		//这一步配置耗时较长，延时有助于减少配置重复次数，节约时间
	}
}

void Disable_Driver(uint8_t id)
{
	int i=0;
	while(i<255)
	{
		i++;
		Read_SDO(STATUS_WORD, 0x00, id);
		if(SDO_Rxbuffer[0]==1)
		{
			if(SDO_Rxbuffer[2]==0x41&&SDO_Rxbuffer[3]==0x60)
			{
				if(!((SDO_Rxbuffer[5]&0x6f)==0x27))		// 已禁能
					break;
			}
			else
				continue;
		}
		else
			continue;
		Write_RPDO1(0x00,0x00,2,id);
		delay_ms(5);
	}
}

void Profile_Torque_Mode_Config(uint8_t id)
{
	Disable_Driver(id);																	// 禁能
	Read_SDO(MODE_OF_OPERATION_SHOW, 0x00, id);			
	delay_ms(5);
	Write_SDO(MODE_OF_OPERATION, 0x00, 4, 1, id);					// 转矩轮廓模式
	delay_ms(5);
	Read_SDO(MODE_OF_OPERATION_SHOW, 0x00, id);
	delay_ms(5);
	
	Target_Torque_Set(0, id);															// 配置目标转矩
	Write_SDO(TORQUE_SLOPE, 0x00, 100000, 4, id);					// 转矩斜率
}

void Target_Torque_Set(float torque, uint8_t id)
{
	if(torque>RATED_TORQUE)
		torque=RATED_TORQUE;
	else if(torque<(-RATED_TORQUE))
		torque=(-RATED_TORQUE);
	
	int16_t torque_set;
	torque_set = (int16_t)(torque/RATED_TORQUE*1000);
	
	// SDO发送力矩数据
//	Write_SDO(TARGET_TORQUE, 0x00, torque_set, 2, id);
	
	// PDO发送力矩数据
	Write_RPDO2((uint32_t)torque_set, 0, 2, id);
}

/*
	mode = 	3 AI控制模式
	mode = 	0 CAN控制模式
	id				CAN id
*/
void AI_Mode_Config(uint8_t mode, uint16_t id)
{
	/* 控制模式读取结果
			AI 	05 81 4B 1B 20 00 03 00 00 00
			CAN	05 81 60 1B 20 00 00 00 00 00
	*/
	int i=0;
	while(i<255)
	{
		i++;
		Read_SDO(AI_CONTROL_MODE, 0x00, id);
		if(SDO_Rxbuffer[0]==1)
		{
			if(SDO_Rxbuffer[2]==0x1b&&SDO_Rxbuffer[3]==0x20)
			{
				if(SDO_Rxbuffer[5]==mode)		// 已完成配置
					break;
			}
			else
				continue;
		}
		else
			continue;
		
		Write_SDO(AI_CONTROL_MODE, 0x00, mode, 2, id);
		delay_ms(5);
	}
}



