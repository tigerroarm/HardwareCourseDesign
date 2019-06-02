/*
 * touch.h
 *
 *  Created on: 2019��5��9��
 *      Author: lbl
 */

#ifndef TOUCH_H_
#define TOUCH_H_

#include <stdio.h>
#include <stdbool.h>
#include "system.h"
#include "altera_avalon_pio_regs.h"
#include "sys/alt_irq.h"              // �ж�

//����оƬ�ͺţ�XPT2046

#define TP_PRES_DOWN 0x80  //����������
#define TP_CATH_PRES 0x40  //�а���������
#define CT_MAX_TOUCH  5    //������֧�ֵĵ���,�̶�Ϊ5��

//********************************define by liukaixin start******************
typedef struct
{
	alt_u16 pos_x;
	alt_u16 pos_y;
	bool touchComeFlag;
	bool touchLeaveFlag;
} TouchPos;


extern TouchPos touchPosInst;

//********************************define by liukaixin stop********************

//������������
typedef struct
{
	unsigned char (*init)(void);			//��ʼ��������������
	unsigned char (*scan)(unsigned char);	//ɨ�败����.0,��Ļɨ��;1,��������;
	void (*adjust)(void);		 			//������У׼
	unsigned short int x[CT_MAX_TOUCH]; 	//��ǰ����
	unsigned short int y[CT_MAX_TOUCH];		//�����������5������,����������x[0],y[0]����:�˴�ɨ��ʱ,����������,��
											//x[4],y[4]�洢��һ�ΰ���ʱ������.
	unsigned char  sta;						//�ʵ�״̬
								//b7:����1/�ɿ�0;
	                            //b6:0,û�а�������;1,�а�������.
								//b5:����
								//b4~b0:���ݴ��������µĵ���(0,��ʾδ����,1��ʾ����)
/////////////////////������У׼����(����������ҪУ׼)//////////////////////
	float xfac;
	float yfac;
	short xoff;
	short yoff;
//�����Ĳ���,��������������������ȫ�ߵ�ʱ��Ҫ�õ�.
//b0:0,����(�ʺ�����ΪX����,����ΪY�����TP)
//   1,����(�ʺ�����ΪY����,����ΪX�����TP)
//b1~6:����.
//b7:0,������
//   1,������
	unsigned char touchtype;
}_m_tp_dev;

extern _m_tp_dev tp_dev;	 	//������������touch.c���涨��

//������оƬ��������
/*
 * RT_MISO	msg[0]			RT_MOSI	ctrl[1]
 * RT_PEN	pen_intr[0]		RT_BUSY	msg[1]
 * RT_CS	ctrl[0]			RT_CLK	ctrl[2]
 */
#define RT_MISO_BASE 	TOUCH_MSG_BASE			//0x8201410
#define RT_PEN_BASE  	TOUCH_PEN_INTR_BASE   	//0x8201440
#define RT_CS_BASE		TOUCH_CTRL_BASE  		//0x8200060
#define RT_MOSI_BASE	TOUCH_CTRL_BASE  		//0x8200060
#define RT_BUSY_BASE	TOUCH_MSG_BASE			//0x8201410
#define RT_CLK_BASE		TOUCH_CTRL_BASE  		//0x8200060

void delay_us(int x);
void TP_Write_Byte(unsigned char num);						//�����оƬд��һ������
unsigned short int TP_Read_AD(unsigned char CMD);			//��ȡADת��ֵ
unsigned short int TP_Read_XOY(unsigned char xy);			//���˲��������ȡ(X/Y)
unsigned char TP_Read_XY(unsigned short int *x,unsigned short int *y);		//˫�����ȡ(X+Y)
unsigned char TP_Read_XY2(unsigned short int *x,unsigned short int *y);		//����ǿ�˲���˫���������ȡ
void TP_Drow_Touch_Point(unsigned short int x,unsigned short int y,unsigned short int color);//��һ������У׼��
void TP_Draw_Big_Point(unsigned short int x,unsigned short int y,unsigned short int color);	//��һ�����
void TP_Save_Adjdata(void);								//����У׼����
unsigned char TP_Get_Adjdata(void);						//��ȡУ׼����
void TP_Adjust(void);									//������У׼
void TP_Adj_Info_Show(unsigned short int x0,unsigned short int y0,unsigned short int x1,unsigned short int y1,unsigned short int x2,unsigned short int y2,unsigned short int x3,unsigned short int y3,unsigned short int fac);//��ʾУ׼��Ϣ
unsigned char TP_Scan(unsigned char tp);				//ɨ��
unsigned char TP_Init(void);							//��ʼ��
void physical2logical(unsigned short xp, unsigned short yp, unsigned short* addx,  unsigned short* addy);						//ת���������굽�߼�����
void logical2physical(unsigned short xl, unsigned short yl, unsigned short* addx,  unsigned short* addy);						//ת���߼����굽��������

void intr_scan_headler(void* isr_context);	//�жϵ�ɨ�躯��
#endif /* TOUCH_H_ */