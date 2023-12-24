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

#include "CAN_receive.h"
#include "main.h"
#include "Data_Exchange.h"
#include "drive.h"
#include "BMI160.h"
#include "Chassis_Move.h"

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;
extern IMU_quat_Recieved_t IMU_quat_R;
extern chassis_move_t chassis_move;
extern motor_measure_t motor_chassis[4];

//motor data read
#define get_motor_measure(ptr, data)                                    \
    {                                                                   \
        (ptr)->last_ecd = (ptr)->ecd;                                   \
        (ptr)->ecd = (uint16_t)((data)[0] << 8 | (data)[1]);            \
        (ptr)->speed_rpm = (uint16_t)((data)[2] << 8 | (data)[3]);      \
        (ptr)->real_current = (uint16_t)((data)[4] << 8 | (data)[5]);  \
        (ptr)->temperate = (data)[6];                                   \
		}

static CAN_TxHeaderTypeDef  gimbal_tx_message;
static uint8_t              gimbal_can_send_data[8];
static CAN_TxHeaderTypeDef  chassis_tx_message;
static uint8_t              chassis_can_send_data[8];
uint8_t angle_mode[2];
static CAN_TxHeaderTypeDef  C_tx_message;
static uint8_t              C_can_send_data[8];
/**
  * @brief          hal CAN fifo call back, receive motor data
  * @param[in]      hcan, the point to CAN handle
  * @retval         none
  */
/**
  * @brief          hal库CAN回调函数,接收电机数据
  * @param[in]      hcan:CAN句柄指针
  * @retval         none
  */
		int32_t coutn3=0;
		void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef rx_header;
    uint8_t rx_data[8];

    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data);

    switch (rx_header.StdId)
    {
				case CAN_2006_M1_ID:
        case CAN_2006_M2_ID:
        case CAN_2006_M3_ID:
        case CAN_2006_M4_ID:
				case CAN_TRIGGER_MOTOR_ID:
				{
						static uint8_t i = 0;
            //get motor id
            i = rx_header.StdId - CAN_2006_M1_ID;
            get_motor_measure(chassis_move.motor[i], rx_data);
						mecanum_wheel_speed_to_vector(chassis_move.motor, &chassis_move.vx_real, &chassis_move.vy_real, &chassis_move.wz_real); // 根据每个轮子的速度 更新整体车的速度
						get_total_angle_and_dist_xy(chassis_move.motor[i]); // 更新角度积分和移动距离分量
						break;
				}
				case QCS_AB_ID:
				case QCS_CD_ID:	
				{
					IMU_Recieve_Translate(rx_header.StdId,rx_data,&IMU_quat_R); // 接收陀螺仪数据
					Quaternion_to_Angles(IMU_quat_R, chassis_move.chassis_angles); // 四元数转换成欧拉角
					break;
				}
//				case CAN_Light1_ID:
//				{
//					if(rx_data[4]=='d')
//						U8_To_Float(rx_data,&TOF.x_dist_1,4);					
//					
//				}
//				case CAN_Light2_ID:
//				{
//					if(rx_data[4]=='d')
//						U8_To_Float(rx_data,&TOF.x_dist_2,4);					
//				}
				case CAN_angle_ID:
				{
					if(rx_data[2]=='d')
					{	
						angle_mode[0]=rx_data[0];
						angle_mode[1]=rx_data[1];					
					}
				}
        default:
        {
            break;
        }
    }
}

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
void CAN_cmd_chassis_reset_ID(void)
{
    uint32_t send_mail_box;
    chassis_tx_message.StdId = 0x700;
    chassis_tx_message.IDE = CAN_ID_STD;
    chassis_tx_message.RTR = CAN_RTR_DATA;
    chassis_tx_message.DLC = 0x08;
    chassis_can_send_data[0] = 0;
    chassis_can_send_data[1] = 0;
    chassis_can_send_data[2] = 0;
    chassis_can_send_data[3] = 0;
    chassis_can_send_data[4] = 0;
    chassis_can_send_data[5] = 0;
    chassis_can_send_data[6] = 0;
    chassis_can_send_data[7] = 0;

    HAL_CAN_AddTxMessage(&CHASSIS_CAN, &chassis_tx_message, chassis_can_send_data, &send_mail_box);
}


/**
  * @brief          send control current of motor (0x201, 0x202, 0x203, 0x204)
  * @param[in]      motor1: (0x201) 2006 motor control current, range [-10000,10000] 
  * @param[in]      motor2: (0x202) 2006 motor control current, range [-10000,10000] 
  * @param[in]      motor3: (0x203) 2006 motor control current, range [-10000,10000] 
  * @param[in]      motor4: (0x204) 2006 motor control current, range [-10000,10000] 
  * @retval         none
  */
/**
  * @brief          发送电机控制电流(0x201,0x202,0x203,0x204)
  * @param[in]      motor1: (0x201) 2006电机控制电流, 范围 [-10000,10000]
  * @param[in]      motor2: (0x202) 2006电机控制电流, 范围 [-10000,10000]
  * @param[in]      motor3: (0x203) 2006电机控制电流, 范围 [-10000,10000]
  * @param[in]      motor4: (0x204) 2006电机控制电流, 范围 [-10000,10000]
  * @retval         none
  */
void CAN_cmd_chassis(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4)
{
    uint32_t send_mail_box;
    chassis_tx_message.StdId = CAN_CHASSIS_ALL_ID;
    chassis_tx_message.IDE = CAN_ID_STD;
    chassis_tx_message.RTR = CAN_RTR_DATA;
    chassis_tx_message.DLC = 0x08;
    chassis_can_send_data[0] = motor1 >> 8;
    chassis_can_send_data[1] = motor1;
    chassis_can_send_data[2] = motor2 >> 8;
    chassis_can_send_data[3] = motor2;
    chassis_can_send_data[4] = motor3 >> 8;
    chassis_can_send_data[5] = motor3;
    chassis_can_send_data[6] = motor4 >> 8;
    chassis_can_send_data[7] = motor4;

    HAL_CAN_AddTxMessage(&CHASSIS_CAN, &chassis_tx_message, chassis_can_send_data, &send_mail_box);
}


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
const motor_measure_t *get_chassis_motor_measure_point(uint8_t i)
{
    return &motor_chassis[(i & 0x03)];
}

/**
  * @brief          C_can_transmit
  * @param[in]      data //发送的信息
  * @retval         none
  */
/**
  * @brief          C板之间通过can通信
  * @param[in]      发送信息
  * @retval         无
  */

void C1_can_transmit( uint8_t data)//length<24
{
	
	  uint32_t send_mail_box;
    C_tx_message.StdId = CAN_MASTER_ID;
    C_tx_message.IDE = CAN_ID_STD;
    C_tx_message.RTR = CAN_RTR_DATA;
    C_tx_message.DLC = 0x08;
	  C_can_send_data[0] = data;
	  C_can_send_data[1] = 'd';	
	  C_can_send_data[2] = '0';	
	  C_can_send_data[3] = '0';		
	  C_can_send_data[4] = '0';			
	  C_can_send_data[5] = '0';	
	  C_can_send_data[6] = '0';	
	  C_can_send_data[7] = '0';		
		HAL_CAN_AddTxMessage(&C_CAN, &C_tx_message, C_can_send_data, &send_mail_box);	
}