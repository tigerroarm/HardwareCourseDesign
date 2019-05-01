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
#include "font_manage.h"
#include "icon_pic_extern.h"



//	sprintf(str, "%02d", num);可以将数字变为字符串

//整个屏幕变量
ScreenContainer tftlcdScreenInst;

ColorInfo defaultColorInfo = {  DEFAULT_BORDER_COLOR, DEFAULT_BKG_COLOR, DEFAULT_OBJ_COLOR, true, true };

char *str1 = "Happy";

//设置区域范围
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
//区域范围自动转换
bool setAreaRangeByAreaFmt( AreaRange *areaPtr, AreaFmt *areaFmtPos, short width, short height )
{

	AreaFmtCenter *tempFmtCenterPos;
	AreaFmtMargin *tempFmtMarginPos;

	bool status = true;

	//处理x方向
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
	//处理y方向
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

//设置区域格式
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

//居中设置区域范围(如果不是偶数，就稍微偏左上角)
bool setAreaRangeCentered( AreaRange *areaPtr, short width, short height, short objXSize, short objYSize )
{
	bool status = true;

	if ( width < objXSize || height < objYSize )//obj尺寸超出模块范围，不考虑对象obj的需求尺寸，obj直接使用模块尺寸
	{
		setAreaRange( areaPtr, 1, width, 1, height );
		status = false;//参数设置有误（有模块的对象尺寸超出边界）
		return status;
	}

	//区域范围正常
	short marginXL = ( width - objXSize ) >> 1;//左边距
	short marginXR = ( width - objXSize ) - marginXL;//右边距
	short marginYU = ( height - objYSize )>> 1;//上边距
	short marginYD = ( height - objYSize ) - marginYU;//下边距

	status = status && setAreaRange( areaPtr, marginXL+1, width-marginXR, marginYU+1, height-marginYD );

	return status;
}
//设置默认居中标签
bool setDefaultTagBlock( TagBlock *tagPtr, char *text, short width, short height )
{
	tagPtr->tagColorInfo = defaultColorInfo;

	tagPtr->wordSpacing = DEFAULT_WORD_SPACING;
	tagPtr->letterSpacing = DEFAULT_LETTER_SPACING;
	tagPtr->wordXSize = SRC_WORD_SIZE_X;//汉字横向宽度
	tagPtr->letterXSize = SRC_LETTER_SIZE_X;//字母横向宽度
	tagPtr->objYSize = SRC_OBJ_SIZE_Y;//文字纵向高度
	(tagPtr->tagText).text = text;

	short textLen = strlen(text);
	(tagPtr->tagText).textLen = textLen;

	(tagPtr->tagColorInfo).objVisible = ( text == NULL ) ? false : true;

	short textWidthSum = getWidthOfText( text, textLen );

	bool status = setAreaRangeCentered( &(tagPtr->textArea ), width, height, textWidthSum, SRC_OBJ_SIZE_Y );

	return status;
}
//设置默认进度条
bool setDefaultProgressBar( ProgressBar *prgBarInst, short width, short height )
{
	bool status = true;
	//基本信息
	prgBarInst->bkgBorderColor = defaultColorInfo;

	prgBarInst->xOffset = 0;
	prgBarInst->xOffsetMax = 280;

	//运动轴
	short dotHeight = ICON_DOT_Y_SIZE;
	short axisHeight = dotHeight / 2;
	short axisWidth = prgBarInst->xOffsetMax + 1;
	status = status && setAreaRangeCentered( &(prgBarInst->axisArea), width, height, axisWidth, axisHeight );
	//轴采用长方形区域的边界和填充的颜色体现
	status = status && setDefaultIconBlock( &(prgBarInst->axis), (void*)0, axisWidth, axisHeight, 0, 0 );

	//进度点基本区域（无偏移量时）
	short dotWidth = ICON_DOT_X_SIZE;
	short dotMarginXL = (prgBarInst->axisArea).x_min - dotWidth/2 - 1;
	short dotMarginYU = ( (prgBarInst->axisArea).y_min - 1 + (prgBarInst->axisArea).y_max - dotHeight ) / 2;
	status = status && setAreaRange( &(prgBarInst->dotBaseArea), dotMarginXL + 1, dotMarginXL + dotWidth,\
					dotMarginYU + 1, dotMarginYU + dotHeight);
	status = status && setDefaultIconBlock( &(prgBarInst->dot), icon_dot, axisWidth, axisHeight, dotWidth, dotHeight );


	return status;

}
//设置默认垂直滚动条
bool setDefaultScrollBarY( ScrollBarY *scrollBarYInst, short width, short height, float barYRatio )
{
	bool status = true;

	scrollBarYInst->barYColorInfo = defaultColorInfo;

	short iconMarginXL = 1;
	short iconMarginXR = 1;
	short iconUpMarginYU = 1;
	short iconDownMarginYD = 1;
	short iconWidth = width - iconMarginXL - iconMarginXR;
	short iconUpHeight = iconWidth;
	short iconDownHeight = iconWidth;
	short iconDownMarginYU = height - iconDownHeight - iconUpMarginYU;
	short iconSpacing = 2;//iconBlock之间的纵向间隔
	short iconBarMoveMarginYU = iconUpMarginYU + iconUpHeight + iconSpacing;
	short iconBarMoveHeight = height - iconDownMarginYD - iconDownHeight - iconSpacing - iconBarMoveMarginYU;

	//上移键图标
	status = status && setAreaRange( &(scrollBarYInst->iconMoveUpArea), iconMarginXL+1, iconMarginXL+iconWidth,\
					iconUpMarginYU+1, iconUpMarginYU+iconUpHeight );
	status = status && setDefaultIconBlock( &(scrollBarYInst->iconMoveUp), icon_move_up, iconWidth, iconUpHeight,\
					ICON_MOVE_UP_X_SIZE, ICON_MOVE_UP_Y_SIZE );

	//下移键图标
	status = status && setAreaRange( &(scrollBarYInst->iconMoveDownArea), iconMarginXL+1, iconMarginXL+iconWidth,\
					iconDownMarginYU+1, iconDownMarginYU+iconDownHeight );
	status = status && setDefaultIconBlock( &(scrollBarYInst->iconMoveDown), icon_move_down, iconWidth, iconDownHeight,\
					ICON_MOVE_DOWN_X_SIZE, ICON_MOVE_DOWN_Y_SIZE );


	//滚动条
	//运动区
	status = status && setAreaRange( &(scrollBarYInst->barMoveArea), iconMarginXL+1, iconMarginXL+iconWidth, \
					iconBarMoveMarginYU+1, iconBarMoveMarginYU+iconBarMoveHeight );

	//滚动条纵向偏移量
	scrollBarYInst->barHeight = barYRatio * iconBarMoveHeight;//计算滚动条的长度，barYRatio是滚动条高度占整个运动区高度的比率
	if ( scrollBarYInst->barHeight > iconBarMoveHeight )//文件数很少，不需要滚动条
	{
		scrollBarYInst->iconBarVisible = false;
		scrollBarYInst->barHeight = iconBarMoveHeight;
	}
	else//文件很多，需要滚动条
	{
		scrollBarYInst->iconBarVisible = true;
	}

	scrollBarYInst->barYOffsetMax = iconBarMoveHeight - scrollBarYInst->barHeight;
	scrollBarYInst->barYoffset = 0;

	//基本区（纵向偏移量为0时的位置）
	status = status && setAreaRange( &(scrollBarYInst->barBaseArea), iconMarginXL+1, iconMarginXL+iconWidth,\
					iconBarMoveMarginYU+1, iconBarMoveMarginYU+scrollBarYInst->barHeight );
	status = status && setDefaultIconBlock( &(scrollBarYInst->iconMoveDown), (void*)0, iconWidth, scrollBarYInst->barHeight, 0, 0 );


	return status;
}
//设置默认居中图标
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

//设置取色板信息(颜色的位置colorPos表示屏幕中选中的位置，根据该位置，显示器会突出该块颜色的边界)
void setDefaultColorTable( ColorTable *colorPickerInst, color_u8 *colorSel, short width, short height )
{
	colorPickerInst->colorIndex = colorSel;
	colorPickerInst->colorXNum = 16;//颜色有16行
	colorPickerInst->colorYNum = 16;//颜色有16列
	colorPickerInst->colorXSize = width / 16;
	colorPickerInst->colorYSize = height / 16;
	colorPickerInst->colorSpace = color_form;
}

//获取文本中汉字与字母的分布表(值为'c'，代表汉字，值为'e'，代表英文字母
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

	short textWidth = ( strType[0] == 'e' ) ? SRC_LETTER_SIZE_X : SRC_WORD_SIZE_X;

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


//设置默认标签组
bool setDefaultTagBlockGroup( TagBlockGroup *tagGroupInst, AreaFmt *tagStaticPos, AreaFmt *tagVarPos, short width, short height,\
			char *tagStaText, char *tagVarText )
{
	bool status = true;

	tagGroupInst->bkgBorderColor = defaultColorInfo;

	//设置静态标签的位置
	AreaRange *tempArea = &(tagGroupInst->staticTagArea);
	status = status && setAreaRangeByAreaFmt( tempArea, tagStaticPos, width, height );
	//设置静态标签内容
	status = status && setDefaultTagBlock( &(tagGroupInst->staticTag), tagStaText, \
			tempArea->x_max - tempArea->x_min + 1, tempArea->y_max - tempArea->y_max + 1);


	//设置动态标签的位置
	tempArea = &(tagGroupInst->varTagArea);
	status = status && setAreaRangeByAreaFmt( tempArea, tagVarPos, width, height );
	//设置动态标签内容
	status = status && setDefaultTagBlock( &(tagGroupInst->varTag), tagVarText, \
			tempArea->x_max - tempArea->x_min + 1, tempArea->y_max - tempArea->y_max + 1);

	return status;
}

//设置默认可编辑数字(动态标签+静态图标)
bool setDefaultNumEditGroup( TagIconGroup *numEditGrpInst, short width, short height, short numMax, short numMin, short num, char *unitName )
{
	bool status = true;

	//设置基本信息
	numEditGrpInst->bkgBorderColor = defaultColorInfo;

	//设置数字变化范围
	numEditGrpInst->value = num;
	numEditGrpInst->valueMax = numMax;
	numEditGrpInst->valueMin = numMin;

	//设置各个块的位置
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

	//设置plusIcon
	status = status && setAreaRange( &(numEditGrpInst->plusIconArea), numTagMarginXL+1, numTagMarginXL+iconBlockWidth,\
				plusIconMarginYU + 1, plusIconMarginYU + iconBlockHeight );
	status = status && setDefaultIconBlock( &(numEditGrpInst->plusIcon), icon_plus, \
				iconBlockWidth, iconBlockHeight, ICON_PLUS_X_SIZE, ICON_PLUS_Y_SIZE );

	//设置numberTag
	char numStr[3];
	sprintf(numStr, "%2d", num);
	status = status && setAreaRange( &(numEditGrpInst->valueTagArea), numTagMarginXL+1, numTagMarginXL+tagWidth,\
				(height>>1)-(tagHeight>>1)+1, (height>>1)+(tagHeight>>1) );
	status = status && setDefaultTagBlock( &(numEditGrpInst->valueTag), numStr, tagWidth, tagHeight );


	//设置minusIcon
	status = status && setAreaRange( &(numEditGrpInst->valueTagArea), numTagMarginXL+1, numTagMarginXL+tagWidth,\
				height - minusIconMarginYD - iconBlockHeight, height - minusIconMarginYD );
	status = status && setDefaultIconBlock( &(numEditGrpInst->minusIcon), icon_minus, \
				iconBlockWidth, iconBlockHeight, ICON_MINUS_X_SIZE, ICON_MINUS_Y_SIZE );


	//设置unitTag
	short unitTagMarginXL = numTagMarginXL+tagWidth + twoTagSpacingX;
	status = status && setAreaRange( &(numEditGrpInst->valueTagArea), unitTagMarginXL+1, unitTagMarginXL+tagWidth,\
					(height>>1)-(tagHeight>>1)+1, (height>>1)+(tagHeight>>1) );
	status = status && setDefaultTagBlock( &(numEditGrpInst->valueTag), unitName, tagWidth, tagHeight );


	return status;
}


//初始化
bool screenInit()
{
	//以下子模块宽度都相同 ( 1 ≤ x ≤ SCR_WIDE )

	bool status = true;
	//初始化屏幕总体尺寸 ( 1 ≤ y ≤ SCR_HEIGHT )
	status = status && setAreaRange( &(tftlcdScreenInst.screenArea), 1, SCR_WIDTH, 1, SCR_HEIGHT );

	//1.初始化顶栏 ( 1 ≤ y ≤ headerHeight )
	short headerHeight = SCR_HEADER_HEIGHT;//顶栏高度 (
	status = status && setAreaRange( &(tftlcdScreenInst.headerArea), 1, SCR_WIDTH, 1, headerHeight );

	status = status && screenHeaderInit( &(tftlcdScreenInst.scrHeader), SCR_WIDTH, headerHeight );

	//2.初始化底栏 ( SCR_HEIGHT - footerHeight + 1 ≤ y ≤ SCR_HEIGHT )
	short footerHeight = SCR_FOTTER_HEIGHT;//底栏高度
	status = status && setAreaRange( &(tftlcdScreenInst.headerArea), 1, SCR_WIDTH, SCR_HEIGHT - footerHeight + 1, SCR_HEIGHT );

	status = status && screenFooterInit( &(tftlcdScreenInst.scrFooter), SCR_WIDTH, footerHeight );

	//3.初始化主栏  ( headerHeight + 1 ≤ y ≤ SCR_HEIGHT - footerHeight )
	short mainAreaHeight = SCR_HEIGHT - headerHeight - footerHeight;
	setAreaRange( &(tftlcdScreenInst.mainArea), 1, SCR_WIDTH, headerHeight + 1, SCR_HEIGHT - footerHeight );
	//注意：txtFilesInfoSpace内的数据必须提前填充好，否则可能运行出错。因此，执行screenInit()之前，首先运行txtFilesInfoSpace信息填充
	status = status && screenMainHomeInit( &(tftlcdScreenInst.scrHome), SCR_WIDTH, mainAreaHeight, &txtFilesInfoSpace );
	status = status && screenMainBookInit( &(tftlcdScreenInst.scrBook), SCR_WIDTH, mainAreaHeight );
	status = status && screenMainSettingInit( &(tftlcdScreenInst.scrSetting), SCR_WIDTH, mainAreaHeight );
	status = status && screenMainColorPickerInit( &(tftlcdScreenInst.scrColorPicker), SCR_WIDTH, mainAreaHeight );

	//4.初始化主栏页面信息
	screenMainAreaInfo( &(tftlcdScreenInst.mainAreaInfo) );

	return status;
}

bool screenHeaderInit( ScreenHeaderTime *scrHeaderInst, short width, short height )
{

	scrHeaderInst->bkgBorderColor = defaultColorInfo;

	short marginX = 100;//横向边距
	short marginY = 5;//纵向边距

	bool status = true;

	status = status && setAreaRange( &(scrHeaderInst->tagTimeArea), marginX+1, width-marginX, marginY+1, height-marginY );

	scrHeaderInst->timeHour = 0;
	scrHeaderInst->timeMinute = 0;
	scrHeaderInst->timeSecond = 0;

	//时间文本为"00:00"(0时0分)
	status = status && setDefaultTagBlock( &(scrHeaderInst->tagTime), "00:00", width - (marginX<<1), height - (marginY<<1) );
//	(scrHeaderInst->tagTime).borderVisible = false;

	return status;
}
bool screenFooterInit( ScreenFooterBtn *scrFooterInst, short width, short height )
{
	scrFooterInst->bkgBorderColor = defaultColorInfo;

	//SETTING键,HOME键,BACK键分别处于下边栏的左、中、右位置
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

bool screenMainHomeInit( ScreenHome *scrHomeInst, short width, short height, const TxtFilesInfo *txtFilesInfoPtr )
{
	scrHomeInst->bkgBorderColor = defaultColorInfo;

	bool status = true;

	//列表
	short blockMarginY = 1;
	short blockHeight = height - (blockMarginY<<1);

	short numListMarginXL = 1;
	short numListWidth = 50;//数字编号列表元素宽度

	short scrollBarMarginXR = 1;
	short scrollBarWidth = 50;//滚动条宽度
	short scrollBarMarginXL = width - scrollBarMarginXR - scrollBarWidth;

	short catalogListSpacingL = 2;//txt文件目录与左侧数字编号列表的间隔
	short catalogListSpacingR = 2;//txt文件目录与右侧滚动条的间隔
	short catalogListMarginXL = numListMarginXL + numListWidth + catalogListSpacingL;
	short catalogListWidth = width - scrollBarMarginXR - scrollBarWidth - catalogListSpacingR - catalogListMarginXL;

	//列表元素个数
	short listElemNum = txtFilesInfoPtr->txtFilesNum;//列表元素个数
	short listElemHeight = CATALOG_ROW_HEIGHT;//列表元素高度




	//列表内容设置
	//两个TextList变量malloc索要空间，并将txtFilesInfoPtr中的有效信息保存到索要的TextList空间中
	TextList numList;
	numList.textNum = listElemNum;
	numList.textArray = (TextType*)malloc( sizeof(TextType) * listElemNum );
	TextType *textAry = numList.textArray;
	int i;
	char tempStr[10];
	for ( i = 0; i < listElemNum; i ++ )
	{
		sprintf(tempStr, "%2d", i + 1 );//获取编号字符串（从" 1"开始,最大"99"）
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




	//数字编号列表
	status = status && setAreaRange( &(scrHomeInst->numBarArea), numListMarginXL+1, numListMarginXL+numListWidth,\
							blockMarginY+1, height-blockMarginY );
	status = status && setDefaultTagList( &(scrHomeInst->numBar), numListWidth, listElemNum, &numList, true,\
							&defaultColorInfo );
	//txt文件目录列表
	status = status && setAreaRange( &(scrHomeInst->txtCatalogArea), catalogListMarginXL+1, catalogListMarginXL+catalogListWidth,\
							blockMarginY+1, height-blockMarginY );
	status = status && setDefaultTagList( &(scrHomeInst->txtCatalog), numListWidth, listElemNum, &catalogList, true,\
							&defaultColorInfo );

	//释放使用完毕的numListPtr与catalogListPtr空间
	free(numList.textArray);

	free(catalogList.textArray);


	//列表纵向移动显示设置
	scrHomeInst->listYOffset = 0;//列表显示时的纵向偏移量
	scrHomeInst->listShowHeight = blockHeight;//列表显示在屏幕上时的高度
	scrHomeInst->listYSize = listElemNum * listElemHeight;//列表纵向全长
	scrHomeInst->listShowYRatio = scrHomeInst->listShowHeight / scrHomeInst->listYSize;
	scrHomeInst->listYOffsetMax = scrHomeInst->listYSize - blockHeight;
	scrHomeInst->listYStepSize = CATALOG_ROW_HEIGHT;

	//滚动条
	status = status && setAreaRange( &(scrHomeInst->turnCatalogBarArea), scrollBarMarginXL + 1, scrollBarMarginXL + scrollBarWidth,\
							blockMarginY+1, height-blockMarginY );
	status = status && setDefaultScrollBarY( &(scrHomeInst->turnCatalogBarY), scrollBarWidth, blockHeight, scrHomeInst->listShowYRatio );




	//某行目录文件名水平滚动设置
	scrHomeInst->txtScrollable = false;//允许滚动否
	scrHomeInst->txtScrollrow = 0;//可滚动的某行行数（一次只有一行可以滚动）
	scrHomeInst->txtScrollxOffset = 0;//当前滚动偏移量
	scrHomeInst->txtScrollwidth = 0;//滚动文本的真实像素宽度（可以超出屏幕宽度）
	scrHomeInst->txtScrollSpacing = width / 2;//滚动文本循环时文本尾与新文本头的间隔


	return status;
}
//设置默认文本标签列表
bool setDefaultTagList( TagList *TagListInst, short elemWidth, short elemHeight, TextList *textListPtr, bool elemBorderVisible,\
		                 ColorInfo *colorInfoPtr )
{
	//设置列表元素区域大小
	bool status = true;
	status = status && setAreaRange( &(TagListInst->elemBaseArea), 1, elemWidth, 1, elemHeight );
	//计算文本所在区域
	//计算文本宽度(使用中文尺寸来估计）
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
	//设置元素文本区域位置
	status = status && setAreaRange( &(TagListInst->textArea), marginXL+1, elemWidth-marginXR, marginYU+1, elemHeight-marginYD );

	short elemNum = textListPtr->textNum;

	TagListInst->wordSpacing = DEFAULT_WORD_SPACING;
	TagListInst->letterSpacing = DEFAULT_LETTER_SPACING;

	TagListInst->elemNum = elemNum;
	TagListInst->elemColorInfo = *colorInfoPtr;

	int i;

	AreaRange *elemAreaArray = (AreaRange*)malloc( sizeof( AreaRange ) * elemNum );
	TagListInst->elemArea = elemAreaArray;
	TagBlock *tagBolckArray = (TagBlock*)malloc( sizeof( TagBlock ) * elemNum );
	TagListInst->elemBlock = tagBolckArray;

	AreaRange *curElemAreaPtr;
	TagBlock *curTagPtr;
	TextType *textArray = textListPtr->textArray;

	short elemYPos = 0;
	for ( i = 0; i < elemNum; i ++ )
	{
		curTagPtr = &(tagBolckArray[i]);
		curElemAreaPtr = &(elemAreaArray[i]);

		curTagPtr->tagColorInfo = TagListInst->elemColorInfo;
		curTagPtr->wordSpacing = TagListInst->wordSpacing;
		curTagPtr->letterSpacing = TagListInst->letterSpacing;
		curTagPtr->tagText = textArray[i];
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
	//可编辑的颜色（电子书背景颜色与字体颜色）
	scrBookInst->bkgColorIndex = 0;
	scrBookInst->txtColorIndex = 255;

	//设置左中右点击区范围
	short leftAreaWidth = width/3;
	short rightAreaWidth = width/3;
	short middleAreaWidth = width - leftAreaWidth - rightAreaWidth;
	status = status && setAreaRange( &(scrBookInst->turnBackPageAreaPos), 1, leftAreaWidth, 1, height );
	status = status && setAreaRange( &(scrBookInst->openPageInfoAreaPos), leftAreaWidth+1, leftAreaWidth+middleAreaWidth, 1, height );
	status = status && setAreaRange( &(scrBookInst->turnBackPageAreaPos), width-rightAreaWidth+1, width, 1, height );

	//设置电子书范围（居中）
	short bookAreaHeight = BOOK_ROW_HEIGHT * BOOK_COL_NUM;
	short marginYU = ( height - bookAreaHeight )>>1;
	short marginYD = height - bookAreaHeight - marginYU;
	status = status && setAreaRange( &(scrBookInst->txtBookArea), 1, width, marginYU+1, height-marginYD );

	//设置电子书阅读进度信息页面范围
	short pageInfoAreaHeight = height/4;
	status = status && setAreaRange( &(scrBookInst->pageInfoArea), 1, width, height-pageInfoAreaHeight, height-1 );

	//设置电子书阅读进度信息页面是否可见
	scrBookInst->pageInfoVisible = false;

	TextType textArray[BOOK_COL_NUM];

	int i;
	for( i = 0; i < BOOK_COL_NUM; i ++ )
	{
		(textArray[i]).text = txtBookSpace[i];
		(textArray[i]).textLen = BOOK_ROW_BYTES;
	}

	ColorInfo colorSel = { DEFAULT_BORDER_COLOR, color_form[scrBookInst->bkgColorIndex], color_form[scrBookInst->txtColorIndex] };
	//设置电子书文本域（文本域为由TagBlock组成的TagList文本标签列表）
	TextList textList = { textArray, BOOK_COL_NUM };
	status = status && setDefaultTagList( &(scrBookInst->txtBook), width, BOOK_ROW_HEIGHT, &textList, false, &colorSel );


	//设置电子书阅读进度信息页面
	status = status && ScreenMainPageInfoInit( &(scrBookInst->pageInfo), width, pageInfoAreaHeight );

	return status;
}

bool ScreenMainPageInfoInit( ScreenPageInfo *scrPageInfoInst, short width, short height )
{
	//基本信息
	scrPageInfoInst->bkgBorderColor = defaultColorInfo;
	scrPageInfoInst->curPageNum = 0;
	scrPageInfoInst->curPagePercent = 0;
	scrPageInfoInst->totalPageNum = 0;

	bool status = true;

	//进度条
	short prgBarMarginYU = 4;
	short prgBarHeight = height/2;
	short prgBarMarginX = 2;
	status = status && setAreaRange( &(scrPageInfoInst->prgBarArea), prgBarMarginX+1, width-prgBarMarginX, prgBarMarginYU+1,\
						prgBarMarginYU + prgBarHeight  );
	status = status && setDefaultProgressBar( &(scrPageInfoInst->prgBarX), width - (prgBarMarginX<<1), prgBarHeight );

	//动态文本标签(高度设定)
	short tagHeight = ( height - prgBarMarginYU - prgBarHeight );
	short tagMarginYD = 5;

	//进度百分数标签(向左侧偏移)格式为"53.33%"
	short tagPercentMarginXL = 10;
	short tagPercWidth = 6 * SRC_LETTER_SIZE_X + 5 * DEFAULT_LETTER_SPACING + 20;//20是横向边界裕量
	status = status && setAreaRange( &(scrPageInfoInst->tagPagePercTageArea), tagPercentMarginXL+1, tagPercentMarginXL+tagPercWidth,\
						height - tagMarginYD - tagHeight + 1 , height - tagMarginYD );
	status = status && setDefaultTagBlock( &(scrPageInfoInst->tagPagePercTage), "33.33%", tagPercWidth, tagHeight );


	//进度页数标签(向右侧偏移)格式为：" 1234/99999"
	short tagPageNumMarginXR = 10;
	short tagPsgeNumWidth = 11 * SRC_LETTER_SIZE_X + 10 * DEFAULT_LETTER_SPACING + 20;//20是横向边界裕量

	status = status && setAreaRange( &(scrPageInfoInst->tagPageNumArea), width-tagPageNumMarginXR-tagPsgeNumWidth+1, width-tagPageNumMarginXR,\
						height - tagMarginYD - tagHeight + 1 , height - tagMarginYD );
	status = status && setDefaultTagBlock( &(scrPageInfoInst->tagPageNum), "33.33%", tagPsgeNumWidth, tagHeight );

	return status;
}

bool screenMainSettingInit( ScreenSetting *scrSettingInst, short width, short height )
{
	bool status = true;

	scrSettingInst->bkgBorderColor = defaultColorInfo;

	//设置左侧所有标签的位置
	short LeftTagMarginXL = 20;//左侧所有标签的左边距
	short LeftTextMarginInTagX = 15;//在标签内部的文本与标签横向边界的距离
	short LeftTagGroupMarginXL = 10;//标签组的左边距
	//高度设定
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

	//设置静态标签：

	//"休息定时"
	short textWidth1 = getWidthOfText( "休息定时", 8 );//4字汉字文本的像素宽度
	short staticTag1XLen = (LeftTextMarginInTagX<<1)+textWidth1;
	short staticTag1XMax = LeftTagMarginXL+staticTag1XLen;

	status = status && setAreaRange( &(scrSettingInst->tag1Area), LeftTagMarginXL+1, staticTag1XMax, tag1CenterY-tag1HalfH+1, tag1CenterY+tag1HalfH );
	status = status && setDefaultTagBlock( &(scrSettingInst->tag1), "休息定时", staticTag1XLen, tag1HalfH<<1 );


	//"自动翻页定时"
	short textWidth2 = getWidthOfText( "自动翻页定时", 12 );//6字汉字文本的像素宽度
	short staticTag2XLen = (LeftTextMarginInTagX<<1)+textWidth2;
	short staticTag2XMax = LeftTagMarginXL+staticTag2XLen;

	status = status && setAreaRange( &(scrSettingInst->tag1Area), LeftTagMarginXL+1, LeftTagMarginXL+staticTag2XLen, tag2CenterY-tag2HalfH+1, tag2CenterY+tag2HalfH );
	status = status && setDefaultTagBlock( &(scrSettingInst->tag1), "自动翻页定时", (LeftTextMarginInTagX<<1)+textWidth2, tag2HalfH<<1 );

	//声明2个定位变量
	AreaFmt tempAreaFmt1;
	AreaFmt tempAreaFmt2;

	//"背景颜色","字体颜色","自动翻页"
	//设置tagGroup位置(水平用边距，垂直用中心距)

	short tagGroupWidth = width*2/3;
	short tagGroupHalfH = editBkgColorHalfH + 5;
	short tagGroupXSpacing = 50;//标签组中两个标签的水平间距

	status = status && setAreaFmt( &tempAreaFmt1, AREA_FMT_MARGIN, AREA_FMT_CENTER, \
			    LeftTagGroupMarginXL, width - tagGroupWidth - LeftTagGroupMarginXL, editBkgColorCenterY, tagGroupHalfH );

	setAreaRangeByAreaFmt( &(scrSettingInst->editBkgColorArea), &tempAreaFmt1, width, height );
	tempAreaFmt1.fmtValY.fmtCenterVal.center = editWordColorCenterY;
	setAreaRangeByAreaFmt( &(scrSettingInst->editWordColorArea), &tempAreaFmt1, width, height );
	tempAreaFmt1.fmtValY.fmtCenterVal.center = editTurnPageModCenterY;
	setAreaRangeByAreaFmt( &(scrSettingInst->editTurnPageModArea), &tempAreaFmt1, width, height );

	//设置tagGroup内容
	//左边的静态标签范围
	short leftTagMarginInGroupXL = LeftTagMarginXL - LeftTagGroupMarginXL;
	status = status && setAreaFmt( &tempAreaFmt1, AREA_FMT_MARGIN, AREA_FMT_CENTER, \
				leftTagMarginInGroupXL, tagGroupWidth - leftTagMarginInGroupXL - staticTag1XLen, editBkgColorCenterY, editBkgColorHalfH );
	//右边的动态标签范围
	short rightTagMarginInGroupXL = leftTagMarginInGroupXL + staticTag1XLen + tagGroupXSpacing;
	short rightTagWidth = 50;//要保证取值大于文本"off"的宽度，即3*SRC_LETTER_SIZE_X+2*DEFAULT_LETTER_SPACING
	status = status && setAreaFmt( &tempAreaFmt2, AREA_FMT_MARGIN, AREA_FMT_CENTER, \
				rightTagMarginInGroupXL, tagGroupWidth - leftTagMarginInGroupXL - rightTagWidth, editBkgColorCenterY, editBkgColorHalfH );

	//设置标签组
	status = status && setDefaultTagBlockGroup( &(scrSettingInst->editBkgColor), &tempAreaFmt1, &tempAreaFmt2, \
				tagGroupWidth, tagGroupHalfH<<1,"背景颜色", (void*)0 );

	tempAreaFmt1.fmtValY.fmtCenterVal.half = editWordColorHalfH;
	tempAreaFmt2.fmtValY.fmtCenterVal.half = editWordColorHalfH;
	status = status && setDefaultTagBlockGroup( &(scrSettingInst->editWordColor), &tempAreaFmt1, &tempAreaFmt2, \
				tagGroupWidth, tagGroupHalfH<<1,"字体颜色", (void*)0 );

	tempAreaFmt1.fmtValY.fmtCenterVal.half = editTurnPageModHalfH;
	tempAreaFmt2.fmtValY.fmtCenterVal.half = editTurnPageModHalfH;
	status = status && setDefaultTagBlockGroup( &(scrSettingInst->editTurnPageMod), &tempAreaFmt1, &tempAreaFmt2, \
				tagGroupWidth, tagGroupHalfH<<1,"自动翻页", "off" );


	//设置可编辑数字域
	short numEditGrpWidth = 100;
	short numEditGrpHeight = 110;

	short tag1NumHourSpacingX = 20;
	short numHourNumMinuteSpacingX = 10;

	short tag2NumSecondSpacingX = 20;


	//闹钟的"时"(默认为1时)
	short alarmHourMarginXL = staticTag1XMax+tag1NumHourSpacingX;
	status = status && setAreaRange( &(scrSettingInst->editAlarmHourArea), alarmHourMarginXL + 1, alarmHourMarginXL + numEditGrpWidth, \
				tag1CenterY - (numEditGrpHeight>>1)+1, tag1CenterY + (numEditGrpHeight>>1) );
	status = status && setDefaultNumEditGroup( &(scrSettingInst->editAlarmHour), numEditGrpWidth, numEditGrpHeight, 23, 0, 1, "时" );

	//闹钟的"分"(默认为0分)
	short alarmMinuteMarginXL = staticTag1XMax + tag1NumHourSpacingX + numEditGrpWidth + numHourNumMinuteSpacingX;
	status = status && setAreaRange( &(scrSettingInst->editAlarmMinuteArea), alarmMinuteMarginXL+1, alarmMinuteMarginXL+numEditGrpWidth, \
				tag1CenterY - (numEditGrpHeight>>1)+1, tag1CenterY + (numEditGrpHeight>>1) );
	status = status && setDefaultNumEditGroup( &(scrSettingInst->editAlarmMinute), numEditGrpWidth, numEditGrpHeight, 59, 0, 0, "分" );

	//自动翻页定时的"秒"(默认为8秒)
	short trunPageSecMarginXL = staticTag2XMax+tag2NumSecondSpacingX;
	status = status && setAreaRange( &(scrSettingInst->editTurnPageSecArea), trunPageSecMarginXL+1, trunPageSecMarginXL+numEditGrpWidth, \
				tag2CenterY - (numEditGrpHeight>>1)+1, tag2CenterY + (numEditGrpHeight>>1) );
	status = status && setDefaultNumEditGroup( &(scrSettingInst->editTurnPageSec), numEditGrpWidth, numEditGrpHeight, 30, 0, 8, "秒" );


	return status;
}
bool screenMainColorPickerInit( ScreenColorPicker *scrColorPickerInst, short width, short height )
{
	scrColorPickerInst->bkgBorderColor = defaultColorInfo;


	bool status = true;
	if ( height < width )
	{
		status = false;
	}
	short marginYU = (height-width) >> 1;//上边距

	status = status && setAreaRange( &(scrColorPickerInst->colorPickerArea), 1, width, marginYU + 1, height - marginYU );

	setDefaultColorTable( &(scrColorPickerInst->colorPicker), (void*)0, width, width );

	return status;
}

void screenMainAreaInfo( ScrMainAreaInfo *mainAreaInfoInst )
{
	//设置当前页面为第一页
	mainAreaInfoInst->curScrIndex = 0;
	//设置第一页为主页
	(mainAreaInfoInst->scrIDStack)[0] = SCR_HOME;
	//清空后面的栈页（即后面不存在栈页）
	int i;
	for ( i = 1; i < 4; i ++ )
	{
		(mainAreaInfoInst->scrIDStack)[i] = SCR_NONE;
	}
}

