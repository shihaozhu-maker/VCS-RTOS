#include "canopen.h"

Canopen_Structure Canopen = 
{
	.Status = 0
};

uint8_t SDO_Rxbuffer[9]={0};		// SDO�������ݻ����� SDO_Rxbuffer[0]Ϊ��־λ��1������ܵ��µ�SDO�ظ�����


void Canopen_Init(void)
{
	uint8_t CAN_Status = 0;
	// GPIO��ʼ��
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
	
	// GPIO����
	GPIO_PinAFConfig(CAN_RX_GPIO, CAN_RX_GPIOPINSOURCE, GPIO_AF_CAN1);
	GPIO_PinAFConfig(CAN_TX_GPIO, CAN_TX_GPIOPINSOURCE, GPIO_AF_CAN1);
	
	// CAN��ʼ��
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
	// ������ = 42M/(1+8+3)/14=250000
	CAN_InitStructure.CAN_SJW = CAN_SJW_4tq;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_8tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;
	CAN_InitStructure.CAN_Prescaler = 14;
	
	CAN_Status = CAN_Init(CANOPEN_CAN, &CAN_InitStructure);
	if(CAN_Status)
		Canopen.Status = 1;
	
	// ����CAN���Ĺ�����
	CAN_FilterInitTypeDef CAN_FilterInitStructure;
	CAN_FilterInitStructure.CAN_FilterNumber = 0;					     							// ѡ�������0-13
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;		 			// ����ģʽ or �б�ģʽ
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;	 			// ������λ�� 16bit or 32bit
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;				   	 					// ��������ʶ��16bit
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;					 							// ��������ʶ��16bit
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;				 					// �����������16bit 0������ 1����ƥ��
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;				 						// �����������16bit 0������ 1����ƥ��
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;		// ѡ��ʹ�øù�������fifo
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;									// ʹ�ܹ�����
	
	CAN_FilterInit(&CAN_FilterInitStructure);

	
	// �жϳ�ʼ��
	NVIC_InitTypeDef NVIC_InitTypeStructure;
	NVIC_InitTypeStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
	NVIC_InitTypeStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitTypeStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitTypeStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitTypeStructure);
	
	CAN_ITConfig(CANOPEN_CAN, CAN_IT_FMP0, ENABLE);
}

// �����豸����NMT�������
void Run_NMT(void)
{
	CanTxMsg TxMessage;	
	TxMessage.StdId = 0x00;
	TxMessage.ExtId = 0x01;
	TxMessage.RTR = CAN_RTR_Data;
	TxMessage.IDE = CAN_Id_Standard;
	TxMessage.DLC = 2;										// ���ݶγ��� 0-8���ֽڣ�
	TxMessage.Data[0] = 0x01;
	TxMessage.Data[1] = 0x00;
	CAN_Transmit(CANOPEN_CAN, &TxMessage);
	
	delay_ms(5);													//ָ��ִ�С�״̬������Ҫռ��ʱ��
}

void Read_SDO(uint16_t index, uint8_t sub_index, uint8_t id)
{
	SDO_Rxbuffer[0]=0;
	
	CanTxMsg TxMessage;	
	TxMessage.StdId = 0x600+id;
	TxMessage.ExtId = 0x01;
	TxMessage.RTR = CAN_RTR_Data;
	TxMessage.IDE = CAN_Id_Standard;
	TxMessage.DLC = 8;										// ���ݶγ��� 0-8���ֽڣ�
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
	SDO_Rxbuffer[0]=0;		// ÿ�η���SDOָ��ǰ��Ҫ����SDO�ظ�������
	
	CanTxMsg TxMessage;	
	TxMessage.StdId = 0x600+id;
	TxMessage.ExtId = 0x01;
	TxMessage.RTR = CAN_RTR_Data;
	TxMessage.IDE = CAN_Id_Standard;
	TxMessage.DLC = 8;										// ���ݶγ��� 0-8���ֽڣ�
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

// �������ݵ�RPDO1
void Write_RPDO1(uint32_t data_l, uint32_t data_h, uint8_t data_byte, uint8_t id)
{
	CanTxMsg TxMessage;	
	TxMessage.StdId = 0x200+id;
	TxMessage.ExtId = 0x01;
	TxMessage.RTR = CAN_RTR_Data;
	TxMessage.IDE = CAN_Id_Standard;
	TxMessage.DLC = data_byte;										// ���ݶγ��� 0-8���ֽڣ�
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

// ��ӳ��RPDO2
void Config_RPDO2(uint8_t id)
{
	Write_SDO(0x1601, 0x00, 1, 1, id);
	Write_SDO(0x1601, 0x01, 0x60710010, 4, id);
	Write_SDO(0x1401, 0x02, 0xff, 1, id);
}

// �������ݵ�RPDO2
void Write_RPDO2(uint32_t data_l, uint32_t data_h, uint8_t data_byte, uint8_t id)
{
	CanTxMsg TxMessage;	
	TxMessage.StdId = 0x300+id;
	TxMessage.ExtId = 0x01;
	TxMessage.RTR = CAN_RTR_Data;
	TxMessage.IDE = CAN_Id_Standard;
	TxMessage.DLC = data_byte;										// ���ݶγ��� 0-8���ֽڣ�
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
	/* ʹ��ǰ״̬Ϊ 05 81 4B 41 60 00 40 02 00 00 */
	/* ʹ�ܺ�״̬Ϊ 05 81 4B 41 60 00 37 06 00 00 */
	int i=0;
	while(i<255)
	{
		i++;
		Read_SDO(STATUS_WORD, 0x00, id);
		if(SDO_Rxbuffer[0]==1)
		{
			if(SDO_Rxbuffer[2]==0x41&&SDO_Rxbuffer[3]==0x60)
			{
				if((SDO_Rxbuffer[5]&0x6f)==0x27)		// ��ʹ��
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
		delay_ms(30);		//��һ�����ú�ʱ�ϳ�����ʱ�����ڼ��������ظ���������Լʱ��
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
				if(!((SDO_Rxbuffer[5]&0x6f)==0x27))		// �ѽ���
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
	Disable_Driver(id);																	// ����
	Read_SDO(MODE_OF_OPERATION_SHOW, 0x00, id);			
	delay_ms(5);
	Write_SDO(MODE_OF_OPERATION, 0x00, 4, 1, id);					// ת������ģʽ
	delay_ms(5);
	Read_SDO(MODE_OF_OPERATION_SHOW, 0x00, id);
	delay_ms(5);
	
	Target_Torque_Set(0, id);															// ����Ŀ��ת��
	Write_SDO(TORQUE_SLOPE, 0x00, 100000, 4, id);					// ת��б��
}

void Target_Torque_Set(float torque, uint8_t id)
{
	if(torque>RATED_TORQUE)
		torque=RATED_TORQUE;
	else if(torque<(-RATED_TORQUE))
		torque=(-RATED_TORQUE);
	
	int16_t torque_set;
	torque_set = (int16_t)(torque/RATED_TORQUE*1000);
	
	// SDO������������
//	Write_SDO(TARGET_TORQUE, 0x00, torque_set, 2, id);
	
	// PDO������������
	Write_RPDO2((uint32_t)torque_set, 0, 2, id);
}

/*
	mode = 	3 AI����ģʽ
	mode = 	0 CAN����ģʽ
	id				CAN id
*/
void AI_Mode_Config(uint8_t mode, uint16_t id)
{
	/* ����ģʽ��ȡ���
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
				if(SDO_Rxbuffer[5]==mode)		// ���������
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



