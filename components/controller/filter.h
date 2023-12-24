#ifndef FILTER_H
#define FILTER_H
#include "main.h"
void filter(int16_t input[4],int16_t input_last[4]);
int16_t FolowPassFilter(int16_t input,int16_t input_last,float a);


#endif
