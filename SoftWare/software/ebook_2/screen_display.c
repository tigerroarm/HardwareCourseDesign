/*
 * screen_display.c
 *
 *  Created on: 2019年5月1日
 *      Author: 12757
 */

#include <stdlib.h>
#include "screen_display.h"
#include "screen.h"
#include "test_value.h"

//调试完记得删了(或者注释)
//extern const alt_u8 gbk_font_x16y16[];

//该数组用于临时测试屏幕显示效果，最后的工程会删除该数组
color_u16 screenShowSpace[SCR_HEIGHT][SCR_WIDTH];

//获取新的绝对位置
bool getAbsPos( const AreaRange *baseAbsPos, const AreaRange *relativePos, AreaRange *newAbsPos )
{
    //检查区域格式是否满足width,height>= 0
    bool status1 = checkAreaRange( baseAbsPos );
    bool status2 = checkAreaRange( relativePos );

    bool status = status1 && status2;

    if ( status )//true
    {
        short absWidth = baseAbsPos->x_max - baseAbsPos->x_min + 1;
        short absHeight = baseAbsPos->y_max - baseAbsPos->y_min + 1;

        //如果两个区间有交集
        if ( relativePos->x_min >= 1 && relativePos->x_min <= absWidth &&
             relativePos->y_min >= 1 && relativePos->y_min <= absHeight )
        {
            newAbsPos->x_min = baseAbsPos->x_min + relativePos->x_min - 1;
            newAbsPos->x_max = baseAbsPos->x_min + relativePos->x_max - 1;
            newAbsPos->y_min = baseAbsPos->y_min + relativePos->y_min - 1;
            newAbsPos->y_max = baseAbsPos->y_min + relativePos->y_max - 1;
            //取交集
            if ( newAbsPos->x_max > baseAbsPos->x_max )
            {
                newAbsPos->x_max = baseAbsPos->x_max;
            }
            if ( newAbsPos->y_max > baseAbsPos->y_max )
            {
                newAbsPos->y_max = baseAbsPos->y_max;
            }
        }
        else//没有交集，则设为保护区间（1,1,0,0）
        {
            newAbsPos->x_min = 1;
            newAbsPos->x_max = 0;
            newAbsPos->y_min = 1;
            newAbsPos->y_max = 0;
        }
    }
    else//false
    {
        //设为保护区间（1,1,0,0）
        newAbsPos->x_min = 1;
        newAbsPos->x_max = 0;
        newAbsPos->y_min = 1;
        newAbsPos->y_max = 0;
    }

	return status;
}

//获取x方向的区域定位数组(返回数组大小）
short getXVaildRange( const char areaOccurInfo[], const AreaRange exceptAreaArray[], const short exceptAreaNum, \
		short *xMinArray, short *xMaxArray, const short width )
{

	char *exceptXposInRow = (char*)malloc( sizeof(char) * (width+1) );

//    if ( exceptAreaNum == 0 )
//    {
//        printf( "width=%d\n", width );
//    }

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

			short xPosMin = ( curExceptArea->x_min > 0 )?(curExceptArea->x_min):1;
			short xPosMax = ( curExceptArea->x_max > width )?width:(curExceptArea->x_max);

			for ( xPos = xPosMin; xPos <= xPosMax; xPos ++ )
			{
				exceptXposInRow[xPos-1] = 'e';//对非背景像素做作标记
			}
		}
	}
	short rangeIndex = 0;

//	printf( "width=%d\n", width );

//    for ( i = 0; i < width+1; i ++ )
//	{
//		printf( "x[%d]=%c\n", i, exceptXposInRow[i] );
//	}

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
				xMaxArray[rangeIndex] = xPos - 1;
				rangeIndex ++;
			}
		}

		preRowXType = curRowXType;
	}
	free(exceptXposInRow);

	return rangeIndex;
}

//画边界(就是画个长方形的边)
bool showBorder( const AreaRange *borderAbsPos, color_u16 borderColor )
{
	short absPosWidth = borderAbsPos->x_max - borderAbsPos->x_min + 1;
	short absPosHeight = borderAbsPos->y_max - borderAbsPos->y_min + 1;


	if ( absPosWidth < 0 || absPosHeight < 0 )
    {
        return false;
    }
    else if ( absPosWidth == 0 || absPosHeight == 0 )//无区域，更无边界，直接返回
    {
        return true;
    }
    else if ( absPosWidth == 1 || absPosHeight == 1 )//只能画一条边界
    {
        int xPos, yPos;
        if ( absPosHeight == 1 )//只有横向线
        {
            //横向画一条边界线
            yPos = borderAbsPos->y_min;
            for ( xPos = borderAbsPos->x_min; xPos <= borderAbsPos->x_max; xPos ++ )
            {
                screenShowSpace[yPos-1][xPos-1] = borderColor;
            }
        }
        else
        {
            //纵向画一条边界线
            xPos = borderAbsPos->x_min;
            for ( yPos = borderAbsPos->y_min; yPos <= borderAbsPos->y_max; yPos ++ )
            {
                screenShowSpace[yPos-1][xPos-1] = borderColor;
            }
        }

    }
    else//一般情况
    {
        int xPos, yPos;
        //横向(x方向)的边界线
        yPos = borderAbsPos->y_min;//上边界线
        for ( xPos = borderAbsPos->x_min; xPos <= borderAbsPos->x_max; xPos ++ )
        {
            screenShowSpace[yPos-1][xPos-1] = borderColor;
        }
        yPos = borderAbsPos->y_max;//下边界线
        for ( xPos = borderAbsPos->x_min; xPos <= borderAbsPos->x_max; xPos ++ )
        {
            screenShowSpace[yPos-1][xPos-1] = borderColor;
        }

        //纵向(y方向)的边界线
        xPos = borderAbsPos->x_min;//左边界线
        for ( yPos = borderAbsPos->y_min + 1; yPos < borderAbsPos->y_max; yPos ++ )
        {
            screenShowSpace[yPos-1][xPos-1] = borderColor;
        }
        xPos = borderAbsPos->x_max;//右边界线
        for ( yPos = borderAbsPos->y_min + 1; yPos < borderAbsPos->y_max; yPos ++ )
        {
            screenShowSpace[yPos-1][xPos-1] = borderColor;
        }

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
	bool status = true, status1;

	short width = absPos->x_max - absPos->x_min + 1;
	short height = absPos->y_max - absPos->y_min + 1;
	short xBasePos = absPos->x_min - 1;
	short yBasePos = absPos->y_min - 1;

	color_u16 bkgColor = bkgBorderColor->bkgColor;

	AreaRange *exceptAreaArrayCopy = malloc( sizeof(AreaRange) * exceptAreaNum );
	int i;
	for ( i = 0; i < exceptAreaNum; i ++ )
    {
        exceptAreaArrayCopy[i] = exceptAreaArray[i];
    }

	if ( bkgBorderColor->borderVisible )//边界可见
	{
		//画边界
		status1 = showBorder( absPos, bkgBorderColor->borderColor );
		status = status && status1;
		//将绝对区域缩小一圈，即除去刚画的边界
		width -= 2;
		height -= 2;
		xBasePos += 1;
		yBasePos += 1;
		//调整exceptAreaArray[]例外区的相对位置
		AreaRange *curExceptArea = exceptAreaArrayCopy;

		for ( i = 0; i < exceptAreaNum; i ++ )
        {
            curExceptArea->x_min -= 1;
            curExceptArea->x_max -= 1;
            curExceptArea->y_min -= 1;
            curExceptArea->y_max -= 1;

            curExceptArea ++;
        }
	}

	//显示背景颜色
	//此处xPos,yPos为相对位置
	short xPos, yPos;
	char *areaOccurInfo = (char *)malloc( sizeof(char) * exceptAreaNum );
//	if ( exceptAreaNum == 0 )
//    {
//        printf( "areaOccurInfo == %p\n", areaOccurInfo );
//        printf( "malloc return == %p\n", malloc( 0 ) );
//    }
	short *xMinArray = (short *)malloc( sizeof(short) * (exceptAreaNum+1) );;
	short *xMaxArray = (short *)malloc( sizeof(short) * (exceptAreaNum+1) );;

	for ( i = 0; i < exceptAreaNum; i ++ )
	{
		areaOccurInfo[i] = 0;//清空
	}
//	printf( "haha1\n" );
//	printf( "absPos=\n" );
//	displayAreaRange( absPos, 0 );
	for ( i = 0; i < exceptAreaNum; i ++ )
    {
//        printf( "exceptAreaArrayCopy[%d]=\n", i );
//        displayAreaRange( exceptAreaArrayCopy+i, 0 );
    }

	short xRangeNum = 0;
	for ( yPos = 1; yPos <= height; yPos ++ )
	{
		//统计该行出现的非背景区
//		printf( "yPos=%d\n", yPos );
		bool changeOccur = false;
		if ( exceptAreaNum <= 0 && yPos == 1 )
        {
            changeOccur = true;
        }
		for ( i = 0; i < exceptAreaNum; i ++ )
		{
			const AreaRange *curExceptArea = &(exceptAreaArrayCopy[i]);
			if ( yPos < curExceptArea->y_min || yPos > curExceptArea->y_max )
			{
				if ( areaOccurInfo[i] != 'n' )
				{
					changeOccur = true;
				}
				areaOccurInfo[i] = 'n';//非背景区
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
		}
//		if ( exceptAreaNum == 0 )
//        {
//            printf( "changeOccur=%d\n", changeOccur );
//        }

		//如果非背景区发生变化，则修改非背景的x定位数组
        if ( changeOccur )
        {
            xRangeNum = getXVaildRange( areaOccurInfo, exceptAreaArrayCopy, exceptAreaNum, xMinArray, xMaxArray, width );

        }
//        printf( "xRangeNum=%d\n", xRangeNum );
        //画背景
//        if ( exceptAreaNum == 0 )
//        {
//            printf( "xRangeNum=%d\n", xRangeNum );
//        }
        for ( i = 0; i < xRangeNum; i ++ )
        {
//            printf( "xRange:[%d,%d]\n", xMinArray[i], xMaxArray[i] );
//            if ( exceptAreaNum == 0 )
//            {
//                printf( "xRange:[%d,%d]\n", xMinArray[i], xMaxArray[i] );
//            }
            short xPosMin = ( xMinArray[i] > 0 )?(xMinArray[i]):0;
            short xPosMax = ( xMaxArray[i] > width )?width:(xMaxArray[i]);
            for ( xPos = xPosMin; xPos <= xPosMax; xPos ++ )
            {
                screenShowSpace[yBasePos+yPos-1][xBasePos+xPos-1] = bkgColor;
            }
        }
	}

	free(areaOccurInfo);
	free(xMinArray);
	free(xMaxArray);
	free(exceptAreaArrayCopy);

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
//	    printf( "yPos = %d\n", yPos );
		short objPicIndex = objPicRowBaseIndex;
		for ( xPos = 1; xPos <= width; xPos ++ )
		{
			if ( (xPos & 0x7) == 0x1 )//xPos低三位为001（等效为xPos % 8 == 1 )
			{
//				printf( "xixi1\n" );
//				printf( "objPicPtr = %p\n", objPicPtr );
				objPicRowPixel = ( objPicPtr != NULL ) ? (objPicPtr[objPicIndex]) : 0x00;
				objPicIndex ++;
//				printf( "xixi1_1\n" );
			}
			if ( (objPicRowPixel & 0x80) == 0x80 )//最高位为1
			{
//				printf( "xixi2\n" );
				screenShowSpace[yBasePos+yPos-1][xBasePos+xPos-1] = objColor;
			}
			else
			{
//				printf( "xixi3\n" );
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
	bool status = true, status1;

	//显示TagBlock的背景与边界
	bool textVisible = (tagPtr->tagColorInfo).objVisible;

	if ( textVisible )//中间的文本需要显示
	{
		//处理背景与边界（挖掉text显示区）
		status1 = showBkgAndBorder( &(tagPtr->tagColorInfo), absPos, &(tagPtr->textArea), 1 );
		status = status && status1;

		//显示中间的text

		AreaRange textAbsPos;//text的绝对位置
		//获取text的绝对位置
		status1 = getAbsPos( absPos, &(tagPtr->textArea), &textAbsPos );
		status = status && status1;

		AreaRange objAbsPos;//字符的绝对位置

		objAbsPos.y_max = textAbsPos.y_max;
		objAbsPos.y_min = textAbsPos.y_min;
		objAbsPos.x_min = textAbsPos.x_min;
        objAbsPos.x_max = objAbsPos.x_min-1;

        short textLen = (tagPtr->tagText).textLen;
        char *text = (tagPtr->tagText).text;
        short textIndex = 0;

        short bkgColor = (tagPtr->tagColorInfo).bkgColor;
        short objColor =  (tagPtr->tagColorInfo).objColor;

        short objYSize = tagPtr->objYSize;

        const alt_u8 *objPicPtr;

		//获取文本中汉字与字母的分布表(中文"c",英文"e",结束标志0)(返回动态字符串，记得用完了要free掉)
		char *strType = getWordLetterArrange( (tagPtr->tagText).text, (tagPtr->tagText).textLen );
		//显示第一个字符
		if ( strType != NULL && strType[0] != 0 )
		{
			if( objAbsPos.x_min <= textAbsPos.x_max )//当前字节位置处于显示范围内
            {
                short objXSize;

                if ( strType[0] == 'e' )
                {
                    objXSize = tagPtr->letterXSize;
                    objPicPtr = getAsciiFont( text[0] );
                    textIndex = 1;
                }
                else
                {
                    objXSize = tagPtr->wordXSize;
                    objPicPtr = getGbkFont( text[0], text[1] );
                    textIndex = 2;
                }

    //            printf( "i= %d\n", 0 );
    //            printf( "strType[%d] = %c\n", 0, strType[0] );

                objAbsPos.x_max = objAbsPos.x_min - 1 + objXSize;
                if ( objAbsPos.x_max > textAbsPos.x_max )
                {
                    objAbsPos.x_max = textAbsPos.x_max;
                }
                status1 = showTextOrIcon( &objAbsPos, objPicPtr, bkgColor, objColor, objXSize, objYSize );
                status = status && status1;

                int i;

                for ( i = 1; strType[i] != 0 && textIndex < textLen; i ++ )
                {
                    //显示间隔
    //				printf( "i= %d\n", i );
    //				printf( "strType[%d] = %c\n", i, strType[i] );
                    objAbsPos.x_min = objAbsPos.x_max + 1;
                    if( objAbsPos.x_min > textAbsPos.x_max )//超出文本显示右边界
                    {
    //					printf( "textAbsPos.x_max = %d\n", textAbsPos.x_max );
    //					printf( "objAbsPos.x_min = %d\n", objAbsPos.x_min );
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

                    if ( objAbsPos.x_max > textAbsPos.x_max )//防止水平超出边界
                    {
                        objAbsPos.x_max = textAbsPos.x_max;
                    }

                    status1 = showTextOrIcon( &objAbsPos, NULL, bkgColor, objColor, objXSize, objYSize );
                    status = status && status1;

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
    //					printf( "text[textIndex] = %x\n", (alt_u8)(text[textIndex]) );
    //					printf( "text[textIndex+1] = %x\n", (alt_u8)(text[textIndex+1]) );
    //					char tempStr[3] = { text[textIndex], text[textIndex+1], 0 };
    //					printf( "char = %s\n", tempStr );
                        textIndex += 2;
                    }
                    objAbsPos.x_max = objAbsPos.x_min - 1 + objXSize;
                    if ( objAbsPos.x_max > textAbsPos.x_max )//防止水平超出边界
                    {
                        objAbsPos.x_max = textAbsPos.x_max;
                    }
    //            displayAreaRange( &objAbsPos, 4 );
    //            printf( "objXSize = %d\n", objXSize );
    //            printf( "objYSize = %d\n", objYSize );
    //            printf( "objPicPtrOffset = %d\n", gbk_font_x16y16 - objPicPtr );
    //            printf( "gbk_font_x16y16 = %p\n", gbk_font_x16y16 );
    //            printf( "objPicPtr = %p\n", objPicPtr );
                    status1 = showTextOrIcon( &objAbsPos, objPicPtr, bkgColor, objColor, objXSize, objYSize );
                    status = status && status1;
                }
            }
		}
//    printf( "\n" );
        //如果文本区右边界还有余留空间，就填充背景颜色
        objAbsPos.x_min = objAbsPos.x_max + 1;
        if ( objAbsPos.x_min <= textAbsPos.x_max )
        {
            objAbsPos.x_max = textAbsPos.x_max;
            status1 = showTextOrIcon( &objAbsPos, NULL, bkgColor, objColor, objAbsPos.x_max - objAbsPos.x_min + 1, objYSize );
            status = status && status1;
        }
        //如果文本区下方有余留空间，就填充背景颜色
        short objAreaYdiff = objAbsPos.y_max - objAbsPos.y_min + 1 - objYSize;
        if ( objAreaYdiff > 0 )
        {
            objAbsPos.x_min = textAbsPos.x_min;
            objAbsPos.x_max = textAbsPos.x_max;
            objAbsPos.y_min = textAbsPos.y_max - objAreaYdiff + 1;
            objAbsPos.y_max = textAbsPos.y_max;

            status1 = showTextOrIcon( &objAbsPos, NULL, bkgColor, objColor, objAbsPos.x_max - objAbsPos.x_min + 1, objAreaYdiff );
            status = status && status1;
        }
		free(strType);
	}
	else//中间的icon不需要显示
	{
		//只处理背景与边界颜色
		status1 = showBkgAndBorder( &(tagPtr->tagColorInfo), absPos, NULL, 0 );
		status = status && status1;
	}

	return status;
}

//显示TagBlock(带纵向基区上偏移)
bool showTagWithYOffset( const TagBlock *tagPtr, const AreaRange *absPos, const short yOffset )
{
	bool status = true, status1;

	//显示TagBlock的背景与边界
	bool textVisible = (tagPtr->tagColorInfo).objVisible;


	short wordPicPtrOffset = 0;
	short letterPicPtrOffset = 0;
	short diffY = 0;//文本区域纵向削减的高度

//	printf( "yOffset = %d\n", yOffset );
//
//	printf( "tagPtr->textArea = \n" );
//	displayAreaRange( &(tagPtr->textArea), 4 );

	if ( yOffset+1 >= (tagPtr->textArea).y_min )
	{
		diffY = yOffset - (tagPtr->textArea).y_min + 1;
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
		status1 = showBkgAndBorder( &(tagPtr->tagColorInfo), &absNewPos, &(tagPtr->textArea), 1 );
		status = status && status1;


		//显示中间的text

		AreaRange textAbsPos;//text的绝对位置
		//获取text的绝对位置
		status1 = getAbsPos( absPos, &(tagPtr->textArea), &textAbsPos );
		status = status && status1;

		AreaRange objAbsPos;//字符的绝对位置

		objAbsPos.y_max = textAbsPos.y_max;
		objAbsPos.y_min = textAbsPos.y_min + diffY;
//		printf( "textAbsPos.y_max = %d\n", textAbsPos.y_max );
//		printf( "textAbsPos.y_min = %d\n", textAbsPos.y_min );
//		printf( "objAbsPos.y_max = %d\n", objAbsPos.y_max );
//		printf( "objAbsPos.y_min = %d\n", objAbsPos.y_min );
		objAbsPos.x_min = textAbsPos.x_min;
        objAbsPos.x_max = objAbsPos.x_min-1;

        short textLen = (tagPtr->tagText).textLen;
        char *text = (tagPtr->tagText).text;
        short textIndex = 0;

        short bkgColor = (tagPtr->tagColorInfo).bkgColor;
        short objColor =  (tagPtr->tagColorInfo).objColor;

        short objYSize = tagPtr->objYSize;

        const alt_u8 *objPicPtr;

		//获取文本中汉字与字母的分布表(中文"c",英文"e",结束标志0)(返回动态字符串，记得用完了要free掉)
		char *strType = getWordLetterArrange( text, textLen );

		//如果有文本需要显示
		if ( strType != NULL || strType[0] != 0 )
		{
            if( objAbsPos.x_min <= textAbsPos.x_max )//当前字节位置处于显示范围内
            {
                short objXSize;

                //显示第一个字符
                if ( strType[0] == 'e' )
                {
                    objXSize = tagPtr->letterXSize;
                    objPicPtr = getAsciiFont( text[0] ) + letterPicPtrOffset;
                    textIndex = 1;
                }
                else
                {
                    objXSize = tagPtr->wordXSize;
                    objPicPtr = getGbkFont( text[0], text[1] ) + wordPicPtrOffset;
                    textIndex = 2;
                }
                objAbsPos.x_max = objAbsPos.x_min - 1 + objXSize;
                if ( objAbsPos.x_max > textAbsPos.x_max )//防止水平超出边界
                {
                    objAbsPos.x_max = textAbsPos.x_max;
                }
                status1 = showTextOrIcon( &objAbsPos, objPicPtr, bkgColor, objColor, objXSize, objYSize );
                status = status && status1;

                int i;
                //显示之后的字符
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
                    if ( objAbsPos.x_max > textAbsPos.x_max )//防止水平超出边界
                    {
                        objAbsPos.x_max = textAbsPos.x_max;
                    }

                    status1 = showTextOrIcon( &objAbsPos, NULL, bkgColor, objColor, objXSize, objYSize );
                    status = status && status1;

                    //显示文本
                    objAbsPos.x_min = objAbsPos.x_max + 1;
                    if( objAbsPos.x_min > textAbsPos.x_max )//超出文本显示右边界
                    {
                        break;
                    }
                    if( strType[i] == 'e' )//当前字符为英文字母
                    {
                        objXSize = tagPtr->letterXSize;
                        objPicPtr = getAsciiFont( text[textIndex] ) + letterPicPtrOffset;
                        textIndex += 1;
                    }
                    else
                    {
                        objXSize = tagPtr->wordXSize;
                        objPicPtr = getGbkFont( text[textIndex], text[textIndex+1] ) + wordPicPtrOffset;
                        textIndex += 2;
                    }
                    objAbsPos.x_max = objAbsPos.x_min - 1 + objXSize;
                    if ( objAbsPos.x_max > textAbsPos.x_max )//防止水平超出边界
                    {
                        objAbsPos.x_max = textAbsPos.x_max;
                    }

                    status1 = showTextOrIcon( &objAbsPos, objPicPtr, bkgColor, objColor, objXSize, objYSize );
                    status = status && status1;
                }
            }
		}
        //如果文本区右边界还有余留空间，就填充背景颜色
        objAbsPos.x_min = objAbsPos.x_max + 1;
        if ( objAbsPos.x_min <= textAbsPos.x_max )
        {
            objAbsPos.x_max = textAbsPos.x_max;
            status1 = showTextOrIcon( &objAbsPos, NULL, bkgColor, objColor, objAbsPos.x_max - objAbsPos.x_min + 1, objYSize );
            status = status && status1;
        }
        //如果文本区下方有余留空间，就填充背景颜色
        short objAreaYdiff = objAbsPos.y_max - objAbsPos.y_min + 1 - objYSize;
//			printf( "objAreaYdiff = %d\n", objAreaYdiff );
        if ( objAreaYdiff > 0 )
        {
            objAbsPos.x_min = textAbsPos.x_min;
            objAbsPos.x_max = textAbsPos.x_max;
            objAbsPos.y_min = textAbsPos.y_max - objAreaYdiff + 1;
            objAbsPos.y_max = textAbsPos.y_max;
            status1 = showTextOrIcon( &objAbsPos, NULL, bkgColor, objColor, objAbsPos.x_max - objAbsPos.x_min + 1, objAreaYdiff );
            status = status && status1;
        }
		free(strType);
	}
	else//中间的icon不需要显示
	{
		//只处理背景与边界颜色
		status1 = showBkgAndBorder( &(tagPtr->tagColorInfo), &absNewPos, NULL, 0 );
		status = status && status1;
		//用背景颜色覆盖掉上边界颜色
		if ( (tagPtr->tagColorInfo).borderVisible && yOffset > 0 )
		{
			AreaRange borderPos;
			borderPos.x_min = absNewPos.x_min;
			borderPos.x_max = absNewPos.x_max;
			borderPos.y_min = borderPos.y_max = absNewPos.y_min;
			status1 = showBorder( &borderPos, (tagPtr->tagColorInfo).bkgColor );
			status = status && status1;
		}
	}

	return status;
}

//显示IconBlock
bool showIconBlock( const IconBlock *iconPtr, const AreaRange *absPos )
{
	bool status = true, status1;

	//显示IconBlock的背景与边界
	bool iconPicVisible = (iconPtr->iconColorInfo).objVisible;

	if ( iconPicVisible )//中间的icon需要显示
	{
		//处理背景与边界（挖掉iconPic显示区）
		status1 = showBkgAndBorder( &(iconPtr->iconColorInfo), absPos, &(iconPtr->picArea), 1 );
		status = status && status1;

		//显示中间的icon图片

		AreaRange iconPicAbsPos;//iconPic的绝对位置
		//获取iconPic的绝对位置
		status1 = getAbsPos( absPos, &(iconPtr->picArea), &iconPicAbsPos );
		status = status && status1;
		//显示icon
		status1 = showTextOrIcon( &iconPicAbsPos, iconPtr->iconPic, (iconPtr->iconColorInfo).bkgColor, (iconPtr->iconColorInfo).objColor,\
				iconPtr->iconXSize, iconPtr->iconYSize );
        status = status && status1;
	}
	else//中间的icon不需要显示
	{
		//只处理背景与边界颜色
		status1 = showBkgAndBorder( &(iconPtr->iconColorInfo), absPos, NULL, 0 );
		status = status && status1;
	}

	return status;
}
//显示TagGroup
bool showTagGroup( const TagGroup *tagGroupInst, const AreaRange *absPos )
{
	bool status = true, status1;

//    printf( "absPos = \n" );
//	displayAreaRange( absPos, 4 );

	//显示TagGroup的背景与边界
	AreaRange AreaArray[2] = { tagGroupInst->staticTagArea, tagGroupInst->varTagArea };
	status1 = showBkgAndBorder( &(tagGroupInst->bkgBorderColor), absPos, AreaArray, 2 );
	status = status && status1;

	//显示2个标签

	//标签绝对位置
	AreaRange tagAbsPos;
//printf( "tagStatic\n" );
	//显示静态标签
	status1 = getAbsPos( absPos, &(tagGroupInst->staticTagArea), &tagAbsPos );
	status = status && status1;
	status1 = showTagBlock( &(tagGroupInst->staticTag), &tagAbsPos );
	status = status && status1;

//    printf( "absPos = \n" );
//	displayAreaRange( absPos, 4 );
//
//    printf( "staticTagArea = \n" );
//	displayAreaRange( &(tagGroupInst->staticTagArea), 4 );
//
//    printf( "tagAbsPos = \n" );
//	displayAreaRange( &tagAbsPos, 4 );
//	printf( "staticTag = \n" );
//	displayTagBlock( &(tagGroupInst->staticTag), 4 );
//printf( "tagVariable\n" );
	//显示动态标签
	status1 = getAbsPos( absPos, &(tagGroupInst->varTagArea), &tagAbsPos );
	status = status && status1;
	status1 = showTagBlock( &(tagGroupInst->varTag), &tagAbsPos );
	status = status && status1;

	return status;
}

//显示TagIconGroup
bool showTagIconGroup( const TagIconGroup *tagIconGpInst, const AreaRange *absPos )
{
	bool status = true, status1;

	//显示TagIconGroup的背景与边界
	AreaRange AreaArray[4] = { tagIconGpInst->plusIconArea, tagIconGpInst->valueTagArea,\
					tagIconGpInst->unitTagArea,tagIconGpInst->minusIconArea };
	status1 = showBkgAndBorder( &(tagIconGpInst->bkgBorderColor), absPos, AreaArray, 4 );
	status = status && status1;

	//显示子模块

	//子模块绝对位置
	AreaRange subModuAbsPos;

	//显示数字加键
	status1 = getAbsPos( absPos, &(tagIconGpInst->plusIconArea), &subModuAbsPos );
	status = status && status1;
	status1 = showIconBlock( &(tagIconGpInst->plusIcon), &subModuAbsPos );
	status = status && status1;

	//显示数字减键
	status1 = getAbsPos( absPos, &(tagIconGpInst->minusIconArea), &subModuAbsPos );
	status = status && status1;
	status1 = showIconBlock( &(tagIconGpInst->minusIcon), &subModuAbsPos );
	status = status && status1;

	//数字显示标签
	status1 = getAbsPos( absPos, &(tagIconGpInst->valueTagArea), &subModuAbsPos );
	status = status && status1;
	status1 = showTagBlock( &(tagIconGpInst->valueTag), &subModuAbsPos );
	status = status && status1;

	//数字单位标签
	status1 = getAbsPos( absPos, &(tagIconGpInst->unitTagArea), &subModuAbsPos );
	status = status && status1;
	status1 = showTagBlock( &(tagIconGpInst->unitTag), &subModuAbsPos );
	status = status && status1;

	return status;
}

//显示TagList
bool showTagList( const TagList *tagListInst, const AreaRange *absPos, const short tagListYOffset )
{
	bool status = true, status1;

//	printf( "tagListInst = \n" );
//	displayTagList( tagListInst, 4 );
//	printf( "absPos = \n" );
//	displayAreaRange( absPos, 4 );
//	printf( "tagListYOffset = %d\n", tagListYOffset );

	//列表显示空间absPos的高度
	short listShowHeight = absPos->y_max - absPos->y_min + 1;
	//列表显示空间absPos在列表TagListInst中的纵向位置
	short listShowYMin = tagListYOffset + 1;
	short listShowYMax = tagListYOffset + listShowHeight;

	short elemHeight = (tagListInst->elemBaseArea).y_max;

	short listShowIndexMin = (listShowYMin-1) / elemHeight;//列表中需要显示tag的最小行数下标（从0开始）
	short listShowIndexMax = (listShowYMax-1) / elemHeight;//列表中需要显示tag的最大行数下标（从0开始）

	short elemNum = tagListInst->elemNum;//列表元素总个数


	if ( listShowIndexMax >= elemNum )
	{
		listShowIndexMax = elemNum - 1;
	}
	//显示TagList列表中每一个落在显示域中的TagBlock
	short elemCurIndex;
	AreaRange curElemAbsPos;
	//计算偏移量tagListYOffset为0时基本模块的绝对位置
	status1 = getAbsPos( absPos, &(tagListInst->elemBaseArea), &curElemAbsPos );
	status = status && status1;
	//计算最小下标模块的绝对位置（叠加偏移量tagListYOffset与最小下标模块的下标elemCurIndex
	short diffY = listShowIndexMin * elemHeight - tagListYOffset;
	curElemAbsPos.y_min += diffY;
	curElemAbsPos.y_max += diffY;

	TagBlock * const tagBlockArray = tagListInst->elemBlock;
//
//	printf( "tagBlockArray = %p\n", tagBlockArray );
//	printf( "elemNum = %d\n", elemNum );
//	printf( "listShowIndexMax = %d\n", listShowIndexMax );

	if ( listShowIndexMin < elemNum && tagBlockArray != NULL && elemNum != 0 )
    {
        bool tagListBottomNarrowOccur = false;//是否出现列表底部高度被削减的现象

        alt_u16 borderColor;

        //显示第一个TagBlock
        short yOffset = absPos->y_min - curElemAbsPos.y_min;
        elemCurIndex = listShowIndexMin;
        if ( elemCurIndex >= 0 && elemCurIndex <= listShowIndexMax )
        {
            status1 = showTagWithYOffset( &(tagBlockArray[elemCurIndex]), &curElemAbsPos, yOffset );
            status = status && status1;
            //纵向移动位置
            curElemAbsPos.y_min += elemHeight;
            curElemAbsPos.y_max += elemHeight;
            borderColor = tagBlockArray[listShowIndexMax].tagColorInfo.bkgColor;
            //用背景颜色覆盖掉上边界颜色
            if ( tagBlockArray[elemCurIndex].tagColorInfo.borderVisible && yOffset > 0 && yOffset < elemHeight - 1 )
            {
                AreaRange moveTopBorderArea;
                moveTopBorderArea.y_min = absPos->y_min;
                moveTopBorderArea.y_max = absPos->y_min;

                //上边界左半段
                moveTopBorderArea.x_min = absPos->x_min+1;
                moveTopBorderArea.x_max = absPos->x_min+(tagListInst->elemTextArea).x_min-2;
                showBorder( &moveTopBorderArea, borderColor );

                //上边界右半段
                moveTopBorderArea.x_min = absPos->x_min+tagListInst->elemTextArea.x_max;
                moveTopBorderArea.x_max = absPos->x_max-1;
                showBorder( &moveTopBorderArea, borderColor );
            }
        }




    //printf( "listShowIndexMax = %d\n", listShowIndexMax );
        //显示之后的TagBlock
        for ( elemCurIndex = listShowIndexMin + 1; elemCurIndex <= listShowIndexMax; elemCurIndex ++ )
        {
    //		printf( "elemCurIndex = %d\n", elemCurIndex );
            //显示其他的TagBlock
            status1 = showTagBlock( &(tagBlockArray[elemCurIndex]), &curElemAbsPos );
    //		printf( "elemCurIndex = %d ,  curElemAbsPos = \n", elemCurIndex );
    //        displayAreaRange( &curElemAbsPos, 4 );
            status = status && status1;
            //纵向移动位置
            curElemAbsPos.y_min += elemHeight;
            curElemAbsPos.y_max += elemHeight;
            if( curElemAbsPos.y_max > absPos->y_max )
            {
//                printf( "curElemAbsPos.y_max = %d\n", curElemAbsPos.y_max );
//                printf( "absPos.y_max = %d\n", absPos->y_max );
                if ( curElemAbsPos.y_max - absPos->y_max < elemHeight-1 )
                {
                    tagListBottomNarrowOccur = true;
                }
                curElemAbsPos.y_max = absPos->y_max;
            }
        }
    //printf( "haha4\n" );
        //显示背景区域（直接用变量curElemAbsPos）
        if ( curElemAbsPos.y_min <= absPos->y_max )
        {
            curElemAbsPos.y_max = absPos->y_max;
            status1 = showBkgAndBorder( &(tagListInst->elemColorInfo), &curElemAbsPos, NULL, 0 );
            status = status && status1;
        }
        //如果列表底部高度被削减,消除列表底部的TagBlock边界
        if ( tagListBottomNarrowOccur )
        {
            AreaRange moveBottomBorderArea;//消除底部的边界

            moveBottomBorderArea.y_max = absPos->y_max;
            moveBottomBorderArea.y_min = absPos->y_max;

            //下边界左半段
            moveBottomBorderArea.x_min = absPos->x_min+1;
            moveBottomBorderArea.x_max = absPos->x_min+(tagListInst->elemTextArea).x_min-2;
            showBorder( &moveBottomBorderArea, borderColor );

            //下边界右半段
            moveBottomBorderArea.x_min = absPos->x_min+tagListInst->elemTextArea.x_max;
            moveBottomBorderArea.x_max = absPos->x_max-1;
            showBorder( &moveBottomBorderArea, borderColor );

            status = status && status1;
        }
    }
    else
    {
        //直接显示背景区域（直接用变量curElemAbsPos）
        curElemAbsPos.y_min = absPos->y_min;
        curElemAbsPos.y_max = absPos->y_max;
        status1 = showBkgAndBorder( &(tagListInst->elemColorInfo), &curElemAbsPos, NULL, 0 );
        status = status && status1;
    }
	return status;
}
//显示ScrollBarY
bool showScrollBarY( const ScrollBarY *scrollBarYInst, const AreaRange *absPos )
{
	bool status = true, status1;


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
		status1 = showBkgAndBorder( &(scrollBarYInst->barYColorInfo), absPos, AreaArray, 3 );
		status = status && status1;


		//显示滚动条
		status1 = getAbsPos( absPos, &barRealArea, &subModuAbsPos );
		status = status && status1;
		status1 = showIconBlock( &(scrollBarYInst->iconBar), &subModuAbsPos );
		status = status && status1;
	}
	else
	{
		//显示ScrollBarY的背景与边界
		AreaRange AreaArray[2] = { scrollBarYInst->iconMoveUpArea, scrollBarYInst->iconMoveDownArea };
		status1 = showBkgAndBorder( &(scrollBarYInst->barYColorInfo), absPos, AreaArray, 2 );
		status = status && status1;
	}

	//显示上移键
	status1 = getAbsPos( absPos, &(scrollBarYInst->iconMoveUpArea), &subModuAbsPos );
	status = status && status1;
	status1 = showIconBlock( &(scrollBarYInst->iconMoveUp), &subModuAbsPos );
	status = status && status1;

	//显示下移键
	status1 = getAbsPos( absPos, &(scrollBarYInst->iconMoveDownArea), &subModuAbsPos );
	status = status && status1;
	status1 = showIconBlock( &(scrollBarYInst->iconMoveDown), &subModuAbsPos );
	status = status && status1;


	return status;
}

//显示ScreenPageInfo
bool showScreenPageInfo( const ScreenPageInfo *scrPageInfoInst, const AreaRange *absPos )
{
	bool status = true, status1;

	//显示ScreenPageInfo的背景与边界
	AreaRange AreaArray[3] = { scrPageInfoInst->prgBarArea, scrPageInfoInst->tagPagePercTageArea,\
					scrPageInfoInst->tagPageNumArea };
	status1 = showBkgAndBorder( &(scrPageInfoInst->bkgBorderColor), absPos, AreaArray, 3 );
	status = status && status1;

	//子模块显示

	//子模块绝对位置
	AreaRange subModuAbsPos;

	//显示水平进度条
	status1 = getAbsPos( absPos, &(scrPageInfoInst->prgBarArea), &subModuAbsPos );
	status = status && status1;
	status1 = showProgressBarX( &(scrPageInfoInst->prgBarX), &subModuAbsPos );
	status = status && status1;

	//显示百分数进度
	status1 = getAbsPos( absPos, &(scrPageInfoInst->tagPagePercTageArea), &subModuAbsPos );
	status = status && status1;
	status1 = showTagBlock( &(scrPageInfoInst->tagPagePercTage), &subModuAbsPos );
	status = status && status1;

	//显示页数与总页数（格式为：" 1234/99999", 1234为当前页数，99999为总页数）
	status1 = getAbsPos( absPos, &(scrPageInfoInst->tagPageNumArea), &subModuAbsPos );
	status = status && status1;
	status1 = showTagBlock( &(scrPageInfoInst->tagPageNum), &subModuAbsPos );
	status = status && status1;

	return status;
}

//显示ProgressBarX
bool showProgressBarX( const ProgressBarX *prgBarXInst, const AreaRange *absPos )
{
	bool status = true, status1;

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
	status1 = showBkgAndBorder( &(prgBarXInst->bkgBorderColor), absPos, &(prgBarXInst->axisArea), 1 );
	status = status && status1;

	//子模块显示

	//子模块绝对位置
	AreaRange subModuAbsPos;

	//显示运动轴
	status1 = getAbsPos( absPos, &(prgBarXInst->axisArea), &subModuAbsPos );
	status = status && status1;
	status1 = showIconBlock( &(prgBarXInst->axis), &subModuAbsPos );
	status = status && status1;

	//显示进度点
	status1 = getAbsPos( absPos, &dotRealArea, &subModuAbsPos );
	status = status && status1;
	status1 = showIconBlock( &(prgBarXInst->dot), &subModuAbsPos );
	status = status && status1;

	return status;
}

//显示颜色板
bool showColorBoard( const ColorBoard *colorTableInst, const AreaRange *absPos )
{
	bool status = true, status1;

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
	short colorXoffset = 1;
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

		colorXoffset = 1;

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
	short borderMargin = 1;//边界距选中块的距离

	if ( colorTableInst->colorIndex != NULL )
	{
		AreaRange colorBorderPos;//相对位置
		short colorIndex = *(colorTableInst->colorIndex);
//		color_u16 borderColor = colorSpace[colorIndex];
		color_u16 borderColor = 0x0000;
		short colorYIndex = ( colorIndex + colorXNum ) / colorXNum;//下标从1开始
		short colorXIndex = colorIndex - (colorYIndex-1) * colorXNum + 1;//下标从1开始
		//首先计算选中颜色块的外边界
		colorBorderPos.x_min = ( colorXIndex - 1 ) * colorXSize + 1 - borderMargin;
		colorBorderPos.x_max = colorXIndex * colorXSize + borderMargin;
		colorBorderPos.y_min = ( colorYIndex - 1 ) * colorYSize + 1 - borderMargin;
		colorBorderPos.y_max = colorYIndex * colorYSize + borderMargin;

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
//			printf( "colorBorderPos = \n" );
//			displayAreaRange( &colorBorderPos, 4 );
//
//			printf( "absPos = \n" );
//			displayAreaRange( absPos, 4 );
			AreaRange colorBorderAbsPos;
			status1 = getAbsPos( absPos, &colorBorderPos, &colorBorderAbsPos );
//          printf( "colorBorderAbsPos = \n" );
//			displayAreaRange( &colorBorderAbsPos, 4 );
			status = status && status1;
			status1 = showBorder( &colorBorderAbsPos, borderColor );
			status = status && status1;
		}
	}

	return status;
}


//显示screen_header
bool showScreenHeader( const ScreenHeaderTime *scrHeaderInst, const AreaRange *scrHeaderAbsPos )
{
	bool status = true, status1;

	//显示screen_header的背景与边界
	status1 = showBkgAndBorder( &(scrHeaderInst->bkgBorderColor), scrHeaderAbsPos, &(scrHeaderInst->tagTimeArea), 1 );
	status = status && status1;
//	displayAreaRange( scrHeaderAbsPos, 4 );

	//显示时间标签
	AreaRange tagTimeAbsPos;//时间标签绝对位置
	//计算绝对位置
	status1 = getAbsPos( scrHeaderAbsPos, &(scrHeaderInst->tagTimeArea), &tagTimeAbsPos );
	status = status && status1;
	//显示Tag
	status1 = showTagBlock( &(scrHeaderInst->tagTime), &tagTimeAbsPos );
	status = status && status1;


	return status;
}
//显示screen_footer
bool showScreenFooter( const ScreenFooterBtn *scrFooterInst, const AreaRange *scrFooterAbsPos )
{
	bool status = true, status1;

	//显示screen_footer的背景与边界
	AreaRange AreaArray[3] = { scrFooterInst->iconSettingArea, scrFooterInst->iconHomeArea, scrFooterInst->iconBackArea };
	status1 = showBkgAndBorder( &(scrFooterInst->bkgBorderColor), scrFooterAbsPos, AreaArray, 3 );
	status = status && status1;

	//显示3个按键
	AreaRange iconAbsPos;//图标绝对位置


	//显示编辑键
	status1 = getAbsPos( scrFooterAbsPos, &(scrFooterInst->iconSettingArea), &iconAbsPos );
	status = status && status1;
	status1 = showIconBlock( &(scrFooterInst->iconSetting), &iconAbsPos );
	status = status && status1;

	//显示主页键
	status1 = getAbsPos( scrFooterAbsPos, &(scrFooterInst->iconHomeArea), &iconAbsPos );
	status = status && status1;
	status1 = showIconBlock( &(scrFooterInst->iconHome), &iconAbsPos );
	status = status && status1;

	//显示返回键
	status1 = getAbsPos( scrFooterAbsPos, &(scrFooterInst->iconBackArea), &iconAbsPos );
	status = status && status1;
	status1 = showIconBlock( &(scrFooterInst->iconBack), &iconAbsPos );
	status = status && status1;

	return status;
}
//显示screen_main_home
bool showScreenMainHome( const ScreenHome *scrHomeInst, const AreaRange *scrMainAbsPos )
{
	bool status = true, status1;

	//显示背景与边界
	AreaRange AreaArray[3] = { scrHomeInst->numBarArea, scrHomeInst->txtCatalogArea, scrHomeInst->turnCatalogBarArea };
	status1 = showBkgAndBorder( &(scrHomeInst->bkgBorderColor), scrMainAbsPos, AreaArray, 3 );
	status = status && status1;

	//显示子模块

	//子模块绝对位置
	AreaRange subModuAbsPos;

	//显示数字编号条（列表）
	status1 = getAbsPos( scrMainAbsPos, &(scrHomeInst->numBarArea), &subModuAbsPos );
	status = status && status1;
	status1 = showTagList( &(scrHomeInst->numBar), &subModuAbsPos, scrHomeInst->listYOffset );
	status = status && status1;
//printf( "lalaa\n" );
	//显示txt文本文件名列表
	status1 = getAbsPos( scrMainAbsPos, &(scrHomeInst->txtCatalogArea), &subModuAbsPos );
	status = status && status1;
	status1 = showTagList( &(scrHomeInst->txtCatalog), &subModuAbsPos, scrHomeInst->listYOffset );
	status = status && status1;
//printf( "lalab\n" );
	//显示上下翻滚动条
	status1 = getAbsPos( scrMainAbsPos, &(scrHomeInst->turnCatalogBarArea), &subModuAbsPos );
	status = status && status1;
	status1 = showScrollBarY( &(scrHomeInst->turnCatalogBarY), &subModuAbsPos );
	status = status && status1;

	return status;
}
//显示screen_main_book
bool showScreenMainBook( const ScreenBook *scrBookInst, const AreaRange *scrMainAbsPos )
{
	bool status = true, status1;

	//如果页面阅读进度信息可见
	if ( scrBookInst->pageInfoVisible )
	{
		//显示背景与边界
		AreaRange AreaArray[2] = { scrBookInst->txtBookArea, scrBookInst->pageInfoArea };
		status1 = showBkgAndBorder( &(scrBookInst->bookColorInfo), scrMainAbsPos, AreaArray, 2 );
		status = status && status1;

		//显示子模块

		//子模块绝对位置
		AreaRange subModuAbsPos;

		//显示电子书区域(因为下方被下面弹出的阅读进度信息覆盖，所以区域变窄
		status1 = getAbsPos( scrMainAbsPos, &(scrBookInst->txtBookNarrowArea), &subModuAbsPos );
		status = status && status1;
		status1 = showTagList( &(scrBookInst->txtBook), &subModuAbsPos, 0 );
		status = status && status1;

		//显示电子书阅读进度信息
		status1 = getAbsPos( scrMainAbsPos, &(scrBookInst->pageInfoArea), &subModuAbsPos );
		status = status && status1;
		status1 = showScreenPageInfo( &(scrBookInst->pageInfo), &subModuAbsPos );
		status = status && status1;
	}
	else
	{
		//显示背景与边界
		status1 = showBkgAndBorder( &(scrBookInst->bookColorInfo), scrMainAbsPos, &(scrBookInst->txtBookArea), 1 );
		status = status && status1;

		//显示电子书区域
		AreaRange subModuAbsPos;
		status1 = getAbsPos( scrMainAbsPos, &(scrBookInst->txtBookArea), &subModuAbsPos );
		status = status && status1;
		status1 = showTagList( &(scrBookInst->txtBook), &subModuAbsPos, 0 );
		status = status && status1;
	}

	return status;
}
//显示screen_main_setting
bool showScreenMainSetting( const ScreenSetting *scrSettingInst, const AreaRange *scrMainAbsPos )
{
	bool status = true, status1;

	//显示screen_footer的背景与边界
	AreaRange AreaArray[8] = { scrSettingInst->tag1Area, scrSettingInst->editAlarmHourArea, scrSettingInst->editAlarmMinuteArea,\
			scrSettingInst->editBkgColorArea, scrSettingInst->editWordColorArea, scrSettingInst->editTurnPageModArea,\
			scrSettingInst->tag2Area, scrSettingInst->editTurnPageSecArea };
	status1 = showBkgAndBorder( &(scrSettingInst->bkgBorderColor), scrMainAbsPos, AreaArray, 8 );
	status = status && status1;

	//显示子模块

	//子模块绝对位置
	AreaRange subModuAbsPos;

	//显示静态标签："休息定时"
	status1 = getAbsPos( scrMainAbsPos, &(scrSettingInst->tag1Area), &subModuAbsPos );
	status = status && status1;
	status1 = showTagBlock( &(scrSettingInst->tag1), &subModuAbsPos );
	status = status && status1;

	//显示可编辑的数字标签图标组：编辑休息定时：时
	status1 = getAbsPos( scrMainAbsPos, &(scrSettingInst->editAlarmHourArea), &subModuAbsPos );
	status = status && status1;
	status1 = showTagIconGroup( &(scrSettingInst->editAlarmHour), &subModuAbsPos );
	status = status && status1;

	//显示可编辑的数字标签图标组：编辑休息定时：分
	status1 = getAbsPos( scrMainAbsPos, &(scrSettingInst->editAlarmMinuteArea), &subModuAbsPos );
	status = status && status1;
	status1 = showTagIconGroup( &(scrSettingInst->editAlarmMinute), &subModuAbsPos );
	status = status && status1;

	//显示标签组：编辑背景颜色
//    printf( "subModuAbsPos = \n" );
//	displayAreaRange( &subModuAbsPos, 4 );
	status1 = getAbsPos( scrMainAbsPos, &(scrSettingInst->editBkgColorArea), &subModuAbsPos );
	status = status && status1;
//    printf( "subModuAbsPos = \n" );
//	displayAreaRange( &subModuAbsPos, 4 );
	status1 = showTagGroup( &(scrSettingInst->editBkgColor), &subModuAbsPos );
	status = status && status1;
//    printf( "subModuAbsPos = \n" );
//	displayAreaRange( &subModuAbsPos, 4 );

	//显示标签组：编辑文本颜色
	status1 = getAbsPos( scrMainAbsPos, &(scrSettingInst->editWordColorArea), &subModuAbsPos );
	status = status && status1;
	status1 = showTagGroup( &(scrSettingInst->editWordColor), &subModuAbsPos );
	status = status && status1;

	//显示标签组：自动翻页与手动翻页切换
	status1 = getAbsPos( scrMainAbsPos, &(scrSettingInst->editTurnPageModArea), &subModuAbsPos );
	status = status && status1;
	status1 = showTagGroup( &(scrSettingInst->editTurnPageMod), &subModuAbsPos );
	status = status && status1;

	//显示静态标签："翻页定时"
	status1 = getAbsPos( scrMainAbsPos, &(scrSettingInst->tag2Area), &subModuAbsPos );
	status = status && status1;
	status1 = showTagBlock( &(scrSettingInst->tag2), &subModuAbsPos );
	status = status && status1;

	//显示可编辑的数字标签图标组：编辑翻页定时：秒
	status1 = getAbsPos( scrMainAbsPos, &(scrSettingInst->editTurnPageSecArea), &subModuAbsPos );
	status = status && status1;
	status1 = showTagIconGroup( &(scrSettingInst->editTurnPageSec), &subModuAbsPos );
	status = status && status1;

	return status;
}
//显示screen_main_color_picker
bool showScreenMainColorPicker( const ScreenColorPicker *scrColorPickerInst, const AreaRange *scrMainAbsPos )
{
	bool status = true, status1;

	//画边界与背景
	//显示边界与背景
	status1 = showBkgAndBorder( &(scrColorPickerInst->bkgBorderColor), scrMainAbsPos, &(scrColorPickerInst->colorPickerArea), 1 );
	status = status && status1;

	//显示取色板
	AreaRange colorTableAbsPos;//色板绝对位置
	status1 = getAbsPos( scrMainAbsPos, &(scrColorPickerInst->colorPickerArea), &colorTableAbsPos );
	status = status && status1;
	status1 = showColorBoard( &(scrColorPickerInst->colorPicker), &colorTableAbsPos );
	status = status && status1;

	return status;
}


//显示screen
bool showScreen( )
{
	bool status = true, status1;

	const ScreenContainer *tftlcdScreenPtr = &tftlcdScreenInst;

	//显示screen_header
	status1 = showScreenHeader( &(tftlcdScreenPtr->scrHeader), &(tftlcdScreenPtr->headerArea) );
	status = status && status1;
	//显示screen_footer
	status1 = showScreenFooter( &(tftlcdScreenPtr->scrFooter), &(tftlcdScreenPtr->footerArea) );
	status = status && status1;

	//显示screen_main

//**********************测试模式时，设置TYPE为0;  正常运行模式时，设置TYPE为1************************************
#define SCREEN_MAIN_SHOW_TYPE 0

#if SCREEN_MAIN_SHOW_TYPE == 1
    short curScreenIndex = (tftlcdScreenPtr->mainAreaInfo).curScrIndex;
	switch( ((tftlcdScreenPtr->mainAreaInfo).scrIDStack)[curScreenIndex] )
#elif SCREEN_MAIN_SHOW_TYPE == 0
    switch( SCR_HOME )
#endif // SCREEN_MAIN_SHOW_TYPE
#undef SCREEN_MAIN_SHOW_TYPE
	{
		case SCR_HOME:
			status1 = showScreenMainHome( &(tftlcdScreenPtr->scrHome), &(tftlcdScreenPtr->mainArea) );
			break;
		case SCR_BOOK:
			status1 = showScreenMainBook( &(tftlcdScreenPtr->scrBook), &(tftlcdScreenPtr->mainArea) );
			break;
		case SCR_SETTING:
			status1 = showScreenMainSetting( &(tftlcdScreenPtr->scrSetting), &(tftlcdScreenPtr->mainArea) );
			break;
		case SCR_COLOR_PICKER:
			status1 = showScreenMainColorPicker( &(tftlcdScreenPtr->scrColorPicker), &(tftlcdScreenPtr->mainArea) );
			break;
		default:
			status = false;
	}

	status = status && status1;

	return status;
}

