#include "arm_control.h"
#include "Chassis_Move.h"
#include "cmsis_os.h"
#include "math.h"
#define TIM_CHANNEL_BASE 0x00000000U
#define claw_close			850
#define claw_open				550
//int sevor_pwm[7] = {1200,1500,1090,2295,2400,1120,1820};	

//int sevor_pwm[7] = {1200, 1500, 800, 880, 1200, 1000, 1820};
//int sevor_pwm_last[7] = {1200,1500,800, 880, 1200,1000,1820};	
//int sevor_pwm_final[7] = {1200,1500,800, 880, 1200,1000,1820}; 
int servo_pwm[5] = {1820, 800, 880, 1200, claw_open};

int delay_time = 10;
int delay_time_turn = 2;
int delay_time_catch = 1;
int step = 1;
int catch_flag = 0;
int	turn_flag=1;
mechine_arm_param_t machine_arm;
extern uint8_t QRCODE[6];
extern uint8_t COLOR[6];
int get_out_order[3];

/*
	µ¥¶À¿ØÖÆÄ³Ò»¸ö¹Ø½ÚÔË¶¯µ½Ä¿±ê½Ç¶È
	*/
void control_servo(uint8_t id, uint16_t target_pwm)
{
	int k = 0;
	TIM_HandleTypeDef htim;
	uint8_t channel;
	switch(id){
		case 0: // ÔÆÌ¨¶æ»ú
		{
			htim = htim1;
			channel = 4;
			machine_arm.first.duty = target_pwm;
			machine_arm.first.duty_target = target_pwm;
			break;
		}
		case 1: // µ××ù¶æ»ú
		{
			htim = htim1;
			channel = 8;
			machine_arm.second.duty = target_pwm;
			machine_arm.second.duty_target = target_pwm;
			break;
		}
		case 2: // 3ºÅ¶æ»ú
		{
			htim = htim1;
			channel = 12;
			machine_arm.third.duty = target_pwm;
			machine_arm.third.duty_target = target_pwm;
			break;
		}
		case 3: // 4ºÅ¶æ»ú
		{
			htim = htim8;
			channel = 0;
			machine_arm.forth.duty = target_pwm;
			machine_arm.forth.duty_target = target_pwm;
			break;
		}
		case 4: // ×¦×Ó¶æ»ú
		{
			htim = htim8;
			channel = 4;
			machine_arm.fifth.duty = target_pwm;
			machine_arm.fifth.duty_target = target_pwm;
			break;
		}
	}
	__HAL_TIM_SET_COMPARE(&htim, TIM_CHANNEL_BASE+channel, target_pwm); // Ö±½Ó¸øPWM
}

/*
	½»Ìæ¿ØÖÆÈý¸ö¹Ø½Ú
	*/
void control_in_turn(uint8_t id_1, uint8_t id_2, uint8_t id_3, int target_pwm_1, uint16_t target_pwm_2, uint16_t target_pwm_3)
{
//	sevor_pwm[id_1] = target_pwm_1;
//	sevor_pwm[id_2] = target_pwm_2;
//	sevor_pwm[id_3] = target_pwm_3;
//	float step[7],pwm[7],pwm_last[7];
//	step[id_1]=fabs((float)(sevor_pwm[id_1]-sevor_pwm_last[id_1])/100);
//	step[id_2]=fabs((float)(sevor_pwm[id_2]-sevor_pwm_last[id_2])/100);
//	step[id_3]=fabs((float)(sevor_pwm[id_3]-sevor_pwm_last[id_3])/100);	
//	pwm[id_1]=(float)sevor_pwm[id_1];
//	pwm[id_2]=(float)sevor_pwm[id_2];
//	pwm[id_3]=(float)sevor_pwm[id_3];	
//	
//	pwm_last[id_1]=(float)sevor_pwm_last[id_1];
//	pwm_last[id_2]=(float)sevor_pwm_last[id_2];
//	pwm_last[id_3]=(float)sevor_pwm_last[id_3];		
//	while(fabs(pwm[id_1]-pwm_last[id_1]) > step[id_1] || fabs(pwm[id_2]-pwm_last[id_2]) > step[id_2] || fabs(pwm[id_3]-pwm_last[id_3]) > step[id_3])
//	{
//		if(fabs(pwm[id_1]-pwm_last[id_1]) > step[id_1])
//		{
//			pwm_last[id_1] += (pwm_last[id_1] < pwm[id_1]) ? step[id_1] : -step[id_1];
//			control_servo(id_1, (int)pwm_last[id_1]);
//		}
//		if(fabs(pwm[id_2]-pwm_last[id_2]) > step[id_2])
//		{
//			pwm_last[id_2] += (pwm_last[id_2] < pwm[id_2]) ? step[id_2] : -step[id_2];
//			control_servo(id_2, (int)pwm_last[id_2]);
//		}
//		if(fabs(pwm[id_3]-pwm_last[id_3]) > step[id_3])
//		{
//			pwm_last[id_3] += (pwm_last[id_3] < pwm[id_3]) ? step[id_3] : -step[id_3];
//			control_servo(id_3, (int)pwm_last[id_3]);
//		}
//		osDelay(delay_time);			
//	}
//	
//		 sevor_pwm_last[id_1]=sevor_pwm[id_1] ;
//		 sevor_pwm_last[id_2]=sevor_pwm[id_2] ;
//		 sevor_pwm_last[id_3]=sevor_pwm[id_3] ;	

//	__HAL_TIM_SET_COMPARE(Steer_Eng2,Steer_Eng2_CH,sevor_pwm[2]);//µ×²¿¶æ»ú	Ô½´óÔ½Äæ		
//	__HAL_TIM_SET_COMPARE(Steer_Eng3,Steer_Eng3_CH,sevor_pwm[3]);//µÚÈý¶æ»ú
//	__HAL_TIM_SET_COMPARE(Steer_Eng4,Steer_Eng4_CH,sevor_pwm[4]);//µÚ¶þ¶æ»ú ¼õÐ¡Ë³Ê±Õë
}


void pwm_control(int pwm1, int pwm2, int pwm3, int pwm4, int pwm5)
{
	if(machine_arm.first.accelerate.accelerate_flag == 0)
	{
		machine_arm.first.duty_target = pwm1;
//		machine_arm.first.accelerate.accelerate_flag = 1;
	}
	if(machine_arm.second.accelerate.accelerate_flag == 0)
	{
		machine_arm.second.duty_target = pwm2;
//		machine_arm.second.accelerate.accelerate_flag = 1;
	}
	if(machine_arm.third.accelerate.accelerate_flag == 0)
	{
		machine_arm.third.duty_target = pwm3;
//		machine_arm.third.accelerate.accelerate_flag = 1;
	}
	if(machine_arm.forth.accelerate.accelerate_flag == 0)
	{
		machine_arm.forth.duty_target = pwm4;
//		machine_arm.forth.accelerate.accelerate_flag = 1;
	}
	if(machine_arm.fifth.accelerate.accelerate_flag == 0)
	{
		machine_arm.fifth.duty_target = pwm5;
//		machine_arm.fifth.accelerate.accelerate_flag = 1;
	}
	osDelay(10);
	while(machine_arm.first.accelerate.accelerate_flag == 1 || machine_arm.second.accelerate.accelerate_flag == 1 || machine_arm.third.accelerate.accelerate_flag == 1 || machine_arm.forth.accelerate.accelerate_flag == 1 || machine_arm.fifth.accelerate.accelerate_flag == 1)
	{
		osDelay(1);
	}
}


void pwm_test()
{
	pwm_control(servo_pwm[0], servo_pwm[1], servo_pwm[2], servo_pwm[3], servo_pwm[4]);
	int i;
	int k;
//	for(i=0;i<=4;i++)
//	{
//		if(sevor_pwm[i]<500)
//			sevor_pwm[i]=500;
//		else if(sevor_pwm[i]>2500)
//			sevor_pwm[i]=2500;		
//	}
//			if(sevor_pwm[6]<500)
//			sevor_pwm[6]=500;
//		else if(sevor_pwm[6]>2500)
//			sevor_pwm[6]=2500;
//		
//			if(sevor_pwm[5]<1050)
//			sevor_pwm[5]=1050;
//		else if(sevor_pwm[5]>1450)
//			sevor_pwm[5]=1450;	
			/************************************/
//	int delay_time_two=1;
//		if(sevor_pwm_last[1]<sevor_pwm[1])
//		{
//			k=0;
//			while((sevor_pwm_last[1]+k)<sevor_pwm[1])
//			{
//				k++;
//			__HAL_TIM_SET_COMPARE(Steer_Eng1,Steer_Eng1_CH,sevor_pwm_last[1]+k);//µ×²¿¶æ»ú	Ô½´óÔ½Äæ
//			osDelay(delay_time_two);
//			}
//		}
//		else if(sevor_pwm_last[1]>sevor_pwm[1])
//		{
//			k=0;
//			while((sevor_pwm_last[1]-k)>sevor_pwm[1])
//			{
//				k++;
//			__HAL_TIM_SET_COMPARE(Steer_Eng1,Steer_Eng1_CH,sevor_pwm_last[1]-k);//µ×²¿¶æ»ú	Ô½´óÔ½Äæ
//			osDelay(delay_time_two);				
//			}
//		}
//		/************************************/
//		if(sevor_pwm_last[2]<sevor_pwm[2])
//		{
//			k=0;
//			while((sevor_pwm_last[2]+k)<sevor_pwm[2])
//			{
//				k++;
//			__HAL_TIM_SET_COMPARE(Steer_Eng2,Steer_Eng2_CH,sevor_pwm_last[2]+k);//µ×²¿¶æ»ú	Ô½´óÔ½Äæ
//			osDelay(delay_time_two);
//			}
//		}
//		else if(sevor_pwm_last[2]>sevor_pwm[2])
//		{
//			k=0;
//			while((sevor_pwm_last[2]-k)>sevor_pwm[2])
//			{
//				k++;
//			__HAL_TIM_SET_COMPARE(Steer_Eng2,Steer_Eng2_CH,sevor_pwm_last[2]-k);//µ×²¿¶æ»ú	Ô½´óÔ½Äæ
//			osDelay(delay_time_two);				
//			}
//		}
//		/************************************/
//		if(sevor_pwm_last[3]<sevor_pwm[3])
//		{
//			k=0;
//			while((sevor_pwm_last[3]+k)<sevor_pwm[3])
//			{
//				k++;
//			__HAL_TIM_SET_COMPARE(Steer_Eng3,Steer_Eng3_CH,sevor_pwm_last[3]+k);//µ×²¿¶æ»ú	Ô½´óÔ½Äæ
//			osDelay(delay_time_two);
//			}
//		}
//		else if(sevor_pwm_last[3]>sevor_pwm[3])
//		{
//			k=0;
//			while((sevor_pwm_last[3]-k)>sevor_pwm[3])
//			{
//				k++;
//			__HAL_TIM_SET_COMPARE(Steer_Eng3,Steer_Eng3_CH,sevor_pwm_last[3]-k);//µ×²¿¶æ»ú	Ô½´óÔ½Äæ
//			osDelay(delay_time_two);				
//			}
//		}		
//		
//		/************************************/		
//		if(sevor_pwm_last[4]<sevor_pwm[4])
//		{
//			k=0;
//			while((sevor_pwm_last[4]+k)<sevor_pwm[4])
//			{
//				k++;
//			__HAL_TIM_SET_COMPARE(Steer_Eng4,Steer_Eng4_CH,sevor_pwm_last[4]+k);//µ×²¿¶æ»ú	Ô½´óÔ½Äæ
//				HAL_Delay(delay_time_two);
//			}
//		}
//		else if(sevor_pwm_last[4]>sevor_pwm[4])
//		{
//			k=0;
//			while((sevor_pwm_last[4]-k)>sevor_pwm[4])
//			{
//				k++;
//			__HAL_TIM_SET_COMPARE(Steer_Eng4,Steer_Eng4_CH,sevor_pwm_last[4]-k);//µ×²¿¶æ»ú	Ô½´óÔ½Äæ
//				HAL_Delay(delay_time_two);				
//			}
//		}
//		/************************************/		
//		if(sevor_pwm_last[5]<sevor_pwm[5])
//		{
//			k=0;
//			while((sevor_pwm_last[5]+k)<sevor_pwm[5])
//			{
//				k++;
//			__HAL_TIM_SET_COMPARE(Steer_Eng5,Steer_Eng5_CH,sevor_pwm_last[5]+k);//µ×²¿¶æ»ú	Ô½´óÔ½Äæ
//				HAL_Delay(delay_time_two);
//			}
//		}
//		else if(sevor_pwm_last[5]>sevor_pwm[5])
//		{
//			k=0;
//			while((sevor_pwm_last[5]-k)>sevor_pwm[5])
//			{
//				k++;
//			__HAL_TIM_SET_COMPARE(Steer_Eng5,Steer_Eng5_CH,sevor_pwm_last[5]-k);//µ×²¿¶æ»ú	Ô½´óÔ½Äæ
//				HAL_Delay(delay_time_two);				
//			}
//		}
//		
//		if(sevor_pwm_last[6]<sevor_pwm[6])
//		{
//			k=0;
//			while((sevor_pwm_last[6]+k)<sevor_pwm[6])
//			{
//				k++;
//			__HAL_TIM_SET_COMPARE(Steer_Eng6,Steer_Eng6_CH,sevor_pwm_last[6]+k);//µ×²¿¶æ»ú	Ô½´óÔ½Äæ
//				HAL_Delay(delay_time_two);
//			}
//		}
//		else if(sevor_pwm_last[6]>sevor_pwm[6])
//		{
//			k=0;
//			while((sevor_pwm_last[6]-k)>sevor_pwm[6])
//			{
//				k++;
//			__HAL_TIM_SET_COMPARE(Steer_Eng6,Steer_Eng6_CH,sevor_pwm_last[6]-k);//µ×²¿¶æ»ú	Ô½´óÔ½Äæ
//				HAL_Delay(delay_time_two);				
//			}
//		}
///******************************/
//	__HAL_TIM_SET_COMPARE(Steer_Eng1,Steer_Eng1_CH,sevor_pwm[1]);	
//	__HAL_TIM_SET_COMPARE(Steer_Eng2,Steer_Eng2_CH,sevor_pwm[2]);//µ×²¿¶æ»ú	Ô½´óÔ½Äæ		
//	__HAL_TIM_SET_COMPARE(Steer_Eng3,Steer_Eng3_CH,sevor_pwm[3]);//µÚÈý¶æ»ú
//	__HAL_TIM_SET_COMPARE(Steer_Eng4,Steer_Eng4_CH,sevor_pwm[4]);//µÚ¶þ¶æ»ú ¼õÐ¡Ë³Ê±Õë
//	__HAL_TIM_SET_COMPARE(Steer_Eng5,Steer_Eng5_CH,sevor_pwm[5]);//¼Ð×Ó		1950¼Ð×¡ 1400ËÉ¿ª
//	__HAL_TIM_SET_COMPARE(Steer_Eng6,Steer_Eng6_CH,sevor_pwm[6]);//¼Ð×Ó		1950¼Ð×¡ 1400ËÉ¿ª

//	for(i=0;i<=6;i++)
//	sevor_pwm_last[i]=sevor_pwm[i];
	
}

//»úÐµ±Û¸ù¾Ý¶þÎ¬ÂëË³ÐòºÍÑÕÉ«×¥È¡ÉÏ²ãÎïÁÏ
void catch_upper_layer() 
{
	if(QRCODE[0]==COLOR[0])
	{	
		catch_according_to_order(1);
		get_out_order[0] = 1;
	}
	else if(QRCODE[0]==COLOR[1])
	{
		catch_according_to_order(2);
		get_out_order[0] = 2;
	}
	else if(QRCODE[0]==COLOR[2])
	{
		catch_according_to_order(3);
		get_out_order[0] = 3;
	}
	
	if(QRCODE[1]==COLOR[0])
	{
		catch_according_to_order(1);
		get_out_order[1] = 1;
	}
	else if(QRCODE[1]==COLOR[1])
	{
		catch_according_to_order(2);
		get_out_order[1] = 2;
	}
	else if(QRCODE[1]==COLOR[2])
	{
		catch_according_to_order(3);
		get_out_order[1] = 3;
	}
	
	if(QRCODE[2]==COLOR[0])
	{
		catch_according_to_order(1);
		get_out_order[2] = 1;
	}
	else if(QRCODE[2]==COLOR[1])
	{
		catch_according_to_order(2);
		get_out_order[2] = 2;
	}
	else if(QRCODE[2]==COLOR[2])
	{
		catch_according_to_order(3);
		get_out_order[2] = 3;
	}
}

//»úÐµ±Û¸ù¾Ý¶þÎ¬ÂëË³ÐòºÍÑÕÉ«×¥È¡ÏÂ²ãÎïÁÏ
void catch_bottom_layer() 
{
	
	if(QRCODE[3]==COLOR[3])
	{
		catch_according_to_order(4);
		get_out_order[0] = 1;
	}
	else if(QRCODE[3]==COLOR[4])
	{
		catch_according_to_order(5);
		get_out_order[0] = 2;
	}
	else if(QRCODE[3]==COLOR[5])
	{
		catch_according_to_order(6);
		get_out_order[0] = 3;
	}
	if(QRCODE[4]==COLOR[3])
	{
		catch_according_to_order(4);
		get_out_order[1] = 1;
	}
	else if(QRCODE[4]==COLOR[4])
	{
		catch_according_to_order(5);
		get_out_order[1] = 2;
	}
	else if(QRCODE[4]==COLOR[5])
	{
		catch_according_to_order(6);
		get_out_order[1] = 3;
	}
	
	if(QRCODE[5]==COLOR[3])
	{
		catch_according_to_order(4);
		get_out_order[2] = 1;
	}
	else if(QRCODE[5]==COLOR[4])
	{
		catch_according_to_order(5);
		get_out_order[2] = 2;
	}
	else if(QRCODE[5]==COLOR[5])
	{
		catch_according_to_order(6);
		get_out_order[2] = 3;
	}
	pwm_control(1860, 1000, 2000, 1100, claw_open);
}

/*
	pos:1-6±íÊ¾´ÓÉÏ²ã×ó²àµÚÒ»¸öµ½ÏÂ²ãÓÒ²àµÚÈý¸öÎïÁÏ
	*/
void catch_according_to_order(uint8_t pos)
{
	switch(pos){
		case 1:
		{
			/************×¥ÉÏ²ã×ó±ß*********/		
			pwm_control(2200, 1900, 2300, 1600, claw_open); //Éìµ½ÉÏÃæ
			pwm_control(2140, 1235, 1860, 1880, claw_open); //×¥×¡  3-1840 4-1860
			pwm_control(2140, 1235, 1860, 1880, claw_close); //¼Ð½ô×¦×Ó		
			
			pwm_control(2200, 1325, 450, 2300, claw_close);  //»º³å
			pwm_control(2200, 1325, 450, 2160, claw_close); //»Øµ½ºóÃæ
			osDelay(100);
			pwm_control(2200, 1340, 500, 2100, claw_open);//ËÉ¿ª×¦×Ó			
			pwm_control(2200, 1000, 800, 2500, claw_open); //Ì§Æð×¦×Ó
			pwm_control(2200, 1900, 2300, 1600, claw_open); //Éìµ½ÉÏÃæ	
			break;
		}
		case 2:
		{
			/************×¥ÉÏ²ãÖÐ¼ä*********/		
			pwm_control(1860, 1900, 2300, 1600, claw_open); //Éìµ½ÉÏÃæ
			pwm_control(1860, 1360, 2050, 1850, claw_open);	//È¥×¥
			pwm_control(1860, 1360, 2050, 1850, claw_close);
			
			pwm_control(1800, 1500, 1500, 1700, claw_close); //»º³å
			pwm_control(1800,1235, 405, 2200, claw_close); //»º³å
			pwm_control(1800, 1235, 405, 2085, claw_close); //»Øµ½ºóÃæ
			osDelay(100);
			pwm_control(1800, 1225, 450, 2000, claw_open); //ÕÅ¿ª×¦×Ó
			pwm_control(1800, 1000, 1000, 2500, claw_open);  //Ì§Æð×¦×Ó·ÀÖ¹Åöµ½	
			pwm_control(1800, 1900, 2300, 1600, claw_open); //Éìµ½ÉÏÃæ
			break;
		}
		case 3:
		{
			/************×¥ÉÏ²ãÓÒ±ß*********/		
			pwm_control(1580, 1900, 2300, 1600, claw_open); //Éìµ½ÉÏÃæ
			pwm_control(1580, 1195, 1810, 1910, claw_open); //×¥×¡ 3ºÍ4 1795, 1885
			pwm_control(1580, 1195, 1810, 1910, claw_close);//ÊÕ»Ø×¦×Ó
		
			pwm_control(1480, 1315, 480, 2335, claw_close); //»º³å
			pwm_control(1480, 1315, 480, 2135, claw_close); //»Øµ½ºóÃæ
			osDelay(100);
			pwm_control(1480, 1340, 500, 2140, claw_open);
			pwm_control(1480, 1000, 1000, 2500, claw_open); //Ì§Æð×¦×Ó
			pwm_control(1480, 1900, 2300, 1600, claw_open); //Éìµ½ÉÏÃæ
			break;
		}
		case 4:
		{
			/************×¥ÏÂ²ã×ó±ß*********/
			pwm_control(2200, 1230, 2450, 1100, 1100); //»ØÎ»
			pwm_control(2200, 1230, 2400, 1300, 1100); //ÍùÏÂ·Å
			machine_arm.third.accelerate.speed_max=40;
			machine_arm.third.accelerate.acc_max=0.2;
			pwm_control(2160, 975, 1780, 1625, claw_open); //Éì½øÈ¥ 1325, 2400, 1000,				1625¿ÉÒÔÔö¼Ó
			
			pwm_control(2160, 975, 1780, 1690, claw_close); //×¥×¡
			machine_arm.third.accelerate.speed_max=15;
			machine_arm.third.accelerate.acc_max=0.15;
			pwm_control(2160, 1275, 2450, 1200, claw_close); //Éì³öÀ´
	
			pwm_control(2400, 1220, 2480, 900, claw_close); //»ØÎ» 
			machine_arm.third.accelerate.speed_max=10;
			machine_arm.third.accelerate.acc_max=0.04;
			pwm_control(2400, 1500, 1400, 900, claw_close); //Ì§ÆðÀ´


			pwm_control(2200, 1325, 450, 2300, claw_close);  //»º³å
			pwm_control(2200, 1325, 450, 2160, claw_close); //»Øµ½ºóÃæ
			osDelay(100);
			pwm_control(2200, 1340, 500, 2100, claw_open);//ËÉ¿ª×¦×Ó			
			pwm_control(2200, 1000, 800, 2500, claw_open); //Ì§Æð×¦×Ó
//			pwm_control(2200, 1900, 2300, 1600, claw_open); //Éìµ½ÉÏÃæ
//			pwm_control(2200, 1800, 1000, 1250, 1100); //»ØÈ¥

			pwm_control(2400, 1230, 2450, 1100, 1100); 
			break;
		}
		case 5:
		{
			/************×¥ÏÂ²ãÖÐ¼ä*********/	
			pwm_control(2400, 1230, 2450, 1100, 1100); //»ØÎ»			
			pwm_control(1860, 1230, 2450, 1100, 1100); //»ØÎ»
		//	pwm_control(1860, 1230, 2400, 1300, 1100); //ÍùÏÂ·Å
					machine_arm.third.accelerate.speed_max=40;
				machine_arm.third.accelerate.acc_max=0.2;		
				machine_arm.forth.accelerate.speed_max=40;
				machine_arm.forth.accelerate.acc_max=0.2;
			pwm_control(1860, 1050, 1990, 1465, claw_open); //Éì½øÈ¥
			
			pwm_control(1860, 1050, 1990, 1485, claw_close); //×¥×¡
											machine_arm.third.accelerate.speed_max=15;
				machine_arm.third.accelerate.acc_max=0.15;	
	
			pwm_control(1860, 1260, 2500, 1200, claw_close);	//Éì³öÀ´ 1275,2450, 1360
									machine_arm.third.accelerate.speed_max=10;
				machine_arm.third.accelerate.acc_max=0.04;
				machine_arm.forth.accelerate.speed_max=10;
				machine_arm.forth.accelerate.acc_max=0.1;	
				pwm_control(2400, 1230, 2400, 1300, claw_close); //×ª³öÀ´
			pwm_control(2400, 1500, 1400, 900, claw_close); //Ì§ÆðÀ´

			
	//		pwm_control(1800, 1500, 1500, 1700, claw_close); //»º³å
//			pwm_control(1800,1235, 405, 2200, claw_close); //»º³å
			pwm_control(1800,1235, 405, 2200, claw_close); //»º³å
			pwm_control(1800, 1235, 405, 2085, claw_close); //»Øµ½ºóÃæ
			osDelay(100);
			pwm_control(1800, 1225, 450, 2000, claw_open); //ÕÅ¿ª×¦×Ó
			pwm_control(1800, 1000, 1000, 2500, claw_open);  //Ì§Æð×¦×Ó·ÀÖ¹Åöµ½	
			pwm_control(2400, 1230, 2450, 1100, 1100);//»ØÎ»
			break;
		}
		case 6:
		{
			/************×¥ÏÂ²ãÓÒ±ß*********/
			pwm_control(1480, 1230, 2450, 1100, claw_open); //»ØÎ»
			pwm_control(1480, 1230, 2400, 1300, claw_open); //ÍùÏÂ·Å
						machine_arm.third.accelerate.speed_max=40;
				machine_arm.third.accelerate.acc_max=0.2;
			pwm_control(1560, 975, 1780, 1625, claw_open); //Éì½øÈ¥
			pwm_control(1560, 975, 1780, 1675, claw_close);
										machine_arm.third.accelerate.speed_max=15;
				machine_arm.third.accelerate.acc_max=0.15;

			pwm_control(1560, 1275, 2450, 1360, claw_close); //Éì³öÀ´ 1275,2450, 1360
			machine_arm.third.accelerate.speed_max=10;
			machine_arm.third.accelerate.acc_max=0.04;	
			pwm_control(1560, 1220, 2480, 900, claw_close); //»ØÎ» 1220,2480, 900
			pwm_control(1280, 1500, 1400, 900, claw_close); //Ì§ÆðÀ´			 1500, 1400, 900

			pwm_control(1480,1315, 480, 2335, claw_close); //»º³å
			pwm_control(1480, 1315, 480, 2135, claw_close); //»Øµ½ºóÃæ
	
			pwm_control(1480, 1340, 500, 2140, claw_open);//ËÉ¿ª×¦×Ó
				pwm_control(1480, 1000, 800, 2500, claw_open); //Ì§Æð×¦×Ó		
			pwm_control(1280, 1230, 2450, 1100, claw_open); //»ØÎ»
			break;
		}
	}
}

/*
	¸ù¾ÝÐòºÅ½«ÅÌ×ÓÖÐµÄÎïÁÏÈ¡³öÀ´£¬´ÓÓÒµ½×óÎª123£¬ºÍÎïÁÏË³ÐòÏà·´
	*/
void get_object_out(uint8_t pos)
{
	switch(pos){
		case 1:
		{
			/*×¥ÓÒ²àÔ²ÅÌÎïÁÏ*/
			pwm_control(2220, 1325, 450, 2300, claw_open); //»º³å
			pwm_control(2220, 1325, 450, 2150, claw_open); //»Øµ½ºóÃæ
			osDelay(100);
			pwm_control(2220, 1335, 450, 2110, claw_close); //ÊÕ»Ø×¦×Ó			
			pwm_control(2220, 1100, 800, 2500, claw_close); //Ì§Æð×¦×Ó
//			pwm_control(2200, 1400, 2400, 1000, claw_close); //»ØÎ» 4ºÅ¹Ø½ÚÌ§¸ß£¬²»È»×ª¶¯µÄÊ±ºò»á´òµ½ÆäËûÎïÁÏ
			break;
		}
		case 2:
		{
			/*×¥ÖÐ¼äÔ²ÅÌÎïÁÏ*/  
			pwm_control(1840, 1200, 405, 2200, claw_open); //»º³å
			pwm_control(1840, 1235, 405, 2055, claw_open); //»Øµ½ºóÃæ
			osDelay(100);
			pwm_control(1840, 1235, 405, 2055, claw_close); //ÊÕ»Ø×¦×Ó		2020
			pwm_control(1840, 1100, 1000, 2500, claw_close);  //Ì§Æð×¦×Ó·ÀÖ¹Åöµ½	
//			pwm_control(1840, 1400, 2400, 1000, claw_close);	//»ØÎ»
			break;
		}
		case 3:
		{
			/*×¥×ó²àÔ²ÅÌÎïÁÏ*/ 
			pwm_control(1480,1315, 450, 2335, claw_open); //»º³å
			pwm_control(1480, 1315, 450, 2105, claw_open); //»Øµ½ºóÃæ
			osDelay(100);
			pwm_control(1480, 1325, 450, 2085, claw_close);//ÊÕ»Ø×¦×Ó
			pwm_control(1480, 1200, 800, 2500, claw_close); //Ì§Æð×¦×Ó
//			pwm_control(1480, 1400, 2400, 1200, claw_close);	//»ØÎ»
			break;
		}
	}
}
void put_object_back(uint8_t pos)
{
	switch(pos){
		case 1:
		{
			/*·Åµ½ÓÒ²àÔ²ÅÌÎïÁÏ*/

			pwm_control(2200, 1325, 450, 2300, claw_close); //»º³å
			pwm_control(2200, 1325, 450, 2150, claw_close); //»Øµ½ºóÃæ
			osDelay(100);
			pwm_control(2200, 1335, 450, 2110, claw_open); //ÊÕ»Ø×¦×Ó			
			pwm_control(2200, 1000, 800, 2500, claw_open); //Ì§Æð×¦×Ó
			pwm_control(2200, 1400, 2400, 1000, claw_open); //»ØÎ» 4ºÅ¹Ø½ÚÌ§¸ß£¬²»È»×ª¶¯µÄÊ±ºò»á´òµ½ÆäËûÎïÁÏ
			break;
		}
		case 2:
		{
			/*·Åµ½ÖÐ¼äÔ²ÅÌÎïÁÏ*/  
			pwm_control(1820, 1235, 405, 2200, claw_close); //»º³å
			pwm_control(1820, 1235, 405, 2055, claw_close); //»Øµ½ºóÃæ
			osDelay(100);
			pwm_control(1820, 1245, 405, 2020, claw_open); //ÊÕ»Ø×¦×Ó
			pwm_control(1820, 1000, 1000, 2500, claw_open);  //Ì§Æð×¦×Ó·ÀÖ¹Åöµ½	
			pwm_control(1840, 1400, 2400, 1000, claw_open);	//»ØÎ»
			break;
		}
		case 3:
		{
			/*·Åµ½×ó²àÔ²ÅÌÎïÁÏ*/ 
			pwm_control(1480,1315, 450, 2335, claw_close); //»º³å
			pwm_control(1480, 1315, 450, 2105, claw_close); //»Øµ½ºóÃæ
			osDelay(100);
			pwm_control(1480, 1325, 450, 2085, claw_open);//ÊÕ»Ø×¦×Ó
			pwm_control(1480, 1000, 800, 2500, claw_open); //Ì§Æð×¦×Ó
			pwm_control(1480, 1400, 2400, 1200, claw_open);	//»ØÎ»
			break;
		}
	}	
	
}
/*
	¸ù¾ÝÐòºÅ·ÅÖÃÎïÁÏ pos ´Ó×óµ½ÓÒ123¶ÔÓ¦×ÅBGR
	*/
void put_down_object(uint8_t pos)
{
	switch(pos){
		case 3:
		{
			/*ÎïÁÏ·ÅÖÃµ½À¶É«»·Î»ÖÃ*/

			pwm_control(2160, 1400, 2400, 1200, claw_close);	//»ØÎ»
			machine_arm.third.accelerate.speed_max=30;
			machine_arm.third.accelerate.acc_max=0.1;		
			pwm_control(2160, 1150, 1965, 1685, claw_close); //ÏÈµ½Ä¿µÄµãÌ§ÆðÎï¿é
	//		pwm_control(2160, 1000, 1805, 1685, claw_close); //ÏÈµ½Ä¿µÄµãÌ§ÆðÎï¿é
			machine_arm.third.accelerate.speed_max=10;
			machine_arm.third.accelerate.acc_max=0.04;		
			pwm_control(2160, 980, 1900, 1685, claw_close); //·Åµ½Îï¿é		//1680
			osDelay(50);
			pwm_control(2160,1045, 1900, 1685,claw_open); //ÊÕ»Ø×¦×Ó
			pwm_control(2160, 1400, 2400, 1200, claw_open);	//»ØÎ»
			break;
		}
		case 2:
		{
			/*ÎïÁÏ·ÅÖÃµ½ÂÌÉ«»·Î»ÖÃ*/
			pwm_control(1880, 1400, 2400, 1200, claw_close);	//»ØÎ»
				machine_arm.third.accelerate.speed_max=30;
				machine_arm.third.accelerate.acc_max=0.1;	
			pwm_control(1880, 1135, 2070, 1580, claw_close); //ÏÈµ½Ä¿µÄµãÌ§ÆðÎï¿é
		//	pwm_control(1880, 1115, 2070, 1480, claw_close); //ÏÈµ½Ä¿µÄµãÌ§ÆðÎï¿é
				machine_arm.third.accelerate.speed_max=10;
				machine_arm.third.accelerate.acc_max=0.04;
			pwm_control(1880, 1060, 2070, 1515, claw_close); //·Åµ½Îï¿é		//1545
			osDelay(50);
			pwm_control(1880,  1080, 2070, 1515, claw_open); //ÊÕ»Ø×¦×Ó
			pwm_control(1880, 1400, 2400, 1200, claw_open);	//»ØÎ»
			break;
		}
		case 1:
		{
			/*ÎïÁÏ·ÅÖÃµ½ºìÉ«»·Î»ÖÃ*/
			pwm_control(1590, 1400, 2400, 1200, claw_close);	//»ØÎ»	
					machine_arm.third.accelerate.speed_max=30;
				machine_arm.third.accelerate.acc_max=0.1;	
			pwm_control(1590, 1055, 1835, 1760, claw_close); //ÏÈµ½Ä¿µÄµãÌ§ÆðÎï¿é
		//				pwm_control(1600, 985, 1870, 1570, claw_close); //ÏÈµ½Ä¿µÄµãÌ§ÆðÎï¿é
				machine_arm.third.accelerate.speed_max=10;
				machine_arm.third.accelerate.acc_max=0.04;		

			pwm_control(1590, 940, 1855, 1675,  claw_close); //·Åµ½Îï¿é			//1705		
			osDelay(50);
			pwm_control(1590,980, 1855, 1675, claw_open); //ÊÕ»Ø×¦×Ó
			pwm_control(1590, 1400, 2400, 1200, claw_open);	//»ØÎ»
			break;
		}
	}
}

/*
	¸ù¾ÝÐòºÅ·ÅÖÃÎïÁÏ pos ´Ó×óµ½ÓÒ123¶ÔÓ¦×ÅBGR
	*/
void put_down_object_area3(uint8_t pos)
{
	switch(pos){
		case 3:
		{
			/*ÎïÁÏ·ÅÖÃµ½À¶É«»·Î»ÖÃ*/
			//pwm_control(2150, 1045, 1890, 1680, claw_close); //·Åµ½Îï¿é		//1745

			pwm_control(2170, 1400, 2400, 1200, claw_close); //ÔÆÌ¨ÏÈ×ª¶¯µ½¶ÔÓ¦½Ç¶È
					machine_arm.third.accelerate.speed_max=30;
				machine_arm.third.accelerate.acc_max=0.1;	
			pwm_control(2170, 1145, 1990, 1640, claw_close); //·Åµ½Îï¿é
				machine_arm.third.accelerate.speed_max=10;
				machine_arm.third.accelerate.acc_max=0.04;					
			osDelay(50);
			pwm_control(2160, 1045, 1990, 1550, claw_close); //·Åµ½Îï¿é	
			pwm_control(2160, 1045, 1990, 1550, claw_open); //·Åµ½Îï¿é
			pwm_control(2160, 1400, 2400, 1200, claw_open);	//»ØÎ»
			break;
		}
		case 2:
		{
			/*ÎïÁÏ·ÅÖÃµ½ÂÌÉ«»·Î»ÖÃ*/

			pwm_control(1870, 1400, 2400, 1200, claw_close); //ÔÆÌ¨ÏÈ×ª¶¯µ½¶ÔÓ¦½Ç¶È
					machine_arm.third.accelerate.speed_max=30;
				machine_arm.third.accelerate.acc_max=0.1;	
			pwm_control(1870, 1185, 2130, 1550, claw_close); //·Åµ½Îï¿é
				machine_arm.third.accelerate.speed_max=10;
				machine_arm.third.accelerate.acc_max=0.04;
			osDelay(50);
			pwm_control(1870, 1150, 2170, 1530, claw_close); //·Åµ½Îï¿é
			pwm_control(1870, 1150, 2170, 1530, claw_open); //·Åµ½Îï¿é
			pwm_control(1870, 1400, 2400, 1200, claw_open);	//»ØÎ»
			break;
		}
		case 1:
		{
			/*ÎïÁÏ·ÅÖÃµ½ºìÉ«»·Î»ÖÃ*/ 
			pwm_control(1590, 1400, 2400, 1100, claw_close); //ÔÆÌ¨ÏÈ×ª¶¯µ½¶ÔÓ¦½Ç¶È
					machine_arm.third.accelerate.speed_max=30;
				machine_arm.third.accelerate.acc_max=0.1;	
			pwm_control(1590, 1110, 1960, 1675, claw_close); //·Åµ½Îï¿é
				machine_arm.third.accelerate.speed_max=10;
				machine_arm.third.accelerate.acc_max=0.04;			
			osDelay(50);
			pwm_control(1580, 1035, 1960, 1605, claw_close); //·Åµ½Îï¿é
			pwm_control(1580, 1035, 1960, 1605, claw_open); //·Åµ½Îï¿é
			pwm_control(1580, 1400, 2400, 1200,claw_open);	//»ØÎ»
			break;
		}
//		case 3:
//		{
//			/*ÎïÁÏ·ÅÖÃµ½À¶É«»·Î»ÖÃ*/
//			pwm_control(2160, 1400, 2400, 1200, 1350);	//»ØÎ»
//				machine_arm.third.accelerate.speed_max=30;
//				machine_arm.third.accelerate.acc_max=0.1;		
//			pwm_control(2160, 925, 1720, 1400, 1350); //ÏÈµ½Ä¿µÄµãÌ§ÆðÎï¿é
//	//		pwm_control(2160, 1000, 1805, 1685, 1350); //ÏÈµ½Ä¿µÄµãÌ§ÆðÎï¿é
//				machine_arm.third.accelerate.speed_max=10;
//				machine_arm.third.accelerate.acc_max=0.04;		
//			pwm_control(2160, 925, 1720, 1800, 1350); //·Åµ½Îï¿é		//1745
//			osDelay(50);
//			pwm_control(2160, 925, 1720, 1800, 800); //ÊÕ»Ø×¦×Ó
//			pwm_control(2160, 1400, 2400, 1200, 800);	//»ØÎ»
//			break;
//		}
//		case 2:
//		{
//			/*ÎïÁÏ·ÅÖÃµ½ÂÌÉ«»·Î»ÖÃ*/
//			pwm_control(1880, 1400, 2400, 1200, 1350);	//»ØÎ»
//				machine_arm.third.accelerate.speed_max=30;
//				machine_arm.third.accelerate.acc_max=0.1;	
//			pwm_control(1880, 1010, 1950, 1200, 1350); //ÏÈµ½Ä¿µÄµãÌ§ÆðÎï¿é
//		//	pwm_control(1880, 1115, 2070, 1480, 1350); //ÏÈµ½Ä¿µÄµãÌ§ÆðÎï¿é
//				machine_arm.third.accelerate.speed_max=10;
//				machine_arm.third.accelerate.acc_max=0.04;
//			pwm_control(1880, 1010, 1950, 1600, 1350); //·Åµ½Îï¿é		//1545
//			osDelay(50);
//			pwm_control(1880, 1010, 1950, 1600, 800); //ÊÕ»Ø×¦×Ó
//			pwm_control(1880, 1400, 2400, 1200, 800);	//»ØÎ»
//			break;
//		}
//		case 1:
//		{
//			/*ÎïÁÏ·ÅÖÃµ½ºìÉ«»·Î»ÖÃ*/ 
//			pwm_control(1600, 1400, 2400, 1200, 1350);	//»ØÎ»	
//					machine_arm.third.accelerate.speed_max=30;
//				machine_arm.third.accelerate.acc_max=0.1;	
//			pwm_control(1600, 880, 1675, 1350, 1350); //ÏÈµ½Ä¿µÄµãÌ§ÆðÎï¿é
//		//				pwm_control(1600, 985, 1870, 1570, 1350); //ÏÈµ½Ä¿µÄµãÌ§ÆðÎï¿é
//				machine_arm.third.accelerate.speed_max=10;
//				machine_arm.third.accelerate.acc_max=0.04;		

//			pwm_control(1600, 880, 1675, 1780, 1350); //·Åµ½Îï¿é			//1705		
//			osDelay(50);
//			pwm_control(1600, 880, 1675, 1780,800); //ÊÕ»Ø×¦×Ó
//			pwm_control(1600, 1400, 2400, 1200, 800);	//»ØÎ»
//			break;
//		}
	}
}


/*
	¸ù¾ÝÐòºÅ·ÅÖÃÎïÁÏ pos ´Ó×óµ½ÓÒ123¶ÔÓ¦×ÅBGR
	*/
void catch_up_object_area3_before(uint8_t pos)
{
	switch(pos){
		case 3:
		{
			/*×¥È¡À¶É«»·Î»ÖÃ*/
			//pwm_control(2150, 1045, 1890, 1680, claw_close); //·Åµ½Îï¿é		//1745
//			pwm_control(2200, 1325, 450, 2150, claw_close); //»Øµ½ºóÃæ
			
			
			pwm_control(2170, 1400, 2400, 1200, claw_open); //ÔÆÌ¨ÏÈ×ª¶¯µ½¶ÔÓ¦½Ç¶È
					machine_arm.third.accelerate.speed_max=30;
				machine_arm.third.accelerate.acc_max=0.1;	
			pwm_control(2170, 1145, 1990, 1640, claw_open); //·Åµ½Îï¿é
				machine_arm.third.accelerate.speed_max=10;
				machine_arm.third.accelerate.acc_max=0.04;					
			osDelay(50);
			pwm_control(2160, 1045, 1990, 1600, claw_open); //·Åµ½Îï¿é	 1550
			pwm_control(2160, 1045, 1990, 1600, claw_close); //·Åµ½Îï¿é 1550
			pwm_control(2160, 1400, 2000, 1200, claw_close);	//»ØÎ»
			break;
		}
		case 2:
		{
			/*×¥È¡ÂÌÉ«»·Î»ÖÃ*/

			pwm_control(1870, 1400, 2400, 1200, claw_open); //ÔÆÌ¨ÏÈ×ª¶¯µ½¶ÔÓ¦½Ç¶È
					machine_arm.third.accelerate.speed_max=30;
				machine_arm.third.accelerate.acc_max=0.1;	
			pwm_control(1870, 1185, 2130, 1550, claw_open); //ÕÒÎï¿é // £¡£¡£¡£¡×¦×Ó·ÅÏÂ
				machine_arm.third.accelerate.speed_max=10;
				machine_arm.third.accelerate.acc_max=0.04;
			osDelay(50);
			pwm_control(1870, 1150, 2170, 1530, claw_open); //ÏÂ½µÎï¿é
			pwm_control(1870, 1150, 2170, 1530, claw_close); //×¥Îï¿é
			pwm_control(1870, 1400, 2200, 1200, claw_close);	//»ØÎ»
			break;
		}
		case 1:
		{
			/*×¥È¡ºìÉ«»·Î»ÖÃ*/ 
			pwm_control(1590, 1400, 2400, 1100, claw_open); //ÔÆÌ¨ÏÈ×ª¶¯µ½¶ÔÓ¦½Ç¶È
					machine_arm.third.accelerate.speed_max=30;
				machine_arm.third.accelerate.acc_max=0.1;	
			pwm_control(1590, 1110, 1960, 1675, claw_open); //ÕÒÎï¿é
				machine_arm.third.accelerate.speed_max=10;
				machine_arm.third.accelerate.acc_max=0.04;			
			osDelay(50);
			pwm_control(1580, 1035, 1960, 1605, claw_open); //ÏÂ½µÎï¿é
			pwm_control(1580, 1035, 1960, 1605, claw_close); //·Åµ½Îï¿é
			pwm_control(1580, 1400, 2000, 1200,claw_close);	//»ØÎ»
			break;
		}

	}
}
void catch_up_object_area3_behind(uint8_t pos)
{
	switch(pos){
		case 3:
		{
			/*×¥È¡À¶É«»·Î»ÖÃ*/
			//pwm_control(2150, 1045, 1890, 1680, claw_close); //·Åµ½Îï¿é		//1745
//			pwm_control(2200, 1325, 450, 2150, claw_close); //»Øµ½ºóÃæ
			
			
			pwm_control(2170, 1400, 2400, 1200, claw_open); //ÔÆÌ¨ÏÈ×ª¶¯µ½¶ÔÓ¦½Ç¶È
					machine_arm.third.accelerate.speed_max=30;
				machine_arm.third.accelerate.acc_max=0.1;	
			pwm_control(2170, 1135, 1940, 1720, claw_open); //·Åµ½Îï¿é
				machine_arm.third.accelerate.speed_max=10;
				machine_arm.third.accelerate.acc_max=0.04;					
			osDelay(50);
			pwm_control(2170, 1075, 1935, 1635, claw_open); //·Åµ½Îï¿é	
			pwm_control(2170, 1075, 1935, 1635, claw_close); //·Åµ½Îï¿é
			pwm_control(2170, 1400, 2000, 1200, claw_close);	//»ØÎ»
			break;
		}
		case 2:
		{
			/*×¥È¡ÂÌÉ«»·Î»ÖÃ*/

			pwm_control(1870, 1400, 2400, 1200, claw_open); //ÔÆÌ¨ÏÈ×ª¶¯µ½¶ÔÓ¦½Ç¶È
					machine_arm.third.accelerate.speed_max=30;
				machine_arm.third.accelerate.acc_max=0.1;	
			pwm_control(1870, 1225, 2090, 1705, claw_open); //ÕÒÎï¿é
				machine_arm.third.accelerate.speed_max=10;
				machine_arm.third.accelerate.acc_max=0.04;
			osDelay(50);
			pwm_control(1870,1175, 2090, 1655,claw_open); //ÏÂ½µÎï¿é
			pwm_control(1870, 1175, 2090, 1655, claw_close); //×¥Îï¿é
			pwm_control(1870, 1400, 2200, 1200, claw_close);	//»ØÎ»
			break;
		}
		case 1:
		{
			/*×¥È¡ºìÉ«»·Î»ÖÃ*/ 
			pwm_control(1590, 1400, 2400, 1100, claw_open); //ÔÆÌ¨ÏÈ×ª¶¯µ½¶ÔÓ¦½Ç¶È
					machine_arm.third.accelerate.speed_max=30;
				machine_arm.third.accelerate.acc_max=0.1;	
			pwm_control(1590, 1110, 1850, 1760, claw_open); //ÕÒÎï¿é
				machine_arm.third.accelerate.speed_max=10;
				machine_arm.third.accelerate.acc_max=0.04;			
			osDelay(50);
			pwm_control(1590, 1035, 1850, 1685, claw_open); //ÏÂ½µÎï¿é
			pwm_control(1590, 1035, 1850, 1685, claw_close); //·Åµ½Îï¿é
			pwm_control(1590, 1400, 2000, 1200,claw_close);	//»ØÎ»
			break;
		}

	}	
	
}
void put_down_object_area3_behind(uint8_t pos)
{
	switch(pos){
		case 3:
		{
			/*×¥È¡À¶É«»·Î»ÖÃ*/
			//pwm_control(2150, 1045, 1890, 1680, claw_close); //·Åµ½Îï¿é		//1745
//			pwm_control(2200, 1325, 450, 2150, claw_close); //»Øµ½ºóÃæ
			
			
			pwm_control(2190, 1400, 2400, 1200, claw_close); //ÔÆÌ¨ÏÈ×ª¶¯µ½¶ÔÓ¦½Ç¶È
					machine_arm.third.accelerate.speed_max=30;
				machine_arm.third.accelerate.acc_max=0.1;	
			pwm_control(2190, 1185, 1990, 1690, claw_close); //·Åµ½Îï¿é
				machine_arm.third.accelerate.speed_max=10;
				machine_arm.third.accelerate.acc_max=0.04;					
			osDelay(50);
			pwm_control(2190, 1130, 1985, 1630, claw_close); //·Åµ½Îï¿é	
			pwm_control(2190, 1130, 1985, 1630,claw_open); //·Åµ½Îï¿é
			pwm_control(2190, 1400, 2000, 1200, claw_open);	//»ØÎ»
			break;
		}
		case 2:
		{
			/*×¥È¡ÂÌÉ«»·Î»ÖÃ*/

			pwm_control(1890, 1400, 2400, 1200, claw_close); //ÔÆÌ¨ÏÈ×ª¶¯µ½¶ÔÓ¦½Ç¶È
					machine_arm.third.accelerate.speed_max=30;
				machine_arm.third.accelerate.acc_max=0.1;	
			pwm_control(1890, 1285, 2140, 1950, claw_close); //ÕÒÎï¿é
				machine_arm.third.accelerate.speed_max=10;
				machine_arm.third.accelerate.acc_max=0.04;
			osDelay(50);
			pwm_control(1890,1215, 2140, 1630,claw_close); //ÏÂ½µÎï¿é
			pwm_control(1890, 1215, 2140, 1630, claw_open); //×¥Îï¿é
			pwm_control(1890, 1400, 2200, 1200, claw_open);	//»ØÎ»
			break;
		}
		case 1:
		{
			/*×¥È¡ºìÉ«»·Î»ÖÃ*/ 
			pwm_control(1590, 1400, 2400, 1100, claw_close); //ÔÆÌ¨ÏÈ×ª¶¯µ½¶ÔÓ¦½Ç¶È
					machine_arm.third.accelerate.speed_max=30;
				machine_arm.third.accelerate.acc_max=0.1;	
			pwm_control(1590, 1205, 1990, 1725, claw_close); //ÕÒÎï¿é
				machine_arm.third.accelerate.speed_max=10;
				machine_arm.third.accelerate.acc_max=0.04;			
			osDelay(50);
			pwm_control(1590, 1160, 1985, 1685, claw_close); //ÏÂ½µÎï¿é
			pwm_control(1590, 1160, 1985, 1685, claw_open); //·Åµ½Îï¿é
			pwm_control(1590, 1400, 2000, 1200,claw_open);	//»ØÎ»
			break;
		}

	}	
	
}

/*
	¸ù¾ÝÐòºÅ×¥ÆðÀ´ÎïÁÏ pos ´Ó×óµ½ÓÒ123¶ÔÓ¦×ÅRGB
	*/


void catch_up_object(uint8_t pos)
{
	switch(pos){
		case 3:
		{
			/*×¥ÆðÀ¶É«»·Î»ÖÃÎïÁÏ*/
		//	pwm_control(2150, 1045, 1890, 1680, claw_close); //·Åµ½Îï¿é		//1745
			pwm_control(2160, 1400, 2400, 1200, claw_open);	//×ª¶¯
			pwm_control(2160, 1000, 1950, 1600, claw_open); //·Åµ½Îï¿é	
			pwm_control(2160, 960, 1850, 1650, claw_close); //×¥×¡	
			
			pwm_control(2160, 985, 420, 2000, claw_close); //»º³å
			pwm_control(2160, 1295, 480, 2070, claw_close); //»Øµ½ºóÃæ
			pwm_control(2160, 1295, 480, 2070, claw_open); //·ÅÏÂ
			pwm_control(2160, 1000, 800, 2500, claw_open); //Ì§Æð×¦×Ó
//			pwm_control(2200, 1900, 2300, 1600, claw_open); //Éìµ½ÉÏÃæ
			break;
		}
		case 2:
		{
			/*×¥ÆðÂÌÉ«»·Î»ÖÃÎïÁÏ*/
			pwm_control(1880, 1400, 2400, 1200, claw_open);	//×ª¶¯
			pwm_control(1880, 1100, 2080, 1450, claw_open); //·Åµ½Îï¿é
			pwm_control(1880, 1060, 2040, 1550, claw_close); //×¥×¡	

			pwm_control(1820, 1000, 1000, 1900, claw_close);	//»º³å		
			pwm_control(1820, 1195, 415, 2010, claw_close); //»Øµ½ºóÃæ
			pwm_control(1820, 1195, 415, 2010, claw_open); //ÊÕ»Ø×¦×Ó
			pwm_control(1820, 1000, 1000, 2500, claw_open);  //Ì§Æð×¦×Ó
//			pwm_control(1claw_open, 1900, 2300, 1600, claw_open); //Éìµ½ÉÏÃæ
			break;
		}
		case 1:
		{
			/*×¥ÆðºìÉ«»·Î»ÖÃÎïÁÏ*/ 
			pwm_control(1580, 1400, 2400, 1200, claw_open);	//×ª¶¯	
			pwm_control(1580, 1030, 1900, 1600, claw_open); //·Åµ½Îï¿é
			pwm_control(1580, 1000, 1855, 1700, claw_close);
			
			pwm_control(1450, 985, 420, 2000, claw_close); //»º³å
			pwm_control(1450, 1345, 485, 2150, claw_close); //»Øµ½ºóÃæ 1320,430,2220 1440,485,2250
			pwm_control(1450, 1345, 485, 2150, claw_open); //ÊÕ»Ø×¦×Ó
			pwm_control(1450, 1000, 1000, 2500, claw_open); //Ì§Æð×¦×Ó
//			pwm_control(1500, 1345, 700, 2400, 800); //Ì§Æð×¦×Ó
//			pwm_control(1500, 1400, 2400, 1200, 800);	//»ØÎ»
			break;
		}
	}
}

/*
	¸ù¾ÝÐòºÅµþ¼Ó·ÅÖÃÎïÁÏ pos ´Ó×óµ½ÓÒ123¶ÔÓ¦×ÅBGR
	*/
void overlay_object(uint8_t pos)
{
	switch(pos){
		case 3:
		{
			/*µþ¼Ó·ÅÖÃÀ¶É«»·Î»ÖÃÎïÁÏ*/
			//control_in_turn(2, 3, 4, 1100,455,2195 );//»º³åÏÂ
//			control_in_turn(2, 3, 4, 1185,1500,1850 );//»º³åÏÂ
			pwm_control(2160, 1400, 2450, 1200, claw_close);	//»ØÎ»
			pwm_control(2160, 1300, 2100, 1300, claw_close); // µþ¼Ó
			pwm_control(2160, 1200, 2070, 1550, claw_close); // µþ¼Ó
			osDelay(50);
			pwm_control(2160, 1150, 1980, 1600, claw_open); //ÊÕ»Ø×¦×Ó
			pwm_control(2160, 1325, 2400, 1000, claw_open);	//ÊÕ»Ø
			break;
		}
		case 2:
		{
			/*µþ¼Ó·ÅÖÃÂÌÉ«»·Î»ÖÃÎïÁÏ*/
//			control_in_turn(2, 3, 4, 1325,1700,1500 );//»º³åÏÂ
			pwm_control(1860, 1400, 2400, 1200, claw_close);	//»ØÎ»
			pwm_control(1860, 1300, 2250, 1300, claw_close);//·Åµ½Îï¿é
			pwm_control(1860, 1300, 2220, 1550, claw_close);//·Åµ½Îï¿é
			osDelay(50);
			pwm_control(1860, 1300, 2220, 1550, claw_open);//·Åµ½Îï¿é
			//pwm_control(1860, 1325, 2160, 1610, claw_open);//ÊÕ»Ø×¦×Ó
			pwm_control(1860, 1350, 2400, 1000, claw_open);	//ÊÕ»Ø
			break;
		}
		case 1:
		{
			/*µþ¼Ó·ÅÖÃºìÉ«»·Î»ÖÃÎïÁÏ*/ 
			//control_in_turn(2, 3, 4, 1100,485,2250 );//»º³åÏÂ
//			control_in_turn(2, 3, 4, 1190,1500,1800 );//»º³åÏÂ
			pwm_control(1600, 1400, 2400, 1000, claw_close);	//»ØÎ»
			pwm_control(1560, 1250, 2100, 1500, claw_close);//·Åµ½Îï¿é
			pwm_control(1560, 1200, 2050, 1600, claw_close);//·Åµ½Îï¿0é
			osDelay(50);
			pwm_control(1560, 1150, 2000, 1400, claw_open);//·Åµ½Îï¿é
			//pwm_control(1600, 1120, 1890, 1600, claw_open); //ÊÕ»Ø×¦×Ó
			pwm_control(1600, 1325, 2400, 1000, claw_open);	//ÊÕ»Ø
			break;
		}
	}
}
/*
	¸ù¾ÝÐòºÅ·ÅÖÃÎïÁÏ pos ´Ó×óµ½ÓÒ123¶ÔÓ¦×ÅBGR
	*/
void put_down_object_in_area1(uint8_t pos)
{
	switch(pos)
		{
		case 1:
		{
			/************·ÅÉÏ²ã×ó±ß*********/		
			//pwm_control(2200, 1000, 800, 2500, claw_close); //Ì§Æð×¦×Ó
			pwm_control(2130, 1900, 2300, 1600, claw_close); //Éìµ½ÉÏÃæ
			pwm_control(2130, 1245, 1845, 1960, claw_close); //·ÅÏÂ
			pwm_control(2130, 1245, 1845, 1960,  claw_close); //½µÏÂÀ´		
			pwm_control(2130, 1245, 1845, 1960,  claw_open); //ËÉ¿ª×¦×Ó				
			pwm_control(2130, 1315, 480, 2335, claw_open); //»º³å
			break;
		}
		case 2:
		{
			/************·ÅÉÏ²ãÖÐ¼ä*********/		
		//	pwm_control(1880, 1000, 800, 2500, claw_close); //Ì§Æð×¦×Ó
			pwm_control(1850, 1475, 2040, 1985, claw_close);	//·ÅÏÂ
			pwm_control(1850, 1400, 2040, 1930, claw_close);	//½µÏÂÀ´
			pwm_control(1850, 1400, 2040, 1930, claw_open);//ËÉ¿ª×¦×Ó
			
			pwm_control(1850, 1500, 1500, 1700, claw_open); //»º³å

			break;
		}
		case 3:
		{
			/************×¥ÉÏ²ãÓÒ±ß*********/		
	//		pwm_control(1580, 1000, 800, 2500, claw_close); //Ì§Æð×¦×Ó
			pwm_control(1570, 1900, 2300, 1600, claw_close); //Éìµ½ÉÏÃæ
			pwm_control(1570, 1280, 1805, 1965, claw_close);//·ÅÏÂ			
			pwm_control(1570, 1240, 1805, 1990, claw_close); //ÏòÏÂ½µ
			pwm_control(1570, 1240, 1805, 1990,claw_open);//ËÉ¿ª×¦×Ó
		
			pwm_control(1570, 1315, 480, 2335, claw_open); //»º³å
			break;
		}
	}
}

/*
	¸ù¾ÝÐòºÅ·ÅÖÃÎïÁÏ pos ´Ó×óµ½ÓÒ123¶ÔÓ¦×ÅBGR
	*/
void overlay_object_in_area1(uint8_t pos)
{
	switch(pos)
		{
		case 1:
		{
			/************·ÅÉÏ²ã×ó±ß*********/		
			//pwm_control(2200, 1000, 800, 2500, claw_close); //Ì§Æð×¦×Ó
			pwm_control(2180, 1900, 2300, 1600, claw_close); //Éìµ½ÉÏÃæ
			pwm_control(2180, 1440, 1805, 2060, claw_close); //·ÅÏÂ
			pwm_control(2180, 1375, 1805, 2100, claw_close); //½µÏÂÀ´		
			pwm_control(2180, 1375, 1805, 2100,  claw_open); //ËÉ¿ª×¦×Ó				
			pwm_control(2180, 1375, 1400, 1500, claw_open); //»º³å
			break;
		}
		case 2:
		{
			/************·ÅÉÏ²ãÖÐ¼ä*********/		
		//	pwm_control(1880, 1000, 800, 2500, claw_close); //Ì§Æð×¦×Ó
			pwm_control(1860, 1585, 2000, 2060,  claw_close);	//·ÅÏÂ
			pwm_control(1860, 1530, 2000, 2100, claw_close);	//½µÏÂÀ´
			pwm_control(1860, 1530, 2000, 2100,  claw_open);//ËÉ¿ª×¦×Ó
			
			pwm_control(1860,  1530, 1400, 1500,claw_open); //»º³å

			break;
		}
		case 3:
		{
			/************×¥ÉÏ²ãÓÒ±ß*********/		
	//		pwm_control(1580, 1000, 800, 2500, claw_close); //Ì§Æð×¦×Ó
			pwm_control(1530, 1440, 1805, 2025, claw_close); //Éìµ½ÉÏÃæ
			pwm_control(1530, 1440, 1835, 2125, claw_close);//·ÅÏÂ			
			pwm_control(1530, 1385, 1830, 2075, claw_close); //ÏòÏÂ½µ
			pwm_control(1530, 1385, 1830, 2075,claw_open);//ËÉ¿ª×¦×Ó
		
			pwm_control(1530, 1385, 1400, 1500, claw_open); //»º³å
			break;
		}
	}
}





// »úÐµ±Û½«ÎïÁÏ·ÅÖÃµ½´Ö¼Ó¹¤ÇøÔÙ¼ÐÆðÀ´
void operator_in_area_2()
{
	// °´ÕÕË³ÐòÈ¡³öÎïÁÏ  
	get_object_out(get_out_order[0]);
	put_down_object(COLOR[get_out_order[0]-1]);
	get_object_out(get_out_order[1]);
	put_down_object(COLOR[get_out_order[1]-1]);
	get_object_out(get_out_order[2]);
	put_down_object(COLOR[get_out_order[2]-1]);
	// °´ÕÕË³Ðò×¥ÆðÀ´ÎïÁÏ
	catch_up_object(QRCODE[0]);
	catch_up_object(QRCODE[1]);
	catch_up_object(QRCODE[2]);
}

// »úÐµ±ÛÔÚ°ë³ÉÆ·Çø·ÅÖÃµÚÒ»²ãÎïÁÏ
void operator_in_area_3()
{
	if(QRCODE[0] == 1)
		get_object_out(3);
	else if(QRCODE[0] == 2)
		get_object_out(2);
	else
		get_object_out(1);
	put_down_object_area3(QRCODE[0]);
	
	if(QRCODE[1] == 1)
		get_object_out(3);
	else if(QRCODE[1] == 2)
		get_object_out(2);
	else
		get_object_out(1);
	put_down_object_area3(QRCODE[1]);
	
	if(QRCODE[2] == 1)
		get_object_out(3);
	else if(QRCODE[2] == 2)
		get_object_out(2);
	else
		get_object_out(1);
	put_down_object_area3(QRCODE[2]);
}

void operator_in_area_2_2()
{
	// °´ÕÕË³ÐòÈ¡³öÎïÁÏ
	get_object_out(get_out_order[0]);
	put_down_object(COLOR[get_out_order[0]+2]);
	get_object_out(get_out_order[1]);
	put_down_object(COLOR[get_out_order[1]+2]);
	get_object_out(get_out_order[2]);
	put_down_object(COLOR[get_out_order[2]+2]);
	// °´ÕÕË³Ðò×¥ÆðÀ´ÎïÁÏ
	catch_up_object(QRCODE[3]);
	catch_up_object(QRCODE[4]);
	catch_up_object(QRCODE[5]);
	pwm_control(1860, 850, 840, 1200, 1000);//¸´Î»
	
}


void operator_in_area_3_2()
{
	if(QRCODE[3] == 1)
		get_object_out(3);
	else if(QRCODE[3] == 2)
		get_object_out(2);
	else
		get_object_out(1);
	overlay_object(QRCODE[3]);
	
	if(QRCODE[4] == 1)
		get_object_out(3);
	else if(QRCODE[4] == 2)
		get_object_out(2);
	else
		get_object_out(1);
	overlay_object(QRCODE[4]);
	
	if(QRCODE[5] == 1)
		get_object_out(3);
	else if(QRCODE[5] == 2)
		get_object_out(2);
	else
		get_object_out(1);
	overlay_object(QRCODE[5]);
	pwm_control(1860, 1500, 2400, 800, claw_open);	//ÊÕ»Ø

}

//************************¾öÈü·ÅÖÃµÚ¶þÇøÓò****************/
/*
	¸ù¾ÝÐòºÅ·ÅÖÃÎïÁÏ pos ´Ó×óµ½ÓÒ123¶ÔÓ¦×ÅBGR
	*/
void put_down_object_final(uint8_t pos)
{
	switch(pos){
		case 1:
		{
			/*ÎïÁÏ·ÅÖÃµ½À¶É«»·Î»ÖÃ*/

			pwm_control(2160, 1400, 2400, 1200, claw_close);	//»ØÎ»
			machine_arm.third.accelerate.speed_max=30;
			machine_arm.third.accelerate.acc_max=0.1;		
			pwm_control(2160, 1150, 1965, 1685, claw_close); //ÏÈµ½Ä¿µÄµãÌ§ÆðÎï¿é
	//		pwm_control(2160, 1000, 1805, 1685, claw_close); //ÏÈµ½Ä¿µÄµãÌ§ÆðÎï¿é
			machine_arm.third.accelerate.speed_max=10;
			machine_arm.third.accelerate.acc_max=0.04;		
			pwm_control(2160, 980, 1900, 1685, claw_close); //·Åµ½Îï¿é		//1680
			osDelay(50);
			pwm_control(2160,1045, 1900, 1685,claw_open); //ÊÕ»Ø×¦×Ó
			pwm_control(2160, 1400, 2400, 1200, claw_open);	//»ØÎ»
			break;
		}
		case 2:
		{
			/*ÎïÁÏ·ÅÖÃµ½ÂÌÉ«»·Î»ÖÃ*/
			pwm_control(1880, 1400, 2400, 1200, claw_close);	//»ØÎ»
				machine_arm.third.accelerate.speed_max=30;
				machine_arm.third.accelerate.acc_max=0.1;	
			pwm_control(1880, 1135, 2070, 1580, claw_close); //ÏÈµ½Ä¿µÄµãÌ§ÆðÎï¿é
		//	pwm_control(1880, 1115, 2070, 1480, claw_close); //ÏÈµ½Ä¿µÄµãÌ§ÆðÎï¿é
				machine_arm.third.accelerate.speed_max=10;
				machine_arm.third.accelerate.acc_max=0.04;
			pwm_control(1880, 1060, 2070, 1515, claw_close); //·Åµ½Îï¿é		//1545
			osDelay(50);
			pwm_control(1880,  1080, 2070, 1515, claw_open); //ÊÕ»Ø×¦×Ó
			pwm_control(1880, 1400, 2400, 1200, claw_open);	//»ØÎ»
			break;
		}
		case 3:
		{
			/*ÎïÁÏ·ÅÖÃµ½ºìÉ«»·Î»ÖÃ*/
			pwm_control(1590, 1400, 2400, 1200, claw_close);	//»ØÎ»	
					machine_arm.third.accelerate.speed_max=30;
				machine_arm.third.accelerate.acc_max=0.1;	
			pwm_control(1590, 1055, 1835, 1760, claw_close); //ÏÈµ½Ä¿µÄµãÌ§ÆðÎï¿é
		//				pwm_control(1600, 985, 1870, 1570, claw_close); //ÏÈµ½Ä¿µÄµãÌ§ÆðÎï¿é
				machine_arm.third.accelerate.speed_max=10;
				machine_arm.third.accelerate.acc_max=0.04;		

			pwm_control(1590, 940, 1855, 1675,  claw_close); //·Åµ½Îï¿é			//1705		
			osDelay(50);
			pwm_control(1590,980, 1855, 1675, claw_open); //ÊÕ»Ø×¦×Ó
			pwm_control(1590, 1400, 2400, 1200, claw_open);	//»ØÎ»
			break;
		}
	}
}
/*
	¸ù¾ÝÐòºÅ×¥ÆðÀ´ÎïÁÏ pos ´Ó×óµ½ÓÒ123¶ÔÓ¦×ÅRGB
	*/


void catch_up_object_final(uint8_t pos)
{
	switch(pos){
		case 1:
		{
			/*×¥ÆðÀ¶É«»·Î»ÖÃÎïÁÏ*/
		//	pwm_control(2150, 1045, 1890, 1680, claw_close); //·Åµ½Îï¿é		//1745
			pwm_control(2160, 1400, 2400, 1200, claw_open);	//×ª¶¯
			pwm_control(2160, 1000, 1950, 1600, claw_open); //·Åµ½Îï¿é	
			pwm_control(2160, 960, 1850, 1650, claw_close); //×¥×¡	
			
			pwm_control(2160, 985, 420, 2000, claw_close); //»º³å
			pwm_control(2160, 1295, 480, 2070, claw_close); //»Øµ½ºóÃæ
			pwm_control(2160, 1295, 480, 2070, claw_open); //·ÅÏÂ
			pwm_control(2160, 1000, 800, 2500, claw_open); //Ì§Æð×¦×Ó
//			pwm_control(2200, 1900, 2300, 1600, claw_open); //Éìµ½ÉÏÃæ
			break;
		}
		case 2:
		{
			/*×¥ÆðÂÌÉ«»·Î»ÖÃÎïÁÏ*/
			pwm_control(1880, 1400, 2400, 1200, claw_open);	//×ª¶¯
			pwm_control(1880, 1100, 2080, 1450, claw_open); //·Åµ½Îï¿é
			pwm_control(1880, 1060, 2040, 1550, claw_close); //×¥×¡	

			pwm_control(1820, 1000, 1000, 1900, claw_close);	//»º³å		
			pwm_control(1820, 1195, 415, 2010, claw_close); //»Øµ½ºóÃæ
			pwm_control(1820, 1195, 415, 2010, claw_open); //ÊÕ»Ø×¦×Ó
			pwm_control(1820, 1000, 1000, 2500, claw_open);  //Ì§Æð×¦×Ó
//			pwm_control(1claw_open, 1900, 2300, 1600, claw_open); //Éìµ½ÉÏÃæ
			break;
		}
		case 3:
		{
			/*×¥ÆðºìÉ«»·Î»ÖÃÎïÁÏ*/ 
			pwm_control(1580, 1400, 2400, 1200, claw_open);	//×ª¶¯	
			pwm_control(1580, 1030, 1900, 1600, claw_open); //·Åµ½Îï¿é
			pwm_control(1580, 1000, 1855, 1700, claw_close);
			
			pwm_control(1450, 985, 420, 2000, claw_close); //»º³å
			pwm_control(1450, 1345, 485, 2150, claw_close); //»Øµ½ºóÃæ 1320,430,2220 1440,485,2250
			pwm_control(1450, 1345, 485, 2150, claw_open); //ÊÕ»Ø×¦×Ó
			pwm_control(1450, 1000, 1000, 2500, claw_open); //Ì§Æð×¦×Ó
//			pwm_control(1500, 1345, 700, 2400, 800); //Ì§Æð×¦×Ó
//			pwm_control(1500, 1400, 2400, 1200, 800);	//»ØÎ»
			break;
		}
	}
}
extern uint8_t wifi_corder[6];
uint8_t get_out_order_finalgame[3];//1,2,3´ú±íºó·½´Ó×óµ½ÓÒÈý¸öÇøÓò
/********************¾öÈü´úÂë***********************/
void catch_object_in_Inventory_before_area(void)//ÎïÆ·Çø×¥È¡
{
/****************×¥µÚÒ»¸ö***********/
	if(wifi_corder[0]==1)
	{
		catch_up_object_area3_before(1);
		put_object_back(3); 
		get_out_order_finalgame[0]=3;
	}
	else if(wifi_corder[0]==2)
	{
		catch_up_object_area3_before(2);
		put_object_back(2);
		get_out_order_finalgame[0]=2;		
	}
	else if(wifi_corder[0]==3)
	{
		catch_up_object_area3_before(3);
		put_object_back(1);
		get_out_order_finalgame[0]=1;		
	}	
	
/****************×¥µÚ¶þ¸ö***********/	
		if(wifi_corder[1]==1)
	{
		catch_up_object_area3_before(1);
		put_object_back(3);
		get_out_order_finalgame[1]=3;
	}
	else if(wifi_corder[1]==2)
	{
		catch_up_object_area3_before(2);
		put_object_back(2);
		get_out_order_finalgame[1]=2;		
	}
	else if(wifi_corder[1]==3)
	{
		catch_up_object_area3_before(3);
		put_object_back(1);
		get_out_order_finalgame[1]=1;		
	}
	/****************×¥µÚÈý¸ö***********/
		if(wifi_corder[2]==1)
	{
		catch_up_object_area3_before(1);
		put_object_back(3);
		get_out_order_finalgame[2]=3;
	}
	else if(wifi_corder[2]==2)
	{
		catch_up_object_area3_before(2);
		put_object_back(2);
		get_out_order_finalgame[2]=2;		
	}
	else if(wifi_corder[2]==3)
	{
		catch_up_object_area3_before(3);
		put_object_back(1);
		get_out_order_finalgame[2]=1;		
	}	
	
}

void catch_object_in_Inventory_behind_area(void)//ÎïÆ·Çø×¥È¡
{
/****************×¥µÚÒ»¸ö***********/
	if(wifi_corder[3]==1)
	{
		catch_up_object_area3_before(1);
		put_object_back(3); 
		get_out_order_finalgame[0]=3;
	}
	else if(wifi_corder[3]==2)
	{
		catch_up_object_area3_before(2);
		put_object_back(2);
		get_out_order_finalgame[0]=2;		
	}
	else if(wifi_corder[3]==3)
	{
		catch_up_object_area3_before(3);
		put_object_back(1);
		get_out_order_finalgame[0]=1;		
	}	
	
/****************×¥µÚ¶þ¸ö***********/	
		if(wifi_corder[4]==1)
	{
		catch_up_object_area3_before(1);
		put_object_back(3);
		get_out_order_finalgame[1]=3;
	}
	else if(wifi_corder[4]==2)
	{
		catch_up_object_area3_before(2);
		put_object_back(2);
		get_out_order_finalgame[1]=2;		
	}
	else if(wifi_corder[4]==3)
	{
		catch_up_object_area3_before(3);
		put_object_back(1);
		get_out_order_finalgame[1]=1;		
	}
	/****************×¥µÚÈý¸ö***********/
		if(wifi_corder[5]==1)
	{
		catch_up_object_area3_before(1);
		put_object_back(3);
		get_out_order_finalgame[2]=3;
	}
	else if(wifi_corder[5]==2)
	{
		catch_up_object_area3_before(2);
		put_object_back(2);
		get_out_order_finalgame[2]=2;		
	}
	else if(wifi_corder[5]==3)
	{
		catch_up_object_area3_before(3);
		put_object_back(1);
		get_out_order_finalgame[2]=1;		
	}	
	
}

// »úÐµ±Û½«ÎïÁÏ·ÅÖÃµ½¾«¼Ó¹¤ÇøÔÙ¼ÐÆðÀ´
void operator_in_area_2_finalgame()
{
	// °´ÕÕË³ÐòÈ¡³öÎïÁÏ  
	get_object_out(get_out_order_finalgame[0]);
	put_down_object_final(get_out_order_finalgame[0]);
	get_object_out(get_out_order_finalgame[1]);
	put_down_object_final(get_out_order_finalgame[1]);
	get_object_out(get_out_order_finalgame[2]);
	put_down_object_final(get_out_order_finalgame[2]);
	// °´ÕÕË³Ðò×¥ÆðÀ´ÎïÁÏ
	catch_up_object_final(get_out_order_finalgame[0]);
	catch_up_object_final(get_out_order_finalgame[1]);
	catch_up_object_final(get_out_order_finalgame[2]);
}


// »úÐµ±Û½«ÎïÁÏ·ÅÖÃµ½¿â´æÇø
void operator_in_area_1_firstfloor_finalgame()
{
	// °´ÕÕË³ÐòÈ¡³öÎïÁÏ  
	get_object_out(get_out_order_finalgame[0]);
	put_down_object_in_area1(get_out_order_finalgame[0]);
	get_object_out(get_out_order_finalgame[1]);
	put_down_object_in_area1(get_out_order_finalgame[1]);
	get_object_out(get_out_order_finalgame[2]);
	put_down_object_in_area1(get_out_order_finalgame[2]);
}
// »úÐµ±Û½«ÎïÁÏµþ·Åµ½¿â´æÇø
void operator_in_area_1_secondfloor_finalgame()
{
	// °´ÕÕË³ÐòÈ¡³öÎïÁÏ  
	get_object_out(get_out_order_finalgame[0]);
	overlay_object_in_area1(get_out_order_finalgame[0]);
	get_object_out(get_out_order_finalgame[1]);
	overlay_object_in_area1(get_out_order_finalgame[1]);
	get_object_out(get_out_order_finalgame[2]);
	overlay_object_in_area1(get_out_order_finalgame[2]);
}

void machine_arm_init(void)
{
	machine_arm.first.duty=servo_pwm[0];
	machine_arm.first.duty_target=servo_pwm[0];
	machine_accelerate_init(1);
	//Ä¬ÈÏ£ºspeed_max=50   acc_max=0.1
	//Èç¹ûÐèÒªÐÞ¸Äspeed_maxºÍacc_maxÐèÔÚ´ËÐÐÒÔºó
	machine_arm.first.accelerate.speed_max=35;
	machine_arm.first.accelerate.acc_max=0.1;
	
	machine_arm.fifth.duty=servo_pwm[4];
	machine_arm.fifth.duty_target=servo_pwm[4];
	machine_accelerate_init(5);
	//Ä¬ÈÏ£ºspeed_max=50   acc_max=0.1
	//Èç¹ûÐèÒªÐÞ¸Äspeed_maxºÍacc_maxÐèÔÚ´ËÐÐÒÔºó
	machine_arm.fifth.accelerate.speed_max=35;
	machine_arm.fifth.accelerate.acc_max=0.15;
	
	machine_arm.second.duty=servo_pwm[1];
	machine_arm.second.duty_target=servo_pwm[1];
	machine_accelerate_init(2);
	//Ä¬ÈÏ£ºspeed_max=50   acc_max=0.1
	//Èç¹ûÐèÒªÐÞ¸Äspeed_maxºÍacc_maxÐèÔÚ´ËÐÐÒÔºó
	machine_arm.second.accelerate.speed_max=10;
	machine_arm.second.accelerate.acc_max=0.04;

	machine_arm.third.duty=servo_pwm[2];
	machine_arm.third.duty_target=servo_pwm[2];
	machine_accelerate_init(3);
	//Ä¬ÈÏ£ºspeed_max=50   acc_max=0.1
	//Èç¹ûÐèÒªÐÞ¸Äspeed_maxºÍacc_maxÐèÔÚ´ËÐÐÒÔºó
	machine_arm.third.accelerate.speed_max=10;
	machine_arm.third.accelerate.acc_max=0.04;
	
	machine_arm.forth.duty=servo_pwm[3];
	machine_arm.forth.duty_target=servo_pwm[3];
	machine_accelerate_init(4);
	//Ä¬ÈÏ£ºspeed_max=50   acc_max=0.1
	//Èç¹ûÐèÒªÐÞ¸Äspeed_maxºÍacc_maxÐèÔÚ´ËÐÐÒÔºó
	machine_arm.forth.accelerate.speed_max=10;
	machine_arm.forth.accelerate.acc_max=0.1;
	
}


void change_234_speed(float second_speed_max,float second_acc_max,float third_speed_max,float third_acc_max,float forth_speed_max,float forth_acc_max)
{
	machine_arm.second.accelerate.speed_max=second_speed_max;
	machine_arm.second.accelerate.acc_max=second_acc_max;
	machine_arm.third.accelerate.speed_max=third_speed_max;
	machine_arm.third.accelerate.acc_max=third_acc_max;
	machine_arm.forth.accelerate.speed_max=forth_speed_max;
	machine_arm.forth.accelerate.acc_max=forth_acc_max;
}

void arm_speed_reset(void)
{
	machine_arm.first.accelerate.speed_max=40;
	machine_arm.first.accelerate.acc_max=0.1;
	machine_arm.fifth.accelerate.speed_max=35;
	machine_arm.fifth.accelerate.acc_max=0.15;
	machine_arm.second.accelerate.speed_max=15;
	machine_arm.second.accelerate.acc_max=0.04;
	machine_arm.third.accelerate.speed_max=15;
	machine_arm.third.accelerate.acc_max=0.04;
	machine_arm.forth.accelerate.speed_max=35;
	machine_arm.forth.accelerate.acc_max=0.1;
}

void machine_accelerate_init(int arm_num)
{
	if(arm_num==1)
	{
		machine_arm.first.accelerate.accelerate_flag=0;//...
		machine_arm.first.accelerate.vallast=servo_pwm[0];
		machine_arm.first.accelerate.result=servo_pwm[0];
		machine_arm.first.accelerate.speed_max=SPEED_MAX;
		machine_arm.first.accelerate.acc_max=ACC_MAX;
	}
	else if(arm_num==2)
	{
		machine_arm.second.accelerate.accelerate_flag=0;//...
		machine_arm.second.accelerate.vallast=servo_pwm[1];
		machine_arm.second.accelerate.result=servo_pwm[1];
		machine_arm.second.accelerate.speed_max=SPEED_MAX;
		machine_arm.second.accelerate.acc_max=ACC_MAX;
	}
	else if(arm_num==3)
	{
		machine_arm.third.accelerate.accelerate_flag=0;//...
		machine_arm.third.accelerate.vallast=servo_pwm[2];
		machine_arm.third.accelerate.result=servo_pwm[2];
		machine_arm.third.accelerate.speed_max=SPEED_MAX;
		machine_arm.third.accelerate.acc_max=ACC_MAX;
	}
	else if(arm_num==4)
	{
		machine_arm.forth.accelerate.accelerate_flag=0;//...
		machine_arm.forth.accelerate.vallast=servo_pwm[3];
		machine_arm.forth.accelerate.result=servo_pwm[3];
		machine_arm.forth.accelerate.speed_max=SPEED_MAX;
		machine_arm.forth.accelerate.acc_max=ACC_MAX;
	}
	else if(arm_num==5)
	{
		machine_arm.fifth.accelerate.accelerate_flag=0;//...
		machine_arm.fifth.accelerate.vallast=servo_pwm[4];
		machine_arm.fifth.accelerate.result=servo_pwm[4];
		machine_arm.fifth.accelerate.speed_max=SPEED_MAX;
		machine_arm.fifth.accelerate.acc_max=ACC_MAX;
	}
}



void servo_change_entrance(pwm_servo_control_param_t *arm_x)
{
	if(arm_x->accelerate.accelerate_flag==0)
		{
		if(arm_x->duty_target != arm_x->accelerate.vallast)
			{
			arm_x->accelerate.valtarget=arm_x->duty_target;
			accelerate_pretreat(arm_x);
		}	
	}
}

//¼ì²é¶æ»úÆëÈ«
void servo_target_change_detect(void)
{
	servo_change_entrance(&(machine_arm.first));
	servo_change_entrance(&(machine_arm.second));
	servo_change_entrance(&(machine_arm.third));
	servo_change_entrance(&(machine_arm.forth));
	servo_change_entrance(&(machine_arm.fifth));
}

//¼ì²é¶æ»úÆëÈ«
void servo_target_change(void){
	if(machine_arm.first.accelerate.accelerate_flag)
	{
		accelerate_deal(&(machine_arm.first));
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, machine_arm.first.accelerate.result);
	}
	if(machine_arm.second.accelerate.accelerate_flag)
	{
		accelerate_deal(&(machine_arm.second));
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, machine_arm.second.accelerate.result);
	}
	if(machine_arm.third.accelerate.accelerate_flag)
	{
		accelerate_deal(&(machine_arm.third));
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, machine_arm.third.accelerate.result);
	}
	if(machine_arm.forth.accelerate.accelerate_flag)
	{
		accelerate_deal(&(machine_arm.forth));
		__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, machine_arm.forth.accelerate.result);
	}
	if(machine_arm.fifth.accelerate.accelerate_flag)
	{
		accelerate_deal(&(machine_arm.fifth));
		__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_2, machine_arm.fifth.accelerate.result);
	}
}

//¼ÓËÙÔ¤´¦Àí²¿·Ö
//Ðè´Óvaltarget != vallast½øÈë
void accelerate_pretreat(pwm_servo_control_param_t *arm_x)
	{
	
	arm_x->accelerate.accelerate_flag=1;
	arm_x->accelerate.delta_val=arm_x->accelerate.valtarget - arm_x->accelerate.vallast;
	arm_x->accelerate.val_zero=arm_x->accelerate.vallast;
	arm_x->accelerate.Ta = arm_x->accelerate.speed_max/arm_x->accelerate.acc_max;
	if(arm_x->accelerate.delta_val>=0){arm_x->accelerate.acc=arm_x->accelerate.acc_max;arm_x->accelerate.speed=arm_x->accelerate.speed_max;}
	else if(arm_x->accelerate.delta_val<0){arm_x->accelerate.acc=-arm_x->accelerate.acc_max;arm_x->accelerate.speed=-arm_x->accelerate.speed_max;}
	arm_x->accelerate.Tm = (arm_x->accelerate.delta_val - arm_x->accelerate.acc*arm_x->accelerate.Ta*arm_x->accelerate.Ta)/arm_x->accelerate.speed;
	arm_x->accelerate.T=1;
	arm_x->accelerate.vallast=arm_x->accelerate.valtarget;
}

//¼ÓËÙ¹ý³ÌÑ­»·²¿·Ö
//Ðè´Óif accelerate_flag==1½øÈë
void accelerate_deal(pwm_servo_control_param_t *arm_x){
	//¾ØÐÎ
	if(arm_x->accelerate.Tm>0){
		if(arm_x->accelerate.T <= arm_x->accelerate.Ta){
			arm_x->accelerate.val_output = round(0.5*arm_x->accelerate.acc*arm_x->accelerate.T*arm_x->accelerate.T);
		}else if(arm_x->accelerate.T<=(arm_x->accelerate.Ta+arm_x->accelerate.Tm)){
			arm_x->accelerate.val_output = round(0.5*arm_x->accelerate.acc*arm_x->accelerate.Ta*arm_x->accelerate.Ta + arm_x->accelerate.speed*(arm_x->accelerate.T-arm_x->accelerate.Ta));
		}else if(arm_x->accelerate.T<=(arm_x->accelerate.Ta+arm_x->accelerate.Tm+arm_x->accelerate.Ta)){
			arm_x->accelerate.val_output = round(0.5*arm_x->accelerate.acc*arm_x->accelerate.Ta*arm_x->accelerate.Ta + arm_x->accelerate.speed*arm_x->accelerate.Tm - 0.5*arm_x->accelerate.acc*(arm_x->accelerate.T-arm_x->accelerate.Ta-arm_x->accelerate.Tm)*(arm_x->accelerate.T-arm_x->accelerate.Ta-arm_x->accelerate.Tm) + arm_x->accelerate.speed*(arm_x->accelerate.T-arm_x->accelerate.Ta-arm_x->accelerate.Tm));
		}else{
			//Íê³É
			arm_x->accelerate.val_output=arm_x->accelerate.delta_val;
			arm_x->accelerate.accelerate_flag=0;
		}
		arm_x->accelerate.delta_output=arm_x->accelerate.val_output-arm_x->accelerate.delta_output;
	}else{
	//Èý½ÇÐÎ
		arm_x->accelerate.Ta = sqrt(arm_x->accelerate.delta_val/arm_x->accelerate.acc);
		if(arm_x->accelerate.T<=arm_x->accelerate.Ta){
			arm_x->accelerate.val_output = round(0.5*arm_x->accelerate.acc*arm_x->accelerate.T*arm_x->accelerate.T);
		}else if(arm_x->accelerate.T<=2*arm_x->accelerate.Ta){
			arm_x->accelerate.val_output = round(0.5*arm_x->accelerate.acc*arm_x->accelerate.Ta*arm_x->accelerate.Ta - 0.5*arm_x->accelerate.acc*(arm_x->accelerate.T-arm_x->accelerate.Ta)*(arm_x->accelerate.T-arm_x->accelerate.Ta)+arm_x->accelerate.acc*arm_x->accelerate.Ta*(arm_x->accelerate.T-arm_x->accelerate.Ta));
		}else{
			//Íê³É
			arm_x->accelerate.val_output=arm_x->accelerate.delta_val;
			arm_x->accelerate.accelerate_flag=0;
		}
	}
	arm_x->accelerate.delta_output=arm_x->accelerate.val_output;
	arm_x->accelerate.result=arm_x->accelerate.val_zero+arm_x->accelerate.val_output;
	arm_x->accelerate.T++;
}

