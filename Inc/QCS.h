/*****************************************************************
**                qianwan's Code
**---------------------------------------------------------------
** ��Ŀ���ƣ�   QCS��Ԫ����������ϵͳ
** ��    �ڣ�   2022-04-12
** ��    �ߣ�   qianwan
** ��    ����   1.2
**---------------------------------------------------------------
** �� �� ����   QCS.h
** ��������   CMSIS 5.8.0
** �ļ����䣺   Ĭ������ϵ��������
*****************************************************************/

#ifndef QCS_H
#define QCS_H

/*---------------------INCLUDES----------------------*/
#include "main.h"
#include "arm_math.h"

/*---------------------DEFINES----------------------*/
#ifndef PI
  #define PI               		3.14159265358979f
#endif

#ifndef PI_Half
  #define PI_Half               1.57079632679489f
#endif

#ifndef PI_Quart
  #define PI_Quart              0.78539816339744f
#endif

#ifndef PI_Eighth
  #define PI_Eighth             0.39269908169872f
#endif

#ifndef PI_Double
  #define PI_Double             6.28318530717958f
#endif

/*---------------------DECLARES----------------------*/
extern float QCS_AHRSq[4];
extern float QCS_IMU_Degree[3];
void QCS_Rotate(float *Q_input,float *Q_output,float *Vector,float angel_Radian);
void QCS_Rotate_Pure(float *Q_input,float *Q_output,float *Vector,float angel_Radian);
void QCS_GetRollAngel(float Q_input[4],float *Roll);
void QCS_GetYawAngel(float Q_input[4],float *Yaw);
void QCS_GetPitchAngel(float Q_input[4],float *Pitch);
void QCS_GetErrorQ(float Q_Now[4],float Q_Target[4],float Q_output[4]);
void QCS_CorrectAHRSq(float q[4],float AHRSQ[4],float YC);
void QCS_CalculatYP(float TargetQ[4],float AHRSQ[4],float *EYaw,float *EPitch);
void QCS_init_data(void);
void QCS_AHRS_update(float gyro[], float accel[],float YC);
void QCS_Show_Degree(float AHRSQ[4], float IMU_Degree[3]);

#endif
