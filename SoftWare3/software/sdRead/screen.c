/*
 * screen.c
 *
 *  Created on: 2019��4��26��
 *      Author: 12757
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "screen.h"
#include "color.h"



//	sprintf(str, "%02d", num);���Խ����ֱ�Ϊ�ַ���

//������Ļ����
ScreenContainer tftlcdScreenInst;

ColorInfo defaultColorInfo = {  DEFAULT_BORDER_COLOR, DEFAULT_BKG_COLOR, DEFAULT_OBJ_COLOR, true, true };

char *str1 = "Happy";


//��ȡ�ı��к�������ĸ�ķֲ���(ֵΪ'c'�������֣�ֵΪ'e'������Ӣ����ĸ
//����ǵ�free���ص�char[]
char *getWordLetterArrange( char *str, short len )
{
	char *strType = (char*)malloc( sizeof( char ) * (len+1) );
	short strTypeVaildNum = 0;

	int i;
	for ( i = 0; i < len; i ++ )
	{
		char curChar = str[i];
		if ( curChar == 0 )//������־
		{
			break;
		}
		if ( curChar < 0 )//��ǰ�ֽ�Ϊ���ָ��ֽ�
		{
			strType[strTypeVaildNum] = 'c';//����CHINESE
			i = i + 1;
			if ( i == len || str[i] == 0 )//�����ַ������������ֽ�ȴ�����ڣ�������
			{
				strType[strTypeVaildNum] = 0;//������־
				return strType;
			}
		}
		else
		{
			strType[strTypeVaildNum] = 'e';//Ӣ��ENGLISH
		}
		strTypeVaildNum ++;
	}
	strType[strTypeVaildNum] = 0;//������־
	return strType;
}

//������Ч�ı�ȫ����ʾʱ�����ؿ��
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

