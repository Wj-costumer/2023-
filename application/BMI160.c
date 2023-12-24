#include "BMI160.h"
#include "QCS.h"
#include "Data_Exchange.h"

IMU_quat_Recieved_t IMU_quat_R;

void IMU_Recieve_Translate(uint32_t identifier, uint8_t *rx_data, IMU_quat_Recieved_t *IMU_quat_Recieved)
{
	uint32_t OffSet=0;
	static uint32_t OffSet_Last;
	OffSet = identifier - QCS_AB_ID;
	U8_To_Float(rx_data,IMU_quat_Recieved->Recieve_Buff+2*OffSet,8);
	if(OffSet_Last == 0 && OffSet==1)
	{
		IMU_quat_Recieved->Data_Syn=1;
		IMU_quat_Recieved->Quaternion[0] = IMU_quat_Recieved->Recieve_Buff[0];
		IMU_quat_Recieved->Quaternion[1] = IMU_quat_Recieved->Recieve_Buff[1];
		IMU_quat_Recieved->Quaternion[2] = IMU_quat_Recieved->Recieve_Buff[2];
		IMU_quat_Recieved->Quaternion[3] = IMU_quat_Recieved->Recieve_Buff[3];
	}
	else{IMU_quat_Recieved->Data_Syn=0;}
	OffSet_Last = OffSet;
}

void Quaternion_to_Angles(IMU_quat_Recieved_t IMU_Data, fp32 *angles)
{
		QCS_GetRollAngel(IMU_Data.Quaternion, &angles[0]);
		QCS_GetPitchAngel(IMU_Data.Quaternion, &angles[1]);
		QCS_GetYawAngel(IMU_Data.Quaternion, &angles[2]);
		angles[0] *= RAD_TO_ANGLE;
		angles[1] *= RAD_TO_ANGLE;
		angles[2] *= RAD_TO_ANGLE;
}