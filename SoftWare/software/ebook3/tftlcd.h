/*
 * tftflcd.h
 *
 *  Created on: 2019��5��28��
 *      Author: 12757
 */

#ifndef TFTFLCD_H_
#define TFTFLCD_H_

#include <stdio.h>
#include "system.h"
#include "altera_avalon_pio_regs.h"
#include "alt_types.h"
#include "unistd.h"

typedef struct
{
	alt_u16 width;
	alt_u16 height;
	alt_u16 dir;//����:0   ������1
	alt_u16 id;//lcd������id
	alt_u16	wramcmd;		//��ʼдgramָ��
	alt_u16 setxcmd;		//����x����ָ��
	alt_u16  setycmd;		//����y����ָ��
} TftLcdInfo;

//ɨ�跽����
#define L2R_U2D  0 //������,���ϵ���
#define L2R_D2U  1 //������,���µ���
#define R2L_U2D  2 //���ҵ���,���ϵ���
#define R2L_D2U  3 //���ҵ���,���µ���

#define U2D_L2R  4 //���ϵ���,������
#define U2D_R2L  5 //���ϵ���,���ҵ���
#define D2U_L2R  6 //���µ���,������
#define D2U_R2L  7 //���µ���,���ҵ���

#define DFT_SCAN_DIR  L2R_U2D  //Ĭ�ϵ�ɨ�跽��

//0����delay 1:delay
#define DELAY_SWITCH 0
#define DELAY_NUM 200

void delay(int time);

//����lcd��������
void LCD_WR_REG(alt_u16 cmd);
void LCD_WR_DATA(alt_u16 data);
alt_u16 LCD_RD_DATA(); //��data

//��дreg�Լ�����
void LCD_WriteReg(alt_u16 LCD_Reg,alt_u16 LCD_RegValue);
alt_u16 LCD_ReadReg(alt_u16 LCD_Reg); //д���ƺ����������ĺ���

//LCD��ʼ��
void LCD_init();
void LCD5310_init();

//LCD�����趨
void LCD_Display_Dir(alt_u8 dir);//���������趨
void LCD_Scan_Dir(alt_u8 dir);//ɨ����ʾ�����趨
//LCD�����趨
void LCD_Set_Window(alt_u16 sx,alt_u16 sy,alt_u16 width,alt_u16 height);	//���ô���

//GRAM����
void LCD_WriteRAM(alt_u16 RGB_Code);
void LCD_WriteRAM_Prepare(void);

//LCD���ܺ���
void LCD_SetCursor(alt_u16 Xpos, alt_u16 Ypos);
void LCD_DrawPoint(alt_u16 x,alt_u16 y,alt_u16 color);
void LCD_DrawLine(alt_u16 x1, alt_u16 y1, alt_u16 x2, alt_u16 y2, alt_u16 color);
void LCD_Clear(alt_u16 color);
void LCD_Fill(alt_u16 sx,alt_u16 sy,alt_u16 ex,alt_u16 ey,alt_u16 color);//��䵥ɫ
void LCD_Color_Fill(alt_u16 sx,alt_u16 sy,alt_u16 ex,alt_u16 ey,alt_u16 *color);//���ָ����ɫ
void LCD_Fast_DrawPoint(alt_u16 x,alt_u16 y,alt_u16 color);



#endif /* TFTFLCD_H_ */