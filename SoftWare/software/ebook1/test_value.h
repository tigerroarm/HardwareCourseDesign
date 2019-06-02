#ifndef TEST_VALUE_H_INCLUDED
#define TEST_VALUE_H_INCLUDED

#include <stdbool.h>
#include "screen_type.h"
#include "txt_read.h"

//������ʾ��
//��ʾ״̬
void displayStatus( char *curFun, char *outerFun, bool status );

//��ʾTxtFilesInfo
void displayTxtFilesInfo( TxtFilesInfo *txtFilesInfoInst, int spaceNum );

//��ʾTxtFile
void displayTxtFile( TxtFile *txtFileInst, int spaceNum );

//��ʾTextType
void displayTextType( TextType *str, int spaceNum );

//mallocһ���ַ������ַ�������ȫΪcharVal
//�����˼ǵ�free
char *getMallocStr( char charVal, char len );


//��ʾ ScreenContainer
void displayScreenContainer( ScreenContainer *scrContainerInst, int spaceNum );

//��ʾ AreaRange
void displayAreaRange( const AreaRange* areaRangeInst, int spaceNum );

//��ʾ ScreenHeaderTime
void displayScreenHeaderTime( ScreenHeaderTime* scrHeaderInst, int spaceNum );

//��ʾ ScreenFooterBtn
void displayScreenFooterBtn( ScreenFooterBtn* scrFooterInst, int spaceNum );

//��ʾ ScreenHome
void displayScreenHome( ScreenHome* scrHomeInst, int spaceNum );

//��ʾ ScreenBook
void displayScreenBook( ScreenBook* scrBookInst, int spaceNum );

//��ʾ ScreenSetting
void displayScreenSetting( ScreenSetting* scrSettingInst, int spaceNum );

//��ʾ ScreenColorPicker
void displayScreenColorPicker( ScreenColorPicker* scrColorPickerInst, int spaceNum );

//��ʾ ScrMainAreaInfo
void displayScrMainAreaInfo( ScrMainAreaInfo* scrMainAreaInfoInst, int spaceNum );

//��ʾ TagBlock
void displayTagBlock( TagBlock *tagPtr, int spaceNum );

//��ʾ IconBlock
void displayIconBlock( IconBlock *iconPtr, int spaceNum );

//��ʾ ColorInfo
void displayColorInfo( ColorInfo *colorInfoPtr, int spaceNum );

//��ʾ ColorBoard
void displayColorBoard( ColorBoard * colorBoardPtr, int spaceNum );

//��ʾ TagIconGroup
void displayTagIconGroup( TagIconGroup *tagIconGroupPtr, int spaceNum );

//��ʾ TagGroup
void displayTagGroup( TagGroup *tagGroupPtr, int spaceNum );

//��ʾ TagList
void displayTagList( TagList *tagListInst, int spaceNum );

//��ʾ ProgressBarX
void displayProgressBarX( ProgressBarX *prgBarXInst, int spaceNum );

//��ʾ ScrollBarY
void displayScrollBarY( ScrollBarY *scrollBarYInst, int spaceNum );

//��ʾ ScreenPageInfo
void displayScreenPageInfo( ScreenPageInfo *scrPageInfoInst, int spaceNum );

//��ʾ ��ɫ��RGB����ǿ��
void displayColorRGB( color_u16 colorVal );

//��ʾ ͼ��
void displayObjPic( const alt_u8 *objPicPtr, short objXSize, short objYSize, int spaceNum );

//�����ɵ���Ļ���鱣�浽�ļ���
void storeScreenArrayToFile( void );

int WriteArray2File(FILE* fp, int row, int column);

//����Ļ������Ϊ0
void clearScreenArray();

#endif // TEST_VALUE_H_INCLUDED