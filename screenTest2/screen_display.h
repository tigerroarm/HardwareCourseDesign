/*
 * screen_display.h
 *
 *  Created on: 2019��5��1��
 *      Author: 12757
 */

#ifndef SCREEN_DISPLAY_H_
#define SCREEN_DISPLAY_H_


#include "screen_type.h"

//������������ʱ������Ļ��ʾЧ�������Ĺ��̻�ɾ��������
//include "screen_display.h"�Ŀ���Ի�ø������ʹ��Ȩ��
extern color_u16 screenShowSpace[SCR_HEIGHT][SCR_WIDTH];

//��ȡ�µľ���λ��
bool getAbsPos( const AreaRange *baseAbsPos, const AreaRange *relativePos, AreaRange *newAbsPos );

//��ȡx���������λ����(���������С��
short getXVaildRange( const char areaOccurInfo[], const AreaRange exceptAreaArray[], const short exceptAreaNum, \
		short *xMinArray, short *xMaxArray, const short width );

//���߽�
bool showBorder( const AreaRange *borderAbsPos, color_u16 borderColor );

//��ʾ�߽��뱳��(widthΪ�ܿ���)
bool showBkgAndBorder( const ColorInfo *bkgBorderColor, const AreaRange *absPos, const AreaRange exceptAreaArray[], const short exceptAreaNum );

//��ʾicon����text
bool showTextOrIcon( const AreaRange *absPos, const alt_u8 *objPicPtr, color_u16 bkgColor, color_u16 objColor, short objXSize, short objYSize );

//��ʾTagBlock
bool showTagBlock( const TagBlock *tagPtr, const AreaRange *absPos );
//��ʾTagBlock(���������ƫ��)
bool showTagWithYOffset( const TagBlock *tagPtr, const AreaRange *absPos, const short yOffset );

//��ʾIconBlock
bool showIconBlock( const IconBlock *iconPtr, const AreaRange *absPos );

//��ʾTagGroup
bool showTagGroup( const TagGroup *tagGroupInst, const AreaRange *absPos );

//��ʾTagIconGroup
bool showTagIconGroup( const TagIconGroup *tagIconGpInst, const AreaRange *absPos );

//��ʾTagList
bool showTagList( const TagList *tagListInst, const AreaRange *absPos, const short tagListYOffset );

//��ʾScrollBarY
bool showScrollBarY( const ScrollBarY *scrollBarYInst, const AreaRange *absPos );

//��ʾScreenPageInfo
bool showScreenPageInfo( const ScreenPageInfo *scrPageInfoInst, const AreaRange *absPos );

//��ʾProgressBarX
bool showProgressBarX( const ProgressBarX *prgBarXInst, const AreaRange *absPos );

//��ʾ��ɫ��
bool showColorBoard( const ColorBoard *colorTableInst, const AreaRange *absPos );


//��ʾ��ɫ����ѡ����ɫ�ı߽�
bool showColorSelBorder( const ColorBoard *colorTableInst, const AreaRange *absPos );
//�����ɫ����ѡ����ɫ�ı߽�
bool clearColorSelBorder( const ColorBoard *colorTableInst, const AreaRange *absPos );


//��ʾscreen_header
bool showScreenHeader( const ScreenHeaderTime *scrHeaderInst, const AreaRange *scrHeaderAbsPos );
//��ʾscreen_footer
bool showScreenFooter( const ScreenFooterBtn *scrFooterInst, const AreaRange *scrFooterAbsPos );
//��ʾscreen_main
bool showScreenMain( const ScreenContainer *scrContainerInst );
//��ʾscreen_main_home
bool showScreenMainHome( const ScreenHome *scrHomeInst, const AreaRange *scrMainAbsPos );
//��ʾscreen_main_book
bool showScreenMainBook( const ScreenBook *scrBookInst, const AreaRange *scrMainAbsPos );
//��ʾscreen_main_setting
bool showScreenMainSetting( const ScreenSetting *scrSettingInst, const AreaRange *scrMainAbsPos );
//��ʾscreen_main_color_picker
bool showScreenMainColorPicker( const ScreenColorPicker *scrColorPickerInst, const AreaRange *scrMainAbsPos );

//��ʾscreen
bool showScreen( );



#endif /* SCREEN_DISPLAY_H_ */