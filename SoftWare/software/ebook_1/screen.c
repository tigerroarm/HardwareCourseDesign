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
#include "font_manage.h"
#include "icon_pic_extern.h"



//	sprintf(str, "%02d", num);���Խ����ֱ�Ϊ�ַ���

//������Ļ����
ScreenContainer tftlcdScreenInst;

ColorInfo defaultColorInfo = {  DEFAULT_BORDER_COLOR, DEFAULT_BKG_COLOR, DEFAULT_OBJ_COLOR };

char *str1 = "Happy";

//��������Χ
bool setAreaRange( AreaRange *areaPtr, short x_min, short x_max, short y_min, short y_max )
{
	areaPtr->x_min = x_min;
	areaPtr->x_max = x_max;
	areaPtr->y_min = y_min;
	areaPtr->y_max = y_max;
	if ( x_min > x_max || y_min > y_max )
	{
		return false;
	}
	return true;
}
//����Χ�Զ�ת��
bool setAreaRangeByAreaFmt( AreaRange *areaPtr, AreaFmt *areaFmtPos, short width, short height )
{

	AreaFmtCenter *tempFmtCenterPos;
	AreaFmtMargin *tempFmtMarginPos;

	bool status = true;

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

	if ( areaPtr->x_min > areaPtr->x_max || areaPtr->y_min > areaPtr->y_max )
	{
		return false;
	}
	if ( areaPtr->x_max > width || areaPtr->y_max > height )
	{
		return false;
	}

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
	bool status = true;

	if ( width < objXSize || height < objYSize )//obj�ߴ糬��ģ�鷶Χ�������Ƕ���obj������ߴ磬objֱ��ʹ��ģ��ߴ�
	{
		setAreaRange( areaPtr, 1, width, 1, height );
		status = false;//��������������ģ��Ķ���ߴ糬���߽磩
		return status;
	}

	//����Χ����
	short marginXL = ( width - objXSize ) >> 1;//��߾�
	short marginXR = ( width - objXSize ) - marginXL;//�ұ߾�
	short marginYU = ( height - objYSize )>> 1;//�ϱ߾�
	short marginYD = ( height - objYSize ) - marginYU;//�±߾�

	status = status && setAreaRange( areaPtr, marginXL+1, width-marginXR, marginYU+1, height-marginYD );

	return status;
}
//����Ĭ�Ͼ��б�ǩ
bool setDefaultTagBlock( TagBlock *tagPtr, char *text, short width, short height )
{
	tagPtr->borderVisible = true;
	tagPtr->tagColorInfo = defaultColorInfo;
	tagPtr->wordSpacing = DEFAULT_WORD_SPACING;
	tagPtr->letterSpacing = DEFAULT_LETTER_SPACING;
	(tagPtr->tagText).text = text;

	short textLen = strlen(text);
	(tagPtr->tagText).textLen = textLen;

	if ( text == NULL )
	{
		tagPtr->textVisible = false;
	}
	else
	{
		tagPtr->textVisible = true;
	}
	short textWidthSum = getWidthOfText( text, textLen );

	bool status = setAreaRangeCentered( &(tagPtr->textArea ), width, height, textWidthSum, SRC_WORD_SIZE_Y );

	return status;
}
//����Ĭ�Ͻ�����
bool setDefaultProgressBar( ProgressBar *prgBarInst, short width, short height )
{
	bool status = true;
	//������Ϣ
	prgBarInst->bkgColor = DEFAULT_BKG_COLOR;
	prgBarInst->borderColor = DEFAULT_BORDER_COLOR;
	prgBarInst->borderVisible = true;

	prgBarInst->xOffset = 0;
	prgBarInst->xOffsetMax = 280;

	//�˶���
	short dotHeight = ICON_DOT_Y_SIZE;
	short axisHeight = dotHeight / 2;
	short axisWidth = prgBarInst->xOffsetMax + 1;
	status = status && setAreaRangeCentered( &(prgBarInst->axisArea), width, height, axisWidth, axisHeight );
	//����ó���������ı߽��������ɫ����
	status = status && setDefaultIconBlock( &(prgBarInst->axis), (void*)0, axisWidth, axisHeight, 0, 0 );

	//���ȵ����������ƫ����ʱ��
	short dotWidth = ICON_DOT_X_SIZE;
	short dotMarginXL = (prgBarInst->axisArea).x_min - dotWidth/2 - 1;
	short dotMarginYU = ( (prgBarInst->axisArea).y_min - 1 + (prgBarInst->axisArea).y_max - dotHeight ) / 2;
	status = status && setAreaRange( &(prgBarInst->dotBaseArea), dotMarginXL + 1, dotMarginXL + dotWidth,\
					dotMarginYU + 1, dotMarginYU + dotHeight);
	status = status && setDefaultIconBlock( &(prgBarInst->dot), icon_dot, axisWidth, axisHeight, dotWidth, dotHeight );


	return status;

}
//����Ĭ�ϴ�ֱ������
bool setDefaultScrollBarY( ScrollBarY *scrollBarYInst, short width, short height )
{
	bool status = true;

	/*
	 *
	 * �ú����ȴ����
	 *
	 */


	return status;
}
//����Ĭ�Ͼ���ͼ��
bool setDefaultIconBlock( IconBlock *iconPtr, const alt_u8 *iconModel, short width, short height, short iconXSize, short iconYSize )
{
	iconPtr->borderVisible = true;
	iconPtr->iconPic = iconModel;
	iconPtr->iconWidth = iconXSize;
	iconPtr->iconXSizeInByte = (iconXSize+7)>>3;//��8������ȡ��
	iconPtr->iconYSizeInByte = iconYSize;

	if ( iconModel == (void*)0 )
	{
		iconPtr->picVisible = false;
	}
	else
	{
		iconPtr->picVisible = true;
	}
	iconPtr->iconColorInfo = defaultColorInfo;

	bool status = setAreaRangeCentered( &(iconPtr->picArea ), width, height, iconXSize, iconYSize );

	return status;
}

//����ȡɫ����Ϣ(��ɫ��λ��colorPos��ʾ��Ļ��ѡ�е�λ�ã����ݸ�λ�ã���ʾ����ͻ���ÿ���ɫ�ı߽�)
void setDefaultColorTable( ColorTable *colorPickerInst, color_u8 *colorSel, short width, short height )
{
	colorPickerInst->colorIndex = colorSel;
	colorPickerInst->colorXNum = 16;//��ɫ��16��
	colorPickerInst->colorYNum = 16;//��ɫ��16��
	colorPickerInst->colorXSize = width / 16;
	colorPickerInst->colorYSize = height / 16;
	colorPickerInst->colorSpace = color_form;
}

//������Ч�ı�ȫ����ʾʱ�����ؿ���
short getWidthOfText( char *str, short len )
{
	char *strType = (char*)malloc( sizeof( char ) );
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
			if ( i == len || str[i] == 0 )//���ֽڲ����ڣ������⣬�����ı�����Ϊ0����ʾ�ı�*str����
			{
				free(strType);
				return 0;
			}
		}
		else
		{
			strType[strTypeVaildNum] = 'e';//Ӣ��ENGLISH
		}
		strTypeVaildNum += 1;
	}
	short textWidth = ( strType[0] == 'e' ) ? SRC_LETTER_SIZE_X : SRC_WORD_SIZE_X;
	for ( i = 1; i < strTypeVaildNum; i ++ )
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
			textWidth = textWidth + SRC_LETTER_SIZE_X;
		}
		else
		{
			textWidth = textWidth + SRC_WORD_SIZE_X;
		}
	}
	free(strType);
	return textWidth;
}


//����Ĭ�ϱ�ǩ��
bool setDefaultTagBlockGroup( TagBlockGroup *tagGroupInst, AreaFmt *tagStaticPos, AreaFmt *tagVarPos, short width, short height,\
			char *tagStaText, char *tagVarText )
{
	bool status = true;

	tagGroupInst->bkgColor = DEFAULT_BKG_COLOR;
	tagGroupInst->borderColor = DEFAULT_BORDER_COLOR;
	tagGroupInst->borderVisible = true;

	//���þ�̬��ǩ��λ��
	AreaRange *tempArea = &(tagGroupInst->staticTagArea);
	status = status && setAreaRangeByAreaFmt( tempArea, tagStaticPos, width, height );
	//���þ�̬��ǩ����
	status = status && setDefaultTagBlock( &(tagGroupInst->staticTag), tagStaText, \
			tempArea->x_max - tempArea->x_min + 1, tempArea->y_max - tempArea->y_max + 1);


	//���ö�̬��ǩ��λ��
	tempArea = &(tagGroupInst->varTagArea);
	status = status && setAreaRangeByAreaFmt( tempArea, tagVarPos, width, height );
	//���ö�̬��ǩ����
	status = status && setDefaultTagBlock( &(tagGroupInst->varTag), tagVarText, \
			tempArea->x_max - tempArea->x_min + 1, tempArea->y_max - tempArea->y_max + 1);

	return status;
}

//����Ĭ�Ͽɱ༭����(��̬��ǩ+��̬ͼ��)
bool setDefaultNumEditGroup( TagIconGroup *numEditGrpInst, short width, short height, short numMax, short numMin, short num, char *unitName )
{
	bool status = true;

	//���û�����Ϣ
	numEditGrpInst->bkgColor = DEFAULT_BKG_COLOR;
	numEditGrpInst->borderColor=  DEFAULT_BORDER_COLOR;
	numEditGrpInst->borderVisible = true;

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
	short twoTagSpacingX = 20;

	//����plusIcon
	status = status && setAreaRange( &(numEditGrpInst->plusIconArea), numTagMarginXL+1, numTagMarginXL+iconBlockWidth,\
				plusIconMarginYU + 1, plusIconMarginYU + iconBlockHeight );
	status = status && setDefaultIconBlock( &(numEditGrpInst->plusIcon), icon_plus, \
				iconBlockWidth, iconBlockHeight, ICON_PLUS_X_SIZE, ICON_PLUS_Y_SIZE );

	//����numberTag
	char numStr[3];
	sprintf(numStr, "%2d", num);
	status = status && setAreaRange( &(numEditGrpInst->valueTagArea), numTagMarginXL+1, numTagMarginXL+tagWidth,\
				(height>>1)-(tagHeight>>1)+1, (height>>1)+(tagHeight>>1) );
	status = status && setDefaultTagBlock( &(numEditGrpInst->valueTag), numStr, tagWidth, tagHeight );


	//����minusIcon
	status = status && setAreaRange( &(numEditGrpInst->valueTagArea), numTagMarginXL+1, numTagMarginXL+tagWidth,\
				height - minusIconMarginYD - iconBlockHeight, height - minusIconMarginYD );
	status = status && setDefaultIconBlock( &(numEditGrpInst->minusIcon), icon_minus, \
				iconBlockWidth, iconBlockHeight, ICON_MINUS_X_SIZE, ICON_MINUS_Y_SIZE );


	//����unitTag
	short unitTagMarginXL = numTagMarginXL+tagWidth + twoTagSpacingX;
	status = status && setAreaRange( &(numEditGrpInst->valueTagArea), unitTagMarginXL+1, unitTagMarginXL+tagWidth,\
					(height>>1)-(tagHeight>>1)+1, (height>>1)+(tagHeight>>1) );
	status = status && setDefaultTagBlock( &(numEditGrpInst->valueTag), unitName, tagWidth, tagHeight );


	return status;
}


//��ʼ��
bool screenInit()
{
	//������ģ����ȶ���ͬ ( 1 �� x �� SCR_WIDE )

	bool status = true;
	//��ʼ����Ļ����ߴ� ( 1 �� y �� SCR_HEIGHT )
	status = status && setAreaRange( &(tftlcdScreenInst.screenArea), 1, SCR_WIDTH, 1, SCR_HEIGHT );

	//1.��ʼ������ ( 1 �� y �� headerHeight )
	short headerHeight = SCR_HEADER_HEIGHT;//�����߶� (
	status = status && setAreaRange( &(tftlcdScreenInst.headerArea), 1, SCR_WIDTH, 1, headerHeight );

	status = status && screenHeaderInit( &(tftlcdScreenInst.scrHeader), SCR_WIDTH, headerHeight );

	//2.��ʼ������ ( SCR_HEIGHT - footerHeight + 1 �� y �� SCR_HEIGHT )
	short footerHeight = SCR_FOTTER_HEIGHT;//�����߶�
	status = status && setAreaRange( &(tftlcdScreenInst.headerArea), 1, SCR_WIDTH, SCR_HEIGHT - footerHeight + 1, SCR_HEIGHT );

	status = status && screenFooterInit( &(tftlcdScreenInst.scrFooter), SCR_WIDTH, footerHeight );

	//3.��ʼ������  ( headerHeight + 1 �� y �� SCR_HEIGHT - footerHeight )
	short mainAreaHeight = SCR_HEIGHT - headerHeight - footerHeight;
	setAreaRange( &(tftlcdScreenInst.mainArea), 1, SCR_WIDTH, headerHeight + 1, SCR_HEIGHT - footerHeight );

	status = status && screenMainHomeInit( &(tftlcdScreenInst.scrHome), SCR_WIDTH, mainAreaHeight, &txtFilesInfoSpace );
	status = status && screenMainBookInit( &(tftlcdScreenInst.scrBook), SCR_WIDTH, mainAreaHeight );
	status = status && screenMainSettingInit( &(tftlcdScreenInst.scrSetting), SCR_WIDTH, mainAreaHeight );
	status = status && screenMainColorPickerInit( &(tftlcdScreenInst.scrColorPicker), SCR_WIDTH, mainAreaHeight );

	//4.��ʼ������ҳ����Ϣ
	screenMainAreaInfo( &(tftlcdScreenInst.mainAreaInfo) );

	return status;
}

bool screenHeaderInit( ScreenHeaderTime *scrHeaderInst, short width, short height )
{

	scrHeaderInst->borderVisible = true;
	scrHeaderInst->bkgColor = DEFAULT_BKG_COLOR;
	scrHeaderInst->borderColor = DEFAULT_BORDER_COLOR;

	short marginX = 100;//����߾�
	short marginY = 5;//����߾�

	bool status = true;

	status = status && setAreaRange( &(scrHeaderInst->tagTimeArea), marginX+1, width-marginX, marginY+1, height-marginY );

	scrHeaderInst->timeHour = 0;
	scrHeaderInst->timeMinute = 0;
	scrHeaderInst->timeSecond = 0;

	//ʱ���ı�Ϊ"00:00"(0ʱ0��)
	status = status && setDefaultTagBlock( &(scrHeaderInst->tagTime), "00:00", width - (marginX<<1), height - (marginY<<1) );
//	(scrHeaderInst->tagTime).borderVisible = false;

	return status;
}
bool screenFooterInit( ScreenFooterBtn *scrFooterInst, short width, short height )
{
	scrFooterInst->borderVisible = true;
	scrFooterInst->borderColor = DEFAULT_BORDER_COLOR;
	scrFooterInst->bkgColor = DEFAULT_BKG_COLOR;

	//SETTING��,HOME��,BACK���ֱ����±��������С���λ��
	short marginX = 20;
	short marginY = 10;
	short iconBlockHeight = height - (marginY<<1);
	short iconBlockWidth = width / 5;

	bool status = true;

	status = status && setAreaRange( &(scrFooterInst->iconSettingArea), marginX+1, marginX+iconBlockWidth, marginY+1, height - marginY );
	status = status && setAreaRangeCentered( &(scrFooterInst->iconHomeArea), width, height, iconBlockWidth, iconBlockHeight );
	status = status && setAreaRange( &(scrFooterInst->iconSettingArea), width-marginX-iconBlockWidth+1, width-marginX, marginY+1, height - marginY );


	status = status && setDefaultIconBlock( &(scrFooterInst->iconSetting), icon_setting, iconBlockWidth, iconBlockHeight, ICON_SETTING_X_SIZE, ICON_SETTING_Y_SIZE );
	status = status && setDefaultIconBlock( &(scrFooterInst->iconHome), icon_setting, iconBlockWidth, iconBlockHeight, ICON_HOME_X_SIZE, ICON_HOME_Y_SIZE );
	status = status && setDefaultIconBlock( &(scrFooterInst->iconBack), icon_setting, iconBlockWidth, iconBlockHeight, ICON_BACK_X_SIZE, ICON_BACK_Y_SIZE );

	return status;
}

bool screenMainHomeInit( ScreenHome *scrHomeInst, short width, short height, TxtFilesInfo *txtFilesInfoPtr )
{
	scrHomeInst->borderVisible = true;
	scrHomeInst->borderColor = DEFAULT_BORDER_COLOR;
	scrHomeInst->bkgColor = DEFAULT_BKG_COLOR;

	bool status = true;

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
	TextList *numListPtr;
	TextList *catalogListPtr;
	/*
	 *
	 *
	 *
	 * �˴�Ҫ����������TextList����malloc��Ҫ�ռ䣬����txtFilesInfoPtr�е���Ч��Ϣ���浽��Ҫ�Ŀռ���
	 *
	 *
	 */


	//���ֱ���б�
	status = status && setAreaRange( &(scrHomeInst->numBarArea), numListMarginXL+1, numListMarginXL+numListWidth,\
							blockMarginY+1, height-blockMarginY );
	status = status && setDefaultTagList( &(scrHomeInst->numBar), numListWidth, listElemNum, numListPtr, true,\
							&defaultColorInfo );
	//txt�ļ�Ŀ¼�б�
	status = status && setAreaRange( &(scrHomeInst->txtCatalogArea), catalogListMarginXL+1, catalogListMarginXL+catalogListWidth,\
							blockMarginY+1, height-blockMarginY );
	status = status && setDefaultTagList( &(scrHomeInst->txtCatalog), numListWidth, listElemNum, catalogListPtr, true,\
							&defaultColorInfo );

	//������
	status = status && setAreaRange( &(scrHomeInst->turnCatalogBarArea), scrollBarMarginXL + 1, scrollBarMarginXL + scrollBarWidth,\
							blockMarginY+1, height-blockMarginY );
	status = status && setDefaultScrollBarY( &(scrHomeInst->turnCatalogBarY), scrollBarWidth, blockHeight );


	//�б������ƶ���ʾ����
	scrHomeInst->listYOffset = 0;//�б���ʾʱ������ƫ����
	scrHomeInst->listYSize = listElemNum * listElemHeight;//�б�����ȫ��
	scrHomeInst->listYOffsetMax = scrHomeInst->listYSize - blockHeight;
	scrHomeInst->listYStepSize = CATALOG_ROW_HEIGHT;


	//ĳ��Ŀ¼�ļ���ˮƽ��������
	scrHomeInst->txtScrollable = false;//����������
	scrHomeInst->txtScrollrow = 0;//�ɹ�����ĳ��������һ��ֻ��һ�п��Թ�����
	scrHomeInst->txtScrollxOffset = 0;//��ǰ����ƫ����
	scrHomeInst->txtScrollwidth = 0;//�����ı�����ʵ���ؿ��ȣ����Գ�����Ļ���ȣ�
	scrHomeInst->txtScrollSpacing = width / 2;//�����ı�ѭ��ʱ�ı�β�����ı�ͷ�ļ��


	return status;
}
//����Ĭ���ı��б�
bool setDefaultTagList( TagList *TagListInst, short elemWidth, short elemHeight, TextList *textListPtr, bool elemBorderVisible,\
		                 ColorInfo *colorInfoPtr )
{
	//�����б�Ԫ�������С
	bool status = true;
	status = status && setAreaRange( &(TagListInst->elemBaseArea), 1, elemWidth, 1, elemHeight );
	//�����ı���������
	//�����ı�����(ʹ�����ĳߴ������ƣ�
	short textWidth = ( BOOK_ROW_BYTES >> 1) * ( SRC_WORD_SIZE_X + DEFAULT_WORD_SPACING );
	short marginXL = ( elemWidth - textWidth )>>1;
	short marginXR = elemWidth - marginXL - textWidth;
	short marginYU = ( elemHeight - SRC_WORD_SIZE_Y )>>1;
	short marginYD = elemHeight - marginYU - SRC_WORD_SIZE_Y;

	if ( elemWidth < textWidth || elemHeight < SRC_WORD_SIZE_Y )
	{
		marginXL = 0;
		marginXR = 0;
		marginYU = 0;
		marginYD = 0;
		status = false;
	}
	//����Ԫ���ı�����λ��
	status = status && setAreaRange( &(TagListInst->textArea), marginXL+1, elemWidth-marginXR, marginYU+1, elemHeight-marginYD );

	short elemNum = textListPtr->textNum;

	TagListInst->wordSpacing = DEFAULT_WORD_SPACING;
	TagListInst->letterSpacing = DEFAULT_LETTER_SPACING;

	TagListInst->elemNum = elemNum;
	TagListInst->elemBorderVisible = elemBorderVisible;
	TagListInst->elemColorInfo = *colorInfoPtr;

	int i;

	AreaRange *elemAreaArray = (AreaRange*)malloc( sizeof( AreaRange ) * elemNum );
	TagBlock *tagBolckArray = (TagBlock*)malloc( sizeof( TagBlock ) * elemNum );

	AreaRange *curElemAreaPtr;
	TagBlock *curTagPtr;
	TextType *textArray = textListPtr->textArray;

	short elemYPos = 0;
	for ( i = 0; i < elemNum; i ++ )
	{
		curTagPtr = &(tagBolckArray[i]);
		curElemAreaPtr = &(elemAreaArray[i]);

		curTagPtr->borderVisible = TagListInst->elemBorderVisible;
		curTagPtr->tagColorInfo = TagListInst->elemColorInfo;
		curTagPtr->wordSpacing = TagListInst->wordSpacing;
		curTagPtr->letterSpacing = TagListInst->letterSpacing;
		curTagPtr->tagText = textArray[i];
		curTagPtr->textVisible = true;
		curTagPtr->textArea = TagListInst->textArea;
		setAreaRange( curElemAreaPtr, 1, elemWidth, elemYPos + 1, elemYPos + elemHeight );
		elemYPos = elemYPos + elemHeight;
	}

	return status;

}

bool screenMainBookInit( ScreenBook *scrBookInst, short width, short height )
{
	bool status = true;

	scrBookInst->borderVisible = true;
	scrBookInst->borderColor = DEFAULT_BORDER_COLOR;
	//�ɱ༭����ɫ�������鱳����ɫ��������ɫ��
	scrBookInst->bkgColorIndex = 0;
	scrBookInst->txtColorIndex = 255;

	//���������ҵ������Χ
	short leftAreaWidth = width/3;
	short rightAreaWidth = width/3;
	short middleAreaWidth = width - leftAreaWidth - rightAreaWidth;
	status = status && setAreaRange( &(scrBookInst->turnBackPageAreaPos), 1, leftAreaWidth, 1, height );
	status = status && setAreaRange( &(scrBookInst->openPageInfoAreaPos), leftAreaWidth+1, leftAreaWidth+middleAreaWidth, 1, height );
	status = status && setAreaRange( &(scrBookInst->turnBackPageAreaPos), width-rightAreaWidth+1, width, 1, height );

	//���õ����鷶Χ�����У�
	short bookAreaHeight = BOOK_ROW_HEIGHT * BOOK_COL_NUM;
	short marginYU = ( height - bookAreaHeight )>>1;
	short marginYD = height - bookAreaHeight - marginYU;
	status = status && setAreaRange( &(scrBookInst->txtBookArea), 1, width, marginYU+1, height-marginYD );

	//���õ������Ķ�������Ϣҳ�淶Χ
	short pageInfoAreaHeight = height/4;
	status = status && setAreaRange( &(scrBookInst->pageInfoArea), 1, width, height-pageInfoAreaHeight, height-1 );

	//���õ������Ķ�������Ϣҳ���Ƿ�ɼ�
	scrBookInst->pageInfoVisable = false;

	TextType textArray[BOOK_COL_NUM];

	int i;
	for( i = 0; i < BOOK_COL_NUM; i ++ )
	{
		(textArray[i]).text = txtBookSpace[i];
		(textArray[i]).textLen = BOOK_ROW_BYTES;
	}

	ColorInfo colorSel = { DEFAULT_BORDER_COLOR, color_form[scrBookInst->bkgColorIndex], color_form[scrBookInst->txtColorIndex] };
	//���õ������ı����ı���Ϊ��TagBlock��ɵ�TagList�ı��б���
	TextList textList = { textArray, BOOK_COL_NUM };
	status = status && setDefaultTagList( &(scrBookInst->txtBook), width, BOOK_ROW_HEIGHT, &textList, false, &colorSel );


	//���õ������Ķ�������Ϣҳ��
	status = status && ScreenMainPageInfoInit( &(scrBookInst->pageInfo), width, pageInfoAreaHeight );

	return status;
}

bool ScreenMainPageInfoInit( ScreenPageInfo *scrPageInfoInst, short width, short height )
{
	//������Ϣ
	scrPageInfoInst->bkgColor = DEFAULT_BKG_COLOR;
	scrPageInfoInst->borderColor = DEFAULT_BORDER_COLOR;
	scrPageInfoInst->borderVisible = true;
	scrPageInfoInst->curPageNum = 0;
	scrPageInfoInst->curPagePercent = 0;
	scrPageInfoInst->totalPageNum = 0;

	bool status = true;

	//������
	short prgBarMarginYU = 4;
	short prgBarHeight = height/2;
	short prgBarMarginX = 2;
	status = status && setAreaRange( &(scrPageInfoInst->prgBarArea), prgBarMarginX+1, width-prgBarMarginX, prgBarMarginYU+1,\
						prgBarMarginYU + prgBarHeight  );
	status = status && setDefaultProgressBar( &(scrPageInfoInst->prgBarX), width - (prgBarMarginX<<1), prgBarHeight );

	//��̬�ı���ǩ(�߶��趨)
	short tagHeight = ( height - prgBarMarginYU - prgBarHeight );
	short tagMarginYD = 5;

	//���Ȱٷ�����ǩ(�����ƫ��)��ʽΪ"53.33%"
	short tagPercentMarginXL = 10;
	short tagPercWidth = 6 * SRC_LETTER_SIZE_X + 5 * DEFAULT_LETTER_SPACING + 20;//20�Ǻ���߽�ԣ��
	status = status && setAreaRange( &(scrPageInfoInst->tagPagePercTageArea), tagPercentMarginXL+1, tagPercentMarginXL+tagPercWidth,\
						height - tagMarginYD - tagHeight + 1 , height - tagMarginYD );
	status = status && setDefaultTagBlock( &(scrPageInfoInst->tagPagePercTage), "33.33%", tagPercWidth, tagHeight );


	//����ҳ����ǩ(���Ҳ�ƫ��)��ʽΪ��" 1234/99999"
	short tagPageNumMarginXR = 10;
	short tagPsgeNumWidth = 11 * SRC_LETTER_SIZE_X + 10 * DEFAULT_LETTER_SPACING + 20;//20�Ǻ���߽�ԣ��

	status = status && setAreaRange( &(scrPageInfoInst->tagPageNumArea), width-tagPageNumMarginXR-tagPsgeNumWidth+1, width-tagPageNumMarginXR,\
						height - tagMarginYD - tagHeight + 1 , height - tagMarginYD );
	status = status && setDefaultTagBlock( &(scrPageInfoInst->tagPageNum), "33.33%", tagPsgeNumWidth, tagHeight );

	return status;
}

bool screenMainSettingInit( ScreenSetting *scrSettingInst, short width, short height )
{
	bool status = true;

	scrSettingInst->borderVisible = true;

	scrSettingInst->bkgColor = DEFAULT_BKG_COLOR;
	scrSettingInst->borderColor = DEFAULT_BORDER_COLOR;

	//����������б�ǩ��λ��
	short LeftTagMarginXL = 20;//������б�ǩ����߾�
	short LeftTextMarginInTagX = 15;//�ڱ�ǩ�ڲ����ı����ǩ����߽�ľ���
	short LeftTagGroupMarginXL = 10;//��ǩ�����߾�
	//�߶��趨
	short tag1CenterY = 60;
	short tag1HalfH = 25;
	short editBkgColorCenterY = tag1CenterY + 70;
	short editBkgColorHalfH = 25;
	short editWordColorCenterY = editBkgColorCenterY + 60;
	short editWordColorHalfH = 25;
	short editTurnPageModCenterY = editWordColorCenterY + 60;
	short editTurnPageModHalfH = 25;
	short tag2CenterY = editTurnPageModCenterY + 70;
	short tag2HalfH = 25;

	if ( tag2CenterY + tag2HalfH > height )
	{
		status = false;
	}

	//���þ�̬��ǩ��

	//"��Ϣ��ʱ"
	short textWidth1 = getWidthOfText( "��Ϣ��ʱ", 8 );//4�ֺ����ı������ؿ���
	short staticTag1XLen = (LeftTextMarginInTagX<<1)+textWidth1;
	short staticTag1XMax = LeftTagMarginXL+staticTag1XLen;

	status = status && setAreaRange( &(scrSettingInst->tag1Area), LeftTagMarginXL+1, staticTag1XMax, tag1CenterY-tag1HalfH+1, tag1CenterY+tag1HalfH );
	status = status && setDefaultTagBlock( &(scrSettingInst->tag1), "��Ϣ��ʱ", staticTag1XLen, tag1HalfH<<1 );


	//"�Զ���ҳ��ʱ"
	short textWidth2 = getWidthOfText( "�Զ���ҳ��ʱ", 12 );//6�ֺ����ı������ؿ���
	short staticTag2XLen = (LeftTextMarginInTagX<<1)+textWidth2;
	short staticTag2XMax = LeftTagMarginXL+staticTag2XLen;

	status = status && setAreaRange( &(scrSettingInst->tag1Area), LeftTagMarginXL+1, LeftTagMarginXL+staticTag2XLen, tag2CenterY-tag2HalfH+1, tag2CenterY+tag2HalfH );
	status = status && setDefaultTagBlock( &(scrSettingInst->tag1), "�Զ���ҳ��ʱ", (LeftTextMarginInTagX<<1)+textWidth2, tag2HalfH<<1 );

	//����2����λ����
	AreaFmt tempAreaFmt1;
	AreaFmt tempAreaFmt2;

	//"������ɫ","������ɫ","�Զ���ҳ"
	//����tagGroupλ��(ˮƽ�ñ߾࣬��ֱ�����ľ�)

	short tagGroupWidth = width*2/3;
	short tagGroupHalfH = editBkgColorHalfH + 5;
	short tagGroupXSpacing = 50;//��ǩ����������ǩ��ˮƽ���

	status = status && setAreaFmt( &tempAreaFmt1, AREA_FMT_MARGIN, AREA_FMT_CENTER, \
			    LeftTagGroupMarginXL, width - tagGroupWidth - LeftTagGroupMarginXL, editBkgColorCenterY, tagGroupHalfH );

	setAreaRangeByAreaFmt( &(scrSettingInst->editBkgColorArea), &tempAreaFmt1, width, height );
	tempAreaFmt1.fmtValY.fmtCenterVal.center = editWordColorCenterY;
	setAreaRangeByAreaFmt( &(scrSettingInst->editWordColorArea), &tempAreaFmt1, width, height );
	tempAreaFmt1.fmtValY.fmtCenterVal.center = editTurnPageModCenterY;
	setAreaRangeByAreaFmt( &(scrSettingInst->editTurnPageModArea), &tempAreaFmt1, width, height );

	//����tagGroup����
	//��ߵľ�̬��ǩ��Χ
	short leftTagMarginInGroupXL = LeftTagMarginXL - LeftTagGroupMarginXL;
	status = status && setAreaFmt( &tempAreaFmt1, AREA_FMT_MARGIN, AREA_FMT_CENTER, \
				leftTagMarginInGroupXL, tagGroupWidth - leftTagMarginInGroupXL - staticTag1XLen, editBkgColorCenterY, editBkgColorHalfH );
	//�ұߵĶ�̬��ǩ��Χ
	short rightTagMarginInGroupXL = leftTagMarginInGroupXL + staticTag1XLen + tagGroupXSpacing;
	short rightTagWidth = 50;//Ҫ��֤ȡֵ�����ı�"off"�Ŀ��ȣ���3*SRC_LETTER_SIZE_X+2*DEFAULT_LETTER_SPACING
	status = status && setAreaFmt( &tempAreaFmt2, AREA_FMT_MARGIN, AREA_FMT_CENTER, \
				rightTagMarginInGroupXL, tagGroupWidth - leftTagMarginInGroupXL - rightTagWidth, editBkgColorCenterY, editBkgColorHalfH );

	//���ñ�ǩ��
	status = status && setDefaultTagBlockGroup( &(scrSettingInst->editBkgColor), &tempAreaFmt1, &tempAreaFmt2, \
				tagGroupWidth, tagGroupHalfH<<1,"������ɫ", (void*)0 );

	tempAreaFmt1.fmtValY.fmtCenterVal.half = editWordColorHalfH;
	tempAreaFmt2.fmtValY.fmtCenterVal.half = editWordColorHalfH;
	status = status && setDefaultTagBlockGroup( &(scrSettingInst->editWordColor), &tempAreaFmt1, &tempAreaFmt2, \
				tagGroupWidth, tagGroupHalfH<<1,"������ɫ", (void*)0 );

	tempAreaFmt1.fmtValY.fmtCenterVal.half = editTurnPageModHalfH;
	tempAreaFmt2.fmtValY.fmtCenterVal.half = editTurnPageModHalfH;
	status = status && setDefaultTagBlockGroup( &(scrSettingInst->editTurnPageMod), &tempAreaFmt1, &tempAreaFmt2, \
				tagGroupWidth, tagGroupHalfH<<1,"�Զ���ҳ", "off" );


	//���ÿɱ༭������
	short numEditGrpWidth = 100;
	short numEditGrpHeight = 110;

	short tag1NumHourSpacingX = 20;
	short numHourNumMinuteSpacingX = 10;

	short tag2NumSecondSpacingX = 20;


	//���ӵ�"ʱ"(Ĭ��Ϊ1ʱ)
	short alarmHourMarginXL = staticTag1XMax+tag1NumHourSpacingX;
	status = status && setAreaRange( &(scrSettingInst->editAlarmHourArea), alarmHourMarginXL + 1, alarmHourMarginXL + numEditGrpWidth, \
				tag1CenterY - (numEditGrpHeight>>1)+1, tag1CenterY + (numEditGrpHeight>>1) );
	status = status && setDefaultNumEditGroup( &(scrSettingInst->editAlarmHour), numEditGrpWidth, numEditGrpHeight, 23, 0, 1, "ʱ" );

	//���ӵ�"��"(Ĭ��Ϊ0��)
	short alarmMinuteMarginXL = staticTag1XMax + tag1NumHourSpacingX + numEditGrpWidth + numHourNumMinuteSpacingX;
	status = status && setAreaRange( &(scrSettingInst->editAlarmMinuteArea), alarmMinuteMarginXL+1, alarmMinuteMarginXL+numEditGrpWidth, \
				tag1CenterY - (numEditGrpHeight>>1)+1, tag1CenterY + (numEditGrpHeight>>1) );
	status = status && setDefaultNumEditGroup( &(scrSettingInst->editAlarmMinute), numEditGrpWidth, numEditGrpHeight, 59, 0, 0, "��" );

	//�Զ���ҳ��ʱ��"��"(Ĭ��Ϊ8��)
	short trunPageSecMarginXL = staticTag2XMax+tag2NumSecondSpacingX;
	status = status && setAreaRange( &(scrSettingInst->editTurnPageSecArea), trunPageSecMarginXL+1, trunPageSecMarginXL+numEditGrpWidth, \
				tag2CenterY - (numEditGrpHeight>>1)+1, tag2CenterY + (numEditGrpHeight>>1) );
	status = status && setDefaultNumEditGroup( &(scrSettingInst->editTurnPageSec), numEditGrpWidth, numEditGrpHeight, 30, 0, 8, "��" );


	return status;
}
bool screenMainColorPickerInit( ScreenColorPicker *scrColorPickerInst, short width, short height )
{
	scrColorPickerInst->borderVisible = true;

	scrColorPickerInst->borderColor = DEFAULT_BORDER_COLOR;//���±߽���ɫ�����������ң�
	scrColorPickerInst->bkgColor = DEFAULT_BKG_COLOR;//������ɫ


	bool status = true;
	if ( height < width )
	{
		status = false;
	}
	short marginYU = (height-width) >> 1;//�ϱ߾�

	status = status && setAreaRange( &(scrColorPickerInst->colorPickerArea), 1, width, marginYU + 1, height - marginYU );

	setDefaultColorTable( &(scrColorPickerInst->colorPicker), (void*)0, width, width );

	return status;
}

void screenMainAreaInfo( ScrMainAreaInfo *mainAreaInfoInst )
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
