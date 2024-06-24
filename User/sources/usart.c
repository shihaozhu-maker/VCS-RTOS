#include "usart.h"

void Usart_Init(void)
{
	// ��ʼ��GPIO
	RCC_AHB1PeriphClockCmd(USART_TX_GPIORCC|USART_RX_GPIORCC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = USART_TX_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(USART_TX_GPIO_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = USART_RX_GPIO_PIN;
	GPIO_Init(USART_RX_GPIO_PORT, &GPIO_InitStruct);
	
	GPIO_PinAFConfig(USART_TX_GPIO_PORT, USART_TX_GPIO_PINSOURCE, GPIO_AF_USART2);
	GPIO_PinAFConfig(USART_RX_GPIO_PORT, USART_RX_GPIO_PINSOURCE, GPIO_AF_USART2);
	
	// ��ʼ������
	RCC_APB1PeriphClockCmd(USART_RCC, ENABLE);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = USART_BAUDRATE;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART_, &USART_InitStructure);
	
	USART_Cmd(USART_, ENABLE);
	
	// ���ô��ڽ����ж�  USART_IT_RXNE ��ȡ���ݼĴ����ǿ�
	NVIC_InitTypeDef NVIC_InitTypeStructure;
	NVIC_InitTypeStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitTypeStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitTypeStructure.NVIC_IRQChannelPreemptionPriority = 10;
	NVIC_InitTypeStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitTypeStructure);
	
	USART_ITConfig(USART_, USART_IT_RXNE, ENABLE);
}

void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	USART_SendData(pUSARTx,ch);
		
	/* �ȴ��������ݼĴ���Ϊ�� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* �ȴ�������� */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC) == RESET);
}

///�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ����� */
		USART_SendData(USART_, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(USART_, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ�����ʹ��getchar����ʱ����ʹ�ܴ��ڽ����ж�
int fgetc(FILE *f)
{
		/* �ȴ������������� */
		while (USART_GetFlagStatus(USART_, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART_);
}

