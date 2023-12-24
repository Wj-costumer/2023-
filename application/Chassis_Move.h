#ifndef CHASSIS_MOVE_H
#define CHASSIS_MOVE_H

#include "struct_typedef.h"
#include "remote_control.h"
#include "pid.h"
#include "math.h"
/* -----------------------Remote Controller Parameters ------------------------------------- */
#define CHASSIS_TASK_INIT_TIME 357 // ������ǰ����һ��ʱ��

#define RC_X_CHANNEL 1 // ǰ����
#define RC_Y_CHANNEL 0 // ���ҷ���
#define RC_WZ_CHANNEL 2 // ��ת��ת����
#define RC_DEADLINE 10  // ң������
#define RC_VX_SEN 0.0005f // ң����ǰ��ҡ�ˣ�max 660��ת���ɳ���ǰ���ٶȣ�m/s���ı���
#define RC_VY_SEN 0.0002f // rocker value (max 660) change to horizontal speed (m/s)
#define RC_WZ_SEN 0.005f // ��������̨��ʱ�� ң������yawң�ˣ�max 660��ת���ɳ�����ת�ٶȵı���
#define MOTOR_RPM_TO_VECTOR_SEN 0.000116355 // rpm->m/s
#define WZ_SET_SCALE 0.1f
#define MPS_TO_RPM 8594.3671f // ң��Ŀ���ٶ�ת���ɵ��ת�ٵı���
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

#define GRID 30.0f // һ��������30cm
#define VX_BASE 0.5f // x��������ƶ��ٶ�(m/s)
#define VY_BASE 0.5f // y��������ƶ��ٶ�(m/s)
#define WZ_BASE 0.0f
#define WHEEL_D 0.08 // ����ֱ��(m)
#define SLOW_SPEED_RATE 36 // ���ٱ�
#define MAX_ROUND 8192 
#define ANGLE_TARGET 0.0  // ȫ���ƶ�Ŀ��Ƕ�
#define ROTATION_ANGLE 90.0 // Ŀ����ת�Ƕ�

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
    uint16_t ecd; // ��ǰת��
    int16_t speed_rpm;
    int16_t given_current; // ��������ֵ
		int16_t real_current; // Ŀ�����ֵ
    uint8_t temperate;
    int16_t last_ecd; // �ϴ�ת��
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
	const RC_ctrl_t *chassis_RC;        //����ʹ�õ�ң����ָ��
	fp32 chassis_angles[3];             //���������Ƕ�
	fp32 chassis_target_angle;					//���Ƴ���תĿ��Ƕ�
	fp32 chassis_real_angle;						//��¼ÿ�νǶ������󳵵�ʵ�ʽǶ�
	chassis_move_mode chassis_mode;     //�����ƶ�ģʽ
	pid_type_def motor_speed_pid;       //�ٶȻ�PID
	pid_type_def angle_turn_pid;        //�ǶȻ�PID
	pid_type_def angle_straight_pid;
	pid_type_def *move_pid;
	pid_type_def position_pid;					//λ��PID
	fp32 vx_set;												//x����Ŀ���ٶ�
	fp32 vy_set;												//y����Ŀ���ٶ�
	fp32 wz_set;                        //��ת���ٶ�
	fp32 vx_real;
	fp32 vy_real;
	fp32 wz_real;
	motor_measure_t* motor[4];						//�����������
	fp32 speed_target[4];								//�ĸ����ӵ�Ŀ���ٶ�
	fp32 x_move_dist;										//x�����ƶ����루���ֵó����ģ�
	fp32 y_move_dist;										//y�����ƶ�����
	fp32 x_target_dist;									//x����Ŀ���ƶ�����
	fp32 y_target_dist;									//y����Ŀ���ƶ�����
	fp32 tof_x_dist;										//x������߽�ľ���
	fp32 tof_y_dist;										//y�������߽�ľ���
	fp32 pos[2];												//�ڳ����е�����λ��
	fp32 target_pos[2];									//ϣ���ƶ�����Ŀ��λ��
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