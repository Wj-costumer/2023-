#ifndef TOF_H
#define TOF_H

#include "main.h"
#include "struct_typedef.h"

typedef struct{
	fp32 x_dist_1;
	fp32 x_dist_2;
	fp32 y_dist_1;
	fp32 y_dist_2;
	fp32 x_error;
	fp32 y_error;
	fp32 angle_x;
	fp32 angle_y;
}TOF_t;

#define L 0.10 
#endif