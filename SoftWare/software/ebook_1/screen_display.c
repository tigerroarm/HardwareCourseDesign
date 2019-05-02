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

//获取新的绝对位置
void getAbsPos( const AreaRange *baseAbsPos, const AreaRange *relativePos, AreaRange *newAbsPos )
{
	newAbsPos->x_min = baseAbsPos->x_min + relativePos->x_min - 1;
	newAbsPos->x_max = baseAbsPos->x_min + relativePos->x_max - 1;
	newAbsPos->y_min = baseAbsPos->y_min + relativePos->y_min - 1;
	newAbsPos->y_max = baseAbsPos->y_min + relativePos->y_max - 1;
}

//获取x方向的区域定位数组(返回数组大小）
short getXVaildRange( const char areaOccurInfo[], const AreaRange exceptAreaArray[], const short exceptAreaNum, \
		short *xMinArray, short *xMaxArray, const short width )
{

	char *exceptXposInRow = (char*)malloc( sizeof(char) * (width+1) );

	int i;
	for ( i = 0; i < width; i ++ )
	{
		exceptXposInRow[i] = 'b';
	}
	exceptXposInRow[width] = 'e';
	short xPos;
	const AreaRange *curExceptArea;
	for ( i = 0; i < exceptAreaNum; i ++ )
	{
		if ( areaOccurInfo[i] == 'e' )
		{
			curExceptArea = &(exceptAreaArray[i]);

			for ( xPos = curExceptArea->x_min; xPos <= curExceptArea->x_max && xPos <= width; xPos ++ )
			{
				exceptXposInRow[xPos-1] = 'e';//对非背景像素做作标记
			}
		}
	}
	short rangeIndex = 0;

	char preRowXType = 'e';
	char curRowXType;
	for ( xPos = 1; xPos <= width + 1; xPos ++ )
	{
		curRowXType = exceptXposInRow[xPos-1];
		if ( curRowXType == 'b' )//当前为背景区
		{
			if ( preRowXType == 'e' )//前面为非背景区
			{
				xMinArray[rangeIndex] = xPos;
			}
		}
		else//当前为非背景区
		{
			if ( preRowXType == 'b' )//前面为背景区
			{
				xMinArray[rangeIndex] = xPos - 1;
				rangeIndex ++;
			}
		}

		preRowXType = curRowXType;
	}

	return rangeIndex;
}

//画边界(就是画个长方形的边)
bool showBorder( const AreaRange *borderAbsPos, color_u16 borderColor )
{
	if ( borderAbsPos->x_min > borderAbsPos->x_max || borderAbsPos->y_min > borderAbsPos->y_max )
	{
		return false;
	}

	int xPos, yPos;
	//横向
	yPos = borderAbsPos->y_min;
	//x方向的边界
	for ( xPos = borderAbsPos->x_min; xPos <= borderAbsPos->x_max; xPos ++ )
	{
		screenShowSpace[yPos-1][xPos-1] = borderColor;
	}
	yPos = borderAbsPos->y_max;
	for ( xPos = borderAbsPos->x_min; xPos <= borderAbsPos->x_max; xPos ++ )
	{
		screenShowSpace[yPos-1][xPos-1] = borderColor;
	}

	//y方向的边界
	xPos = borderAbsPos->x_min;
	for ( yPos = borderAbsPos->y_min + 1; yPos < borderAbsPos->y_max; yPos ++ )
	{
		screenShowSpace[yPos-1][xPos-1] = borderColor;
	}
	xPos = borderAbsPos->x_max;
	for ( yPos = borderAbsPos->y_min + 1; yPos < borderAbsPos->y_max; yPos ++ )
	{
		screenShowSpace[yPos-1][xPos-1] = borderColor;
	}

	return true;
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

	color_u16 bkgColor = bkgBorderColor->bkgColor;

	if ( bkgBorderColor->borderVisible )//边界可见
	{
		//画边界
		status = status && showBorder( absPos, bkgBorderColor->borderColor );
		//将绝对区域缩小一圈，即除去刚画的边界
		width -= 2;
		height -= 2;
		xBasePos += 1;
		yBasePos += 1;
	}

	//显示背景颜色
	//此处xPos,yPos为相对位置
	short xPos, yPos;
	char *areaOccurInfo = (char *)malloc( sizeof(char) * exceptAreaNum );
	short *xMinArray = (short *)malloc( sizeof(short) * (exceptAreaNum+1) );;
	short *xMaxArray = (short *)malloc( sizeof(short) * (exceptAreaNum+1) );;
	int i;
	for ( i = 0; i < exceptAreaNum; i ++ )
	{
		areaOccurInfo[i] = 0;//清空
	}
	short xRangeNum = 0;
	for ( yPos = 1; yPos <= height; yPos ++ )
	{
		//统计该行出现的非背景区
		bool changeOccur = false;
		for ( i = 0; i < exceptAreaNum; i ++ )
		{
			const AreaRange *curExceptArea = &(exceptAreaArray[i]);
			if ( yPos < curExceptArea->y_min || yPos > curExceptArea->y_max )
			{
				if ( areaOccurInfo[i] != 'n' )
				{
					changeOccur = true;
				}
				areaOccurInfo[i] = 'n';//非背景区
				continue;
			}
			else
			{
				if ( areaOccurInfo[i] != 'e' )
				{
					changeOccur = true;
				}
				//对非背景像素做作标记
				areaOccurInfo[i] = 'e';
			}
			//如果非背景区发生变化，则修改非背景的x定位数组
			if ( changeOccur )
			{
				xRangeNum = getXVaildRange( areaOccurInfo, exceptAreaArray, exceptAreaNum, xMinArray, xMaxArray, width );
			}
			//画背景
			for ( i = 0; i < xRangeNum; i ++ )
			{
				for ( xPos = xMinArray[i]; xPos <= xMaxArray[i]; xPos ++ )
				{
					screenShowSpace[yBasePos+yPos-1][xBasePos+xPos-1] = bkgColor;
				}
			}
		}

	}

	free(areaOccurInfo);
	free(xMinArray);
	free(xMaxArray);

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


//显示TagBlock
bool showTagBlock( const TagBlock *tagPtr, const AreaRange *absPos )
{
	bool status = true;

	//显示TagBlock的背景与边界
	bool textVisible = (tagPtr->tagColorInfo).objVisible;

	if ( textVisible )//中间的文本需要显示
	{
		//处理背景与边界（挖掉text显示区）
		status = status && showBkgAndBorder( &(tagPtr->tagColorInfo), absPos, &(tagPtr->textArea), 1 );

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
			status = status && showTextOrIcon( &objAbsPos, objPicPtr, bkgColor, objColor, objXSize, objYSize );

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


				status = status && showTextOrIcon( &objAbsPos, NULL, bkgColor, objColor, objXSize, objYSize );

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

				status = status && showTextOrIcon( &objAbsPos, objPicPtr, bkgColor, objColor, objXSize, objYSize );
			}
			//如果文本区右边界还有余留空间，就填充背景颜色
			objAbsPos.x_min = objAbsPos.x_max + 1;
			if ( objAbsPos.x_min <= textAbsPos.x_max )
			{
				objAbsPos.x_max = textAbsPos.x_max;
				status = status && showTextOrIcon( &objAbsPos, objPicPtr, bkgColor, objColor, objAbsPos.x_max - objAbsPos.x_min + 1, objYSize );
			}
			//如果文本区下方有余留空间，就填充背景颜色
			short objAreaYdiff = objAbsPos.y_max - objAbsPos.y_min + 1 - objYSize;
			if ( objAreaYdiff > 0 )
			{
				objAbsPos.x_min = textAbsPos.x_min;
				objAbsPos.x_max = textAbsPos.x_max;
				objAbsPos.y_min = textAbsPos.y_max - objAreaYdiff + 1;
				objAbsPos.y_max = textAbsPos.y_max;
				status = status && showTextOrIcon( &objAbsPos, NULL, bkgColor, objColor, objAbsPos.x_max - objAbsPos.x_min + 1, objAreaYdiff );
			}
		}
		free(strType);
	}
	else//中间的icon不需要显示
	{
		//只处理背景与边界颜色
		status = status && showBkgAndBorder( &(tagPtr->tagColorInfo), absPos, NULL, 0 );
	}

	return status;
}

//显示TagBlock(带纵向基区上偏移)
bool showTagWithYOffset( const TagBlock *tagPtr, const AreaRange *absPos, const short yOffset )
{
	bool status = true;

	//显示TagBlock的背景与边界
	bool textVisible = (tagPtr->tagColorInfo).objVisible;


	short wordPicPtrOffset = 0;
	short letterPicPtrOffset = 0;
	short diffY = 0;//文本区域纵向削减的高度

	if ( yOffset >= (tagPtr->textArea).y_min )
	{
		short diffY = yOffset - (tagPtr->textArea).y_min + 1;
		short objXByte = ((tagPtr->wordXSize)+7)>>3;//除8，向上取整
		wordPicPtrOffset = diffY * objXByte;
		objXByte = ((tagPtr->letterXSize)+7)>>3;//除8，向上取整
		letterPicPtrOffset = diffY * objXByte;
	}

	//absPos被yOffset削减后的区域
	AreaRange absNewPos = *absPos;
	absNewPos.y_min += yOffset;

	if ( textVisible )//中间的文本需要显示
	{
		//处理背景与边界（挖掉text显示区）
		status = status && showBkgAndBorder( &(tagPtr->tagColorInfo), &absNewPos, &(tagPtr->textArea), 1 );
		//用背景颜色覆盖掉上边界颜色
		if ( (tagPtr->tagColorInfo).borderVisible && yOffset > 0 )
		{
			AreaRange borderPos;
			borderPos.x_min = absNewPos.x_min;
			borderPos.x_max = absNewPos.x_max;
			borderPos.y_min = borderPos.y_max = absNewPos.y_min;
			status = status && showBorder( &borderPos, (tagPtr->tagColorInfo).bkgColor );
		}


		//显示中间的text

		AreaRange textAbsPos;//text的绝对位置
		//获取text的绝对位置
		getAbsPos( absPos, &(tagPtr->textArea), &textAbsPos );

		AreaRange objAbsPos;//字符的绝对位置

		objAbsPos.y_max = textAbsPos.y_max;
		objAbsPos.y_min = textAbsPos.y_min + diffY;
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
				objPicPtr = getAsciiFont( text[0] ) + letterPicPtrOffset;
			}
			else
			{
				objXSize = tagPtr->wordXSize;
				objPicPtr = getGbkFont( text[0], text[1] ) + wordPicPtrOffset;
			}
			objAbsPos.x_max = objAbsPos.x_min - 1 + objXSize;
			status = status && showTextOrIcon( &objAbsPos, objPicPtr, bkgColor, objColor, objXSize, objYSize );

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


				status = status && showTextOrIcon( &objAbsPos, NULL, bkgColor, objColor, objXSize, objYSize );

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

				status = status && showTextOrIcon( &objAbsPos, objPicPtr, bkgColor, objColor, objXSize, objYSize );
			}
			//如果文本区右边界还有余留空间，就填充背景颜色
			objAbsPos.x_min = objAbsPos.x_max + 1;
			if ( objAbsPos.x_min <= textAbsPos.x_max )
			{
				objAbsPos.x_max = textAbsPos.x_max;
				status = status && showTextOrIcon( &objAbsPos, objPicPtr, bkgColor, objColor, objAbsPos.x_max - objAbsPos.x_min + 1, objYSize );
			}
			//如果文本区下方有余留空间，就填充背景颜色
			short objAreaYdiff = objAbsPos.y_max - objAbsPos.y_min + 1 - objYSize;
			if ( objAreaYdiff > 0 )
			{
				objAbsPos.x_min = textAbsPos.x_min;
				objAbsPos.x_max = textAbsPos.x_max;
				objAbsPos.y_min = textAbsPos.y_max - objAreaYdiff + 1;
				objAbsPos.y_max = textAbsPos.y_max;
				status = status && showTextOrIcon( &objAbsPos, NULL, bkgColor, objColor, objAbsPos.x_max - objAbsPos.x_min + 1, objAreaYdiff );
			}
		}
		free(strType);
	}
	else//中间的icon不需要显示
	{
		//只处理背景与边界颜色
		status = status && showBkgAndBorder( &(tagPtr->tagColorInfo), &absNewPos, NULL, 0 );
		//用背景颜色覆盖掉上边界颜色
		if ( (tagPtr->tagColorInfo).borderVisible && yOffset > 0 )
		{
			AreaRange borderPos;
			borderPos.x_min = absNewPos.x_min;
			borderPos.x_max = absNewPos.x_max;
			borderPos.y_min = borderPos.y_max = absNewPos.y_min;
			status = status && showBorder( &borderPos, (tagPtr->tagColorInfo).bkgColor );
		}
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
		status = status && showBkgAndBorder( &(iconPtr->iconColorInfo), absPos, &(iconPtr->picArea), 1 );

		//显示中间的icon图片

		AreaRange iconPicAbsPos;//iconPic的绝对位置
		//获取iconPic的绝对位置
		getAbsPos( absPos, &(iconPtr->picArea), &iconPicAbsPos );
		//显示icon
		status = status && showTextOrIcon( &iconPicAbsPos, iconPtr->iconPic, (iconPtr->iconColorInfo).bkgColor, (iconPtr->iconColorInfo).objColor,\
				iconPtr->iconXSize, iconPtr->iconYSize );
	}
	else//中间的icon不需要显示
	{
		//只处理背景与边界颜色
		status = status && showBkgAndBorder( &(iconPtr->iconColorInfo), absPos, NULL, 0 );
	}

	return status;
}
//显示TagGroup
bool showTagGroup( const TagGroup *tagGroupInst, const AreaRange *absPos )
{
	bool status = true;

	//显示TagGroup的背景与边界
	AreaRange AreaArray[2] = { tagGroupInst->staticTagArea, tagGroupInst->varTagArea };
	status = status && showBkgAndBorder( &(tagGroupInst->bkgBorderColor), absPos, AreaArray, 2 );

	//显示2个标签

	//标签绝对位置
	AreaRange tagAbsPos;

	//显示静态标签
	getAbsPos( absPos, &(tagGroupInst->staticTagArea), &tagAbsPos );
	status = status && showTagBlock( &(tagGroupInst->staticTag), &tagAbsPos );

	//显示动态标签
	getAbsPos( absPos, &(tagGroupInst->varTagArea), &tagAbsPos );
	status = status && showTagBlock( &(tagGroupInst->varTag), &tagAbsPos );

	return status;
}

//显示TagIconGroup
bool showTagIconGroup( const TagIconGroup *tagIconGpInst, const AreaRange *absPos )
{
	bool status = true;

	//显示TagIconGroup的背景与边界
	AreaRange AreaArray[4] = { tagIconGpInst->plusIconArea, tagIconGpInst->valueTagArea,\
					tagIconGpInst->unitTagArea,tagIconGpInst->minusIconArea };
	status = status && showBkgAndBorder( &(tagIconGpInst->bkgBorderColor), absPos, AreaArray, 4 );

	//显示子模块

	//子模块绝对位置
	AreaRange subModuAbsPos;

	//显示数字加键
	getAbsPos( absPos, &(tagIconGpInst->plusIconArea), &subModuAbsPos );
	status = status && showIconBlock( &(tagIconGpInst->plusIcon), &subModuAbsPos );

	//显示数字减键
	getAbsPos( absPos, &(tagIconGpInst->minusIconArea), &subModuAbsPos );
	status = status && showIconBlock( &(tagIconGpInst->minusIcon), &subModuAbsPos );

	//数字显示标签
	getAbsPos( absPos, &(tagIconGpInst->valueTagArea), &subModuAbsPos );
	status = status && showTagBlock( &(tagIconGpInst->valueTag), &subModuAbsPos );

	//数字单位标签
	getAbsPos( absPos, &(tagIconGpInst->unitTagArea), &subModuAbsPos );
	status = status && showTagBlock( &(tagIconGpInst->unitTag), &subModuAbsPos );

	return status;
}

//显示TagList
bool showTagList( const TagList *TagListInst, const AreaRange *absPos, const short tagListYOffset )
{
	bool status = true;

	//列表显示空间absPos的高度
	short listShowHeight = absPos->y_max - absPos->y_min + 1;
	//列表显示空间absPos在列表TagListInst中的纵向位置
	short listShowYMin = tagListYOffset + 1;
	short listShowYMax = tagListYOffset + listShowHeight;

	short elemHeight = (TagListInst->elemBaseArea).y_max;

	short listShowIndexMin = (listShowYMin-1) / elemHeight;//列表中需要显示tag的最小行数下标（从0开始）
	short listShowIndexMax = (listShowYMax-1) / elemHeight;//列表中需要显示tag的最大行数下标（从0开始）

	short elemNum = TagListInst->elemNum;//列表元素总个数


	if ( listShowIndexMax >= elemNum )
	{
		listShowIndexMax = elemNum - 1;
	}
	//显示TagList列表中每一个落在显示域中的TagBlock
	short elemCurIndex;
	AreaRange curElemAbsPos;
	//计算偏移量tagListYOffset为0时基本模块的绝对位置
	getAbsPos( absPos, &(TagListInst->elemBaseArea), &curElemAbsPos );
	//计算最小下标模块的绝对位置（叠加偏移量tagListYOffset与最小下标模块的下标elemCurIndex
	short diffY = listShowIndexMin * elemHeight - tagListYOffset;
	curElemAbsPos.y_min += diffY;
	curElemAbsPos.y_max += diffY;

	TagBlock * const tagBlockArray = TagListInst->elemBlock;

	//显示第一个TagBlock
	elemCurIndex = listShowIndexMin;
	if ( elemCurIndex <= listShowIndexMax )
	{
		status = status && showTagWithYOffset( &(tagBlockArray[elemCurIndex]), &curElemAbsPos, absPos->y_min - curElemAbsPos.y_min );
		//纵向移动位置
		curElemAbsPos.y_min += elemHeight;
		curElemAbsPos.y_max += elemHeight;
	}


	for ( elemCurIndex = listShowIndexMin + 1; elemCurIndex <= listShowIndexMax; elemCurIndex ++ )
	{
		//显示其他的TagBlock
		status = status && showTagBlock( &(tagBlockArray[elemCurIndex]), &curElemAbsPos );
		//纵向移动位置
		curElemAbsPos.y_min += elemHeight;
		curElemAbsPos.y_max += elemHeight;
	}

	//显示背景区域（直接用变量curElemAbsPos）
	if ( curElemAbsPos.y_min <= absPos->y_max )
	{
		curElemAbsPos.y_max = absPos->y_max;
		status = status && showBkgAndBorder( &(TagListInst->elemColorInfo), &curElemAbsPos, NULL, 0 );
	}

	return status;
}
//显示ScrollBarY
bool showScrollBarY( const ScrollBarY *scrollBarYInst, const AreaRange *absPos )
{
	bool status = true;


	//子模块绝对位置临时存储变量
	AreaRange subModuAbsPos;


	//如果滚动条可见
	if ( scrollBarYInst->iconBarVisible )
	{

		//滚动条纵向偏移量
		short barYoffset = scrollBarYInst->barYoffset;

		//计算滚动条偏移后的位置
		const AreaRange *barBaseAreaPtr = &(scrollBarYInst->barBaseArea);
		AreaRange barRealArea;

		barRealArea.x_min = barBaseAreaPtr->x_min;
		barRealArea.x_max = barBaseAreaPtr->x_max;
		barRealArea.y_min = barBaseAreaPtr->y_min + barYoffset;
		barRealArea.y_max = barBaseAreaPtr->y_max + barYoffset;


		//显示ScrollBarY的背景与边界
		AreaRange AreaArray[3] = { scrollBarYInst->iconMoveUpArea, barRealArea, scrollBarYInst->iconMoveDownArea };
		status = status && showBkgAndBorder( &(scrollBarYInst->barYColorInfo), absPos, AreaArray, 3 );


		//显示滚动条
		getAbsPos( absPos, &barRealArea, &subModuAbsPos );
		status = status && showIconBlock( &(scrollBarYInst->iconBar), &subModuAbsPos );
	}
	else
	{
		//显示ScrollBarY的背景与边界
		AreaRange AreaArray[2] = { scrollBarYInst->iconMoveUpArea, scrollBarYInst->iconMoveDownArea };
		status = status && showBkgAndBorder( &(scrollBarYInst->barYColorInfo), absPos, AreaArray, 2 );
	}

	//显示上移键
	getAbsPos( absPos, &(scrollBarYInst->iconMoveUpArea), &subModuAbsPos );
	status = status && showIconBlock( &(scrollBarYInst->iconMoveUp), &subModuAbsPos );

	//显示下移键
	getAbsPos( absPos, &(scrollBarYInst->iconMoveDownArea), &subModuAbsPos );
	status = status && showIconBlock( &(scrollBarYInst->iconMoveDown), &subModuAbsPos );


	return status;
}

//显示ScreenPageInfo
bool showScreenPageInfo( const ScreenPageInfo *scrPageInfoInst, const AreaRange *absPos )
{
	bool status = true;

	//显示ScreenPageInfo的背景与边界
	AreaRange AreaArray[3] = { scrPageInfoInst->prgBarArea, scrPageInfoInst->tagPagePercTageArea,\
					scrPageInfoInst->tagPageNumArea };
	status = status && showBkgAndBorder( &(scrPageInfoInst->bkgBorderColor), absPos, AreaArray, 3 );

	//子模块显示

	//子模块绝对位置
	AreaRange subModuAbsPos;

	//显示水平进度条
	getAbsPos( absPos, &(scrPageInfoInst->prgBarArea), &subModuAbsPos );
	status = status && showProgressBarX( &(scrPageInfoInst->prgBarX), &subModuAbsPos );

	//显示百分数进度
	getAbsPos( absPos, &(scrPageInfoInst->tagPagePercTageArea), &subModuAbsPos );
	status = status && showTagBlock( &(scrPageInfoInst->tagPagePercTage), &subModuAbsPos );

	//显示页数与总页数（格式为：" 1234/99999", 1234为当前页数，99999为总页数）
	getAbsPos( absPos, &(scrPageInfoInst->tagPageNumArea), &subModuAbsPos );
	status = status && showTagBlock( &(scrPageInfoInst->tagPageNum), &subModuAbsPos );

	return status;
}

//显示ProgressBarX
bool showProgressBarX( const ProgressBarX *prgBarXInst, const AreaRange *absPos )
{
	bool status = true;

	//进度点基本区域（偏移量为0）
	const AreaRange *dotBaseAreaPtr = &(prgBarXInst->dotBaseArea);
	//进度点横向偏移量
	short dotXoffset = prgBarXInst->xOffset;

	//计算滚动条偏移后的位置
	AreaRange dotRealArea;

	dotRealArea.x_min = dotBaseAreaPtr->x_min + dotXoffset;
	dotRealArea.x_max = dotBaseAreaPtr->x_max + dotXoffset;
	dotRealArea.y_min = dotBaseAreaPtr->y_min;
	dotRealArea.y_max = dotBaseAreaPtr->y_max;


	//显示ProgressBarX的背景与边界（由于点非常小，所以把点的区域例外去掉了）
//	AreaRange AreaArray[2] = { prgBarXInst->axisArea, dotRealArea };
//	status = status && showBkgAndBorder( &(prgBarXInst->bkgBorderColor), absPos, AreaArray, 2 );
	status = status && showBkgAndBorder( &(prgBarXInst->bkgBorderColor), absPos, &(prgBarXInst->axisArea), 1 );

	//子模块显示

	//子模块绝对位置
	AreaRange subModuAbsPos;

	//显示运动轴
	getAbsPos( absPos, &(prgBarXInst->axisArea), &subModuAbsPos );
	status = status && showIconBlock( &(prgBarXInst->axis), &subModuAbsPos );

	//显示进度点
	getAbsPos( absPos, &dotRealArea, &subModuAbsPos );
	status = status && showIconBlock( &(prgBarXInst->dot), &subModuAbsPos );

	return status;
}

//显示颜色板
bool showColorBoard( const ColorBoard *colorTableInst, const AreaRange *absPos )
{
	bool status = true;

	//每个色块
	const short colorXSize = colorTableInst->colorXSize;//横向长度
	const short colorYSize = colorTableInst->colorYSize;//纵向长度
	//色块个数
	const short colorXNum = colorTableInst->colorXNum;//横向颜色个数
	const short colorYNum = colorTableInst->colorYNum;//纵向颜色个数
	const short colorSNum = colorXNum * colorYNum;//颜色总个数
	//颜色映射表
	const color_u16 *const colorSpace = colorTableInst->colorSpace;

	//区域尺寸
	short xBasePos = absPos->x_min - 1;
	short yBasePos = absPos->y_min - 1;
	short width = absPos->x_max - xBasePos;
	short height = absPos->y_max - yBasePos;

	short curColorBaseIndex= 0;
	short curColorIndex = 0;

	//xPos,yPos为相对位置
	short xPos, yPos;
	short colorYoffset = 0;
	short colorXoffset = 0;
	//显示色板
	for ( yPos = 1; yPos <= height; yPos ++ )
	{
		if ( colorYoffset == colorYSize )
		{
			colorYoffset = 1;
			curColorBaseIndex += colorXNum;
		}
		else
		{
			colorYoffset ++;
		}
		curColorIndex = curColorBaseIndex;

		colorXoffset = 0;

		for( xPos = 1; xPos <= width && curColorIndex < colorSNum; xPos ++ )
		{
			screenShowSpace[yBasePos + yPos-1][xBasePos+xPos-1] = colorSpace[curColorIndex];

			if ( colorXoffset == colorXSize )
			{
				colorXoffset = 1;
				curColorIndex ++;
			}
			else
			{
				colorXoffset ++;
			}
		}
	}


	//突出选中颜色的边界

	if ( colorTableInst->colorIndex != NULL )
	{
		AreaRange colorBorderPos;//相对位置
		short colorIndex = *(colorTableInst->colorIndex);
		color_u16 borderColor = colorSpace[colorIndex];
		short colorYIndex = ( colorIndex + colorXNum ) / colorXNum;
		short colorXIndex = colorIndex - (colorYIndex-1) * colorXNum + 1;
		//首先计算选中颜色块的外边界
		colorBorderPos.x_min = colorXIndex * (colorXSize-1);
		colorBorderPos.x_max = colorXIndex * colorXSize + 1;
		colorBorderPos.y_min = colorYIndex * (colorYSize-1);
		colorBorderPos.y_max = colorYIndex * colorYSize + 1;

		//选中颜色块的外边界在显示域absPos内
		if ( colorBorderPos.x_min <= width && colorBorderPos.y_min <= height )
		{
			if ( colorBorderPos.x_min < 1 )
			{
				colorBorderPos.x_min = 1;
			}
			if ( colorBorderPos.y_min < 1 )
			{
				colorBorderPos.y_min = 1;
			}
			if ( colorBorderPos.x_max > width )
			{
				colorBorderPos.x_max = width;
			}
			if ( colorBorderPos.y_max > height )
			{
				colorBorderPos.y_max = height;
			}
			AreaRange colorBorderAbsPos;
			getAbsPos( absPos, &colorBorderPos, &colorBorderAbsPos );
			status = status && showBorder( &colorBorderAbsPos, borderColor );
		}
	}

	return status;
}


//显示screen_header
bool showScreenHeader( const ScreenHeaderTime *scrHeaderInst, const AreaRange *scrHeaderAbsPos )
{
	bool status = true;

	//显示screen_header的背景与边界
	status = status && showBkgAndBorder( &(scrHeaderInst->bkgBorderColor), scrHeaderAbsPos, &(scrHeaderInst->tagTimeArea), 1 );

	//显示时间标签
	AreaRange tagTimeAbsPos;//时间标签绝对位置
	//计算绝对位置
	getAbsPos( scrHeaderAbsPos, &(scrHeaderInst->tagTimeArea), &tagTimeAbsPos );
	//显示Tag
	status = status && showTagBlock( &(scrHeaderInst->tagTime), &tagTimeAbsPos );


	return status;
}
//显示screen_footer
bool showScreenFooter( const ScreenFooterBtn *scrFooterInst, const AreaRange *scrFooterAbsPos )
{
	bool status = true;

	//显示screen_footer的背景与边界
	AreaRange AreaArray[3] = { scrFooterInst->iconSettingArea, scrFooterInst->iconHomeArea, scrFooterInst->iconBackArea };
	status = status && showBkgAndBorder( &(scrFooterInst->bkgBorderColor), scrFooterAbsPos, AreaArray, 3 );

	//显示3个按键
	AreaRange iconAbsPos;//图标绝对位置


	//显示编辑键
	getAbsPos( scrFooterAbsPos, &(scrFooterInst->iconSettingArea), &iconAbsPos );
	status = status && showIconBlock( &(scrFooterInst->iconSetting), &iconAbsPos );

	//显示主页键
	getAbsPos( scrFooterAbsPos, &(scrFooterInst->iconHomeArea), &iconAbsPos );
	status = status && showIconBlock( &(scrFooterInst->iconHome), &iconAbsPos );

	//显示返回键
	getAbsPos( scrFooterAbsPos, &(scrFooterInst->iconBackArea), &iconAbsPos );
	status = status && showIconBlock( &(scrFooterInst->iconBack), &iconAbsPos );

	return status;
}
//显示screen_main_home
bool showScreenMainHome( const ScreenHome *scrHomeInst, const AreaRange *scrMainAbsPos )
{
	bool status = true;

	//显示背景与边界
	AreaRange AreaArray[3] = { scrHomeInst->numBarArea, scrHomeInst->txtCatalogArea, scrHomeInst->turnCatalogBarArea };
	status = status && showBkgAndBorder( &(scrHomeInst->bkgBorderColor), scrMainAbsPos, AreaArray, 3 );

	//显示子模块

	//子模块绝对位置
	AreaRange subModuAbsPos;

	//显示数字编号条（列表）
	getAbsPos( scrMainAbsPos, &(scrHomeInst->numBarArea), &subModuAbsPos );
	status = status && showTagList( &(scrHomeInst->numBar), &subModuAbsPos, scrHomeInst->listYOffset );

	//显示txt文本文件名列表
	getAbsPos( scrMainAbsPos, &(scrHomeInst->txtCatalogArea), &subModuAbsPos );
	status = status && showTagList( &(scrHomeInst->txtCatalog), &subModuAbsPos, scrHomeInst->listYOffset );

	//显示上下翻滚动条
	getAbsPos( scrMainAbsPos, &(scrHomeInst->turnCatalogBarArea), &subModuAbsPos );
	status = status && showScrollBarY( &(scrHomeInst->turnCatalogBarY), &subModuAbsPos );

	return status;
}
//显示screen_main_book
bool showScreenMainBook( const ScreenBook *scrBookInst, const AreaRange *scrMainAbsPos )
{
	bool status = true;

	//如果页面阅读进度信息可见
	if ( scrBookInst->pageInfoVisible )
	{
		//显示背景与边界
		AreaRange AreaArray[2] = { scrBookInst->txtBookArea, scrBookInst->pageInfoArea };
		status = status && showBkgAndBorder( &(scrBookInst->bookColorInfo), scrMainAbsPos, AreaArray, 2 );

		//显示子模块

		//子模块绝对位置
		AreaRange subModuAbsPos;

		//显示电子书区域(因为下方被下面弹出的阅读进度信息覆盖，所以区域变窄
		getAbsPos( scrMainAbsPos, &(scrBookInst->txtBookNarrowArea), &subModuAbsPos );
		status = status && showTagList( &(scrBookInst->txtBook), &subModuAbsPos, 0 );

		//显示电子书阅读进度信息
		getAbsPos( scrMainAbsPos, &(scrBookInst->pageInfoArea), &subModuAbsPos );
		status = status && showScreenPageInfo( &(scrBookInst->pageInfo), &subModuAbsPos );
	}
	else
	{
		//显示背景与边界
		status = status && showBkgAndBorder( &(scrBookInst->bookColorInfo), scrMainAbsPos, &(scrBookInst->txtBookArea), 1 );

		//显示电子书区域
		AreaRange subModuAbsPos;
		getAbsPos( scrMainAbsPos, &(scrBookInst->txtBookArea), &subModuAbsPos );
		status = status && showTagList( &(scrBookInst->txtBook), &subModuAbsPos, 0 );
	}

	return status;
}
//显示screen_main_setting
bool showScreenMainSetting( const ScreenSetting *scrSettingInst, const AreaRange *scrMainAbsPos )
{
	bool status = true;

	//显示screen_footer的背景与边界
	AreaRange AreaArray[8] = { scrSettingInst->tag1Area, scrSettingInst->editAlarmHourArea, scrSettingInst->editAlarmMinuteArea,\
			scrSettingInst->editBkgColorArea, scrSettingInst->editWordColorArea, scrSettingInst->editTurnPageModArea,\
			scrSettingInst->tag2Area, scrSettingInst->editTurnPageSecArea };
	status = status && showBkgAndBorder( &(scrSettingInst->bkgBorderColor), scrMainAbsPos, AreaArray, 8 );

	//显示子模块

	//子模块绝对位置
	AreaRange subModuAbsPos;

	//显示静态标签："休息定时"
	getAbsPos( scrMainAbsPos, &(scrSettingInst->tag1Area), &subModuAbsPos );
	status = status && showTagBlock( &(scrSettingInst->tag1), &subModuAbsPos );

	//显示可编辑的数字标签图标组：编辑休息定时：时
	getAbsPos( scrMainAbsPos, &(scrSettingInst->editAlarmHourArea), &subModuAbsPos );
	status = status && showTagIconGroup( &(scrSettingInst->editAlarmHour), &subModuAbsPos );

	//显示可编辑的数字标签图标组：编辑休息定时：分
	getAbsPos( scrMainAbsPos, &(scrSettingInst->editAlarmMinuteArea), &subModuAbsPos );
	status = status && showTagIconGroup( &(scrSettingInst->editAlarmMinute), &subModuAbsPos );

	//显示标签组：编辑背景颜色
	getAbsPos( scrMainAbsPos, &(scrSettingInst->editBkgColorArea), &subModuAbsPos );
	status = status && showTagGroup( &(scrSettingInst->editBkgColor), &subModuAbsPos );

	//显示标签组：编辑文本颜色
	getAbsPos( scrMainAbsPos, &(scrSettingInst->editWordColorArea), &subModuAbsPos );
	status = status && showTagGroup( &(scrSettingInst->editWordColor), &subModuAbsPos );

	//显示标签组：自动翻页与手动翻页切换
	getAbsPos( scrMainAbsPos, &(scrSettingInst->editTurnPageModArea), &subModuAbsPos );
	status = status && showTagGroup( &(scrSettingInst->editTurnPageMod), &subModuAbsPos );

	//显示静态标签："翻页定时"
	getAbsPos( scrMainAbsPos, &(scrSettingInst->tag2Area), &subModuAbsPos );
	status = status && showTagBlock( &(scrSettingInst->tag2), &subModuAbsPos );

	//显示可编辑的数字标签图标组：编辑翻页定时：秒
	getAbsPos( scrMainAbsPos, &(scrSettingInst->editTurnPageSecArea), &subModuAbsPos );
	status = status && showTagIconGroup( &(scrSettingInst->editTurnPageSec), &subModuAbsPos );

	return status;
}
//显示screen_main_color_picker
bool showScreenMainColorPicker( const ScreenColorPicker *scrColorPickerInst, const AreaRange *scrMainAbsPos )
{
	bool status = true;

	//画边界与背景
	//显示边界与背景
	status = status && showBkgAndBorder( &(scrColorPickerInst->bkgBorderColor), scrMainAbsPos, &(scrColorPickerInst->colorPickerArea), 1 );

	//显示取色板
	AreaRange colorTableAbsPos;//色板绝对位置
	getAbsPos( scrMainAbsPos, &(scrColorPickerInst->colorPickerArea), &colorTableAbsPos );
	status = status && showColorBoard( &(scrColorPickerInst->colorPicker), &colorTableAbsPos );

	return status;
}


//显示screen
bool showScreen( )
{
	bool status = true;

	const ScreenContainer *tftlcdScreenPtr = &tftlcdScreenInst;

	//显示screen_header
	status = status && showScreenHeader( &(tftlcdScreenPtr->scrHeader), &(tftlcdScreenPtr->headerArea) );
	//显示screen_footer
	status = status && showScreenFooter( &(tftlcdScreenPtr->scrFooter), &(tftlcdScreenPtr->footerArea) );

	//显示screen_main
	short curScreenIndex = (tftlcdScreenPtr->mainAreaInfo).curScrIndex;
	switch( ((tftlcdScreenPtr->mainAreaInfo).scrIDStack)[curScreenIndex] )
	{
		case SCR_HOME:
			status = status && showScreenMainHome( &(tftlcdScreenPtr->scrHome), &(tftlcdScreenPtr->mainArea) );
			break;
		case SCR_BOOK:
			status = status && showScreenMainBook( &(tftlcdScreenPtr->scrBook), &(tftlcdScreenPtr->mainArea) );
			break;
		case SCR_SETTING:
			status = status && showScreenMainSetting( &(tftlcdScreenPtr->scrSetting), &(tftlcdScreenPtr->mainArea) );
			break;
		case SCR_COLOR_PICKER:
			status = status && showScreenMainColorPicker( &(tftlcdScreenPtr->scrColorPicker), &(tftlcdScreenPtr->mainArea) );
			break;
		default:
			status = false;
	}


	return status;
}

