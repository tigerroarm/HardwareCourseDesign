#ifndef TEST_VALUE_H_INCLUDED
#define TEST_VALUE_H_INCLUDED

#include <stdbool.h>
#include "screen_type.h"
#include "txt_read.h"

//数据显示库
//显示状态
void displayStatus( char *curFun, char *outerFun, bool status );

//显示TxtFilesInfo
void displayTxtFilesInfo( TxtFilesInfo *txtFilesInfoInst, int spaceNum );

//显示TxtFile
void displayTxtFile( TxtFile *txtFileInst, int spaceNum );

//显示TextType
void displayTextType( TextType *str, int spaceNum );

//malloc一个字符串，字符串内容全为charVal
//用完了记得free
char *getMallocStr( char charVal, char len );


//显示 ScreenContainer
void displayScreenContainer( ScreenContainer *scrContainerInst, int spaceNum );

//显示 AreaRange
void displayAreaRange( const AreaRange* areaRangeInst, int spaceNum );

//显示 ScreenHeaderTime
void displayScreenHeaderTime( ScreenHeaderTime* scrHeaderInst, int spaceNum );

//显示 ScreenFooterBtn
void displayScreenFooterBtn( ScreenFooterBtn* scrFooterInst, int spaceNum );

//显示 ScreenHome
void displayScreenHome( ScreenHome* scrHomeInst, int spaceNum );

//显示 ScreenBook
void displayScreenBook( ScreenBook* scrBookInst, int spaceNum );

//显示 ScreenSetting
void displayScreenSetting( ScreenSetting* scrSettingInst, int spaceNum );

//显示 ScreenColorPicker
void displayScreenColorPicker( ScreenColorPicker* scrColorPickerInst, int spaceNum );

//显示 ScrMainAreaInfo
void displayScrMainAreaInfo( ScrMainAreaInfo* scrMainAreaInfoInst, int spaceNum );

//显示 TagBlock
void displayTagBlock( TagBlock *tagPtr, int spaceNum );

//显示 IconBlock
void displayIconBlock( IconBlock *iconPtr, int spaceNum );

//显示 ColorInfo
void displayColorInfo( ColorInfo *colorInfoPtr, int spaceNum );

//显示 ColorBoard
void displayColorBoard( ColorBoard * colorBoardPtr, int spaceNum );

//显示 TagIconGroup
void displayTagIconGroup( TagIconGroup *tagIconGroupPtr, int spaceNum );

//显示 TagGroup
void displayTagGroup( TagGroup *tagGroupPtr, int spaceNum );

//显示 TagList
void displayTagList( TagList *tagListInst, int spaceNum );

//显示 ProgressBarX
void displayProgressBarX( ProgressBarX *prgBarXInst, int spaceNum );

//显示 ScrollBarY
void displayScrollBarY( ScrollBarY *scrollBarYInst, int spaceNum );

//显示 ScreenPageInfo
void displayScreenPageInfo( ScreenPageInfo *scrPageInfoInst, int spaceNum );

//显示 颜色中RGB分量强度
void displayColorRGB( color_u16 colorVal );

//显示 图标
void displayObjPic( const alt_u8 *objPicPtr, short objXSize, short objYSize, int spaceNum );

//将生成的屏幕数组保存到文件中
void storeScreenArrayToFile( void );

int WriteArray2File(FILE* fp, int row, int column);

//将屏幕数组置为0
void clearScreenArray();

#endif // TEST_VALUE_H_INCLUDED
