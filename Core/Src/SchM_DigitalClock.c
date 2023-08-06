/*
 * SchM_DigitalClock.c
 *
 *  Created on: Jul 2, 2023
 *      Author: Jesus
 */

#include "SchM_DigitalClock.h"

#include "rtc.h"
#include "usart.h"
#include "gpio.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define SIZE_RX    8u
#define Debug_ITM  0
#define Max_TxUART 0x15u
#define Max_RxUART 8u

extern RTC_HandleTypeDef hrtc;
extern UART_HandleTypeDef huart1;

typedef struct{

	uint8_t hours;
	uint8_t minutes;

}time_storage;

typedef struct Local_RTC_t{

	RTC_TimeTypeDef Local_RTC_Time;
	RTC_DateTypeDef Local_RTC_Date;
}Local_RTC_T;

typedef struct
{
	uint16_t Message_Length;
	uint8_t  Message_Data[Max_TxUART];
}TxUART;

typedef struct
{
	uint16_t Message_Length;
	uint8_t  Message_Data[Max_RxUART];
}RxUART;

typedef enum
{
	time,
	date,

	max_buffer_tx
}data_type_T;

typedef enum
{
	settings,
	Time,
	Date,
	home,
	cfg,

	max_nextion_str
}str_nextion_T;


uint8_t fixed_end_nextion [] = {0xFF, 0xFF, 0xFF};
uint8_t time_format_am[] = "am";
uint8_t time_format_pm[] = "pm";
uint8_t time_format[2] = {0};

const char * Str_Nextion[max_nextion_str] = {"Settings", "time", "date", "home", "Cfg"};


TaskHandle_t Get_Time_Handler = NULL;
TaskHandle_t Print_Time_Handler = NULL;
TaskHandle_t Process_Rx_Data_Handler = NULL;
TaskHandle_t Set_Time_Handler = NULL;

QueueHandle_t Time_Queue_Handler = NULL;
QueueHandle_t User_Input_Time_Date_Queue_Handler = NULL;


static uint8_t Format_to_12(RTC_TimeTypeDef const *RTC_Struct_Info);
//static RTC_TimeTypeDef Format_to_24(uint16_t Rx_buffer, uint8_t meridiem);
void wrapper_tx_data(Local_RTC_T const *data, TxUART * buffer_data, data_type_T conversion_type);


/*
 * Support functions
 * */
static uint8_t Format_to_12(RTC_TimeTypeDef const *RTC_Struct_Info)
{
	uint8_t hours_local_var = RTC_Struct_Info->Hours;
	strcpy((char*)time_format,(char*)time_format_am);
	if(RTC_Struct_Info->Hours == 0x00u)
	{
		hours_local_var = 12;
	}
	else if(RTC_Struct_Info->Hours > 12u)
	{
		hours_local_var = RTC_Struct_Info->Hours - 12u;
		strcpy((char*)time_format,(char*)time_format_pm);
	}
	else if(RTC_Struct_Info->Hours == 12u)
	{
		strcpy((char*)time_format,(char*)time_format_am);
	}
	return hours_local_var;
}
//static RTC_TimeTypeDef Format_to_24(uint16_t Rx_buffer, uint8_t meridiem)
//{
//	RTC_TimeTypeDef local_RTC;
//	local_RTC.Minutes = Rx_buffer % 1000;
//	local_RTC.Hours   = Rx_buffer / 1000;
//
//	if(meridiem == 'p')
//	{
//		local_RTC.Hours += 12U;
//	}
//
//	return local_RTC;
//}

void wrapper_tx_data(Local_RTC_T const *data, TxUART * buffer_data, data_type_T conversion_type)
{
	if(time == conversion_type)
	{
		buffer_data->Message_Length = sprintf((char*)buffer_data->Message_Data, "t0.txt=\"%02d:%02d:%02d %s\"",data->Local_RTC_Time.Hours, data->Local_RTC_Time.Minutes, data->Local_RTC_Time.Seconds, time_format);
	}
	else
	{
		buffer_data->Message_Length = sprintf((char*)buffer_data->Message_Data, "t1.txt=\"%d/%d/%d\"",data->Local_RTC_Date.Date, data->Local_RTC_Date.Month, data->Local_RTC_Date.Year);
	}
}

void Task_Generation(void)
{
	Time_Queue_Handler = xQueueCreate(2,sizeof(TxUART));
	User_Input_Time_Date_Queue_Handler = xQueueCreate(1,sizeof(TxUART));

	xTaskCreate(Get_RTC_Runnable, "Get Time ", 100, NULL, 2, &Get_Time_Handler);
	xTaskCreate(Print_Time_Runnable, "Print Time", 100, NULL, 2, &Print_Time_Handler);
	xTaskCreate(Process_UART_Data_Runnable, "UART Rx", 100, NULL, 2, &Process_Rx_Data_Handler);
}

void Get_RTC_Runnable(void * parameters)
{
	Local_RTC_T RTC_container;
	TxUART tx_buffer[max_buffer_tx];
	BaseType_t Queue_status = pdFALSE;
	for(;;)
	{
#if Debug_ITM
		printf("Getting Time Alive\n");
#endif
		printf("Getting Time Alive\n");
		HAL_RTC_GetTime(&hrtc, &RTC_container.Local_RTC_Time, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &RTC_container.Local_RTC_Date, RTC_FORMAT_BIN);
		//Updating Hours
		RTC_container.Local_RTC_Time.Hours = Format_to_12(&RTC_container.Local_RTC_Time);
		wrapper_tx_data((Local_RTC_T const *)&RTC_container, &tx_buffer[time], time);
		//tx_buffer[date].Message_Length = sprintf((char*)tx_buffer[date].Message_Data, "t1.txt=\"%d/%d/%d\"",RTC_container.Local_RTC_Date.WeekDay, RTC_container.Local_RTC_Date.Month, RTC_container.Local_RTC_Date.Year);
		wrapper_tx_data((Local_RTC_T const *)&RTC_container, &tx_buffer[date], date);

#if Debug_ITM
		printf("%s\n", tx_buffer[time].Message_Data);
#endif

		Queue_status = xQueueSend(Time_Queue_Handler, &tx_buffer[time], ( TickType_t )0);
		Queue_status = xQueueSend(Time_Queue_Handler, &tx_buffer[date], ( TickType_t )0);
		taskYIELD();
	}
}


/**
  * @brief  This task prints out data time in Nextion screen.
  *
  * @note   This task is executed in cooperation mode. This task uses a Queue in order to get data.
  *         Also This task calls the UART-Tx to send info.
  *
  * @param  None
  * @retval None
  */

void Print_Time_Runnable(void * parameters)
{
	TxUART tx_buffer[max_buffer_tx];
	for(;;)
	{
		printf("Print Time Alive\n");
		xQueueReceive(Time_Queue_Handler, &tx_buffer[time], ( TickType_t )0);
		xQueueReceive(Time_Queue_Handler, &tx_buffer[date], ( TickType_t )0);
#if 0
		HAL_UART_Transmit_IT(&huart1, &Buffer_time, 16);
		HAL_UART_Transmit_IT(&huart1, &fixed_end_nextion, 3);

#endif
		HAL_UART_Transmit(&huart1, tx_buffer[time].Message_Data, tx_buffer[time].Message_Length, 800);
		HAL_UART_Transmit(&huart1, fixed_end_nextion, 3, 300);
		HAL_UART_Transmit(&huart1, tx_buffer[date].Message_Data, tx_buffer[date].Message_Length, 800);
		HAL_UART_Transmit(&huart1, fixed_end_nextion, 3, 300);

		taskYIELD();
	}
}

void Process_UART_Data_Runnable(void * parameters)
{
	RxUART rx_buffer = {0};
	char * found_str = NULL;
	static uint8_t f_status = 0x00;
	//BaseType_t L_Status_UART_Rx;
	for(;;)
	{
		uint8_t str_iter = 0;
		str_nextion_T menu = max_nextion_str;
		printf("UART Rx alive\n");
		HAL_UART_Receive(&huart1, rx_buffer.Message_Data, SIZE_RX, 900);

		for(str_iter = settings; str_iter < max_nextion_str; str_iter++)
		{
			found_str = strstr((char *)rx_buffer.Message_Data, Str_Nextion[str_iter]);
			if(found_str)
			{
				menu = str_iter;
				break;
			}
		}

		switch(menu)
		{
		case settings:
			vTaskSuspend(Print_Time_Handler);
			printf("Suspending Print_Time\n");
			break;
		case home:
			vTaskResume(Print_Time_Handler);
			break;
		case Time:
			f_status = 0x01;
			break;
		case Date:
			f_status = 0x01;
			break;
		case cfg:
			f_status = 0x00;
			break;
		default:
			break;
		}

		while(f_status)
		{
			HAL_UART_Receive(&huart1, rx_buffer.Message_Data, SIZE_RX, 900);
		}

		memset(rx_buffer.Message_Data, 0x00, sizeof(rx_buffer.Message_Data)); //Cleaning buffer
		taskYIELD();
	}
}

