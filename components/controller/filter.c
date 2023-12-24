#include "filter.h"
#include "main.h"
#define input_line 1500
#define input_difference_max 500
int16_t give_current[4];
int16_t give_current_last[4];
void filter(int16_t input[4],int16_t input_last[4])
{
	
	int i;

	for(i=0;i<=3;i++)
	{
			if(input[i]>1500)
			{
				if((input[i]-input_last[i])>input_difference_max)
				{
					give_current[i]=FolowPassFilter(input[i],input_last[i],0.8);
				}
				else
				{
					give_current[i]=FolowPassFilter(input[i],input_last[i],0.15);
				}
			}
			else
			{
					give_current[i]=FolowPassFilter(input[i],input_last[i],0.15);
			}
			give_current_last[i]=give_current[i];
	}


}
int16_t FolowPassFilter(int16_t input,int16_t input_last,float a)
{
	int16_t output;
	output=(int16_t)(a*input+(1-a)*input_last);
	return output;
}

