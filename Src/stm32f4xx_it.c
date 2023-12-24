/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_it.h"
#include "FreeRTOS.h"
#include "task.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */
#include "pid.h"
#include "WIFI.h"
/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
 
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
#define Size 50
#define Size2 128
uint8_t RxBuffer_1[Size];
uint8_t RxBuffer_2[Size2];

uint8_t Steer_Eng=97; //串口接收舵机选择
uint8_t value=50;			//窜口接收转动角度信息,初始化202.5
uint8_t buff;
uint8_t dir;							//舵机增量方向
uint16_t add=20;         //机械臂微调增量 
uint16_t PWM[5];        		//机械臂PWM值
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;
extern TIM_HandleTypeDef htim2;
extern DMA_HandleTypeDef hdma_usart3_rx;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart6;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
#if (INCLUDE_xTaskGetSchedulerState == 1 )
  if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
  {
#endif /* INCLUDE_xTaskGetSchedulerState */
  xPortSysTickHandler();
#if (INCLUDE_xTaskGetSchedulerState == 1 )
  }
#endif /* INCLUDE_xTaskGetSchedulerState */
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles EXTI line0 interrupt.
  */
void EXTI0_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI0_IRQn 0 */

  /* USER CODE END EXTI0_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
  /* USER CODE BEGIN EXTI0_IRQn 1 */

  /* USER CODE END EXTI0_IRQn 1 */
}

/**
  * @brief This function handles EXTI line3 interrupt.
  */
void EXTI3_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI3_IRQn 0 */

  /* USER CODE END EXTI3_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(DRDY_IST8310_Pin);
  /* USER CODE BEGIN EXTI3_IRQn 1 */

  /* USER CODE END EXTI3_IRQn 1 */
}

/**
  * @brief This function handles EXTI line4 interrupt.
  */
void EXTI4_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI4_IRQn 0 */

  /* USER CODE END EXTI4_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(INT1_ACCEL_Pin);
  /* USER CODE BEGIN EXTI4_IRQn 1 */

  /* USER CODE END EXTI4_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream1 global interrupt.
  */
void DMA1_Stream1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream1_IRQn 0 */

  /* USER CODE END DMA1_Stream1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart3_rx);
  /* USER CODE BEGIN DMA1_Stream1_IRQn 1 */

  /* USER CODE END DMA1_Stream1_IRQn 1 */
}

/**
  * @brief This function handles CAN1 RX0 interrupts.
  */
void CAN1_RX0_IRQHandler(void)
{
  /* USER CODE BEGIN CAN1_RX0_IRQn 0 */

  /* USER CODE END CAN1_RX0_IRQn 0 */
  HAL_CAN_IRQHandler(&hcan1);
  /* USER CODE BEGIN CAN1_RX0_IRQn 1 */

  /* USER CODE END CAN1_RX0_IRQn 1 */
}

/**
  * @brief This function handles EXTI line[9:5] interrupts.
  */
void EXTI9_5_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI9_5_IRQn 0 */

  /* USER CODE END EXTI9_5_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(INT1_GYRO_Pin);
  /* USER CODE BEGIN EXTI9_5_IRQn 1 */

  /* USER CODE END EXTI9_5_IRQn 1 */
}

/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */

  /* USER CODE END TIM2_IRQn 0 */
  HAL_TIM_IRQHandler(&htim2);
  /* USER CODE BEGIN TIM2_IRQn 1 */

  /* USER CODE END TIM2_IRQn 1 */
}

/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */

  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */
	// PID_TEST_UART(&huart1); // 如果需要调PID 取消注释
	
	volatile uint8_t receive;
	// receive interrupt 接收中断
	if(huart1.Instance->SR & UART_FLAG_RXNE)
	{
			HAL_UART_IRQHandler(&huart1);
			HAL_UART_Receive_IT(&huart1,(uint8_t *)&RxBuffer_1, Size); //重新打开串口中断
	}
	// idle interrupt 空闲中断
	else if(huart1.Instance->SR & UART_FLAG_IDLE)
	{
			receive = huart1.Instance->DR;
	}
  /* USER CODE END USART1_IRQn 1 */
}


/**
  * @brief This function handles CAN2 RX0 interrupts.
  */
void CAN2_RX0_IRQHandler(void)
{
  /* USER CODE BEGIN CAN2_RX0_IRQn 0 */
	
  /* USER CODE END CAN2_RX0_IRQn 0 */
  HAL_CAN_IRQHandler(&hcan2);
  /* USER CODE BEGIN CAN2_RX0_IRQn 1 */

  /* USER CODE END CAN2_RX0_IRQn 1 */
}

/**
  * @brief This function handles USART6 global interrupt.
  */
extern uint8_t	wifi_success;
uint8_t wifi_corder[6];
uint8_t find_wifi_corder_flag=0;
extern uint8_t receive_u6[128];
void USART6_IRQHandler(void)
{
  /* USER CODE BEGIN USART6_IRQn 0 */
	volatile uint8_t receive;
	// receive interrupt 接收中断
	

	ATK_MW8266D_UART_IRQHandler();
	if(wifi_success==1)
	{
		for(uint8_t i=0;i<=127;i++)
		{
			if(receive_u6[i]=='+')
			{
				if(receive_u6[i-2]<=57
					&&receive_u6[i-2]>=48&&
				receive_u6[i-1]<=57&&
				receive_u6[i-1]>=48&&
				receive_u6[i-3]<=57&&
				receive_u6[i-3]>48&&
				receive_u6[i+1]<=57&&
				receive_u6[i+1]>=48&&
				receive_u6[i+2]<=57&&
				receive_u6[i+2]>=48&&
				receive_u6[i+3]<=57&&
				receive_u6[i+3]>=48)
				{
				wifi_corder[0]=receive_u6[i-3]-48;
				wifi_corder[1]=receive_u6[i-2]-48;
				wifi_corder[2]=receive_u6[i-1]-48;
				wifi_corder[3]=receive_u6[i+1]-48;
				wifi_corder[4]=receive_u6[i+2]-48;
				wifi_corder[5]=receive_u6[i+3]-48;
				find_wifi_corder_flag=1;
				wifi_success=0;
				}
			}
		}
		
	}
}
		      //receive = huart6.Instance->DR;
//		   atk_mw8266d_uart_rx_restart();  
//		atk_mw8266d_uart_rx_get_frame();
//			ATK_MW8266D_UART_IRQHandler();
//	if(huart6.Instance->SR & UART_FLAG_RXNE)
//	{
//			HAL_UART_IRQHandler(&huart6);
//		
//			/* USER CODE BEGIN USART1_IRQn 1 */
////        receive = huart6.Instance->DR;		
////			if(receive!=0x00)
////			{
////				
////				if(usart6_len>5)
////					usart6_len%=6;
////				RxBuffer_2[usart6_len]=receive;
////				usart6_len++;
////			}
//					uint8_t k=0;		
//		        receive = huart6.Instance->DR;
//			HAL_UART_Receive_IT(&huart6,(uint8_t *)&RxBuffer_2, 1); //重新打开串口中断
//	}
//		for(uint8_t i=0;i<50;i++)
//		{
//			if(RxBuffer_2[i]=='+')
//			{

//				for(uint8_t j=3;j>=1;j--)
//			{
//				if(RxBuffer_2[i-j]<'9'&&RxBuffer_2[i-j]>'0')
//				{
//					wifi_corder[k]=RxBuffer_2[i-j];
//					k++;
//				}
//				
//			}
//			
//				for(uint8_t j=1;j>=3;j++)
//			{
//				if(RxBuffer_2[i+j]<'9'&&RxBuffer_2[i+j]>'0')
//				{
//					wifi_corder[k]=RxBuffer_2[i+j];
//					k++;
//				}
//				if(j==3)
//					k=0;
//			}
//			if(k==6)
//				find_wifi_corder_flag=1;
//			
//			}
//		}
//	}		
	
	// idle interrupt 空闲中断
//	else if(huart6.Instance->SR & UART_FLAG_IDLE)
//	{
//			receive = huart6.Instance->DR;
//	}
//  /* USER CODE END USART6_IRQn 0 */
//  HAL_UART_IRQHandler(&huart6);
  /* USER CODE BEGIN USART6_IRQn 1 */
	
  /* USER CODE END USART6_IRQn 1 */


/* USER CODE BEGIN 1 */
/* USER CODE END 1 */
