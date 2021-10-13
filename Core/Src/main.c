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
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SIZE_RX 18
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

extern RTC_HandleTypeDef hrtc;
extern UART_HandleTypeDef huart1;

TaskHandle_t Get_Time_Handler = NULL;
TaskHandle_t Print_Time_Handler = NULL;
TaskHandle_t Process_Rx_Data_Handler = NULL;

QueueHandle_t Time_Queue_Handler = NULL;
QueueHandle_t Time_UART_Rx_Queue_Handler = NULL;

BaseType_t Status;
char * found = NULL;

uint8_t fixed_end_nextion [] = {0xFF, 0xFF, 0xFF};
uint8_t String_Search_Nextion[] = "Settings";

static uint8_t Buffer_time[25] = {0};
volatile uint8_t UART1_rxBuffer[SIZE_RX] = {0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

static void Get_Time_RTC_Runnable(void * parameters);
static void Print_Time_Runnable(void * parameters);
static void Process_UART_Data_unnable(void * parameters);

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

  Time_Queue_Handler = xQueueCreate(1,sizeof(RTC_TimeTypeDef));
  Time_UART_Rx_Queue_Handler = xQueueCreate(1,sizeof(UART1_rxBuffer));

  Status = xTaskCreate(Get_Time_RTC_Runnable, "Get Time ", 100, NULL, 2, &Get_Time_Handler);
  Status = xTaskCreate(Print_Time_Runnable, "Print Time", 100, NULL, 2, &Print_Time_Handler);
  Status = xTaskCreate(Process_UART_Data_unnable, "UART Rx", 100, NULL, 3, &Process_Rx_Data_Handler);


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

static void Process_UART_Data_unnable(void * parameters)
{
	uint8_t UART1_rxBuffer_Runnable[SIZE_RX] = {0};
	BaseType_t L_Status_UART_Rx;
	for(;;)
	{
		HAL_UART_Receive_IT (&huart1, (uint8_t *)UART1_rxBuffer, SIZE_RX);
		printf("Process UART TX Rx Alive\n");
		L_Status_UART_Rx = xQueueReceive(Time_UART_Rx_Queue_Handler, &UART1_rxBuffer_Runnable, portMAX_DELAY);

		if(L_Status_UART_Rx == pdTRUE)
		{
			found = strstr((char *)UART1_rxBuffer,(char *)String_Search_Nextion);

			if(found)
			{
				memset(UART1_rxBuffer, 0, sizeof(UART1_rxBuffer));
				printf("Settings config activated\n");
				vTaskSuspend(Get_Time_Handler);
				printf("Suspending Get time task\n");
				vTaskSuspend(Print_Time_Handler);
				printf("Suspending Print time task\n");
				taskYIELD();


			}
			else
			{
				memset(UART1_rxBuffer, 0, sizeof(UART1_rxBuffer));
				printf("Settings config No-activated\n");
				vTaskResume(Get_Time_Handler);
				printf("Alive Get time task\n");
				vTaskResume(Print_Time_Handler);
				printf("Alive Print time task\n");
				taskYIELD();
			}
		}
		else
		{
			taskYIELD();
		}
	}
}

static void Get_Time_RTC_Runnable(void * parameters)
{
	RTC_TimeTypeDef Local_RTC_Data;
	for(;;)
	{
		printf("Getting Time Alive\n");
		HAL_RTC_GetTime(&hrtc, &Local_RTC_Data, RTC_FORMAT_BIN);
		xQueueSend(Time_Queue_Handler, &Local_RTC_Data, 0);
		taskYIELD();
	}
}

static void Print_Time_Runnable(void * parameters)
{
	RTC_TimeTypeDef L_RTC_Data;
	for(;;)
	{
		xQueueReceive(Time_Queue_Handler, &L_RTC_Data, portMAX_DELAY);
		uint16_t leng_message_uart = sprintf((char *)Buffer_time, "t0.txt=\"%02d:%02d:%02d\"", L_RTC_Data.Hours, L_RTC_Data.Minutes, L_RTC_Data.Seconds);
#if 0
		HAL_UART_Transmit_IT(&huart1, &Buffer_time, 16);
		HAL_UART_Transmit_IT(&huart1, &fixed_end_nextion, 3);
#endif
		HAL_UART_Transmit(&huart1, Buffer_time, leng_message_uart, 1000);
		HAL_UART_Transmit(&huart1, fixed_end_nextion, 3, 1000);
		printf("%d:%d:%d\n", L_RTC_Data.Hours, L_RTC_Data.Minutes, L_RTC_Data.Seconds);
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
	memset(UART1_rxBuffer, 0, sizeof(UART1_rxBuffer));
	xQueueSendFromISR(Time_UART_Rx_Queue_Handler,&UART1_rxBuffer, NULL);
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
