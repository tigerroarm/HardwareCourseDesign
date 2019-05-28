/*
 * screen.h
 *
 * 本库实现屏幕内容的显示
 *
 *  Created on: 2019年4月27日
 *      Author: 12757
 */

#ifndef SCREEN_H_
#define SCREEN_H_

#include <stdio.h>
#include "screen_type.h"
#include "font_manage.h"
#include "txt_read.h"

//整个屏幕变量
extern ScreenContainer tftlcdScreenInst;

//计算有效文本全部显示时的像素宽度
short getWidthOfText( char *str, short len );

//获取文本中汉字与字母的分布表(返回动态数组，记得free)
char *getWordLetterArrange( char *str, short len );


#endif /* SCREEN_H_ */
