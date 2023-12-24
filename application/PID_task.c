#include "PID.h"
#include "main.h"
#include "cmsis_os.h"
#include "CAN_receive.h"


extern chassis_move_t chassis_move;
extern fp32 origin_error;
extern int rotation_flag;
extern SemaphoreHandle_t Rotation_Handle;

void PID_CALC_TASK()
{
		//wait a time
    osDelay(CHASSIS_TASK_INIT_TIME);
		while(1)
		{
			taskENTER_CRITICAL(); // 进入临界区
			// 车整体速度分解成各个轮子的速度
//			chassis_move.wz_set = PID_calc(&chassis_move.angle_turn_pid, chassis_move.chassis_angles[2], chassis_move.chassis_target_angle); 

			vector_to_mecanum_wheel_speed(chassis_move.vx_set, chassis_move.vy_set, chassis_move.wz_set, chassis_move.speed_target);
			uint32_t PRT = xTaskGetTickCount();
			
			// PID计算
			for (int i=0; i<4; i++)
			{		
				chassis_move.speed_target[i] *= MPS_TO_RPM; // m/s->rpm
				chassis_move.motor[i]->given_current = (int)PID_calc(&chassis_move.move_pid[i], chassis_move.motor[i]->speed_rpm, chassis_move.speed_target[i]);			
			}
			// 发送控制电流
			CAN_cmd_chassis(chassis_move.motor[0]->given_current, chassis_move.motor[1]->given_current, chassis_move.motor[2]->given_current, chassis_move.motor[3]->given_current);
			taskEXIT_CRITICAL(); // 离开临界区
			osDelayUntil(&PRT,1); // PID计算频率1khz
		}
}