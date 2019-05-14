/*
 * lcd.h
 *
 *  Created on: 2019��5��12��
 *      Author: lbl
 */

#ifndef LCD_H_
#define LCD_H_

//LCD��Ҫ������
typedef struct
{
	unsigned short int width;			//LCD ���
	unsigned short int height;			//LCD �߶�
	unsigned short int id;				//LCD ID
	unsigned char  dir;			//���������������ƣ�0��������1��������
	unsigned short int	wramcmd;		//��ʼдgramָ��
	unsigned short int setxcmd;		//����x����ָ��
	unsigned short int  setycmd;		//����y����ָ��
}_lcd_dev;

//LCD����
extern _lcd_dev lcddev;	//����LCD��Ҫ����
//LCD�Ļ�����ɫ�ͱ���ɫ
extern unsigned short int  POINT_COLOR;//Ĭ�Ϻ�ɫ
extern unsigned short int  BACK_COLOR; //������ɫ.Ĭ��Ϊ��ɫ

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

//������ɫ
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
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ

#define LIGHTGREEN     	 0X841F //ǳ��ɫ
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)

void LCD_Clear(unsigned short int color);
void LCD_DrawLine(unsigned short int x1, unsigned short int y1, unsigned short int x2, unsigned short int y2);							//����
void LCD_ShowNum(unsigned short int x,unsigned short int y,unsigned int num,unsigned char len,unsigned char size);
void LCD_DrawPoint(unsigned short int x,unsigned short int y);
void LCD_Draw_Circle(unsigned short int x0,unsigned short int y0,unsigned short int r);
void LCD_ShowString(unsigned short int x,unsigned short int y,unsigned short int width,unsigned short int height,unsigned char size,unsigned char *p);
#endif /* LCD_H_ */
