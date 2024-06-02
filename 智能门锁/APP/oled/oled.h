#ifndef _oled_H
#define _oled_H

#include "system.h"


#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据


//OLED控制用函数
void OLED_WR_Byte(u8 dat,u8 cmd);	 
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);

void OLED_Init(void);
void OLED_Clear(void);
void OLED_Clear_NOupdate(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size); 
void OLED_Show_Font(u16 x,u16 y,u8 fnum);
void OLED_Show_Image(u8 fnum);   
void OLED_Set_Pos(unsigned char x, unsigned char y);	   							   		    

void Show_Str(u16 x,u16 y,u16 width,u16 height,u8*str,u8 size,u8 mode);

u16 LCD_BGR2RGB(u16 c);
void Gui_Circle(u16 X,u16 Y,u16 R,u16 fc); 
void Gui_DrawLine(u16 x0, u16 y0,u16 x1, u16 y1,u16 Color);  
void Gui_box(u16 x, u16 y, u16 w, u16 h,u16 bc);
void Gui_box2(u16 x,u16 y,u16 w,u16 h, u8 mode);
void DisplayButtonDown(u16 x1,u16 y1,u16 x2,u16 y2);
void DisplayButtonUp(u16 x1,u16 y1,u16 x2,u16 y2);
void Gui_DrawFont_GBK12(u16 x, u16 y, u16 fc, u16 bc, u8 *s);
void Gui_DrawFont_GBK16(u16 x, u16 y, u16 fc, u16 bc, u8 *s);
void Gui_DrawFont_GBK24(u16 x, u16 y, u16 fc, u16 bc, u8 *s);







void OLED_Fill_rectangle(u8 x0,u8 y0,u8 width,u8 height,u8 color);
void OLED_DrawLine(u8 x1, u8 y1, u8 x2, u8 y2,u8 color);
void OLED_DrawRectangle(u8 x1, u8 y1, u8 x2, u8 y2,u8 color);
void OLED_Draw_Circle(u8 x0,u8 y0,u8 r,u8 color);
void OLED_Draw_Bigpoint(u8 x0,u8 y0,u8 color);
void OLED_Draw_vline(u8 x0,u8 y0,u8 len,u8 color);
void OLED_Draw_hline(u8 x0,u8 y0,u8 len,u8 color);
void OLED_Fill_circle(u8 x0,u8 y0,u8 r,u8 color);
void OLED_Draw_ellipse(u8 x0, u8 y0, u8 rx, u8 ry,u8 color);
void OLED_DrawBMP(u8 x0, u8 y0,u8 x1, u8 y1,u8 BMP[]);






#endif
