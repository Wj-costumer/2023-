#include "pid.h"
#include "drive.h"
#include "CAN_receive.h"
#include "test_task.h"
#include "pid.h"


extern pid_type_def motor_pid;
extern pid_type_def angle_turn_pid;
extern pid_type_def angle_straight_pid;
//extern int speed_target[4];
int drive_flag=straight_drive_flag;
//extern 	int16_t give_current[4];
//extern float angles[3];
//extern float angle_target;
//extern int16_t speed[4];
//int16_t stragiht_speedtarget=800;

//void drive_mode(int drive_flag)
//{
//		int i;
//		if(drive_flag==turn_drive_flag)//turn_drive_flag
//		{
//				for(i=0;i<=3;i++)
//				{
//					speed_target[i]=-PID_calc(&angle_turn_pid,angles[2],angle_target);
//					give_current[i]=(int)PID_calc(&motor_pid,speed[i],speed_target[i]);		
//				}
//		}
//		
//		if(drive_flag==straight_drive_flag)//straight_drive_flag
//		{
//				int error;
//				error=PID_calc(&angle_straight_pid,angles[2],angle_target);
//				give_current[0]=(int)PID_calc(&motor_pid,speed[0],-stragiht_speedtarget-error);		
//				give_current[1]=(int)PID_calc(&motor_pid,speed[1],stragiht_speedtarget-error);		
//				give_current[2]=(int)PID_calc(&motor_pid,speed[2],stragiht_speedtarget-error);		
//				give_current[3]=(int)PID_calc(&motor_pid,speed[3],-stragiht_speedtarget-error);		
//		}
//		
//		if(drive_flag==without_current_flag)//straight_drive_flag
//		{
//				give_current[0]=0;		
//				give_current[1]=0;		
//				give_current[2]=0;		
//				give_current[3]=0;		
//		}
//		
//		if(drive_flag==no_move_flag)
//		{	
//				int i;
//				for(i=0;i<=3;i++)
//				give_current[i]=(int)PID_calc(&motor_pid,speed[i],0);		
//		}
//}

//void car_forward(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4)
//{
//	speed_target[0]=-motor1;
//	speed_target[1]=motor2;
//	speed_target[2]=motor3;
//	speed_target[3]=-motor4;
//}

//void car_back(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4)
//{
//	speed_target[0]=motor1;
//	speed_target[1]=-motor2;
//	speed_target[2]=-motor3;
//	speed_target[3]=motor4;	
//}

//void car_right(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4)
//{
//	speed_target[0]=motor1;
//	speed_target[1]=motor2;
//	speed_target[2]=-motor3;
//	speed_target[3]=-motor4;
//}

//void car_left(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4)
//{
//	speed_target[0]=-motor1;
//	speed_target[1]=-motor2;
//	speed_target[2]=motor3;
//	speed_target[3]=motor4;
//}

//void car_rightturn(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4)
//{
//	speed_target[0]=motor1;
//	speed_target[1]=motor2;
//	speed_target[2]=motor3;
//	speed_target[3]=motor4;
//}

//void car_leftturn(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4)
//{
//	speed_target[0]=-motor1;
//	speed_target[1]=-motor2;
//	speed_target[2]=-motor3;
//	speed_target[3]=-motor4;
//}


/*
	根据角度积分出各个轮子移动距离分量
	*/
void get_total_angle_and_dist_xy(motor_measure_t *ptr)
{
	int res1, res2, delta;
	if (ptr->ecd - ptr->last_ecd > 4096)
		ptr->round_cnt --;
	else if(ptr->ecd - ptr->last_ecd < -4096)
		ptr->round_cnt ++;
	
	if (ptr->ecd < ptr->last_ecd)
	{
		res1 = ptr->ecd + 8192 - ptr->last_ecd; //正转，delta=+
		res2 = ptr->ecd - ptr->last_ecd; //反转	delta=-
	}
	else
	{
		res1 = ptr->ecd - 8192 - ptr->last_ecd; // 反转 delta--
		res2 = ptr->ecd - ptr->last_ecd; // //正转	delta +
	}
	delta = fabs(res1) < fabs(res2) ? res1 : res2; // 取较小角度
	ptr->total_angle += (int)delta;
}

