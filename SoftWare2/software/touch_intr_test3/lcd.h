/*
 * lcd.h
 *
 *  Created on: 2019年5月12日
 *      Author: lbl
 */

#ifndef LCD_H_
#define LCD_H_

//LCD重要参数集
typedef struct
{
	unsigned short int width;			//LCD 宽度
	unsigned short int height;			//LCD 高度
	unsigned short int id;				//LCD ID
	unsigned char  dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。
	unsigned short int	wramcmd;		//开始写gram指令
	unsigned short int setxcmd;		//设置x坐标指令
	unsigned short int  setycmd;		//设置y坐标指令
}_lcd_dev;

//LCD参数
extern _lcd_dev lcddev;	//管理LCD重要参数
//LCD的画笔颜色和背景色
extern unsigned short int  POINT_COLOR;//默认红色
extern unsigned short int  BACK_COLOR; //背景颜色.默认为白色

//扫描方向定义
#define L2R_U2D  0 //从左到右,从上到下
#define L2R_D2U  1 //从左到右,从下到上
#define R2L_U2D  2 //从右到左,从上到下
#define R2L_D2U  3 //从右到左,从下到上

#define U2D_L2R  4 //从上到下,从左到右
#define U2D_R2L  5 //从上到下,从右到左
#define D2U_L2R  6 //从下到上,从左到右
#define D2U_R2L  7 //从下到上,从右到左

#define DFT_SCAN_DIR  L2R_U2D  //默认的扫描方向

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000
#define BLUE         	 0x001F
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色

#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

void LCD_Clear(unsigned short int color);
void LCD_DrawLine(unsigned short int x1, unsigned short int y1, unsigned short int x2, unsigned short int y2);							//画线
void LCD_ShowNum(unsigned short int x,unsigned short int y,unsigned int num,unsigned char len,unsigned char size);
void LCD_DrawPoint(unsigned short int x,unsigned short int y);
void LCD_Draw_Circle(unsigned short int x0,unsigned short int y0,unsigned short int r);
void LCD_ShowString(unsigned short int x,unsigned short int y,unsigned short int width,unsigned short int height,unsigned char size,unsigned char *p);
#endif /* LCD_H_ */
