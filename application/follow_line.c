#include "follow_line.h"
#include "angle_correct.h"
#include "main.h"
#include "CAN_receive.h"
#include "Lcd_Driver.h"
#include "math.h"
#include "Chassis_Move.h"
#include "cmsis_os.h"


extern follow_line light;
extern chassis_move_t chassis_move;
#define PI 3.14159265358979f

fp32 x_target_dist, y_target_dist;
int follow_blackline_flag = 0;

void move_targetpoint_followline(fp32 target_pos[2],follow_dir DIR)
{
	chassis_move.target_pos[0] = target_pos[0];
	chassis_move.target_pos[1] = target_pos[1];
	x_target_dist = chassis_move.target_pos[0] - chassis_move.pos[0];
	y_target_dist = chassis_move.target_pos[1] - chassis_move.pos[1];
	chassis_move.x_target_dist = x_target_dist * cos(-chassis_move.chassis_real_angle/180*PI) - y_target_dist * sin(-chassis_move.chassis_real_angle/180*PI);
	chassis_move.y_target_dist = x_target_dist * sin(-chassis_move.chassis_real_angle/180*PI) + y_target_dist * cos(-chassis_move.chassis_real_angle/180*PI);
	chassis_move.x_move_dist = 0;
	chassis_move.y_move_dist = 0;
	// ����ϴ��ƶ��ǶȻ���
	for(int i = 0; i < 4; i++)
		chassis_move.motor[i]->total_angle = 0;
//	int dir[2]={0,0};//�趨��ֱ��ˮƽ����  #define PI               3.14159265358979f
//	//�жϷ���
//	if(chassis_move.target_pos[0]>chassis_move.pos[0])//���Ŀ����ڳ���������  x����
//				dir[0]=1;
//	else if(chassis_move.target_pos[0]<chassis_move.pos[0])//���Ŀ����ڳ���������
//				dir[0]=2;
//	else 
//				dir[0]=0;//���Ŀ���ͳ�������ֱ����һ��
	// ����Ŀ��������ѭ��
	chassis_move.vx_set=0.0;
	chassis_move.vy_set=0.0;
	if(DIR==FRONT)
	follow_frontline();
	else if(DIR==LEFT)
	follow_leftline();
	else if(DIR==RIGHT)
	follow_rightline();	
	else if(DIR==BEHIND)
	follow_behindline();	

	
	update_position();
	chassis_move.vx_set = 0.0;
	chassis_move.vy_set = 0.0;
	chassis_move.wz_set=0.0;	
	chassis_move.pos[0] += chassis_move.x_move_dist * cos(-chassis_move.chassis_real_angle) + chassis_move.y_move_dist * sin(-chassis_move.chassis_real_angle);
	chassis_move.pos[1] += -chassis_move.x_move_dist * sin(-chassis_move.chassis_real_angle) + chassis_move.y_move_dist * cos(-chassis_move.chassis_real_angle);
	
//	chassis_move.pos[0] += chassis_move.x_move_dist;
//	chassis_move.pos[1] += chassis_move.y_move_dist;
	// ��ǰλ����ΪĿ��λ��
	chassis_move.target_pos[0] = chassis_move.pos[0];
	chassis_move.target_pos[1] = chassis_move.pos[1];

}

/*
												|x������
												|
												|
												|
												|
												|				
		y������--------------

*/


void follow_frontline()
{
	while(fabs(chassis_move.x_target_dist - chassis_move.x_move_dist) > X_MIN_LIMIT )
	{
		chassis_move.vx_set=0.3; // 0.2
		int turnleft=(light.behind_line[0]+light.behind_line[1]+light.front_line[3]+light.front_line[4]);
		int turnright=(light.behind_line[3]+light.behind_line[4]+light.front_line[0]+light.front_line[1]);
		update_position();
		if(((light.behind_line[0]+light.behind_line[1]+light.behind_line[3]+light.behind_line[4]+light.behind_line[2])<=4)
			||(light.front_line[0]+light.front_line[1]+light.front_line[3]+light.front_line[4]+light.front_line[2])<=4)//ֻ�е�ǰ��ͺ��涼û�������ߵ�ʱ��ſ�ʼѰ��
		{
			if((light.behind_line[2]==1)&&(light.front_line[2]==1))//���ǰ���м䶼ɨ���˾ͱ���ֱ��
			{			
				chassis_move.vx_set=0.3; // 0.2
				chassis_move.vy_set=0.0;
				chassis_move.wz_set=0.0;			
			}
			else if(((light.behind_line[0]+light.behind_line[1])>0)&&((light.front_line[3]+light.front_line[4])>0))//���ƫ���˾Ϳ���
			{
				chassis_move.wz_set=0.1*turnleft;		// 0.4					
			}
			else if	(((light.behind_line[3]+light.behind_line[4])>0)&&((light.front_line[0]+light.front_line[1])>0))//���ƫ���˾Ϳ���
			{
				chassis_move.wz_set=-0.1*turnright;	//0.4					
			}
			else if	(((light.behind_line[3]+light.behind_line[4])>0)&&((light.front_line[3]+light.front_line[4])>0))		//���ƫ��������
			{
			chassis_move.wz_set=0.0;
			chassis_move.vy_set=0.1;				
			}
			else if	(((light.behind_line[0]+light.behind_line[1])>0)&&((light.front_line[0]+light.front_line[1])>0))		//���ƫ��������
			{
			chassis_move.wz_set=0.0;
			chassis_move.vy_set=-0.1;				
			}
		}
	}
}

void follow_behindline()
{
	while(fabs(chassis_move.x_target_dist - chassis_move.x_move_dist) > X_MIN_LIMIT )
	{
		chassis_move.vx_set=-0.3; // 0.2
		int turnleft=(light.behind_line[0]+light.behind_line[1]+light.front_line[3]+light.front_line[4]);
		int turnright=(light.behind_line[3]+light.behind_line[4]+light.front_line[0]+light.front_line[1]);
		update_position();
		if(((light.behind_line[0]+light.behind_line[1]+light.behind_line[3]+light.behind_line[4]+light.behind_line[2])<=4)
			||(light.behind_line[0]+light.behind_line[1]+light.behind_line[3]+light.behind_line[4]+light.behind_line[2])<=4)//ֻ�е�ǰ��ͺ��涼û�������ߵ�ʱ��ſ�ʼѰ��
		{
			if((light.behind_line[2]==1)&&(light.front_line[2]==1))//���ǰ���м䶼ɨ���˾ͱ���ֱ��
			{			
				chassis_move.vx_set=-0.3; // 0.2
				chassis_move.vy_set=0.0;
				chassis_move.wz_set=0.0;			
			}
			else if(((light.behind_line[0]+light.behind_line[1])>0)&&((light.front_line[3]+light.front_line[4])>0))//���ƫ���˾Ϳ���
			{
				chassis_move.wz_set=0.1*turnleft;		// 0.4					
			}
			else if	(((light.behind_line[3]+light.behind_line[4])>0)&&((light.front_line[0]+light.front_line[1])>0))//���ƫ���˾Ϳ���
			{
				chassis_move.wz_set=-0.1*turnright;	//0.4					
			}
			else if	(((light.behind_line[3]+light.behind_line[4])>0)&&((light.front_line[3]+light.front_line[4])>0))		//���ƫ��������
			{
			chassis_move.wz_set=0.0;
			chassis_move.vy_set=0.1;				
			}
			else if	(((light.behind_line[0]+light.behind_line[1])>0)&&((light.front_line[0]+light.front_line[1])>0))		//���ƫ��������
			{
			chassis_move.wz_set=0.0;
			chassis_move.vy_set=-0.1;				
			}
		}
	}
}


void follow_leftline()
{
	while(fabs(chassis_move.y_target_dist - chassis_move.y_move_dist) > Y_MIN_LIMIT )
	{
		chassis_move.vy_set=0.3; // 0.2
		int turnleft=(light.left_line[0]+light.left_line[1]+light.right_line[3]+light.right_line[4]);
		int turnright=(light.left_line[3]+light.left_line[4]+light.right_line[0]+light.right_line[1]);
		update_position();
		if(((light.left_line[0]+light.left_line[1]+light.left_line[3]+light.left_line[4]+light.left_line[2])<=4)
			||(light.right_line[0]+light.right_line[1]+light.right_line[3]+light.right_line[4]+light.right_line[2])<=4)//ֻ�е�ǰ��ͺ��涼û�������ߵ�ʱ��ſ�ʼѰ��
		{
			if((light.left_line[2]==1)&&(light.right_line[2]==1))//���ǰ���м䶼ɨ���˾ͱ���ֱ��
			{			
				chassis_move.vx_set=0.0; // 0.2
				chassis_move.vy_set=0.3;
				chassis_move.wz_set=0.0;			
			}
			else if(((light.left_line[0]+light.left_line[1])>0)&&((light.right_line[3]+light.right_line[4])>0))//���ƫ���˾Ϳ���
			{
				chassis_move.wz_set=0.1*turnleft;		// 0.4					
			}
			else if	(((light.left_line[3]+light.left_line[4])>0)&&((light.right_line[0]+light.right_line[1])>0))//���ƫ���˾Ϳ���
			{
				chassis_move.wz_set=-0.1*turnright;	//0.4					
			}
			else if	(((light.left_line[3]+light.left_line[4])>0)&&((light.right_line[3]+light.right_line[4])>0))		//���ƫ��������
			{
			chassis_move.wz_set=0.0;
			chassis_move.vx_set=0.1;				
			}
			else if	(((light.left_line[0]+light.left_line[1])>0)&&((light.right_line[0]+light.right_line[1])>0))		//���ƫ��������
			{
			chassis_move.wz_set=0.0;
			chassis_move.vx_set=-0.1;				
			}
		}
	}
}


void follow_rightline()
{
	while(fabs(chassis_move.y_target_dist - chassis_move.y_move_dist) > Y_MIN_LIMIT )
	{
		chassis_move.vy_set=-0.3; // 0.2
		int turnleft=(light.left_line[0]+light.left_line[1]+light.right_line[3]+light.right_line[4]);
		int turnright=(light.left_line[3]+light.left_line[4]+light.right_line[0]+light.right_line[1]);
		update_position();
		if(((light.left_line[0]+light.left_line[1]+light.left_line[3]+light.left_line[4]+light.left_line[2])<=4)
			||(light.right_line[0]+light.right_line[1]+light.right_line[3]+light.right_line[4]+light.right_line[2])<=4)//ֻ�е�ǰ��ͺ��涼û�������ߵ�ʱ��ſ�ʼѰ��
		{
			if((light.left_line[2]==1)&&(light.right_line[2]==1))//���ǰ���м䶼ɨ���˾ͱ���ֱ��
			{			
				chassis_move.vx_set=0.0; // 0.2
				chassis_move.vy_set=-0.3;
				chassis_move.wz_set=0.0;			
			}
			else if(((light.left_line[0]+light.left_line[1])>0)&&((light.right_line[3]+light.right_line[4])>0))//���ƫ���˾Ϳ���
			{
				chassis_move.wz_set=0.1*turnleft;		// 0.4					
			}
			else if	(((light.left_line[3]+light.left_line[4])>0)&&((light.right_line[0]+light.right_line[1])>0))//���ƫ���˾Ϳ���
			{
				chassis_move.wz_set=-0.1*turnright;	//0.4					
			}
			else if	(((light.left_line[3]+light.left_line[4])>0)&&((light.right_line[3]+light.right_line[4])>0))		//���ƫ��������
			{
			chassis_move.wz_set=0.0;
			chassis_move.vx_set=0.1;				
			}
			else if	(((light.left_line[0]+light.left_line[1])>0)&&((light.right_line[0]+light.right_line[1])>0))		//���ƫ��������
			{
			chassis_move.wz_set=0.0;
			chassis_move.vx_set=-0.1;				
			}
		}
	}
}


void follow_line_go_to_downblackline()
{
	chassis_move.x_move_dist = 0;
	chassis_move.y_move_dist = 0;
		// ����ϴ��ƶ��ǶȻ���
	for(int i = 0; i < 4; i++)
		chassis_move.motor[i]->total_angle = 0;

	
	while((light.behind_line[0]+light.behind_line[1]+light.behind_line[3]+light.behind_line[4]+light.behind_line[2])<5)
	{
		chassis_move.vx_set=0.1; // 0.2
		int turnleft=(light.behind_line[0]+light.behind_line[1]+light.front_line[3]+light.front_line[4]);
		int turnright=(light.behind_line[3]+light.behind_line[4]+light.front_line[0]+light.front_line[1]);
		update_position();
		if(((light.behind_line[0]+light.behind_line[1]+light.behind_line[3]+light.behind_line[4]+light.behind_line[2])<=4)
			||(light.behind_line[0]+light.behind_line[1]+light.behind_line[3]+light.behind_line[4]+light.behind_line[2])<=4)//ֻ�е�ǰ��ͺ��涼û�������ߵ�ʱ��ſ�ʼѰ��
		{
			if((light.behind_line[2]==1&&light.front_line[2]==1))//���ǰ���м䶼ɨ���˾ͱ���ֱ��
		{			
			chassis_move.vx_set=0.1; // 0.2
			chassis_move.vy_set=0.0;
			chassis_move.wz_set=0.0;			
		}
			else if	((light.behind_line[0]+light.behind_line[1]>0)&&((light.front_line[3]+light.front_line[4])>0))//���ƫ���˾Ϳ���
		{
			chassis_move.wz_set=0.2*turnleft;		// 0.4	
					
		}
			else if	(((light.behind_line[3]+light.behind_line[4])>0)&&((light.front_line[0]+light.front_line[1])>0))//���ƫ���˾Ϳ���
		{
			chassis_move.wz_set=-0.2*turnright;	//0.4		
					
		}
			else if	(((light.behind_line[3]+light.behind_line[4])>0)&&((light.front_line[3]+light.front_line[4])>0))		//���ƫ��������
			{
			chassis_move.wz_set=0.0;
			chassis_move.vy_set=0.1;				
				
			}
			else if	(((light.behind_line[0]+light.behind_line[1])>0)&&((light.front_line[0]+light.front_line[1])>0))		//���ƫ��������
			{
			chassis_move.wz_set=0.0;
			chassis_move.vy_set=-0.1;				
				
			}
			
		}
	}	
	chassis_move.vx_set = 0.0;
	chassis_move.vy_set = 0.0;
	chassis_move.wz_set=0.0;	
	update_position();

	chassis_move.pos[0] += chassis_move.x_move_dist * cos(-chassis_move.chassis_real_angle) + chassis_move.y_move_dist * sin(-chassis_move.chassis_real_angle);
	chassis_move.pos[1] += -chassis_move.x_move_dist * sin(-chassis_move.chassis_real_angle) + chassis_move.y_move_dist * cos(-chassis_move.chassis_real_angle);
	
}

void follow_line_go_to_downfrontline()
{
	chassis_move.x_move_dist = 0;
	chassis_move.y_move_dist = 0;
		// ����ϴ��ƶ��ǶȻ���
	for(int i = 0; i < 4; i++)
		chassis_move.motor[i]->total_angle = 0;

int backlight;
			int frontlight;
	while(((light.front_line[0]+light.front_line[1]+light.front_line[3]+light.front_line[4]+light.front_line[2])<5)||light.behind_line[2]==0||backlight>0)
	{
			backlight=light.behind_line[0]+light.behind_line[1]+light.behind_line[3]+light.behind_line[4];
			frontlight=light.front_line[0]+light.front_line[1]+light.front_line[3]+light.front_line[4]+light.front_line[2];
		if(frontlight<=2)
		chassis_move.vx_set=0.1; // 0.2
		else if(frontlight==5)
		chassis_move.vx_set=0;			
		int turnleft=(light.behind_line[0]+light.behind_line[1]+light.front_line[3]+light.front_line[4]);
		int turnright=(light.behind_line[3]+light.behind_line[4]+light.front_line[0]+light.front_line[1]);
		update_position();
//		if(((light.behind_line[0]+light.behind_line[1]+light.behind_line[3]+light.behind_line[4]+light.behind_line[2])<=4)
//			||(light.behind_line[0]+light.behind_line[1]+light.behind_line[3]+light.behind_line[4]+light.behind_line[2])<=4)//ֻ�е�ǰ��ͺ��涼û�������ߵ�ʱ��ſ�ʼѰ��
//		{
			if((light.behind_line[2]==1&&light.front_line[2]==1))//���ǰ���м䶼ɨ���˾ͱ���ֱ��
		{			
			chassis_move.vx_set=0.1; // 0.2
			chassis_move.vy_set=0.0;
			chassis_move.wz_set=0.0;			
		}
			else if	((light.behind_line[0]+light.behind_line[1]>0)&&((light.front_line[3]+light.front_line[4])>0))//���ƫ���˾Ϳ���
		{
			chassis_move.wz_set=0.2*turnleft;		// 0.4	
					
		}
			else if	(((light.behind_line[3]+light.behind_line[4])>0)&&((light.front_line[0]+light.front_line[1])>0))//���ƫ���˾Ϳ���
		{
			chassis_move.wz_set=-0.2*turnright;	//0.4		
					
		}
			else if	(((light.behind_line[3]+light.behind_line[4])>0)&&((light.front_line[3]+light.front_line[4])>0))		//���ƫ��������
			{
			chassis_move.wz_set=0.0;
			chassis_move.vy_set=0.1;				
				
			}
			else if	(((light.behind_line[0]+light.behind_line[1])>0)&&((light.front_line[0]+light.front_line[1])>0))		//���ƫ��������
			{
			chassis_move.wz_set=0.0;
			chassis_move.vy_set=-0.1;				
				
			}
			
//}
	}	
	chassis_move.vx_set = 0.0;
	chassis_move.vy_set = 0.0;
	chassis_move.wz_set=0.0;	
	update_position();

	chassis_move.pos[0] += chassis_move.x_move_dist * cos(-chassis_move.chassis_real_angle) + chassis_move.y_move_dist * sin(-chassis_move.chassis_real_angle);
	chassis_move.pos[1] += -chassis_move.x_move_dist * sin(-chassis_move.chassis_real_angle) + chassis_move.y_move_dist * cos(-chassis_move.chassis_real_angle);
	
}


void follow_line_task(void const * argument)
{
		//wait a time
    osDelay(CHASSIS_TASK_INIT_TIME);
		while(1)
		{
//			if(follow_blackline_flag)
//			{
//				while((light.behind_line[0]+light.behind_line[1]+light.behind_line[3]+light.behind_line[4]+light.behind_line[2])<5 || (light.front_line[0]+light.front_line[1]+light.front_line[3]+light.front_line[4]+light.front_line[2]) > 1)
//				{
//					chassis_move.vx_set=0.1; // 0.2
//					int turnleft=(light.behind_line[0]+light.behind_line[1]+light.front_line[3]+light.front_line[4]);
//					int turnright=(light.behind_line[3]+light.behind_line[4]+light.front_line[0]+light.front_line[1]);
//					if(((light.behind_line[0]+light.behind_line[1]+light.behind_line[3]+light.behind_line[4]+light.behind_line[2])<=4)
//							||(light.front_line[0]+light.front_line[1]+light.front_line[3]+light.front_line[4]+light.front_line[2])<=4)//ֻ�е�ǰ��ͺ��涼û�������ߵ�ʱ��ſ�ʼѰ��
//					{
//							if(light.behind_line[2]==1 && light.front_line[2]==1 && (light.front_line[0]+light.front_line[1]+light.front_line[3]+light.front_line[4]+light.front_line[2]) > 1)//���ǰ���м䶼ɨ���˾ͱ���ֱ��
//							{			
//									chassis_move.vx_set=0.1; // 0.2
//									chassis_move.vy_set=0.0;
//									chassis_move.wz_set=0.0;			
//							}
//							else if	((light.behind_line[0]+light.behind_line[1]>0)&&((light.front_line[3]+light.front_line[4])>0))//���ƫ���˾Ϳ���
//							{
//									chassis_move.wz_set=0.2*turnleft;		// 0.4	
//					
//							}
//							else if	(((light.behind_line[3]+light.behind_line[4])>0)&&((light.front_line[0]+light.front_line[1])>0))//���ƫ���˾Ϳ���
//							{
//									chassis_move.wz_set=-0.2*turnright;	//0.4		
//					
//							}
//							else if	(((light.behind_line[3]+light.behind_line[4])>0)&&((light.front_line[3]+light.front_line[4])>0))		//���ƫ��������
//							{
//									chassis_move.wz_set=0.0;
//									chassis_move.vy_set=0.1;				
//							}
//							else if	(((light.behind_line[0]+light.behind_line[1])>0)&&((light.front_line[0]+light.front_line[1])>0))		//���ƫ��������
//							{
//									chassis_move.wz_set=0.0;
//									chassis_move.vy_set=-0.1;				
//							}
//					}
//				}
//				chassis_move.vx_set = 0.0;
//				chassis_move.vy_set = 0.0;
//				chassis_move.wz_set=0.0;
//			}
			osDelay(1);
		}
}