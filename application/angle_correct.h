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
	uint8_t left_line[5];	//左边的灯
	uint8_t right_line[5];//右边的灯
	uint8_t front_line[5];//前面的灯
	uint8_t behind_line[5];	//后面的灯
	
	uint8_t last_state[2];//记录上次状态
	uint8_t state[2];//记录当前状态
	uint8_t target_point[2];//期望位置
	uint8_t current_point[2];	//当前位置
	
	uint8_t left_last_line[5];	//找到黑线计为1 全部找完清零
	uint8_t right_last_line[5];
	uint8_t front_last_line[5];
	uint8_t behind_last_line[5];		
	
	
	fp32 left_last_move[5];	//找到黑线记录当前轮子转的距离  左右侧记录x位移  上下侧记录y位移
	fp32 right_last_move[5];
	fp32 front_last_move[5];
	fp32 behind_last_move[5];		
	
	uint8_t left_max;				//记录左右上下侧找到的最大的和最小的点的位置
	uint8_t left_min;	
	uint8_t right_max;
	uint8_t right_min;
	uint8_t front_max;
	uint8_t front_min;
	uint8_t behind_max;
	uint8_t behind_min;	
	
	fp32 left_move_postion;//记录每次判断为黑线时轮子移动的距离，使得每次判断黑线时与上次相比要大于某个阈值防止在同一条黑线重复判断
	fp32 right_move_postion;
	fp32 front_move_postion;
	fp32 behind_move_postion;


	int			left_cross_line; 		//记录已经经过的黑线
	int			right_cross_line; 		//记录已经经过的黑线
	int			behind_cross_line; 		//记录已经经过的黑线
	int			front_cross_line; 		//记录已经经过的黑线	
	
	uint8_t dir_x;		//记录方向
	uint8_t dir_y;	
	
}follow_line;

fp32 judege_blackline(fp32 move[5]);
void count_line_init();
void followline_move_target_point(float target_point[2]);
void	count_line();
void read_pin();
void front_middle_correct(void);
#endif
