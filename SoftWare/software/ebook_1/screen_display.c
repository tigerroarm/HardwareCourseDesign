/*
 * screen_display.c
 *
 *  Created on: 2019��5��1��
 *      Author: 12757
 */

#include "stdlib.h"
#include "screen_display.h"
#include "screen.h"


//������������ʱ������Ļ��ʾЧ�������Ĺ��̻�ɾ��������
color_u16 screenShowSpace[SCR_HEIGHT][SCR_WIDTH];

//���߽�(���ǻ��������εı�)
void showBorder( const AreaRange *borderPos, color_u16 borderColor )
{
	int xPos, yPos;
	//����
	yPos = borderPos->y_min;
	//x����ı߽�
	for ( xPos = borderPos->x_min; xPos <= borderPos->x_max; xPos ++ )
	{
		screenShowSpace[yPos-1][xPos-1] = borderColor;
	}
	yPos = borderPos->y_max;
	for ( xPos = borderPos->x_min; xPos <= borderPos->x_max; xPos ++ )
	{
		screenShowSpace[yPos-1][xPos-1] = borderColor;
	}

	//y����ı߽�
	xPos = borderPos->x_min;
	for ( yPos = borderPos->y_min + 1; yPos < borderPos->y_max; yPos ++ )
	{
		screenShowSpace[yPos-1][xPos-1] = borderColor;
	}
	xPos = borderPos->x_max;
	for ( yPos = borderPos->y_min + 1; yPos < borderPos->y_max; yPos ++ )
	{
		screenShowSpace[yPos-1][xPos-1] = borderColor;
	}

}

//��ʾ�߽��뱳��
//exceptAreaArrayΪ�Ǳ��������������飨�����ʽΪ�������
//bkgBorderColorΪ��ɫ��Ϣ��������ɫ���߽���ɫ���߽���ɫ�Ƿ�ɼ�(������ɼ�����һ��ʹ�ñ�����ɫ
//exceptAreaArray[]�ǷǱ������򹹳ɵ����飬 exceptAreaNumΪ����Ԫ�ظ���
//exceptAreaArray[]��ʽΪ�������λ��
//absPosΪ����λ��
bool showBkgAndBorder( const ColorInfo *bkgBorderColor, const AreaRange *absPos, const AreaRange exceptAreaArray[], const short exceptAreaNum )
{
	bool status = true;

	short width = absPos->x_max - absPos->x_min + 1;
	short height = absPos->y_max - absPos->y_min + 1;
	short xBasePos = absPos->x_min - 1;
	short yBasePos = absPos->y_min - 1;

	if ( bkgBorderColor->borderVisible )//�߽�ɼ�
	{
		//���߽�
		showBorder( absPos, bkgBorderColor->borderColor );
		//������������СһȦ������ȥ�ջ��ı߽�
		width -= 2;
		height -= 2;
		xBasePos += 1;
		yBasePos += 1;
	}

	//��ʾ������ɫ
	//ͳ��ÿһ�зǱ�����������

	bool *exceptXposInRow = malloc( sizeof(char) * width );


	//�˴�xPos,yPosΪ���λ��
	short xPos, yPos;
	for ( yPos = 1; yPos <= height; yPos ++ )
	{
		int i;
		for ( i = 0; i < width; i ++ )
		{
			exceptXposInRow[i] = 0;//���
		}
		//ͳ��ÿһ�зǱ�����������
		const AreaRange *curExceptArea;
		for ( i = 0; i < exceptAreaNum; i ++ )//����ÿһ���Ǳ�����
		{
			curExceptArea = &(exceptAreaArray[i]);
			if ( yPos < curExceptArea->y_min || yPos > curExceptArea->y_max )
			{
				continue;
			}
			else
			{
				for ( xPos = curExceptArea->x_min; xPos <= curExceptArea->x_max; xPos ++ )
				{
					exceptXposInRow[xPos-1] = 1;//�ԷǱ��������������
				}
			}
		}
		//������
		color_u16 bkgColor = bkgBorderColor->bkgColor;
		for ( xPos = 1; xPos <= width; xPos ++ )
		{
			if ( !exceptXposInRow[xPos-1] )//�������ص�
			{
				screenShowSpace[yBasePos+yPos-1][xBasePos+xPos-1] = bkgColor;
			}
		}
	}

	free(exceptXposInRow);
	return status;
}

//��ʾscreen_header
bool showScreenHeader( const ScreenHeaderTime *scrHeaderInst, const AreaRange *scrHeaderAbsPos )
{
	bool status = true;

	//��ʾscreen_header�ı�����߽�
	showBkgAndBorder( &(scrHeaderInst->bkgBorderColor), scrHeaderAbsPos, &(scrHeaderInst->tagTimeArea), 1 );

	//��ʾʱ���ǩ
	AreaRange tagTimeAbsPos;//ʱ���ǩ����λ��
	//�������λ��
	getAbsPos( scrHeaderAbsPos, &(scrHeaderInst->tagTimeArea), &tagTimeAbsPos );
	//��ʾTag
	showTagBlock( &(scrHeaderInst->tagTime), &tagTimeAbsPos );


	return status;
}
//��ʾscreen_footer
bool showScreenFooter( const ScreenFooterBtn *scrFooterInst, const AreaRange *scrFooterAbsPos )
{
	bool status = true;

	//��ʾscreen_footer�ı�����߽�
	AreaRange AreaArray[3] = { scrFooterInst->iconSettingArea, scrFooterInst->iconHomeArea, scrFooterInst->iconBackArea };
	showBkgAndBorder( &(scrFooterInst->bkgBorderColor), scrFooterAbsPos, AreaArray, 3 );

	//��ʾ3������
	AreaRange iconAbsPos;//ͼ�����λ��


	//��ʾ�༭��
	getAbsPos( scrFooterAbsPos, &(scrFooterInst->iconSettingArea), &iconAbsPos );
	showIconBlock( &(scrFooterInst->iconSetting), &iconAbsPos );

	//��ʾ��ҳ��
	getAbsPos( scrFooterAbsPos, &(scrFooterInst->iconHomeArea), &iconAbsPos );
	showIconBlock( &(scrFooterInst->iconHome), &iconAbsPos );

	//��ʾ���ؼ�
	getAbsPos( scrFooterAbsPos, &(scrFooterInst->iconBackArea), &iconAbsPos );
	showIconBlock( &(scrFooterInst->iconBack), &iconAbsPos );

	return status;
}

//��ȡ�µľ���λ��
void getAbsPos( const AreaRange *baseAbsPos, const AreaRange *relativePos, AreaRange *newAbsPos )
{
	newAbsPos->x_min = baseAbsPos->x_min + relativePos->x_min - 1;
	newAbsPos->x_max = baseAbsPos->x_min + relativePos->x_max - 1;
	newAbsPos->y_min = baseAbsPos->y_min + relativePos->y_min - 1;
	newAbsPos->y_max = baseAbsPos->y_min + relativePos->y_max - 1;
}

//��ʾTagBlock
bool showTagBlock( const TagBlock *tagPtr, const AreaRange *absPos )
{
	bool status = true;

	//��ʾTagBlock�ı�����߽�
	bool textVisible = (tagPtr->tagColorInfo).objVisible;

	if ( textVisible )//�м���ı���Ҫ��ʾ
	{
		//����������߽磨�ڵ�text��ʾ����
		showBkgAndBorder( &(tagPtr->tagColorInfo), absPos, &(tagPtr->textArea), 1 );

		//��ʾ�м��text

		AreaRange textAbsPos;//text�ľ���λ��
		//��ȡtext�ľ���λ��
		getAbsPos( absPos, &(tagPtr->textArea), &textAbsPos );

		AreaRange objAbsPos;//�ַ��ľ���λ��

		objAbsPos.y_max = textAbsPos.y_max;
		objAbsPos.y_min = textAbsPos.y_min;
		objAbsPos.x_min = textAbsPos.x_min;

		//��ȡ�ı��к�������ĸ�ķֲ���(����"c",Ӣ��"e",������־0)(���ض�̬�ַ������ǵ�������Ҫfree��)
		char *strType = getWordLetterArrange( (tagPtr->tagText).text, (tagPtr->tagText).textLen );
		//��ʾ��һ���ַ�
		if ( strType == NULL || strType[0] == 0 )
		{
			free(strType);
		}
		else
		{
			short textLen = (tagPtr->tagText).textLen;
			char *text = (tagPtr->tagText).text;
			short textIndex = 0;

			short bkgColor = (tagPtr->tagColorInfo).bkgColor;
			short objColor =  (tagPtr->tagColorInfo).objColor;

			short objYSize = tagPtr->objYSize;

			alt_u8 *objPicPtr;

			short objXSize;

			if ( strType[0] == 'e' )
			{
				objXSize = tagPtr->letterXSize;
				objPicPtr = getAsciiFont( text[0] );
			}
			else
			{
				objXSize = tagPtr->wordXSize;
				objPicPtr = getGbkFont( text[0], text[1] );
			}
			objAbsPos.x_max = objAbsPos.x_min - 1 + objXSize;
			showTextOrIcon( &objAbsPos, objPicPtr, bkgColor, objColor, objXSize, objYSize );

			int i;

			for ( i = 1; strType[i] != 0 && textIndex < textLen; i ++ )
			{
				//��ʾ���
				objAbsPos.x_min = objAbsPos.x_max + 1;
				if( objAbsPos.x_min > textAbsPos.x_max )//�����ı���ʾ�ұ߽�
				{
					break;
				}
				if( strType[i] == 'e' && strType[i-1] == 'e' )//��ǰ�ַ�ΪӢ����ĸ&&ǰһ��ΪӢ����ĸ
				{
					objXSize = tagPtr->letterSpacing;
				}
				else
				{
					objXSize = tagPtr->wordSpacing;
				}
				objAbsPos.x_max = objAbsPos.x_min - 1 + objXSize;


				showTextOrIcon( &objAbsPos, NULL, bkgColor, objColor, objXSize, objYSize );

				//��ʾ�ı�
				objAbsPos.x_min = objAbsPos.x_max + 1;
				if( objAbsPos.x_min > textAbsPos.x_max )//�����ı���ʾ�ұ߽�
				{
					break;
				}
				if( strType[i] == 'e' )//��ǰ�ַ�ΪӢ����ĸ
				{
					objXSize = tagPtr->letterXSize;
					objPicPtr = getAsciiFont( text[textIndex] );
					textIndex += 1;
				}
				else
				{
					objXSize = tagPtr->wordXSize;
					objPicPtr = getGbkFont( text[textIndex], text[textIndex+1] );
					textIndex += 2;
				}
				objAbsPos.x_max = objAbsPos.x_min - 1 + objXSize;

				showTextOrIcon( &objAbsPos, objPicPtr, bkgColor, objColor, objXSize, objYSize );
			}
			//����ı����ұ߽绹�������ռ䣬����䱳����ɫ
			objAbsPos.x_min = objAbsPos.x_max + 1;
			if ( objAbsPos.x_min <= textAbsPos.x_max )
			{
				objAbsPos.x_max = textAbsPos.x_max;
				showTextOrIcon( &objAbsPos, objPicPtr, bkgColor, objColor, objAbsPos.x_max - objAbsPos.x_min + 1, objYSize );
			}
			//����ı����·��������ռ䣬����䱳����ɫ
			short objAreaYdiff = objAbsPos.y_max - objAbsPos.y_min + 1 - objYSize;
			if ( objAreaYdiff > 0 )
			{
				objAbsPos.x_min = textAbsPos.x_min;
				objAbsPos.x_max = textAbsPos.x_max;
				objAbsPos.y_min = textAbsPos.y_max - objAreaYdiff;
				objAbsPos.y_max = textAbsPos.y_max;
				showTextOrIcon( &objAbsPos, NULL, bkgColor, objColor, objAbsPos.x_max - objAbsPos.x_min + 1, objAreaYdiff );
			}
		}
		free(strType);
	}
	else//�м��icon����Ҫ��ʾ
	{
		//ֻ����������߽���ɫ
		showBkgAndBorder( &(tagPtr->tagColorInfo), absPos, NULL, 0 );
	}

	return status;
}

//��ʾIconBlock
bool showIconBlock( const IconBlock *iconPtr, const AreaRange *absPos )
{
	bool status = true;

	//��ʾIconBlock�ı�����߽�
	bool iconPicVisible = (iconPtr->iconColorInfo).objVisible;

	if ( iconPicVisible )//�м��icon��Ҫ��ʾ
	{
		//����������߽磨�ڵ�iconPic��ʾ����
		showBkgAndBorder( &(iconPtr->iconColorInfo), absPos, &(iconPtr->picArea), 1 );

		//��ʾ�м��iconͼƬ

		AreaRange iconPicAbsPos;//iconPic�ľ���λ��
		//��ȡiconPic�ľ���λ��
		getAbsPos( absPos, &(iconPtr->picArea), &iconPicAbsPos );
		//��ʾicon
		showTextOrIcon( &iconPicAbsPos, iconPtr->iconPic, (iconPtr->iconColorInfo).bkgColor, (iconPtr->iconColorInfo).objColor,\
				iconPtr->iconXSize, iconPtr->iconYSize );
	}
	else//�м��icon����Ҫ��ʾ
	{
		//ֻ����������߽���ɫ
		showBkgAndBorder( &(iconPtr->iconColorInfo), absPos, NULL, 0 );
	}

	return status;
}
//��ʾicon����text
bool showTextOrIcon( const AreaRange *absPos, const alt_u8 *objPicPtr, color_u16 bkgColor, color_u16 objColor, short objXSize, short objYSize )
{
	bool status = true;


	short objXByte = (objXSize+7)>>3;//��8������ȡ��

	short width = absPos->x_max - absPos->x_min + 1;
	short height = absPos->y_max - absPos->y_min + 1;

	if ( width > objXSize )
	{
		width = objXSize;
	}
	if ( height > objYSize )
	{
		height = objYSize;
	}
	short xBasePos = absPos->x_min - 1;
	short yBasePos = absPos->y_min - 1;

	alt_u8 objPicRowPixel;//���������

	//xPos,yPosΪ���λ��
	short xPos, yPos;
	short objPicRowBaseIndex = 0;
	for ( yPos = 1; yPos <= height; yPos ++ )
	{
		short objPicIndex = objPicRowBaseIndex;
		for ( xPos = 1; xPos <= width; xPos ++ )
		{
			if ( (xPos & 0x7) == 0x1 )//xPos����λΪ001����ЧΪxPos % 8 == 1 )
			{
				objPicRowPixel = ( objPicPtr != NULL ) ? objPicPtr[objPicIndex] : 0x00;

				objPicIndex ++;
			}
			if ( (objPicRowPixel & 0x80) == 0x80 )//���λΪ1
			{
				screenShowSpace[yBasePos+yPos-1][xBasePos+xPos-1] = objColor;
			}
			else
			{
				screenShowSpace[yBasePos+yPos-1][xBasePos+xPos-1] = bkgColor;
			}
			objPicRowPixel = objPicRowPixel<<1;//����һλ������������Ϣ�������λ
		}
		objPicRowBaseIndex = objPicRowBaseIndex + objXByte;
	}

	return status;
}