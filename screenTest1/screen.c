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
#include "icon_pic_extern.h"
#include "test_value.h"
#include "color.h"



//	sprintf(str, "%02d", num);���Խ����ֱ�Ϊ�ַ���

//������Ļ����
ScreenContainer tftlcdScreenInst;

ColorInfo defaultColorInfo = {  DEFAULT_BORDER_COLOR, DEFAULT_BKG_COLOR, DEFAULT_OBJ_COLOR, true, true };

char *str1 = "Happy";

//�������Χ�����Ϊ��ʽ�����򷵻�false
bool checkAreaRange( const AreaRange *areaPtr )
{
    bool status = true;

    //��ʽҪ��:
    //1.width, height >= 0
    //2.x_min,y_min >= 1

    if ( areaPtr->x_min > areaPtr->x_max + 1 )//��������width = 0�����
	{
		status = false;
	}
	else if ( areaPtr->x_min < 1 )
    {
        status = false;
    }
	if ( areaPtr->y_min > areaPtr->y_max + 1 )//��������height = 0�����
	{
		status = false;
	}
	else if ( areaPtr->y_min < 1 )
    {
        status = false;
    }

	return status;

}

//��������Χ
bool setAreaRange( AreaRange *areaPtr, short x_min, short x_max, short y_min, short y_max )
{
	areaPtr->x_min = x_min;
	areaPtr->x_max = x_max;
	areaPtr->y_min = y_min;
	areaPtr->y_max = y_max;

	bool status = checkAreaRange( areaPtr );

	return status;
}
//����Χ�Զ�ת��
bool setAreaRangeByAreaFmt( AreaRange *areaPtr, AreaFmt *areaFmtPos, short width, short height )
{

	AreaFmtCenter *tempFmtCenterPos;
	AreaFmtMargin *tempFmtMarginPos;

	bool status = true, status1;

	//����x����
	switch( areaFmtPos->fmtTypeX )
	{
		case AREA_FMT_CENTER:
			tempFmtCenterPos = &((areaFmtPos->fmtValX).fmtCenterVal);

			areaPtr->x_min = tempFmtCenterPos->center - tempFmtCenterPos->half + 1;
			areaPtr->x_max = tempFmtCenterPos->center + tempFmtCenterPos->half;

			break;

		case AREA_FMT_MARGIN:
			tempFmtMarginPos = &((areaFmtPos->fmtValX).fmtMarginVal);

			areaPtr->x_min = tempFmtMarginPos->margin_LU + 1;
			areaPtr->x_max = width - tempFmtMarginPos->margin_RD;

			break;
		default:
			status = false;
	}
	//����y����
	switch( areaFmtPos->fmtTypeY )
	{
		case AREA_FMT_CENTER:
			tempFmtCenterPos = &((areaFmtPos->fmtValY).fmtCenterVal);

			areaPtr->y_min = tempFmtCenterPos->center - tempFmtCenterPos->half + 1;
			areaPtr->y_max = tempFmtCenterPos->center + tempFmtCenterPos->half;

			break;

		case AREA_FMT_MARGIN:
			tempFmtMarginPos = &((areaFmtPos->fmtValY).fmtMarginVal);

			areaPtr->y_min = tempFmtMarginPos->margin_LU + 1;
			areaPtr->y_max = width - tempFmtMarginPos->margin_RD;

			break;
		default:
			status = false;
	}

    status1 = checkAreaRange( areaPtr );
    status = status && status1;


	return status;

}

//���������ʽ
bool setAreaFmt( AreaFmt *areaFmtPos, enum areaFmtID fmtXID, enum areaFmtID fmtYID, short xC_xML, short xHf_xMR, short yC_yMU, short yHf_yMD )
{
	bool status = true;
	areaFmtPos->fmtTypeX = fmtXID;
	areaFmtPos->fmtTypeY = fmtYID;

	switch( fmtXID )
	{
		case AREA_FMT_CENTER:
			(areaFmtPos->fmtValX).fmtCenterVal.center = xC_xML;
			(areaFmtPos->fmtValX).fmtCenterVal.half = xHf_xMR;
			break;

		case AREA_FMT_MARGIN:
			(areaFmtPos->fmtValX).fmtMarginVal.margin_LU = xC_xML;
			(areaFmtPos->fmtValX).fmtMarginVal.margin_RD = xHf_xMR;
			break;
		default:
			status = false;
	}
	switch( fmtYID )
	{
		case AREA_FMT_CENTER:
			(areaFmtPos->fmtValY).fmtCenterVal.center = yC_yMU;
			(areaFmtPos->fmtValY).fmtCenterVal.half = yHf_yMD;
			break;

		case AREA_FMT_MARGIN:
			(areaFmtPos->fmtValY).fmtMarginVal.margin_LU = yC_yMU;
			(areaFmtPos->fmtValY).fmtMarginVal.margin_RD = yHf_yMD;
			break;
		default:
			status = false;
	}
	return status;
}

//������������Χ(�������ż��������΢ƫ���Ͻ�)
bool setAreaRangeCentered( AreaRange *areaPtr, short width, short height, short objXSize, short objYSize )
{
	bool status = true, status1;

	short marginXL, marginXR, marginYU, marginYD;

	//obj�ߴ糬��ģ�鷶Χʱ�������Ƕ���obj������ߴ磬objֱ��ʹ��ģ��ߴ�
	if ( width < objXSize )
	{
		marginXL = 0;
		marginXR = 0;
		status = false;//��������������ģ��Ķ���ߴ糬�����ұ߽磩
	}
	else//��������
    {
        marginXL = ( width - objXSize ) >> 1;//��߾�
        marginXR = ( width - objXSize ) - marginXL;//�ұ߾�
    }

	if ( height < objYSize )
    {
        marginYU = 0;
        marginYD = 0;
        status = false;//��������������ģ��Ķ���ߴ糬�����±߽磩
    }
    else//��������
    {
        marginYU = ( height - objYSize )>> 1;//�ϱ߾�
        marginYD = ( height - objYSize ) - marginYU;//�±߾�
    }

	status1 = setAreaRange( areaPtr, marginXL+1, width-marginXR, marginYU+1, height-marginYD );
	status = status && status1;

	return status;
}
//����Ĭ�Ͼ��б�ǩ
bool setDefaultTagBlock( TagBlock *tagPtr, char *text, short width, short height )
{
	tagPtr->tagColorInfo = defaultColorInfo;

	tagPtr->wordSpacing = DEFAULT_WORD_SPACING;
	tagPtr->letterSpacing = DEFAULT_LETTER_SPACING;
	tagPtr->wordXSize = SCR_WORD_SIZE_X;//���ֺ������
	tagPtr->letterXSize = SCR_LETTER_SIZE_X;//��ĸ�������
	tagPtr->objYSize = SCR_OBJ_SIZE_Y;//��������߶�
	(tagPtr->tagText).text = text;

	short textLen = ( text == NULL )?0:strlen(text);

	(tagPtr->tagText).textLen = textLen;

	(tagPtr->tagColorInfo).objVisible = ( text == NULL ) ? false : true;

	short textWidthSum = getWidthOfText( text, textLen );

	bool status = setAreaRangeCentered( &(tagPtr->textArea ), width, height, textWidthSum, SCR_OBJ_SIZE_Y );

	return status;
}
//����Ĭ�Ͻ�����
bool setDefaultProgressBarX( ProgressBarX *prgBarInst, short width, short height )
{
	bool status = true, status1;
	//������Ϣ
	prgBarInst->bkgBorderColor = defaultColorInfo;

	prgBarInst->xOffset = 0;
	prgBarInst->xOffsetMax = 280;

	//�˶���
	short dotHeight = ICON_DOT_Y_SIZE;
	short axisHeight = dotHeight / 2;
	short axisWidth = prgBarInst->xOffsetMax + 1;
	status1 = setAreaRangeCentered( &(prgBarInst->axisArea), width, height, axisWidth, axisHeight );
	status = status && status1;
	//����ó���������ı߽��������ɫ����
	status1 = setDefaultIconBlock( &(prgBarInst->axis), (void*)0, axisWidth, axisHeight, 0, 0 );
	status = status && status1;


	//���ȵ����������ƫ����ʱ��
	short dotWidth = ICON_DOT_X_SIZE;
	short dotMarginXL = (prgBarInst->axisArea).x_min - dotWidth/2 - 1;
	short dotMarginYU = ( (prgBarInst->axisArea).y_min - 1 + (prgBarInst->axisArea).y_max - dotHeight ) / 2;
	status1 = setAreaRange( &(prgBarInst->dotBaseArea), dotMarginXL + 1, dotMarginXL + dotWidth,\
					dotMarginYU + 1, dotMarginYU + dotHeight);
    status = status && status1;
	status1 = setDefaultIconBlock( &(prgBarInst->dot), icon_dot, dotWidth, dotHeight, dotWidth, dotHeight );
	status = status && status1;

	//�û������
	status1 = setAreaRangeCentered( &(prgBarInst->clickArea), width, height, axisWidth + dotWidth, height / 2 );
	status = status && status1;


	return status;

}
//����Ĭ�ϴ�ֱ������
bool setDefaultScrollBarY( ScrollBarY *scrollBarYInst, short width, short height, float barYRatio )
{
	bool status = true, status1;

	scrollBarYInst->barYColorInfo = defaultColorInfo;

	short iconMarginXL = 1;
	short iconMarginXR = 1;
	short iconUpMarginYU = 1;
	short iconDownMarginYD = 1;
	short iconWidth = width - iconMarginXL - iconMarginXR;
	short iconUpHeight = iconWidth;
	short iconDownHeight = iconWidth;
	short iconDownMarginYU = height - iconDownHeight - iconUpMarginYU;
	short iconSpacing = 2;//iconBlock֮���������
	short iconBarMoveMarginYU = iconUpMarginYU + iconUpHeight + iconSpacing;
	short iconBarMoveHeight = height - iconDownMarginYD - iconDownHeight - iconSpacing - iconBarMoveMarginYU;

	//���Ƽ�ͼ��
	status1 = setAreaRange( &(scrollBarYInst->iconMoveUpArea), iconMarginXL+1, iconMarginXL+iconWidth,\
					iconUpMarginYU+1, iconUpMarginYU+iconUpHeight );
    status = status && status1;
	status1 = setDefaultIconBlock( &(scrollBarYInst->iconMoveUp), icon_move_up, iconWidth, iconUpHeight,\
					ICON_MOVE_UP_X_SIZE, ICON_MOVE_UP_Y_SIZE );
    status = status && status1;

	//���Ƽ�ͼ��
	status1 = setAreaRange( &(scrollBarYInst->iconMoveDownArea), iconMarginXL+1, iconMarginXL+iconWidth,\
					iconDownMarginYU+1, iconDownMarginYU+iconDownHeight );
    status = status && status1;
	status1 = setDefaultIconBlock( &(scrollBarYInst->iconMoveDown), icon_move_down, iconWidth, iconDownHeight,\
					ICON_MOVE_DOWN_X_SIZE, ICON_MOVE_DOWN_Y_SIZE );
    status = status && status1;

//printf( "status4_1 = %s\n", status1?"true":"false" );
	//������
	//�˶���
	status1 = setAreaRange( &(scrollBarYInst->barMoveArea), iconMarginXL+1, iconMarginXL+iconWidth, \
					iconBarMoveMarginYU+1, iconBarMoveMarginYU+iconBarMoveHeight );
    status = status && status1;
//printf( "status4_2 = %s\n", status1?"true":"false" );
	//����������ƫ����
	if( barYRatio <= 1 )
    {
        //����������ĳ��ȣ�barYRatio�ǹ������߶�ռ�����˶����߶ȵı���
        scrollBarYInst->barHeight = barYRatio * iconBarMoveHeight;
        scrollBarYInst->iconBarVisible = true;
    }
    else
    {
        scrollBarYInst->iconBarVisible = false;
        scrollBarYInst->barHeight = iconBarMoveHeight;
    }


	scrollBarYInst->barYOffsetMax = iconBarMoveHeight - scrollBarYInst->barHeight;
	scrollBarYInst->barYoffset = 0;

	//������������ƫ����Ϊ0ʱ��λ�ã�
	status1 = setAreaRange( &(scrollBarYInst->barBaseArea), iconMarginXL+1, iconMarginXL+iconWidth,\
					iconBarMoveMarginYU+1, iconBarMoveMarginYU+scrollBarYInst->barHeight );
//    printf( "width = %d\n", scrollBarYInst->barBaseArea.x_max - scrollBarYInst->barBaseArea.x_min + 1 );
//    printf( "height = %d\n", scrollBarYInst->barBaseArea.y_max - scrollBarYInst->barBaseArea.y_min + 1 );
//    printf( "scrollBarYInst->barHeight = %d\n", scrollBarYInst->barHeight );

    status = status && status1;
//printf( "status4_3 = %s\n", status1?"true":"false" );
	status1 = setDefaultIconBlock( &(scrollBarYInst->iconBar), (void*)0, iconWidth, scrollBarYInst->barHeight, 0, 0 );
	status = status && status1;


	return status;
}
//����Ĭ�Ͼ���ͼ��
bool setDefaultIconBlock( IconBlock *iconPtr, const alt_u8 *iconModel, short width, short height, short iconXSize, short iconYSize )
{
	iconPtr->iconColorInfo = defaultColorInfo;

	iconPtr->iconPic = iconModel;
	iconPtr->iconXSize = iconXSize;
	iconPtr->iconYSize = iconYSize;

	(iconPtr->iconColorInfo).objVisible = ( iconModel == NULL )? false : true;



	bool status = setAreaRangeCentered( &(iconPtr->picArea ), width, height, iconXSize, iconYSize );


	return status;
}

//����ȡɫ����Ϣ(��ɫ��λ��colorPos��ʾ��Ļ��ѡ�е�λ�ã����ݸ�λ�ã���ʾ����ͻ���ÿ���ɫ�ı߽�)
void setDefaultColorBoard( ColorBoard *colorPickerInst, color_u8 *colorSel, short width, short height )
{
	colorPickerInst->colorIndex = colorSel;
	colorPickerInst->colorXNum = COLOR_FORM_X_NUM;//��ɫ��16��
	colorPickerInst->colorYNum = COLOR_FORM_Y_NUM;//��ɫ��16��
	colorPickerInst->colorXSize = width / 16;
	colorPickerInst->colorYSize = height / 16;
//    colorPickerInst->colorXSize = 35;
//    colorPickerInst->colorYSize = 38;
	colorPickerInst->colorSpace = color_form;
}

//��ȡ�ı��к�������ĸ�ķֲ���(ֵΪ'c'���������֣�ֵΪ'e'������Ӣ����ĸ
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

//������Ч�ı�ȫ����ʾʱ�����ؿ���
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

//�����ļ��������������
bool openTxtScrollX( ScreenHome *scrHomeInst, int scrollTxtFileIndex )
{
    scrHomeInst->txtScrollable = true;
    scrHomeInst->txtScrollrow = scrollTxtFileIndex;
    scrHomeInst->txtScrollXOffset = 0;

    TextType *tempText = &(scrHomeInst->txtCatalog.elemBlock[scrollTxtFileIndex].tagText);

    //��������ļ�������Ч�ı�����
    scrHomeInst->txtScrollwidth = getWidthOfText( tempText->text, tempText->textLen );

    //�б�Ԫ���ı����Ŀ���
    short elemTextAreaWidth = scrHomeInst->txtCatalog.elemTextArea.x_max - scrHomeInst->txtCatalog.elemTextArea.x_min + 1;


    //����ı�̫��
    if ( scrHomeInst->txtScrollwidth < elemTextAreaWidth / 2 )
    {
        scrHomeInst->txtScrollSpacing = elemTextAreaWidth - scrHomeInst->txtScrollwidth;
    }
    else
    {
        //����ı������ȵ�һ���С
        scrHomeInst->txtScrollSpacing = elemTextAreaWidth / 2;
    }
    return true;
}


//����Ĭ�ϱ�ǩ��
bool setDefaultTagGroup( TagGroup *tagGroupInst, AreaFmt *tagStaticPos, AreaFmt *tagVarPos, short width, short height,\
			char *tagStaText, char *tagVarText )
{
	bool status = true, status1;
//    printf( "vlaue = %d\n", height );
	tagGroupInst->bkgBorderColor = defaultColorInfo;

	//���þ�̬��ǩ��λ��
	AreaRange *tempArea = &(tagGroupInst->staticTagArea);
	status1 = setAreaRangeByAreaFmt( tempArea, tagStaticPos, width, height );
	status = status && status1;
//	displayAreaRange( tempArea, 0 );
	//���þ�̬��ǩ����
	status1 = setDefaultTagBlock( &(tagGroupInst->staticTag), tagStaText, \
			tempArea->x_max - tempArea->x_min + 1, tempArea->y_max - tempArea->y_min + 1);
    status = status && status1;



	//���ö�̬��ǩ��λ��
	tempArea = &(tagGroupInst->varTagArea);
	status1 = setAreaRangeByAreaFmt( tempArea, tagVarPos, width, height );
	status = status && status1;
//	displayAreaRange( tempArea, 0 );
	//���ö�̬��ǩ����

	status1 = setDefaultTagBlock( &(tagGroupInst->varTag), tagVarText, \
			tempArea->x_max - tempArea->x_min + 1, tempArea->y_max - tempArea->y_min + 1);
    status = status && status1;

	return status;
}

//����Ĭ�Ͽɱ༭����(��̬��ǩ+��̬ͼ��)
bool setDefaultNumEditGroup( TagIconGroup *numEditGrpInst, short width, short height, short numMax, short numMin, short num, char *unitName )
{
	bool status = true, status1;

	//���û�����Ϣ
	numEditGrpInst->bkgBorderColor = defaultColorInfo;

	//�������ֱ仯��Χ
	numEditGrpInst->value = num;
	numEditGrpInst->valueMax = numMax;
	numEditGrpInst->valueMin = numMin;

	//���ø������λ��
	/*
	 * ________________________
	 * |                      |
	 * |  plusIcon            |
	 * |                      |
	 * |  numTag    unitTag   |
	 * |                      |
	 * |  minusIcon           |
	 * |______________________|
	 */

	short tagHeight = 30;
	short tagWidth = 30;
	short iconBlockWidth = 30;
	short iconBlockHeight = 20;
	short numTagMarginXL = 10;
	short plusIconMarginYU = 10;
	short minusIconMarginYD = 10;
	short twoTagSpacingX = 10;

	//����plusIcon
	status1 = setAreaRange( &(numEditGrpInst->plusIconArea), numTagMarginXL+1, numTagMarginXL+iconBlockWidth,\
				plusIconMarginYU + 1, plusIconMarginYU + iconBlockHeight );
    status = status && status1;
	status1 = setDefaultIconBlock( &(numEditGrpInst->plusIcon), icon_plus, \
				iconBlockWidth, iconBlockHeight, ICON_PLUS_X_SIZE, ICON_PLUS_Y_SIZE );
    status = status && status1;

	//����numberTag
	char *numStr = (char*)malloc( sizeof(char) * 7 );
	if ( num < 0 || num > 99 )
    {
        num = 0;
    }
	sprintf(numStr, "%2d", num);
	status1 = setAreaRange( &(numEditGrpInst->valueTagArea), numTagMarginXL+1, numTagMarginXL+tagWidth,\
				(height>>1)-(tagHeight>>1)+1, (height>>1)+(tagHeight>>1) );
    status = status && status1;
	status1 = setDefaultTagBlock( &(numEditGrpInst->valueTag), numStr, tagWidth, tagHeight );
	status = status && status1;


	//����minusIcon
	status1 = setAreaRange( &(numEditGrpInst->minusIconArea), numTagMarginXL+1, numTagMarginXL+tagWidth,\
				height - minusIconMarginYD - iconBlockHeight+1, height - minusIconMarginYD );
    status = status && status1;
	status1 = setDefaultIconBlock( &(numEditGrpInst->minusIcon), icon_minus, \
				iconBlockWidth, iconBlockHeight, ICON_MINUS_X_SIZE, ICON_MINUS_Y_SIZE );
    status = status && status1;


	//����unitTag
	short unitTagMarginXL = numTagMarginXL + tagWidth + twoTagSpacingX;
	status1 = setAreaRange( &(numEditGrpInst->unitTagArea), unitTagMarginXL+1, unitTagMarginXL+tagWidth,\
					(height>>1)-(tagHeight>>1)+1, (height>>1)+(tagHeight>>1) );
    status = status && status1;
	status1 = setDefaultTagBlock( &(numEditGrpInst->unitTag), unitName, tagWidth, tagHeight );
	status = status && status1;


	return status;
}


//��ʼ��
bool screenInit()
{
	//������ģ����ȶ���ͬ ( 1 �� x �� SCR_WIDE )

	bool status = true, status1;
	//��ʼ����Ļ����ߴ� ( 1 �� y �� SCR_HEIGHT )
	status1 = setAreaRange( &(tftlcdScreenInst.screenArea), 1, SCR_WIDTH, 1, SCR_HEIGHT );
	status = status && status1;

	//1.��ʼ������ ( 1 �� y �� headerHeight )
	short headerHeight = SCR_HEADER_HEIGHT;//�����߶� (
	status1 = setAreaRange( &(tftlcdScreenInst.headerArea), 1, SCR_WIDTH, 1, headerHeight );
	status = status && status1;

	status1 = screenHeaderInit( &(tftlcdScreenInst.scrHeader), SCR_WIDTH, headerHeight );
	status = status && status1;

	//2.��ʼ������ ( SCR_HEIGHT - footerHeight + 1 �� y �� SCR_HEIGHT )
	short footerHeight = SCR_FOTTER_HEIGHT;//�����߶�
	status1 = setAreaRange( &(tftlcdScreenInst.footerArea), 1, SCR_WIDTH, SCR_HEIGHT - footerHeight + 1, SCR_HEIGHT );
	status = status && status1;

	status1 = screenFooterInit( &(tftlcdScreenInst.scrFooter), SCR_WIDTH, footerHeight );
	status = status && status1;



	//3.��ʼ������  ( headerHeight + 1 �� y �� SCR_HEIGHT - footerHeight )
	short mainAreaHeight = SCR_HEIGHT - headerHeight - footerHeight;
	setAreaRange( &(tftlcdScreenInst.mainArea), 1, SCR_WIDTH, headerHeight + 1, SCR_HEIGHT - footerHeight );

	//ע�⣺txtFilesInfoSpace�ڵ����ݱ�����ǰ���ã�����������г�������ˣ�ִ��screenInit()֮ǰ����������txtFilesInfoSpace��Ϣ���
	status1 = screenMainHomeInit( &(tftlcdScreenInst.scrHome), SCR_WIDTH, mainAreaHeight, &txtFilesInfoSpace );
	status = status && status1;
	status1 = screenMainBookInit( &(tftlcdScreenInst.scrBook), SCR_WIDTH, mainAreaHeight );
	status = status && status1;
	status1 = screenMainSettingInit( &(tftlcdScreenInst.scrSetting), SCR_WIDTH, mainAreaHeight );
	status = status && status1;
	status1 = screenMainColorPickerInit( &(tftlcdScreenInst.scrColorPicker), SCR_WIDTH, mainAreaHeight );
	status = status && status1;

	//4.��ʼ������ҳ����Ϣ
	screenMainAreaInfoInst( &(tftlcdScreenInst.mainAreaInfo) );


	//5.�����Զ�������ԣ���ɫ���߽��Ƿ�ɼ��ȣ����޸�֮ǰ�������õ���Ļ�ṹ�����Ĭ�����ԣ�
    screenSetCustomInfo( &tftlcdScreenInst );
	return status;
}

bool screenHeaderInit( ScreenHeaderTime *scrHeaderInst, short width, short height )
{

	scrHeaderInst->bkgBorderColor = defaultColorInfo;

	short marginX = 100;//����߾�
	short marginY = 5;//����߾�

	bool status = true, status1;

	status1 = setAreaRange( &(scrHeaderInst->tagTimeArea), marginX+1, width-marginX, marginY+1, height-marginY );
	status = status && status1;


	//ʱ���ı�Ϊ"00:00"(0ʱ0��)
	status1 = setDefaultTagBlock( &(scrHeaderInst->tagTime), "00:00", width - (marginX<<1), height - (marginY<<1) );
	status = status && status1;
//	(scrHeaderInst->tagTime).borderVisible = false;

	return status;
}
bool screenFooterInit( ScreenFooterBtn *scrFooterInst, short width, short height )
{
	scrFooterInst->bkgBorderColor = defaultColorInfo;

	//SETTING��,HOME��,BACK���ֱ����±��������С���λ��
	short marginX = 20;
	short marginY = 10;
	short iconBlockHeight = height - (marginY<<1);
	short iconBlockWidth = width / 5;

	bool status = true, status1;

	status1 = setAreaRange( &(scrFooterInst->iconSettingArea), marginX+1, marginX+iconBlockWidth, marginY+1, height - marginY );
	status = status && status1;
	status1 = setAreaRangeCentered( &(scrFooterInst->iconHomeArea), width, height, iconBlockWidth, iconBlockHeight );
	status = status && status1;
	status1 = setAreaRange( &(scrFooterInst->iconBackArea), width-marginX-iconBlockWidth+1, width-marginX, marginY+1, height - marginY );
    status = status && status1;

	status1 = setDefaultIconBlock( &(scrFooterInst->iconSetting), icon_setting, iconBlockWidth, iconBlockHeight, ICON_SETTING_X_SIZE, ICON_SETTING_Y_SIZE );
	status = status && status1;
	status1 = setDefaultIconBlock( &(scrFooterInst->iconHome), icon_home, iconBlockWidth, iconBlockHeight, ICON_HOME_X_SIZE, ICON_HOME_Y_SIZE );
	status = status && status1;
	status1 = setDefaultIconBlock( &(scrFooterInst->iconBack), icon_back, iconBlockWidth, iconBlockHeight, ICON_BACK_X_SIZE, ICON_BACK_Y_SIZE );
    status = status && status1;

	return status;
}

bool screenMainHomeInit( ScreenHome *scrHomeInst, short width, short height, const TxtFilesInfo *txtFilesInfoPtr )
{
	scrHomeInst->bkgBorderColor = defaultColorInfo;

	bool status = true, status1;

	//�б�
	short blockMarginY = 1;
	short blockHeight = height - (blockMarginY<<1);

	short numListMarginXL = 1;
	short numListWidth = 50;//���ֱ���б�Ԫ�ؿ���

	short scrollBarMarginXR = 1;
	short scrollBarWidth = 50;//����������
	short scrollBarMarginXL = width - scrollBarMarginXR - scrollBarWidth;

	short catalogListSpacingL = 2;//txt�ļ�Ŀ¼��������ֱ���б��ļ��
	short catalogListSpacingR = 2;//txt�ļ�Ŀ¼���Ҳ�������ļ��
	short catalogListMarginXL = numListMarginXL + numListWidth + catalogListSpacingL;
	short catalogListWidth = width - scrollBarMarginXR - scrollBarWidth - catalogListSpacingR - catalogListMarginXL;

	//�б�Ԫ�ظ���
	short listElemNum = txtFilesInfoPtr->txtFilesNum;//�б�Ԫ�ظ���
	short listElemHeight = CATALOG_ROW_HEIGHT;//�б�Ԫ�ظ߶�




	//�б���������
	//����TextList����malloc��Ҫ�ռ䣬����txtFilesInfoPtr�е���Ч��Ϣ���浽��Ҫ��TextList�ռ���
	TextList numList;
	numList.textNum = listElemNum;
	numList.textArray = (TextType*)malloc( sizeof(TextType) * listElemNum );
	TextType *textAry = numList.textArray;
	int i;
	char tempStr[10];
	for ( i = 0; i < listElemNum; i ++ )
	{
		sprintf(tempStr, "%2d", i + 1 );//��ȡ����ַ�������" 1"��ʼ,���"99"��
		(textAry[i]).text = (char*)malloc( sizeof(char) * 3 );
		((textAry[i]).text)[0] = tempStr[0];
		((textAry[i]).text)[1] = tempStr[1];
		((textAry[i]).text)[2] = 0;
		(textAry[i]).textLen = 2;
	}


	TextList catalogList;
	catalogList.textNum = listElemNum;
	catalogList.textArray = (TextType*)malloc( sizeof(TextType) * listElemNum );
	TextType *catalogAry = catalogList.textArray;
	TxtFile *txtFileArray = txtFilesInfoPtr->txtFileList;

	for ( i = 0; i < listElemNum; i ++ )
	{
		catalogAry[i] = (txtFileArray[i]).txtFileName;
	}
	if ( listElemNum <= 0 )
    {
        free(catalogList.textArray);
        catalogList.textArray = NULL;
    }




	//���ֱ���б�
    ColorInfo numBarElemColorInfo;
    numBarElemColorInfo.bkgColor = getColor16BitByRGB( 242, 223, 85 );
    numBarElemColorInfo.borderColor = getColor16BitByRGB( 159, 152, 11 );
    numBarElemColorInfo.objColor = getColor16BitByRGB( 7, 135, 163 );
    numBarElemColorInfo.borderVisible = true;
    numBarElemColorInfo.objVisible = true;
	status1 = setAreaRange( &(scrHomeInst->numBarArea), numListMarginXL+1, numListMarginXL+numListWidth,\
							blockMarginY+1, height-blockMarginY );
    status = status && status1;
	status1 = setDefaultTagList( &(scrHomeInst->numBar), numListWidth, listElemHeight, getWidthOfText( "99", 2 ),\
                              &numList, &numBarElemColorInfo );
//    printf( "status1 = %s\n", status1?"true":"false" );
    status = status && status1;
//    printf( "status2 = %s\n", status1?"true":"false" );

	//txt�ļ�Ŀ¼�б�
    ColorInfo txtCatalogElemColorInfo;
    txtCatalogElemColorInfo.bkgColor = getColor16BitByRGB( 72, 193, 133 );
    txtCatalogElemColorInfo.borderColor = getColor16BitByRGB( 159, 152, 11 );
    txtCatalogElemColorInfo.objColor = getColor16BitByRGB( 7, 135, 163 );
    txtCatalogElemColorInfo.borderVisible = true;
    txtCatalogElemColorInfo.objVisible = true;
	short textMarginXInlistElem = 5;
	status1 = setAreaRange( &(scrHomeInst->txtCatalogArea), catalogListMarginXL+1, catalogListMarginXL+catalogListWidth,\
							blockMarginY+1, height-blockMarginY );
    status = status && status1;
	status1 = setDefaultTagList( &(scrHomeInst->txtCatalog), catalogListWidth, listElemHeight,\
                             catalogListWidth - (textMarginXInlistElem<<1),\
                              &catalogList, &txtCatalogElemColorInfo );
    status = status && status1;
//    printf( "status3 = %s\n", status1?"true":"false" );
	//�ͷ�ʹ����ϵ�numListPtr��catalogListPtr�ռ�
	free(numList.textArray);

	free(catalogList.textArray);


	//�б������ƶ���ʾ����
	scrHomeInst->listYOffset = 0;//�б���ʾʱ������ƫ����
	scrHomeInst->listShowHeight = blockHeight;//�б���ʾ����Ļ��ʱ�ĸ߶�
	scrHomeInst->listYSize = listElemNum * listElemHeight;//�б�����ȫ��
	if ( listElemNum > 0 )
    {
        scrHomeInst->listShowYRatio = 1.0 * scrHomeInst->listShowHeight / scrHomeInst->listYSize;
    }
	else
    {
        scrHomeInst->listShowYRatio = 10000;
    }
	scrHomeInst->listYOffsetMax = scrHomeInst->listYSize - blockHeight;
	scrHomeInst->listYStepSize = CATALOG_ROW_HEIGHT;

	if ( scrHomeInst->listYOffsetMax < 0 )//�б�����̫�̣�����Ҫ����
    {
        scrHomeInst->listYOffsetMax = 0;
    }

	//������
	status1 = setAreaRange( &(scrHomeInst->turnCatalogBarArea), scrollBarMarginXL + 1, scrollBarMarginXL + scrollBarWidth,\
							blockMarginY+1, height-blockMarginY );
//    printf( "status4 = %s\n", status1?"true":"false" );
    status = status && status1;
	status1 = setDefaultScrollBarY( &(scrHomeInst->turnCatalogBarY), scrollBarWidth, blockHeight, scrHomeInst->listShowYRatio );
    status = status && status1;

//    printf( "status5 = %s\n", status1?"true":"false" );

	//ĳ��Ŀ¼�ļ���ˮƽ��������
	scrHomeInst->txtScrollable = false;//����������
	scrHomeInst->txtScrollrow = 0;//�ɹ�����ĳ��������һ��ֻ��һ�п��Թ�����
	scrHomeInst->txtScrollXOffset = 0;//��ǰˮƽ����ƫ����
	scrHomeInst->txtScrollXStepSize = 0;//ˮƽ��������
	scrHomeInst->txtScrollwidth = 0;//�����ı�����ʵ���ؿ��ȣ����Գ�����Ļ���ȣ�
	scrHomeInst->txtScrollSpacing = width / 2;//�����ı�ѭ��ʱ�ı�β�����ı�ͷ�ļ��


	return status;
}
//����Ĭ���ı���ǩ�б�
bool setDefaultTagList( TagList *TagListInst, short elemWidth, short elemHeight, short textWidth,\
                       TextList *textListPtr, ColorInfo *colorInfoPtr )
{
	//�����б�Ԫ�������С
	bool status = true, status1;


	status1 = setAreaRange( &(TagListInst->elemBaseArea), 1, elemWidth, 1, elemHeight );
	status = status && status1;
	//�����ı���������
	short marginXL = ( elemWidth - textWidth )>>1;
	short marginXR = elemWidth - marginXL - textWidth;
	short marginYU = ( elemHeight - SCR_WORD_SIZE_Y )>>1;
	short marginYD = elemHeight - marginYU - SCR_WORD_SIZE_Y;

	if ( elemWidth < textWidth )
	{
		marginXL = 0;
		marginXR = 0;
		status = false;
	}
	if ( elemHeight < SCR_WORD_SIZE_Y )
    {
        marginYU = 0;
		marginYD = 0;
		status = false;
    }

	//����Ԫ���ı�����λ��
	status1 = setAreaRange( &(TagListInst->elemTextArea), marginXL+1, elemWidth-marginXR, marginYU+1, elemHeight-marginYD );
	status = status && status1;

	short elemNum = textListPtr->textNum;

	TagListInst->wordSpacing = DEFAULT_WORD_SPACING;
	TagListInst->letterSpacing = DEFAULT_LETTER_SPACING;

	TagListInst->elemNum = elemNum;
	TagListInst->elemColorInfo = *colorInfoPtr;

	int i;


	TagBlock *tagBolckArray = (TagBlock*)malloc( sizeof( TagBlock ) * elemNum );
	if ( elemNum <= 0 )
    {
        free(tagBolckArray);
        tagBolckArray = NULL;
    }
    TagListInst->elemBlock = tagBolckArray;


	TagBlock *curTagPtr;
	TextType *textArray = textListPtr->textArray;

	short elemYPos = 0;
	for ( i = 0; i < elemNum; i ++ )
	{
		curTagPtr = &(tagBolckArray[i]);

		curTagPtr->tagColorInfo = TagListInst->elemColorInfo;
		curTagPtr->wordSpacing = TagListInst->wordSpacing;
		curTagPtr->letterSpacing = TagListInst->letterSpacing;
		curTagPtr->wordXSize = SCR_WORD_SIZE_X;
		curTagPtr->letterXSize = SCR_LETTER_SIZE_X;
		curTagPtr->objYSize = SCR_OBJ_SIZE_Y;
		curTagPtr->tagText = textArray[i];
		curTagPtr->textArea = TagListInst->elemTextArea;

		elemYPos = elemYPos + elemHeight;
	}

	return status;

}

bool screenMainBookInit( ScreenBook *scrBookInst, short width, short height )
{
	bool status = true, status1;

	scrBookInst->bookColorInfo = defaultColorInfo;


	//���������ҵ������Χ
	short leftAreaWidth = width/3;
	short rightAreaWidth = width/3;
	short middleAreaWidth = width - leftAreaWidth - rightAreaWidth;
	status1 = setAreaRange( &(scrBookInst->turnBackPageAreaPos), 1, leftAreaWidth, 1, height );
	status = status && status1;
	status1 = setAreaRange( &(scrBookInst->openPageInfoAreaPos), leftAreaWidth+1, leftAreaWidth+middleAreaWidth, 1, height );
	status = status && status1;
	status1 = setAreaRange( &(scrBookInst->turnNextPageAreaPos), width-rightAreaWidth+1, width, 1, height );
	status = status && status1;

	//���õ����鷶Χ�����У�
	short bookAreaHeight = BOOK_ROW_HEIGHT * BOOK_COL_NUM;
	short marginYU = ( height - bookAreaHeight )>>1;
	short marginYD = height - bookAreaHeight - marginYU;
	status1 = setAreaRange( &(scrBookInst->txtBookArea), 1, width, marginYU+1, height-marginYD );
	status = status && status1;

	//���õ������Ķ�������Ϣҳ�淶Χ
	short pageInfoAreaHeight = height/4;
	status1 = setAreaRange( &(scrBookInst->pageInfoArea), 1, width, height-pageInfoAreaHeight, height-1 );
	status = status && status1;

	//���õ�����խ����Χ�����������Ķ�������ʾʱ�������鷶Χ��խ��
	status1 = setAreaRange( &(scrBookInst->txtBookNarrowArea), 1, width, marginYU+1, height-pageInfoAreaHeight-1 );
	status = status && status1;

	//���õ������Ķ�������Ϣҳ���Ƿ�ɼ�
	scrBookInst->pageInfoVisible = false;

	TextType textArray[BOOK_COL_NUM];

	int i;
	for( i = 0; i < BOOK_COL_NUM; i ++ )
	{
		(textArray[i]).text = txtBookSpace[i];
		(textArray[i]).textLen = BOOK_ROW_BYTES;
	}

	ColorInfo colorSel;
	colorSel.bkgColor = color_form[DEFAULT_BOOK_BKG_COLOR];
	colorSel.borderColor = DEFAULT_BORDER_COLOR;
	colorSel.objColor = color_form[DEFAULT_BOOK_TEXT_COLOR];
	colorSel.borderVisible = false;
	colorSel.objVisible = true;
	//���õ������ı����ı���Ϊ��TagBlock��ɵ�TagList�ı���ǩ�б���
	TextList textList = { textArray, BOOK_COL_NUM };
	status1 = setDefaultTagList( &(scrBookInst->txtBook), width, BOOK_ROW_HEIGHT, BOOK_ROW_WIDTH,\
                              &textList, &colorSel );
	status = status && status1;


	//���õ������Ķ�������Ϣҳ��
	status1 = ScreenMainPageInfoInit( &(scrBookInst->pageInfo), width, pageInfoAreaHeight );
	status = status && status1;

	return status;
}

bool ScreenMainPageInfoInit( ScreenPageInfo *scrPageInfoInst, short width, short height )
{
	//������Ϣ
	scrPageInfoInst->bkgBorderColor = defaultColorInfo;
	scrPageInfoInst->txtFileInfo = NULL;


	bool status = true, status1;

	//������
	short prgBarMarginYU = 4;
	short prgBarHeight = height/2;
	short prgBarMarginX = 2;
	status1 = setAreaRange( &(scrPageInfoInst->prgBarArea), prgBarMarginX+1, width-prgBarMarginX, prgBarMarginYU+1,\
						prgBarMarginYU + prgBarHeight  );
    status = status && status1;


	status1 = setDefaultProgressBarX( &(scrPageInfoInst->prgBarX), width - (prgBarMarginX<<1), prgBarHeight );
	status = status && status1;

	//��̬�ı���ǩ(�߶��趨)
	short tagMarginYD = 5;
	short tagHeight = height - prgBarMarginYU - prgBarHeight - tagMarginYD;


	//���Ȱٷ�����ǩ(�����ƫ��)��ʽΪ"53.33%"
	short tagPercentMarginXL = 10;
	short tagPercWidth = 6 * SCR_LETTER_SIZE_X + 5 * DEFAULT_LETTER_SPACING + 20;//20�Ǻ���߽�ԣ��
	status1 = setAreaRange( &(scrPageInfoInst->tagPagePercTageArea), tagPercentMarginXL+1, tagPercentMarginXL+tagPercWidth,\
						height - tagMarginYD - tagHeight + 1 , height - tagMarginYD );
    status = status && status1;
    char *strPercent = (char*)malloc( 10 );
    strcpy( strPercent, "100.00%" );
	status1 = setDefaultTagBlock( &(scrPageInfoInst->tagPagePercTage), strPercent, tagPercWidth, tagHeight );
	status = status && status1;


	//����ҳ����ǩ(���Ҳ�ƫ��)��ʽΪ��" 1234/99999"
	short tagPageNumMarginXR = 10;
	short tagPsgeNumWidth = 11 * SCR_LETTER_SIZE_X + 10 * DEFAULT_LETTER_SPACING + 20;//20�Ǻ���߽�ԣ��

	status1 = setAreaRange( &(scrPageInfoInst->tagPageNumArea), width-tagPageNumMarginXR-tagPsgeNumWidth+1, width-tagPageNumMarginXR,\
						height - tagMarginYD - tagHeight + 1 , height - tagMarginYD );
    status = status && status1;
    char *strPageNum = (char*)malloc( 15 );
    strcpy( strPageNum, " 1234/99999" );
	status1 = setDefaultTagBlock( &(scrPageInfoInst->tagPageNum), strPageNum, tagPsgeNumWidth, tagHeight );
	status = status && status1;

	return status;
}

bool screenMainSettingInit( ScreenSetting *scrSettingInst, short width, short height )
{
	bool status = true, status1;

	scrSettingInst->bkgBorderColor = defaultColorInfo;

	//��ɫ��Ϣ�༭�洢��
	scrSettingInst->bkgColorIndex = DEFAULT_BOOK_BKG_COLOR;
	scrSettingInst->txtColorIndex = DEFAULT_BOOK_TEXT_COLOR;

	//����������б�ǩ��λ��
	short LeftTagMarginXL = 15;//������б�ǩ����߾�
	short LeftTextMarginInTagX = 5;//�ڱ�ǩ�ڲ����ı����ǩ����߽�ľ���
	short LeftTagGroupMarginXL = 10;//��ǩ�����߾�
	//�߶��趨
	short tag1CenterY = 60;
	short tag1HalfH = 25;
	short editBkgColorCenterY = tag1CenterY + 80;
	short editBkgColorHalfH = 19;
	short editWordColorCenterY = editBkgColorCenterY + 50;
	short editWordColorHalfH = 19;
	short editTurnPageModCenterY = editWordColorCenterY + 50;
	short editTurnPageModHalfH = 19;
	short tag2CenterY = editTurnPageModCenterY + 80;
	short tag2HalfH = 25;

	if ( tag2CenterY + tag2HalfH > height )
	{
		status = false;
	}

	scrSettingInst->autoTurnPage = false;

	//���þ�̬��ǩ��

	//"��Ϣ��ʱ"
	short textWidth1 = getWidthOfText( "��Ϣ��ʱ", 8 );//4�ֺ����ı������ؿ���
	short staticTag1XLen = (LeftTextMarginInTagX<<1)+textWidth1;
	short staticTag1XMax = LeftTagMarginXL+staticTag1XLen;

	status1 = setAreaRange( &(scrSettingInst->tag1Area), LeftTagMarginXL+1, staticTag1XMax, tag1CenterY-tag1HalfH+1, tag1CenterY+tag1HalfH );
	status = status && status1;
	status1 = setDefaultTagBlock( &(scrSettingInst->tag1), "��Ϣ��ʱ", staticTag1XLen, tag1HalfH<<1 );
    status = status && status1;

	//"�Զ���ҳ��ʱ"
	short textWidth2 = getWidthOfText( "��ҳ��ʱ", 8 );//4�ֺ����ı������ؿ���
	short staticTag2XLen = (LeftTextMarginInTagX<<1)+textWidth2;
	short staticTag2XMax = LeftTagMarginXL+staticTag2XLen;

	status1 = setAreaRange( &(scrSettingInst->tag2Area), LeftTagMarginXL+1, LeftTagMarginXL+staticTag2XLen, tag2CenterY-tag2HalfH+1, tag2CenterY+tag2HalfH );
	status = status && status1;
	status1 = setDefaultTagBlock( &(scrSettingInst->tag2), "��ҳ��ʱ", (LeftTextMarginInTagX<<1)+textWidth2, tag2HalfH<<1 );
    status = status && status1;

	//����2����λ����
	AreaFmt tempAreaFmt1;
	AreaFmt tempAreaFmt2;

	//"������ɫ","������ɫ","�Զ���ҳ"
	//����tagGroupλ��(ˮƽ�ñ߾࣬��ֱ�����ľ�)

	short twoTagMarginYInTagGroup = 3;
	short tagGroupWidth = width*2/3;
	short tagGroupHalfH = editBkgColorHalfH + twoTagMarginYInTagGroup;
	short tagGroupXSpacing = 50;//��ǩ����������ǩ��ˮƽ���

	status1 = setAreaFmt( &tempAreaFmt1, AREA_FMT_MARGIN, AREA_FMT_CENTER, \
			    LeftTagGroupMarginXL, width - tagGroupWidth - LeftTagGroupMarginXL, editBkgColorCenterY, tagGroupHalfH );
    status = status && status1;

	status1 = setAreaRangeByAreaFmt( &(scrSettingInst->editBkgColorArea), &tempAreaFmt1, width, height );
	status = status && status1;

	tempAreaFmt1.fmtValY.fmtCenterVal.center = editWordColorCenterY;
	status1 = setAreaRangeByAreaFmt( &(scrSettingInst->editWordColorArea), &tempAreaFmt1, width, height );
	status = status && status1;

	tempAreaFmt1.fmtValY.fmtCenterVal.center = editTurnPageModCenterY;
	status1 = setAreaRangeByAreaFmt( &(scrSettingInst->editTurnPageModArea), &tempAreaFmt1, width, height );
	status = status && status1;

	//����tagGroup����
	//��ߵľ�̬��ǩ��Χ
	short leftTagMarginInGroupXL = LeftTagMarginXL - LeftTagGroupMarginXL;
	status1 = setAreaFmt( &tempAreaFmt1, AREA_FMT_MARGIN, AREA_FMT_CENTER, \
				leftTagMarginInGroupXL, tagGroupWidth - leftTagMarginInGroupXL - staticTag1XLen, tagGroupHalfH, editBkgColorHalfH );
	status = status && status1;
	//�ұߵĶ�̬��ǩ��Χ
	short rightTagMarginInGroupXL = leftTagMarginInGroupXL + staticTag1XLen + tagGroupXSpacing;
	short rightTagWidth = 50;//Ҫ��֤ȡֵ�����ı�"off"�Ŀ��ȣ���3*SRC_LETTER_SIZE_X+2*DEFAULT_LETTER_SPACING
	status1 = setAreaFmt( &tempAreaFmt2, AREA_FMT_MARGIN, AREA_FMT_CENTER, \
				rightTagMarginInGroupXL, tagGroupWidth - rightTagMarginInGroupXL - rightTagWidth, tagGroupHalfH, editBkgColorHalfH );
    status = status && status1;

	//���ñ�ǩ��
	status1 = setDefaultTagGroup( &(scrSettingInst->editBkgColor), &tempAreaFmt1, &tempAreaFmt2, \
				tagGroupWidth, tagGroupHalfH<<1,"������ɫ", (void*)0 );
    status = status && status1;

	tempAreaFmt1.fmtValY.fmtCenterVal.center = tagGroupHalfH;
	tempAreaFmt1.fmtValY.fmtCenterVal.half = editWordColorHalfH;
	tempAreaFmt2.fmtValY.fmtCenterVal.center = tagGroupHalfH;
	tempAreaFmt2.fmtValY.fmtCenterVal.half = editWordColorHalfH;
	status = setDefaultTagGroup( &(scrSettingInst->editWordColor), &tempAreaFmt1, &tempAreaFmt2, \
				tagGroupWidth, tagGroupHalfH<<1,"������ɫ", (void*)0 );
    status = status && status1;

	tempAreaFmt1.fmtValY.fmtCenterVal.half = editTurnPageModHalfH;
	tempAreaFmt2.fmtValY.fmtCenterVal.half = editTurnPageModHalfH;
	status1 = setDefaultTagGroup( &(scrSettingInst->editTurnPageMod), &tempAreaFmt1, &tempAreaFmt2, \
				tagGroupWidth, tagGroupHalfH<<1,"�Զ���ҳ", "off" );
    status = status && status1;



	//���ÿɱ༭������
	short numEditGrpWidth = 90;
	short numEditGrpHeight = 110;

	short tag1NumHourSpacingX = 20;
	short numHourNumMinuteSpacingX = 15;

	short tag2NumSecondSpacingX = 20;


	//���ӵ�"ʱ"(Ĭ��Ϊ1ʱ)
	short alarmHourMarginXL = staticTag1XMax+tag1NumHourSpacingX;
	status1 = setAreaRange( &(scrSettingInst->editAlarmHourArea), alarmHourMarginXL + 1, alarmHourMarginXL + numEditGrpWidth, \
				tag1CenterY - (numEditGrpHeight>>1)+1, tag1CenterY + (numEditGrpHeight>>1) );
    status = status && status1;
	status1 = setDefaultNumEditGroup( &(scrSettingInst->editAlarmHour), numEditGrpWidth, numEditGrpHeight, 23, 0, 1, "ʱ" );
	status = status && status1;

	//���ӵ�"��"(Ĭ��Ϊ0��)
	short alarmMinuteMarginXL = staticTag1XMax + tag1NumHourSpacingX + numEditGrpWidth + numHourNumMinuteSpacingX;
	status1 = setAreaRange( &(scrSettingInst->editAlarmMinuteArea), alarmMinuteMarginXL+1, alarmMinuteMarginXL+numEditGrpWidth, \
				tag1CenterY - (numEditGrpHeight>>1)+1, tag1CenterY + (numEditGrpHeight>>1) );
	status = status && status1;

	status1 = setDefaultNumEditGroup( &(scrSettingInst->editAlarmMinute), numEditGrpWidth, numEditGrpHeight, 59, 0, 0, "��" );
    status = status && status1;

	//�Զ���ҳ��ʱ��"��"(Ĭ��Ϊ8��)
	short trunPageSecMarginXL = staticTag2XMax+tag2NumSecondSpacingX;
	status1 = setAreaRange( &(scrSettingInst->editTurnPageSecArea), trunPageSecMarginXL+1, trunPageSecMarginXL+numEditGrpWidth, \
				tag2CenterY - (numEditGrpHeight>>1)+1, tag2CenterY + (numEditGrpHeight>>1) );
    status = status && status1;
	status1 = setDefaultNumEditGroup( &(scrSettingInst->editTurnPageSec), numEditGrpWidth, numEditGrpHeight, 30, 0, 8, "��" );
    status = status && status1;

	return status;
}
bool screenMainColorPickerInit( ScreenColorPicker *scrColorPickerInst, short width, short height )
{
	scrColorPickerInst->bkgBorderColor = defaultColorInfo;

//printf( "haha = %d\n", (scrColorPickerInst->bkgBorderColor).objColor );
	bool status = true, status1;
	if ( height < width )
	{
		status = false;
	}
	short marginYU = (height-width) >> 1;//�ϱ߾�

	status1 = setAreaRange( &(scrColorPickerInst->colorPickerArea), 1, width, marginYU + 1, height - marginYU );
    status = status && status1;

//	setDefaultColorBoard( &(scrColorPickerInst->colorPicker), (void*)0, width, width );
    color_u8 *colorSel = malloc( sizeof( color_u8) );
    *colorSel = 32;
    setDefaultColorBoard( &(scrColorPickerInst->colorPicker), colorSel, width, width );

	return status;
}

void screenMainAreaInfoInst( ScrMainAreaInfo *mainAreaInfoInst )
{
	//���õ�ǰҳ��Ϊ��һҳ
	mainAreaInfoInst->curScrIndex = 0;
	//���õ�һҳΪ��ҳ
	(mainAreaInfoInst->scrIDStack)[0] = SCR_HOME;


	//��պ����ջҳ�������治����ջҳ��
	int i;
	for ( i = 1; i < 4; i ++ )
	{
		(mainAreaInfoInst->scrIDStack)[i] = SCR_NONE;
	}
}

//�����Զ������Ļ�ṹ������(�����Զ��������޸Ķ���������У����ڹ���)
void screenSetCustomInfo( ScreenContainer *screenContainerInst )
{

//1.����
    ScreenHeaderTime *scrHeaderInst = &(screenContainerInst->scrHeader);

    //1.0 bkgBorderColor
    scrHeaderInst->bkgBorderColor.bkgColor = getColor16BitByRGB( 210, 125, 33 );
    scrHeaderInst->bkgBorderColor.borderColor = getColor16BitByRGB( 48, 216, 39 );

    //1.1 tagTime(ʱ���ǩ)
    scrHeaderInst->tagTime.tagColorInfo.bkgColor = getColor16BitByRGB( 52, 92, 85 );
    scrHeaderInst->tagTime.tagColorInfo.borderColor = getColor16BitByRGB( 168, 220, 97 );
    scrHeaderInst->tagTime.tagColorInfo.objColor = getColor16BitByRGB( 9, 255, 211 );
//    scrHeaderInst->tagTime.tagColorInfo.borderVisible = false;


//2.����
	ScreenFooterBtn *scrFooterInst = &(screenContainerInst->scrFooter);
	scrFooterInst->bkgBorderColor.bkgColor = getColor16BitByRGB( 187, 136, 68 );
	scrFooterInst->bkgBorderColor.borderColor = getColor16BitByRGB( 48, 216, 39 );


	scrFooterInst->iconBack.iconColorInfo.bkgColor = getColor16BitByRGB( 83, 94, 51 );
	scrFooterInst->iconHome.iconColorInfo.bkgColor = getColor16BitByRGB( 83, 94, 51 );
	scrFooterInst->iconSetting.iconColorInfo.bkgColor = getColor16BitByRGB( 83, 94, 51 );

//3.HOMEҳ��
	ScreenHome *scrHomeInst = &(screenContainerInst->scrHome);
	scrHomeInst->bkgBorderColor.bkgColor = getColor16BitByRGB( 83, 94, 51 );
	scrHomeInst->turnCatalogBarY.barYColorInfo.bkgColor = getColor16BitByRGB( 218, 242, 254 );
	scrHomeInst->turnCatalogBarY.iconMoveUp.iconColorInfo.bkgColor = getColor16BitByRGB( 119, 202, 200 );
	scrHomeInst->turnCatalogBarY.iconMoveDown.iconColorInfo.bkgColor = getColor16BitByRGB( 119, 202, 200 );
	scrHomeInst->turnCatalogBarY.iconBar.iconColorInfo.bkgColor = getColor16BitByRGB( 158, 172, 155 );
	scrHomeInst->listYOffset = scrHomeInst->listYStepSize * 2.5;
	scrHomeInst->turnCatalogBarY.barYoffset = \
                scrHomeInst->turnCatalogBarY.barYOffsetMax * scrHomeInst->listYOffset / scrHomeInst->listYOffsetMax;
//	scrHomeInst->listYOffset = 17;

//4.BOOKҳ��
	ScreenBook *scrBookInst = &(screenContainerInst->scrBook);
    scrBookInst->pageInfoVisible = true;//��pageInfo������Ч��
    scrBookInst->pageInfo.bkgBorderColor.bkgColor = getColor16BitByRGB( 83, 94, 51 );
    scrBookInst->pageInfo.prgBarX.bkgBorderColor.bkgColor = getColor16BitByRGB( 168, 220, 97 );
    scrBookInst->pageInfo.prgBarX.axis.iconColorInfo.bkgColor = getColor16BitByRGB( 91, 91, 255 );
    scrBookInst->pageInfo.prgBarX.dot.iconColorInfo.bkgColor = getColor16BitByRGB( 7, 56, 56 );
    scrBookInst->pageInfo.tagPageNum.tagColorInfo.bkgColor = getColor16BitByRGB( 241, 225, 27 );
    scrBookInst->pageInfo.tagPagePercTage.tagColorInfo.bkgColor = getColor16BitByRGB( 21, 247, 89 );
    scrBookInst->pageInfo.prgBarX.xOffset = scrBookInst->pageInfo.prgBarX.xOffsetMax * 0.33;

    scrBookInst->pageInfo.txtFileInfo = txtFilesInfoSpace.txtFileList + 0;


//5.SETTINGҳ��
	ScreenSetting *scrSettingInst = &(screenContainerInst->scrSetting);

	//5.0 bkgBorderColor
	scrSettingInst->bkgBorderColor.bkgColor = getColor16BitByRGB( 69, 210, 186 );

	//5.1 editAlarmHour
	scrSettingInst->editAlarmHour.bkgBorderColor.bkgColor = getColor16BitByRGB( 227, 210, 57 );

    scrSettingInst->editAlarmHour.plusIcon.iconColorInfo.bkgColor = getColor16BitByRGB( 53, 116, 232 );
	scrSettingInst->editAlarmHour.minusIcon.iconColorInfo.bkgColor = getColor16BitByRGB( 53, 116, 232 );
	scrSettingInst->editAlarmHour.valueTag.tagColorInfo.bkgColor = getColor16BitByRGB( 53, 116, 232 );
	scrSettingInst->editAlarmHour.unitTag.tagColorInfo.bkgColor = getColor16BitByRGB( 53, 116, 232 );

	//5.2 editAlarmMinute
	scrSettingInst->editAlarmMinute.bkgBorderColor.bkgColor = getColor16BitByRGB( 227, 210, 57 );

    scrSettingInst->editAlarmMinute.plusIcon.iconColorInfo.bkgColor = getColor16BitByRGB( 53, 116, 232 );
	scrSettingInst->editAlarmMinute.minusIcon.iconColorInfo.bkgColor = getColor16BitByRGB( 53, 116, 232 );
	scrSettingInst->editAlarmMinute.valueTag.tagColorInfo.bkgColor = getColor16BitByRGB( 53, 116, 232 );
	scrSettingInst->editAlarmMinute.unitTag.tagColorInfo.bkgColor = getColor16BitByRGB( 53, 116, 232 );

	//5.3 editBkgColor
	scrSettingInst->editBkgColor.bkgBorderColor.bkgColor = getColor16BitByRGB( 53, 116, 232 );

	scrSettingInst->editBkgColor.staticTag.tagColorInfo.bkgColor = getColor16BitByRGB( 239, 128, 46 );
	alt_u8 colorIndex = scrSettingInst->bkgColorIndex;
	scrSettingInst->editBkgColor.varTag.tagColorInfo.bkgColor = color_form[colorIndex];


	//5.4 editWordColor
	scrSettingInst->editWordColor.bkgBorderColor.bkgColor = getColor16BitByRGB( 53, 116, 232 );

    scrSettingInst->editWordColor.staticTag.tagColorInfo.bkgColor = getColor16BitByRGB( 239, 128, 46 );
    colorIndex = scrSettingInst->txtColorIndex;
	scrSettingInst->editWordColor.varTag.tagColorInfo.bkgColor = color_form[colorIndex];


	//5.5 editTurnPageMod
	scrSettingInst->editTurnPageMod.bkgBorderColor.bkgColor = getColor16BitByRGB( 53, 116, 232 );

    scrSettingInst->editTurnPageMod.staticTag.tagColorInfo.bkgColor = getColor16BitByRGB( 239, 128, 46 );
	scrSettingInst->editTurnPageMod.varTag.tagColorInfo.bkgColor = getColor16BitByRGB( 53, 116, 232 );


	//5.6  editTurnPageSec
	scrSettingInst->editTurnPageSec.bkgBorderColor.bkgColor = getColor16BitByRGB( 227, 210, 57 );

    scrSettingInst->editTurnPageSec.plusIcon.iconColorInfo.bkgColor = getColor16BitByRGB( 53, 116, 232 );
	scrSettingInst->editTurnPageSec.minusIcon.iconColorInfo.bkgColor = getColor16BitByRGB( 53, 116, 232 );
	scrSettingInst->editTurnPageSec.valueTag.tagColorInfo.bkgColor = getColor16BitByRGB( 53, 116, 232 );
	scrSettingInst->editTurnPageSec.unitTag.tagColorInfo.bkgColor = getColor16BitByRGB( 53, 116, 232 );




	//5.7 tag1("��Ϣ��ʱ")
	scrSettingInst->tag1.tagColorInfo.bkgColor = getColor16BitByRGB( 53, 116, 232 );
	scrSettingInst->tag1.tagColorInfo.bkgColor = getColor16BitByRGB( 239, 128, 46 );


	//5.8 tag2("��ҳ��ʱ")
	scrSettingInst->tag2.tagColorInfo.bkgColor = getColor16BitByRGB( 53, 116, 232 );
    scrSettingInst->tag2.tagColorInfo.bkgColor = getColor16BitByRGB( 239, 128, 46 );



//6.COLOR_PICKERҳ��
	ScreenColorPicker *scrColorPickerInst = &(screenContainerInst->scrColorPicker);

	//6.0 bkgBorderColor
	scrColorPickerInst->bkgBorderColor.bkgColor = getColor16BitByRGB( 53, 116, 232 );
	scrColorPickerInst->bkgBorderColor.borderColor = getColor16BitByRGB( 53, 116, 232 );

	//6.1 ָ�����ɫ
	scrColorPickerInst->colorPicker.colorIndex = &(scrSettingInst->bkgColorIndex);

}