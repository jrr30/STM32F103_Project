/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "rtc.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define SIZE_RX    8u
#define Debug_ITM  0
#define Max_TxUART 0x15u
#define Max_RxUART 8u
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

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


TaskHandle_t Get_Time_Handler = NULL;
TaskHandle_t Print_Time_Handler = NULL;
TaskHandle_t Process_Rx_Data_Handler = NULL;
TaskHandle_t Set_Time_Handler = NULL;


QueueHandle_t Time_Queue_Handler = NULL;
QueueHandle_t User_Input_Time_Date_Queue_Handler = NULL;

BaseType_t Status;
char * found = NULL;

uint8_t fixed_end_nextion [] = {0xFF, 0xFF, 0xFF};
uint8_t time_format_am[] = "am";
uint8_t time_format_pm[] = "pm";
uint8_t time_format[2] = {0};

const char * Str_Nextion[max_nextion_str] = {"Settings", "time", "date", "home", "Cfg"};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

static void Get_RTC_Runnable(void * parameters);
static void Print_Time_Runnable(void * parameters);
static void Process_UART_Data_Runnable(void * parameters);

static uint8_t Format_to_12(RTC_TimeTypeDef const *RTC_Struct_Info);
static RTC_TimeTypeDef Format_to_24(uint16_t Rx_buffer, uint8_t meridiem);
void wrapper_tx_data(Local_RTC_T const *data, TxUART * buffer_data, data_type_T conversion_type);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_RTC_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  Time_Queue_Handler = xQueueCreate(2,sizeof(TxUART));
  User_Input_Time_Date_Queue_Handler = xQueueCreate(1,sizeof(TxUART));

  Status = xTaskCreate(Get_RTC_Runnable, "Get Time ", 100, NULL, 2, &Get_Time_Handler);
  Status = xTaskCreate(Print_Time_Runnable, "Print Time", 100, NULL, 2, &Print_Time_Handler);
  Status = xTaskCreate(Process_UART_Data_Runnable, "UART Rx", 100, NULL, 2, &Process_Rx_Data_Handler);


  vTaskStartScheduler();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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
static RTC_TimeTypeDef Format_to_24(uint16_t Rx_buffer, uint8_t meridiem)
{
	RTC_TimeTypeDef local_RTC;
	local_RTC.Minutes = Rx_buffer % 1000;
	local_RTC.Hours   = Rx_buffer / 1000;

	if(meridiem == 'p')
	{
		local_RTC.Hours += 12U;
	}

	return local_RTC;
}

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
static void Process_UART_Data_Runnable(void * parameters)
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

/**
  * @brief  This task reads registers from RTC for time.
  *
  * @note   This task is executed in cooperation mode. This task uses a Queue in order to transfer data.
  *
  * @param  None
  * @retval None
  */


static void Get_RTC_Runnable(void * parameters)
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

static void Print_Time_Runnable(void * parameters)
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


int _write(int file, char *ptr, int len)
{
	int DataIdx;

	for (DataIdx = 0; DataIdx < len; DataIdx++)
	{
		ITM_SendChar(*ptr++);
	}
	return len;
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
   __NOP();// do nothing here
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	__NOP();// do nothing here
}
/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM4 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM4) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
