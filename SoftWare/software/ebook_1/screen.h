/*
 * screen.h
 *
 * 本库实现屏幕内容的显示
 *
 *  Created on: 2019年4月27日
 *      Author: 12757
 */

#ifndef SCREEN_H_
#define SCREEN_H_

#include "screen_type.h"

#define SCR_WIDTH 320
#define SCR_HEIGHT 480

#define DEFAULT_BORDER_COLOR 0x0000
#define DEFAULT_BKG_COLOR 0x1234
#define DEFAULT_OBJ_COLOR 0xffff



//默认中文字符大小
#define SRC_WORD_SIZE_X 16
#define SRC_WORD_SIZE_Y 16
#define DEFAULT_WORD_SPACING 2//间距
//默认英文字母大小
#define SRC_LETTER_SIZE_X 8
#define SRC_LETTER_SIZE_Y 16
#define DEFAULT_LETTER_SPACING 1 //间距

//设置区域范围
void setAreaRange( AreaRange *areaPtr, short x_min, short x_max, short y_min, short y_max );
//设置默认标签(如果标签内放不下文本，则返回false)
bool setDefaultTagBlock( TagBlock *tagPtr, char *text, short width, short height );

//初始化屏幕
bool screenInit();

bool screenHeaderInit( ScreenHeaderTime *scrHeaderInst, short width, short height );
bool screenFooterInit( ScreenFooterBtn *scrFooterInst, short width, short height );

bool screenMainHomeInit( ScreenHome *scrHomeInst, short width, short height );
bool screenMainBookInit( ScreenBook *scrBookInst, short width, short height );
bool screenMainSettingInit( ScreenSetting *scrSettingInst, short width, short height );
bool screenMainColorPickerInit( ScreenColorPicker *scrColorPickerInst, short width, short height );

void screenMainAreaInfo( ScrMainAreaInfo *mainAreaInfoInst );

#endif /* SCREEN_H_ */
