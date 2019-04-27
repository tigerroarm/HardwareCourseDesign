/*
 * screen.c
 *
 *  Created on: 2019年4月26日
 *      Author: 12757
 */

#include "screen_type.h"
#include "screen.h"
#include <string.h>
#include <stdio.h>

//	sprintf(str, "%02d", num);可以将数字变为字符串

//整个屏幕变量
ScreenContainer tftlcdScreenInst;

ColorInfo defaultColorInfo = {  DEFAULT_BORDER_COLOR, DEFAULT_BKG_COLOR, DEFAULT_OBJ_COLOR };

char *str1 = "Happy";

//设置区域范围
void setAreaRange( AreaRange *areaPtr, short x_min, short x_max, short y_min, short y_max )
{
	areaPtr->x_min = x_min;
	areaPtr->x_max = x_max;
	areaPtr->y_min = y_min;
	areaPtr->y_max = y_max;
}

//设置默认居中标签
bool setDefaultTagBlock( TagBlock *tagPtr, char *text, short width, short height )
{
	bool status = true;

	tagPtr->borderVisible = true;
	tagPtr->tagColorInfo = defaultColorInfo;
	tagPtr->wordSpacing = DEFAULT_WORD_SPACING;
	(tagPtr->tagText).text = text;

	short textLen = strlen(text);
	(tagPtr->tagText).textSize = textLen;

	if ( text == NULL )
	{
		tagPtr->textVisible = false;
	}
	else
	{
		tagPtr->textVisible = true;
	}
	short textWidthSum = (SRC_LETTER_SIZE_X+DEFAULT_LETTER_SPACING) * textLen - DEFAULT_LETTER_SPACING;

	short marginX = ( width - textWidthSum ) >> 1;
	short marginY = ( height - SRC_WORD_SIZE_Y )>> 1;


	if ( marginX < 0 )//文本超出标签范围
	{
		status = false;
		marginX = 0;
	}
	if ( marginY < 0 )//文本超出标签范围
	{
		status = false;
		marginY = 0;
	}

	setAreaRange( &(tagPtr->textArea ), marginX+1, width-marginX, marginY+1, height-marginY );

	return status;
}

//初始化
bool screenInit()
{
	//以下子模块宽度都相同 ( 1 ≤ x ≤ SCR_WIDE )


	//初始化屏幕总体尺寸 ( 1 ≤ y ≤ SCR_HEIGHT )
	setAreaRange( &(tftlcdScreenInst.screenArea), 1, SCR_WIDTH, 1, SCR_HEIGHT );

	//1.初始化顶栏 ( 1 ≤ y ≤ headerHeight )
	short headerHeight = 40;//顶栏高度 (
	setAreaRange( &(tftlcdScreenInst.headerArea), 1, SCR_WIDTH, 1, headerHeight );

	bool status1 = screenHeaderInit( &(tftlcdScreenInst.scrHeader), SCR_WIDTH, headerHeight );

	//2.初始化底栏 ( SCR_HEIGHT - footerHeight + 1 ≤ y ≤ SCR_HEIGHT )
	short footerHeight = 60;//底栏高度
	setAreaRange( &(tftlcdScreenInst.headerArea), 1, SCR_WIDTH, SCR_HEIGHT - footerHeight + 1, SCR_HEIGHT );

	bool status2 = screenFooterInit( &(tftlcdScreenInst.scrFooter), SCR_WIDTH, footerHeight );

	//3.初始化主栏  ( headerHeight + 1 ≤ y ≤ SCR_HEIGHT - footerHeight )
	short mainAreaHeight = SCR_HEIGHT - headerHeight - footerHeight;
	setAreaRange( &(tftlcdScreenInst.mainArea), 1, SCR_WIDTH, headerHeight + 1, SCR_HEIGHT - footerHeight );

	bool status3 = screenMainHomeInit( &(tftlcdScreenInst.scrHome), SCR_WIDTH, mainAreaHeight );
	bool status4 = screenMainBookInit( &(tftlcdScreenInst.scrBook), SCR_WIDTH, mainAreaHeight );
	bool status5 = screenMainSettingInit( &(tftlcdScreenInst.scrSetting), SCR_WIDTH, mainAreaHeight );
	bool status6 = screenMainColorPickerInit( &(tftlcdScreenInst.scrColorPicker), SCR_WIDTH, mainAreaHeight );

	//4.初始化主栏页面信息
	screenMainAreaInfo( &(tftlcdScreenInst.mainAreaInfo) );

	return status1 && status2 && status3 && status4 && status5 && status6;
}

bool screenHeaderInit( ScreenHeaderTime *scrHeaderInst, short width, short height )
{
	bool status = true;

	scrHeaderInst->borderVisible = true;
	scrHeaderInst->bkgColor = DEFAULT_BKG_COLOR;
	scrHeaderInst->borderColor = DEFAULT_BORDER_COLOR;

	short marginX = 100;//横向边距
	short marginY = 5;//纵向边距

	setAreaRange( &(scrHeaderInst->tagTimeArea), marginX+1, width-marginX, marginY+1, height-marginY );

	scrHeaderInst->timeHour = 0;
	scrHeaderInst->timeMinute = 0;
	scrHeaderInst->timeSecond = 0;

	//时间文本为"00:00"(0时0分)
	status = setDefaultTagBlock( &(scrHeaderInst->tagTime), "00:00", width - (marginX<<1), height - (marginY<<1) );
//	(scrHeaderInst->tagTime).borderVisible = false;

	return status;
}
bool screenFooterInit( ScreenFooterBtn *scrFooterInst, short width, short height )
{
	bool status = true;

	return status;
}

bool screenMainHomeInit( ScreenHome *scrHomeInst, short width, short height )
{
	bool status = true;

	return status;
}
bool screenMainBookInit( ScreenBook *scrBookInst, short width, short height )
{
	bool status = true;

	return status;
}
bool screenMainSettingInit( ScreenSetting *scrSettingInst, short width, short height )
{
	bool status = true;

	return status;
}
bool screenMainColorPickerInit( ScreenColorPicker *scrColorPickerInst, short width, short height )
{
	bool status = true;

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

