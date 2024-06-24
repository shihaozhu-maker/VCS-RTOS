#include "spi.h"


void spi_Init(void)
{
	SPI_InitTypeDef SPI_InitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;

    // 1. Enable the clock for SPI2 and its corresponding GPIO ports
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  // Assuming SPI2 is on GPIOB

    // 2. Configure the GPIO pins for SPI2
    GPIO_InitStruct.GPIO_Pin = SPI_NSS_GPIO_PIN | SPI_SCK_GPIO_PIN | SPI_MISO_GPIO_PIN | SPI_MOSI_GPIO_PIN; // SCK, MISO, MOSI
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;			// зЂвт
    GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);

    // Connect SPI2 pins to SPI alternate function
	GPIO_PinAFConfig(GPIOB, SPI_NSS_GPIO_PINSOURCE, GPIO_AF_SPI2); // NSS
    GPIO_PinAFConfig(GPIOB, SPI_SCK_GPIO_PINSOURCE, GPIO_AF_SPI2); // SCK
    GPIO_PinAFConfig(GPIOB, SPI_MISO_GPIO_PINSOURCE, GPIO_AF_SPI2); // MISO
    GPIO_PinAFConfig(GPIOB, SPI_MOSI_GPIO_PINSOURCE, GPIO_AF_SPI2); // MOSI

    // 3. SPI2 configuration
    SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStruct.SPI_Mode = SPI_Mode_Slave;
    SPI_InitStruct.SPI_DataSize = SPI_DataSize_16b;
    SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; // Adjust as needed   42/4=10.5M
    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStruct.SPI_CRCPolynomial = 7;  // Default value
    SPI_Init(SPIX, &SPI_InitStruct);
	
	//SPI_SSOutputCmd(SPIX, DISABLE);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

    // 4. Enable SPI2
	SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_RXNE, ENABLE);
    SPI_Cmd(SPIX, ENABLE);
}


