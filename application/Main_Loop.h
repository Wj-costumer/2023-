#ifndef _MAIN_LOOP_H
#define _MAIN_LOOP_H

#include "struct_typedef.h"
#include "cmsis_os.h"
#include "Chassis_Move.h"
#include "Servo_task.h"
#include "angle_correct.h"

#define MAIN_LOOP_INIT_TIME 357

void Main_Loop_TASK();

#endif 