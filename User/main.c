#include "stm32f4xx.h"

#include "vibration_system.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "semphr.h"
#include "timers.h"
#include "queue.h"


static TaskHandle_t AppTaskCreate_Handle = NULL;	 	// 创建任务句柄
static TaskHandle_t LED_Task_Handle = NULL;				// LED任务句柄
static TaskHandle_t KEY_Task_Handle = NULL;				// KEY任务句柄
static TaskHandle_t OLED_Task_Handle = NULL;			// OLED任务句柄
static TaskHandle_t ENCODER_Task_Handle = NULL;			// ENCODER任务句柄
static TaskHandle_t USART_TX_Task_Handle = NULL;		// USART_TX任务句柄
static TaskHandle_t DELAY_Task_Handle = NULL;			// DELAY任务句柄
TaskHandle_t Chairp_Task_Handle = NULL;					// Chairp任务句柄
static TaskHandle_t USART_RX_Task_Handle = NULL;		// 串口接收任务句柄
static TaskHandle_t FFT_ANALYSE_Task_Handle = NULL;		// FFT分析任务句柄
static TaskHandle_t SPI_Task_Handle = NULL;				// spi发送任务句柄



EventGroupHandle_t KEY_EventGroup;									// 按键事件组
EventGroupHandle_t OLED_EventGroup;									// OLED事件组
SemaphoreHandle_t OLED_BinarySemaphore;							// OLED二值信号量，硬件保护
TimerHandle_t USART_TX_Timer;												// 串口发送定时器
SemaphoreHandle_t USART_TX_BinarySemaphore;					// OLED二值信号量
TimerHandle_t DELAY_UPDATE_Timer;										// 历史数据更新定时器
QueueHandle_t USART_RX_Queue;												// 串口接收数据消息队列
TimerHandle_t FFT_ANALYSE_Timer;										// fft分析定时器


static void AppTaskCreate(void);
static void LED_Task(void *parameter);
static void KEY_Task(void *parameter);
static void OLED_Task(void *parameter);
static void ENCODER_Task(void *parameter);
static void USART_TX_Task(void *parameter);
static void UASRT_TX_TimerCallback(void *parameter);
static void Chairp_Task(void *parameter);
static void DELAY_Task(void *parameter);
static void DELAY_UPDATE_TimerCallback(void *parameter);
static void USART_RX_Task(void *parameter);
static void FFT_ANALYSE_Task(void *parameter);
static void FFT_ANALYSE_TimerCallback(void *parameter);
static void SPI_Task(void *parameter);

extern uint8_t Top_Device;
extern Driver_Struct Driver1;
extern Driver_Struct Driver2;
extern Options_Struct Option;
extern Encoder_Struct Encoder1;
extern Encoder_Struct Encoder2;
extern Delay_Feedback_Struct Delay_Feedback_Controller1;
extern Delay_Feedback_Struct Delay_Feedback_Controller2;

/*******************************主函数*******************************/
int main(void)
{
	
	Vibration_System_Init();
	
	
	BaseType_t xReturn = pdPASS;
	xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate,
											  (const char*)"AppTaskCreate", 
												(uint16_t)128,
												(void *) NULL,
												(UBaseType_t)1,		
												(TaskHandle_t*)&AppTaskCreate_Handle );
								
	if(xReturn == pdPASS)
    vTaskStartScheduler();
	while(1);
}
/*****************************主函数End******************************/

/* AppTaskCreate任务函数 */
static void AppTaskCreate(void)
{
	taskENTER_CRITICAL();
	BaseType_t xReturn;
	
	/* 创建LED_Task任务 */
	xReturn = pdPASS;
	xReturn = xTaskCreate((TaskFunction_t )LED_Task,
											  (const char*)"LED_Task", 
												(uint16_t)128,
												(void *) NULL,
												(UBaseType_t)1,
												(TaskHandle_t*)&LED_Task_Handle );
	if(xReturn != pdPASS)
		while(1);
	
	/* 创建按键检测任务 */
	KEY_EventGroup = xEventGroupCreate();
	xReturn = xTaskCreate((TaskFunction_t )KEY_Task,
											  (const char*)"KEY_Task", 
												(uint16_t)128,
												(void *) NULL,
												(UBaseType_t)1,
												(TaskHandle_t*)&KEY_Task_Handle );
	if(xReturn != pdPASS)
		while(1);
	
	/* 创建OLED任务 */
	OLED_EventGroup = xEventGroupCreate();
	OLED_BinarySemaphore = xSemaphoreCreateBinary();
	xSemaphoreGive(OLED_BinarySemaphore);
	xReturn = xTaskCreate((TaskFunction_t )OLED_Task,
											  (const char*)"OLED_Task", 
												(uint16_t)128,
												(void *) NULL,
												(UBaseType_t)1,
												(TaskHandle_t*)&OLED_Task_Handle );
	if(xReturn != pdPASS)
		while(1);
	
	/* 创建ENCODER任务 */
	xReturn = xTaskCreate((TaskFunction_t )ENCODER_Task,
											  (const char*)"ENCODER_Task", 
												(uint16_t)128,
												(void *) NULL,
												(UBaseType_t)1,
												(TaskHandle_t*)&ENCODER_Task_Handle );
	if(xReturn != pdPASS)
		while(1);
	
	/* 创建USART发送任务 */
	USART_TX_BinarySemaphore = xSemaphoreCreateBinary();		// 二值信号量
	xSemaphoreGive(USART_TX_BinarySemaphore);
	USART_TX_Timer = xTimerCreate("USART_TX_Timer", pdMS_TO_TICKS(5), pdTRUE, NULL, UASRT_TX_TimerCallback);		// 软件定时器
	xTimerStart(USART_TX_Timer, 0);
	xReturn = xTaskCreate((TaskFunction_t )USART_TX_Task,
											  (const char*)"USART_TX_Task", 
												(uint16_t)128,
												(void *) NULL,
												(UBaseType_t)1,
												(TaskHandle_t*)&USART_TX_Task_Handle );
	if(xReturn != pdPASS)
		while(1);
	
	/* 创建Chairp任务 */
	xReturn = xTaskCreate((TaskFunction_t )Chairp_Task,
											  (const char*)"Chairp_Task", 
												(uint16_t)128,
												(void *) NULL,
												(UBaseType_t)1,
												(TaskHandle_t*)&Chairp_Task_Handle );
	if(xReturn != pdPASS)
		while(1);
	
	/* 创建DELAY任务 */
	DELAY_UPDATE_Timer = xTimerCreate("DELAY_UPDATE_Timer", pdMS_TO_TICKS(1), pdTRUE, NULL, DELAY_UPDATE_TimerCallback);
	xTimerStart(DELAY_UPDATE_Timer, 0);
	xReturn = xTaskCreate((TaskFunction_t )DELAY_Task,
											  (const char*)"DELAY_Task", 
												(uint16_t)128,
												(void *) NULL,
												(UBaseType_t)1,
												(TaskHandle_t*)&DELAY_Task_Handle );
	if(xReturn != pdPASS)
		while(1);
	
	/* 创建USART_RX任务 */
	USART_RX_Queue = xQueueCreate(5,16);		// 串口接收消息队列
	xReturn = xTaskCreate((TaskFunction_t )USART_RX_Task,
											  (const char*)"USART_RX_Task", 
												(uint16_t)128,
												(void *) NULL,
												(UBaseType_t)1,
												(TaskHandle_t*)&USART_RX_Task_Handle );
	if(xReturn != pdPASS)
		while(1);
	
	/* 创建FFT_ANALYSE任务 */
	FFT_ANALYSE_Timer = xTimerCreate("FFT_ANALYSE_Timer", pdMS_TO_TICKS(SAMPLE_TIME_MS), pdTRUE, NULL, FFT_ANALYSE_TimerCallback);
	xTimerStart(FFT_ANALYSE_Timer, 0);
	xReturn = xTaskCreate((TaskFunction_t )FFT_ANALYSE_Task,
											  (const char*)"FFT_ANALYSE_Task", 
												(uint16_t)(1024*6),
												(void *) NULL,
												(UBaseType_t)1,
												(TaskHandle_t*)&FFT_ANALYSE_Task_Handle );
												
	/* SPI发送任务 */
	xReturn = xTaskCreate((TaskFunction_t )SPI_Task,
											  (const char*)"SPI_Task_Task", 
												(uint16_t)(128),
												(void *) NULL,
												(UBaseType_t)2,
												(TaskHandle_t*)&SPI_Task_Handle);
												
	if(xReturn != pdPASS)
		while(1);
	
	
  vTaskDelete(AppTaskCreate_Handle); //删除AppTaskCreate任务，该任务只被执行一次
  taskEXIT_CRITICAL();
}

/* LED任务函数 */
static void LED_Task(void *parameter)
{
	while(1)
	{
		LED_OverTurn(4);
		vTaskDelay(500);
	}
}

/* 按键任务函数 */
static void KEY_Task(void *parameter)
{
	while(1)
	{
		EventBits_t uxBits;
		uxBits = xEventGroupWaitBits(KEY_EventGroup, 
																	KEY11_Event|KEY12_Event|KEY13_Event|KEY14_Event|KEY21_Event|KEY22_Event|KEY23_Event|KEY24_Event,
                                  pdTRUE, 
                                  pdFALSE, 
                                  portMAX_DELAY);
		
		if (xSemaphoreTake(OLED_BinarySemaphore, portMAX_DELAY) == pdTRUE)		// 需要获取信号量，才能进行oled显示，避免多个程序同时对oled操作
		{
			if(uxBits&KEY11_Event)
			{
				Key11_Function();
				xEventGroupSetBits(OLED_EventGroup,OLED_EVENT_UPDATE|OLED_EVENT_UPDATE_DATA|OLED_EVENT_UPDATE_CURSOR); 
			}
			if(uxBits&KEY12_Event)
			{
				Key12_Function();
				xEventGroupSetBits(OLED_EventGroup,OLED_EVENT_UPDATE|OLED_EVENT_UPDATE_DATA|OLED_EVENT_UPDATE_CURSOR); 
			}
			if(uxBits&KEY13_Event)
			{
				Key13_Function();
				xEventGroupSetBits(OLED_EventGroup,OLED_EVENT_UPDATE_CURSOR); 
			}
			if(uxBits&KEY14_Event)
			{
				Key14_Function();
				xEventGroupSetBits(OLED_EventGroup,OLED_EVENT_UPDATE_DATA|OLED_EVENT_UPDATE_CURSOR); 
			}
			if(uxBits&KEY21_Event)
			{
				Key21_Function();
				xEventGroupSetBits(OLED_EventGroup,OLED_EVENT_UPDATE_DATA|OLED_EVENT_UPDATE_CURSOR); 
			}
			if(uxBits&KEY22_Event)
			{
				Key22_Function();
				xEventGroupSetBits(OLED_EventGroup,OLED_EVENT_UPDATE_DATA|OLED_EVENT_UPDATE_CURSOR); 
			}
			if(uxBits&KEY23_Event)
			{
				Key23_Function();
				xEventGroupSetBits(OLED_EventGroup,OLED_EVENT_UPDATE_DATA|OLED_EVENT_UPDATE_CURSOR); 
			}
			if(uxBits&KEY24_Event)
			{
				Key24_Function();
				xEventGroupSetBits(OLED_EventGroup,OLED_EVENT_UPDATE_DATA|OLED_EVENT_UPDATE_CURSOR); 
			}
		}
	}
}

/* OLED刷新任务 */
static void OLED_Task(void *parameter)
{
	while(1)
	{
		EventBits_t uxBits;
		uxBits = xEventGroupWaitBits(OLED_EventGroup, 
																	OLED_EVENT_UPDATE|OLED_EVENT_UPDATE_DATA|OLED_EVENT_UPDATE_CURSOR,
                                  pdTRUE, 
                                  pdFALSE, 
                                  portMAX_DELAY);
		if(uxBits&OLED_EVENT_UPDATE)
			OLED_Update();
		if(uxBits&OLED_EVENT_UPDATE_DATA)
			OLED_Update_Data();
		if(uxBits&OLED_EVENT_UPDATE_CURSOR)
			OLED_Update_Cursor();
		
		 xSemaphoreGive(OLED_BinarySemaphore);		// 刷新完毕后释放信号量
	}
}

/* ENCODER数据刷新任务 */
static void ENCODER_Task(void *parameter)
{
	while(1)
	{
		Get_Encoder_Value();
		Get_Driver_Encoder_Value(&Driver1);
		Get_Driver_Encoder_Value(&Driver2);
		vTaskDelay(20);
		
		if((Top_Device==1&&Driver1.Status==5) || (Top_Device==2&&Driver2.Status==5) || (Top_Device==3) || (Top_Device==4))
		{
			if (xSemaphoreTake(OLED_BinarySemaphore, portMAX_DELAY) == pdTRUE)		// 获取信号量才能刷新oled
			{
				xEventGroupSetBits(OLED_EventGroup,OLED_EVENT_UPDATE_DATA); 
			}
		}
	}
}

/* USART发送任务 */
void USART_TX_Task(void *parameter)
{
	uint32_t ulNotificationValue = 0;
	while(1)
	{
		ulNotificationValue = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		switch(ulNotificationValue)
		{
			case 3:
			{
				Get_Encoder_Value();
				printf("%.3f,0,0,0,0,0,0\n",Encoder1.Distance/1000.0);
				break;
			}
			case 4:
			{
				Get_Encoder_Value();
				printf("%.3f,0,0,0,0,0,0\n",Encoder2.Distance/1000.0);
				break;
			}
			default:
			{
			}
		}
		xSemaphoreGive(USART_TX_BinarySemaphore);
	}
}
void UASRT_TX_TimerCallback(void *parameter)
{
	if (xSemaphoreTake(USART_TX_BinarySemaphore, portMAX_DELAY) == pdTRUE)
	{
		xTaskNotify(USART_TX_Task_Handle,Option.Usart_DataTx_Option,eSetValueWithOverwrite );		// 通知
	}
}

/* 更新Chirp任务 */
void Chairp_Task(void *parameter)
{
	uint32_t ulNotificationValue = 0;
	while(1)
	{
		ulNotificationValue = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		if(ulNotificationValue&0x01)	// 更新Driver1 的扫频数组
			Update_Chairp_data(&Driver1);
		if(ulNotificationValue&0x02)
			Update_Chairp_data(&Driver2);
	}
}

/* 更新DELAY任务 */
void DELAY_Task(void *parameter)
{
	while(1)
	{
		 ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

		switch(Option.DRC1_DataSource_Option)
		{
			case 3:
			{
				Get_Encoder_Value();
				Distance_History_Update(&Delay_Feedback_Controller1, Encoder1.Distance);
				break;
			}
			case 4:
			{
				Get_Encoder_Value();
				Distance_History_Update(&Delay_Feedback_Controller1, Encoder2.Distance);
				break;
			}
			default:
				Distance_History_Update(&Delay_Feedback_Controller1, 0);
		}
		
		switch(Option.DRC2_DataSource_Option)
		{
			case 3:
			{
				Get_Encoder_Value();
				Distance_History_Update(&Delay_Feedback_Controller2, Encoder1.Distance);
				break;
			}
			case 4:
			{
				Get_Encoder_Value();
				Distance_History_Update(&Delay_Feedback_Controller2, Encoder2.Distance);
				break;
			}
			default:
				Distance_History_Update(&Delay_Feedback_Controller1, 0);
		}
	}
}
	
void DELAY_UPDATE_TimerCallback(void *parameter)
{
	xTaskNotifyGive(DELAY_Task_Handle);
}

/* 串口接收任务 */
// M01E		M表示模式切换 E为结尾
void USART_RX_Task(void *parameter)
{
	BaseType_t xReturn = pdTRUE;
	uint8_t data[15];
	while(1)
	{
		xReturn = xQueueReceive(USART_RX_Queue, data, portMAX_DELAY);
		int length = data[0];
		int index=1;
		while(length>=3)
		{
			switch(data[index])
			{
				case 'D':		// 切换顶层设备
				{
					if('0'<=data[index+1] && data[index+1]<='9' && '0'<=data[index+2] && data[index+2]<='9') 
					{
						int device_num = (data[index+1]-'0')*10+(data[index+2]-'0');
						if(device_num>=0 && device_num<=16 && device_num!=7 && device_num!=8)
							Top_Device = device_num;
					}
					break;
				}
				case 'M':		// 切换模式
				{
					break;
				}
				default:
					break;
			}
			
			
			length-=3;
			index+=3;
		}
		
		if (xSemaphoreTake(OLED_BinarySemaphore, portMAX_DELAY) == pdTRUE)		// 获取信号量才能刷新oled
		{
			xEventGroupSetBits(OLED_EventGroup,OLED_EVENT_UPDATE|OLED_EVENT_UPDATE_DATA|OLED_EVENT_UPDATE_CURSOR); 
		}
	}
}

/* FFT分析任务 */
void FFT_ANALYSE_Task(void *parameter)
{
	int fft_flag = 0;
	while(1)
	{
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		
		Get_Encoder_Value();
		float32_t new_data = (float32_t)Encoder2.Distance/1000;
		FFT_Data_Update(new_data);
		
		fft_flag++;
		if(fft_flag==20)
		{
			Calculate_fft();
			fft_flag=0;
		}
	}
}

void FFT_ANALYSE_TimerCallback(void *parameter)
{
	xTaskNotifyGive(FFT_ANALYSE_Task_Handle);
}

/* SPI发送任务 */


volatile int16_t spi_data = 0;
void SPI_Task(void *parameter)
{
	while(1)
	{
		vTaskDelay(1000);
	}
}




