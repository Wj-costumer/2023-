/**
  ****************************(C) COPYRIGHT 2019 DJI****************************
  * @file       test_task.c/h
  * @brief      buzzer warning task.��������������
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
  * @brief          test����
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

#define pos_length 1 // λ���������鳤��
fp32 target_pos[pos_length][2] = {{0.6, 0.1}};	// pos[0] ǰ������ pos[1] ����ƽ�Ʒ���

int point_goal = 0; // ��¼�����Ŀ������
extern fp32 speed_target[4];

extern uint8_t RxBuffer_1;
extern uint8_t RxBuffer_2;
extern uint8_t Size;
extern uint8_t Rx_Size;
extern TOF_t TOF;
extern int find_QRCODE_flag;
extern int find_objects_flag;

int set_vector_flag = 1; // �ж��Ƿ����Ŀ���
int move_flag = 0;	// �ж�Ŀ�������Ƿ�ȫ��������
int origin_angle_flag = 1; // ��¼��ʼ�˶�ʱ�����ǳ�ʼ�Ƕ�
int rotation_flag = 0; // �ж��Ƿ���ת

fp32 origin_error;
extern uint8_t receive[4];
#define	PCA9698	0x40       //PCA9698��ַ 


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

