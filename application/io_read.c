#include "io_read.h"
#include "gpio.h"
#include "stm32f4xx_hal.h"
#include "bsp_delay.h"
static GPIO_InitTypeDef GPIO_InitStruct;

#define SCL_Type     GPIOF
#define SDA_Type     GPIOF

#define SCL_GPIO    GPIO_PIN_1
#define SDA_GPIO    GPIO_PIN_0
#define SDA_OUT(X)   if(X) \
					 HAL_GPIO_WritePin(SDA_Type, SDA_GPIO, GPIO_PIN_SET); \
				     else  \
					 HAL_GPIO_WritePin(SDA_Type, SDA_GPIO, GPIO_PIN_RESET);
 
#define SCL_OUT(X)   if(X) \
					 HAL_GPIO_WritePin(SCL_Type, SCL_GPIO, GPIO_PIN_SET); \
				     else  \
					 HAL_GPIO_WritePin(SCL_Type, SCL_GPIO, GPIO_PIN_RESET);    
                                     
#define SDA_IN		 HAL_GPIO_ReadPin(SDA_Type,SDA_GPIO)//ֻ������ģʽ���ܶ�ȡ��ƽ״̬
						 
#define	PCA9698	0x40       //PCA9698��ַ 
uint8_t udata_IOC4[] = {0Xff}; /* ���� I/O4 ��Ϊ��������� */
#define u8  uint8_t
#define u16 uint16_t
#define u32 uint32_t
						 
/*****************************************
  SDA����ת��Ϊ OUT���ģʽ(���ģʽ��ֹͣ ��ʼ�ź�) 
******************************************/
void IIC_SDA_Mode_OUT(void)
{
  GPIO_InitStruct.Pin = SDA_GPIO;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(SDA_Type, &GPIO_InitStruct);
}
/*****************************************
  SDA����ת��Ϊ ����ģʽ(����ģʽ������������) 
******************************************/
void IIC_SDA_Mode_IN(void)
{
  GPIO_InitStruct.Pin = SDA_GPIO;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(SDA_Type, &GPIO_InitStruct);
}
/*****************************************
  IIC��ʼ�ź�
******************************************/
void IIC_Start(void)//IIC��ʼ�ź�
{
	//����Ϊ���ģʽ
	IIC_SDA_Mode_OUT();
	
	//����״̬���������Ǹߵ�ƽ
	SDA_OUT(1);
	SCL_OUT(1) ;
	delay_us(5);
	
	//����������
	SDA_OUT(0);
	delay_us(5);
	
	//������ʱ����
	SCL_OUT(0) ;
	delay_us(5);
}
//IICֹͣ�ź�
void IIC_Stop(void)
{
	//����Ϊ���ģʽ
	IIC_SDA_Mode_OUT();
 
	//����
	SDA_OUT(0);
	SCL_OUT(0);
	delay_us(5);
	
	//ʱ����������
	SCL_OUT(1);
	delay_us(5);
	
	//�ٰ�����������
	SDA_OUT(1);
	delay_us(5);
}
void IIC_Send_Byte(uint8_t d)//��������8λ���ݸ��ӻ�MSB ��λ�ȷ�
{
    uint8_t i = 0;
    	//����Ϊ���ģʽ
	IIC_SDA_Mode_OUT();
	
	SDA_OUT(0);
	SCL_OUT(0);
	delay_us(5);
	for(i=0;i<8;i++)
	{
		if(d & (0x1<<(7-i)))//��ʾ������1
			SDA_OUT(1)
		else
			SDA_OUT(0);
		
		delay_us(5);
		SCL_OUT(1);//����ʱ���ߣ����߶Է�����Զ���
		
		delay_us(5);
		SCL_OUT(0);//����ʱ���ߣ����߶Է�����ʱ���������׼������
	}
 
}
uint8_t IIC_Wait_Ack(void)//�ȴ��ӻ�������Ӧ����߲�Ӧ��
{
	uint8_t ack = 0;
		u8 ucErrTime=0;
	//����Ϊ����ģʽ
	IIC_SDA_Mode_IN();
	
	//ʱ��������,ʱ����Ϊ�ߵ�ƽ�ڼ䣬���������ݻ���ack������Ч��
	SCL_OUT(1);
	delay_us(5);
	while(SDA_IN)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 0;
		}		
	}
	ack=1;
	
	SCL_OUT(0);
    
	delay_us(5);
	return ack;

}
uint8_t  IIC_Read_Byte(void)//�ӻ�����8λ���ݸ�����
{
	uint8_t i =0;
	uint8_t data = 0;
	//����Ϊ����ģʽ
	IIC_SDA_Mode_IN();
	//������ʱ���ߣ�׼������
	SCL_OUT(0);
	delay_us(5);
	
	for(i=0;i<8;i++)
	{
		SCL_OUT(1);//ʱ����Ϊ�ߵ�ƽ�ڼ����ݲ�����Ч��
		delay_us(5);
		if(SDA_IN == 1)
			data |= (0x1<<(7-i));//���ݾ���1
		else
			data &= ~(0x1<<(7-i));//���ݾ���0
		
		SCL_OUT (0);//���߶Է���ʱ׼�����ݣ��ȱ��д
		delay_us(5);
	}
	return data;
}
void IIC_Ack(uint8_t ack)//��������Ӧ����߲�Ӧ����ӻ�
{
	//����Ϊ���ģʽ
	IIC_SDA_Mode_OUT();
	
	SDA_OUT(0);
	SCL_OUT(0);
	delay_us(5);
	
	SDA_OUT(ack) ;//���͸�/�͵�ƽ--->���Ͳ�Ӧ��/Ӧ��
	delay_us(5);
	
	SCL_OUT(1);//���ߴӻ����Ѿ�׼�������ݣ�����Զ�ȡ��
	delay_us(5);
	
	SCL_OUT (0);//����ʱ���ߣ�����ack����
	delay_us(5);

}
//***************************************************************/
//�����ӵ�ַ���������ֽ����ݺ��� ISendByte(u8 sla,u8 c)
//���β�sla:��ַ��c���������ݣ��޷���ֵ
//***************************************************************/
int ISendByte(uint8_t sla,uint8_t c)
{
	IIC_Start();         //��������
	IIC_Send_Byte(sla);       //����������ַ
	if(IIC_Wait_Ack()==0)
		return(0);
	IIC_Send_Byte(c);         //��������
	if(IIC_Wait_Ack()==0)
		return(0);
	IIC_Stop();          //�������� 
	return(1);
	
	
}
//***************************************************************/
//�����ӵ�ַ�������Ͷ��ֽ����ݺ���ISendStr(u8 sla,u8 suba,u8 *s,u8 no)
//���β�sla:��ַ��suba���ӵ�ַ��*s���������飻no���ֽڸ������з���ֵ1
//***************************************************************/
int ISendStr(uint8_t sla,uint8_t suba,uint8_t *s,uint8_t no)
{
	uint8_t i;
	
	IIC_Start();          //��������

	IIC_Send_Byte(sla);       //����������ַ
	if(IIC_Wait_Ack()==0)
		return(0);

	IIC_Send_Byte(suba);      //���������ӵ�ַ
	if(IIC_Wait_Ack()==0)
		return(0);
	
	for(i=0;i<no;i++)
	{   
		IIC_Send_Byte(*s);       //��������
		if(IIC_Wait_Ack()==0)return(0);
		s++;
	}
	 
	IIC_Stop();          //��������
	 
	return(1);
}	
//***************************************************************/
//�����ӵ�ַ�������ֽ����ݺ���IRcvByte(u8 sla,u8 *c)
//���β�sla:��ַ��suba���������ݣ�*c�����з���ֵ1
//***************************************************************/
int IRcvByte(uint8_t sla,uint8_t *c)
{
	IIC_Start();        //��������

	IIC_Send_Byte(sla+1);    //����������ַ
	if(IIC_Wait_Ack()==0)
		return(0);

	*c=IIC_Read_Byte();       //��ȡ����
	IIC_Ack(1);         //���ͷǾʹ�λ
	IIC_Stop();         //��������
	return(1);
}	
//***************************************************************/
//�����ӵ�ַ�������ն��ֽ����ݺ���IRcvStr(u8 sla,u8 suba,u8 *s,u8 no)
//���β�sla:��ַ��suba���ӵ�ַ��*s���������飻no���ֽڸ������з���ֵ1
//***************************************************************/
int IRcvStr(uint8_t sla,uint8_t suba,uint8_t *s,uint8_t no)
{
	uint8_t i;
	
	IIC_Start();           //��������
	IIC_Send_Byte(sla);         //����������ַ
	if(IIC_Wait_Ack()==0)
		return(0);
	IIC_Send_Byte(suba);        //���������ӵ�ַ
	if(IIC_Wait_Ack()==0)
		return(0);	
	IIC_Start();
	IIC_Send_Byte(sla+1);
	if(IIC_Wait_Ack()==0)
		return(0);
	for(i=0;i<no-1;i++)
	{   
		*s=IIC_Read_Byte();       //��������
		IIC_Ack(0);         //���;ʹ�λ 
		s++;
	} 
	*s=IIC_Read_Byte();
	IIC_Ack(1);  			   //���ͷ�Ӧλ
	IIC_Stop();          //�������� 
	return(1);
}
uint8_t receive[4];
void io_read_init()
{
	
	u8 All_ctrl[1]={0x80};
	//����ȫ����ƼĴ������飨����ΪĬ��ֵ��
	u8 Select_Mode[1]={0x12};
	//����ģʽѡ��Ĵ�������������ΪӦ��ʱ����ı� ʹ���ж���Ӧ��
	u8 GPIO_H[1]={0xff};
	//����I/O�˿�״̬����
	u8 GPIO_L[1]={0x00};
	//����I/O�˿�״̬����
  //u8 In_array[1]={0x7f};
	//����I/Oĳ�˿ں�Ϊ�жϱ�־λ
	delay_ms(10);
	ISendStr(PCA9698,0X18,GPIO_H,1);//����A��I/OΪ����
	ISendStr(PCA9698,0X19,GPIO_H,1);//����B��I/OΪ����
	ISendStr(PCA9698,0X1A,GPIO_H,1);//����C��I/OΪ����
	ISendStr(PCA9698,0X1B,GPIO_H,1);//����D��I/OΪ����
	ISendStr(PCA9698,0X1C,GPIO_H,1);//����E��I/OΪ����


}
void 	io_read()
{
//		for(int i=0;i<=3;i++)	
//		receive[i]=0;
		IRcvStr(PCA9698,0X00,&receive[0],1);//��ȡA�ĵ�ƽ
		IRcvStr(PCA9698,0X01,&receive[1],1);//��ȡB�ĵ�ƽ
		IRcvStr(PCA9698,0X02,&receive[2],1);//��ȡC�ĵ�ƽ
	  IRcvStr(PCA9698,0X03,&receive[3],1);//��ȡD�ĵ�ƽ		
	
}