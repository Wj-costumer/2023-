#include "bsp_key.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

int start_flag = 0;

void KEY_Task(void const *parameter)
{	
  while (1)
  {
    if(Key_Scan(KEY_GPIO_Port, KEY_Pin) == GPIO_PIN_RESET)
    {
			start_flag = ~start_flag&0x01;
			vTaskDelay(20);/* ��ʱ20��tick */
		}
		vTaskDelay(1);/* ��ʱ20��tick */
	}
}
