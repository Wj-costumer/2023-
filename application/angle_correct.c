#include "angle_correct.h"
#include "main.h"
#include "CAN_receive.h"
#include "Lcd_Driver.h"
#include "math.h"
extern uint8_t angle_mode[2];
extern chassis_move_t chassis_move;
fp32 vx[2],vy[2],vz[2];
follow_line light;
extern 	uint8_t receive[4];

void read_pin()//读取io拓展版传来的信息值
{
	uint8_t left=receive[0];
	uint8_t right=receive[1];
	uint8_t front=receive[2];
	uint8_t behind=receive[3];	
	for(int i=0;i<=4;i++)
	{
		
		if((left&0x01)==1)
			light.left_line[i]=1;		//L2是0，L1是1,M是2，R1是3,R2是4 
		else 
			light.left_line[i]=0;			
		
		left=left>>1;
	}
	for(int i=0;i<=4;i++)
	{
		
		if((behind&0x01)==1)
			light.behind_line[i]=1;		//L2是0，L1是1，M是2，R1是3，R2是4
		else 
			light.behind_line[i]=0;			
		
		behind=behind>>1;
	}	
	for(int i=0;i<=4;i++)
	{
		
		if((right&0x01)==1)
			light.right_line[i]=1;	//R2是0，R1是1,M是2，L1是3,L2是4 
		else 
			light.right_line[i]=0;			
		
		right=right>>1;
	}	
	for(int i=0;i<=4;i++)
	{
		
		if((front&0x01)==1)			
			light.front_line[i]=1;	//R2是0，R1是1,M是2，L1是3,L2是4 
		else 
			light.front_line[i]=0;			
		
		front=front>>1;
	}	
	
	
	
}
	int behind=0;
	int front=0;
	int left=0;
	int right=0;
extern fp32 angle_error;
#define beh	light.behind_line[0]+light.behind_line[1]+light.behind_line[3]+light.behind_line[4]
#define fro	light.front_line[3]+light.front_line[4]+light.front_line[0]+light.front_line[1]
#define lef	light.left_line[0]+light.left_line[1]+light.left_line[3]+light.left_line[4]
#define rig	light.right_line[3]+light.right_line[4]+light.right_line[0]+light.right_line[1]
void angle_correct(void)
{

	while((light.behind_line[2]==0||light.front_line[2]==0||light.left_line[2]==0||light.right_line[2]==0)||(beh+fro+lef+rig)>0)
	{
		
			behind=(light.behind_line[0]+light.behind_line[1]+light.behind_line[3]+light.behind_line[4]);
		 front=(light.front_line[3]+light.front_line[4]+light.front_line[0]+light.front_line[1]);
		 left=(light.left_line[0]+light.left_line[1]+light.left_line[3]+light.left_line[4]);
		 right=(light.right_line[3]+light.right_line[4]+light.right_line[0]+light.right_line[1]);
		


	if(((light.left_line[0]+light.left_line[1]+light.left_line[3]+light.left_line[4]+light.left_line[2])==0)&&
		(light.right_line[0]+light.right_line[1]+light.right_line[3]+light.right_line[4]+light.right_line[2])==0)
					chassis_move.vx_set=0.05;	

		if((light.behind_line[2]!=1||light.front_line[2]!=1)||((behind!=0)||front!=0))//如果前后中间不为1
	{
		
		 if	(((light.behind_line[3]+light.behind_line[4])>0)&&((light.front_line[3]+light.front_line[4])>0))		//如果偏右了左移
			{
//			chassis_move.wz_set=0.0;
//			chassis_move.vy_set=0.07;				
				light.state[0]=1;
			}
			else if	(((light.behind_line[0]+light.behind_line[1])>0)&&((light.front_line[0]+light.front_line[1])>0))		//如果偏左了右移
			{
//			chassis_move.wz_set=0.0;
//			chassis_move.vy_set=-0.07;				
				light.state[0]=2;
			}			
		else if(((light.behind_line[0]+light.behind_line[1])>0)||((light.front_line[3]+light.front_line[4])>0))//要逆时针
		{
//			chassis_move.wz_set=0.45;				
					light.state[0]=3;
		}

			else if	(((light.behind_line[3]+light.behind_line[4])>0)||((light.front_line[0]+light.front_line[1])>0))//要顺时针
		{
//			chassis_move.wz_set=-0.45;			
				light.state[0]=4;	
		}
	
	}
	else if((light.behind_line[2]==1&&light.front_line[2]==1))
	{
//			chassis_move.wz_set=0.0;
//			chassis_move.vy_set=0.0;
			light.state[0]=5;
	}
	
	
				if((behind+light.behind_line[2]+front+light.front_line[2])==0)
				light.state[0]=light.last_state[0];//如果左右都找不到那就按照上次的状态
	
	light.last_state[0]=light.state[0];
				
				
	switch(light.state[0])
	{
		case 1:
		{
			
			chassis_move.wz_set=0.0;
			chassis_move.vy_set=0.05;		//偏右了向左移动
			break;
		}
		case 2:
		{
			
			chassis_move.wz_set=0.0;
			chassis_move.vy_set=-0.05;				//偏左了向右移动
				
			break;
		}			
		case 3:
		{
			
			chassis_move.wz_set=0.20;	//逆时针
				
			break;
		}			
		case 4:
		{
			
		chassis_move.wz_set=-0.20;//顺时针
			break;
		}					
		case 5:
		{
			chassis_move.wz_set=0.0;
			chassis_move.vy_set=0.0;	
		chassis_move.wz_set=0.0;//静止
		}
	}
	
	
	
	if((light.left_line[2]!=1||light.right_line[2]!=1)||((left!=0)||right!=0))//如果左右中间不为1
	{
		if((light.left_line[0]+light.left_line[1]>0)||((light.right_line[0]+light.right_line[1])>0))//偏前了往后	
		{
//			chassis_move.vx_set =-0.07;			
				light.state[1]=1;
		}
			else if((light.left_line[3]+light.left_line[4]>0)||((light.right_line[3]+light.right_line[4])>0))//偏后了往前
		{
//			chassis_move.vx_set=0.07;			
				light.state[1]=2;
		}
	}
	else if((light.left_line[2]==1&&light.right_line[2]==1))
	{
//			chassis_move.vx_set=0.0;			
				light.state[1]=3;
	}
	
	
				if((left+light.left_line[2]+right+light.right_line[2])==0)
				light.state[1]=light.last_state[1];//如果左右都找不到那就按照上次的状态
	
	light.last_state[1]=light.state[1];
	switch (light.state[1])
	{
		case 1:
		{
				chassis_move.vx_set =-0.05;	//偏前了往后		
				break;
		}
		case	2:
		{
				chassis_move.vx_set =0.05;	//偏后了往前		
				break;			
			
		}
		case 3:
		{
				chassis_move.vx_set=0.0;		//静止
				break;
		}
		
	}
		
	
}
	angle_error=chassis_move.chassis_target_angle - chassis_move.chassis_angles[2];
	chassis_move.vx_set = 0.0;
	chassis_move.vy_set = 0.0;
	chassis_move.wz_set=0.0;
	chassis_move.chassis_real_angle = chassis_move.chassis_target_angle;
}
void front_middle_correct(void)
{
	
	if(light.front_line[2]==1)
	{
		chassis_move.vx_set = 0.0;
		chassis_move.vy_set = 0.0;
		chassis_move.wz_set=0.0;
	}		
 if((light.front_line[0]+light.front_line[1])>=1)
	{
		chassis_move.vx_set = 0.0;
		chassis_move.vy_set = -0.05;
		chassis_move.wz_set=0.0;		
	}
 if((light.front_line[4]+light.front_line[3])>=1)
	{
		chassis_move.vx_set = 0.0;
		chassis_move.vy_set = 0.05;
		chassis_move.wz_set=0.0;		
	}	
}
//int angle_correct(uint8_t mode[2])
//{
////*****************************//
//	if(mode[0]==left_move)
//	{
//		vx[0]=0;
//		vy[0]=0.05;
//		vz[0]=0.0;
//	}
//	else if(mode[0]==still)
//	{
//		vx[0]=0;
//		vy[0]=0;
//		vz[0]=0;		
//	}
//	else if(mode[0]==right_move)
//	{
//		vx[0]=0;
//		vy[0]=-0.05;
//		vz[0]=0;		
//	}	
////******************************//
//	if(mode[1]==forward)
//	{
//		vx[1]=0.05;
//		vy[1]=0;
//		vz[1]=0;
//	}
//	else if(mode[1]==still)
//	{
//		vx[1]=0;
//		vy[1]=0;
//		vz[1]=0;		
//	}
//	else if(mode[1]==turn_right)
//	{
//		vx[1]=0;
//		vy[1]=0;
//		vz[1]=-0.2;	
//	}	
//	else if(mode[1]==turn_left)
//	{
//		vx[1]=0;
//		vy[1]=0;
//		vz[1]=0.2;
//	}		
//	else if(mode[1]==back)
//	{
//		vx[1]=-0.05;
//		vy[1]=0;
//		vz[1]=0;		
//		
//	}

//		chassis_move.vx_set=vx[1]+vx[0];
//		chassis_move.vy_set=vy[1]+vy[0];
//		chassis_move.wz_set=vz[1]+vz[0];
//		C1_can_transmit(1);	
//		Gui_DrawFont_Num32(10,10,BLACK,WHITE,angle_mode[0]); 
//		Gui_DrawFont_Num32(50,50,BLACK,WHITE,angle_mode[1]);		
//			
//		if(mode[0]==still&&mode[1]==still)
//		{	
//			chassis_move.vx_set=0;
//			chassis_move.vy_set=0;
//			chassis_move.wz_set=0;			
//			return 0;
//		}
//			
//			else 
//			return 1;
//}

//void set_point()
//{
//		//x正方向由起点向左，y正向方由起点向上
//	if(light.current_point[0]<light.target_point[0])
//		light.dir_x=move_left;//向左
//	else if(light.current_point[0]>light.target_point[0])
//		light.dir_x=move_right;//向右
//	
//	if(light.current_point[1]>light.target_point[1])
//		light.dir_y=move_back;	//向后
//	else if(light.current_point[1]<light.target_point[1])
//		light.dir_y=move_front;//向前
//	float	vector_rate[2];
//	float delta_x = light.target_point[0] - light.current_point[0];
//	float delta_y = light.target_point[1] - light.current_point[1];
//	if(delta_x ==0 && delta_y ==0)		//如果当前位置处于理想位置则将速度赋为0
//	{
//		chassis_move.vx_set = 0.0;
//		chassis_move.vy_set = 0.0;
//	}
//	else
//	{
//		vector_rate[0] = delta_x / SQR(delta_x, delta_y);
//		vector_rate[1] = delta_y / SQR(delta_x, delta_y);
//		chassis_move.vx_set = vector_rate[0] * VX_BASE; // x方向分速度
//		chassis_move.vy_set = vector_rate[1] * VY_BASE; // y方向分速度
//	}			
//}
//void followline_move_target_point(float target_point[2])
//{
//	count_line_init();
//	light.target_point[0]=target_point[0];
//	light.target_point[1]=target_point[1];
//	
//	
//}

void count_line_init()
{
	light.target_point[0]=0;
	light.target_point[1]=0;
	light.current_point[0]=0;
	light.current_point[1]=0;	
	light.left_move_postion=0;
	light.right_move_postion=0;
	light.behind_move_postion=0;
	light.front_move_postion=0;	
	light.left_cross_line=0;
	light.right_cross_line=0;
	light.front_cross_line=0;
	light.behind_cross_line=0;
}
void	count_line()
{
	/*
	chassis_move.x_move_dist = (chassis_move.motor[1]->total_angle - chassis_move.motor[0]->total_angle)/ MAX_ROUND / SLOW_SPEED_RATE /2.0 * WHEEL_D * PI ;
	chassis_move.y_move_dist
*/
	
	for(int i=0;i<5;i++)
		{	
					if(light.left_line[i]==1)		//如果某个灯找到黑线，就在标志中记为1，并且更新此时的轮子移动距离，
			{

						light.left_last_line[i]=1;
						light.left_last_move[i]=	chassis_move.y_move_dist;
						if(i>light.left_max)
							light.left_max=i;
						if(i<light.left_min)
							light.left_min=i;
			}
					if(light.right_line[i]==1)
			{

						light.right_last_line[i]=1;
						light.right_last_move[i]=	chassis_move.y_move_dist;
						if(i>light.right_max)
							light.right_max=i;
						if(i<light.right_min)
							light.right_min=i;
			}
					if(light.front_line[i]==1)
			{

						light.front_last_line[i]=1;
						light.front_last_move[i]=	chassis_move.y_move_dist;
						if(i>light.front_max)
							light.front_max=i;
						if(i<light.front_min)
							light.front_min=i;
			}
					if(light.behind_line[i]==1)
			{

						light.behind_last_line[i]=1;
						light.behind_last_move[i]=	chassis_move.y_move_dist;
						if(i>light.behind_max)
							light.behind_max=i;
						if(i<light.behind_min)
							light.behind_min=i;
			}
		}
		float k=light.left_move_postion-light.left_last_move[2];
	/*******************************找左边*****************************/
					if(((light.left_line[0]+light.left_line[1]+light.left_line[2]+light.left_line[3]+light.left_line[4])>=5)
						&&(judege_blackline(light.left_last_move)<LIMIT)
					&&((k<-MAX)
					||(k>MAX)))
					//当有五个灯找到黑线并且这几个灯找到时的距离的方差小于一定值且这次的位置和上次的位置比起来大于某个值时判断为找到黑线
			{
					light.left_cross_line++;
				
					light.left_move_postion=light.left_last_move[2];
						for(int i=0;i<5;i++)
					{	
						light.left_last_line[i]=0;
						//light.left_last_move[i]=0;
					}	
			}
			
	/*******************************找右边*****************************/			
					if(((light.right_last_line[0]+light.right_last_line[1]+light.right_last_line[2]+light.right_last_line[3]+light.right_last_line[4])>=4)
						&&(judege_blackline(light.right_last_move)<LIMIT)&&(ABS(light.right_move_postion-light.right_last_move[2]))>MAX)
			{
					light.right_cross_line++;
					light.right_move_postion=light.right_last_move[2];
						for(int i=0;i<5;i++)
					{	
						light.right_last_line[i]=0;
						light.right_last_move[i]=0;
					}	
			}
	/*******************************找前边*****************************/
					if(((light.front_last_line[0]+light.front_last_line[1]+light.front_last_line[2]+light.front_last_line[3]+light.front_last_line[4])>=4)
						&&(judege_blackline(light.front_last_move)<LIMIT)&&(ABS(light.front_move_postion-light.front_last_move[2]))>MAX)
			{
					light.front_cross_line++;
				light.front_move_postion=light.front_last_move[2];
						for(int i=0;i<5;i++)
					{	
						light.front_last_line[i]=0;
						light.front_last_move[i]=0;
					}	
			}			
			
	/*******************************找后边*****************************/
					if(((light.behind_last_line[0]+light.behind_last_line[1]+light.behind_last_line[2]+light.behind_last_line[3]+light.behind_last_line[4])>=4)
						&&(judege_blackline(light.behind_last_move)<LIMIT)&&(ABS(light.behind_move_postion-light.behind_last_move[2]))>MAX)
			{
					light.behind_cross_line++;
				light.behind_move_postion=light.behind_last_move[2];				
						for(int i=0;i<5;i++)
					{	
						light.behind_last_line[i]=0;
						light.behind_last_move[i]=0;
					}	
			}			
			
			
}

fp32 judege_blackline(fp32 move[5])//判断五个点成为黑线时的轮子移动距离
{
	fp32 mean=0,variance=0;
	for(int i=0;i<3;i++)
		mean+=move[i];
		mean=mean/3;	
	for(int i=0;i<3;i++)
		variance+=powf((move[i]-mean),(float)2);
	
	return variance;
}