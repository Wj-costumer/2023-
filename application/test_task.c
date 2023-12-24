/**
  ****************************(C) COPYRIGHT 2019 DJI****************************
  * @file       test_task.c/h
  * @brief      buzzer warning task.蜂鸣器报警任务
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Nov-11-2019     RM              1. done
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2019 DJI****************************
  */

#include "test_task.h"
#include "main.h"
#include "cmsis_os.h"
#include "CAN_receive.h"
#include "usart.h"
#include "can.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"
#include "bsp_delay.h"
#include "bsp_can.h"
#include "pid.h"
#include "filter.h"
#include "drive.h"
#include "remote_control.h"
#include "Gui.h"
#include "Lcd_Driver.h"
#include "Chassis_Move.h"
#include "TOF.h"
#include "math.h"
#include "io_read.h"
#include "angle_correct.h"
#include "follow_line.h"

/**
  * @brief          test task
  * @param[in]      pvParameters: NULL
  * @retval         none
  */
/**
  * @brief          test任务
  * @param[in]      pvParameters: NULL
  * @retval         none
  */
//extern float angles[3];
//extern int rc_flag;
//extern RC_ctrl_t rc_ctrl;
//float angle_target=0;
extern int start_flag;
extern chassis_move_t chassis_move;
extern fp32 origin_error;

#define pos_length 1 // 位置坐标数组长度
fp32 target_pos[pos_length][2] = {{0.6, 0.1}};	// pos[0] 前进方向 pos[1] 左右平移方向

int point_goal = 0; // 记录到达的目标点个数
extern fp32 speed_target[4];

extern uint8_t RxBuffer_1;
extern uint8_t RxBuffer_2;
extern uint8_t Size;
extern uint8_t Rx_Size;
extern TOF_t TOF;
extern int find_QRCODE_flag;
extern int find_objects_flag;

int set_vector_flag = 1; // 判断是否更新目标点
int move_flag = 0;	// 判断目标坐标是否全部运行完
int origin_angle_flag = 1; // 记录开始运动时陀螺仪初始角度
int rotation_flag = 0; // 判断是否旋转

fp32 origin_error;
extern uint8_t receive[4];
#define	PCA9698	0x40       //PCA9698地址 


void test_task(void const * argument)
{
		//wait a time
    osDelay(CHASSIS_TASK_INIT_TIME);
		chassis_init();
		io_read_init();
		count_line_init();
		while(1)
		{
			io_read();
			read_pin();
			osDelay(1); 
		}
}

