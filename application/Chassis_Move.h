#ifndef CHASSIS_MOVE_H
#define CHASSIS_MOVE_H

#include "struct_typedef.h"
#include "remote_control.h"
#include "pid.h"
#include "math.h"
/* -----------------------Remote Controller Parameters ------------------------------------- */
#define CHASSIS_TASK_INIT_TIME 357 // 任务开启前空闲一段时间

#define RC_X_CHANNEL 1 // 前后方向
#define RC_Y_CHANNEL 0 // 左右方向
#define RC_WZ_CHANNEL 2 // 左转右转方向
#define RC_DEADLINE 10  // 遥杆死区
#define RC_VX_SEN 0.0005f // 遥控器前进摇杆（max 660）转化成车体前进速度（m/s）的比例
#define RC_VY_SEN 0.0002f // rocker value (max 660) change to horizontal speed (m/s)
#define RC_WZ_SEN 0.005f // 不跟随云台的时候 遥控器的yaw遥杆（max 660）转化成车体旋转速度的比例
#define MOTOR_RPM_TO_VECTOR_SEN 0.000116355 // rpm->m/s
#define WZ_SET_SCALE 0.1f
#define MPS_TO_RPM 8594.3671f // 遥控目标速度转换成电机转速的比例
#define MOTOR_DISTANCE_TO_CENTER 0.11f

#define turn_drive_flag 1
#define straight_drive_flag 2
#define without_current_flag 3
#define no_move_flag 4

#define MOTOR_PID_KP 2.4f //motor pid
#define MOTOR_PID_KI 0.003f    
#define MOTOR_PID_KD 0.3f    
#define MOTOR_PID_MAX_OUT  5000.0f 
#define MOTOR_PID_MAX_IOUT 4800.0f  //motor I max

#define ANGLE_PID_TURN_KP 0.01f //angle pid
#define ANGLE_PID_TURN_KI 0.001f   
#define ANGLE_PID_TURN_KD 0.0f    
#define ANGLE_PID_TURN_MAX_OUT 10.0f 
#define ANGLE_PID_TURN_MAX_IOUT 1.0f  //angle I max

#define MOVE_PID_KP 0.6f //motor pid
#define MOVE_PID_KI 0.008f    
#define MOVE_PID_KD 0.3f    
#define MOVE_PID_MAX_OUT  3000.0f 
#define MOVE_PID_MAX_IOUT 2000.0f  //motor I max

#define POSITION_PID_KP 0.1f //motor pid
#define POSITION_PID_KI 0.005f    
#define POSITION_PID_KD 0.0f    
#define POSITION_PID_MAX_OUT  0.1f 
#define POSITION_PID_MAX_IOUT 0.05f  //motor I max

#define GRID 30.0f // 一个格子是30cm
#define VX_BASE 0.5f // x方向基本移动速度(m/s)
#define VY_BASE 0.5f // y方向基本移动速度(m/s)
#define WZ_BASE 0.0f
#define WHEEL_D 0.08 // 轮子直径(m)
#define SLOW_SPEED_RATE 36 // 减速比
#define MAX_ROUND 8192 
#define ANGLE_TARGET 0.0  // 全向移动目标角度
#define ROTATION_ANGLE 90.0 // 目标旋转角度

#define X_RATE 24000.45
#define Y_RATE 24288.32

#define X_MAX_LIMIT 0.03
#define Y_MAX_LIMIT 0.10
#define X_MIN_LIMIT 0.002
#define Y_MIN_LIMIT 0.002
#define DIST_LIMIT 0.003

// can receiver motor data
typedef struct
{
    uint16_t ecd; // 当前转矩
    int16_t speed_rpm;
    int16_t given_current; // 给定电流值
		int16_t real_current; // 目标电流值
    uint8_t temperate;
    int16_t last_ecd; // 上次转矩
		int32_t round_cnt;
		fp32 total_angle;
} motor_measure_t;

typedef enum
{
	MOVE_TO_RULES,
	MOVE_TO_RC,
}chassis_move_mode;

typedef struct
{
	const RC_ctrl_t *chassis_RC;        //底盘使用的遥控器指针
	fp32 chassis_angles[3];             //底盘三个角度
	fp32 chassis_target_angle;					//控制车旋转目标角度
	fp32 chassis_real_angle;						//记录每次角度修正后车的实际角度
	chassis_move_mode chassis_mode;     //底盘移动模式
	pid_type_def motor_speed_pid;       //速度环PID
	pid_type_def angle_turn_pid;        //角度环PID
	pid_type_def angle_straight_pid;
	pid_type_def *move_pid;
	pid_type_def position_pid;					//位置PID
	fp32 vx_set;												//x方向目标速度
	fp32 vy_set;												//y方向目标速度
	fp32 wz_set;                        //旋转角速度
	fp32 vx_real;
	fp32 vy_real;
	fp32 wz_real;
	motor_measure_t* motor[4];						//电机测量变量
	fp32 speed_target[4];								//四个轮子的目标速度
	fp32 x_move_dist;										//x方向移动距离（积分得出来的）
	fp32 y_move_dist;										//y方向移动距离
	fp32 x_target_dist;									//x方向目标移动距离
	fp32 y_target_dist;									//y方向目标移动距离
	fp32 tof_x_dist;										//x方向离边界的距离
	fp32 tof_y_dist;										//y方向距离边界的距离
	fp32 pos[2];												//在场地中的坐标位置
	fp32 target_pos[2];									//希望移动到的目标位置
}chassis_move_t;

#define ABS(x)	( (x>0) ? (x) : (-x) )
#define sqrt1(x, y)   sqrt(x*x + y*y)

void chassis_init();
void set_vector();
void vector_to_mecanum_wheel_speed(fp32 vx_set, fp32 vy_set, fp32 wz_set, fp32 wheel_speed[4]);
void mecanum_wheel_speed_to_vector(motor_measure_t* motor_measure[], fp32 *vx_set, fp32 *vy_set, fp32 *wz_set);
int reach_position();
void move_forward();
void update_position();
void move_to_target_point(fp32 target_pos[2]);
void rotation(fp32 target_angle);
#endif