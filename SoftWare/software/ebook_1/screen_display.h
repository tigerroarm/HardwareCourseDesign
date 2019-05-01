/*
 * screen_display.h
 *
 *  Created on: 2019年5月1日
 *      Author: 12757
 */

#ifndef SCREEN_DISPLAY_H_
#define SCREEN_DISPLAY_H_


#include "screen_type.h"

extern color_u16 screenShowSpace[SCR_HEIGHT][SCR_WIDTH];

//画边界
void showBorder( const AreaRange *borderPos, color_u16 borderColor );

//显示边界与背景
bool showBkgAndBorder( const ColorInfo *bkgBorderColor, const AreaRange *absPos, const AreaRange exceptAreaArray[], const short exceptAreaNum );

//显示screen_header
bool showScreenHeader( const ScreenHeaderTime *scrHeaderInst, const AreaRange *scrHeaderAbsPos );
//显示screen_footer
bool showScreenFooter( const ScreenFooterBtn *scrFooterInst, const AreaRange *scrFooterAbsPos );

//显示TagBlock
bool showTagBlock( const TagBlock *tagPtr, const AreaRange *absPos );

//显示IconBlock
bool showIconBlock( const IconBlock *iconPtr, const AreaRange *absPos );

//显示icon或者text
bool showTextOrIcon( const AreaRange *absPos, const alt_u8 *objPicPtr, color_u16 bkgColor, color_u16 objColor, short objXSize, short objYSize );

//获取新的绝对位置
void getAbsPos( const AreaRange *baseAbsPos, const AreaRange *relativePos, AreaRange *newAbsPos );

#endif /* SCREEN_DISPLAY_H_ */
