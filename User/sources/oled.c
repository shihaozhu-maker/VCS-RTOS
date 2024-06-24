#include "oled.h"

OLED_Struct OLED =
{
	.Status = 0
};

void OLED_WriteCmd(uint8_t OLED_Command)
{
	I2C_Start();
	I2C_Send_Byte(OLED_WRITE);
	I2C_Wait_Ack();	 
	I2C_Send_Byte(0x00);
	I2C_Wait_Ack();	 
	I2C_Send_Byte(OLED_Command);
	I2C_Wait_Ack();		
	I2C_Stop();
}

void OLED_WriteData(uint8_t OLED_Data)
{
	I2C_Start();
	I2C_Send_Byte(OLED_WRITE);
	I2C_Wait_Ack();	 
	I2C_Send_Byte(0x40);
	I2C_Wait_Ack();	 
	I2C_Send_Byte(OLED_Data);
	I2C_Wait_Ack();		
	I2C_Stop();
}

uint8_t OLED_CheckDevice(void)
{
	uint8_t ack = 0;
	uint32_t n_trials = 0;
	
	do
	{
		I2C_Start();
		I2C_Send_Byte(OLED_WRITE);//发送芯片地址 方向为写
		ack = I2C_Wait_Ack();	
			
		if(n_trials > OLED_MAX_TRIALS_NUMBER)		/* 超过设备最大检查次数 */
			return 1;
		n_trials++;
	}while(ack);
	
	I2C_Stop();
	return 0;		/* 应答返回 */
}

void OLED_Init(void)
{
	if(OLED_CheckDevice())
	{
		OLED.Status = 0;
		return;
	}
	
	OLED_WriteCmd(0xAE); //display off
	
	OLED_WriteCmd(0x20); //Set Memory Addressing Mode	
	OLED_WriteCmd(0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	
	OLED_WriteCmd(0xb0); //Set Page Start Address for Page Addressing Mode,0-7
	
	OLED_WriteCmd(0xc8); //Set COM Output Scan Direction
	
	OLED_WriteCmd(0x00); //设置起始列坐标的低四位
	OLED_WriteCmd(0x10); //设置起始列坐标的高四位
	
	OLED_WriteCmd(0x40); //--set start line address
	
	OLED_WriteCmd(0x81); //设置对比度
	OLED_WriteCmd(0x5f); //0x00~0xff
	
	OLED_WriteCmd(0xa1); //--set segment re-map 0 to 127
	
	OLED_WriteCmd(0xa6); //--set normal display a6/a7 显示翻转
	
	OLED_WriteCmd(0xa8); //--set multiplex ratio(1 to 63)
	OLED_WriteCmd(0x3f); //
	
	OLED_WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	
	OLED_WriteCmd(0xd3); //-set display offset
	OLED_WriteCmd(0x00); //-not offset
	
	OLED_WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
	OLED_WriteCmd(0xf0); //--set divide ratio
	
	OLED_WriteCmd(0xd9); //--set pre-charge period
	OLED_WriteCmd(0x22); //
	
	OLED_WriteCmd(0xda); //--set com pins hardware configuration
	OLED_WriteCmd(0x12);
	
	OLED_WriteCmd(0xdb); //--set vcomh
	OLED_WriteCmd(0x20); //0x20,0.77xVcc
	
	OLED_WriteCmd(0x8d); //--set DC-DC enable
	OLED_WriteCmd(0x14); //
	
	OLED_WriteCmd(0xaf); //--turn on oled panel
	
	OLED.Status = 1;
}

/* 显示显存 */
void OLED_Display_Video_Memory(char Video_Memory[8][128], uint8_t x, uint8_t y)
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		OLED_WriteCmd(0xb0+m);		//page0-page1
		OLED_WriteCmd(0x00);		//low column start address
		OLED_WriteCmd(0x10);		//high column start address

		for(n=0; n<128; n++)
		{
			OLED_WriteData(Video_Memory[(m+y)%8][((n+x)%128)]);
		}
	}
}

/* 显示字模 x为起始横纵像素点 y为起始纵坐标页数 N表示字模中第N个字*/
void OLED_Show_Matrix(unsigned char x, unsigned char y, Font* const caodetab, unsigned char N)
{
	uint8_t i=0;
	uint8_t j=0;
	uint8_t row = caodetab->Font_Hight/8;
	uint8_t column = caodetab->Font_Width;

	uint32_t codebit_size = caodetab->Font_Hight*caodetab->Font_Width/8;
	uint32_t adder = codebit_size*N;
	
	
	for(i=0; i<row; i++)
	{
		OLED_SetPos(x,y+i);
		for(j=0; j<column; j++)
		{
			OLED_WriteData(caodetab->Font_data[adder]);
			adder++;
		}
	}
}

void OLED_Show_Matrix_Pointer(unsigned char x, unsigned char y, Font_Pointer pointer, unsigned char N)
{
	uint8_t i=0;
	uint8_t j=0;
	uint8_t row = pointer.Font_Hight/8;
	uint8_t column = pointer.Font_Width;

	uint32_t codebit_size = pointer.Font_Hight*pointer.Font_Width/8;
	uint32_t adder = codebit_size*N;
	
	
	for(i=0; i<row; i++)
	{
		OLED_SetPos(x,y+i);
		for(j=0; j<column; j++)
		{
			OLED_WriteData(pointer.Font_data[adder]);
			adder++;
		}
	}
}

/* 显示数字字模*/
void OLED_Show_Number(unsigned char x, unsigned char y, uint8_t number)
{
	extern Font_Pointer Number;
	
	uint8_t i=0;
	uint8_t j=0;
	uint8_t row = Number.Font_Hight/8;
	uint8_t column = Number.Font_Width;

	uint32_t codebit_size = Number.Font_Hight*Number.Font_Width/8;
	uint32_t adder = codebit_size*number;
	
	for(i=0; i<row; i++)
	{
		OLED_SetPos(x,y+i);
		for(j=0; j<column; j++)
		{
			OLED_WriteData(Number.Font_data[adder]);
			adder++;
		}
	}	
}

/* 设置显示起始坐标 */
void OLED_SetPos(uint8_t x , uint8_t y)
{
	OLED_WriteCmd(0xb0+y);								//page0-page1
	OLED_WriteCmd(x&0x0f);								//low column start address
	OLED_WriteCmd(((x&0xf0)>>4)|0x10);		//high column start address
}


extern Driver_Struct Driver1;
extern Driver_Struct Driver2;
extern Encoder_Struct Encoder1;
extern Encoder_Struct Encoder2;
extern Delay_Feedback_Struct Delay_Feedback_Controller1;
extern Delay_Feedback_Struct Delay_Feedback_Controller2;
extern Canopen_Structure Canopen;

extern char Device_List[8][128];
extern char Driver1_AN[8][128];
extern char Driver1_SIN[8][128];
extern char Driver1_Displacement[8][128];
extern char Driver1_SIN_Displacement[8][128];
extern char Driver1_EN[8][128];
extern char Driver1_BUSY[8][128];
extern char Driver2_AN[8][128];
extern char Driver2_SIN[8][128];
extern char Driver2_Displacement[8][128];
extern char Driver2_SIN_Displacement[8][128];
extern char Driver2_EN[8][128];
extern char Driver2_BUSY[8][128];
extern char Encoder1_vm[8][128];
extern char Encoder2_vm[8][128];
extern char Uart_Tx_vm[8][128];
extern char DRC1_Off_vm[8][128];
extern char DRC1_On_vm[8][128];
extern char DRC1_On_Config_vm[8][128];
extern char DRC1_Off_Config_vm[8][128];
extern char DRC2_Off_vm[8][128];
extern char DRC2_On_vm[8][128];
extern char DRC2_On_Config_vm[8][128];
extern char DRC2_Off_Config_vm[8][128];
extern char Can_Enable_vm[8][128];
extern char Can_Out_vm[8][128];
extern char Sweep1_On_vm[8][128];
extern char Sweep1_Off_vm[8][128];
extern char Sweep1_On_Config_vm[8][128];
extern char Sweep1_Off_Config_vm[8][128];
extern char Sweep2_On_vm[8][128];
extern char Sweep2_Off_vm[8][128];
extern char Sweep2_On_Config_vm[8][128];
extern char Sweep2_Off_Config_vm[8][128];



extern Font Square_Brackets;
extern Font Select_Mark;
extern Font Not_Select_Mark;
extern Font_Pointer RSSI;
extern Font Underline;

extern uint8_t Top_Device;
extern Options_Struct Option;

void OLED_Update(void)
{
	//Options_Struct* option = &Option;
	
	switch(Top_Device)
	{
		// device list
		case 0:
		{
			OLED_Display_Video_Memory(Device_List, 0, 0);
			if(Driver1.Status!=0)
				OLED_Show_Matrix(5*8, 2, &Select_Mark, 0);
			else
				OLED_Show_Matrix(5*8, 2, &Not_Select_Mark, 0);
			
			if(Driver2.Status!=0)
				OLED_Show_Matrix(13*8, 2, &Select_Mark, 0);
			else
				OLED_Show_Matrix(13*8, 2, &Not_Select_Mark, 0);
			if(Encoder1.Status!=0)
				OLED_Show_Matrix(5*8, 4, &Select_Mark, 0);
			else
				OLED_Show_Matrix(5*8, 4, &Not_Select_Mark, 0);
			if(Encoder2.Status!=0)
				OLED_Show_Matrix(13*8, 4, &Select_Mark, 0);
			else
				OLED_Show_Matrix(13*8, 4, &Not_Select_Mark, 0);
				
			OLED_Show_Matrix(5*8, 6, &Select_Mark, 0);
			OLED_Show_Matrix(13*8, 6, &Select_Mark, 0);
			break;
		}
		// driver1
		case 1:
		{
			if(Driver1.Status==1)
				OLED_Display_Video_Memory(Driver1_AN, 0, 0);
			else if(Driver1.Status==2)
				OLED_Display_Video_Memory(Driver1_SIN, 0, 0);
			else if(Driver1.Status==3)
				OLED_Display_Video_Memory(Driver1_Displacement, 0, 0);
			else if(Driver1.Status==4)
				OLED_Display_Video_Memory(Driver1_SIN_Displacement, 0, 0);
			else if(Driver1.Status==5)
				OLED_Display_Video_Memory(Driver1_EN, 0, 0);
			else if(Driver1.Status==6)
				OLED_Display_Video_Memory(Driver1_BUSY, 0, 0);
			break;
		}
		
		// driver2
		case 2:
		{
			if(Driver2.Status==1)
				OLED_Display_Video_Memory(Driver2_AN, 0, 0);
			else if(Driver2.Status==2)
				OLED_Display_Video_Memory(Driver2_SIN, 0, 0);
			else if(Driver2.Status==3)
				OLED_Display_Video_Memory(Driver2_Displacement, 0, 0);
			else if(Driver2.Status==4)
				OLED_Display_Video_Memory(Driver2_SIN_Displacement, 0, 0);
			else if(Driver2.Status==5)
				OLED_Display_Video_Memory(Driver2_EN, 0, 0);
			else if(Driver2.Status==6)
				OLED_Display_Video_Memory(Driver2_BUSY, 0, 0);
			break;
		}
		// Encoder1
		case 3:
		{
			OLED_Display_Video_Memory(Encoder1_vm, 0, 0);
			break;
		}
		// Encoder2
		case 4:
		{
			OLED_Display_Video_Memory(Encoder2_vm, 0, 0);
			break;
		}
		// Usart
		case 5:
		{
			OLED_Display_Video_Memory(Uart_Tx_vm, 0, 0);
			break;
		}
		// canopen
		case 6:
		{
			if(Canopen.Status == 1)
				OLED_Display_Video_Memory(Can_Enable_vm, 0, 0);
			else if(Canopen.Status == 2)
				OLED_Display_Video_Memory(Can_Out_vm, 0, 0);
			break;
		}
		// DRC1
		case 9:
		{
			if(Delay_Feedback_Controller1.status==0)
				OLED_Display_Video_Memory(DRC1_Off_vm, 0, 0);
			else if(Delay_Feedback_Controller1.status==1)
				OLED_Display_Video_Memory(DRC1_On_vm, 0, 0);
			break;
		}
		// DRC1_Config
		case 10:
		{
			if(Delay_Feedback_Controller1.status==0)
				OLED_Display_Video_Memory(DRC1_Off_Config_vm, 0, 0);
			else if(Delay_Feedback_Controller1.status==1)
				OLED_Display_Video_Memory(DRC1_On_Config_vm, 0, 0);
			break;
		}
		// DRC2
		case 11:
		{
			if(Delay_Feedback_Controller2.status==0)
				OLED_Display_Video_Memory(DRC2_Off_vm, 0, 0);
			else if(Delay_Feedback_Controller2.status==1)
				OLED_Display_Video_Memory(DRC2_On_vm, 0, 0);
			break;
		}
		// DRC2_Config
		case 12:
		{
			if(Delay_Feedback_Controller2.status==0)
				OLED_Display_Video_Memory(DRC2_Off_Config_vm, 0, 0);
			else if(Delay_Feedback_Controller2.status==1)
				OLED_Display_Video_Memory(DRC2_On_Config_vm, 0, 0);
			break;
		}
		// Sweep1
		case 13:
		{
			if(Driver1.Sweep_wave->output_enable == 1)
				OLED_Display_Video_Memory(Sweep1_On_vm, 0, 0);
			else
				OLED_Display_Video_Memory(Sweep1_Off_vm, 0, 0);
			break;
		}
		// Sweep1_Config
		case 14:
		{
			if(Driver1.Sweep_wave->output_enable == 1)
				OLED_Display_Video_Memory(Sweep1_On_Config_vm, 0, 0);
			else
				OLED_Display_Video_Memory(Sweep1_Off_Config_vm, 0, 0);
			break;
		}
		// Sweep2
		case 15:
		{
			if(Driver2.Sweep_wave->output_enable == 1)
				OLED_Display_Video_Memory(Sweep2_On_vm, 0, 0);
			else
				OLED_Display_Video_Memory(Sweep2_Off_vm, 0, 0);
			break;
		}
		// Sweep2_Config
		case 16:
		{
			if(Driver2.Sweep_wave->output_enable == 1)
				OLED_Display_Video_Memory(Sweep2_On_Config_vm, 0, 0);
			else
				OLED_Display_Video_Memory(Sweep2_Off_Config_vm, 0, 0);
			break;
		}
		default:
			break;
	}
}

/*更新显示数据*/
void OLED_Update_Data(void)
{
	//Options_Struct* option = &Option;
	
	float32_t dataf; 
	uint32_t intdata;
	int8_t databit;
	int32_t encoderdata;
	
	if(Top_Device==1)
	{
		if(Driver1.Status==1)
		{
			dataf = Driver1.Output_Voltage;
			if(dataf<0)
			{
				OLED_Show_Number(10*8, 3, 12);
				dataf = -dataf;
			}
			else
				OLED_Show_Number(10*8, 3, 11);
			
			intdata = OLED_Data_Round(dataf);
			databit = intdata/100%10;
			OLED_Show_Number(11*8, 3, databit);
			
			databit = intdata/10%10;
			OLED_Show_Number(13*8, 3, databit);
			
			databit = intdata%10;
			OLED_Show_Number(14*8, 3, databit);
		}
		else if(Driver1.Status==2)
		{
			// 更新幅值
			dataf = Driver1.Sinwave->current_amplitude;
			if(dataf<0)
			{
				OLED_Show_Number(10*8, 3, 12);
				dataf = -dataf;
			}
			else
				OLED_Show_Number(10*8, 3, 11);
			
			intdata = OLED_Data_Round(dataf);
			databit = intdata/100%10;
			OLED_Show_Number(11*8, 3, databit);
			
			databit = intdata/10%10;
			OLED_Show_Number(13*8, 3, databit);
			
			databit = intdata%10;
			OLED_Show_Number(14*8, 3, databit);
			
			// 更新频率
			dataf = Driver1.Sinwave->current_frequency;
			if(dataf<0)
				dataf = -dataf;
			
			intdata = OLED_Data_Round(dataf);
			databit = intdata/100%10;
			OLED_Show_Number(10*8, 6, databit);
			
			databit = intdata/10%10;
			OLED_Show_Number(12*8, 6, databit);
			
			databit = intdata%10;
			OLED_Show_Number(13*8, 6, databit);
		}
		else if(Driver1.Status==3)
		{
			dataf = Driver1.Target_Displacement;
			if(dataf<0)
			{
				OLED_Show_Number(10*8, 3, 12);
				dataf = -dataf;
			}
			else
				OLED_Show_Number(10*8, 3, 11);
			
			intdata = OLED_Data_Round(dataf);
			databit = intdata/100%10;
			OLED_Show_Number(11*8, 3, databit);
			
			databit = intdata/10%10;
			OLED_Show_Number(13*8, 3, databit);
		}
		else if(Driver1.Status==4)
		{
			//  更新幅值
			dataf = Driver1.Sinwave->current_amplitude;
			if(dataf<0)
			{
				OLED_Show_Number(10*8, 3, 12);
				dataf = -dataf;
			}
			else
				OLED_Show_Number(10*8, 3, 11);
			
			intdata = OLED_Data_Round(dataf);
			databit = intdata/100%10;
			OLED_Show_Number(11*8, 3, databit);
			
			databit = intdata/10%10;
			OLED_Show_Number(13*8, 3, databit);
			
			// 更新频率
			dataf = Driver1.Sinwave->current_frequency;
			if(dataf<0)
				dataf = -dataf;
			
			intdata = OLED_Data_Round(dataf);
			databit = intdata/100%10;
			OLED_Show_Number(10*8, 6, databit);
			
			databit = intdata/10%10;
			OLED_Show_Number(12*8, 6, databit);
			
			databit = intdata%10;
			OLED_Show_Number(13*8, 6, databit);
		}
		else if(Driver1.Status==5)
		{
			// 更新distance数据
			encoderdata = Driver1.Distance;
			if(encoderdata<0)
			{
				OLED_Show_Number(10*8, 6, 12);
				encoderdata = -encoderdata;
			}
			else
				OLED_Show_Number(10*8, 6, 11);
			
			databit = encoderdata/1000%10;
			OLED_Show_Number(11*8, 6, databit);
			
			databit = encoderdata/100%10;
			OLED_Show_Number(12*8, 6, databit);
			
			databit = encoderdata/10%10;
			OLED_Show_Number(13*8, 6, databit);
			
			databit = encoderdata%10;
			OLED_Show_Number(14*8, 6, databit);
		}
	}
	else if(Top_Device==2)
	{
		if(Driver2.Status==1)
		{
			dataf = Driver2.Output_Voltage;
			if(dataf<0)
			{
				OLED_Show_Number(10*8, 3, 12);
				dataf = -dataf;
			}
			else
				OLED_Show_Number(10*8, 3, 11);
			
			intdata = OLED_Data_Round(dataf);
			databit = intdata/100%10;
			OLED_Show_Number(11*8, 3, databit);
			
			databit = intdata/10%10;
			OLED_Show_Number(13*8, 3, databit);
			
			databit = intdata%10;
			OLED_Show_Number(14*8, 3, databit);
		}
		else if(Driver2.Status==2)
		{
			// 更新幅值
			dataf = Driver2.Sinwave->current_amplitude;
			if(dataf<0)
			{
				OLED_Show_Number(10*8, 3, 12);
				dataf = -dataf;
			}
			else
				OLED_Show_Number(10*8, 3, 11);
			
			intdata = OLED_Data_Round(dataf);
			databit = intdata/100%10;
			OLED_Show_Number(11*8, 3, databit);
			
			databit = intdata/10%10;
			OLED_Show_Number(13*8, 3, databit);
			
			databit = intdata%10;
			OLED_Show_Number(14*8, 3, databit);
			
			// 更新频率
			dataf = Driver2.Sinwave->current_frequency;
			if(dataf<0)
				dataf = -dataf;
			
			intdata = OLED_Data_Round(dataf);
			databit = intdata/100%10;
			OLED_Show_Number(10*8, 6, databit);
			
			databit = intdata/10%10;
			OLED_Show_Number(12*8, 6, databit);
			
			databit = intdata%10;
			OLED_Show_Number(13*8, 6, databit);
		}
		else if(Driver2.Status==3)
		{
			dataf = Driver2.Target_Displacement;
			if(dataf<0)
			{
				OLED_Show_Number(10*8, 3, 12);
				dataf = -dataf;
			}
			else
				OLED_Show_Number(10*8, 3, 11);
			
			intdata = OLED_Data_Round(dataf);
			databit = intdata/100%10;
			OLED_Show_Number(11*8, 3, databit);
			
			databit = intdata/10%10;
			OLED_Show_Number(13*8, 3, databit);
		}
		else if(Driver2.Status==4)
		{
			// 更新幅值
			dataf = Driver2.Sinwave->current_amplitude;
			if(dataf<0)
			{
				OLED_Show_Number(10*8, 3, 12);
				dataf = -dataf;
			}
			else
				OLED_Show_Number(10*8, 3, 11);
			
			intdata = OLED_Data_Round(dataf);
			databit = intdata/100%10;
			OLED_Show_Number(11*8, 3, databit);
			
			databit = intdata/10%10;
			OLED_Show_Number(13*8, 3, databit);
			
			// 更新频率
			dataf = Driver2.Sinwave->current_frequency;
			if(dataf<0)
				dataf = -dataf;
			
			intdata = OLED_Data_Round(dataf);
			databit = intdata/100%10;
			OLED_Show_Number(10*8, 6, databit);
			
			databit = intdata/10%10;
			OLED_Show_Number(12*8, 6, databit);
			
			databit = intdata%10;
			OLED_Show_Number(13*8, 6, databit);
		}
		else if(Driver2.Status==5)
		{
			// 更新distance数据
			encoderdata = Driver2.Distance;
			if(encoderdata<0)
			{
				OLED_Show_Number(10*8, 6, 12);
				encoderdata = -encoderdata;
			}
			else
				OLED_Show_Number(10*8, 6, 11);
			
			databit = encoderdata/1000%10;
			OLED_Show_Number(11*8, 6, databit);
			
			databit = encoderdata/100%10;
			OLED_Show_Number(12*8, 6, databit);
			
			databit = encoderdata/10%10;
			OLED_Show_Number(13*8, 6, databit);
			
			databit = encoderdata%10;
			OLED_Show_Number(14*8, 6, databit);
		}
	}
	else if(Top_Device==3)
	{
		if(Encoder1.Status==1)
		{
			// 更新distance数据
			encoderdata = Encoder1.Distance;
			if(encoderdata<0)
			{
				OLED_Show_Number(9*8, 6, 12);
				encoderdata = -encoderdata;
			}
			else
				OLED_Show_Number(9*8, 6, 11);
			
			databit = encoderdata/1000%10;
			OLED_Show_Number(10*8, 6, databit);
			
			databit = encoderdata/100%10;
			OLED_Show_Number(12*8, 6, databit);
			
			databit = encoderdata/10%10;
			OLED_Show_Number(13*8, 6, databit);
			
			// 更新rssi
			if(Encoder1.Magnetic_Field<5)
				OLED_Show_Matrix_Pointer(14*8, 3, RSSI, 0);
			else if(Encoder1.Magnetic_Field<20)
				OLED_Show_Matrix_Pointer(14*8, 3, RSSI, 1);
			else if(Encoder1.Magnetic_Field<40)
				OLED_Show_Matrix_Pointer(14*8, 3, RSSI, 2);
			else if(Encoder1.Magnetic_Field<60)
				OLED_Show_Matrix_Pointer(14*8, 3, RSSI, 3);
			else if(Encoder1.Magnetic_Field<80)
				OLED_Show_Matrix_Pointer(14*8, 3, RSSI, 4);
			else
				OLED_Show_Matrix_Pointer(14*8, 3, RSSI, 5);
		}
	}
	else if(Top_Device==4)
	{
		if(Encoder2.Status==1)
		{
			encoderdata = Encoder2.Distance;
			if(encoderdata<0)
			{
				OLED_Show_Number(9*8, 6, 12);
				encoderdata = -encoderdata;
			}
			else
				OLED_Show_Number(9*8, 6, 11);
			
			databit = encoderdata/1000%10;
			OLED_Show_Number(10*8, 6, databit);
			
			databit = encoderdata/100%10;
			OLED_Show_Number(12*8, 6, databit);
			
			databit = encoderdata/10%10;
			OLED_Show_Number(13*8, 6, databit);
			
			// 更新rssi
			if(Encoder2.Magnetic_Field<5)
				OLED_Show_Matrix_Pointer(14*8, 3, RSSI, 0);
			else if(Encoder2.Magnetic_Field<20)
				OLED_Show_Matrix_Pointer(14*8, 3, RSSI, 1);
			else if(Encoder2.Magnetic_Field<40)
				OLED_Show_Matrix_Pointer(14*8, 3, RSSI, 2);
			else if(Encoder2.Magnetic_Field<60)
				OLED_Show_Matrix_Pointer(14*8, 3, RSSI, 3);
			else if(Encoder2.Magnetic_Field<80)
				OLED_Show_Matrix_Pointer(14*8, 3, RSSI, 4);
			else
				OLED_Show_Matrix_Pointer(14*8, 3, RSSI, 5);
		}
	}
	else if(Top_Device==9)
	{
		//更新g
		databit = ((int)Delay_Feedback_Controller1.g)/1000%10;
		OLED_Show_Number(8*8, 3, databit);
		
		databit = ((int)Delay_Feedback_Controller1.g)/100%10;
		OLED_Show_Number(9*8, 3, databit);
		
		databit = ((int)Delay_Feedback_Controller1.g)/10%10;
		OLED_Show_Number(10*8, 3, databit);
	
		databit = ((int)Delay_Feedback_Controller1.g)%10;
		OLED_Show_Number(11*8, 3, databit);
		//更新tau
		databit = Delay_Feedback_Controller1.tau_ms/1000%10;
		OLED_Show_Number(10*8, 6, databit);
		
		databit = Delay_Feedback_Controller1.tau_ms/100%10;
		OLED_Show_Number(11*8, 6, databit);
		
		databit = Delay_Feedback_Controller1.tau_ms/10%10;
		OLED_Show_Number(12*8, 6, databit);
	
		databit = Delay_Feedback_Controller1.tau_ms%10;
		OLED_Show_Number(13*8, 6, databit);
	}
	else if(Top_Device==11)
	{
		//更新g
		databit = ((int)Delay_Feedback_Controller2.g)/1000%10;
		OLED_Show_Number(8*8, 3, databit);
		
		databit = ((int)Delay_Feedback_Controller2.g)/100%10;
		OLED_Show_Number(9*8, 3, databit);
		
		databit = ((int)Delay_Feedback_Controller2.g)/10%10;
		OLED_Show_Number(10*8, 3, databit);
	
		databit = ((int)Delay_Feedback_Controller2.g)%10;
		OLED_Show_Number(11*8, 3, databit);
		//更新tau
		databit = Delay_Feedback_Controller2.tau_ms/1000%10;
		OLED_Show_Number(10*8, 6, databit);
		
		databit = Delay_Feedback_Controller2.tau_ms/100%10;
		OLED_Show_Number(11*8, 6, databit);
		
		databit = Delay_Feedback_Controller2.tau_ms/10%10;
		OLED_Show_Number(12*8, 6, databit);
	
		databit = Delay_Feedback_Controller2.tau_ms%10;
		OLED_Show_Number(13*8, 6, databit);
	}
	else if(Top_Device==13)
	{
		// 更新幅值
		intdata = OLED_Data_Round(Driver1.Sweep_wave->amplitude);
		databit = intdata/100%10;
		OLED_Show_Number(9*8+4, 2, databit);
		databit = intdata/10%10;
		OLED_Show_Number(11*8+4, 2, databit);
		databit = intdata%10;
		OLED_Show_Number(12*8+4, 2, databit);
		
		// 更新扫频频率
		intdata = OLED_Data_Round(Driver1.Sweep_wave->frequency_begin);
		databit = intdata/1000%10;
		OLED_Show_Number(8*8-4, 4, databit);
		databit = intdata/100%10;
		OLED_Show_Number(9*8-4, 4, databit);
		databit = intdata/10%10;
		OLED_Show_Number(11*8-8, 4, databit);
		
		intdata = OLED_Data_Round(Driver1.Sweep_wave->frequency_end);
		databit = intdata/1000%10;
		OLED_Show_Number(13*8-6, 4, databit);
		databit = intdata/100%10;
		OLED_Show_Number(14*8-6, 4, databit);
		databit = intdata/10%10;
		OLED_Show_Number(15*8-2, 4, databit);
	
		
		//更新ts
		intdata = Driver1.Sweep_wave->time;
		databit = intdata/100%10;
		OLED_Show_Number(9*8-4, 6, databit);
		databit = intdata/10%10;
		OLED_Show_Number(10*8-4, 6, databit);
		databit = intdata%10;
		OLED_Show_Number(11*8-4, 6, databit);
	}
	else if(Top_Device==15)
	{
		// 更新幅值
		intdata = OLED_Data_Round(Driver2.Sweep_wave->amplitude);
		databit = intdata/100%10;
		OLED_Show_Number(9*8+4, 2, databit);
		databit = intdata/10%10;
		OLED_Show_Number(11*8+4, 2, databit);
		databit = intdata%10;
		OLED_Show_Number(12*8+4, 2, databit);
		
		// 更新扫频频率
		intdata = OLED_Data_Round(Driver2.Sweep_wave->frequency_begin);
		databit = intdata/1000%10;
		OLED_Show_Number(8*8-4, 4, databit);
		databit = intdata/100%10;
		OLED_Show_Number(9*8-4, 4, databit);
		databit = intdata/10%10;
		OLED_Show_Number(11*8-8, 4, databit);
		
		intdata = OLED_Data_Round(Driver2.Sweep_wave->frequency_end);
		databit = intdata/1000%10;
		OLED_Show_Number(13*8-6, 4, databit);
		databit = intdata/100%10;
		OLED_Show_Number(14*8-6, 4, databit);
		databit = intdata/10%10;
		OLED_Show_Number(15*8-2, 4, databit);
	
		
		//更新ts
		intdata = Driver2.Sweep_wave->time;
		databit = intdata/100%10;
		OLED_Show_Number(9*8-4, 6, databit);
		databit = intdata/10%10;
		OLED_Show_Number(10*8-4, 6, databit);
		databit = intdata%10;
		OLED_Show_Number(11*8-4, 6, databit);
	}
}


/* 显示数据四舍五入  两位小数*/
uint32_t OLED_Data_Round(float32_t data)
{
	uint32_t intdata = (int)(data*1000);
	uint8_t bit3 = intdata%10;
	if(bit3<5)
		return intdata/10;
	else 
		return intdata/10+1;
}

/*更新光标*/
void OLED_Update_Cursor(void)
{
	
	Options_Struct* option = &Option;
	
	switch(Top_Device)
	{
		case 1:
		{
			if((Driver1.Status==1)||(Driver1.Status==3))
			{
				if(option->Normal_Option==0)
				{
					OLED_Show_Matrix(48-3, 3, &Square_Brackets, 2);
					OLED_Show_Matrix(71, 3, &Square_Brackets, 2);
					OLED_Show_Matrix(48-3, 6, &Square_Brackets, 2);
					OLED_Show_Matrix(71, 6, &Square_Brackets, 2);
				}
				else if(option->Normal_Option==1)
				{
					OLED_Show_Matrix(48-3, 3, &Square_Brackets, 0);
					OLED_Show_Matrix(71, 3, &Square_Brackets, 1);
					OLED_Show_Matrix(48-3, 6, &Square_Brackets, 2);
					OLED_Show_Matrix(71, 6, &Square_Brackets, 2);
				}
			}
			else if((Driver1.Status==2)||(Driver1.Status==4))
			{
				if(option->Normal_Option==0)
				{
					OLED_Show_Matrix(48-3, 3, &Square_Brackets, 2);
					OLED_Show_Matrix(71, 3, &Square_Brackets, 2);
					OLED_Show_Matrix(48-3, 6, &Square_Brackets, 2);
					OLED_Show_Matrix(71, 6, &Square_Brackets, 2);
				}
				else if(option->Normal_Option==1)
				{
					OLED_Show_Matrix(48-3, 3, &Square_Brackets, 0);
					OLED_Show_Matrix(71, 3, &Square_Brackets, 1);
					OLED_Show_Matrix(48-3, 6, &Square_Brackets, 2);
					OLED_Show_Matrix(71, 6, &Square_Brackets, 2);
				}
				else if(option->Normal_Option==2)
				{
					OLED_Show_Matrix(48-3, 3, &Square_Brackets, 2);
					OLED_Show_Matrix(71, 3, &Square_Brackets, 2);
					OLED_Show_Matrix(48-3, 6, &Square_Brackets, 0);
					OLED_Show_Matrix(71, 6, &Square_Brackets, 1);
				}
			}
			break;
		}
		case 2:
		{
			if((Driver2.Status==1)||(Driver2.Status==3))
			{
				if(option->Normal_Option==0)
				{
					OLED_Show_Matrix(48-3, 3, &Square_Brackets, 2);
					OLED_Show_Matrix(71, 3, &Square_Brackets, 2);
					OLED_Show_Matrix(48-3, 6, &Square_Brackets, 2);
					OLED_Show_Matrix(71, 6, &Square_Brackets, 2);
				}
				else if(option->Normal_Option==1)
				{
					OLED_Show_Matrix(48-3, 3, &Square_Brackets, 0);
					OLED_Show_Matrix(71, 3, &Square_Brackets, 1);
					OLED_Show_Matrix(48-3, 6, &Square_Brackets, 2);
					OLED_Show_Matrix(71, 6, &Square_Brackets, 2);
				}
			}
			else if((Driver2.Status==2)||(Driver2.Status==4))
			{
				if(option->Normal_Option==0)
				{
					OLED_Show_Matrix(48-3, 3, &Square_Brackets, 2);
					OLED_Show_Matrix(71, 3, &Square_Brackets, 2);
					OLED_Show_Matrix(48-3, 6, &Square_Brackets, 2);
					OLED_Show_Matrix(71, 6, &Square_Brackets, 2);
				}
				else if(option->Normal_Option==1)
				{
					OLED_Show_Matrix(48-3, 3, &Square_Brackets, 0);
					OLED_Show_Matrix(71, 3, &Square_Brackets, 1);
					OLED_Show_Matrix(48-3, 6, &Square_Brackets, 2);
					OLED_Show_Matrix(71, 6, &Square_Brackets, 2);
				}
				else if(option->Normal_Option==2)
				{
					OLED_Show_Matrix(48-3, 3, &Square_Brackets, 2);
					OLED_Show_Matrix(71, 3, &Square_Brackets, 2);
					OLED_Show_Matrix(48-3, 6, &Square_Brackets, 0);
					OLED_Show_Matrix(71, 6, &Square_Brackets, 1);
				}
			}
			break;
		}
		case 5:
		{
			if(option->Usart_DataTx_Option==0)
			{
				OLED_Show_Matrix(7*8-3, 3, &Square_Brackets, 2);
				OLED_Show_Matrix(10*8, 3, &Square_Brackets, 2);
				
				OLED_Show_Matrix(12*8-3, 3, &Square_Brackets, 2);
				OLED_Show_Matrix(15*8, 3, &Square_Brackets, 2);
				
				OLED_Show_Matrix(7*8-3, 6, &Square_Brackets, 2);
				OLED_Show_Matrix(10*8, 6, &Square_Brackets, 2);
				
				OLED_Show_Matrix(12*8-3, 6, &Square_Brackets, 2);
				OLED_Show_Matrix(15*8, 6, &Square_Brackets, 2);
			}
			else if(option->Usart_DataTx_Option==1)
			{
				OLED_Show_Matrix(7*8-3, 3, &Square_Brackets, 0);
				OLED_Show_Matrix(10*8, 3, &Square_Brackets, 1);
				
				OLED_Show_Matrix(12*8-3, 3, &Square_Brackets, 2);
				OLED_Show_Matrix(15*8, 3, &Square_Brackets, 2);
				
				OLED_Show_Matrix(7*8-3, 6, &Square_Brackets, 2);
				OLED_Show_Matrix(10*8, 6, &Square_Brackets, 2);
				
				OLED_Show_Matrix(12*8-3, 6, &Square_Brackets, 2);
				OLED_Show_Matrix(15*8, 6, &Square_Brackets, 2);
			}
			else if(option->Usart_DataTx_Option==2)
			{
				OLED_Show_Matrix(7*8-3, 3, &Square_Brackets, 2);
				OLED_Show_Matrix(10*8, 3, &Square_Brackets, 2);
				
				OLED_Show_Matrix(12*8-3, 3, &Square_Brackets, 0);
				OLED_Show_Matrix(15*8, 3, &Square_Brackets, 1);
				
				OLED_Show_Matrix(7*8-3, 6, &Square_Brackets, 2);
				OLED_Show_Matrix(10*8, 6, &Square_Brackets, 2);
				
				OLED_Show_Matrix(12*8-3, 6, &Square_Brackets, 2);
				OLED_Show_Matrix(15*8, 6, &Square_Brackets, 2);
			}
			else if(option->Usart_DataTx_Option==3)
			{
				OLED_Show_Matrix(7*8-3, 3, &Square_Brackets, 2);
				OLED_Show_Matrix(10*8, 3, &Square_Brackets, 2);
				
				OLED_Show_Matrix(12*8-3, 3, &Square_Brackets, 2);
				OLED_Show_Matrix(15*8, 3, &Square_Brackets, 2);
				
				OLED_Show_Matrix(7*8-3, 6, &Square_Brackets, 0);
				OLED_Show_Matrix(10*8, 6, &Square_Brackets, 1);
				
				OLED_Show_Matrix(12*8-3, 6, &Square_Brackets, 2);
				OLED_Show_Matrix(15*8, 6, &Square_Brackets, 2);
			}
			else if(option->Usart_DataTx_Option==4)
			{
				OLED_Show_Matrix(7*8-3, 3, &Square_Brackets, 2);
				OLED_Show_Matrix(10*8, 3, &Square_Brackets, 2);
				
				OLED_Show_Matrix(12*8-3, 3, &Square_Brackets, 2);
				OLED_Show_Matrix(15*8, 3, &Square_Brackets, 2);
				
				OLED_Show_Matrix(7*8-3, 6, &Square_Brackets, 2);
				OLED_Show_Matrix(10*8, 6, &Square_Brackets, 2);
				
				OLED_Show_Matrix(12*8-3, 6, &Square_Brackets, 0);
				OLED_Show_Matrix(15*8, 6, &Square_Brackets, 1);
			}
			break;
		}
		case 6:
		{
			uint8_t Set_Options;
			if(Canopen.Status == 1)
				Set_Options = option->Driver_Current_Status;
			else if(Canopen.Status == 2)
				Set_Options = option->Driver_CAN_Out;
			else
				Set_Options = 0;
			
			if(Set_Options == 0)
			{
				OLED_Show_Matrix(6*8-3, 3, &Square_Brackets, 2);
				OLED_Show_Matrix(15*8-1, 3, &Square_Brackets, 2);
				
				OLED_Show_Matrix(6*8-3, 6, &Square_Brackets, 2);
				OLED_Show_Matrix(15*8-1, 6, &Square_Brackets, 2);
			}
			else if(Set_Options == 1)
			{
				OLED_Show_Matrix(6*8-3, 3, &Square_Brackets, 2);
				OLED_Show_Matrix(15*8-1, 3, &Square_Brackets, 2);
				
				OLED_Show_Matrix(6*8-3, 6, &Square_Brackets, 0);
				OLED_Show_Matrix(15*8-1, 6, &Square_Brackets, 1);
			}
			else if(Set_Options == 2)
			{
				OLED_Show_Matrix(6*8-3, 3, &Square_Brackets, 0);
				OLED_Show_Matrix(15*8-1, 3, &Square_Brackets, 1);
				
				OLED_Show_Matrix(6*8-3, 6, &Square_Brackets, 2);
				OLED_Show_Matrix(15*8-1, 6, &Square_Brackets, 2);
			}
			else if(Set_Options == 3)
			{
				OLED_Show_Matrix(6*8-3, 3, &Square_Brackets, 0);
				OLED_Show_Matrix(15*8-1, 3, &Square_Brackets, 1);
				
				OLED_Show_Matrix(6*8-3, 6, &Square_Brackets, 0);
				OLED_Show_Matrix(15*8-1, 6, &Square_Brackets, 1);
			}
			break;
		}
		case 9:
		{
			if(option->Normal_Option==0)
			{
				OLED_Show_Matrix(6*8-3, 3, &Square_Brackets, 2);
				OLED_Show_Matrix(7*8-1, 3, &Square_Brackets, 2);
				
				OLED_Show_Matrix(6*8-3, 6, &Square_Brackets, 2);
				OLED_Show_Matrix(9*8-1, 6, &Square_Brackets, 2);
			}
			else if(option->Normal_Option==1)
			{
				OLED_Show_Matrix(6*8-3, 3, &Square_Brackets, 0);
				OLED_Show_Matrix(7*8-1, 3, &Square_Brackets, 1);
				
				OLED_Show_Matrix(6*8-3, 6, &Square_Brackets, 2);
				OLED_Show_Matrix(9*8-1, 6, &Square_Brackets, 2);
			}
			else if(option->Normal_Option==2)
			{
				OLED_Show_Matrix(6*8-3, 3, &Square_Brackets, 2);
				OLED_Show_Matrix(7*8-1, 3, &Square_Brackets, 2);
				
				OLED_Show_Matrix(6*8-3, 6, &Square_Brackets, 0);
				OLED_Show_Matrix(9*8-1, 6, &Square_Brackets, 1);
			}
			break;
		}
		case 10:
		{
			if(option->DRC1_DataSource_Option==0)
			{
				OLED_Show_Matrix(7*8-3, 3, &Square_Brackets, 2);
				OLED_Show_Matrix(10*8, 3, &Square_Brackets, 2);
				
				OLED_Show_Matrix(12*8-3, 3, &Square_Brackets, 2);
				OLED_Show_Matrix(15*8, 3, &Square_Brackets, 2);
				
				OLED_Show_Matrix(7*8-3, 6, &Square_Brackets, 2);
				OLED_Show_Matrix(10*8, 6, &Square_Brackets, 2);
				
				OLED_Show_Matrix(12*8-3, 6, &Square_Brackets, 2);
				OLED_Show_Matrix(15*8, 6, &Square_Brackets, 2);
			}
			else if(option->DRC1_DataSource_Option==1)
			{
				OLED_Show_Matrix(7*8-3, 3, &Square_Brackets, 0);
				OLED_Show_Matrix(10*8, 3, &Square_Brackets, 1);
				
				OLED_Show_Matrix(12*8-3, 3, &Square_Brackets, 2);
				OLED_Show_Matrix(15*8, 3, &Square_Brackets, 2);
				
				OLED_Show_Matrix(7*8-3, 6, &Square_Brackets, 2);
				OLED_Show_Matrix(10*8, 6, &Square_Brackets, 2);
				
				OLED_Show_Matrix(12*8-3, 6, &Square_Brackets, 2);
				OLED_Show_Matrix(15*8, 6, &Square_Brackets, 2);
			}
			else if(option->DRC1_DataSource_Option==2)
			{
				OLED_Show_Matrix(7*8-3, 3, &Square_Brackets, 2);
				OLED_Show_Matrix(10*8, 3, &Square_Brackets, 2);
				
				OLED_Show_Matrix(12*8-3, 3, &Square_Brackets, 0);
				OLED_Show_Matrix(15*8, 3, &Square_Brackets, 1);
				
				OLED_Show_Matrix(7*8-3, 6, &Square_Brackets, 2);
				OLED_Show_Matrix(10*8, 6, &Square_Brackets, 2);
				
				OLED_Show_Matrix(12*8-3, 6, &Square_Brackets, 2);
				OLED_Show_Matrix(15*8, 6, &Square_Brackets, 2);
			}
			else if(option->DRC1_DataSource_Option==3)
			{
				OLED_Show_Matrix(7*8-3, 3, &Square_Brackets, 2);
				OLED_Show_Matrix(10*8, 3, &Square_Brackets, 2);
				
				OLED_Show_Matrix(12*8-3, 3, &Square_Brackets, 2);
				OLED_Show_Matrix(15*8, 3, &Square_Brackets, 2);
				
				OLED_Show_Matrix(7*8-3, 6, &Square_Brackets, 0);
				OLED_Show_Matrix(10*8, 6, &Square_Brackets, 1);
				
				OLED_Show_Matrix(12*8-3, 6, &Square_Brackets, 2);
				OLED_Show_Matrix(15*8, 6, &Square_Brackets, 2);
			}
			else if(option->DRC1_DataSource_Option==4)
			{
				OLED_Show_Matrix(7*8-3, 3, &Square_Brackets, 2);
				OLED_Show_Matrix(10*8, 3, &Square_Brackets, 2);
				
				OLED_Show_Matrix(12*8-3, 3, &Square_Brackets, 2);
				OLED_Show_Matrix(15*8, 3, &Square_Brackets, 2);
				
				OLED_Show_Matrix(7*8-3, 6, &Square_Brackets, 2);
				OLED_Show_Matrix(10*8, 6, &Square_Brackets, 2);
				
				OLED_Show_Matrix(12*8-3, 6, &Square_Brackets, 0);
				OLED_Show_Matrix(15*8, 6, &Square_Brackets, 1);
			}
			break;
		}
		case 11:
		{
			if(option->Normal_Option==0)
			{
				OLED_Show_Matrix(6*8-3, 3, &Square_Brackets, 2);
				OLED_Show_Matrix(7*8-1, 3, &Square_Brackets, 2);
				
				OLED_Show_Matrix(6*8-3, 6, &Square_Brackets, 2);
				OLED_Show_Matrix(9*8-1, 6, &Square_Brackets, 2);
			}
			else if(option->Normal_Option==1)
			{
				OLED_Show_Matrix(6*8-3, 3, &Square_Brackets, 0);
				OLED_Show_Matrix(7*8-1, 3, &Square_Brackets, 1);
				
				OLED_Show_Matrix(6*8-3, 6, &Square_Brackets, 2);
				OLED_Show_Matrix(9*8-1, 6, &Square_Brackets, 2);
			}
			else if(option->Normal_Option==2)
			{
				OLED_Show_Matrix(6*8-3, 3, &Square_Brackets, 2);
				OLED_Show_Matrix(7*8-1, 3, &Square_Brackets, 2);
				
				OLED_Show_Matrix(6*8-3, 6, &Square_Brackets, 0);
				OLED_Show_Matrix(9*8-1, 6, &Square_Brackets, 1);
			}
			break;
		}
		case 12:
		{
			if(option->DRC2_DataSource_Option==0)
			{
				OLED_Show_Matrix(7*8-3, 3, &Square_Brackets, 2);
				OLED_Show_Matrix(10*8, 3, &Square_Brackets, 2);
				
				OLED_Show_Matrix(12*8-3, 3, &Square_Brackets, 2);
				OLED_Show_Matrix(15*8, 3, &Square_Brackets, 2);
				
				OLED_Show_Matrix(7*8-3, 6, &Square_Brackets, 2);
				OLED_Show_Matrix(10*8, 6, &Square_Brackets, 2);
				
				OLED_Show_Matrix(12*8-3, 6, &Square_Brackets, 2);
				OLED_Show_Matrix(15*8, 6, &Square_Brackets, 2);
			}
			else if(option->DRC2_DataSource_Option==1)
			{
				OLED_Show_Matrix(7*8-3, 3, &Square_Brackets, 0);
				OLED_Show_Matrix(10*8, 3, &Square_Brackets, 1);
				
				OLED_Show_Matrix(12*8-3, 3, &Square_Brackets, 2);
				OLED_Show_Matrix(15*8, 3, &Square_Brackets, 2);
				
				OLED_Show_Matrix(7*8-3, 6, &Square_Brackets, 2);
				OLED_Show_Matrix(10*8, 6, &Square_Brackets, 2);
				
				OLED_Show_Matrix(12*8-3, 6, &Square_Brackets, 2);
				OLED_Show_Matrix(15*8, 6, &Square_Brackets, 2);
			}
			else if(option->DRC2_DataSource_Option==2)
			{
				OLED_Show_Matrix(7*8-3, 3, &Square_Brackets, 2);
				OLED_Show_Matrix(10*8, 3, &Square_Brackets, 2);
				
				OLED_Show_Matrix(12*8-3, 3, &Square_Brackets, 0);
				OLED_Show_Matrix(15*8, 3, &Square_Brackets, 1);
				
				OLED_Show_Matrix(7*8-3, 6, &Square_Brackets, 2);
				OLED_Show_Matrix(10*8, 6, &Square_Brackets, 2);
				
				OLED_Show_Matrix(12*8-3, 6, &Square_Brackets, 2);
				OLED_Show_Matrix(15*8, 6, &Square_Brackets, 2);
			}
			else if(option->DRC2_DataSource_Option==3)
			{
				OLED_Show_Matrix(7*8-3, 3, &Square_Brackets, 2);
				OLED_Show_Matrix(10*8, 3, &Square_Brackets, 2);
				
				OLED_Show_Matrix(12*8-3, 3, &Square_Brackets, 2);
				OLED_Show_Matrix(15*8, 3, &Square_Brackets, 2);
				
				OLED_Show_Matrix(7*8-3, 6, &Square_Brackets, 0);
				OLED_Show_Matrix(10*8, 6, &Square_Brackets, 1);
				
				OLED_Show_Matrix(12*8-3, 6, &Square_Brackets, 2);
				OLED_Show_Matrix(15*8, 6, &Square_Brackets, 2);
			}
			else if(option->DRC2_DataSource_Option==4)
			{
				OLED_Show_Matrix(7*8-3, 3, &Square_Brackets, 2);
				OLED_Show_Matrix(10*8, 3, &Square_Brackets, 2);
				
				OLED_Show_Matrix(12*8-3, 3, &Square_Brackets, 2);
				OLED_Show_Matrix(15*8, 3, &Square_Brackets, 2);
				
				OLED_Show_Matrix(7*8-3, 6, &Square_Brackets, 2);
				OLED_Show_Matrix(10*8, 6, &Square_Brackets, 2);
				
				OLED_Show_Matrix(12*8-3, 6, &Square_Brackets, 0);
				OLED_Show_Matrix(15*8, 6, &Square_Brackets, 1);
			}
			break;
		}
		case 13:
		{
			if(option->Normal_Option == 0)
			{
				OLED_Show_Matrix(6*8-6, 2, &Square_Brackets, 2);
				OLED_Show_Matrix(9*8-5, 2, &Square_Brackets, 2);
				
				OLED_Show_Matrix(8*8-6, 4, &Square_Brackets, 2);
				OLED_Show_Matrix(11*8-1, 4, &Square_Brackets, 2);
				
				OLED_Show_Matrix(13*8-8, 4, &Square_Brackets, 2);
				OLED_Show_Matrix(15*8+5, 4, &Square_Brackets, 2);
				
				OLED_Show_Matrix(6*8-6, 6, &Square_Brackets, 2);
				OLED_Show_Matrix(8*8-5, 6, &Square_Brackets, 2);
			}
			else if(option->Normal_Option == 1)
			{
				OLED_Show_Matrix(6*8-6, 2, &Square_Brackets, 0);
				OLED_Show_Matrix(9*8-5, 2, &Square_Brackets, 1);
				
				OLED_Show_Matrix(8*8-6, 4, &Square_Brackets, 2);
				OLED_Show_Matrix(11*8-1, 4, &Square_Brackets, 2);
				
				OLED_Show_Matrix(13*8-8, 4, &Square_Brackets, 2);
				OLED_Show_Matrix(15*8+5, 4, &Square_Brackets, 2);
				
				OLED_Show_Matrix(6*8-6, 6, &Square_Brackets, 2);
				OLED_Show_Matrix(8*8-5, 6, &Square_Brackets, 2);
			}
			else if(option->Normal_Option == 2)
			{
				OLED_Show_Matrix(6*8-6, 2, &Square_Brackets, 2);
				OLED_Show_Matrix(9*8-5, 2, &Square_Brackets, 2);
				
				OLED_Show_Matrix(8*8-6, 4, &Square_Brackets, 0);
				OLED_Show_Matrix(11*8-1, 4, &Square_Brackets, 1);
				
				OLED_Show_Matrix(13*8-8, 4, &Square_Brackets, 2);
				OLED_Show_Matrix(15*8+5, 4, &Square_Brackets, 2);
				
				OLED_Show_Matrix(6*8-6, 6, &Square_Brackets, 2);
				OLED_Show_Matrix(8*8-5, 6, &Square_Brackets, 2);
			}
			else if(option->Normal_Option == 3)
			{
				OLED_Show_Matrix(6*8-6, 2, &Square_Brackets, 2);
				OLED_Show_Matrix(9*8-5, 2, &Square_Brackets, 2);
				
				OLED_Show_Matrix(8*8-6, 4, &Square_Brackets, 2);
				OLED_Show_Matrix(11*8-1, 4, &Square_Brackets, 2);
				
				OLED_Show_Matrix(13*8-8, 4, &Square_Brackets, 0);
				OLED_Show_Matrix(15*8+5, 4, &Square_Brackets, 1);
				
				OLED_Show_Matrix(6*8-6, 6, &Square_Brackets, 2);
				OLED_Show_Matrix(8*8-5, 6, &Square_Brackets, 2);
			}
			else if(option->Normal_Option == 4)
			{
				OLED_Show_Matrix(6*8-6, 2, &Square_Brackets, 2);
				OLED_Show_Matrix(9*8-5, 2, &Square_Brackets, 2);
				
				OLED_Show_Matrix(8*8-6, 4, &Square_Brackets, 2);
				OLED_Show_Matrix(11*8-1, 4, &Square_Brackets, 2);
				
				OLED_Show_Matrix(13*8-8, 4, &Square_Brackets, 2);
				OLED_Show_Matrix(15*8+5, 4, &Square_Brackets, 2);
				
				OLED_Show_Matrix(6*8-6, 6, &Square_Brackets, 0);
				OLED_Show_Matrix(8*8-5, 6, &Square_Brackets, 1);
			}
			break;
		}
		case 14:
		{
			if(option->Sweep1_Mode == 1)
			{
				OLED_Show_Matrix(8*8-3, 3, &Square_Brackets, 0);
				OLED_Show_Matrix(15*8, 3, &Square_Brackets, 1);
				
				OLED_Show_Matrix(7*8-3, 6, &Square_Brackets, 2);
				OLED_Show_Matrix(15*8, 6, &Square_Brackets, 2);
			}
			else if(option->Sweep1_Mode == 2)
			{
				OLED_Show_Matrix(8*8-3, 3, &Square_Brackets, 2);
				OLED_Show_Matrix(15*8, 3, &Square_Brackets, 2);
				
				OLED_Show_Matrix(7*8-3, 6, &Square_Brackets, 0);
				OLED_Show_Matrix(15*8, 6, &Square_Brackets, 1);
			}
			break;
		}
		case 15:
		{
			if(option->Normal_Option == 0)
			{
				OLED_Show_Matrix(6*8-6, 2, &Square_Brackets, 2);
				OLED_Show_Matrix(9*8-5, 2, &Square_Brackets, 2);
				
				OLED_Show_Matrix(8*8-6, 4, &Square_Brackets, 2);
				OLED_Show_Matrix(11*8-1, 4, &Square_Brackets, 2);
				
				OLED_Show_Matrix(13*8-8, 4, &Square_Brackets, 2);
				OLED_Show_Matrix(15*8+5, 4, &Square_Brackets, 2);
				
				OLED_Show_Matrix(6*8-6, 6, &Square_Brackets, 2);
				OLED_Show_Matrix(8*8-5, 6, &Square_Brackets, 2);
			}
			else if(option->Normal_Option == 1)
			{
				OLED_Show_Matrix(6*8-6, 2, &Square_Brackets, 0);
				OLED_Show_Matrix(9*8-5, 2, &Square_Brackets, 1);
				
				OLED_Show_Matrix(8*8-6, 4, &Square_Brackets, 2);
				OLED_Show_Matrix(11*8-1, 4, &Square_Brackets, 2);
				
				OLED_Show_Matrix(13*8-8, 4, &Square_Brackets, 2);
				OLED_Show_Matrix(15*8+5, 4, &Square_Brackets, 2);
				
				OLED_Show_Matrix(6*8-6, 6, &Square_Brackets, 2);
				OLED_Show_Matrix(8*8-5, 6, &Square_Brackets, 2);
			}
			else if(option->Normal_Option == 2)
			{
				OLED_Show_Matrix(6*8-6, 2, &Square_Brackets, 2);
				OLED_Show_Matrix(9*8-5, 2, &Square_Brackets, 2);
				
				OLED_Show_Matrix(8*8-6, 4, &Square_Brackets, 0);
				OLED_Show_Matrix(11*8-1, 4, &Square_Brackets, 1);
				
				OLED_Show_Matrix(13*8-8, 4, &Square_Brackets, 2);
				OLED_Show_Matrix(15*8+5, 4, &Square_Brackets, 2);
				
				OLED_Show_Matrix(6*8-6, 6, &Square_Brackets, 2);
				OLED_Show_Matrix(8*8-5, 6, &Square_Brackets, 2);
			}
			else if(option->Normal_Option == 3)
			{
				OLED_Show_Matrix(6*8-6, 2, &Square_Brackets, 2);
				OLED_Show_Matrix(9*8-5, 2, &Square_Brackets, 2);
				
				OLED_Show_Matrix(8*8-6, 4, &Square_Brackets, 2);
				OLED_Show_Matrix(11*8-1, 4, &Square_Brackets, 2);
				
				OLED_Show_Matrix(13*8-8, 4, &Square_Brackets, 0);
				OLED_Show_Matrix(15*8+5, 4, &Square_Brackets, 1);
				
				OLED_Show_Matrix(6*8-6, 6, &Square_Brackets, 2);
				OLED_Show_Matrix(8*8-5, 6, &Square_Brackets, 2);
			}
			else if(option->Normal_Option == 4)
			{
				OLED_Show_Matrix(6*8-6, 2, &Square_Brackets, 2);
				OLED_Show_Matrix(9*8-5, 2, &Square_Brackets, 2);
				
				OLED_Show_Matrix(8*8-6, 4, &Square_Brackets, 2);
				OLED_Show_Matrix(11*8-1, 4, &Square_Brackets, 2);
				
				OLED_Show_Matrix(13*8-8, 4, &Square_Brackets, 2);
				OLED_Show_Matrix(15*8+5, 4, &Square_Brackets, 2);
				
				OLED_Show_Matrix(6*8-6, 6, &Square_Brackets, 0);
				OLED_Show_Matrix(8*8-5, 6, &Square_Brackets, 1);
			}
			break;
		}
		case 16:
		{
			if(option->Sweep2_Mode == 1)
			{
				OLED_Show_Matrix(8*8-3, 3, &Square_Brackets, 0);
				OLED_Show_Matrix(15*8, 3, &Square_Brackets, 1);
				
				OLED_Show_Matrix(7*8-3, 6, &Square_Brackets, 2);
				OLED_Show_Matrix(15*8, 6, &Square_Brackets, 2);
			}
			else if(option->Sweep2_Mode == 2)
			{
				OLED_Show_Matrix(8*8-3, 3, &Square_Brackets, 2);
				OLED_Show_Matrix(15*8, 3, &Square_Brackets, 2);
				
				OLED_Show_Matrix(7*8-3, 6, &Square_Brackets, 0);
				OLED_Show_Matrix(15*8, 6, &Square_Brackets, 1);
			}
			break;
		}
		default:
			break;
	}
}

