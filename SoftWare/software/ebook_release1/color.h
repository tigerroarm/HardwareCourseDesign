/*
 * color.h
 *
 *  Created on: 2019��4��28��
 *      Author: 12757
 */

#ifndef COLOR_H_
#define COLOR_H_


//16λɫ��5(R)+6(G)+5(B)
typedef unsigned short color_u16;
//8λɫ��256ɫ����ʹ��ɫ���ѯ��Ӧ��16λɫ
typedef unsigned char color_u8;

typedef struct
{
	unsigned char redVal;  //5bit [15:11]
	unsigned char greenVal;//6bit [10: 5]
	unsigned char blueVal; //5bit [4 : 0]
} ColorRGB;

//8λɫ��16λɫ��ӳ���
#define COLOR_FORM_X_NUM 16//������ɫ����
#define COLOR_FORM_Y_NUM 16//������ɫ����
#define COLOR_FORM_SIZE 256
extern const color_u16 color_form[];


//��RGB 24λɫת��Ϊ16λɫ
color_u16 getColor16BitByRGB( color_u8 red, color_u8 green, color_u8 blue );

#endif /* COLOR_H_ */
