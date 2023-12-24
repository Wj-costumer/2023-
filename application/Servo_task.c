#include "Servo_task.h"
#include "arm_control.h"
#include "angle_correct.h"
int carry_finish[2]={0}; // 上层or下层搬运完成

extern int QRCODE[];
extern uint8_t COLOR[];
extern int sevor_pwm[7];
extern int sevor_pwm_final[7];
extern int target_pwm_in[3];
extern int target_pwm_out[3];
extern uint8_t correct_front_middle_flag;
extern uint8_t correct_shizi_flag;
extern int start_flag;
void servo_task()
{
	osDelay(2000);
	while(1)
	{
		if(start_flag)
		{
			
			if(correct_shizi_flag)
			{
				angle_correct();
			}
			if(correct_front_middle_flag)
			{
				front_middle_correct();
				
			}
		}
	osDelay(1);
		}
	}