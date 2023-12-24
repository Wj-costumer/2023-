#ifndef FOLLOW_LINE_H
#define FOLLOW_LINE_H
#include "main.h"
#include "CAN_receive.h"
typedef enum
{
	LEFT,
	RIGHT,
	FRONT,
	BEHIND
	
	
}follow_dir;
void move_targetpoint_followline(fp32 target_pos[2],follow_dir DIR);
void follow_frontline();
void follow_line_go_to_downblackline();
void follow_line_task(void const * argument);
void follow_line_go_to_downfrontline();
void follow_leftline();
void follow_rightline();
void follow_behindline();

#endif