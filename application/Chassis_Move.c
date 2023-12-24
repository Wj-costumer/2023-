#include "Chassis_Move.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "Pid.h"
#include "math.h"
#include "QCS.h"
#include "CAN_receive.h"

chassis_move_t chassis_move;

fp32 speed_target[4];
fp32 speed[4];
motor_measure_t motor_chassis[4];
pid_type_def motor_pid;
pid_type_def angle_turn_pid;
pid_type_def angle_straight_pid;
pid_type_def move_pid[4];
pid_type_def position_pid;
pid_type_def position_pid_;
extern RC_ctrl_t rc_ctrl;

const fp32 motor_pid_PID[3] = {MOTOR_PID_KP, MOTOR_PID_KI, MOTOR_PID_KD};
const fp32 angle_pid_turn_PID[3] = {ANGLE_PID_TURN_KP, ANGLE_PID_TURN_KI, ANGLE_PID_TURN_KD};
const fp32 move_pid_PID[3] = {MOVE_PID_KP, MOVE_PID_KI, MOVE_PID_KD};
const fp32 position_pid_PID[3] = {POSITION_PID_KP, POSITION_PID_KI, POSITION_PID_KD};
const fp32 position_pid_PID_[3] = {POSITION_PID_KP*0.5, POSITION_PID_KI*0.5, POSITION_PID_KD*0.5};
/**
  * @brief          初始化"chassis_move"变量，包括pid初始化， 遥控器指针初始化，2006底盘电机指针初始化，陀螺仪角度指针初始化
  * @param[out]     chassis_move_init:"chassis_move"变量指针.
  * @retval         none
  */
void chassis_init()
{
		PID_init(&motor_pid,PID_POSITION,motor_pid_PID, MOTOR_PID_MAX_OUT, MOTOR_PID_MAX_IOUT);
		PID_init(&angle_turn_pid,PID_POSITION,angle_pid_turn_PID, ANGLE_PID_TURN_MAX_OUT, ANGLE_PID_TURN_MAX_IOUT);
		PID_init(&move_pid[0],PID_DELTA,move_pid_PID, MOVE_PID_MAX_OUT, MOVE_PID_MAX_IOUT);
		PID_init(&move_pid[1],PID_DELTA,move_pid_PID, MOVE_PID_MAX_OUT, MOVE_PID_MAX_IOUT);
		PID_init(&move_pid[2],PID_DELTA,move_pid_PID, MOVE_PID_MAX_OUT, MOVE_PID_MAX_IOUT);
		PID_init(&move_pid[3],PID_DELTA,move_pid_PID, MOVE_PID_MAX_OUT, MOVE_PID_MAX_IOUT);
		PID_init(&position_pid,PID_POSITION,position_pid_PID, POSITION_PID_MAX_OUT, POSITION_PID_MAX_IOUT);
		PID_init(&position_pid_,PID_POSITION,position_pid_PID_, POSITION_PID_MAX_OUT, POSITION_PID_MAX_IOUT);
		chassis_move.motor_speed_pid = motor_pid;
		chassis_move.angle_turn_pid = angle_turn_pid;
		chassis_move.angle_straight_pid = angle_straight_pid;
		chassis_move.move_pid = move_pid;
		chassis_move.chassis_mode = MOVE_TO_RULES;
		chassis_move.position_pid = position_pid;
		for(int i=0; i < 3; i++)
		{	
			chassis_move.chassis_angles[i] = 0.0;
		}
		chassis_move.chassis_target_angle = 0.0;
		chassis_move.chassis_real_angle = 0.0;
		chassis_move.vx_set = 0.0;
		chassis_move.vy_set = 0.0;
		chassis_move.wz_set = 0.0;
		chassis_move.vx_real = 0.0;
		chassis_move.vy_real = 0.0;
		chassis_move.wz_real = 0.0;
		chassis_move.chassis_RC = &rc_ctrl;
		for(int j=0; j < 4; j++)
		{
			chassis_move.motor[j] = &motor_chassis[j];
			chassis_move.motor[j]->given_current = 0;
			chassis_move.motor[j]->round_cnt = 0;
			chassis_move.motor[j]->total_angle = 0;
			chassis_move.speed_target[j] = 0.0;
		}
		chassis_move.x_move_dist = 0.0;
		chassis_move.y_move_dist = 0.0;
		chassis_move.x_target_dist = 0.0;
		chassis_move.y_target_dist = 0.0;
		chassis_move.tof_x_dist = 0.0;
		chassis_move.tof_y_dist = 0.0;
		chassis_move.pos[0] = 0.0;
		chassis_move.pos[1] = 0.0;
		chassis_move.target_pos[0] = 0.0;
		chassis_move.target_pos[1] = 0.0;
}

/*
	逆运动学：根据设定的xyz方向目标速度分解出各个轮子的速度
*/
void vector_to_mecanum_wheel_speed(fp32 vx_set, fp32 vy_set, fp32 wz_set, fp32 wheel_speed[4])
{
		wheel_speed[0] = -vx_set - vy_set + (WZ_SET_SCALE - 1.0f) * MOTOR_DISTANCE_TO_CENTER * wz_set;
    wheel_speed[1] = vx_set - vy_set + (WZ_SET_SCALE - 1.0f) * MOTOR_DISTANCE_TO_CENTER * wz_set;
    wheel_speed[2] = vx_set + vy_set + (-WZ_SET_SCALE - 1.0f) * MOTOR_DISTANCE_TO_CENTER * wz_set;
    wheel_speed[3] = -vx_set + vy_set + (-WZ_SET_SCALE - 1.0f) * MOTOR_DISTANCE_TO_CENTER * wz_set;
}

/*
	正运动学：根据各个轮子的速度求出车整体的速度
*/
void mecanum_wheel_speed_to_vector(motor_measure_t* motor_measure[], fp32 *vx_set, fp32 *vy_set, fp32 *wz_set)
{
	*wz_set = (motor_measure[0]->speed_rpm + motor_measure[1]->speed_rpm + motor_measure[2]->speed_rpm + motor_measure[3]->speed_rpm)/(2*((WZ_SET_SCALE - 1.0f) * MOTOR_DISTANCE_TO_CENTER + (-WZ_SET_SCALE - 1.0f) * MOTOR_DISTANCE_TO_CENTER)) * MOTOR_RPM_TO_VECTOR_SEN;
	*vx_set = (motor_measure[1]->speed_rpm - motor_measure[0]->speed_rpm)/2.0 * MOTOR_RPM_TO_VECTOR_SEN;
	*vy_set = (-motor_measure[0]->speed_rpm - motor_measure[1]->speed_rpm)/2.0 * MOTOR_RPM_TO_VECTOR_SEN; // + WZ_SET_SCALE * MOTOR_DISTANCE_TO_CENTER * (*wz_set))
}

/* 
	根据xy方向移动距离更新车的位置
	*/
fp32 x_dist, y_dist;

void update_position()
{

	x_dist = (chassis_move.motor[1]->total_angle - chassis_move.motor[0]->total_angle)/ MAX_ROUND / SLOW_SPEED_RATE /2.0 * WHEEL_D * PI;
	y_dist = (-chassis_move.motor[0]->total_angle - chassis_move.motor[1]->total_angle) / MAX_ROUND / SLOW_SPEED_RATE /2.0 * WHEEL_D * PI;
	// 计算相邻两个目标点运动过程中xy方向移动的距离
	chassis_move.x_move_dist = x_dist;
	chassis_move.y_move_dist = y_dist;
}

/*
	根据当前位置和目标位置设定移动速度
	*/
void set_vector()
{
	float delta_x, delta_y, vector_rate[2], vector[2];
	delta_x = chassis_move.x_target_dist - chassis_move.x_move_dist;		//0.6		-0.5
	delta_y = chassis_move.y_target_dist - chassis_move.y_move_dist;		//0.45	0

	if(fabs(delta_x) < X_MIN_LIMIT && fabs(delta_y) < Y_MIN_LIMIT)
	{
		chassis_move.vx_set = 0.0;
		chassis_move.vy_set = 0.0;
	}
	else
	{
		vector_rate[0] = delta_x / sqrt(delta_x*delta_x+ delta_y*delta_y);
		vector_rate[1] = delta_y / sqrt(delta_x*delta_x+ delta_y*delta_y);
		vector[0] = vector_rate[0] * VX_BASE; // x方向分速度
		vector[1] = vector_rate[1] * VY_BASE; // y方向分速度
		chassis_move.vx_set = vector[0];
		chassis_move.vy_set = vector[1];		
	}
}


fp32 dist1;
int reach_position()
{	
	int reach_flag = 0;

	dist1 = sqrt((chassis_move.x_target_dist - chassis_move.x_move_dist)*(chassis_move.x_target_dist - chassis_move.x_move_dist)+
	(chassis_move.y_target_dist - chassis_move.y_move_dist)*(chassis_move.y_target_dist - chassis_move.y_move_dist));
	// xy方向移动距离近似目标距离，判定到达目标点，停止运动
	if(chassis_move.x_move_dist == chassis_move.x_target_dist && chassis_move.y_move_dist == chassis_move.y_target_dist)
		reach_flag = 0;
	else if(dist1 < DIST_LIMIT)
	{
		reach_flag = 1;
	}
		return reach_flag;
}

void move_forward()
{
			vector_to_mecanum_wheel_speed(chassis_move.vx_set, chassis_move.vy_set, WZ_BASE, chassis_move.speed_target);
			for (int i=0; i<4; i++)
			{		
				chassis_move.motor[i]->given_current = (int)PID_calc(&chassis_move.move_pid[i], chassis_move.motor[i]->speed_rpm, chassis_move.speed_target[i]);		
				//chassis_move.motor[i]->given_current = (int)PID_calc(&chassis_move.move_pid[i], chassis_move.motor[i]->speed_rpm, speed_target[i]);		
			}
			CAN_cmd_chassis(chassis_move.motor[0]->given_current, chassis_move.motor[1]->given_current, chassis_move.motor[2]->given_current, chassis_move.motor[3]->given_current);
			chassis_move.pos[0] = chassis_move.target_pos[0];
			chassis_move.pos[1] = chassis_move.target_pos[1];
}
int32_t count=0;
void move_to_target_point(fp32 target_pos[2])
{
	fp32 x_target_dist, y_target_dist;
	// 设置目标位置坐标
	chassis_move.target_pos[0] = target_pos[0];
	chassis_move.target_pos[1] = target_pos[1];
	x_target_dist = chassis_move.target_pos[0] - chassis_move.pos[0];
	y_target_dist = chassis_move.target_pos[1] - chassis_move.pos[1];
	chassis_move.x_target_dist = x_target_dist * cos(-chassis_move.chassis_real_angle/180.0*PI) - y_target_dist * sin(-chassis_move.chassis_real_angle/180.0*PI);
	chassis_move.y_target_dist = x_target_dist * sin(-chassis_move.chassis_real_angle/180.0*PI) + y_target_dist * cos(-chassis_move.chassis_real_angle/180.0*PI);
	chassis_move.x_move_dist = 0;
	chassis_move.y_move_dist = 0;
	// 清空上次移动角度积分
	for(int i = 0; i < 4; i++)
		chassis_move.motor[i]->total_angle = 0;
	// 到达目标点后跳出循环
	while(reach_position() == 0)
	{
		count++;
		// 修改目标速度
		set_vector();
		// 更新车本次移动距离
		update_position();
	}
//	update_position();
	chassis_move.vx_set = 0.0;
	chassis_move.vy_set = 0.0;
	chassis_move.pos[0] += chassis_move.x_move_dist *  cos(-chassis_move.chassis_real_angle/180.0*PI) + chassis_move.y_move_dist * sin(-chassis_move.chassis_real_angle/180.0*PI);
	chassis_move.pos[1] += -chassis_move.x_move_dist * sin(-chassis_move.chassis_real_angle/180.0*PI) + chassis_move.y_move_dist * cos(-chassis_move.chassis_real_angle/180.0*PI);
	// 当前位置设为目标位置
	chassis_move.target_pos[0] = chassis_move.pos[0];
	chassis_move.target_pos[1] = chassis_move.pos[1];
}

extern int rotation_flag;
extern SemaphoreHandle_t Rotation_Handle;
fp32 angle_error=0;
/*
	原地旋转目标角度
	*/
void rotation(fp32 target_angle)
{
	BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
	xReturn = xSemaphoreTake(Rotation_Handle, portMAX_DELAY);
	chassis_move.chassis_target_angle = target_angle;
	angle_error = chassis_move.chassis_angles[2]-chassis_move.chassis_real_angle;
	if(pdTRUE == xReturn)
	{
		// 根据陀螺仪计算出整体角速度
		while(Dead_Limit(chassis_move.chassis_target_angle - chassis_move.chassis_angles[2]+angle_error, ANGLE_MAX_ERROR))
		{
			chassis_move.wz_set = PID_calc(&chassis_move.angle_turn_pid, chassis_move.chassis_angles[2]-angle_error, chassis_move.chassis_target_angle); 
		}
		chassis_move.chassis_real_angle = target_angle;
		chassis_move.wz_set = 0.0;
		rotation_flag = 0; 
		xSemaphoreGive(Rotation_Handle);
	}
}