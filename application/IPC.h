#ifndef _IPC_H_
#define _IPC_H_

#include "main.h"
#include "usart.h"





void Status_choose(void);
void SCI_Send_Datas(UART_HandleTypeDef *huart) ;
void PWM_drive(void);
void Arm_Adjust(void);

#endif
