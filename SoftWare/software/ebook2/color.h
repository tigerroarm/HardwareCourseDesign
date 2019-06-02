/*
 * color.h
 *
 *  Created on: 2019年4月28日
 *      Author: 12757
 */

#ifndef COLOR_H_
#define COLOR_H_


//16位色：5(R)+6(G)+5(B)
typedef unsigned short color_u16;
//8位色，256色，可使用色板查询对应的16位色
typedef unsigned char color_u8;

typedef struct
{
	unsigned char redVal;  //5bit [15:11]
	unsigned char greenVal;//6bit [10: 5]
	unsigned char blueVal; //5bit [4 : 0]
} ColorRGB;

//8位色与16位色的映射表
#define COLOR_FORM_X_NUM 16//横向颜色个数
#define COLOR_FORM_Y_NUM 16//纵向颜色个数
#define COLOR_FORM_SIZE 256
extern const color_u16 color_form[];


//将RGB 24位色转换为16位色
color_u16 getColor16BitByRGB( color_u8 red, color_u8 green, color_u8 blue );

#endif /* COLOR_H_ */
