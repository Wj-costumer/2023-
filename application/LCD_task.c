#include "LCD_task.h"
#include "GUI.h"
#include "Lcd_Driver.h"
#include "main.h"

#include "freertos.h"
#include "cmsis_os.h"
extern int find_QRCODE_flag;
extern uint8_t QRCODE[6];
extern uint8_t COLOR[6];
extern int find_all_color_flag[2];
extern uint8_t wifi_corder[6];
extern uint8_t find_wifi_corder_flag;
extern uint8_t wifi_success;
void LCD_TASK()
{
	Lcd_Clear(WHITE);
	while(1)
	{
		if(wifi_success)
		{
			
			Gui_DrawFont_Num32(10, 10, BLACK, WHITE, 1);			
		}
		if(find_wifi_corder_flag)
		{
			Gui_DrawFont_Num32(10, 10, BLACK, WHITE, wifi_corder[0]);
			Gui_DrawFont_Num32(46, 10, BLACK, WHITE, wifi_corder[1]);
			Gui_DrawFont_Num32(82, 10, BLACK, WHITE, wifi_corder[2]);
			Gui_DrawFont_Num32(10, 50, BLACK, WHITE, wifi_corder[3]);
			Gui_DrawFont_Num32(46, 50, BLACK, WHITE, wifi_corder[4]);
			Gui_DrawFont_Num32(82, 50, BLACK, WHITE, wifi_corder[5]);			
		}
//	Gui_DrawFont_Num32(10, 10, BLACK, WHITE, 0);
		if(find_QRCODE_flag)
		{
			Gui_DrawFont_Num32(10, 10, BLACK, WHITE, QRCODE[0]);
			Gui_DrawFont_Num32(46, 10, BLACK, WHITE, QRCODE[1]);
			Gui_DrawFont_Num32(82, 10, BLACK, WHITE, QRCODE[2]);
			Gui_DrawFont_Num32(10, 50, BLACK, WHITE, QRCODE[3]);
			Gui_DrawFont_Num32(46, 50, BLACK, WHITE, QRCODE[4]);
			Gui_DrawFont_Num32(82, 50, BLACK, WHITE, QRCODE[5]);
		}
		if(find_all_color_flag[0])
		{
			Lcd_Clear(BLACK);
			Gui_DrawFont_Num32(10, 10, WHITE, BLACK, COLOR[0]);
			Gui_DrawFont_Num32(46, 10, WHITE, BLACK, COLOR[1]);
			Gui_DrawFont_Num32(82, 10, WHITE, BLACK, COLOR[2]);
			find_QRCODE_flag = 0;
			find_all_color_flag[0] = 0;
		}
		if(find_all_color_flag[1])
		{
			Lcd_Clear(BLACK);
			Gui_DrawFont_Num32(10, 10, WHITE, BLACK, COLOR[3]);
			Gui_DrawFont_Num32(46, 10, WHITE, BLACK, COLOR[4]);
			Gui_DrawFont_Num32(82, 10, WHITE, BLACK, COLOR[5]);
			find_all_color_flag[0] = 0;
			find_all_color_flag[1] = 0;
		}
		if(find_wifi_corder_flag)
		{
			Gui_DrawFont_Num32(10, 10, BLACK, WHITE, wifi_corder[0]);
			Gui_DrawFont_Num32(46, 10, BLACK, WHITE, wifi_corder[1]);
			Gui_DrawFont_Num32(82, 10, BLACK, WHITE, wifi_corder[2]);
			Gui_DrawFont_Num32(10, 50, BLACK, WHITE, wifi_corder[3]);
			Gui_DrawFont_Num32(46, 50, BLACK, WHITE, wifi_corder[4]);
			Gui_DrawFont_Num32(82, 50, BLACK, WHITE, wifi_corder[5]);			
			
		}
		osDelay(1);
	}
}