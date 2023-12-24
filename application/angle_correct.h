#ifndef ANGLE_CORRECT_H
#define ANGLE_CORRECT_H
#include "main.h"
#include "struct_typedef.h"

void angle_correct(void);

#define back  			1
#define turn_right	2
#define turn_left 	3
#define still		 	4
#define left_move  5
#define forward    6
#define right_move    7

#define move_left  	 1
#define move_right	 2
#define move_front   3
#define move_back    4
#define no_move 		 0

#define LIMIT		0.05
#define MAX			0.2

typedef struct
{
	uint8_t left_line[5];	//��ߵĵ�
	uint8_t right_line[5];//�ұߵĵ�
	uint8_t front_line[5];//ǰ��ĵ�
	uint8_t behind_line[5];	//����ĵ�
	
	uint8_t last_state[2];//��¼�ϴ�״̬
	uint8_t state[2];//��¼��ǰ״̬
	uint8_t target_point[2];//����λ��
	uint8_t current_point[2];	//��ǰλ��
	
	uint8_t left_last_line[5];	//�ҵ����߼�Ϊ1 ȫ����������
	uint8_t right_last_line[5];
	uint8_t front_last_line[5];
	uint8_t behind_last_line[5];		
	
	
	fp32 left_last_move[5];	//�ҵ����߼�¼��ǰ����ת�ľ���  ���Ҳ��¼xλ��  ���²��¼yλ��
	fp32 right_last_move[5];
	fp32 front_last_move[5];
	fp32 behind_last_move[5];		
	
	uint8_t left_max;				//��¼�������²��ҵ������ĺ���С�ĵ��λ��
	uint8_t left_min;	
	uint8_t right_max;
	uint8_t right_min;
	uint8_t front_max;
	uint8_t front_min;
	uint8_t behind_max;
	uint8_t behind_min;	
	
	fp32 left_move_postion;//��¼ÿ���ж�Ϊ����ʱ�����ƶ��ľ��룬ʹ��ÿ���жϺ���ʱ���ϴ����Ҫ����ĳ����ֵ��ֹ��ͬһ�������ظ��ж�
	fp32 right_move_postion;
	fp32 front_move_postion;
	fp32 behind_move_postion;


	int			left_cross_line; 		//��¼�Ѿ������ĺ���
	int			right_cross_line; 		//��¼�Ѿ������ĺ���
	int			behind_cross_line; 		//��¼�Ѿ������ĺ���
	int			front_cross_line; 		//��¼�Ѿ������ĺ���	
	
	uint8_t dir_x;		//��¼����
	uint8_t dir_y;	
	
}follow_line;

fp32 judege_blackline(fp32 move[5]);
void count_line_init();
void followline_move_target_point(float target_point[2]);
void	count_line();
void read_pin();
void front_middle_correct(void);
#endif
