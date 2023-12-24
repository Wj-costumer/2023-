#include "Main_Loop.h"
#include "follow_line.h"
#include "arm_control.h" 
#include "usart.h"

fp32 QRCODE_pos[2] = {0.6, 0.45}; // ��ά��λ��  
fp32 RAW_Material_Area_pos[2] = {1.65, 0.45}; // ԭ��������λ��
fp32 ALL_Process_Area_pos[2] = {1.65, 1.05}; // ���ӹ�������λ��
fp32 Complete_Process_Area_pos[2] = {1.05, 1.65}; // ���Ʒ������λ��
fp32 POINT[3][2]={{0.4, 0.4},{0.15, 0.6},{0.45, 1.80}};	//��������
fp32 END_pos[2] = {0, 2.10}; // ������λ��
fp32 Object_pos[2] = {1.05, 1.65}; // ��Ʒ��λ��
fp32 before_Object_pos1[2] = {0.45, 0.45}; //  ��Ʒ���Ҳ�λ��
fp32 before_Object_pos2[2] = {1.05, 0.45}; //  ��Ʒ���Ҳ�λ��
fp32 Inventory_area_pos[2] = {1.65, 0.45}; //  ������Ҳ�λ��
fp32 Go_back_first_step[2] = {1.65, 1.65}; //  ��������һ��
fp32 Go_back_second_step[2] = {0.45, 1.65}; //  �������ڶ���
fp32 Go_back_third_step[2] = {0, 2.10	}; //  ������������

extern chassis_move_t chassis_move;
extern mechine_arm_param_t machine_arm;
extern uint8_t angle_mode[2];	
extern int start_flag;
extern int find_QRCODE_flag;
extern int find_COLOR_flag;
extern int rotation_flag;
extern chassis_move_t chassis_move;
extern UART_HandleTypeDef huart1;
extern fp32 angle_error;
extern int follow_blackline_flag;
extern uint8_t wifi_corder[6];
extern uint8_t find_wifi_corder_flag;
follow_dir DIR;
uint8_t correct_front_middle_flag=0;
uint8_t correct_shizi_flag=0;
void Main_Loop_TASK()
{
	osDelay(MAIN_LOOP_INIT_TIME);
	machine_arm_init();
	while(1)
	{	
//		pwm_test();
		if(start_flag == 1)
		{
//			find_wifi_corder_flag=1;
//			wifi_corder[0]=1;
//			wifi_corder[1]=2;
//			wifi_corder[2]=3;
//			wifi_corder[3]=3;
//			wifi_corder[4]=2;
//			wifi_corder[5]=1;			
			if(find_wifi_corder_flag)//����ҵ���wifi����
			{				
				move_to_target_point(before_Object_pos1);
				chassis_move.pos[0] = before_Object_pos1[0];
				chassis_move.pos[1] = before_Object_pos1[1];
				move_targetpoint_followline(before_Object_pos2,FRONT);//�ƶ�����Ʒ���Ҳ�
				chassis_move.pos[0] = before_Object_pos2[0];
				chassis_move.pos[1] = before_Object_pos2[1];			
				angle_correct();//����
				move_targetpoint_followline(Object_pos,LEFT);//�ƶ�����Ʒ��
				angle_correct();//����
				chassis_move.pos[0] = Object_pos[0];
				chassis_move.pos[1] = Object_pos[1];
				rotation(90);
				angle_correct();//����	
											correct_shizi_flag=1;//�ǶȽ�������
				/***********ץǰ�������************/
			catch_object_in_Inventory_before_area();		
				/*********************************/
											correct_shizi_flag=0;//�ǶȽ�������				
				
		
				move_to_target_point(ALL_Process_Area_pos);//�ƶ������ӹ���
				angle_correct();//����	
				rotation(0);				
				angle_correct();//����	
				chassis_move.pos[0] = ALL_Process_Area_pos[0];
				chassis_move.pos[1] = ALL_Process_Area_pos[1];	
				follow_line_go_to_downblackline();				
				/***********���ӹ�������************/
				correct_front_middle_flag=1;
					operator_in_area_2_finalgame();		
									correct_front_middle_flag=0;
				/*********************************/	



				move_to_target_point(Inventory_area_pos);//�ƶ��������	
				chassis_move.pos[0] = Inventory_area_pos[0];
				chassis_move.pos[1] = Inventory_area_pos[1];					
				angle_correct();//����
				rotation(-90);
				angle_correct();//����
				
				follow_line_go_to_downblackline();
				
				/***********�ſ������һ������************/
								correct_front_middle_flag=1;
				operator_in_area_1_firstfloor_finalgame();
													correct_front_middle_flag=0;
				/*********************************/	



/*****************��ʼ�ڶ���************************/
				rotation(90);
				move_targetpoint_followline(Go_back_first_step,FRONT);//�ƶ�����Ʒ��
				chassis_move.pos[0] = Go_back_first_step[0];
				chassis_move.pos[1] = Go_back_first_step[1];				
				move_targetpoint_followline(Object_pos,LEFT);//�ƶ�����Ʒ��
				chassis_move.pos[0] = Object_pos[0];
				chassis_move.pos[1] = Object_pos[1];		
				angle_correct();//����
			//	rotation(90);
				angle_correct();//����	
				
				follow_line_go_to_downblackline();
				/***********ץ���������************/
													correct_front_middle_flag=1;
			catch_object_in_Inventory_behind_area();
				/*********************************/
																	correct_front_middle_flag=0;
				
		
				move_to_target_point(ALL_Process_Area_pos);//�ƶ������ӹ���	
				angle_correct();//����	
				rotation(0);
				angle_correct();//����	
				chassis_move.pos[0] = ALL_Process_Area_pos[0];
				chassis_move.pos[1] = ALL_Process_Area_pos[1];	
				follow_line_go_to_downblackline();				
				/***********���ӹ�������************/
																	correct_front_middle_flag=1;
				operator_in_area_2_finalgame();
																	correct_front_middle_flag=0;
				/*********************************/	



				move_to_target_point(Inventory_area_pos);//�ƶ��������	
				chassis_move.pos[0] = Inventory_area_pos[0];
				chassis_move.pos[1] = Inventory_area_pos[1];
				angle_correct();//����
				rotation(-90);
				angle_correct();//����
				follow_line_go_to_downfrontline();
				/***********�ſ�����ڶ�������************/
												correct_front_middle_flag=1;
				operator_in_area_1_secondfloor_finalgame();
				/*********************************/	
												correct_front_middle_flag=0;

				/*************����******************/
				move_targetpoint_followline(Go_back_first_step,BEHIND);//	
				chassis_move.pos[0] = Go_back_first_step[0];
				chassis_move.pos[1] = Go_back_first_step[1];				
				move_targetpoint_followline(Go_back_second_step,RIGHT);
				chassis_move.pos[0] = Go_back_second_step[0];
				chassis_move.pos[1] = Go_back_second_step[1];
				move_to_target_point(Go_back_third_step);		
			start_flag=0;			
		}
				//pwm_test();
		}

		// ��ѭ��ʱ�ͽ���
		osDelay(1);
	}
}
