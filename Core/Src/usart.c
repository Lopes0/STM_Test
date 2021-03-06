/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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

/* Includes ------------------------------------------------------------------*/
#include "usart.h"
#include <stdio.h>

/* USER CODE BEGIN 0 */

//UART3 RX data structures:
uint8_t UART3Rx_Buffer[128];
uint8_t UART3Tx_Buffer[128];
uint8_t Rx_Buffer[128];
uint8_t Tx_Buffer[128];
int receive_flag = 0;
int transmit_flag = 0;
volatile uint8_t UART3Rx_index = 0;
volatile uint8_t UART3Tx_index = 0;
volatile uint8_t Rx_index = 0;
volatile uint8_t Tx_index = 0;

/* USER CODE END 0 */

UART_HandleTypeDef huart3;

/* USART3 init function */

void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspInit 0 */

  /* USER CODE END USART3_MspInit 0 */
    /* USART3 clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();

    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**USART3 GPIO Configuration
    PD8     ------> USART3_TX
    PD9     ------> USART3_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* USART3 interrupt Init */
    HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspInit 1 */

  /* USER CODE END USART3_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspDeInit 0 */

  /* USER CODE END USART3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();

    /**USART3 GPIO Configuration
    PD8     ------> USART3_TX
    PD9     ------> USART3_RX
    */
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_8|GPIO_PIN_9);

    /* USART3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspDeInit 1 */

  /* USER CODE END USART3_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
//ter atenção à itnrodução a este intervalo
//FIFO, com apontador [head & tail]
void init_UART3Receive(void) {
	//set the interrupt for UART3 Rx
	HAL_UART_Receive_IT(&huart3, &UART3Rx_Buffer [UART3Rx_index],1);
}

void init_UART3Transmit(void) {
	//set the interrupt for UART3 Rx
	HAL_UART_Transmit_IT(&huart3, &UART3Rx_Buffer [UART3Rx_index],1);
}

//implementation of UART ISR
void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart) {
	if (huart->Instance == USART3) {
		UART3Rx_index++;
		UART3Rx_index &= ~(1<<7); //keep index inside the limits
		//set the interrupt for UART3 Rx again
		HAL_UART_Receive_IT(&huart3, &UART3Rx_Buffer[UART3Rx_index],1);
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart) {
	if (huart->Instance == USART3) {
		UART3Tx_index++;
		UART3Tx_index &= ~(1<<7); //keep index inside the limits
		//set the interrupt for UART3 Rx again
		HAL_UART_Transmit_IT(&huart3, &UART3Tx_Buffer[UART3Tx_index],1);
	}
}

void newMessage() {
	static int local_index = 0;
	int out_index = 0;
	while (local_index != UART3Rx_index) {
		Rx_Buffer[out_index] = UART3Rx_Buffer[local_index];
		out_index++;
		local_index++;
		local_index &= ~(1<<7);
		receive_flag = 1;
	}
	Rx_Buffer[out_index] = '\0';
}

void transmitMessage() {
	static int local_index = 0;
	int out_index = 0;
	while (local_index != Tx_index) {
		UART3Tx_Buffer[out_index] = Tx_Buffer[local_index];
		out_index++;
		local_index++;
		local_index &= ~(1<<7);
		//receive_flag = 1;
	}
	//Rx_Buffer[out_index] = '\0';
}

//redefining stdout
int fputc(int ch, FILE *f) {
	HAL_UART_Transmit(&huart3, (uint8_t*)&ch, 1, 100); //aqui ñ é usada a interrupção para Tx
	return ch;
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
