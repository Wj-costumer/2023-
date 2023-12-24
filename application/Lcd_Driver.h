#include "stm32f4xx_hal.h"
#include "stm32f407xx.h"

#define RED  	0xf800
#define GREEN	0x07e0
#define BLUE 	0x001f
#define WHITE	0xffff
#define BLACK	0x0000
#define YELLOW  0xFFE0
#define GRAY0   0xEF7D   	//灰色0 3165 00110 001011 00101
#define GRAY1   0x8410      	//灰色1      00000 000000 00000
#define GRAY2   0x4208      	//灰色2  1111111111011111




#define LCD_CTRL_1   	  GPIOB		//定义TFT数据端口
#define LCD_CTRL_2			GPIOI   // SCK SDA专用	
#define LCD_RS         	GPIO_PIN_7	//TFT --RS/DC
#define LCD_CS        	GPIO_PIN_14  //TFT --CS/CE
#define LCD_RST     		GPIO_PIN_15	//TFT --RST
#define LCD_SCL        	GPIO_PIN_12	//TFT --SCL/SCK
#define LCD_SDA        	GPIO_PIN_13	//TFT --SDA/DIN




//SET操作语句宏定义
#define	LCD_CS_SET  	LCD_CTRL_1->BSRR=LCD_CS    
#define	LCD_RS_SET  	LCD_CTRL_2->BSRR=LCD_RS    
#define	LCD_SDA_SET  	LCD_CTRL_1->BSRR=LCD_SDA    
#define	LCD_SCL_SET  	LCD_CTRL_1->BSRR=LCD_SCL    
#define	LCD_RST_SET  	LCD_CTRL_1->BSRR=LCD_RST    
#define	LCD_LED_SET  	LCD_CTRL_1->BSRR=LCD_LED   

//RESET操作语句宏定义
#define	LCD_CS_CLR  	LCD_CTRL_1->BSRR=(uint32_t)LCD_CS << 16U    
#define	LCD_RS_CLR  	LCD_CTRL_2->BSRR=(uint32_t)LCD_RS << 16U    
#define	LCD_SDA_CLR  	LCD_CTRL_1->BSRR=(uint32_t)LCD_SDA << 16U    
#define	LCD_SCL_CLR  	LCD_CTRL_1->BSRR=(uint32_t)LCD_SCL << 16U    
#define	LCD_RST_CLR  	LCD_CTRL_1->BSRR=(uint32_t)LCD_RST << 16U 

#define LCD_DATAOUT(x) LCD_DATA->ODR=x; //数据输出
#define LCD_DATAIN     LCD_DATA->IDR;   //数据输入

#define LCD_WR_DATA(data){\
LCD_RS_SET;\
LCD_CS_CLR;\
LCD_DATAOUT(data);\
LCD_WR_CLR;\
LCD_WR_SET;\
LCD_CS_SET;\
} 



void LCD_GPIO_Init(void);
void Lcd_WriteIndex(uint8_t Index);
void Lcd_WriteData(uint8_t Data);
void Lcd_WriteReg(uint8_t Index,uint8_t Data);
uint16_t Lcd_ReadReg(uint8_t LCD_Reg);
void Lcd_Reset(void);
void Lcd_Init(void);
void Lcd_Clear(uint16_t Color);
void Lcd_SetXY(uint16_t x,uint16_t y);
void Gui_DrawPoint(uint16_t x,uint16_t y,uint16_t Data);
unsigned int Lcd_ReadPoint(uint16_t x,uint16_t y);

