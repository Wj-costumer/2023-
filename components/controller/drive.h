#ifndef DRIVE_H
#define DRIVE_H

#include "struct_typedef.h"
#include "QCS.h"
#include "Chassis_Move.h"

#define angle_max 8192
#define angle_min 0
#define count_time 0.01   //tim1 10ms

void car_forward(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);
void car_back(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);
void car_right(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);
void car_left(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);
void car_rightturn(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);
void car_leftturn(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);
//void drive_mode(int drive_flag );
float distance();
void get_total_angle_and_dist_xy(motor_measure_t *ptr);
#endif

