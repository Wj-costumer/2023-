#ifndef _STRING_OPERATOR_H
#define _STRING_OPERATOR_H

#include "struct_typedef.h"

int length_string(char *s);
fp32 abstract_distance(char *s, fp32 *dist);
char *reverse(char *s);
char *my_itoa(int n);
#endif