/**
  ****************************(C) COPYRIGHT 2019 DJI****************************
  * @file       can_receive.c/h
  * @brief      there is CAN interrupt function  to receive motor data,
  *             and CAN send function to send motor current to control motor.
  *             这里是CAN中断接收函数，接收电机数据,CAN发送函数发送电机电流控制电机.
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Dec-26-2018     RM              1. done
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2019 DJI****************************
  */

#ifndef CAN_RECEIVE_H
#define CAN_RECEIVE_H

#include "struct_typedef.h"
#include "Chassis_Move.h"
#define CHASSIS_CAN hcan1
#define C_CAN hcan2

/* CAN send and receive ID */
typedef enum
{
    CAN_CHASSIS_ALL_ID = 0x200,
    CAN_2006_M1_ID = 0x201,
    CAN_2006_M2_ID = 0x202,
    CAN_2006_M3_ID = 0x203,
    CAN_2006_M4_ID = 0x204,
    CAN_TRIGGER_MOTOR_ID = 0x207,
		CAN_GIMBAL_ALL_ID = 0x1FF,
		CAN_Light1_ID = 0x208,
		CAN_Light2_ID =0x209,
		CAN_angle_ID = 0x210,	
		CAN_MASTER_ID =0X211,
} can_msg_id_e;

/**
  * @brief          send CAN packet of ID 0x700, it will set chassis motor 2006 to quick ID setting
  * @param[in]      none
  * @retval         none
  */
/**
  * @brief          发送ID为0x700的CAN包,它会设置2006电机进入快速设置ID
  * @param[in]      none
  * @retval         none
  */
extern void CAN_cmd_chassis_reset_ID(void);

/**
  * @brief          send control current of motor (0x201, 0x202, 0x203, 0x204)
  * @param[in]      motor1: (0x201) 2006 motor control current, range [-16384,16384] 
  * @param[in]      motor2: (0x202) 2006 motor control current, range [-16384,16384] 
  * @param[in]      motor3: (0x203) 2006 motor control current, range [-16384,16384] 
  * @param[in]      motor4: (0x204) 2006 motor control current, range [-16384,16384] 
  * @retval         none
  */
/**
  * @brief          发送电机控制电流(0x201,0x202,0x203,0x204)
  * @param[in]      motor1: (0x201) 2006电机控制电流, 范围 [-16384,16384]
  * @param[in]      motor2: (0x202) 2006电机控制电流, 范围 [-16384,16384]
  * @param[in]      motor3: (0x203) 2006电机控制电流, 范围 [-16384,16384]
  * @param[in]      motor4: (0x204) 2006电机控制电流, 范围 [-16384,16384]
  * @retval         none
  */
extern void CAN_cmd_chassis(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);

/**
  * @brief          return the chassis 2006 motor data point
  * @param[in]      i: motor number,range [0,3]
  * @retval         motor data point
  */
/**
  * @brief          返回底盘电机 2006电机数据指针
  * @param[in]      i: 电机编号,范围[0,3]
  * @retval         电机数据指针
  */
extern const motor_measure_t *get_chassis_motor_measure_point(uint8_t i);
void C1_can_transmit( uint8_t data);
#endif
