#ifndef BMI_IMU_H
#define BMI_IMU_H

#include "struct_typedef.h"

#define QCS_AB_ID 0x322
#define QCS_CD_ID 0x323
#define RAD_TO_ANGLE 57.29578049

typedef struct
{
	float Quaternion[4];
	float Recieve_Buff[4];
	uint8_t Data_Syn;
}IMU_quat_Recieved_t;


void IMU_Recieve_Translate(uint32_t identifier, uint8_t *rx_data, IMU_quat_Recieved_t *IMU_quat_Recieved);
void Quaternion_to_Angles(IMU_quat_Recieved_t IMU_Data, fp32 *angles);
#endif