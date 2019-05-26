#include <stdio.h>
#include <stdlib.h>

#include "test_value.h"
#include "alt_types.h"

extern color_u16 screenShowSpace[SCR_HEIGHT][SCR_WIDTH];

//curFun为当前执行函数的名字，outerFun是调用该函数的函数名
void displayStatus( char *curFun, char *outerFun, bool status )
{
    printf( "%s() in %s() status = %d\n", curFun, outerFun, status );
}

//显示TxtFilesInfo
void displayTxtFilesInfo( TxtFilesInfo *txtFilesInfoInst, int spaceNum )
{

    char *spaceStr = getMallocStr( ' ', spaceNum );
    if ( txtFilesInfoInst == NULL )
    {
        printf( "%sNULL\n", spaceStr );
        free(spaceStr);
        return;
    }

    printf( "%s", spaceStr );
    printf( "TxtFile * txtFileList = %p\n", txtFilesInfoInst->txtFileList );
    printf( "%s", spaceStr );
    printf( "int txtFilesNum = %d\n", txtFilesInfoInst->txtFilesNum );
    printf( "%s", spaceStr );
    printf( "int curOpenFileIndex = %d\n", txtFilesInfoInst->curOpenFileIndex );


    TxtFile *txtFileListPtr = txtFilesInfoInst->txtFileList;//动态数组（第一次使用时需要malloc分配空间）
	int txtFilesNum = txtFilesInfoInst->txtFilesNum;//最大值为99，即最多读取SD卡中的99个文件



    if ( txtFileListPtr != NULL )
    {
        printf( "\n" );
        int i;
        for ( i = 0; i < txtFilesNum; i ++ )
        {
            printf( "%s", spaceStr );
            printf( "txtFileList[%d]={\n", i );
            displayTxtFile( txtFileListPtr, spaceNum + 4 );
            txtFileListPtr ++;
            printf( "%s", spaceStr );
            printf( "}\n" );
        }
    }

    free(spaceStr);
}

//显示TxtFile
void displayTxtFile( TxtFile *txtFileInst, int spaceNum )
{
    char *spaceStr = getMallocStr( ' ', spaceNum );
    if ( txtFileInst == NULL )
    {
        printf( "%sNULL\n", spaceStr );
        free(spaceStr);
        return;
    }
    printf( "%sTextType txtFileName = {\n", spaceStr );
    displayTextType( &(txtFileInst->txtFileName), spaceNum + 4 );
    printf( "%s}\n", spaceStr );
    printf( "%s", spaceStr );
    printf( "int curPageNum = %d\n", txtFileInst->curPageNum );
    printf( "%s", spaceStr );
    printf( "int totalPageNum = %d\n", txtFileInst->totalPageNum );
    printf( "%s", spaceStr );
    printf( "float curPageRatio = %f\n", txtFileInst->curPageRatio );
    printf( "%s", spaceStr );
    printf( "unsigned int curPageBytePosStart = %d\n", txtFileInst->curPageBytePosStart );
    printf( "%s", spaceStr );
    printf( "unsigned int curPageBytePosEnd = %d\n", txtFileInst->curPageBytePosEnd );
    printf( "%s", spaceStr );
    printf( "unsigned int fileSizeInBytes = %d\n", txtFileInst->fileSizeInBytes );

	free(spaceStr);
}

//显示TextType
void displayTextType( TextType *str, int spaceNum )
{
    char *spaceStr = getMallocStr( ' ', spaceNum );
    if ( str == NULL )
    {
        printf( "%sNULL\n", spaceStr );
        free(spaceStr);
        return;
    }
    printf( "%s", spaceStr );
    printf( "char *text = \"%s\"\n", str->text );
    printf( "%s", spaceStr );
    printf( "short textLen = %d\n", str->textLen );
    free(spaceStr);
}

//malloc一个字符串，字符串内容全为charVal
//用完了记得free
char* getMallocStr( char charVal, char len )
{
    char *charStr = (char*)malloc( sizeof(char)*(len+1) );
    int i;
    for ( i = 0; i < len; i ++ )
    {
        charStr[i] = charVal;
    }
    charStr[i] = 0;
    return charStr;
}

//显示 ScreenContainer
void displayScreenContainer( ScreenContainer *scrContainerInst, int spaceNum )
{
	char *spaceStr = getMallocStr( ' ', spaceNum );
    if ( scrContainerInst == NULL )
    {
        printf( "%sNULL\n", spaceStr );
        free(spaceStr);
        return;
    }

//    printf( "%sAreaRange screenArea = {\n", spaceStr );
//    displayAreaRange( &(scrContainerInst->screenArea), spaceNum + 4 );
//    printf( "%s}\n", spaceStr );
//
//    printf( "%sAreaRange headerArea = {\n", spaceStr );
//    displayAreaRange( &(scrContainerInst->headerArea), spaceNum + 4 );
//    printf( "%s}\n", spaceStr );
//
//    printf( "%sAreaRange footerArea = {\n", spaceStr );
//    displayAreaRange( &(scrContainerInst->footerArea), spaceNum + 4 );
//    printf( "%s}\n", spaceStr );

//    printf( "%sAreaRange mainArea = {\n", spaceStr );
//    displayAreaRange( &(scrContainerInst->mainArea), spaceNum + 4 );
//    printf( "%s}\n", spaceStr );
//



//    printf( "%sScreenHeaderTime scrHeader = {\n", spaceStr );
//    displayScreenHeaderTime( &(scrContainerInst->scrHeader), spaceNum + 4 );
//    printf( "%s}\n", spaceStr );

//    printf( "%sScreenFooterBtn scrFooter = {\n", spaceStr );
//    displayScreenFooterBtn( &(scrContainerInst->scrFooter), spaceNum + 4 );
//    printf( "%s}\n", spaceStr );
//
//    printf( "%sScreenHome scrHome = {\n", spaceStr );
//    displayScreenHome( &(scrContainerInst->scrHome), spaceNum + 4 );
//    printf( "%s}\n", spaceStr );

    printf( "%sScreenBook scrBook = {\n", spaceStr );
    displayScreenBook( &(scrContainerInst->scrBook), spaceNum + 4 );
    printf( "%s}\n", spaceStr );

//    printf( "%sScreenSetting scrSetting = {\n", spaceStr );
//    displayScreenSetting( &(scrContainerInst->scrSetting), spaceNum + 4 );
//    printf( "%s}\n", spaceStr );

//    printf( "%sScreenColorPicker scrColorPicker = {\n", spaceStr );
//    displayScreenColorPicker( &(scrContainerInst->scrColorPicker), spaceNum + 4 );
//    printf( "%s}\n", spaceStr );

//    printf( "%sScrMainAreaInfo mainAreaInfo = {\n", spaceStr );
//    displayScrMainAreaInfo( &(scrContainerInst->mainAreaInfo), spaceNum + 4 );
//    printf( "%s}\n", spaceStr );

	free( spaceStr );
}
//显示 AreaRange
void displayAreaRange( const AreaRange* areaRangeInst, int spaceNum )
{
	char *spaceStr = getMallocStr( ' ', spaceNum );
    if ( areaRangeInst == NULL )
    {
        printf( "%sNULL\n", spaceStr );
        free(spaceStr);
        return;
    }

    //打印为图像

    printf( "%s", spaceStr );
    printf( "   ____________\n" );
    printf( "%s", spaceStr );
    printf( "%3d|          |    height = %3d\n", areaRangeInst->y_min, \
           areaRangeInst->y_max - areaRangeInst->y_min + 1 );
    printf( "%s", spaceStr );
    printf( "   |          |\n" );
    printf( "%s", spaceStr );
    printf( "%3d|__________|     width = %3d\n", areaRangeInst->y_max, \
           areaRangeInst->x_max - areaRangeInst->x_min + 1 );
    printf( "%s", spaceStr );
    printf( "   %3d     %3d\n", areaRangeInst->x_min, areaRangeInst->x_max );



    //打印为区间
//    printf( "%s", spaceStr );
//    printf( "short [x_min, x_max] = [%d, %d]\n", areaRangeInst->x_min, areaRangeInst->x_max );
//
//    printf( "%s", spaceStr );
//    printf( "short [y_min, y_max] = [%d, %d]\n", areaRangeInst->y_min, areaRangeInst->y_max );

    //直接打印
//    printf( "%s", spaceStr );
//    printf( "short x_min = %d\n", areaRangeInst->x_min );
//
//    printf( "%s", spaceStr );
//    printf( "short x_max = %d\n", areaRangeInst->x_max );
//
//    printf( "%s", spaceStr );
//    printf( "short y_min = %d\n", areaRangeInst->y_min );
//
//    printf( "%s", spaceStr );
//    printf( "short y_max = %d\n", areaRangeInst->y_max );

    free(spaceStr);
}

//显示 ScreenHeaderTime
void displayScreenHeaderTime( ScreenHeaderTime* scrHeaderInst, int spaceNum )
{
	char *spaceStr = getMallocStr( ' ', spaceNum );
    if ( scrHeaderInst == NULL )
    {
        printf( "%sNULL\n", spaceStr );
        free(spaceStr);
        return;
    }


    printf( "%sColorInfo bkgBorderColor = {\n", spaceStr );
    displayColorInfo( &(scrHeaderInst->bkgBorderColor), spaceNum + 4 );
    printf( "%s}\n", spaceStr );


    printf( "%sAreaRange tagTimeArea = {\n", spaceStr );
    displayAreaRange( &(scrHeaderInst->tagTimeArea), spaceNum + 4 );
    printf( "%s}\n", spaceStr );


    printf( "%sTagBlock tagTime = {\n", spaceStr );

    displayTagBlock( &(scrHeaderInst->tagTime), spaceNum + 4 );
    printf( "%s}\n", spaceStr );

    free(spaceStr);
}

//显示 ScreenFooterBtn
void displayScreenFooterBtn( ScreenFooterBtn* scrFooterInst, int spaceNum )
{
	char *spaceStr = getMallocStr( ' ', spaceNum );
    if ( scrFooterInst == NULL )
    {
        printf( "%sNULL\n", spaceStr );
        free(spaceStr);
        return;
    }


    printf( "%sColorInfo bkgBorderColor = {\n", spaceStr );
    displayColorInfo( &(scrFooterInst->bkgBorderColor), spaceNum + 4 );
    printf( "%s}\n", spaceStr );



    printf( "%sAreaRange iconSettingArea = {\n", spaceStr );
    displayAreaRange( &(scrFooterInst->iconSettingArea), spaceNum + 4 );
    printf( "%s}\n", spaceStr );

    printf( "%sAreaRange iconHomeArea = {\n", spaceStr );
    displayAreaRange( &(scrFooterInst->iconHomeArea), spaceNum + 4 );
    printf( "%s}\n", spaceStr );

    printf( "%sAreaRange iconBackArea = {\n", spaceStr );
    displayAreaRange( &(scrFooterInst->iconBackArea), spaceNum + 4 );
    printf( "%s}\n", spaceStr );



    printf( "%sIconBlock iconSetting = {\n", spaceStr );
    displayIconBlock( &(scrFooterInst->iconSetting), spaceNum + 4 );
    printf( "%s}\n", spaceStr );

    printf( "%sIconBlock iconHome = {\n", spaceStr );
    displayIconBlock( &(scrFooterInst->iconHome), spaceNum + 4 );
    printf( "%s}\n", spaceStr );

    printf( "%sIconBlock iconBack = {\n", spaceStr );
    displayIconBlock( &(scrFooterInst->iconBack), spaceNum + 4 );
    printf( "%s}\n", spaceStr );

    free(spaceStr);
}

//显示 ScreenHome
void displayScreenHome( ScreenHome* scrHomeInst, int spaceNum )
{
	char *spaceStr = getMallocStr( ' ', spaceNum );
    if ( scrHomeInst == NULL )
    {
        printf( "%sNULL\n", spaceStr );
        free(spaceStr);
        return;
    }

    printf( "%sColorInfo bkgBorderColor = {\n", spaceStr );
    displayColorInfo( &(scrHomeInst->bkgBorderColor), spaceNum + 4 );
    printf( "%s}\n", spaceStr );



    printf( "%sAreaRange numBarArea = {\n", spaceStr );
    displayAreaRange( &(scrHomeInst->numBarArea), spaceNum + 4 );
    printf( "%s}\n", spaceStr );

    printf( "%sAreaRange txtCatalogArea = {\n", spaceStr );
    displayAreaRange( &(scrHomeInst->txtCatalogArea), spaceNum + 4 );
    printf( "%s}\n", spaceStr );

    printf( "%sAreaRange turnCatalogBarArea = {\n", spaceStr );
    displayAreaRange( &(scrHomeInst->turnCatalogBarArea), spaceNum + 4 );
    printf( "%s}\n", spaceStr );



    printf( "%s", spaceStr );
    printf( "short listYOffset = %d\n", scrHomeInst->listYOffset );
    printf( "%s", spaceStr );
    printf( "short listYOffsetMax = %d\n", scrHomeInst->listYOffsetMax );
    printf( "%s", spaceStr );
    printf( "short listYSize = %d\n", scrHomeInst->listYSize );
    printf( "%s", spaceStr );
    printf( "short listYStepSize = %d\n", scrHomeInst->listYStepSize );
    printf( "%s", spaceStr );
    printf( "short listShowHeight = %d\n", scrHomeInst->listShowHeight );
    printf( "%s", spaceStr );
    printf( "float listShowYRatio = %f\n", scrHomeInst->listShowYRatio );



    printf( "%s", spaceStr );
    printf( "bool txtScrollable = %s\n", (scrHomeInst->txtScrollable)?"ture":"false" );
    printf( "%s", spaceStr );
    printf( "short txtScrollrow = %d\n", scrHomeInst->txtScrollrow );
    printf( "%s", spaceStr );
    printf( "short txtScrollXOffset = %d\n", scrHomeInst->txtScrollXOffset );
    printf( "%s", spaceStr );
    printf( "short txtScrollXStepSize = %d\n", scrHomeInst->txtScrollXStepSize );
    printf( "%s", spaceStr );
    printf( "short txtScrollwidth = %d\n", scrHomeInst->txtScrollwidth );
    printf( "%s", spaceStr );
    printf( "short txtScrollSpacing = %d\n", scrHomeInst->txtScrollSpacing );



    printf( "%sTagList numBar = {\n", spaceStr );
    displayTagList( &(scrHomeInst->numBar), spaceNum + 4 );
    printf( "%s}\n", spaceStr );

    printf( "%sTagList txtCatalog = {\n", spaceStr );
    displayTagList( &(scrHomeInst->txtCatalog), spaceNum + 4 );
    printf( "%s}\n", spaceStr );



    printf( "%sScrollBarY turnCatalogBarY = {\n", spaceStr );
    displayScrollBarY( &(scrHomeInst->turnCatalogBarY), spaceNum + 4 );
    printf( "%s}\n", spaceStr );


    free(spaceStr);
}

//显示 ScreenBook
void displayScreenBook( ScreenBook* scrBookInst, int spaceNum )
{
	char *spaceStr = getMallocStr( ' ', spaceNum );
    if ( scrBookInst == NULL )
    {
        printf( "%sNULL\n", spaceStr );
        free(spaceStr);
        return;
    }

    printf( "%sColorInfo bookColorInfo = {\n", spaceStr );
    displayColorInfo( &(scrBookInst->bookColorInfo), spaceNum + 4 );
    printf( "%s}\n", spaceStr );



    printf( "%sAreaRange txtBookArea = {\n", spaceStr );
    displayAreaRange( &(scrBookInst->txtBookArea), spaceNum + 4 );
    printf( "%s}\n", spaceStr );

    printf( "%sAreaRange txtBookNarrowArea = {\n", spaceStr );
    displayAreaRange( &(scrBookInst->txtBookNarrowArea), spaceNum + 4 );
    printf( "%s}\n", spaceStr );

    printf( "%sAreaRange pageInfoArea = {\n", spaceStr );
    displayAreaRange( &(scrBookInst->pageInfoArea), spaceNum + 4 );
    printf( "%s}\n", spaceStr );

    printf( "%sAreaRange turnBackPageAreaPos = {\n", spaceStr );
    displayAreaRange( &(scrBookInst->turnBackPageAreaPos), spaceNum + 4 );
    printf( "%s}\n", spaceStr );

    printf( "%sAreaRange turnNextPageAreaPos = {\n", spaceStr );
    displayAreaRange( &(scrBookInst->turnNextPageAreaPos), spaceNum + 4 );
    printf( "%s}\n", spaceStr );

    printf( "%sAreaRange openPageInfoAreaPos = {\n", spaceStr );
    displayAreaRange( &(scrBookInst->openPageInfoAreaPos), spaceNum + 4 );
    printf( "%s}\n", spaceStr );


//
//    printf( "%sTagList txtBook = {\n", spaceStr );
//    displayTagList ( &(scrBookInst->txtBook), spaceNum + 4 );
//    printf( "%s}\n", spaceStr );
//


    printf( "%s", spaceStr );
    printf( "bool pageInfoVisible = %s\n", (scrBookInst->pageInfoVisible)?"true":"false" );



    printf( "%sScreenPageInfo pageInfo = {\n", spaceStr );
    displayScreenPageInfo( &(scrBookInst->pageInfo), spaceNum + 4 );
    printf( "%s}\n", spaceStr );



    free(spaceStr);
}

//显示 ScreenSetting
void displayScreenSetting( ScreenSetting* scrSettingInst, int spaceNum )
{
	char *spaceStr = getMallocStr( ' ', spaceNum );
    if ( scrSettingInst == NULL )
    {
        printf( "%sNULL\n", spaceStr );
        free(spaceStr);
        return;
    }

    printf( "%sColorInfo bkgBorderColor = {\n", spaceStr );
    displayColorInfo( &(scrSettingInst->bkgBorderColor), spaceNum + 4 );
    printf( "%s}\n", spaceStr );



    printf( "%sAreaRange tag1Area = {\n", spaceStr );
    displayAreaRange( &(scrSettingInst->tag1Area), spaceNum + 4 );
    printf( "%s}\n", spaceStr );

    printf( "%sAreaRange editAlarmHourArea = {\n", spaceStr );
    displayAreaRange( &(scrSettingInst->editAlarmHourArea), spaceNum + 4 );
    printf( "%s}\n", spaceStr );

    printf( "%sAreaRange editAlarmMinuteArea = {\n", spaceStr );
    displayAreaRange( &(scrSettingInst->editAlarmMinuteArea), spaceNum + 4 );
    printf( "%s}\n", spaceStr );

    printf( "%sAreaRange editBkgColorArea = {\n", spaceStr );
    displayAreaRange( &(scrSettingInst->editBkgColorArea), spaceNum + 4 );
    printf( "%s}\n", spaceStr );

    printf( "%sAreaRange editWordColorArea = {\n", spaceStr );
    displayAreaRange( &(scrSettingInst->editWordColorArea), spaceNum + 4 );
    printf( "%s}\n", spaceStr );

    printf( "%sAreaRange editTurnPageModArea = {\n", spaceStr );
    displayAreaRange( &(scrSettingInst->editTurnPageModArea), spaceNum + 4 );
    printf( "%s}\n", spaceStr );

    printf( "%sAreaRange tag2Area = {\n", spaceStr );
    displayAreaRange( &(scrSettingInst->tag2Area), spaceNum + 4 );
    printf( "%s}\n", spaceStr );

    printf( "%sAreaRange editTurnPageSecArea = {\n", spaceStr );
    displayAreaRange( &(scrSettingInst->editTurnPageSecArea), spaceNum + 4 );
    printf( "%s}\n", spaceStr );



    printf( "%sTagBlock tag1 = {\n", spaceStr );
    displayTagBlock( &(scrSettingInst->tag1), spaceNum + 4 );
    printf( "%s}\n", spaceStr );

    printf( "%sTagBlock tag2 = {\n", spaceStr );
    displayTagBlock( &(scrSettingInst->tag2), spaceNum + 4 );
    printf( "%s}\n", spaceStr );



    printf( "%sTagIconGroup editAlarmHour = {\n", spaceStr );
    displayTagIconGroup( &(scrSettingInst->editAlarmHour), spaceNum + 4 );
    printf( "%s}\n", spaceStr );

    printf( "%sTagIconGroup editAlarmMinute = {\n", spaceStr );
    displayTagIconGroup( &(scrSettingInst->editAlarmMinute), spaceNum + 4 );
    printf( "%s}\n", spaceStr );

    printf( "%sTagIconGroup editTurnPageSec = {\n", spaceStr );
    displayTagIconGroup( &(scrSettingInst->editTurnPageSec), spaceNum + 4 );
    printf( "%s}\n", spaceStr );



    printf( "%sTagGroup editBkgColor = {\n", spaceStr );
    displayTagGroup( &(scrSettingInst->editBkgColor), spaceNum + 4 );
    printf( "%s}\n", spaceStr );

    printf( "%sTagGroup editWordColor = {\n", spaceStr );
    displayTagGroup( &(scrSettingInst->editWordColor), spaceNum + 4 );
    printf( "%s}\n", spaceStr );

    printf( "%sTagGroup editTurnPageMod = {\n", spaceStr );
    displayTagGroup( &(scrSettingInst->editTurnPageMod), spaceNum + 4 );
    printf( "%s}\n", spaceStr );


    printf( "%s", spaceStr );
    printf( "color_u8 bkgColorIndex = %d\n", scrSettingInst->bkgColorIndex );
    printf( "%s", spaceStr );
    printf( "color_u8 txtColorIndex = %d\n", scrSettingInst->txtColorIndex );
    printf( "%s", spaceStr );
    printf( "bool autoTurnPage = %s\n", (scrSettingInst->autoTurnPage)?"true":"false" );

    free(spaceStr);

}

//显示 ScreenColorPicker
void displayScreenColorPicker( ScreenColorPicker* scrColorPickerInst, int spaceNum )
{

	char *spaceStr = getMallocStr( ' ', spaceNum );
    if ( scrColorPickerInst == NULL )
    {
        printf( "%sNULL\n", spaceStr );
        free(spaceStr);
        return;
    }

    printf( "%sColorInfo bkgBorderColor = {\n", spaceStr );
    displayColorInfo( &(scrColorPickerInst->bkgBorderColor), spaceNum + 4 );
    printf( "%s}\n", spaceStr );


    printf( "%sAreaRange colorPickerArea = {\n", spaceStr );
    displayAreaRange( &(scrColorPickerInst->colorPickerArea), spaceNum + 4 );
    printf( "%s}\n", spaceStr );


    printf( "%sColorBoard colorPicker = {\n", spaceStr );
    displayColorBoard( &(scrColorPickerInst->colorPicker), spaceNum + 4 );
    printf( "%s}\n", spaceStr );


    free(spaceStr);
}

//显示 ScrMainAreaInfo
void displayScrMainAreaInfo( ScrMainAreaInfo* scrMainAreaInfoInst, int spaceNum )
{
	char *spaceStr = getMallocStr( ' ', spaceNum );
    if ( scrMainAreaInfoInst == NULL )
    {
        printf( "%sNULL\n", spaceStr );
        free(spaceStr);
        return;
    }

    printf( "%s", spaceStr );
    printf( "short curScrIndex = %d\n", scrMainAreaInfoInst->curScrIndex );
    printf( "%s", spaceStr );
    printf( "enum scrID scrIDStack[] = %p\n", scrMainAreaInfoInst->scrIDStack );


    int i;

    char enumScrIDName[5][17] = { "SCR_NONE","SCR_HOME","SCR_BOOK","SCR_SETTING","SCR_COLOR_PICKER" };

    for ( i = 0; i < 4; i ++ )
    {
        printf( "%senum scrID scrIDStack[%d] = ", spaceStr, i );
        enum scrID curID = (scrMainAreaInfoInst->scrIDStack)[i];
        printf( "%s\n", enumScrIDName[(int)curID] );
    }

    free(spaceStr);

}

//显示 TagBlock
void displayTagBlock( TagBlock *tagPtr, int spaceNum )
{
	char *spaceStr = getMallocStr( ' ', spaceNum );
    if ( tagPtr == NULL )
    {
        printf( "%sNULL\n", spaceStr );
        free(spaceStr);
        return;
    }

    printf( "%sColorInfo tagColorInfo = {\n", spaceStr );
    displayColorInfo( &(tagPtr->tagColorInfo), spaceNum + 4 );
    printf( "%s}\n", spaceStr );


    printf( "%sAreaRange textArea = {\n", spaceStr );
    displayAreaRange( &(tagPtr->textArea), spaceNum + 4 );
    printf( "%s}\n", spaceStr );

    printf( "%sTextType tagText = {\n", spaceStr );
    displayTextType( &(tagPtr->tagText), spaceNum + 4 );
    printf( "%s}\n", spaceStr );


    printf( "%s", spaceStr );
    printf( "short wordSpacing = %d\n", tagPtr->wordSpacing );
    printf( "%s", spaceStr );
    printf( "short letterSpacing = %d\n", tagPtr->letterSpacing );
    printf( "%s", spaceStr );
    printf( "short wordXSize = %d\n", tagPtr->wordXSize );
    printf( "%s", spaceStr );
    printf( "short letterXSize = %d\n", tagPtr->letterXSize );
    printf( "%s", spaceStr );
    printf( "short objYSize = %d\n", tagPtr->objYSize );


    free(spaceStr);

}

//显示 IconBlock
void displayIconBlock( IconBlock *iconPtr, int spaceNum )
{
	char *spaceStr = getMallocStr( ' ', spaceNum );
    if ( iconPtr == NULL )
    {
        printf( "%sNULL\n", spaceStr );
        free(spaceStr);
        return;
    }



    printf( "%sColorInfo iconColorInfo = {\n", spaceStr );
    displayColorInfo( &(iconPtr->iconColorInfo), spaceNum + 4 );
    printf( "%s}\n", spaceStr );



    printf( "%sAreaRange picArea = {\n", spaceStr );
    displayAreaRange( &(iconPtr->picArea), spaceNum + 4 );
    printf( "%s}\n", spaceStr );



    printf( "%s", spaceStr );
    printf( "short iconXSize = %d\n", iconPtr->iconXSize );
    printf( "%s", spaceStr );
    printf( "short iconYSize = %d\n", iconPtr->iconYSize );
    printf( "%s", spaceStr );
    printf( "const alt_u8 *iconPic = %p\n", iconPtr->iconPic );


    printf( "%sconst alt_u8 iconPic[%d×%d] = {\n", spaceStr, iconPtr->iconXSize, iconPtr->iconYSize );
    displayObjPic( iconPtr->iconPic, iconPtr->iconXSize, iconPtr->iconYSize, spaceNum + 4 );
    printf( "%s}\n", spaceStr );


    free(spaceStr);


}
//显示 ColorInfo
void displayColorInfo( ColorInfo *colorInfoPtr, int spaceNum )
{
	char *spaceStr = getMallocStr( ' ', spaceNum );
    if ( colorInfoPtr == NULL )
    {
        printf( "%sNULL\n", spaceStr );
        free(spaceStr);
        return;
    }

	printf( "%s", spaceStr );
    printf( "color_u16 borderColor = %04x ", colorInfoPtr->borderColor );
    displayColorRGB( colorInfoPtr->borderColor );
    printf( "\n%s", spaceStr );
    printf( "color_u16 bkgColor    = %04x ", colorInfoPtr->bkgColor );
    displayColorRGB( colorInfoPtr->bkgColor );
    printf( "\n%s", spaceStr );
    printf( "color_u16 objColor    = %04x ", colorInfoPtr->objColor );
    displayColorRGB( colorInfoPtr->objColor );
    printf( "\n%s", spaceStr );
    printf( "bool borderVisible = %s\n", (colorInfoPtr->borderVisible)?"true":"false" );
    printf( "%s", spaceStr );
    printf( "bool objVisible = %s\n", (colorInfoPtr->objVisible)?"true":"false" );


    free(spaceStr);
}

//显示 ColorBoard
void displayColorBoard( ColorBoard * colorBoardPtr, int spaceNum )
{
    char *spaceStr = getMallocStr( ' ', spaceNum );
    if ( colorBoardPtr == NULL )
    {
        printf( "%sNULL\n", spaceStr );
        free(spaceStr);
        return;
    }


    printf( "%s", spaceStr );
    printf( "short colorXSize = %d\n", colorBoardPtr->colorXSize );
    printf( "%s", spaceStr );
    printf( "short colorYSize = %d\n", colorBoardPtr->colorYSize );

    printf( "%s", spaceStr );
    printf( "short colorXNum = %d\n", colorBoardPtr->colorXNum );
    printf( "%s", spaceStr );
    printf( "short colorYNum = %d\n", colorBoardPtr->colorYNum );

    printf( "%s", spaceStr );
    printf( "const color_u16 *colorSpace = %p\n", colorBoardPtr->colorSpace );
    printf( "%s", spaceStr );
    printf( "color_u8 *colorIndex = %p\n", colorBoardPtr->colorIndex );


    free(spaceStr);
}

//显示 TagIconGroup
void displayTagIconGroup( TagIconGroup *tagIconGroupPtr, int spaceNum )
{
	char *spaceStr = getMallocStr( ' ', spaceNum );
    if ( tagIconGroupPtr == NULL )
    {
        printf( "%sNULL\n", spaceStr );
        free(spaceStr);
        return;
    }

	printf( "%sColorInfo bkgBorderColor = {\n", spaceStr );
    displayColorInfo( &(tagIconGroupPtr->bkgBorderColor), spaceNum + 4 );
    printf( "%s}\n", spaceStr );



    printf( "%sAreaRange plusIconArea = {\n", spaceStr );
    displayAreaRange( &(tagIconGroupPtr->plusIconArea), spaceNum + 4 );
    printf( "%s}\n", spaceStr );

    printf( "%sAreaRange minusIconArea = {\n", spaceStr );
    displayAreaRange( &(tagIconGroupPtr->minusIconArea), spaceNum + 4 );
    printf( "%s}\n", spaceStr );

    printf( "%sAreaRange valueTagArea = {\n", spaceStr );
    displayAreaRange( &(tagIconGroupPtr->valueTagArea), spaceNum + 4 );
    printf( "%s}\n", spaceStr );

    printf( "%sAreaRange unitTagArea = {\n", spaceStr );
    displayAreaRange( &(tagIconGroupPtr->unitTagArea), spaceNum + 4 );
    printf( "%s}\n", spaceStr );



    printf( "%sIconBlock plusIcon = {\n", spaceStr );
    displayIconBlock( &(tagIconGroupPtr->plusIcon), spaceNum + 4 );
    printf( "%s}\n", spaceStr );

    printf( "%sIconBlock minusIcon = {\n", spaceStr );
    displayIconBlock( &(tagIconGroupPtr->minusIcon), spaceNum + 4 );
    printf( "%s}\n", spaceStr );



    printf( "%sTagBlock valueTag = {\n", spaceStr );
    displayTagBlock( &(tagIconGroupPtr->valueTag), spaceNum + 4 );
    printf( "%s}\n", spaceStr );

    printf( "%sTagBlock unitTag = {\n", spaceStr );
    displayTagBlock( &(tagIconGroupPtr->unitTag), spaceNum + 4 );
    printf( "%s}\n", spaceStr );



    printf( "%s", spaceStr );
    printf( "short value = %d\n", tagIconGroupPtr->value );
    printf( "%s", spaceStr );
    printf( "short valueMax = %d\n", tagIconGroupPtr->valueMax );
    printf( "%s", spaceStr );
    printf( "short valueMin = %d\n", tagIconGroupPtr->valueMin );


	free(spaceStr);

}

//显示 TagGroup
void displayTagGroup( TagGroup *tagGroupPtr, int spaceNum )
{
	char *spaceStr = getMallocStr( ' ', spaceNum );
    if ( tagGroupPtr == NULL )
    {
        printf( "%sNULL\n", spaceStr );
        free(spaceStr);
        return;
    }

	printf( "%sColorInfo bkgBorderColor = {\n", spaceStr );
    displayColorInfo( &(tagGroupPtr->bkgBorderColor), spaceNum + 4 );
    printf( "%s}\n", spaceStr );



    printf( "%sAreaRange staticTagArea = {\n", spaceStr );
    displayAreaRange( &(tagGroupPtr->staticTagArea), spaceNum + 4 );
    printf( "%s}\n", spaceStr );

    printf( "%sAreaRange varTagArea = {\n", spaceStr );
    displayAreaRange( &(tagGroupPtr->varTagArea), spaceNum + 4 );
    printf( "%s}\n", spaceStr );



    printf( "%sTagBlock staticTag = {\n", spaceStr );
    displayTagBlock( &(tagGroupPtr->staticTag), spaceNum + 4 );
    printf( "%s}\n", spaceStr );

    printf( "%sTagBlock varTag = {\n", spaceStr );
    displayTagBlock( &(tagGroupPtr->varTag), spaceNum + 4 );
    printf( "%s}\n", spaceStr );



    free(spaceStr);
}

//显示 TagList
void displayTagList( TagList *tagListInst, int spaceNum )
{
    char *spaceStr = getMallocStr( ' ', spaceNum );
    if ( tagListInst == NULL )
    {
        printf( "%sNULL\n", spaceStr );
        free(spaceStr);
        return;
    }

    printf( "%sColorInfo elemColorInfo = {\n", spaceStr );
    displayColorInfo( &(tagListInst->elemColorInfo), spaceNum + 4 );
    printf( "%s}\n", spaceStr );



    printf( "%sAreaRange elemBaseArea = {\n", spaceStr );
    displayAreaRange( &(tagListInst->elemBaseArea), spaceNum + 4 );
    printf( "%s}\n", spaceStr );

    printf( "%sAreaRange elemTextArea = {\n", spaceStr );
    displayAreaRange( &(tagListInst->elemTextArea), spaceNum + 4 );
    printf( "%s}\n", spaceStr );


    printf( "%s", spaceStr );
    printf( "short wordSpacing = %d\n", tagListInst->wordSpacing );
    printf( "%s", spaceStr );
    printf( "short letterSpacing = %d\n", tagListInst->letterSpacing );
    printf( "%s", spaceStr );
    printf( "short elemNum = %d\n", tagListInst->elemNum );


    TagBlock *elemBlock = tagListInst->elemBlock;//列表元素显示信息(数组)

    printf( "%s", spaceStr );
    printf( "TagBlock *elemBlock = %p\n", elemBlock );


	int i;
	for ( i = 0; i < tagListInst->elemNum; i ++ )
    {
        printf( "%sTagBlock elemBlock[%d] = {\n", spaceStr, i );
        displayTagBlock( elemBlock, spaceNum + 4 );
        printf( "%s}\n", spaceStr );
        elemBlock ++;
    }



    free(spaceStr);

}

//显示 ProgressBarX
void displayProgressBarX( ProgressBarX *prgBarXInst, int spaceNum )
{
    char *spaceStr = getMallocStr( ' ', spaceNum );
    if ( prgBarXInst == NULL )
    {
        printf( "%sNULL\n", spaceStr );
        free(spaceStr);
        return;
    }

    printf( "%sColorInfo bkgBorderColor = {\n", spaceStr );
    displayColorInfo( &(prgBarXInst->bkgBorderColor), spaceNum + 4 );
    printf( "%s}\n", spaceStr );



    printf( "%sAreaRange axisArea = {\n", spaceStr );
    displayAreaRange( &(prgBarXInst->axisArea), spaceNum + 4 );
    printf( "%s}\n", spaceStr );

    printf( "%sAreaRange dotBaseArea = {\n", spaceStr );
    displayAreaRange( &(prgBarXInst->dotBaseArea), spaceNum + 4 );
    printf( "%s}\n", spaceStr );

    printf( "%sAreaRange clickArea = {\n", spaceStr );
    displayAreaRange( &(prgBarXInst->clickArea), spaceNum + 4 );
    printf( "%s}\n", spaceStr );



    printf( "%sIconBlock axis = {\n", spaceStr );
    displayIconBlock( &(prgBarXInst->axis), spaceNum + 4 );
    printf( "%s}\n", spaceStr );

    printf( "%sIconBlock dot = {\n", spaceStr );
    displayIconBlock( &(prgBarXInst->dot), spaceNum + 4 );
    printf( "%s}\n", spaceStr );



    printf( "%s", spaceStr );
    printf( "short xOffset = %d\n", prgBarXInst->xOffset );
    printf( "%s", spaceStr );
    printf( "short xOffsetMax = %d\n", prgBarXInst->xOffsetMax );


    free(spaceStr);

}

//显示 ScrollBarY
void displayScrollBarY( ScrollBarY *scrollBarYInst, int spaceNum )
{
    char *spaceStr = getMallocStr( ' ', spaceNum );
    if ( scrollBarYInst == NULL )
    {
        printf( "%sNULL\n", spaceStr );
        free(spaceStr);
        return;
    }


    printf( "%sColorInfo barYColorInfo = {\n", spaceStr );
    displayColorInfo( &(scrollBarYInst->barYColorInfo), spaceNum + 4 );
    printf( "%s}\n", spaceStr );



    printf( "%sAreaRange iconMoveUpArea = {\n", spaceStr );
    displayAreaRange( &(scrollBarYInst->iconMoveUpArea), spaceNum + 4 );
    printf( "%s}\n", spaceStr );

    printf( "%sAreaRange iconMoveDownArea = {\n", spaceStr );
    displayAreaRange( &(scrollBarYInst->iconMoveDownArea), spaceNum + 4 );
    printf( "%s}\n", spaceStr );

    printf( "%sAreaRange barBaseArea = {\n", spaceStr );
    displayAreaRange( &(scrollBarYInst->barBaseArea), spaceNum + 4 );
    printf( "%s}\n", spaceStr );

    printf( "%sAreaRange barMoveArea = {\n", spaceStr );
    displayAreaRange( &(scrollBarYInst->barMoveArea), spaceNum + 4 );
    printf( "%s}\n", spaceStr );



    printf( "%sIconBlock iconMoveUp = {\n", spaceStr );
    displayIconBlock( &(scrollBarYInst->iconMoveUp), spaceNum + 4 );
    printf( "%s}\n", spaceStr );

    printf( "%sIconBlock iconMoveDown = {\n", spaceStr );
    displayIconBlock( &(scrollBarYInst->iconMoveDown), spaceNum + 4 );
    printf( "%s}\n", spaceStr );

    printf( "%sIconBlock iconBar = {\n", spaceStr );
    displayIconBlock( &(scrollBarYInst->iconBar), spaceNum + 4 );
    printf( "%s}\n", spaceStr );



    printf( "%s", spaceStr );
    printf( "short barYoffset = %d\n", scrollBarYInst->barYoffset );
    printf( "%s", spaceStr );
    printf( "short barYOffsetMax = %d\n", scrollBarYInst->barYOffsetMax );
    printf( "%s", spaceStr );
    printf( "short barHeight = %d\n", scrollBarYInst->barHeight );
    printf( "%s", spaceStr );
    printf( "bool iconBarVisible = %s\n", (scrollBarYInst->iconBarVisible)?"true":"false" );



    free(spaceStr);
}

//显示 ScreenPageInfo
void displayScreenPageInfo( ScreenPageInfo *scrPageInfoInst, int spaceNum )
{

    char *spaceStr = getMallocStr( ' ', spaceNum );
    if ( scrPageInfoInst == NULL )
    {
        printf( "%sNULL\n", spaceStr );
        free(spaceStr);
        return;
    }

    printf( "%sColorInfo bkgBorderColor = {\n", spaceStr );
    displayColorInfo( &(scrPageInfoInst->bkgBorderColor), spaceNum + 4 );
    printf( "%s}\n", spaceStr );



    printf( "%sAreaRange prgBarArea = {\n", spaceStr );
    displayAreaRange( &(scrPageInfoInst->prgBarArea), spaceNum + 4 );
    printf( "%s}\n", spaceStr );

    printf( "%sAreaRange tagPagePercTageArea = {\n", spaceStr );
    displayAreaRange( &(scrPageInfoInst->tagPagePercTageArea), spaceNum + 4 );
    printf( "%s}\n", spaceStr );

    printf( "%sAreaRange tagPageNumArea = {\n", spaceStr );
    displayAreaRange( &(scrPageInfoInst->tagPageNumArea), spaceNum + 4 );
    printf( "%s}\n", spaceStr );

    printf( "%sProgressBarX prgBarX = {\n", spaceStr );
    displayProgressBarX( &(scrPageInfoInst->prgBarX), spaceNum + 4 );
	printf( "%s}\n", spaceStr );

    printf( "%sTagBlock tagPagePercTage = {\n", spaceStr );
    displayTagBlock( &(scrPageInfoInst->tagPagePercTage), spaceNum + 4 );
    printf( "%s}\n", spaceStr );

    printf( "%sTagBlock tagPageNum = {\n", spaceStr );
    displayTagBlock( &(scrPageInfoInst->tagPageNum), spaceNum + 4 );
    printf( "%s}\n", spaceStr );



    printf( "%sconst TxtFile *txtFileInfo = {\n", spaceStr );
    displayTxtFile( (scrPageInfoInst->txtFileInfo), spaceNum + 4 );
    printf( "%s}\n", spaceStr );



    free(spaceStr);
}

void displayColorRGB( color_u16 colorVal )
{
    alt_u8 colorRed, colorGreen, colorBlue;

    colorRed = ( colorVal & 0xf800 ) >> 8;

    colorGreen = ( colorVal & 0x07e0 ) >> 3;

    colorBlue = ( colorVal & 0x001f ) << 3;

    printf( "(R:%3d, G:%3d, B:%3d)", colorRed, colorGreen, colorBlue );
}

//显示 图标
void displayObjPic( const alt_u8 *objPicPtr, short objXSize, short objYSize, int spaceNum )
{
    char *spaceStr = getMallocStr( ' ', spaceNum );
    if ( objPicPtr == NULL )
    {
        printf( "%sNULL\n", spaceStr );
        free(spaceStr);
        return;
    }

    short objXByte = (objXSize+7)>>3;//除8，向上取整

    int xPos, yPos;

    short objPicRowBaseIndex = 0;
    alt_u8 objPicRowPixel = 0;
	for ( yPos = 0; yPos < objYSize; yPos ++ )
	{
		printf( "%s", spaceStr );
		short objPicIndex = objPicRowBaseIndex;
		for ( xPos = 0; xPos < objXSize; xPos ++ )
		{
			if ( (xPos & 0x7) == 0x0 )//xPos低三位为000（等效为xPos % 8 == 0 )
			{
				objPicRowPixel = ( objPicPtr != NULL ) ? objPicPtr[objPicIndex] : 0x00;

				objPicIndex ++;
			}
			if ( (objPicRowPixel & 0x80) == 0x80 )//最高位为1
			{
				printf( "* " );
			}
			else
			{
				printf( "  " );
			}
			objPicRowPixel = objPicRowPixel<<1;//左移一位，将新像素信息移至最高位
		}
		printf( "\n" );
		objPicRowBaseIndex = objPicRowBaseIndex + objXByte;
	}

    free(spaceStr);

}

//将生成的屏幕数组保存到文件中
void storeScreenArrayToFile( void )
{
    FILE *fp = fopen("image.txt", "wt" );

    if(fp == NULL)
    {
        printf( "failed\n" );
        return;
    }
    else
    {
        WriteArray2File( fp, SCR_HEIGHT, SCR_WIDTH );
        fclose(fp);
    }
}

int WriteArray2File(FILE* fp, int row, int column)
{
    int i,j;

    // print a row
    for( i = 0; i < row; i++)
    {
        // print an element
        for( j = 0; j < column; j++)
        {
            fprintf( fp, "%d ", screenShowSpace[i][j] );
        }
        fprintf( fp, "\n" );
    }

    return 0;
}
//将屏幕数组置为0
void clearScreenArray()
{
    int i, j;
    for ( i = 0; i < SCR_HEIGHT; i ++ )
    {
        for ( j = 0; j < SCR_WIDTH; j ++ )
        {
            screenShowSpace[i][j] = 0;
        }
    }



}
