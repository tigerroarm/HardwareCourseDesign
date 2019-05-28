/*
 * screen.c
 *
 *  Created on: 2019年4月26日
 *      Author: 12757
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "screen.h"
#include "color.h"



//	sprintf(str, "%02d", num);可以将数字变为字符串

//整个屏幕变量
ScreenContainer tftlcdScreenInst;

ColorInfo defaultColorInfo = {  DEFAULT_BORDER_COLOR, DEFAULT_BKG_COLOR, DEFAULT_OBJ_COLOR, true, true };

char *str1 = "Happy";


//获取文本中汉字与字母的分布表(值为'c'，代表汉字，值为'e'，代表英文字母
//用完记得free返回的char[]
char *getWordLetterArrange( char *str, short len )
{
	char *strType = (char*)malloc( sizeof( char ) * (len+1) );
	short strTypeVaildNum = 0;

	int i;
	for ( i = 0; i < len; i ++ )
	{
		char curChar = str[i];
		if ( curChar == 0 )//结束标志
		{
			break;
		}
		if ( curChar < 0 )//当前字节为汉字高字节
		{
			strType[strTypeVaildNum] = 'c';//中文CHINESE
			i = i + 1;
			if ( i == len || str[i] == 0 )//到了字符串结束，低字节却不存在，有问题
			{
				strType[strTypeVaildNum] = 0;//结束标志
				return strType;
			}
		}
		else
		{
			strType[strTypeVaildNum] = 'e';//英文ENGLISH
		}
		strTypeVaildNum ++;
	}
	strType[strTypeVaildNum] = 0;//结束标志
	return strType;
}

//计算有效文本全部显示时的像素宽度
short getWidthOfText( char *str, short len )
{
	char *strType = getWordLetterArrange( str, len );

	if ( strType == NULL )
	{
		return 0;
	}
	if ( strType[0] == 0 )
	{
		free(strType);
		return 0;
	}

	short textWidth = ( strType[0] == 'e' ) ? SCR_LETTER_SIZE_X : SCR_WORD_SIZE_X;

	int i;
	for ( i = 1; strType[i] != 0; i ++ )
	{
		if( strType[i-1] == 'e' && strType[i] == 'e' )
		{
			textWidth = textWidth + DEFAULT_LETTER_SPACING;
		}
		else
		{
			textWidth = textWidth + DEFAULT_WORD_SPACING;
		}
		if ( strType[i] == 'e' )
		{
			textWidth = textWidth + SCR_LETTER_SIZE_X;
		}
		else
		{
			textWidth = textWidth + SCR_WORD_SIZE_X;
		}
	}
	free(strType);
	return textWidth;
}

