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

#include <stdio.h>
#include "screen_type.h"
#include "font_manage.h"
#include "txt_read.h"

//������Ļ����
extern ScreenContainer tftlcdScreenInst;

//�������Χ�����Ϊ��ʽ�����򷵻�false
bool checkAreaRange( const AreaRange *areaPtr );

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
bool setDefaultProgressBarX( ProgressBarX *prgBarInst, short width, short height );
//����Ĭ�ϴ�ֱ������
bool setDefaultScrollBarY( ScrollBarY *scrollBarYInst, short width, short height, float barYRatio );
//����Ĭ��ȡɫ����Ϣ
void setDefaultColorBoard( ColorBoard *colorPickerInst, color_u8 *colorSel, short width, short height );

//����Ĭ���ı���ǩ�б�
bool setDefaultTagList( TagList *TagListInst, short elemWidth, short elemHeight, short textWidth,\
                       TextList *textListPtr, ColorInfo *colorInfoPtr );

//����Ĭ�ϱ�ǩ��
bool setDefaultTagGroup( TagGroup *tagGroupInst, AreaFmt *tagStaticPos, AreaFmt *tagVarPos, short width, short height,\
			char *tagStaText, char *tagVarText );
//����Ĭ�Ͽɱ༭����(��̬��ǩ+��̬ͼ��)
bool setDefaultNumEditGroup( TagIconGroup *numEditGrpInst, short width, short height, short numMax, short numMin, short num, char *unitName );

//������Ч�ı�ȫ����ʾʱ�����ؿ���
short getWidthOfText( TextType *textInst );

//��ȡ�ı��к�������ĸ�ķֲ���(���ض�̬���飬�ǵ�free)
char *getWordLetterArrange( TextType *textInst );

//�����ļ��������������(���򿪹�������)
bool openTxtScrollX( ScreenHome *scrHomeInst, int scrollTxtFileIndex );

//��ʼ����Ļ
bool screenInit();

bool screenHeaderInit( ScreenHeaderTime *scrHeaderInst, short width, short height );
bool screenFooterInit( ScreenFooterBtn *scrFooterInst, short width, short height );

bool screenMainHomeInit( ScreenHome *scrHomeInst, short width, short height, const TxtFilesInfo *txtFilesInfoPtr );
bool screenMainBookInit( ScreenBook *scrBookInst, short width, short height );
bool screenMainSettingInit( ScreenSetting *scrSettingInst, short width, short height );
bool screenMainColorPickerInit( ScreenColorPicker *scrColorPickerInst, short width, short height );

bool ScreenMainPageInfoInit( ScreenPageInfo *scrPageInfoInst, short width, short height );

void screenMainAreaInfoInst( ScrMainAreaInfo *mainAreaInfoInst );


//�����Զ������Ļ�ṹ������(�����Զ��������޸Ķ���������У����ڹ���)
void screenSetCustomInfo( ScreenContainer *screenContainerInst );



//screen��Ϣͬ������
//ͬ������ҳ�����Ϣ
void screenSyncSettingColor();
//ͬ��������ҳ�����ɫ
void screenSyncBookColor();

//ͬ��ʱ�䵽�ϱ�����ʱ���ǩ�е�ʱ���ַ���
void screenSyncHeaderTime();

//������ʱ��ͬ����timeFlag��
void syncTimeFlag();

//�����Զ���ҳ������ر�
void setAutoTPGstate( bool state );

//ͬ���ı������ʽ��BOOKҳ��
void screenSyncBookEcdfmt( enum encodingFmt fmt );

//ͬ��ҳ����ȱ�ǩ
bool screenSyncPageRatio( ScreenPageInfo *scrPageInfoInst );


#endif /* SCREEN_H_ */