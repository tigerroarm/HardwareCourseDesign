/*
 * screen_display.h
 *
 *  Created on: 2019年5月1日
 *      Author: 12757
 */

#ifndef SCREEN_DISPLAY_H_
#define SCREEN_DISPLAY_H_


#include "screen_type.h"

//该数组用于临时测试屏幕显示效果，最后的工程会删除该数组
//include "screen_display.h"的库可以获得该数组的使用权限
extern color_u16 screenShowSpace[SCR_HEIGHT][SCR_WIDTH];

//获取新的绝对位置
bool getAbsPos( const AreaRange *baseAbsPos, const AreaRange *relativePos, AreaRange *newAbsPos );

//获取x方向的区域定位数组(返回数组大小）
short getXVaildRange( const char areaOccurInfo[], const AreaRange exceptAreaArray[], const short exceptAreaNum, \
		short *xMinArray, short *xMaxArray, const short width );

//画边界
bool showBorder( const AreaRange *borderAbsPos, color_u16 borderColor );

//显示边界与背景(width为总宽度)
bool showBkgAndBorder( const ColorInfo *bkgBorderColor, const AreaRange *absPos, const AreaRange exceptAreaArray[], const short exceptAreaNum );

//显示icon或者text
bool showTextOrIcon( const AreaRange *absPos, const alt_u8 *objPicPtr, color_u16 bkgColor, color_u16 objColor, short objXSize, short objYSize );

//显示TagBlock
bool showTagBlock( const TagBlock *tagPtr, const AreaRange *absPos );
//显示TagBlock(带纵向基区偏移)
bool showTagWithYOffset( const TagBlock *tagPtr, const AreaRange *absPos, const short yOffset );

//显示IconBlock
bool showIconBlock( const IconBlock *iconPtr, const AreaRange *absPos );

//显示TagGroup
bool showTagGroup( const TagGroup *tagGroupInst, const AreaRange *absPos );

//显示TagIconGroup
bool showTagIconGroup( const TagIconGroup *tagIconGpInst, const AreaRange *absPos );

//显示TagList
bool showTagList( const TagList *tagListInst, const AreaRange *absPos, const short tagListYOffset );

//显示ScrollBarY
bool showScrollBarY( const ScrollBarY *scrollBarYInst, const AreaRange *absPos );

//显示ScreenPageInfo
bool showScreenPageInfo( const ScreenPageInfo *scrPageInfoInst, const AreaRange *absPos );

//显示ProgressBarX
bool showProgressBarX( const ProgressBarX *prgBarXInst, const AreaRange *absPos );

//显示颜色板
bool showColorBoard( const ColorBoard *colorTableInst, const AreaRange *absPos );


//显示颜色板中选中颜色的边界
bool showColorSelBorder( const ColorBoard *colorTableInst, const AreaRange *absPos );
//清除颜色板中选中颜色的边界
bool clearColorSelBorder( const ColorBoard *colorTableInst, const AreaRange *absPos );


//显示screen_header
bool showScreenHeader( const ScreenHeaderTime *scrHeaderInst, const AreaRange *scrHeaderAbsPos );
//显示screen_footer
bool showScreenFooter( const ScreenFooterBtn *scrFooterInst, const AreaRange *scrFooterAbsPos );
//显示screen_main
bool showScreenMain( const ScreenContainer *scrContainerInst );
//显示screen_main_home
bool showScreenMainHome( const ScreenHome *scrHomeInst, const AreaRange *scrMainAbsPos );
//显示screen_main_book
bool showScreenMainBook( const ScreenBook *scrBookInst, const AreaRange *scrMainAbsPos );
//显示screen_main_setting
bool showScreenMainSetting( const ScreenSetting *scrSettingInst, const AreaRange *scrMainAbsPos );
//显示screen_main_color_picker
bool showScreenMainColorPicker( const ScreenColorPicker *scrColorPickerInst, const AreaRange *scrMainAbsPos );

//显示screen
bool showScreen( );



#endif /* SCREEN_DISPLAY_H_ */
