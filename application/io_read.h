#ifndef IO_READ_H
#define IO_READ_H
#include "stm32f4xx_hal.h"


#define IO_CTRL			GPIOF   // SCK SDAר��	
#define IO_SCL        	GPIO_PIN_1	//TFT --SCL/SCK
#define IO_SDA        	GPIO_PIN_0	//TFT --SDA/DIN


void io_read_init();
void 	io_read();
int IRcvStr(uint8_t sla,uint8_t suba,uint8_t *s,uint8_t no);
int IRcvByte(uint8_t sla,uint8_t *c);
int ISendStr(uint8_t sla,uint8_t suba,uint8_t *s,uint8_t no);
int ISendByte(uint8_t sla,uint8_t c);
void IIC_Ack(uint8_t ack);//��������Ӧ����߲�Ӧ����ӻ�
uint8_t  IIC_Read_Byte(void);//�ӻ�����8λ���ݸ�����
uint8_t IIC_Wait_Ack(void);//�ȴ��ӻ�������Ӧ����߲�Ӧ��
void IIC_Send_Byte(uint8_t d);//��������8λ���ݸ��ӻ�MSB ��λ�ȷ�
void IIC_Stop(void);
void IIC_Start(void);//IIC��ʼ�ź�
void IIC_SDA_Mode_IN(void);
void IIC_SDA_Mode_OUT(void);
#endif