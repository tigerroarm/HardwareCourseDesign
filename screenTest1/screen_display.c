/*
 * screen_display.c
 *
 *  Created on: 2019��5��1��
 *      Author: 12757
 */

#include <stdlib.h>
#include "screen_display.h"
#include "screen.h"
#include "test_value.h"

//������ǵ�ɾ��(����ע��)
//extern const alt_u8 gbk_font_x16y16[];

//������������ʱ������Ļ��ʾЧ�������Ĺ��̻�ɾ��������
color_u16 screenShowSpace[SCR_HEIGHT][SCR_WIDTH];

//��ȡ�µľ���λ��
bool getAbsPos( const AreaRange *baseAbsPos, const AreaRange *relativePos, AreaRange *newAbsPos )
{
    //��������ʽ�Ƿ�����width,height>= 0
    bool status1 = checkAreaRange( baseAbsPos );
    bool status2 = checkAreaRange( relativePos );

    bool status = status1 && status2;

    if ( status )//true
    {
        short absWidth = baseAbsPos->x_max - baseAbsPos->x_min + 1;
        short absHeight = baseAbsPos->y_max - baseAbsPos->y_min + 1;

        //������������н���
        if ( relativePos->x_min >= 1 && relativePos->x_min <= absWidth &&
             relativePos->y_min >= 1 && relativePos->y_min <= absHeight )
        {
            newAbsPos->x_min = baseAbsPos->x_min + relativePos->x_min - 1;
            newAbsPos->x_max = baseAbsPos->x_min + relativePos->x_max - 1;
            newAbsPos->y_min = baseAbsPos->y_min + relativePos->y_min - 1;
            newAbsPos->y_max = baseAbsPos->y_min + relativePos->y_max - 1;
            //ȡ����
            if ( newAbsPos->x_max > baseAbsPos->x_max )
            {
                newAbsPos->x_max = baseAbsPos->x_max;
            }
            if ( newAbsPos->y_max > baseAbsPos->y_max )
            {
                newAbsPos->y_max = baseAbsPos->y_max;
            }
        }
        else//û�н���������Ϊ�������䣨1,1,0,0��
        {
            newAbsPos->x_min = 1;
            newAbsPos->x_max = 0;
            newAbsPos->y_min = 1;
            newAbsPos->y_max = 0;
        }
    }
    else//false
    {
        //��Ϊ�������䣨1,1,0,0��
        newAbsPos->x_min = 1;
        newAbsPos->x_max = 0;
        newAbsPos->y_min = 1;
        newAbsPos->y_max = 0;
    }

	return status;
}

//��ȡx���������λ����(���������С��
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
				exceptXposInRow[xPos-1] = 'e';//�ԷǱ��������������
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
		if ( curRowXType == 'b' )//��ǰΪ������
		{
			if ( preRowXType == 'e' )//ǰ��Ϊ�Ǳ�����
			{
				xMinArray[rangeIndex] = xPos;
			}
		}
		else//��ǰΪ�Ǳ�����
		{
			if ( preRowXType == 'b' )//ǰ��Ϊ������
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

//���߽�(���ǻ��������εı�)
bool showBorder( const AreaRange *borderAbsPos, color_u16 borderColor )
{
	short absPosWidth = borderAbsPos->x_max - borderAbsPos->x_min + 1;
	short absPosHeight = borderAbsPos->y_max - borderAbsPos->y_min + 1;


	if ( absPosWidth < 0 || absPosHeight < 0 )
    {
        return false;
    }
    else if ( absPosWidth == 0 || absPosHeight == 0 )//�����򣬸��ޱ߽磬ֱ�ӷ���
    {
        return true;
    }
    else if ( absPosWidth == 1 || absPosHeight == 1 )//ֻ�ܻ�һ���߽�
    {
        int xPos, yPos;
        if ( absPosHeight == 1 )//ֻ�к�����
        {
            //����һ���߽���
            yPos = borderAbsPos->y_min;
            for ( xPos = borderAbsPos->x_min; xPos <= borderAbsPos->x_max; xPos ++ )
            {
                screenShowSpace[yPos-1][xPos-1] = borderColor;
            }
        }
        else
        {
            //����һ���߽���
            xPos = borderAbsPos->x_min;
            for ( yPos = borderAbsPos->y_min; yPos <= borderAbsPos->y_max; yPos ++ )
            {
                screenShowSpace[yPos-1][xPos-1] = borderColor;
            }
        }

    }
    else//һ�����
    {
        int xPos, yPos;
        //����(x����)�ı߽���
        yPos = borderAbsPos->y_min;//�ϱ߽���
        for ( xPos = borderAbsPos->x_min; xPos <= borderAbsPos->x_max; xPos ++ )
        {
            screenShowSpace[yPos-1][xPos-1] = borderColor;
        }
        yPos = borderAbsPos->y_max;//�±߽���
        for ( xPos = borderAbsPos->x_min; xPos <= borderAbsPos->x_max; xPos ++ )
        {
            screenShowSpace[yPos-1][xPos-1] = borderColor;
        }

        //����(y����)�ı߽���
        xPos = borderAbsPos->x_min;//��߽���
        for ( yPos = borderAbsPos->y_min + 1; yPos < borderAbsPos->y_max; yPos ++ )
        {
            screenShowSpace[yPos-1][xPos-1] = borderColor;
        }
        xPos = borderAbsPos->x_max;//�ұ߽���
        for ( yPos = borderAbsPos->y_min + 1; yPos < borderAbsPos->y_max; yPos ++ )
        {
            screenShowSpace[yPos-1][xPos-1] = borderColor;
        }

    }

	return true;
}

//��ʾ�߽��뱳��
//exceptAreaArrayΪ�Ǳ��������������飨�����ʽΪ�������
//bkgBorderColorΪ��ɫ��Ϣ��������ɫ���߽���ɫ���߽���ɫ�Ƿ�ɼ�(������ɼ�����һ��ʹ�ñ�����ɫ
//exceptAreaArray[]�ǷǱ������򹹳ɵ����飬 exceptAreaNumΪ����Ԫ�ظ���
//exceptAreaArray[]��ʽΪ�������λ��
//absPosΪ����λ��
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
//    if ( absPos->y_max == 329 )
//    {
//        printf( "borderVisible=%d\n", bkgBorderColor->borderVisible );
//    }
	if ( bkgBorderColor->borderVisible )//�߽�ɼ�
	{

		//���߽�
		status1 = showBorder( absPos, bkgBorderColor->borderColor );
		status = status && status1;
		//������������СһȦ������ȥ�ջ��ı߽�
		width -= 2;
		height -= 2;
		xBasePos += 1;
		yBasePos += 1;
		//����exceptAreaArray[]�����������λ��
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

	//��ʾ������ɫ
	//�˴�xPos,yPosΪ���λ��
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
		areaOccurInfo[i] = 0;//���
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
		//ͳ�Ƹ��г��ֵķǱ�����
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
				areaOccurInfo[i] = 'n';//�Ǳ�����
			}
			else
			{
				if ( areaOccurInfo[i] != 'e' )
				{
					changeOccur = true;
				}
				//�ԷǱ��������������
				areaOccurInfo[i] = 'e';
			}
		}
//		if ( exceptAreaNum == 0 )
//        {
//            printf( "changeOccur=%d\n", changeOccur );
//        }

		//����Ǳ����������仯�����޸ķǱ�����x��λ����
        if ( changeOccur )
        {
            xRangeNum = getXVaildRange( areaOccurInfo, exceptAreaArrayCopy, exceptAreaNum, xMinArray, xMaxArray, width );

        }
//        printf( "xRangeNum=%d\n", xRangeNum );
        //������
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
//	    printf( "yPos = %d\n", yPos );
		short objPicIndex = objPicRowBaseIndex;
		for ( xPos = 1; xPos <= width; xPos ++ )
		{
			if ( (xPos & 0x7) == 0x1 )//xPos����λΪ001����ЧΪxPos % 8 == 1 )
			{
//				printf( "xixi1\n" );
//				printf( "objPicPtr = %p\n", objPicPtr );
				objPicRowPixel = ( objPicPtr != NULL ) ? (objPicPtr[objPicIndex]) : 0x00;
				objPicIndex ++;
//				printf( "xixi1_1\n" );
			}
			if ( (objPicRowPixel & 0x80) == 0x80 )//���λΪ1
			{
//				printf( "xixi2\n" );
				screenShowSpace[yBasePos+yPos-1][xBasePos+xPos-1] = objColor;
			}
			else
			{
//				printf( "xixi3\n" );
				screenShowSpace[yBasePos+yPos-1][xBasePos+xPos-1] = bkgColor;
			}
			objPicRowPixel = objPicRowPixel<<1;//����һλ������������Ϣ�������λ
		}
		objPicRowBaseIndex = objPicRowBaseIndex + objXByte;
	}

	return status;
}


//��ʾTagBlock
bool showTagBlock( const TagBlock *tagPtr, const AreaRange *absPos )
{
	bool status = true, status1;

	//��ʾTagBlock�ı�����߽�
	bool textVisible = (tagPtr->tagColorInfo).objVisible;

	if ( textVisible )//�м���ı���Ҫ��ʾ
	{
		//����������߽磨�ڵ�text��ʾ����
		status1 = showBkgAndBorder( &(tagPtr->tagColorInfo), absPos, &(tagPtr->textArea), 1 );
		status = status && status1;

		//��ʾ�м��text

		AreaRange textAbsPos;//text�ľ���λ��
		//��ȡtext�ľ���λ��
		status1 = getAbsPos( absPos, &(tagPtr->textArea), &textAbsPos );
		status = status && status1;

		AreaRange objAbsPos;//�ַ��ľ���λ��

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

		//��ȡ�ı��к�������ĸ�ķֲ���(����"c",Ӣ��"e",������־0)(���ض�̬�ַ������ǵ�������Ҫfree��)
		char *strType = getWordLetterArrange( (tagPtr->tagText).text, (tagPtr->tagText).textLen );
		//��ʾ��һ���ַ�
		if ( strType != NULL && strType[0] != 0 )
		{
			if( objAbsPos.x_min <= textAbsPos.x_max )//��ǰ�ֽ�λ�ô�����ʾ��Χ��
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
                    //��ʾ���
    //				printf( "i= %d\n", i );
    //				printf( "strType[%d] = %c\n", i, strType[i] );
                    objAbsPos.x_min = objAbsPos.x_max + 1;
                    if( objAbsPos.x_min > textAbsPos.x_max )//�����ı���ʾ�ұ߽�
                    {
    //					printf( "textAbsPos.x_max = %d\n", textAbsPos.x_max );
    //					printf( "objAbsPos.x_min = %d\n", objAbsPos.x_min );
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

                    if ( objAbsPos.x_max > textAbsPos.x_max )//��ֹˮƽ�����߽�
                    {
                        objAbsPos.x_max = textAbsPos.x_max;
                    }

                    status1 = showTextOrIcon( &objAbsPos, NULL, bkgColor, objColor, objXSize, objYSize );
                    status = status && status1;

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
    //					printf( "text[textIndex] = %x\n", (alt_u8)(text[textIndex]) );
    //					printf( "text[textIndex+1] = %x\n", (alt_u8)(text[textIndex+1]) );
    //					char tempStr[3] = { text[textIndex], text[textIndex+1], 0 };
    //					printf( "char = %s\n", tempStr );
                        textIndex += 2;
                    }
                    objAbsPos.x_max = objAbsPos.x_min - 1 + objXSize;
                    if ( objAbsPos.x_max > textAbsPos.x_max )//��ֹˮƽ�����߽�
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
        //����ı����ұ߽绹�������ռ䣬����䱳����ɫ
        objAbsPos.x_min = objAbsPos.x_max + 1;
        if ( objAbsPos.x_min <= textAbsPos.x_max )
        {
            objAbsPos.x_max = textAbsPos.x_max;
            status1 = showTextOrIcon( &objAbsPos, NULL, bkgColor, objColor, objAbsPos.x_max - objAbsPos.x_min + 1, objYSize );
            status = status && status1;
        }
        //����ı����·��������ռ䣬����䱳����ɫ
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
	else//�м��icon����Ҫ��ʾ
	{
		//ֻ����������߽���ɫ
		status1 = showBkgAndBorder( &(tagPtr->tagColorInfo), absPos, NULL, 0 );
		status = status && status1;
	}

	return status;
}

//��ʾTagBlock(�����������ƫ��)
bool showTagWithYOffset( const TagBlock *tagPtr, const AreaRange *absPos, const short yOffset )
{
	bool status = true, status1;

	//��ʾTagBlock�ı�����߽�
	bool textVisible = (tagPtr->tagColorInfo).objVisible;

//    displayTagBlock( tagPtr, 0 );
//    displayAreaRange( absPos, 0 );

	short wordPicPtrOffset = 0;
	short letterPicPtrOffset = 0;
	short diffY = 0;//�ı��������������ĸ߶�

//	printf( "yOffset = %d\n", yOffset );
//
//	printf( "tagPtr->textArea = \n" );
//	displayAreaRange( &(tagPtr->textArea), 4 );

	if ( yOffset+1 >= (tagPtr->textArea).y_min )
	{
		diffY = yOffset - (tagPtr->textArea).y_min + 1;
		short objXByte = ((tagPtr->wordXSize)+7)>>3;//��8������ȡ��
		wordPicPtrOffset = diffY * objXByte;
		objXByte = ((tagPtr->letterXSize)+7)>>3;//��8������ȡ��
		letterPicPtrOffset = diffY * objXByte;
	}

	//absPos��yOffset�����������
	AreaRange absNewPos = *absPos;
	absNewPos.y_min += yOffset;

	if ( textVisible )//�м���ı���Ҫ��ʾ
	{
		//����������߽磨�ڵ�text��ʾ����
		AreaRange exceptArea;
		exceptArea = tagPtr->textArea;
		exceptArea.y_max -= yOffset;
		exceptArea.y_min -= yOffset;
		status1 = showBkgAndBorder( &(tagPtr->tagColorInfo), &absNewPos, &exceptArea, 1 );
		status = status && status1;


		//��ʾ�м��text

		AreaRange textAbsPos;//text�ľ���λ��
		//��ȡtext�ľ���λ��
		status1 = getAbsPos( absPos, &(tagPtr->textArea), &textAbsPos );
		status = status && status1;

		AreaRange objAbsPos;//�ַ��ľ���λ��

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

		//��ȡ�ı��к�������ĸ�ķֲ���(����"c",Ӣ��"e",������־0)(���ض�̬�ַ������ǵ�������Ҫfree��)
		char *strType = getWordLetterArrange( text, textLen );

		//������ı���Ҫ��ʾ
		if ( strType != NULL || strType[0] != 0 )
		{
            if( objAbsPos.x_min <= textAbsPos.x_max )//��ǰ�ֽ�λ�ô�����ʾ��Χ��
            {
                short objXSize;

                //��ʾ��һ���ַ�
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
                if ( objAbsPos.x_max > textAbsPos.x_max )//��ֹˮƽ�����߽�
                {
                    objAbsPos.x_max = textAbsPos.x_max;
                }
                status1 = showTextOrIcon( &objAbsPos, objPicPtr, bkgColor, objColor, objXSize, objYSize );
                status = status && status1;

                int i;
                //��ʾ֮����ַ�
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
                    if ( objAbsPos.x_max > textAbsPos.x_max )//��ֹˮƽ�����߽�
                    {
                        objAbsPos.x_max = textAbsPos.x_max;
                    }

                    status1 = showTextOrIcon( &objAbsPos, NULL, bkgColor, objColor, objXSize, objYSize );
                    status = status && status1;

                    //��ʾ�ı�
                    objAbsPos.x_min = objAbsPos.x_max + 1;
                    if( objAbsPos.x_min > textAbsPos.x_max )//�����ı���ʾ�ұ߽�
                    {
                        break;
                    }
                    if( strType[i] == 'e' )//��ǰ�ַ�ΪӢ����ĸ
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
                    if ( objAbsPos.x_max > textAbsPos.x_max )//��ֹˮƽ�����߽�
                    {
                        objAbsPos.x_max = textAbsPos.x_max;
                    }

                    status1 = showTextOrIcon( &objAbsPos, objPicPtr, bkgColor, objColor, objXSize, objYSize );
                    status = status && status1;
                }
            }
		}
        //����ı����ұ߽绹�������ռ䣬����䱳����ɫ
        objAbsPos.x_min = objAbsPos.x_max + 1;
        if ( objAbsPos.x_min <= textAbsPos.x_max )
        {
            objAbsPos.x_max = textAbsPos.x_max;
            status1 = showTextOrIcon( &objAbsPos, NULL, bkgColor, objColor, objAbsPos.x_max - objAbsPos.x_min + 1, objYSize );
            status = status && status1;
        }
        //����ı����·��������ռ䣬����䱳����ɫ
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
	else//�м��icon����Ҫ��ʾ
	{
		//ֻ����������߽���ɫ
		status1 = showBkgAndBorder( &(tagPtr->tagColorInfo), &absNewPos, NULL, 0 );
		status = status && status1;
		//�ñ�����ɫ���ǵ��ϱ߽���ɫ
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

//��ʾIconBlock
bool showIconBlock( const IconBlock *iconPtr, const AreaRange *absPos )
{
	bool status = true, status1;

	//��ʾIconBlock�ı�����߽�
	bool iconPicVisible = (iconPtr->iconColorInfo).objVisible;

	if ( iconPicVisible )//�м��icon��Ҫ��ʾ
	{
		//����������߽磨�ڵ�iconPic��ʾ����
		status1 = showBkgAndBorder( &(iconPtr->iconColorInfo), absPos, &(iconPtr->picArea), 1 );
		status = status && status1;

		//��ʾ�м��iconͼƬ

		AreaRange iconPicAbsPos;//iconPic�ľ���λ��
		//��ȡiconPic�ľ���λ��
		status1 = getAbsPos( absPos, &(iconPtr->picArea), &iconPicAbsPos );
		status = status && status1;
		//��ʾicon
		status1 = showTextOrIcon( &iconPicAbsPos, iconPtr->iconPic, (iconPtr->iconColorInfo).bkgColor, (iconPtr->iconColorInfo).objColor,\
				iconPtr->iconXSize, iconPtr->iconYSize );
        status = status && status1;
	}
	else//�м��icon����Ҫ��ʾ
	{
		//ֻ����������߽���ɫ
		status1 = showBkgAndBorder( &(iconPtr->iconColorInfo), absPos, NULL, 0 );
		status = status && status1;
	}

	return status;
}
//��ʾTagGroup
bool showTagGroup( const TagGroup *tagGroupInst, const AreaRange *absPos )
{
	bool status = true, status1;

//    printf( "absPos = \n" );
//	displayAreaRange( absPos, 4 );

	//��ʾTagGroup�ı�����߽�
	AreaRange AreaArray[2] = { tagGroupInst->staticTagArea, tagGroupInst->varTagArea };
	status1 = showBkgAndBorder( &(tagGroupInst->bkgBorderColor), absPos, AreaArray, 2 );
	status = status && status1;

	//��ʾ2����ǩ

	//��ǩ����λ��
	AreaRange tagAbsPos;
//printf( "tagStatic\n" );
	//��ʾ��̬��ǩ
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
	//��ʾ��̬��ǩ
	status1 = getAbsPos( absPos, &(tagGroupInst->varTagArea), &tagAbsPos );
	status = status && status1;
	status1 = showTagBlock( &(tagGroupInst->varTag), &tagAbsPos );
	status = status && status1;

	return status;
}

//��ʾTagIconGroup
bool showTagIconGroup( const TagIconGroup *tagIconGpInst, const AreaRange *absPos )
{
	bool status = true, status1;

	//��ʾTagIconGroup�ı�����߽�
	AreaRange AreaArray[4] = { tagIconGpInst->plusIconArea, tagIconGpInst->valueTagArea,\
					tagIconGpInst->unitTagArea,tagIconGpInst->minusIconArea };
	status1 = showBkgAndBorder( &(tagIconGpInst->bkgBorderColor), absPos, AreaArray, 4 );
	status = status && status1;

	//��ʾ��ģ��

	//��ģ�����λ��
	AreaRange subModuAbsPos;

	//��ʾ���ּӼ�
	status1 = getAbsPos( absPos, &(tagIconGpInst->plusIconArea), &subModuAbsPos );
	status = status && status1;
	status1 = showIconBlock( &(tagIconGpInst->plusIcon), &subModuAbsPos );
	status = status && status1;

	//��ʾ���ּ���
	status1 = getAbsPos( absPos, &(tagIconGpInst->minusIconArea), &subModuAbsPos );
	status = status && status1;
	status1 = showIconBlock( &(tagIconGpInst->minusIcon), &subModuAbsPos );
	status = status && status1;

	//������ʾ��ǩ
	status1 = getAbsPos( absPos, &(tagIconGpInst->valueTagArea), &subModuAbsPos );
	status = status && status1;
	status1 = showTagBlock( &(tagIconGpInst->valueTag), &subModuAbsPos );
	status = status && status1;

	//���ֵ�λ��ǩ
	status1 = getAbsPos( absPos, &(tagIconGpInst->unitTagArea), &subModuAbsPos );
	status = status && status1;
	status1 = showTagBlock( &(tagIconGpInst->unitTag), &subModuAbsPos );
	status = status && status1;

	return status;
}

//��ʾTagList
bool showTagList( const TagList *tagListInst, const AreaRange *absPos, const short tagListYOffset )
{
	bool status = true, status1;

//	printf( "tagListInst = \n" );
//	displayTagList( tagListInst, 4 );
//	printf( "absPos = \n" );
//	displayAreaRange( absPos, 4 );
//	printf( "tagListYOffset = %d\n", tagListYOffset );

	//�б���ʾ�ռ�absPos�ĸ߶�
	short listShowHeight = absPos->y_max - absPos->y_min + 1;
	//�б���ʾ�ռ�absPos���б�TagListInst�е�����λ��
	short listShowYMin = tagListYOffset + 1;
	short listShowYMax = tagListYOffset + listShowHeight;

	short elemHeight = (tagListInst->elemBaseArea).y_max;

	short listShowIndexMin = (listShowYMin-1) / elemHeight;//�б�����Ҫ��ʾtag����С�����±꣨��0��ʼ��
	short listShowIndexMax = (listShowYMax-1) / elemHeight;//�б�����Ҫ��ʾtag����������±꣨��0��ʼ��

	short elemNum = tagListInst->elemNum;//�б�Ԫ���ܸ���


	if ( listShowIndexMax >= elemNum )
	{
		listShowIndexMax = elemNum - 1;
	}
	//��ʾTagList�б���ÿһ��������ʾ���е�TagBlock
	short elemCurIndex;
	AreaRange curElemAbsPos;
	//����ƫ����tagListYOffsetΪ0ʱ����ģ��ľ���λ��
	status1 = getAbsPos( absPos, &(tagListInst->elemBaseArea), &curElemAbsPos );
	status = status && status1;
	//������С�±�ģ��ľ���λ�ã�����ƫ����tagListYOffset����С�±�ģ����±�elemCurIndex
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
        bool tagListBottomNarrowOccur = false;//�Ƿ�����б��ײ��߶ȱ�����������

        alt_u16 borderColor;

        //��ʾ��һ��TagBlock
        short yOffset = absPos->y_min - curElemAbsPos.y_min;
        elemCurIndex = listShowIndexMin;
        if ( elemCurIndex >= 0 && elemCurIndex <= listShowIndexMax )
        {
            status1 = showTagWithYOffset( &(tagBlockArray[elemCurIndex]), &curElemAbsPos, yOffset );
            status = status && status1;
            //�����ƶ�λ��
            curElemAbsPos.y_min += elemHeight;
            curElemAbsPos.y_max += elemHeight;
            borderColor = tagBlockArray[listShowIndexMax].tagColorInfo.bkgColor;
            //�ñ�����ɫ���ǵ��ϱ߽���ɫ
            if ( tagBlockArray[elemCurIndex].tagColorInfo.borderVisible && yOffset > 0 && yOffset < elemHeight - 1 )
            {
                AreaRange moveTopBorderArea;
                moveTopBorderArea.y_min = absPos->y_min;
                moveTopBorderArea.y_max = absPos->y_min;

                //�ϱ߽�����
                moveTopBorderArea.x_min = absPos->x_min+1;
                moveTopBorderArea.x_max = absPos->x_min+(tagListInst->elemTextArea).x_min-2;
                showBorder( &moveTopBorderArea, borderColor );

                //�ϱ߽��Ұ��
                moveTopBorderArea.x_min = absPos->x_min+tagListInst->elemTextArea.x_max;
                moveTopBorderArea.x_max = absPos->x_max-1;
                showBorder( &moveTopBorderArea, borderColor );
            }
        }




    //printf( "listShowIndexMax = %d\n", listShowIndexMax );
        //��ʾ֮���TagBlock
        for ( elemCurIndex = listShowIndexMin + 1; elemCurIndex <= listShowIndexMax; elemCurIndex ++ )
        {
    //		printf( "elemCurIndex = %d\n", elemCurIndex );
            //��ʾ������TagBlock
            status1 = showTagBlock( &(tagBlockArray[elemCurIndex]), &curElemAbsPos );
    //		printf( "elemCurIndex = %d ,  curElemAbsPos = \n", elemCurIndex );
    //        displayAreaRange( &curElemAbsPos, 4 );
            status = status && status1;
            //�����ƶ�λ��
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
        //��ʾ��������ֱ���ñ���curElemAbsPos��
        if ( curElemAbsPos.y_min <= absPos->y_max )
        {
            curElemAbsPos.y_max = absPos->y_max;
            status1 = showBkgAndBorder( &(tagListInst->elemColorInfo), &curElemAbsPos, NULL, 0 );
            status = status && status1;
        }
        //����б��ײ��߶ȱ�����,�����б��ײ���TagBlock�߽�
        if ( tagListBottomNarrowOccur )
        {
            AreaRange moveBottomBorderArea;//�����ײ��ı߽�

            moveBottomBorderArea.y_max = absPos->y_max;
            moveBottomBorderArea.y_min = absPos->y_max;

            //�±߽�����
            moveBottomBorderArea.x_min = absPos->x_min+1;
            moveBottomBorderArea.x_max = absPos->x_min+(tagListInst->elemTextArea).x_min-2;
            showBorder( &moveBottomBorderArea, borderColor );

            //�±߽��Ұ��
            moveBottomBorderArea.x_min = absPos->x_min+tagListInst->elemTextArea.x_max;
            moveBottomBorderArea.x_max = absPos->x_max-1;
            showBorder( &moveBottomBorderArea, borderColor );

            status = status && status1;
        }
    }
    else
    {
        //ֱ����ʾ��������ֱ���ñ���curElemAbsPos��
        curElemAbsPos.y_min = absPos->y_min;
        curElemAbsPos.y_max = absPos->y_max;
        status1 = showBkgAndBorder( &(tagListInst->elemColorInfo), &curElemAbsPos, NULL, 0 );
        status = status && status1;
    }
	return status;
}
//��ʾScrollBarY
bool showScrollBarY( const ScrollBarY *scrollBarYInst, const AreaRange *absPos )
{
	bool status = true, status1;


	//��ģ�����λ����ʱ�洢����
	AreaRange subModuAbsPos;


	//����������ɼ�
	if ( scrollBarYInst->iconBarVisible )
	{

		//����������ƫ����
		short barYoffset = scrollBarYInst->barYoffset;

		//���������ƫ�ƺ��λ��
		const AreaRange *barBaseAreaPtr = &(scrollBarYInst->barBaseArea);
		AreaRange barRealArea;

		barRealArea.x_min = barBaseAreaPtr->x_min;
		barRealArea.x_max = barBaseAreaPtr->x_max;
		barRealArea.y_min = barBaseAreaPtr->y_min + barYoffset;
		barRealArea.y_max = barBaseAreaPtr->y_max + barYoffset;


		//��ʾScrollBarY�ı�����߽�
		AreaRange AreaArray[3] = { scrollBarYInst->iconMoveUpArea, barRealArea, scrollBarYInst->iconMoveDownArea };
		status1 = showBkgAndBorder( &(scrollBarYInst->barYColorInfo), absPos, AreaArray, 3 );
		status = status && status1;


		//��ʾ������
		status1 = getAbsPos( absPos, &barRealArea, &subModuAbsPos );
		status = status && status1;
		status1 = showIconBlock( &(scrollBarYInst->iconBar), &subModuAbsPos );
		status = status && status1;
	}
	else
	{
		//��ʾScrollBarY�ı�����߽�
		AreaRange AreaArray[2] = { scrollBarYInst->iconMoveUpArea, scrollBarYInst->iconMoveDownArea };
		status1 = showBkgAndBorder( &(scrollBarYInst->barYColorInfo), absPos, AreaArray, 2 );
		status = status && status1;
	}

	//��ʾ���Ƽ�
	status1 = getAbsPos( absPos, &(scrollBarYInst->iconMoveUpArea), &subModuAbsPos );
	status = status && status1;
	status1 = showIconBlock( &(scrollBarYInst->iconMoveUp), &subModuAbsPos );
	status = status && status1;

	//��ʾ���Ƽ�
	status1 = getAbsPos( absPos, &(scrollBarYInst->iconMoveDownArea), &subModuAbsPos );
	status = status && status1;
	status1 = showIconBlock( &(scrollBarYInst->iconMoveDown), &subModuAbsPos );
	status = status && status1;


	return status;
}

//��ʾScreenPageInfo
bool showScreenPageInfo( const ScreenPageInfo *scrPageInfoInst, const AreaRange *absPos )
{
	bool status = true, status1;

	//��ʾScreenPageInfo�ı�����߽�
	AreaRange AreaArray[3] = { scrPageInfoInst->prgBarArea, scrPageInfoInst->tagPagePercTageArea,\
					scrPageInfoInst->tagPageNumArea };
	status1 = showBkgAndBorder( &(scrPageInfoInst->bkgBorderColor), absPos, AreaArray, 3 );
	status = status && status1;

	//��ģ����ʾ

	//��ģ�����λ��
	AreaRange subModuAbsPos;

	//��ʾˮƽ������
	status1 = getAbsPos( absPos, &(scrPageInfoInst->prgBarArea), &subModuAbsPos );
	status = status && status1;
	status1 = showProgressBarX( &(scrPageInfoInst->prgBarX), &subModuAbsPos );
	status = status && status1;

	//��ʾ�ٷ�������
	status1 = getAbsPos( absPos, &(scrPageInfoInst->tagPagePercTageArea), &subModuAbsPos );
	status = status && status1;
	status1 = showTagBlock( &(scrPageInfoInst->tagPagePercTage), &subModuAbsPos );
	status = status && status1;

	//��ʾҳ������ҳ������ʽΪ��" 1234/99999", 1234Ϊ��ǰҳ����99999Ϊ��ҳ����
	status1 = getAbsPos( absPos, &(scrPageInfoInst->tagPageNumArea), &subModuAbsPos );
	status = status && status1;
	status1 = showTagBlock( &(scrPageInfoInst->tagPageNum), &subModuAbsPos );
	status = status && status1;

	return status;
}

//��ʾProgressBarX
bool showProgressBarX( const ProgressBarX *prgBarXInst, const AreaRange *absPos )
{
	bool status = true, status1;

	//���ȵ��������ƫ����Ϊ0��
	const AreaRange *dotBaseAreaPtr = &(prgBarXInst->dotBaseArea);
	//���ȵ����ƫ����
	short dotXoffset = prgBarXInst->xOffset;

	//���������ƫ�ƺ��λ��
	AreaRange dotRealArea;

	dotRealArea.x_min = dotBaseAreaPtr->x_min + dotXoffset;
	dotRealArea.x_max = dotBaseAreaPtr->x_max + dotXoffset;
	dotRealArea.y_min = dotBaseAreaPtr->y_min;
	dotRealArea.y_max = dotBaseAreaPtr->y_max;


	//��ʾProgressBarX�ı�����߽磨���ڵ�ǳ�С�����԰ѵ����������ȥ���ˣ�
//	AreaRange AreaArray[2] = { prgBarXInst->axisArea, dotRealArea };
//	status = status && showBkgAndBorder( &(prgBarXInst->bkgBorderColor), absPos, AreaArray, 2 );
	status1 = showBkgAndBorder( &(prgBarXInst->bkgBorderColor), absPos, &(prgBarXInst->axisArea), 1 );
	status = status && status1;

	//��ģ����ʾ

	//��ģ�����λ��
	AreaRange subModuAbsPos;

	//��ʾ�˶���
	status1 = getAbsPos( absPos, &(prgBarXInst->axisArea), &subModuAbsPos );
	status = status && status1;
	status1 = showIconBlock( &(prgBarXInst->axis), &subModuAbsPos );
	status = status && status1;

	//��ʾ���ȵ�
	status1 = getAbsPos( absPos, &dotRealArea, &subModuAbsPos );
	status = status && status1;
	status1 = showIconBlock( &(prgBarXInst->dot), &subModuAbsPos );
	status = status && status1;

	return status;
}

//��ʾ��ɫ��
bool showColorBoard( const ColorBoard *colorTableInst, const AreaRange *absPos )
{
	bool status = true, status1;

	//ÿ��ɫ��
	const short colorXSize = colorTableInst->colorXSize;//���򳤶�
	const short colorYSize = colorTableInst->colorYSize;//���򳤶�
	//ɫ�����
	const short colorXNum = colorTableInst->colorXNum;//������ɫ����
	const short colorYNum = colorTableInst->colorYNum;//������ɫ����
	const short colorSNum = colorXNum * colorYNum;//��ɫ�ܸ���
	//��ɫӳ���
	const color_u16 *const colorSpace = colorTableInst->colorSpace;

	//����ߴ�
	short xBasePos = absPos->x_min - 1;
	short yBasePos = absPos->y_min - 1;
	short width = absPos->x_max - xBasePos;
	short height = absPos->y_max - yBasePos;

	short curColorBaseIndex= 0;
	short curColorIndex = 0;

	//xPos,yPosΪ���λ��
	short xPos, yPos;
	short colorYoffset = 0;
	short colorXoffset = 1;
	//��ʾɫ��
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


	//ͻ��ѡ����ɫ�ı߽�
    status1 = showColorSelBorder( colorTableInst, absPos );
    status = status && status1;

	return status;
}
//��ʾ��ɫ����ѡ����ɫ�ı߽�
bool showColorSelBorder( const ColorBoard *colorTableInst, const AreaRange *absPos )
{
    bool status = true, status1;

    //ÿ��ɫ��
	const short colorXSize = colorTableInst->colorXSize;//���򳤶�
	const short colorYSize = colorTableInst->colorYSize;//���򳤶�
	//ɫ�����
	const short colorXNum = colorTableInst->colorXNum;//������ɫ����
//	const short colorYNum = colorTableInst->colorYNum;//������ɫ����
//	const short colorSNum = colorXNum * colorYNum;//��ɫ�ܸ���
	//��ɫӳ���
	const color_u16 *const colorSpace = colorTableInst->colorSpace;

	//����ߴ�
	short xBasePos = absPos->x_min - 1;
	short yBasePos = absPos->y_min - 1;
	short width = absPos->x_max - xBasePos;
	short height = absPos->y_max - yBasePos;

    //ͻ��ѡ����ɫ�ı߽�
	short borderMargin = 1;//�߽��ѡ�п�ľ���

	if ( colorTableInst->colorIndex != NULL )
	{
		AreaRange colorBorderPos;//���λ��
		short colorIndex = *(colorTableInst->colorIndex);
//		color_u16 borderColor = colorSpace[colorIndex];
//		color_u16 borderColor = 0x0000;
        color_u16 borderColor = 0xffff - colorSpace[colorIndex];
		short colorYIndex = ( colorIndex + colorXNum ) / colorXNum;//�±��1��ʼ
		short colorXIndex = colorIndex - (colorYIndex-1) * colorXNum + 1;//�±��1��ʼ
		//���ȼ���ѡ����ɫ�����߽�
		colorBorderPos.x_min = ( colorXIndex - 1 ) * colorXSize + 1 - borderMargin;
		colorBorderPos.x_max = colorXIndex * colorXSize + borderMargin;
		colorBorderPos.y_min = ( colorYIndex - 1 ) * colorYSize + 1 - borderMargin;
		colorBorderPos.y_max = colorYIndex * colorYSize + borderMargin;

		//ѡ����ɫ�����߽�����ʾ��absPos��
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
//�����ɫ����ѡ����ɫ�ı߽�
bool clearColorSelBorder( const ColorBoard *colorTableInst, const AreaRange *absPos )
{
    bool status = true, status1;

    //ÿ��ɫ��
	const short colorXSize = colorTableInst->colorXSize;//���򳤶�
	const short colorYSize = colorTableInst->colorYSize;//���򳤶�
	//ɫ�����
	const short colorXNum = colorTableInst->colorXNum;//������ɫ����
//	const short colorYNum = colorTableInst->colorYNum;//������ɫ����
//	const short colorSNum = colorXNum * colorYNum;//��ɫ�ܸ���
	//��ɫӳ���
	const color_u16 *const colorSpace = colorTableInst->colorSpace;


	//����ߴ�
//	short xBasePos = absPos->x_min - 1;
//	short yBasePos = absPos->y_min - 1;
//	short width = absPos->x_max - xBasePos;
//	short height = absPos->y_max - yBasePos;

    //�ҵ�ԭ��ѡ�е���ɫ
//	short borderMargin = 1;//�߽��ѡ�п�ľ���

	if ( colorTableInst->colorIndex != NULL )
	{
//		AreaRange colorBorderPos;//���λ��
		short colorIndex = *(colorTableInst->colorIndex);
//		color_u16 borderColor = colorSpace[colorIndex];

		short colorYIndex = colorIndex / colorXNum;//�±��0��ʼ
		short colorXIndex = colorIndex - colorYIndex * colorXNum;//�±��0��ʼ

		//���ڵ�8��ɫ���������һ�Σ��ͻָ���
		ColorInfo blockColor;
		blockColor.borderVisible = false;
		blockColor.objVisible = false;

		AreaRange blockColorArea;
		AreaRange blockColorAbsArea;
		AreaRange colorSelArea;
		colorSelArea.x_min = colorXIndex * colorXSize + 1;
		colorSelArea.x_max = ( colorXIndex + 1 ) * colorXSize;
        colorSelArea.y_min = colorYIndex * colorXSize + 1;
		colorSelArea.y_max = ( colorYIndex + 1 ) * colorYSize;

		short xOffset[8] = { -colorXSize, 0, colorXSize,
                             -colorXSize,    colorXSize,
                             -colorXSize, 0, colorXSize };
		short yOffset[8] = { -colorYSize, -colorYSize, -colorYSize,
                                   0    ,                    0    ,
                              colorYSize,  colorYSize,  colorYSize };
        short blockColorIndex[8] = { - colorXNum - 1, - colorXNum, - colorXNum + 1,
                                            -1      ,                     +1      ,
                                       colorXNum - 1,   colorXNum,   colorXNum + 1 };

        int i;
        for ( i = 0; i < 8; i ++ )
        {
            blockColorArea.x_min = colorSelArea.x_min + xOffset[i];
            blockColorArea.x_max = colorSelArea.x_max + xOffset[i];
            blockColorArea.y_min = colorSelArea.y_min + yOffset[i];
            blockColorArea.y_max = colorSelArea.y_max + yOffset[i];
            getAbsPos( absPos, &blockColorArea, &blockColorAbsArea );

            alt_u8 curBlockColorIndex = colorIndex + blockColorIndex[i];
            blockColor.bkgColor = colorSpace[curBlockColorIndex];

            status1 = showBkgAndBorder( &blockColor, &blockColorAbsArea, NULL, 0 );
            status = status && status1;
        }

	}


    return status;
}


//��ʾscreen_header
bool showScreenHeader( const ScreenHeaderTime *scrHeaderInst, const AreaRange *scrHeaderAbsPos )
{
	bool status = true, status1;

	//��ʾscreen_header�ı�����߽�
	status1 = showBkgAndBorder( &(scrHeaderInst->bkgBorderColor), scrHeaderAbsPos, &(scrHeaderInst->tagTimeArea), 1 );
	status = status && status1;
//	displayAreaRange( scrHeaderAbsPos, 4 );

	//��ʾʱ���ǩ
	AreaRange tagTimeAbsPos;//ʱ���ǩ����λ��
	//�������λ��
	status1 = getAbsPos( scrHeaderAbsPos, &(scrHeaderInst->tagTimeArea), &tagTimeAbsPos );
	status = status && status1;
	//��ʾTag
	status1 = showTagBlock( &(scrHeaderInst->tagTime), &tagTimeAbsPos );
	status = status && status1;


	return status;
}
//��ʾscreen_footer
bool showScreenFooter( const ScreenFooterBtn *scrFooterInst, const AreaRange *scrFooterAbsPos )
{
	bool status = true, status1;

	//��ʾscreen_footer�ı�����߽�
	AreaRange AreaArray[3] = { scrFooterInst->iconSettingArea, scrFooterInst->iconHomeArea, scrFooterInst->iconBackArea };
	status1 = showBkgAndBorder( &(scrFooterInst->bkgBorderColor), scrFooterAbsPos, AreaArray, 3 );
	status = status && status1;

	//��ʾ3������
	AreaRange iconAbsPos;//ͼ�����λ��


	//��ʾ�༭��
	status1 = getAbsPos( scrFooterAbsPos, &(scrFooterInst->iconSettingArea), &iconAbsPos );
	status = status && status1;
	status1 = showIconBlock( &(scrFooterInst->iconSetting), &iconAbsPos );
	status = status && status1;

	//��ʾ��ҳ��
	status1 = getAbsPos( scrFooterAbsPos, &(scrFooterInst->iconHomeArea), &iconAbsPos );
	status = status && status1;
	status1 = showIconBlock( &(scrFooterInst->iconHome), &iconAbsPos );
	status = status && status1;

	//��ʾ���ؼ�
	status1 = getAbsPos( scrFooterAbsPos, &(scrFooterInst->iconBackArea), &iconAbsPos );
	status = status && status1;
	status1 = showIconBlock( &(scrFooterInst->iconBack), &iconAbsPos );
	status = status && status1;

	return status;
}

//��ʾscreen_main
bool showScreenMain( const ScreenContainer *scrContainerInst )
{
    bool status = true, status1;
    //��ʾscreen_main

//**********************����ģʽʱ������TYPEΪ0;  ��������ģʽʱ������TYPEΪ1************************************
#define SCREEN_MAIN_SHOW_TYPE 1

#if SCREEN_MAIN_SHOW_TYPE == 1
    short curScreenIndex = (scrContainerInst->mainAreaInfo).curScrIndex;
	switch( ((scrContainerInst->mainAreaInfo).scrIDStack)[curScreenIndex] )
#elif SCREEN_MAIN_SHOW_TYPE == 0
    switch( SCR_HOME )
#endif // SCREEN_MAIN_SHOW_TYPE
#undef SCREEN_MAIN_SHOW_TYPE
	{
		case SCR_HOME:
			status1 = showScreenMainHome( &(scrContainerInst->scrHome), &(scrContainerInst->mainArea) );
			break;
		case SCR_BOOK:
			status1 = showScreenMainBook( &(scrContainerInst->scrBook), &(scrContainerInst->mainArea) );
			break;
		case SCR_SETTING:
			status1 = showScreenMainSetting( &(scrContainerInst->scrSetting), &(scrContainerInst->mainArea) );
			break;
		case SCR_COLOR_PICKER:
			status1 = showScreenMainColorPicker( &(scrContainerInst->scrColorPicker), &(scrContainerInst->mainArea) );
			break;
		default:
			status1 = false;
	}

	status = status && status1;

	return status;
}

//��ʾscreen_main_home
bool showScreenMainHome( const ScreenHome *scrHomeInst, const AreaRange *scrMainAbsPos )
{
	bool status = true, status1;

	//��ʾ������߽�
	AreaRange AreaArray[3] = { scrHomeInst->numBarArea, scrHomeInst->txtCatalogArea, scrHomeInst->turnCatalogBarArea };
	status1 = showBkgAndBorder( &(scrHomeInst->bkgBorderColor), scrMainAbsPos, AreaArray, 3 );
	status = status && status1;

	//��ʾ��ģ��

	//��ģ�����λ��
	AreaRange subModuAbsPos;

	//��ʾ���ֱ�������б���
	status1 = getAbsPos( scrMainAbsPos, &(scrHomeInst->numBarArea), &subModuAbsPos );
	status = status && status1;
	status1 = showTagList( &(scrHomeInst->numBar), &subModuAbsPos, scrHomeInst->listYOffset );
	status = status && status1;
//printf( "lalaa\n" );
	//��ʾtxt�ı��ļ����б�
	status1 = getAbsPos( scrMainAbsPos, &(scrHomeInst->txtCatalogArea), &subModuAbsPos );
	status = status && status1;
	status1 = showTagList( &(scrHomeInst->txtCatalog), &subModuAbsPos, scrHomeInst->listYOffset );
	status = status && status1;
//printf( "lalab\n" );
	//��ʾ���·�������
	status1 = getAbsPos( scrMainAbsPos, &(scrHomeInst->turnCatalogBarArea), &subModuAbsPos );
	status = status && status1;
	status1 = showScrollBarY( &(scrHomeInst->turnCatalogBarY), &subModuAbsPos );
	status = status && status1;

	return status;
}
//��ʾscreen_main_book
bool showScreenMainBook( const ScreenBook *scrBookInst, const AreaRange *scrMainAbsPos )
{
	bool status = true, status1;

	//���ҳ���Ķ�������Ϣ�ɼ�
	if ( scrBookInst->pageInfoVisible )
	{
		//��ʾ������߽�
		AreaRange AreaArray[2] = { scrBookInst->txtBookArea, scrBookInst->pageInfoArea };
		status1 = showBkgAndBorder( &(scrBookInst->bookColorInfo), scrMainAbsPos, AreaArray, 2 );
		status = status && status1;

		//��ʾ��ģ��

		//��ģ�����λ��
		AreaRange subModuAbsPos;

		//��ʾ����������(��Ϊ�·������浯�����Ķ�������Ϣ���ǣ����������խ
		status1 = getAbsPos( scrMainAbsPos, &(scrBookInst->txtBookNarrowArea), &subModuAbsPos );
		status = status && status1;
		status1 = showTagList( &(scrBookInst->txtBook), &subModuAbsPos, 0 );
		status = status && status1;

		//��ʾ�������Ķ�������Ϣ
		status1 = getAbsPos( scrMainAbsPos, &(scrBookInst->pageInfoArea), &subModuAbsPos );
		status = status && status1;
		status1 = showScreenPageInfo( &(scrBookInst->pageInfo), &subModuAbsPos );
		status = status && status1;
	}
	else
	{
		//��ʾ������߽�
		status1 = showBkgAndBorder( &(scrBookInst->bookColorInfo), scrMainAbsPos, &(scrBookInst->txtBookArea), 1 );
		status = status && status1;

		//��ʾ����������
		AreaRange subModuAbsPos;
		status1 = getAbsPos( scrMainAbsPos, &(scrBookInst->txtBookArea), &subModuAbsPos );
		status = status && status1;
		status1 = showTagList( &(scrBookInst->txtBook), &subModuAbsPos, 0 );
		status = status && status1;
	}

	return status;
}
//��ʾscreen_main_setting
bool showScreenMainSetting( const ScreenSetting *scrSettingInst, const AreaRange *scrMainAbsPos )
{
	bool status = true, status1;

	//��ʾscreen_footer�ı�����߽�
	AreaRange AreaArray[8] = { scrSettingInst->tag1Area, scrSettingInst->editAlarmHourArea, scrSettingInst->editAlarmMinuteArea,\
			scrSettingInst->editBkgColorArea, scrSettingInst->editWordColorArea, scrSettingInst->editTurnPageModArea,\
			scrSettingInst->tag2Area, scrSettingInst->editTurnPageSecArea };
	status1 = showBkgAndBorder( &(scrSettingInst->bkgBorderColor), scrMainAbsPos, AreaArray, 8 );
	status = status && status1;

	//��ʾ��ģ��

	//��ģ�����λ��
	AreaRange subModuAbsPos;

	//��ʾ��̬��ǩ��"��Ϣ��ʱ"
	status1 = getAbsPos( scrMainAbsPos, &(scrSettingInst->tag1Area), &subModuAbsPos );
	status = status && status1;
	status1 = showTagBlock( &(scrSettingInst->tag1), &subModuAbsPos );
	status = status && status1;

	//��ʾ�ɱ༭�����ֱ�ǩͼ���飺�༭��Ϣ��ʱ��ʱ
	status1 = getAbsPos( scrMainAbsPos, &(scrSettingInst->editAlarmHourArea), &subModuAbsPos );
	status = status && status1;
	status1 = showTagIconGroup( &(scrSettingInst->editAlarmHour), &subModuAbsPos );
	status = status && status1;

	//��ʾ�ɱ༭�����ֱ�ǩͼ���飺�༭��Ϣ��ʱ����
	status1 = getAbsPos( scrMainAbsPos, &(scrSettingInst->editAlarmMinuteArea), &subModuAbsPos );
	status = status && status1;
	status1 = showTagIconGroup( &(scrSettingInst->editAlarmMinute), &subModuAbsPos );
	status = status && status1;

	//��ʾ��ǩ�飺�༭������ɫ
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

	//��ʾ��ǩ�飺�༭�ı���ɫ
	status1 = getAbsPos( scrMainAbsPos, &(scrSettingInst->editWordColorArea), &subModuAbsPos );
	status = status && status1;
	status1 = showTagGroup( &(scrSettingInst->editWordColor), &subModuAbsPos );
	status = status && status1;

	//��ʾ��ǩ�飺�Զ���ҳ���ֶ���ҳ�л�
	status1 = getAbsPos( scrMainAbsPos, &(scrSettingInst->editTurnPageModArea), &subModuAbsPos );
	status = status && status1;
	status1 = showTagGroup( &(scrSettingInst->editTurnPageMod), &subModuAbsPos );
	status = status && status1;

	//��ʾ��̬��ǩ��"��ҳ��ʱ"
	status1 = getAbsPos( scrMainAbsPos, &(scrSettingInst->tag2Area), &subModuAbsPos );
	status = status && status1;
	status1 = showTagBlock( &(scrSettingInst->tag2), &subModuAbsPos );
	status = status && status1;

	//��ʾ�ɱ༭�����ֱ�ǩͼ���飺�༭��ҳ��ʱ����
	status1 = getAbsPos( scrMainAbsPos, &(scrSettingInst->editTurnPageSecArea), &subModuAbsPos );
	status = status && status1;
	status1 = showTagIconGroup( &(scrSettingInst->editTurnPageSec), &subModuAbsPos );
	status = status && status1;

	return status;
}
//��ʾscreen_main_color_picker
bool showScreenMainColorPicker( const ScreenColorPicker *scrColorPickerInst, const AreaRange *scrMainAbsPos )
{
	bool status = true, status1;

	//���߽��뱳��
	//��ʾ�߽��뱳��
	status1 = showBkgAndBorder( &(scrColorPickerInst->bkgBorderColor), scrMainAbsPos, &(scrColorPickerInst->colorPickerArea), 1 );
	status = status && status1;

	//��ʾȡɫ��
	AreaRange colorTableAbsPos;//ɫ�����λ��
	status1 = getAbsPos( scrMainAbsPos, &(scrColorPickerInst->colorPickerArea), &colorTableAbsPos );
	status = status && status1;
	status1 = showColorBoard( &(scrColorPickerInst->colorPicker), &colorTableAbsPos );
	status = status && status1;

	return status;
}


//��ʾscreen
bool showScreen( )
{
	bool status = true, status1;

	const ScreenContainer *tftlcdScreenPtr = &tftlcdScreenInst;

	//��ʾscreen_header
	status1 = showScreenHeader( &(tftlcdScreenPtr->scrHeader), &(tftlcdScreenPtr->headerArea) );
	status = status && status1;
	//��ʾscreen_footer
	status1 = showScreenFooter( &(tftlcdScreenPtr->scrFooter), &(tftlcdScreenPtr->footerArea) );
	status = status && status1;

	//��ʾscreen_main
    status1 = showScreenMain( tftlcdScreenPtr );
	status = status && status1;

	return status;
}
