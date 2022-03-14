/*
 * Task_MGR.C
 *
 *  Created on: Jan 26, 2022
 *      Author: Jesus Rodriguez
 */

#if 0

#include "Task_MGR.h"



void Process_UART_Data_Runnable(void * parameters)
{
	uint8_t UART1_rxBuffer_Runnable[SIZE_RX] = {0};
	char * found_str = NULL;

	//BaseType_t L_Status_UART_Rx;
	for(;;)
	{
#if Debug_ITM
		printf("UART Rx alive\n");
#endif
		HAL_UART_Receive(&huart1, UART1_rxBuffer_Runnable, SIZE_RX, 1000);
		found_str = strstr((char *)UART1_rxBuffer_Runnable, (char *)String_Search_Nextion);
		if(found_str)
		{
#if Debug_ITM
			printf("Setting found\n");
#endif
			vTaskSuspend(Print_Time_Handler);
			printf("Suspending Print_Time\n");
			memset(UART1_rxBuffer_Runnable, 0x00, sizeof(UART1_rxBuffer_Runnable));
		}
		else if(strstr((char *)UART1_rxBuffer_Runnable, "m"))
		{
			RTC_TimeTypeDef local_time = {0x00U};
#if Debug_ITM
			printf("Current time to be set is %s\n", UART1_rxBuffer_Runnable);
#endif
			local_time = Format_to_24(atoi((const char *)UART1_rxBuffer_Runnable));
#if Debug_ITM
			printf("Time that is going to be set: %d:%d:%d\n", local_time.Hours, local_time.Minutes, local_time.Seconds);
#endif

			if(UART1_rxBuffer_Runnable[6] == 'a')
			{
				HAL_RTC_SetTime(&hrtc,&local_time,RTC_FORMAT_BIN);
			}
			else if(UART1_rxBuffer_Runnable[6] == 'p')
			{
				local_time.Hours += 12U;
				HAL_RTC_SetTime(&hrtc,&local_time,RTC_FORMAT_BIN);
			}

			vTaskResume(Print_Time_Handler);
#if Debug_ITM
			printf("Resuming Get_Time\n");
#endif
			memset(UART1_rxBuffer_Runnable, 0x00, sizeof(UART1_rxBuffer_Runnable));
		}
		taskYIELD();
	}
}

void Get_Time_RTC_Runnable(void * parameters)
{
	RTC_TimeTypeDef Local_RTC_Data;
	TxUART tx_buffer[max_buffer];
	for(;;)
	{
#if Debug_ITM
		printf("Getting Time Alive\n");
#endif

		HAL_RTC_GetTime(&hrtc, &Local_RTC_Data, RTC_FORMAT_BIN);
		//Updating Hours
		Local_RTC_Data.Hours = Format_to_12(&Local_RTC_Data);
		wrapper_tx_data(&Local_RTC_Data, &tx_buffer[time], time);

#if Debug_ITM
		printf("%s\n", tx_buffer[time].Message_Data);
#endif

		xQueueSend(Time_Queue_Handler, &tx_buffer[time], 0);
		taskYIELD();
	}
}

void Print_Time_Runnable(void * parameters)
{
	TxUART Time;
	for(;;)
	{
		xQueueReceive(Time_Queue_Handler, &Time, portMAX_DELAY);
#if 0
		HAL_UART_Transmit_IT(&huart1, &Buffer_time, 16);
		HAL_UART_Transmit_IT(&huart1, &fixed_end_nextion, 3);

#endif
		HAL_UART_Transmit(&huart1, Time.Message_Data, Time.Message_Length, 1000);
		HAL_UART_Transmit(&huart1, fixed_end_nextion, 3, 1000);
		printf("Nextion Time: %s\n", Time.Message_Data);

		taskYIELD();
	}
}
#endif
