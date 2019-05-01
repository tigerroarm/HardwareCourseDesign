/*
 * screen.h
 *
 * ����ʵ����Ļ���ݵ���ʾ
 *
 *  Created on: 2019��4��27��
 *      Author: 12757
 */

#ifndef SCREEN_H_
#define SCREEN_H_

#include "screen_type.h"
#include "font_manage.h"
#include "txt_read.h"

//��������Χ
bool setAreaRange( AreaRange *areaPtr, short x_min, short x_max, short y_min, short y_max );
//������������Χ
bool setAreaRangeCentered( AreaRange *areaPtr, short width, short height, short objXSize, short objYSize );
//����Χ�Զ�ת��
bool setAreaRangeByAreaFmt( AreaRange *areaPtr, AreaFmt *areaFmtPos, short width, short height );
//���������ʽ
bool setAreaFmt( AreaFmt *areaFmtPos, enum areaFmtID fmtXID, enum areaFmtID fmtYID, short xC_xML, short xHf_xMR, short yC_yMU, short yHf_yMD );
//����Ĭ�ϱ�ǩ(�����ǩ�ڷŲ����ı����򷵻�false)
bool setDefaultTagBlock( TagBlock *tagPtr, char *text, short width, short height );
//����Ĭ��ͼ��
bool setDefaultIconBlock( IconBlock *iconPtr, const alt_u8 *iconModel, short width, short height, short iconXSize, short iconYSize );
//����Ĭ�Ͻ�����
bool setDefaultProgressBar( ProgressBar *prgBarInst, short width, short height );
//����Ĭ�ϴ�ֱ������
bool setDefaultScrollBarY( ScrollBarY *scrollBarYInst, short width, short height, float barYRatio );
//����Ĭ��ȡɫ����Ϣ
void setDefaultColorTable( ColorTable *colorPickerInst, color_u8 *colorSel, short width, short height );

//����Ĭ���ı���ǩ�б�
bool setDefaultTagList( TagList *TagListInst, short elemWidth, short elemHeight, TextList *textListPtr, bool elemBorderVisible,\
						ColorInfo *colorInfoPtr );

//����Ĭ�ϱ�ǩ��
bool setDefaultTagBlockGroup( TagBlockGroup *tagGroupInst, AreaFmt *tagStaticPos, AreaFmt *tagVarPos, short width, short height,\
			char *tagStaText, char *tagVarText );
//����Ĭ�Ͽɱ༭����(��̬��ǩ+��̬ͼ��)
bool setDefaultNumEditGroup( TagIconGroup *numEditGrpInst, short width, short height, short numMax, short numMin, short num, char *unitName );

//������Ч�ı�ȫ����ʾʱ�����ؿ��
short getWidthOfText( char *str, short len );

//��ʼ����Ļ
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
