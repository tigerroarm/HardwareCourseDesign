/*
 * lcd.c
 *
 *  Created on: 2019��5��12��
 *      Author: lbl
 */

#include "lcd.h"
#include <stdio.h>

//LCD�Ļ�����ɫ�ͱ���ɫ
unsigned short int POINT_COLOR=0x0000;	//������ɫ
unsigned short int BACK_COLOR=0xFFFF;  //����ɫ

//����LCD��Ҫ����
//Ĭ��Ϊ����
_lcd_dev lcddev;

void LCD_Clear(unsigned short int color){
	printf("i am LCD_Clear\n");
	return;
}

void LCD_ShowNum(unsigned short int x,unsigned short int y,unsigned int num,unsigned char len,unsigned char size){
	printf("i am LCD_showNum\n");
	return;
}

void LCD_DrawLine(unsigned short int x1, unsigned short int y1, unsigned short int x2, unsigned short int y2){
	printf("i am LCD_DrawLine");
	return;

}

void LCD_DrawPoint(unsigned short int x,unsigned short int y){
	printf("i am LCD_DrawPoint\n");
	return;
}

void LCD_Draw_Circle(unsigned short int x0,unsigned short int y0,unsigned short int r){
	printf("i am LCD_DrawCircle\n");
	return;
}

void LCD_ShowString(unsigned short int x,unsigned short int y,unsigned short int width,unsigned short int height,unsigned char size,unsigned char *p){
	printf("i am LCD_showString\n");
	return;
}
