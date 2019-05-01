/*
 * screen_display.h
 *
 *  Created on: 2019��5��1��
 *      Author: 12757
 */

#ifndef SCREEN_DISPLAY_H_
#define SCREEN_DISPLAY_H_


#include "screen_type.h"

extern color_u16 screenShowSpace[SCR_HEIGHT][SCR_WIDTH];

//���߽�
void showBorder( const AreaRange *borderPos, color_u16 borderColor );

//��ʾ�߽��뱳��
bool showBkgAndBorder( const ColorInfo *bkgBorderColor, const AreaRange *absPos, const AreaRange exceptAreaArray[], const short exceptAreaNum );

//��ʾscreen_header
bool showScreenHeader( const ScreenHeaderTime *scrHeaderInst, const AreaRange *scrHeaderAbsPos );
//��ʾscreen_footer
bool showScreenFooter( const ScreenFooterBtn *scrFooterInst, const AreaRange *scrFooterAbsPos );

//��ʾTagBlock
bool showTagBlock( const TagBlock *tagPtr, const AreaRange *absPos );

//��ʾIconBlock
bool showIconBlock( const IconBlock *iconPtr, const AreaRange *absPos );

//��ʾicon����text
bool showTextOrIcon( const AreaRange *absPos, const alt_u8 *objPicPtr, color_u16 bkgColor, color_u16 objColor, short objXSize, short objYSize );

//��ȡ�µľ���λ��
void getAbsPos( const AreaRange *baseAbsPos, const AreaRange *relativePos, AreaRange *newAbsPos );

#endif /* SCREEN_DISPLAY_H_ */