#ifndef __BSP_KEY_H
#define	__BSP_KEY_H
 
#include "stm32f4xx.h"

uint8_t Key_Scan(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
#endif 