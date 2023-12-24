#ifndef _ARM_Control_H_
#define _ARM_Control_H_

#include "stm32f4xx_hal.h"
#include "main.h"
#include "tim.h"

//对应角度的PWM输出值 
#define Angle0 500
#define Angle67_5 1000
#define Angle135 1500
#define Angle202_5 2000
#define Angle270 2500

/*
	舵机接线：
	Steer_Eng1:底盘舵机
	...
	Steer_Eng5:夹子舵机
	

*/
//舵机对应的定时器
#define Steer_Eng1	&htim1
#define Steer_Eng2	&htim1
#define Steer_Eng3  &htim1
#define Steer_Eng4  &htim8
#define Steer_Eng5  &htim8
#define Steer_Eng6  &htim1

//舵机对应的输出通道
#define Steer_Eng1_CH	TIM_CHANNEL_1
#define Steer_Eng2_CH	TIM_CHANNEL_3
#define Steer_Eng3_CH	TIM_CHANNEL_4
#define Steer_Eng4_CH	TIM_CHANNEL_1
#define Steer_Eng5_CH	TIM_CHANNEL_2
#define Steer_Eng6_CH	TIM_CHANNEL_2
//PWM输出限幅
#define PWM_Max 2500
#define PWM_Min 500

/////////////////////////////////////

#define SPEED_MAX 20
#define ACC_MAX 1

typedef struct{
	unsigned char accelerate_flag;
	int delta_val;
	int valtarget;
	int vallast;
	int val_zero;
	int val_output;
	int delta_output;
	int result;
	int speed;
	double acc;
	int speed_max;
	double acc_max;
	short Ta;
	short Tm;
	short T;
	
} accelerate_param_t;

//PWM舵机
typedef struct
{	
  int duty_target;
  int duty;

  accelerate_param_t accelerate;	
  
} pwm_servo_control_param_t;

typedef struct{
	pwm_servo_control_param_t first; // 云台舵机
	pwm_servo_control_param_t second; // 底座舵机
	pwm_servo_control_param_t third; // 三号舵机
	pwm_servo_control_param_t forth; // 四号舵机
	pwm_servo_control_param_t fifth; // 五号舵机
}mechine_arm_param_t;

void pwm_test();
void catch_upper_layer();
void catch_bottom_layer();
void catch_according_to_order(uint8_t pos);
void get_object_out(uint8_t pos);
void put_down_object(uint8_t pos);
void catch_up_object(uint8_t pos);
void operator_in_area_2();
void operator_in_area_3();
void operator_in_area_2_2();
void operator_in_area_3_2();
void control_servo(uint8_t id, uint16_t target_pwm);
void control_in_turn(uint8_t id_1, uint8_t id_2, uint8_t id_3, int target_pwm_1, uint16_t target_pwm_2, uint16_t target_pwm_3);
void put_down_object_area3(uint8_t pos);
void change_234_speed(float second_speed_max,float second_acc_max,float third_speed_max,float third_acc_max,float forth_speed_max,float forth_acc_max);
void arm_speed_reset(void);
void machine_arm_init(void);
void machine_accelerate_init(int arm_num);
void servo_change_entrance(pwm_servo_control_param_t *arm_x);
void servo_target_change_detect(void);
void servo_target_change(void);
void accelerate_pretreat(pwm_servo_control_param_t *arm_x);
void accelerate_deal(pwm_servo_control_param_t *arm_x);
void pwm_control(int pwm1, int pwm2, int pwm3, int pwm4, int pwm5);
void overlay_object(uint8_t pos);
void put_object_back(uint8_t pos);
/***************决赛*******************/
void catch_up_object_area3_before(uint8_t pos);
void catch_up_object_area3_behind(uint8_t pos);
void put_down_object_area3_behind(uint8_t pos);
void put_down_object_in_area1(uint8_t pos);
void overlay_object_in_area1(uint8_t pos);
void put_down_object_final(uint8_t pos);
void catch_up_object_final(uint8_t pos);
void operator_in_area_1_firstfloor_finalgame();
void operator_in_area_1_secondfloor_finalgame();
void catch_object_in_Inventory_behind_area(void);
void catch_object_in_Inventory_before_area(void);//物品区抓取
void operator_in_area_2_finalgame();
#endif
