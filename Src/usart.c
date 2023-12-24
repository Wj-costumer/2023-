/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "usart.h"

/* USER CODE BEGIN 0 */
#include "cmsis_os.h"
#include "pid.h"
#include "Chassis_Move.h"
#include "string_operator.h"
#include "string.h"
#include "TOF.h"
#include "arm_control.h"
#include "arm_control.h"

/* USER CODE END 0 */

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart3;
UART_HandleTypeDef huart6;
DMA_HandleTypeDef hdma_usart3_rx;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}
/* USART3 init function */

void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 100000;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_EVEN;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}
/* USART6 init function */

void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 115200;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PB7     ------> USART1_RX
    PA9     ------> USART1_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspInit 0 */

  /* USER CODE END USART3_MspInit 0 */
    /* USART3 clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**USART3 GPIO Configuration
    PC11     ------> USART3_RX
    PC10     ------> USART3_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* USART3 DMA Init */
    /* USART3_RX Init */
    hdma_usart3_rx.Instance = DMA1_Stream1;
    hdma_usart3_rx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart3_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart3_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart3_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart3_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart3_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart3_rx.Init.Mode = DMA_NORMAL;
    hdma_usart3_rx.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_usart3_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart3_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart3_rx);

    /* USART3 interrupt Init */
    HAL_NVIC_SetPriority(USART3_IRQn, 4, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspInit 1 */

  /* USER CODE END USART3_MspInit 1 */
  }
  else if(uartHandle->Instance==USART6)
  {
  /* USER CODE BEGIN USART6_MspInit 0 */

  /* USER CODE END USART6_MspInit 0 */
    /* USART6 clock enable */
    __HAL_RCC_USART6_CLK_ENABLE();

    __HAL_RCC_GPIOG_CLK_ENABLE();
    /**USART6 GPIO Configuration
    PG14     ------> USART6_TX
    PG9     ------> USART6_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_USART6;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    /* USART6 interrupt Init */
    HAL_NVIC_SetPriority(USART6_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART6_IRQn);
  /* USER CODE BEGIN USART6_MspInit 1 */
    HAL_NVIC_SetPriority(USART6_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART6_IRQn);
  /* USER CODE END USART6_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PB7     ------> USART1_RX
    PA9     ------> USART1_TX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_7);

    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspDeInit 0 */

  /* USER CODE END USART3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();

    /**USART3 GPIO Configuration
    PC11     ------> USART3_RX
    PC10     ------> USART3_TX
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_11|GPIO_PIN_10);

    /* USART3 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);

    /* USART3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspDeInit 1 */

  /* USER CODE END USART3_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART6)
  {
  /* USER CODE BEGIN USART6_MspDeInit 0 */

  /* USER CODE END USART6_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART6_CLK_DISABLE();

    /**USART6 GPIO Configuration
    PG14     ------> USART6_TX
    PG9     ------> USART6_RX
    */
    HAL_GPIO_DeInit(GPIOG, GPIO_PIN_14|GPIO_PIN_9);

    /* USART6 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART6_IRQn);
  /* USER CODE BEGIN USART6_MspDeInit 1 */

  /* USER CODE END USART6_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
extern uint8_t RxBuffer_1[];
extern uint8_t RxBuffer_2[];
extern uint8_t Rx_Size;
extern chassis_move_t chassis_move;
//extern int speed_target[4];
//extern int16_t speed[4];
extern pid_type_def motor_pid;
extern pid_type_def angle_turn_pid;
extern pid_type_def angle_straight_pid;
//extern float angles[3];
//extern float angle_target;
//extern float distance_integral;
//extern int16_t motor_ecd[4];

//extern double Intergral[4];
extern int drive_flag;
//extern fp32 INS_angle_next[3];

int find_QRCODE_flag = 0;
int find_COLOR_flag[6] = {0};
int find_all_color_flag[2] = {0};
uint8_t QRCODE[6];
uint8_t COLOR[6];

///*
//	串口和上位机通信
//*/
//void SCI_Send_Datas(UART_HandleTypeDef *huart) 
//{
//  int i,j;
//  static unsigned short int send_data[3][4] = { { 0 }, { 0 }, { 0 } };
//  short int checksum=0;
//  uint8_t xorsum=0,high,low;
//	uint8_t buf1[]={'S'};
//	uint8_t buf2[]={'T'};
//	
//	uint8_t buf3[]={0};
//	uint8_t buf4[]={0};
//	uint8_t buf5[]={0};
//	uint8_t buf6[]={0};

//  //send_data[0][0] = (unsigned short int)((PWM[1]*270-500*270)/2000); 
//  //send_data[0][1] = (unsigned short int)((PWM[2]*270-500*270)/2000);
//  //send_data[0][2] = (unsigned short int)((PWM[3]*270-500*270)/2000);
//	send_data[0][0] = (unsigned short int)(chassis_move.motor[1]->ecd); 
//  send_data[0][1] = (unsigned short int)(chassis_move.motor[1]->ecd);
//  send_data[0][2] = (unsigned short int)(chassis_move.motor[1]->last_ecd);
//  send_data[0][3] = (unsigned short int)(0);		
//	//send_data[0][2] = (unsigned short int)(speed_target[1]);
//	//send_data[0][3] = (unsigned short int)(speed[1]);

//  send_data[1][0] = (unsigned short int)(motor_pid.Kp*100);
//  send_data[1][1] = (unsigned short int)(motor_pid.Ki*100);
//  send_data[1][2] = (unsigned short int)(motor_pid.Kd*100);
//  send_data[1][3] = (unsigned short int)(0);

////  send_data[2][0] = (unsigned short int)(Intergral[1]);
//  send_data[2][1] = (unsigned short int)(0);
//  send_data[2][2] = (unsigned short int)(0);
//  send_data[2][3] = (unsigned short int)(0);
//		
//  //uart_write_byte(uart_num, 'S');
//  //uart_write_byte(uart_num, 'T');
//	HAL_UART_Transmit( huart,buf1,1,1000);
//	HAL_UART_Transmit( huart,buf2,1,1000);
//	
//  for (i = 0; i < 3; i++)
//    for (j = 0; j < 4; j++)
//    {
//      //low=(unsigned char)(send_data[i][j] & 0x00ff);
//      //high=(unsigned char)(send_data[i][j] >> 8u);
//      //uart_write_byte(uart_num,low ); uart_write_byte(uart_num, high);
//			low=(uint8_t )(send_data[i][j] & 0x00ff);
//			high=(uint8_t )(send_data[i][j] >> 8u);
//			*buf3=low;
//			*buf4=high;
//			
//			HAL_UART_Transmit( huart,buf3,1,1000);
//			HAL_UART_Transmit( huart,buf4,1,1000);
//			
//			
//      checksum+=low; checksum+=high;
//      xorsum^=low; xorsum^= high;
//    }
//		//uart_write_byte(uart_num, (unsigned char)(checksum & 0x00ff));
//		//uart_write_byte(uart_num,xorsum);
//		*buf5=checksum & 0x00ff;
//		*buf6=xorsum;
//		HAL_UART_Transmit( huart,buf5,1,1000);
//		HAL_UART_Transmit( huart,buf6,1,1000);		
//}

#define Size 50
#define Rough 5 // 粗调参数
#define Refined 1 // 细调参数


extern uint8_t RxBuffer_2[Size];
extern int servo_pwm[5];
uint8_t *send_string[7];

void PID_TEST_UART(UART_HandleTypeDef *huart)
{
	if(RxBuffer_2[0] == 'a')
	{
		servo_pwm[2] += Rough;
		send_string[2] = my_itoa(servo_pwm[2]);
		HAL_UART_Transmit(&huart6, send_string[2], strlen(send_string[2]), 100);					
	}	
	else if(RxBuffer_2[0] == 'b')
	{
		servo_pwm[2] -= Rough;
		send_string[2] = my_itoa(servo_pwm[2]);
		HAL_UART_Transmit(&huart6, send_string[2], strlen(send_string[2]), 100);					
	}			
	else if(RxBuffer_2[0] == 'c')
	{
		servo_pwm[3] += Rough;
		send_string[3] = my_itoa(servo_pwm[3]);
		HAL_UART_Transmit(&huart6, send_string[3], strlen(send_string[3]), 100);					
	}	
	else if(RxBuffer_2[0] == 'd')
	{
		servo_pwm[3] -= Rough;
		send_string[3] = my_itoa(servo_pwm[3]);
		HAL_UART_Transmit(&huart6, send_string[3], strlen(send_string[3]), 100);					
	}				
	else if(RxBuffer_2[0] == 'e')
	{
		servo_pwm[4] += Rough;
		send_string[4] = my_itoa(servo_pwm[4]);
		HAL_UART_Transmit(&huart6, send_string[4], strlen(send_string[4]), 100);					
	}	
	else if(RxBuffer_2[0] == 'f')
	{
		servo_pwm[4] -= Rough;
		send_string[4] = my_itoa(servo_pwm[4]);
		HAL_UART_Transmit(&huart6, send_string[4], strlen(send_string[4]), 100);					
	}	
	else if(RxBuffer_2[0] == 'g')
	{
		servo_pwm[5] += 10;
		send_string[5] = my_itoa(servo_pwm[5]);
		HAL_UART_Transmit(&huart6, send_string[5], strlen(send_string[5]), 100);					
	}	
	else if(RxBuffer_2[0] == 'h')
	{
		servo_pwm[5] -= 10;
		send_string[5] = my_itoa(servo_pwm[5]);
		HAL_UART_Transmit(&huart6, send_string[5], strlen(send_string[5]), 100);					

	}		
	else if(RxBuffer_2[0] == 'i')
	{
		servo_pwm[6] += 10;
		send_string[6] = my_itoa(servo_pwm[6]);
		HAL_UART_Transmit(&huart6, send_string[6], strlen(send_string[6]), 100);					
	}	
	else if(RxBuffer_2[0] == 'j')
	{
		servo_pwm[6] -= 10;
		send_string[6] = my_itoa(servo_pwm[6]);
		HAL_UART_Transmit(&huart6, send_string[6], strlen(send_string[6]), 100);					

	}					
}

fp32 dist;
extern TOF_t TOF;
extern chassis_move_t chassis_move;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1) // 使用串口1和openmv通信
  {
			uint8_t temp = RxBuffer_1[0];
			if (temp == 0x31|| temp == 0x32 || temp == 0x33) // '1'或'2'或'3' 开头
			{
				if(find_QRCODE_flag==0)
				{
					find_QRCODE_flag = 1;
					int j = 0;
					for(int i=0; i < 7; i++)
					{
						if(RxBuffer_1[i] != 0x2B) // 去除中间的'+'号
							QRCODE[j++] = RxBuffer_1[i] - '0';
					}
				}
			}
			else if (temp == 'a' || temp == 'b' || temp == 'c' || temp == 'd' || temp == 'e' || temp == 'f')
			{
					switch(temp){
						case 'a':
						{
							if(!find_COLOR_flag[0])
							{
								COLOR[0] = RxBuffer_1[1] - '0';
								find_COLOR_flag[0] = 1;
							}
							break;
						}
						case 'b':
						{
							if(!find_COLOR_flag[1])
							{
								COLOR[1] = RxBuffer_1[1] - '0';
								find_COLOR_flag[1] = 1;
							}
							break;
						}
						case 'c':
						{
							if(!find_COLOR_flag[2])
							{
								COLOR[2] = RxBuffer_1[1] - '0';
								find_COLOR_flag[2] = 1;
							}
							break;
						}
						case 'd':
						{
							if(!find_COLOR_flag[3])
							{
								COLOR[3] = RxBuffer_1[1] - '0';
								find_COLOR_flag[3] = 1;
							}
							break;
						}
						case 'e':
						{
							if(!find_COLOR_flag[4])
							{
								COLOR[4] = RxBuffer_1[1] - '0';
								find_COLOR_flag[4] = 1;
							}
							break;
						}
						case 'f':
						{
							if(!find_COLOR_flag[5])
							{
								COLOR[5] = RxBuffer_1[1] - '0';
								find_COLOR_flag[5] = 1;
							}
							break;
						}
					}
			}
			//HAL_UART_Transmit(&huart6 , RxBuffer_1, Size, 100); // 串口1发送接收到的数据
			HAL_UART_Receive_IT(&huart1,(uint8_t *)RxBuffer_1, 200);//重新开中断
			for(uint8_t i=0; i < Rx_Size; i++)
				RxBuffer_1[i]=0; //清除接收缓存
  }
	else if(huart->Instance==USART6)
  { 
			PID_TEST_UART(&huart6);
			HAL_UART_Receive_IT(&huart6,(uint8_t *)RxBuffer_2, 200);//重新开中断
			for(uint8_t i=0; i < Rx_Size; i++)
				RxBuffer_2[i]=0; //清除接收缓存
			// Rx_Size=0;//清除接收的计数，下次接收重新计数
  }
}

void find_colors_upper()
{
	// 控制机械臂到上层色块识别位置
	pwm_control(1860, 850, 840, 1200, 1000);
	//control_in_turn(2, 3, 4, 850, 840, 1200);
	//control_servo(6, 1860); // 上层中间颜色识别
	while(!COLOR[1])
	{
		HAL_UART_Transmit(&huart1, "FIND_COLORS_2\n", 14, 1000);
		osDelay(10);
	}
	pwm_control(2100, 850, 840, 1200, 1000); // 上层左层颜色识别
	while(!COLOR[0])
	{
		HAL_UART_Transmit(&huart1, "FIND_COLORS_1\n", 14, 1000);
		osDelay(10);
	}
	pwm_control(1550, 850, 840, 1200, 1000); // 上层右层颜色识别
	while(!COLOR[2])
	{
		HAL_UART_Transmit(&huart1, "FIND_COLORS_3\n", 14, 1000);
		osDelay(10);
	}
	find_all_color_flag[0] = 1;
	pwm_control(1860, 850, 840, 1200, 1000); // 上层右层颜色识别
}

void find_colors_bottom()
{
	// 控制机械臂到下层色块识别位置
	pwm_control(1860, 900, 1050, 1200, 1000); // 上层右层颜色识别
//	control_in_turn(2, 3, 4, 900, 1050, 1200);
//	control_servo(6, 1860); // 下层中间颜色识别
	while(!COLOR[4])
	{
		HAL_UART_Transmit(&huart1, "FIND_COLORS_5\n", 14, 1000);
		osDelay(10);
	}
	pwm_control(2030, 900, 1050, 1200, 1000); // 下层左侧颜色识别
	while(!COLOR[3])
	{
		HAL_UART_Transmit(&huart1, "FIND_COLORS_4\n", 14, 1000);
		osDelay(10);
	}
	pwm_control(1580, 900, 1050, 1200, 1000);// 下层右侧颜色识别
	while(!COLOR[5])
	{
		HAL_UART_Transmit(&huart1, "FIND_COLORS_6\n", 14, 1000);
		osDelay(10);
	}
	find_all_color_flag[1] = 1;
	pwm_control(1860, 900, 1050, 1200, 1000);
}

/* USER CODE END 1 */
