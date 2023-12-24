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
                                     
#define SDA_IN		 HAL_GPIO_ReadPin(SDA_Type,SDA_GPIO)//只有输入模式才能读取电平状态
						 
#define	PCA9698	0x40       //PCA9698地址 
uint8_t udata_IOC4[] = {0Xff}; /* 设置 I/O4 口为输入的数据 */
#define u8  uint8_t
#define u16 uint16_t
#define u32 uint32_t
						 
/*****************************************
  SDA引脚转变为 OUT输出模式(输出模式给停止 开始信号) 
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
  SDA引脚转变为 输入模式(输入模式传输具体的数据) 
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
  IIC开始信号
******************************************/
void IIC_Start(void)//IIC开始信号
{
	//设置为输出模式
	IIC_SDA_Mode_OUT();
	
	//空闲状态两个引脚是高电平
	SDA_OUT(1);
	SCL_OUT(1) ;
	delay_us(5);
	
	//拉低数据线
	SDA_OUT(0);
	delay_us(5);
	
	//再拉低时钟线
	SCL_OUT(0) ;
	delay_us(5);
}
//IIC停止信号
void IIC_Stop(void)
{
	//设置为输出模式
	IIC_SDA_Mode_OUT();
 
	//拉低
	SDA_OUT(0);
	SCL_OUT(0);
	delay_us(5);
	
	//时钟线先拉高
	SCL_OUT(1);
	delay_us(5);
	
	//再把数据线拉高
	SDA_OUT(1);
	delay_us(5);
}
void IIC_Send_Byte(uint8_t d)//主机发送8位数据给从机MSB 高位先发
{
    uint8_t i = 0;
    	//设置为输出模式
	IIC_SDA_Mode_OUT();
	
	SDA_OUT(0);
	SCL_OUT(0);
	delay_us(5);
	for(i=0;i<8;i++)
	{
		if(d & (0x1<<(7-i)))//表示数据是1
			SDA_OUT(1)
		else
			SDA_OUT(0);
		
		delay_us(5);
		SCL_OUT(1);//拉高时钟线，告诉对方你可以读了
		
		delay_us(5);
		SCL_OUT(0);//拉低时钟线，告诉对方你暂时别读，我在准备数据
	}
 
}
uint8_t IIC_Wait_Ack(void)//等待从机给主机应答或者不应答
{
	uint8_t ack = 0;
		u8 ucErrTime=0;
	//设置为输入模式
	IIC_SDA_Mode_IN();
	
	//时钟线拉高,时钟线为高电平期间，不管是数据还是ack都是有效的
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
uint8_t  IIC_Read_Byte(void)//从机发送8位数据给主机
{
	uint8_t i =0;
	uint8_t data = 0;
	//设置为输入模式
	IIC_SDA_Mode_IN();
	//先拉低时钟线，准备数据
	SCL_OUT(0);
	delay_us(5);
	
	for(i=0;i<8;i++)
	{
		SCL_OUT(1);//时钟线为高电平期间数据才是有效的
		delay_us(5);
		if(SDA_IN == 1)
			data |= (0x1<<(7-i));//数据就是1
		else
			data &= ~(0x1<<(7-i));//数据就是0
		
		SCL_OUT (0);//告诉对方此时准备数据，先别读写
		delay_us(5);
	}
	return data;
}
void IIC_Ack(uint8_t ack)//主机发送应答或者不应答给从机
{
	//设置为输出模式
	IIC_SDA_Mode_OUT();
	
	SDA_OUT(0);
	SCL_OUT(0);
	delay_us(5);
	
	SDA_OUT(ack) ;//发送高/低电平--->发送不应答/应答
	delay_us(5);
	
	SCL_OUT(1);//告诉从机我已经准备好数据，你可以读取了
	delay_us(5);
	
	SCL_OUT (0);//拉低时钟线，发送ack结束
	delay_us(5);

}
//***************************************************************/
//向无子地址器件发送字节数据函数 ISendByte(u8 sla,u8 c)
//有形参sla:地址；c：发送数据；无返回值
//***************************************************************/
int ISendByte(uint8_t sla,uint8_t c)
{
	IIC_Start();         //启动总线
	IIC_Send_Byte(sla);       //发送器件地址
	if(IIC_Wait_Ack()==0)
		return(0);
	IIC_Send_Byte(c);         //发送数据
	if(IIC_Wait_Ack()==0)
		return(0);
	IIC_Stop();          //结束总线 
	return(1);
	
	
}
//***************************************************************/
//向有子地址器件发送多字节数据函数ISendStr(u8 sla,u8 suba,u8 *s,u8 no)
//有形参sla:地址；suba：子地址；*s：数据数组；no：字节个数；有返回值1
//***************************************************************/
int ISendStr(uint8_t sla,uint8_t suba,uint8_t *s,uint8_t no)
{
	uint8_t i;
	
	IIC_Start();          //启动总线

	IIC_Send_Byte(sla);       //发送器件地址
	if(IIC_Wait_Ack()==0)
		return(0);

	IIC_Send_Byte(suba);      //发送器件子地址
	if(IIC_Wait_Ack()==0)
		return(0);
	
	for(i=0;i<no;i++)
	{   
		IIC_Send_Byte(*s);       //发送数据
		if(IIC_Wait_Ack()==0)return(0);
		s++;
	}
	 
	IIC_Stop();          //结束总线
	 
	return(1);
}	
//***************************************************************/
//向无子地址器件读字节数据函数IRcvByte(u8 sla,u8 *c)
//有形参sla:地址；suba：发送数据；*c：；有返回值1
//***************************************************************/
int IRcvByte(uint8_t sla,uint8_t *c)
{
	IIC_Start();        //启动总线

	IIC_Send_Byte(sla+1);    //发送器件地址
	if(IIC_Wait_Ack()==0)
		return(0);

	*c=IIC_Read_Byte();       //读取数据
	IIC_Ack(1);         //发送非就答位
	IIC_Stop();         //结束总线
	return(1);
}	
//***************************************************************/
//向有子地址器件接收多字节数据函数IRcvStr(u8 sla,u8 suba,u8 *s,u8 no)
//有形参sla:地址；suba：子地址；*s：数据数组；no：字节个数；有返回值1
//***************************************************************/
int IRcvStr(uint8_t sla,uint8_t suba,uint8_t *s,uint8_t no)
{
	uint8_t i;
	
	IIC_Start();           //启动总线
	IIC_Send_Byte(sla);         //发送器件地址
	if(IIC_Wait_Ack()==0)
		return(0);
	IIC_Send_Byte(suba);        //发送器件子地址
	if(IIC_Wait_Ack()==0)
		return(0);	
	IIC_Start();
	IIC_Send_Byte(sla+1);
	if(IIC_Wait_Ack()==0)
		return(0);
	for(i=0;i<no-1;i++)
	{   
		*s=IIC_Read_Byte();       //发送数据
		IIC_Ack(0);         //发送就答位 
		s++;
	} 
	*s=IIC_Read_Byte();
	IIC_Ack(1);  			   //发送非应位
	IIC_Stop();          //结束总线 
	return(1);
}
uint8_t receive[4];
void io_read_init()
{
	
	u8 All_ctrl[1]={0x80};
	//设置全组控制寄存器数组（这里为默认值）
	u8 Select_Mode[1]={0x12};
	//设置模式选择寄存器（这里配置为应答时输出改变 使能中断响应）
	u8 GPIO_H[1]={0xff};
	//设置I/O端口状态数组
	u8 GPIO_L[1]={0x00};
	//设置I/O端口状态数组
  //u8 In_array[1]={0x7f};
	//设置I/O某端口号为中断标志位
	delay_ms(10);
	ISendStr(PCA9698,0X18,GPIO_H,1);//配置A组I/O为输入
	ISendStr(PCA9698,0X19,GPIO_H,1);//配置B组I/O为输入
	ISendStr(PCA9698,0X1A,GPIO_H,1);//配置C组I/O为输入
	ISendStr(PCA9698,0X1B,GPIO_H,1);//配置D组I/O为输入
	ISendStr(PCA9698,0X1C,GPIO_H,1);//配置E组I/O为输入


}
void 	io_read()
{
//		for(int i=0;i<=3;i++)	
//		receive[i]=0;
		IRcvStr(PCA9698,0X00,&receive[0],1);//读取A的电平
		IRcvStr(PCA9698,0X01,&receive[1],1);//读取B的电平
		IRcvStr(PCA9698,0X02,&receive[2],1);//读取C的电平
	  IRcvStr(PCA9698,0X03,&receive[3],1);//读取D的电平		
	
}