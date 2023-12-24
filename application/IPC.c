#include "IPC.h"
#include "arm_control.h"
#include "stm32f4xx_it.h"
#include "Chassis_Move.h"
/*
	��е�۵���
*/
extern uint16_t PWM[5];        		//��е��PWMֵ
extern chassis_move_t 	chassis_move;
void SCI_Send_Datas(UART_HandleTypeDef *huart) //��λ�����ݷ��ͣ���λ�����Ʋ���ͼ
{
    int i,j;
  static unsigned short int send_data[3][4] = { { 0 }, { 0 }, { 0 } };
  short int checksum=0;
  uint8_t xorsum=0,high,low;
	uint8_t buf1[]={'S'};
	uint8_t buf2[]={'T'};
	
	uint8_t buf3[]={0};
	uint8_t buf4[]={0};
	uint8_t buf5[]={0};
	uint8_t buf6[]={0};
	
	//��һ������
  //send_data[0][0] = (unsigned short int)((PWM[1]*270-500*270)/2000); 
  //send_data[0][1] = (unsigned short int)((PWM[2]*270-500*270)/2000);
  //send_data[0][2] = (unsigned short int)((PWM[3]*270-500*270)/2000);
	send_data[0][0] = (unsigned short int)(PWM[1]); 
  send_data[0][1] = (unsigned short int)(PWM[2]);
  send_data[0][2] = (unsigned short int)(PWM[3]);
  send_data[0][3] = (unsigned short int)(0);

	//
  send_data[1][0] = (unsigned short int)(PWM[0]);		//��̨���
  send_data[1][1] = (unsigned short int)(0);
  send_data[1][2] = (unsigned short int)(0);
  send_data[1][3] = (unsigned short int)(0);

  send_data[2][0] = (unsigned short int)(0);		//���Ӷ��
  send_data[2][1] = (unsigned short int)(chassis_move.x_move_dist*10000);
  send_data[2][2] = (unsigned short int)(0);
  send_data[2][3] = (unsigned short int)(0);

  //uart_write_byte(uart_num, 'S');
  //uart_write_byte(uart_num, 'T');
	HAL_UART_Transmit( huart,buf1,1,1000);
	HAL_UART_Transmit( huart,buf2,1,1000);
	
  for (i = 0; i < 3; i++)
    for (j = 0; j < 4; j++)
    {
      //low=(unsigned char)(send_data[i][j] & 0x00ff);
      //high=(unsigned char)(send_data[i][j] >> 8u);
      //uart_write_byte(uart_num,low ); uart_write_byte(uart_num, high);
			low=(uint8_t )(send_data[i][j] & 0x00ff);
			high=(uint8_t )(send_data[i][j] >> 8u);
			*buf3=low;
			*buf4=high;
			
			HAL_UART_Transmit( huart,buf3,1,1000);
			HAL_UART_Transmit( huart,buf4,1,1000);
			
			
      checksum+=low; checksum+=high;
      xorsum^=low; xorsum^= high;
    }
  //uart_write_byte(uart_num, (unsigned char)(checksum & 0x00ff));
  //uart_write_byte(uart_num,xorsum);
		*buf5=checksum & 0x00ff;
		*buf6=xorsum;
		HAL_UART_Transmit( huart,buf5,1,1000);
		HAL_UART_Transmit( huart,buf6,1,1000);
			
}

/*
void Steer_drive(uint8_t value)
{
#if (value==97)	//a -> PWM1
#define Steer_Eng Steer_Eng1
#define	Steer_Eng_CH	Steer_Eng1_CH
#endif

}
*/

extern uint8_t Steer_Eng; //������
extern uint8_t value; //ת���Ƕ�ѡ��
extern uint8_t buff;			//��������
extern uint16_t add;         //��е��΢������ 
extern uint8_t dir;
/*
	a~e��ѡ��1~5�Ŷ��
	
	0~4��ѡ����ת���Ƕ� 
			0��0
			1��45
			2��90
			3��135
			4��180
*/

void Status_choose(void)
{
	 if((buff>=97)&&(buff<=101))
			Steer_Eng=buff;
	 if((buff>=48)&&(buff<=52))
		  value=buff;
	 if(buff=='i')
			dir='i';
	 if(buff=='j')
			dir='j';
}


//����ֵ�
void PWM_drive()
{
	/*uint8_t num=Steer_Eng-97;
	//uint16_t values=value-
	if(value==48)PWM[num]=Angle0;
	if(value==49)PWM[num]=Angle67_5;
	if(value==50)PWM[num]=Angle135;
	if(value==51)PWM[num]=Angle202_5;
	if(value==52)PWM[num]=Angle270;
	
	__HAL_TIM_SET_COMPARE(Steer_Eng,,Angle0);*/
	
	if(Steer_Eng==97) //a->1�Ŷ��
	{
	if(value==48)  //0
	{__HAL_TIM_SET_COMPARE(Steer_Eng1,Steer_Eng1_CH,1500);   //ֹͣ�ź�
		PWM[0]=0;
	}
	if(value==49)	 //1
	{	__HAL_TIM_SET_COMPARE(Steer_Eng1,Steer_Eng1_CH,1200);   //˳ʱ��ת���ź�
		PWM[0]=1 ;
	}
	if(value==50)	 //2
	{	__HAL_TIM_SET_COMPARE(Steer_Eng1,Steer_Eng1_CH,1800);   //��ʱ��ת���ź��ź�
		PWM[0]=2 ;
	}
	
//	if(value==50) //2
//	{__HAL_TIM_SET_COMPARE(Steer_Eng1,Steer_Eng1_CH,Angle135);
//		PWM[0]=Angle135;
//	}
//	if(value==51) //3
//	{__HAL_TIM_SET_COMPARE(Steer_Eng1,Steer_Eng1_CH,Angle202_5);
//		PWM[0]=Angle202_5;
//	}
//	if(value==52) //4
//	{__HAL_TIM_SET_COMPARE(Steer_Eng1,Steer_Eng1_CH,Angle270);
//		PWM[0]=Angle270;
	
}
	
	if(Steer_Eng==98) //b->2�Ŷ��
	{
	if(value==48)  //0
	{	__HAL_TIM_SET_COMPARE(Steer_Eng2,Steer_Eng2_CH,1000);
		PWM[1]=Angle0;
	}
	if(value==49)	 //1
	{	__HAL_TIM_SET_COMPARE(Steer_Eng2,Steer_Eng2_CH,1200);
		PWM[1]=Angle67_5;
	}
	if(value==50) //2
	{	__HAL_TIM_SET_COMPARE(Steer_Eng2,Steer_Eng2_CH,Angle135);
		PWM[1]=Angle135;
	}
	if(value==51) //3
	{	__HAL_TIM_SET_COMPARE(Steer_Eng2,Steer_Eng2_CH,1700);
		PWM[1]=Angle202_5;
	}	
	if(value==52) //4
	{	__HAL_TIM_SET_COMPARE(Steer_Eng2,Steer_Eng2_CH,Angle202_5);
		PWM[1]=Angle270;
	}
	}
	
	if(Steer_Eng==99) //c->3�Ŷ��
	{
	if(value==48)  //0
	{	__HAL_TIM_SET_COMPARE(Steer_Eng3,Steer_Eng3_CH,Angle0);
		PWM[2]=Angle0;
	}
	if(value==49)	 //1
	{	__HAL_TIM_SET_COMPARE(Steer_Eng3,Steer_Eng3_CH,Angle67_5);
		PWM[2]=Angle67_5;
	}
	if(value==50) //2
	{	__HAL_TIM_SET_COMPARE(Steer_Eng3,Steer_Eng3_CH,Angle135);
		PWM[2]=Angle135;
	}
	if(value==51) //3
	{	__HAL_TIM_SET_COMPARE(Steer_Eng3,Steer_Eng3_CH,Angle202_5);
		PWM[2]=Angle202_5;
	}
	if(value==52) //4
	{	__HAL_TIM_SET_COMPARE(Steer_Eng3,Steer_Eng3_CH,Angle270);
		PWM[2]=Angle270;
	}
	}
	
	if(Steer_Eng==100) //d->4�Ŷ��
	{
	if(value==48)  //0
	{	__HAL_TIM_SET_COMPARE(Steer_Eng4,Steer_Eng4_CH,Angle0);
		PWM[3]=Angle0;
	}
	if(value==49)	 //1
	{	__HAL_TIM_SET_COMPARE(Steer_Eng4,Steer_Eng4_CH,Angle67_5);
		PWM[3]=Angle67_5;
	}
	if(value==50) //2
	{	__HAL_TIM_SET_COMPARE(Steer_Eng4,Steer_Eng4_CH,Angle135);
		PWM[3]=Angle135;
	}
	if(value==51)//3
	{	__HAL_TIM_SET_COMPARE(Steer_Eng4,Steer_Eng4_CH,Angle202_5);
		PWM[3]=Angle202_5;
	}
	if(value==52) //4
	{	__HAL_TIM_SET_COMPARE(Steer_Eng4,Steer_Eng4_CH,Angle270);
		PWM[3]=Angle270;
	}
	}

	if(Steer_Eng==101) //e->5�Ŷ��
	{
	if(value==48)  //0
	{	__HAL_TIM_SET_COMPARE(Steer_Eng5,Steer_Eng5_CH,1500);  //ֹͣ�ź�
		PWM[4]=0;
	}
	if(value==49)	 //1
	{	__HAL_TIM_SET_COMPARE(Steer_Eng5,Steer_Eng5_CH,1200);  //צ���ſ�
		PWM[4]=1;
	}
	if(value==50)
	{
		__HAL_TIM_SET_COMPARE(Steer_Eng5,Steer_Eng5_CH,1800); //צ�ӱպ�
		PWM[4]=2;
	}
//	if(value==50) //2
//	{	__HAL_TIM_SET_COMPARE(Steer_Eng5,Steer_Eng5_CH,Angle135);
//		PWM[4]=Angle135;
//	}
//	if(value==51) //3
//	{	__HAL_TIM_SET_COMPARE(Steer_Eng5,Steer_Eng5_CH,Angle202_5);
//		PWM[4]=Angle202_5;
//	}
//	if(value==52) //4
//	{	__HAL_TIM_SET_COMPARE(Steer_Eng5,Steer_Eng5_CH,Angle270);
//		PWM[4]=Angle270;
//	}
	}
}


void Arm_Adjust() //��е��΢��
{
	value=0;
	
	uint8_t num=Steer_Eng-97;
	if(dir=='i') //i ASCII�� 105 ����
	{	PWM[num]=PWM[num]+add;
		HAL_GPIO_WritePin(GPIOH,GPIO_PIN_11,GPIO_PIN_SET);
	if(PWM[num]>=PWM_Max)PWM[num]=PWM_Max;	
	}
	if(dir=='j') //j ��С
	{PWM[num]=PWM[num]-add;
	if(PWM[num]<=PWM_Min)PWM[num]=PWM_Min;
	}
	
	if(Steer_Eng==97)  //a
	{	__HAL_TIM_SET_COMPARE(Steer_Eng1,Steer_Eng1_CH,PWM[num]);
	}
	if(Steer_Eng==98)	 //b
	{	__HAL_TIM_SET_COMPARE(Steer_Eng2,Steer_Eng2_CH,PWM[num]);
	}
	if(Steer_Eng==99) //c
	{	__HAL_TIM_SET_COMPARE(Steer_Eng3,Steer_Eng3_CH,PWM[num]);
	}
	if(Steer_Eng==100) //d
	{	__HAL_TIM_SET_COMPARE(Steer_Eng4,Steer_Eng4_CH,PWM[num]);
	}
	if(Steer_Eng==101) //4
	{	__HAL_TIM_SET_COMPARE(Steer_Eng5,Steer_Eng5_CH,PWM[num]);
	}
			
}



