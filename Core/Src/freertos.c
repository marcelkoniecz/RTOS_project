/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bma220.h"
#include "crc8.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define FLAG_NEWDATA 0x00000001U

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for Read_raw_data */
osThreadId_t Read_raw_dataHandle;
const osThreadAttr_t Read_raw_data_attributes = {
  .name = "Read_raw_data",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal5,
};
/* Definitions for Calculate_Value */
osThreadId_t Calculate_ValueHandle;
const osThreadAttr_t Calculate_Value_attributes = {
  .name = "Calculate_Value",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal4,
};
/* Definitions for Send_data_UART */
osThreadId_t Send_data_UARTHandle;
const osThreadAttr_t Send_data_UART_attributes = {
  .name = "Send_data_UART",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal5,
};
/* Definitions for rec_conf_UART */
osThreadId_t rec_conf_UARTHandle;
const osThreadAttr_t rec_conf_UART_attributes = {
  .name = "rec_conf_UART",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for Meas_Timer */
osTimerId_t Meas_TimerHandle;
const osTimerAttr_t Meas_Timer_attributes = {
  .name = "Meas_Timer"
};
/* Definitions for SendDataTImer */
osTimerId_t SendDataTImerHandle;
const osTimerAttr_t SendDataTImer_attributes = {
  .name = "SendDataTImer"
};
/* Definitions for calculateData_mutex */
osMutexId_t calculateData_mutexHandle;
const osMutexAttr_t calculateData_mutex_attributes = {
  .name = "calculateData_mutex"
};
/* Definitions for changingUARTParam_mutex */
osMutexId_t changingUARTParam_mutexHandle;
const osMutexAttr_t changingUARTParam_mutex_attributes = {
  .name = "changingUARTParam_mutex"
};
/* Definitions for getData_mutex */
osMutexId_t getData_mutexHandle;
const osMutexAttr_t getData_mutex_attributes = {
  .name = "getData_mutex"
};
/* Definitions for newDataRead */
osEventFlagsId_t newDataReadHandle;
const osEventFlagsAttr_t newDataRead_attributes = {
  .name = "newDataRead"
};
/* Definitions for UARTConfigChange */
osEventFlagsId_t UARTConfigChangeHandle;
const osEventFlagsAttr_t UARTConfigChange_attributes = {
  .name = "UARTConfigChange"
};
/* Definitions for DataCalculated */
osEventFlagsId_t DataCalculatedHandle;
const osEventFlagsAttr_t DataCalculated_attributes = {
  .name = "DataCalculated"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartReadI2CData(void *argument);
void CalculateRawData(void *argument);
void SendDataUART(void *argument);
void recConfUART(void *argument);
void Callback01(void *argument);
void Callback02(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* creation of calculateData_mutex */
  calculateData_mutexHandle = osMutexNew(&calculateData_mutex_attributes);

  /* creation of changingUARTParam_mutex */
  changingUARTParam_mutexHandle = osMutexNew(&changingUARTParam_mutex_attributes);

  /* creation of getData_mutex */
  getData_mutexHandle = osMutexNew(&getData_mutex_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* creation of Meas_Timer */
  Meas_TimerHandle = osTimerNew(Callback01, osTimerPeriodic, NULL, &Meas_Timer_attributes);

  /* creation of SendDataTImer */
  SendDataTImerHandle = osTimerNew(Callback02, osTimerPeriodic, NULL, &SendDataTImer_attributes);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of Read_raw_data */
  Read_raw_dataHandle = osThreadNew(StartReadI2CData, NULL, &Read_raw_data_attributes);

  /* creation of Calculate_Value */
  Calculate_ValueHandle = osThreadNew(CalculateRawData, NULL, &Calculate_Value_attributes);

  /* creation of Send_data_UART */
  Send_data_UARTHandle = osThreadNew(SendDataUART, NULL, &Send_data_UART_attributes);

  /* creation of rec_conf_UART */
  rec_conf_UARTHandle = osThreadNew(recConfUART, NULL, &rec_conf_UART_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* creation of newDataRead */
  newDataReadHandle = osEventFlagsNew(&newDataRead_attributes);

  /* creation of UARTConfigChange */
  UARTConfigChangeHandle = osEventFlagsNew(&UARTConfigChange_attributes);

  /* creation of DataCalculated */
  DataCalculatedHandle = osEventFlagsNew(&DataCalculated_attributes);

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartReadI2CData */
/**
  * @brief  Function implementing the Read_raw_data thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartReadI2CData */
void StartReadI2CData(void *argument)
{
  /* USER CODE BEGIN StartReadI2CData */
  /* Infinite loop */
  for(;;)
  {
//      HAL_GPIO_TogglePin(LD2_GPIO_Port,LD2_Pin);
      osMutexAcquire(getData_mutexHandle, osWaitForever);
      BMA220_ReadAcc(&acc);
      osMutexRelease(getData_mutexHandle);
      osEventFlagsSet(newDataReadHandle,FLAG_NEWDATA);
    osDelay(100);
  }
  /* USER CODE END StartReadI2CData */
}

/* USER CODE BEGIN Header_CalculateRawData */
/**
* @brief Function implementing the Calculate_Value thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_CalculateRawData */
void CalculateRawData(void *argument)
{
  /* USER CODE BEGIN CalculateRawData */
  /* Infinite loop */
  for(;;){
      osEventFlagsWait(newDataReadHandle,FLAG_NEWDATA,osFlagsWaitAny,osWaitForever);
      osMutexAcquire(getData_mutexHandle,osWaitForever);
      osMutexAcquire(calculateData_mutexHandle,osWaitForever);
      BMA220_CalcAcc(&acc);
      osMutexRelease(calculateData_mutexHandle);
      osMutexRelease(getData_mutexHandle);
    osDelay(10);
  }
  /* USER CODE END CalculateRawData */
}

/* USER CODE BEGIN Header_SendDataUART */
/**
* @brief Function implementing the Send_data_UART thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_SendDataUART */
void SendDataUART(void *argument)
{
  /* USER CODE BEGIN SendDataUART */
  /* Infinite loop */
  char bufToSent[50];
  for(;;)
  {
      osMutexAcquire(calculateData_mutexHandle,osWaitForever);
      BMA220_CreateString(&acc,bufToSent);
      HAL_GPIO_TogglePin(LD2_GPIO_Port,LD2_Pin);
      HAL_UART_Transmit(&huart2, bufToSent,strlen(bufToSent), HAL_MAX_DELAY);
      osMutexRelease(calculateData_mutexHandle);
    osDelay(1000);
  }
  /* USER CODE END SendDataUART */
}

/* USER CODE BEGIN Header_recConfUART */
/**
* @brief Function implementing the rec_conf_UART thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_recConfUART */
void recConfUART(void *argument)
{
  /* USER CODE BEGIN recConfUART */
  /* Infinite loop */
  for(;;)
  {
//      HAL_GPIO_TogglePin(LD2_GPIO_Port,LD2_Pin);
    osDelay(10);
  }
  /* USER CODE END recConfUART */
}

/* Callback01 function */
void Callback01(void *argument)
{
  /* USER CODE BEGIN Callback01 */

  /* USER CODE END Callback01 */
}

/* Callback02 function */
void Callback02(void *argument)
{
  /* USER CODE BEGIN Callback02 */

  /* USER CODE END Callback02 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

