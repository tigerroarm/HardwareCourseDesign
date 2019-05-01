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
#include "font_manage.h"
#include "txt_read.h"

//设置区域范围
bool setAreaRange( AreaRange *areaPtr, short x_min, short x_max, short y_min, short y_max );
//居中设置区域范围
bool setAreaRangeCentered( AreaRange *areaPtr, short width, short height, short objXSize, short objYSize );
//区域范围自动转换
bool setAreaRangeByAreaFmt( AreaRange *areaPtr, AreaFmt *areaFmtPos, short width, short height );
//设置区域格式
bool setAreaFmt( AreaFmt *areaFmtPos, enum areaFmtID fmtXID, enum areaFmtID fmtYID, short xC_xML, short xHf_xMR, short yC_yMU, short yHf_yMD );
//设置默认标签(如果标签内放不下文本，则返回false)
bool setDefaultTagBlock( TagBlock *tagPtr, char *text, short width, short height );
//设置默认图标
bool setDefaultIconBlock( IconBlock *iconPtr, const alt_u8 *iconModel, short width, short height, short iconXSize, short iconYSize );
//设置默认进度条
bool setDefaultProgressBar( ProgressBar *prgBarInst, short width, short height );
//设置默认垂直滚动条
bool setDefaultScrollBarY( ScrollBarY *scrollBarYInst, short width, short height, float barYRatio );
//设置默认取色板信息
void setDefaultColorTable( ColorTable *colorPickerInst, color_u8 *colorSel, short width, short height );

//设置默认文本标签列表
bool setDefaultTagList( TagList *TagListInst, short elemWidth, short elemHeight, TextList *textListPtr, bool elemBorderVisible,\
						ColorInfo *colorInfoPtr );

//设置默认标签组
bool setDefaultTagBlockGroup( TagBlockGroup *tagGroupInst, AreaFmt *tagStaticPos, AreaFmt *tagVarPos, short width, short height,\
			char *tagStaText, char *tagVarText );
//设置默认可编辑数字(动态标签+静态图标)
bool setDefaultNumEditGroup( TagIconGroup *numEditGrpInst, short width, short height, short numMax, short numMin, short num, char *unitName );

//计算有效文本全部显示时的像素宽度
short getWidthOfText( char *str, short len );

//初始化屏幕
bool screenInit();

bool screenHeaderInit( ScreenHeaderTime *scrHeaderInst, short width, short height );
bool screenFooterInit( ScreenFooterBtn *scrFooterInst, short width, short height );

bool screenMainHomeInit( ScreenHome *scrHomeInst, short width, short height, const TxtFilesInfo *txtFilesInfoPtr );
bool screenMainBookInit( ScreenBook *scrBookInst, short width, short height );
bool screenMainSettingInit( ScreenSetting *scrSettingInst, short width, short height );
bool screenMainColorPickerInit( ScreenColorPicker *scrColorPickerInst, short width, short height );

bool ScreenMainPageInfoInit( ScreenPageInfo *scrPageInfoInst, short width, short height );

void screenMainAreaInfo( ScrMainAreaInfo *mainAreaInfoInst );

#endif /* SCREEN_H_ */
