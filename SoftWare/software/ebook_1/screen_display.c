/*
 * screen_display.c
 *
 *  Created on: 2019年5月1日
 *      Author: 12757
 */

#include "stdlib.h"
#include "screen_display.h"
#include "screen.h"


//该数组用于临时测试屏幕显示效果，最后的工程会删除该数组
color_u16 screenShowSpace[SCR_HEIGHT][SCR_WIDTH];

//画边界(就是画个长方形的边)
void showBorder( const AreaRange *borderPos, color_u16 borderColor )
{
	int xPos, yPos;
	//横向
	yPos = borderPos->y_min;
	//x方向的边界
	for ( xPos = borderPos->x_min; xPos <= borderPos->x_max; xPos ++ )
	{
		screenShowSpace[yPos-1][xPos-1] = borderColor;
	}
	yPos = borderPos->y_max;
	for ( xPos = borderPos->x_min; xPos <= borderPos->x_max; xPos ++ )
	{
		screenShowSpace[yPos-1][xPos-1] = borderColor;
	}

	//y方向的边界
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

//显示边界与背景
//exceptAreaArray为非背景区的区域数组（区域格式为相对区域）
//bkgBorderColor为颜色信息：背景颜色、边界颜色、边界颜色是否可见(如果不可见，则一律使用背景颜色
//exceptAreaArray[]是非背景区域构成的数组， exceptAreaNum为数组元素个数
//exceptAreaArray[]格式为相对区域位置
//absPos为绝对位置
bool showBkgAndBorder( const ColorInfo *bkgBorderColor, const AreaRange *absPos, const AreaRange exceptAreaArray[], const short exceptAreaNum )
{
	bool status = true;

	short width = absPos->x_max - absPos->x_min + 1;
	short height = absPos->y_max - absPos->y_min + 1;
	short xBasePos = absPos->x_min - 1;
	short yBasePos = absPos->y_min - 1;

	if ( bkgBorderColor->borderVisible )//边界可见
	{
		//画边界
		showBorder( absPos, bkgBorderColor->borderColor );
		//将绝对区域缩小一圈，即除去刚画的边界
		width -= 2;
		height -= 2;
		xBasePos += 1;
		yBasePos += 1;
	}

	//显示背景颜色
	//统计每一行非背景区的像素

	bool *exceptXposInRow = malloc( sizeof(char) * width );


	//此处xPos,yPos为相对位置
	short xPos, yPos;
	for ( yPos = 1; yPos <= height; yPos ++ )
	{
		int i;
		for ( i = 0; i < width; i ++ )
		{
			exceptXposInRow[i] = 0;//清空
		}
		//统计每一行非背景区的像素
		const AreaRange *curExceptArea;
		for ( i = 0; i < exceptAreaNum; i ++ )//遍历每一个非背景区
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
					exceptXposInRow[xPos-1] = 1;//对非背景像素做作标记
				}
			}
		}
		//画背景
		color_u16 bkgColor = bkgBorderColor->bkgColor;
		for ( xPos = 1; xPos <= width; xPos ++ )
		{
			if ( !exceptXposInRow[xPos-1] )//背景像素点
			{
				screenShowSpace[yBasePos+yPos-1][xBasePos+xPos-1] = bkgColor;
			}
		}
	}

	free(exceptXposInRow);
	return status;
}

//显示screen_header
bool showScreenHeader( const ScreenHeaderTime *scrHeaderInst, const AreaRange *scrHeaderAbsPos )
{
	bool status = true;

	//显示screen_header的背景与边界
	showBkgAndBorder( &(scrHeaderInst->bkgBorderColor), scrHeaderAbsPos, &(scrHeaderInst->tagTimeArea), 1 );

	//显示时间标签
	AreaRange tagTimeAbsPos;//时间标签绝对位置
	//计算绝对位置
	getAbsPos( scrHeaderAbsPos, &(scrHeaderInst->tagTimeArea), &tagTimeAbsPos );
	//显示Tag
	showTagBlock( &(scrHeaderInst->tagTime), &tagTimeAbsPos );


	return status;
}
//显示screen_footer
bool showScreenFooter( const ScreenFooterBtn *scrFooterInst, const AreaRange *scrFooterAbsPos )
{
	bool status = true;

	//显示screen_footer的背景与边界
	AreaRange AreaArray[3] = { scrFooterInst->iconSettingArea, scrFooterInst->iconHomeArea, scrFooterInst->iconBackArea };
	showBkgAndBorder( &(scrFooterInst->bkgBorderColor), scrFooterAbsPos, AreaArray, 3 );

	//显示3个按键
	AreaRange iconAbsPos;//图标绝对位置


	//显示编辑键
	getAbsPos( scrFooterAbsPos, &(scrFooterInst->iconSettingArea), &iconAbsPos );
	showIconBlock( &(scrFooterInst->iconSetting), &iconAbsPos );

	//显示主页键
	getAbsPos( scrFooterAbsPos, &(scrFooterInst->iconHomeArea), &iconAbsPos );
	showIconBlock( &(scrFooterInst->iconHome), &iconAbsPos );

	//显示返回键
	getAbsPos( scrFooterAbsPos, &(scrFooterInst->iconBackArea), &iconAbsPos );
	showIconBlock( &(scrFooterInst->iconBack), &iconAbsPos );

	return status;
}

//获取新的绝对位置
void getAbsPos( const AreaRange *baseAbsPos, const AreaRange *relativePos, AreaRange *newAbsPos )
{
	newAbsPos->x_min = baseAbsPos->x_min + relativePos->x_min - 1;
	newAbsPos->x_max = baseAbsPos->x_min + relativePos->x_max - 1;
	newAbsPos->y_min = baseAbsPos->y_min + relativePos->y_min - 1;
	newAbsPos->y_max = baseAbsPos->y_min + relativePos->y_max - 1;
}

//显示TagBlock
bool showTagBlock( const TagBlock *tagPtr, const AreaRange *absPos )
{
	bool status = true;

	//显示TagBlock的背景与边界
	bool textVisible = (tagPtr->tagColorInfo).objVisible;

	if ( textVisible )//中间的文本需要显示
	{
		//处理背景与边界（挖掉text显示区）
		showBkgAndBorder( &(tagPtr->tagColorInfo), absPos, &(tagPtr->textArea), 1 );

		//显示中间的text

		AreaRange textAbsPos;//text的绝对位置
		//获取text的绝对位置
		getAbsPos( absPos, &(tagPtr->textArea), &textAbsPos );

		AreaRange objAbsPos;//字符的绝对位置

		objAbsPos.y_max = textAbsPos.y_max;
		objAbsPos.y_min = textAbsPos.y_min;
		objAbsPos.x_min = textAbsPos.x_min;

		//获取文本中汉字与字母的分布表(中文"c",英文"e",结束标志0)(返回动态字符串，记得用完了要free掉)
		char *strType = getWordLetterArrange( (tagPtr->tagText).text, (tagPtr->tagText).textLen );
		//显示第一个字符
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
				//显示间隔
				objAbsPos.x_min = objAbsPos.x_max + 1;
				if( objAbsPos.x_min > textAbsPos.x_max )//超出文本显示右边界
				{
					break;
				}
				if( strType[i] == 'e' && strType[i-1] == 'e' )//当前字符为英文字母&&前一个为英文字母
				{
					objXSize = tagPtr->letterSpacing;
				}
				else
				{
					objXSize = tagPtr->wordSpacing;
				}
				objAbsPos.x_max = objAbsPos.x_min - 1 + objXSize;


				showTextOrIcon( &objAbsPos, NULL, bkgColor, objColor, objXSize, objYSize );

				//显示文本
				objAbsPos.x_min = objAbsPos.x_max + 1;
				if( objAbsPos.x_min > textAbsPos.x_max )//超出文本显示右边界
				{
					break;
				}
				if( strType[i] == 'e' )//当前字符为英文字母
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
			//如果文本区右边界还有余留空间，就填充背景颜色
			objAbsPos.x_min = objAbsPos.x_max + 1;
			if ( objAbsPos.x_min <= textAbsPos.x_max )
			{
				objAbsPos.x_max = textAbsPos.x_max;
				showTextOrIcon( &objAbsPos, objPicPtr, bkgColor, objColor, objAbsPos.x_max - objAbsPos.x_min + 1, objYSize );
			}
			//如果文本区下方有余留空间，就填充背景颜色
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
	else//中间的icon不需要显示
	{
		//只处理背景与边界颜色
		showBkgAndBorder( &(tagPtr->tagColorInfo), absPos, NULL, 0 );
	}

	return status;
}

//显示IconBlock
bool showIconBlock( const IconBlock *iconPtr, const AreaRange *absPos )
{
	bool status = true;

	//显示IconBlock的背景与边界
	bool iconPicVisible = (iconPtr->iconColorInfo).objVisible;

	if ( iconPicVisible )//中间的icon需要显示
	{
		//处理背景与边界（挖掉iconPic显示区）
		showBkgAndBorder( &(iconPtr->iconColorInfo), absPos, &(iconPtr->picArea), 1 );

		//显示中间的icon图片

		AreaRange iconPicAbsPos;//iconPic的绝对位置
		//获取iconPic的绝对位置
		getAbsPos( absPos, &(iconPtr->picArea), &iconPicAbsPos );
		//显示icon
		showTextOrIcon( &iconPicAbsPos, iconPtr->iconPic, (iconPtr->iconColorInfo).bkgColor, (iconPtr->iconColorInfo).objColor,\
				iconPtr->iconXSize, iconPtr->iconYSize );
	}
	else//中间的icon不需要显示
	{
		//只处理背景与边界颜色
		showBkgAndBorder( &(iconPtr->iconColorInfo), absPos, NULL, 0 );
	}

	return status;
}
//显示icon或者text
bool showTextOrIcon( const AreaRange *absPos, const alt_u8 *objPicPtr, color_u16 bkgColor, color_u16 objColor, short objXSize, short objYSize )
{
	bool status = true;


	short objXByte = (objXSize+7)>>3;//除8，向上取整

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

	alt_u8 objPicRowPixel;//横向的像素

	//xPos,yPos为相对位置
	short xPos, yPos;
	short objPicRowBaseIndex = 0;
	for ( yPos = 1; yPos <= height; yPos ++ )
	{
		short objPicIndex = objPicRowBaseIndex;
		for ( xPos = 1; xPos <= width; xPos ++ )
		{
			if ( (xPos & 0x7) == 0x1 )//xPos低三位为001（等效为xPos % 8 == 1 )
			{
				objPicRowPixel = ( objPicPtr != NULL ) ? objPicPtr[objPicIndex] : 0x00;

				objPicIndex ++;
			}
			if ( (objPicRowPixel & 0x80) == 0x80 )//最高位为1
			{
				screenShowSpace[yBasePos+yPos-1][xBasePos+xPos-1] = objColor;
			}
			else
			{
				screenShowSpace[yBasePos+yPos-1][xBasePos+xPos-1] = bkgColor;
			}
			objPicRowPixel = objPicRowPixel<<1;//左移一位，将新像素信息移至最高位
		}
		objPicRowBaseIndex = objPicRowBaseIndex + objXByte;
	}

	return status;
}
