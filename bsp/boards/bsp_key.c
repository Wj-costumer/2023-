#include "bsp_key.h"
#include "main.h"
#include "stm32f4xx_hal_gpio.h"

uint8_t Key_Scan(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
		uint8_t state;
		state = HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin);
		if(state == GPIO_PIN_RESET)
		{
			while(HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin) == GPIO_PIN_RESET);
			return GPIO_PIN_RESET;
		}
		else
			return GPIO_PIN_SET;
}