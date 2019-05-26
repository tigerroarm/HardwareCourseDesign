/*
 * touch_resp.c
 *
 *  Created on: 2019年4月27日
 *      Author: 12757
 */

#include <stdio.h>
#include <string.h>
#include "touch_resp.h"
#include "screen_display.h"
#include "txt_read.h"
#include "screen.h"

TouchTaskInfo touchPosFlag;

//判断触摸位置是否在绝对区域内
bool posValidInAbsPos( PosType touchPos, AreaRange *absPos )
{

    if ( absPos->x_min <= touchPos.x_pos && touchPos.x_pos <= absPos->x_max \
      && absPos->y_min <= touchPos.y_pos && touchPos.y_pos <= absPos->y_max )
    {
        return true;
    }
    else
    {
        return false;
    }
}

//主函数对触摸位置进行响应操作
bool screenRespTask( ScreenContainer *screenInst )
{
    bool status = true;
    bool status1;
    bool posValid;

    if ( !touchPosFlag.touchOccur )//新触摸未到来
    {
        return status;
    }
    //触摸到来
    touchPosFlag.touchOccur = false;//清除触摸到来标志

    //开始判断触发类型

    //如果为上边栏
    posValid = posValidInAbsPos( touchPosFlag.touchPos, &(screenInst->headerArea) );
//    printf( "posValid=%s\n", posValid?"true":"false" );
    if ( posValid )
    {
        //上边栏目前无操作，直接返回
        return status;
    }

    //如果为下边栏
    posValid = posValidInAbsPos( touchPosFlag.touchPos, &(screenInst->footerArea) );
//    printf( "posValid=%s\n", posValid?"true":"false" );
    if ( posValid )
    {
        status1 = scrFooterResp( &(screenInst->scrFooter), &(screenInst->mainAreaInfo),\
                                     &(screenInst->footerArea), touchPosFlag.touchPos );
        status = status && status1;
        return status;
    }

    //如果为主页面
    posValid = posValidInAbsPos( touchPosFlag.touchPos, &(screenInst->mainArea) );
//    printf( "posValid=%s\n", posValid?"true":"false" );
    if ( posValid )
    {
//**********************测试模式时，设置TYPE为0;  正常运行模式时，设置TYPE为1************************************
    #define SCREEN_MAIN_SHOW_TYPE 1

    #if SCREEN_MAIN_SHOW_TYPE == 1
        short curScreenIndex = (screenInst->mainAreaInfo).curScrIndex;
        switch( ((screenInst->mainAreaInfo).scrIDStack)[curScreenIndex] )
    #elif SCREEN_MAIN_SHOW_TYPE == 0
        switch( SCR_BOOK )
    #endif // SCREEN_MAIN_SHOW_TYPE
    #undef SCREEN_MAIN_SHOW_TYPE
        {
            case SCR_HOME:
                status1 = scrMainHomeResp( &(screenInst->scrHome), &(screenInst->mainAreaInfo),\
                                           &(screenInst->mainArea), touchPosFlag.touchPos, &(screenInst->scrBook) );
                break;
            case SCR_BOOK:
                status1 = scrMainBookResp( &(screenInst->scrBook), &(screenInst->mainAreaInfo),\
                                           &(screenInst->mainArea), touchPosFlag.touchPos );
                break;
            case SCR_SETTING:
                status1 = scrMainSettingResp( &(screenInst->scrSetting), &(screenInst->mainAreaInfo),\
                                           &(screenInst->mainArea), touchPosFlag.touchPos, &(screenInst->scrColorPicker) );
                break;
            case SCR_COLOR_PICKER:
                status1 = scrMainColorPickerResp( &(screenInst->scrColorPicker), &(screenInst->mainAreaInfo),\
                                           &(screenInst->mainArea), touchPosFlag.touchPos );
                break;
            default:
                status1 = false;
        }
        status = status && status1;
        return status;
    }

    return status;
}

//下边栏对触摸位置进行响应操作
bool scrFooterResp( ScreenFooterBtn *scrFooterInst, ScrMainAreaInfo *mainAreaInfo,\
                        AreaRange *absPos, PosType touchPos )
{
    bool status = true;
    bool status1, status2;
    bool posValid = false;

    AreaRange tempAbsArea;//暂存图标绝对地址
    //先对按下的按键进行判断，再进行相应屏幕操作

    //是否为后退键区域
    status1 = getAbsPos( absPos, &(scrFooterInst->iconBackArea), &tempAbsArea );// 后退键区域
    posValid = posValidInAbsPos( touchPos, &tempAbsArea );

    if(posValid){

        if(mainAreaInfo->curScrIndex){
            mainAreaInfo->curScrIndex--;
        }
        else{
        }
        status2  = showScreenMain( &tftlcdScreenInst );
        return status&status1&status2;
    }

    //是否为主页键区域
    status1 = getAbsPos( absPos, &(scrFooterInst->iconHomeArea), &tempAbsArea );// 主页键区域
    posValid = posValidInAbsPos( touchPos, &tempAbsArea );

    if(posValid){

        mainAreaInfo->curScrIndex = 0;
        status2  = showScreenMain( &tftlcdScreenInst );
        return status&status1&status2;
    }

    //是否为设置键区域
    status1 = getAbsPos( absPos, &(scrFooterInst->iconSettingArea), &tempAbsArea );// 设置键区域
    posValid = posValidInAbsPos( touchPos, &tempAbsArea );
    //如果当前屏幕页面栈出现过设置，则index指向之；否则向栈写入设置
    if(posValid){

        int i = 0;
        bool findsetting = false;
        int endofindex = mainAreaInfo->curScrIndex;
        for(i = 0; i <= endofindex; i++){
            if((mainAreaInfo->scrIDStack)[i] == SCR_SETTING){
                mainAreaInfo->curScrIndex = i;
                findsetting = true;
                break;
            }
        }

        if(!findsetting){
            mainAreaInfo->curScrIndex++;
            (mainAreaInfo->scrIDStack)[mainAreaInfo->curScrIndex] = SCR_SETTING;
        }
        status2  = showScreenMain( &tftlcdScreenInst );
        return status&status1&status2;
    }

    return status;
}

//主栏home对触摸位置进行响应操作
bool scrMainHomeResp( ScreenHome *scrHomeInst, ScrMainAreaInfo *mainAreaInfo,\
                        AreaRange *absPos, PosType touchPos, ScreenBook *scrBookInst )
{
    bool status = true;
    bool status1;
    bool posValid;

    printf( "hitHome\n" );

    //先对按下的按键进行判断，再进行相应屏幕操作
    AreaRange subModuAbsArea;
//displayAreaRange( absPos, 0 );
//displayAreaRange( &(scrHomeInst->numBarArea), 0 );

    //如果点击的是数字条
    status1 = getAbsPos( absPos, &(scrHomeInst->numBarArea), &subModuAbsArea );
    posValid = posValidInAbsPos( touchPos, &subModuAbsArea );
//displayAreaRange( &subModuAbsArea, 0 );
    if ( posValid )
    {
        printf( "numbar\n" );
        //触摸点的相对位置
        short touchPosRelaY = touchPos.y_pos - subModuAbsArea.y_min;

        //计算触摸点在列表中的纵向相对位置
        short touchYInList = touchPosRelaY + scrHomeInst->listYOffset;//位置从0开始

        //计算触摸点位于列表中的第几个元素
        short touchListElemIndex = touchYInList / scrHomeInst->numBar.elemBaseArea.y_max;

        //开启单行文件名横向滚动功能
        status1 = openTxtScrollX( scrHomeInst, touchListElemIndex );
        status = status && status1;
        return status;
    }

    //如果点击的是目录
    status1 = getAbsPos( absPos, &(scrHomeInst->txtCatalogArea), &subModuAbsArea );
    posValid = posValidInAbsPos( touchPos, &subModuAbsArea );
//displayAreaRange( &subModuAbsArea, 0 );
    if ( posValid )
    {
        printf( "txtCatalog\n" );
        //触摸点的相对位置
        short touchPosRelaY = touchPos.y_pos - subModuAbsArea.y_min;

        //计算触摸点在列表中的纵向相对位置
        short touchYInList = touchPosRelaY + scrHomeInst->listYOffset;//位置从0开始

        //计算触摸点位于列表中的第几个元素
        short touchListElemIndex = touchYInList / scrHomeInst->txtCatalog.elemBaseArea.y_max;

        //打开该文件
        scrBookInst->pageInfo.txtFileInfo = openTxtFile( touchListElemIndex );
        //如果文件能打开，就跳转到BOOK页面
        if ( scrBookInst->pageInfo.txtFileInfo != NULL )
        {
            mainAreaInfo->curScrIndex ++;
            if ( mainAreaInfo->curScrIndex > 3 )
            {
                status = false;
                mainAreaInfo->curScrIndex = 1;
            }
            mainAreaInfo->scrIDStack[mainAreaInfo->curScrIndex] = SCR_BOOK;
            showScreenMain( &tftlcdScreenInst );
        }
        return status;
    }

    //如果点击的是纵向滚动条
    status1 = getAbsPos( absPos, &(scrHomeInst->turnCatalogBarArea), &subModuAbsArea );
    posValid = posValidInAbsPos( touchPos, &subModuAbsArea );
//displayAreaRange( &subModuAbsArea, 0 );
    if ( posValid )
    {
        printf( "scrollBarY\n" );
        status1 = scrollBarYResp( &(scrHomeInst->turnCatalogBarY), &subModuAbsArea, touchPos, scrHomeInst );
        status = status && status1;
        return status;
    }

    return status;
}

//主栏book对触摸位置进行响应操作
bool scrMainBookResp( ScreenBook *scrBookInst, ScrMainAreaInfo *mainAreaInfo,\
                        AreaRange *absPos, PosType touchPos )
{
    bool status = true;
    bool status1;
    bool posValid;

    AreaRange txtBookAbsArea;
    AreaRange subModuAbsArea;

    //将scrBookInst->pageInfoVisible存入另外一个变量pageInfoVisible,防止scrBookInst->pageInfoVisible被修改后，
    //使用scrBookInst->pageInfoVisible造成判断错误
    bool pageInfoVisible = scrBookInst->pageInfoVisible;

    //先对按下的按键进行判断，再进行相应屏幕操作
    //如果页面进度信息可见
    if ( pageInfoVisible )
    {

        status1 = getAbsPos( absPos, &(scrBookInst->txtBookNarrowArea), &txtBookAbsArea );
        status = status && status1;
    }
    else
    {
        status1 = getAbsPos( absPos, &(scrBookInst->txtBookArea), &txtBookAbsArea );
        status = status && status1;
    }

    posValid = posValidInAbsPos( touchPos, &txtBookAbsArea );


    //如果点击的是txtbook区域
    if ( posValid )
    {
        printf( "hitBookArea\n" );
        //判断左中右区域点的哪个
        //1.左区（上一页）
        status1 = getAbsPos( absPos, &(scrBookInst->turnBackPageAreaPos), &subModuAbsArea );
        status = status && status1;

        posValid = posValidInAbsPos( touchPos, &subModuAbsArea );
        if ( posValid )
        {
            readBookOfTxtFile( scrBookInst->pageInfo.txtFileInfo, BOOK_PAGE_PRE, 0 );

            status1 = showTagList( &(scrBookInst->txtBook), &txtBookAbsArea, 0 );
            status = status && status1;
        }

        //2.右区（下一页）
        status1 = getAbsPos( absPos, &(scrBookInst->turnNextPageAreaPos), &subModuAbsArea );
        status = status && status1;

        posValid = posValidInAbsPos( touchPos, &subModuAbsArea );
        if ( posValid )
        {
            readBookOfTxtFile( scrBookInst->pageInfo.txtFileInfo, BOOK_PAGE_NEXT, 0 );

            status1 = showTagList( &(scrBookInst->txtBook), &txtBookAbsArea, 0 );
            status = status && status1;
        }

        //3.中区（打开或关闭页面进度信息）
        status1 = getAbsPos( absPos, &(scrBookInst->openPageInfoAreaPos), &subModuAbsArea );
        status = status && status1;

        posValid = posValidInAbsPos( touchPos, &subModuAbsArea );
        if ( posValid )
        {
            printf( "hitBookMiddleArea\n" );
            AreaRange subModuAbsPos;
            if ( scrBookInst->pageInfoVisible )
            {
                scrBookInst->pageInfoVisible = false;
                printf( "pageInfoVisible = false\n" );
                //显示电子书未显示区域
                status1 = getAbsPos( absPos, &(scrBookInst->pageInfoArea), &subModuAbsPos );
                status = status && status1;
                short yOffset = scrBookInst->pageInfoArea.y_min - scrBookInst->txtBookNarrowArea.y_min;
//                clearScreenArray();
                status1 = showTagList( &(scrBookInst->txtBook), &subModuAbsPos, yOffset );
                status = status && status1;
            }
            else
            {
                scrBookInst->pageInfoVisible = true;
                printf( "pageInfoVisible = true\n" );
                //显示电子书阅读进度信息
                status1 = getAbsPos( absPos, &(scrBookInst->pageInfoArea), &subModuAbsPos );
                status = status && status1;
                status1 = showScreenPageInfo( &(scrBookInst->pageInfo), &subModuAbsPos );
                status = status && status1;
            }
        }
    }

    //如果页面进度信息可见
    if ( pageInfoVisible )
    {
        status1 = getAbsPos( absPos, &(scrBookInst->pageInfoArea), &subModuAbsArea );
        status = status && status1;

        posValid = posValidInAbsPos( touchPos, &subModuAbsArea );
        //如果点击的是页面进度信息
        if ( posValid )
        {
            scrPageInfoResp( &(scrBookInst->pageInfo), &subModuAbsArea, touchPos );
        }
    }


    return status;

}

//主栏setting对触摸位置进行响应操作
bool scrMainSettingResp( ScreenSetting *scrSettingInst, ScrMainAreaInfo *mainAreaInfo,\
                        AreaRange *absPos, PosType touchPos, ScreenColorPicker *scrColorPickerInst )
{

    bool status = true;
    bool status1, status2, status3;
    bool posValid = false;
    char *tempStr;
    AreaRange tempAbsArea, safeArea, AreatoShow;

    //先对按下的按键进行判断，再进行相应屏幕操作

    //plus button of hour
    status1 = getAbsPos( absPos, &(scrSettingInst->editAlarmHourArea), &safeArea );//position of hour editor
    tempAbsArea = safeArea;
    getAbsPos(&tempAbsArea, &((scrSettingInst->editAlarmHour).valueTagArea), &AreatoShow);// area to show
    status2 = getAbsPos( &tempAbsArea, &((scrSettingInst->editAlarmHour).plusIconArea), &safeArea );//position of plus button of hour editor
    posValid = posValidInAbsPos( touchPos, &safeArea );

    if(posValid){
        if((scrSettingInst->editAlarmHour).value >= (scrSettingInst->editAlarmHour).valueMax){
            (scrSettingInst->editAlarmHour).value = (scrSettingInst->editAlarmHour).valueMin;
        }
        else{
            (scrSettingInst->editAlarmHour).value++;
        }

        tempStr = scrSettingInst->editAlarmHour.valueTag.tagText.text;
        sprintf( tempStr, "%2d", scrSettingInst->editAlarmHour.value );
        scrSettingInst->editAlarmHour.valueTag.tagText.textLen = strlen( tempStr );

        status3 = showTagBlock(&(scrSettingInst->editAlarmHour.valueTag), &AreatoShow);
        printf("hello %d\n", (scrSettingInst->editAlarmHour).value);
        return status&status1&status2&status3;
    }

    //plus button of minute
    status1 = getAbsPos( absPos, &(scrSettingInst->editAlarmMinuteArea), &safeArea );//position of hour editor
    tempAbsArea = safeArea;
    getAbsPos(&tempAbsArea, &((scrSettingInst->editAlarmMinute).valueTagArea), &AreatoShow);// area to show
    status2 = getAbsPos( &tempAbsArea, &((scrSettingInst->editAlarmMinute).plusIconArea), &safeArea );//position of plus button of hour editor
    posValid = posValidInAbsPos( touchPos, &safeArea );
    if(posValid){
        if((scrSettingInst->editAlarmMinute).value >= (scrSettingInst->editAlarmMinute).valueMax){
            (scrSettingInst->editAlarmMinute).value = (scrSettingInst->editAlarmMinute).valueMin;
        }
        else{
            (scrSettingInst->editAlarmMinute).value++;
        }

        tempStr = scrSettingInst->editAlarmMinute.valueTag.tagText.text;
        sprintf( tempStr, "%2d", scrSettingInst->editAlarmMinute.value );
        scrSettingInst->editAlarmMinute.valueTag.tagText.textLen = strlen( tempStr );

        status3 = showTagBlock(&(scrSettingInst->editAlarmMinute.valueTag), &AreatoShow);
        return status&status1&status2&status3;
    }

    //plus button of second
    status1 = getAbsPos( absPos, &(scrSettingInst->editTurnPageSecArea), &safeArea );//position of hour editor
    tempAbsArea = safeArea;
    getAbsPos(&tempAbsArea, &((scrSettingInst->editTurnPageSec).valueTagArea), &AreatoShow);// area to show
    status2 = getAbsPos( &tempAbsArea, &((scrSettingInst->editTurnPageSec).plusIconArea), &safeArea );//position of plus button of hour editor
    posValid = posValidInAbsPos( touchPos, &safeArea );
    if(posValid){
        if((scrSettingInst->editTurnPageSec).value >= (scrSettingInst->editTurnPageSec).valueMax){
            (scrSettingInst->editTurnPageSec).value = (scrSettingInst->editTurnPageSec).valueMin;
        }
        else{
            (scrSettingInst->editAlarmHour).value++;
        }

        tempStr = scrSettingInst->editTurnPageSec.valueTag.tagText.text;
        sprintf( tempStr, "%2d", scrSettingInst->editTurnPageSec.value );
        scrSettingInst->editTurnPageSec.valueTag.tagText.textLen = strlen( tempStr );

        status3 = showTagBlock(&(scrSettingInst->editTurnPageSec.valueTag), &AreatoShow);
        return status&status1&status2&status3;
    }

    //minus button of hour
    status1 = getAbsPos( absPos, &(scrSettingInst->editAlarmHourArea), &safeArea );//position of hour editor
    tempAbsArea = safeArea;
    getAbsPos(&tempAbsArea, &((scrSettingInst->editAlarmHour).valueTagArea), &AreatoShow);// area to show
    status2 = getAbsPos( &tempAbsArea, &((scrSettingInst->editAlarmHour).minusIconArea), &safeArea );//position of plus button of hour editor
    posValid = posValidInAbsPos( touchPos, &safeArea );
    if(posValid){
        if((scrSettingInst->editAlarmHour).value <= (scrSettingInst->editAlarmHour).valueMin){
            (scrSettingInst->editAlarmHour).value = (scrSettingInst->editAlarmHour).valueMax;
        }
        else{
            (scrSettingInst->editAlarmHour).value--;
        }

        tempStr = scrSettingInst->editAlarmHour.valueTag.tagText.text;
        sprintf( tempStr, "%2d", scrSettingInst->editAlarmHour.value );
        scrSettingInst->editAlarmHour.valueTag.tagText.textLen = strlen( tempStr );

        status3 = showTagBlock(&(scrSettingInst->editAlarmHour.valueTag), &AreatoShow);
        return status&status1&status2&status3;
    }

    //minus button of minute
    status1 = getAbsPos( absPos, &(scrSettingInst->editAlarmMinuteArea), &safeArea );//position of hour editor
    tempAbsArea = safeArea;
    getAbsPos(&tempAbsArea, &((scrSettingInst->editAlarmMinute).valueTagArea), &AreatoShow);// area to show
    status2 = getAbsPos( &tempAbsArea, &((scrSettingInst->editAlarmMinute).minusIconArea), &safeArea );//position of plus button of hour editor
    posValid = posValidInAbsPos( touchPos, &safeArea );
    if(posValid){
        if((scrSettingInst->editAlarmMinute).value <= (scrSettingInst->editAlarmMinute).valueMin){
            (scrSettingInst->editAlarmMinute).value = (scrSettingInst->editAlarmMinute).valueMax;
        }
        else{
            (scrSettingInst->editAlarmMinute).value--;
        }

        tempStr = scrSettingInst->editAlarmMinute.valueTag.tagText.text;
        sprintf( tempStr, "%2d", scrSettingInst->editAlarmMinute.value );
        scrSettingInst->editAlarmMinute.valueTag.tagText.textLen = strlen( tempStr );

        status3 = showTagBlock(&(scrSettingInst->editAlarmMinute.valueTag), &AreatoShow);
        return status&status1&status2&status3;
    }

    //minus button of second
    status1 = getAbsPos( absPos, &(scrSettingInst->editTurnPageSecArea), &safeArea );//position of hour editor
    tempAbsArea = safeArea;
    getAbsPos(&tempAbsArea, &((scrSettingInst->editTurnPageSec).valueTagArea), &AreatoShow);// area to show
    status2 = getAbsPos( &tempAbsArea, &((scrSettingInst->editTurnPageSec).minusIconArea), &safeArea );//position of plus button of hour editor
    posValid = posValidInAbsPos( touchPos, &safeArea );
    if(posValid){
        if((scrSettingInst->editTurnPageSec).value <= (scrSettingInst->editTurnPageSec).valueMin){
            (scrSettingInst->editTurnPageSec).value = (scrSettingInst->editTurnPageSec).valueMax;
        }
        else{
            (scrSettingInst->editTurnPageSec).value--;
        }

        tempStr = scrSettingInst->editTurnPageSec.valueTag.tagText.text;
        sprintf( tempStr, "%2d", scrSettingInst->editTurnPageSec.value );
        scrSettingInst->editTurnPageSec.valueTag.tagText.textLen = strlen( tempStr );

        status3 = showTagBlock(&(scrSettingInst->editTurnPageSec.valueTag), &AreatoShow);
        return status&status1&status2&status3;
    }

    //background color editor
    status1 = getAbsPos( absPos, &(scrSettingInst->editBkgColorArea), &safeArea);// position of background color editor
    tempAbsArea = safeArea;
    getAbsPos(&tempAbsArea, &((scrSettingInst->editBkgColor).varTagArea), &AreatoShow);// area to show
    status2 = getAbsPos( &tempAbsArea, &((scrSettingInst->editBkgColor).varTagArea), &safeArea );//position of plus button of hour editor
    posValid = posValidInAbsPos( touchPos, &safeArea );
    if(posValid){
        printf("bkgcolor\n");
        (mainAreaInfo->curScrIndex)++;
        (mainAreaInfo->scrIDStack)[mainAreaInfo->curScrIndex] = SCR_COLOR_PICKER;
        scrColorPickerInst->colorPicker.colorIndex = &(scrSettingInst->bkgColorIndex);
        status3  = showScreenMain( &tftlcdScreenInst );
        return status&status1&status2&status3;
    }
    //font color editor
    status1 = getAbsPos( absPos, &(scrSettingInst->editWordColorArea), &safeArea);// position of background color editor
    tempAbsArea = safeArea;
    getAbsPos(&tempAbsArea, &((scrSettingInst->editWordColor).varTagArea), &AreatoShow);// area to show
    status2 = getAbsPos( &tempAbsArea, &((scrSettingInst->editWordColor).varTagArea), &safeArea );//position of plus button of hour editor
    posValid = posValidInAbsPos( touchPos, &safeArea );
    if(posValid){
        (mainAreaInfo->curScrIndex)++;
        (mainAreaInfo->scrIDStack)[mainAreaInfo->curScrIndex] = SCR_COLOR_PICKER;
        scrColorPickerInst->colorPicker.colorIndex = &(scrSettingInst->txtColorIndex);
        status3  = showScreenMain( &tftlcdScreenInst );
        return status&status1&status2&status3;
    }

    //auto page turner
    status1 = getAbsPos( absPos, &(scrSettingInst->editTurnPageModArea), &safeArea);// position of background color editor
    tempAbsArea = safeArea;
    getAbsPos(&tempAbsArea, &((scrSettingInst->editTurnPageMod).varTagArea), &AreatoShow);// area to show
    status2 = getAbsPos( &tempAbsArea, &((scrSettingInst->editTurnPageMod).varTagArea), &safeArea );//position of plus button of hour editor
    posValid = posValidInAbsPos( touchPos, &safeArea );
    if(posValid){
         if ( scrSettingInst->autoTurnPage )
         {
             scrSettingInst->editTurnPageMod.varTag.tagText.text = "off";
             scrSettingInst->editTurnPageMod.varTag.tagText.textLen = 3;
         }
         else
         {
             scrSettingInst->editTurnPageMod.varTag.tagText.text = "on";
             scrSettingInst->editTurnPageMod.varTag.tagText.textLen = 2;
         }

         scrSettingInst->autoTurnPage = ! scrSettingInst->autoTurnPage;
         status3 = showTagBlock(&(scrSettingInst->editTurnPageMod).varTag, &AreatoShow);
         return status&status1&status2&status3;
    }

    return status;

}

//主栏colorPicker对触摸位置进行响应操作
bool scrMainColorPickerResp( ScreenColorPicker *scrColorPickerInst, ScrMainAreaInfo *mainAreaInfo,\
                        AreaRange *absPos, PosType touchPos )
{
    bool status = true;
    bool status1;
    bool posValid;



    //先对按下的按键进行判断，再进行相应屏幕操作

    AreaRange colorTableAbsArea;
    status1 = getAbsPos( absPos, &(scrColorPickerInst->colorPickerArea), &colorTableAbsArea );
    status = status && status1;

    posValid = posValidInAbsPos( touchPos, &colorTableAbsArea );

//    printf( "posValid=%s\n", posValid?"true":"false" );

    if ( posValid )
    {
        //取色板对按键进行响应操作
        status1 = colorBoardResp( &(scrColorPickerInst->colorPicker), &colorTableAbsArea, touchPos );
        status = status && status1;
    }

    return status;

}

//ColorBoard对触摸位置进行响应操作
bool colorBoardResp( ColorBoard *colorPicker, AreaRange *absPos, PosType touchPos )
{
    bool status = true;
    bool status1;
    bool posValid;

//    displayAreaRange( absPos, 0 );

    //计算触摸位置在colorPicker中的相对位置
    PosType touchRelaPos;
    touchRelaPos.x_pos = touchPos.x_pos - absPos->x_min + 1;
    touchRelaPos.y_pos = touchPos.y_pos - absPos->y_min + 1;

    //计算触摸的色块位置
    short colorXIndex, colorYIndex;

    colorXIndex = ( touchRelaPos.x_pos - 1) / colorPicker->colorXSize;//从0开始
    colorYIndex = ( touchRelaPos.y_pos - 1) / colorPicker->colorYSize;//从0开始

    //新选中的颜色在256色中的位置
    short newColorSel = colorXIndex + colorYIndex * colorPicker->colorXNum;



    //清除原先选中的色块
    status1 = clearColorSelBorder( colorPicker, absPos );
    status = status && status1;



    //将新颜色下标保存起来
    if ( colorPicker->colorIndex != (void*)0 )
    {
        *(colorPicker->colorIndex) = newColorSel;
    }
    else
    {
        status = false;
    }

    //显示新选中的颜色
    status1 = showColorSelBorder( colorPicker, absPos );
    status = status && status1;


    return status;
}

//页面进度信息对触摸位置进行响应操作
bool scrPageInfoResp( ScreenPageInfo *scrPageInfoInst, AreaRange *absPos, PosType touchPos )
{
    bool status = true;
    bool status1;
    bool posValid;

    AreaRange subModuAbsArea;

    status1 = getAbsPos( absPos, &(scrPageInfoInst->prgBarArea), &subModuAbsArea );
    status = status && status1;

    posValid = posValidInAbsPos( touchPos, &subModuAbsArea );
    if ( posValid )
    {
        posValid = prgBarXResp( &(scrPageInfoInst->prgBarX), &subModuAbsArea, touchPos );
        //如果确实点击到进度条可点击区
        if ( posValid )
        {
            //修改阅读进度，从SD卡中读取新内容
            float pageRatio = 1.0 * scrPageInfoInst->prgBarX.xOffset / scrPageInfoInst->prgBarX.xOffsetMax;
            printf( "offset = %d, max = %d\n", scrPageInfoInst->prgBarX.xOffset, scrPageInfoInst->prgBarX.xOffsetMax );
            readBookOfTxtFile( scrPageInfoInst->txtFileInfo, BOOK_PAGE_JUMP, pageRatio );
            //修改进度百分比
            char *strPercent = scrPageInfoInst->tagPagePercTage.tagText.text;
            sprintf( strPercent, "%6.2f", pageRatio * 100 );
            strPercent[6] = '%';
            strPercent[7] = 0;
            scrPageInfoInst->tagPagePercTage.tagText.textLen = 7;
//            printf( "haha1\n" );

            //修改页数
            if ( scrPageInfoInst->txtFileInfo != NULL )
            {
                char *strPageNum = scrPageInfoInst->tagPageNum.tagText.text;
                char tempStr1[10];
                sprintf( tempStr1, "%5d", scrPageInfoInst->txtFileInfo->curPageNum );
                short tempStrLen1 = strlen( tempStr1 );
                if ( tempStrLen1 > 5 )
                {
                    tempStrLen1 = 5;
                }
                tempStr1[tempStrLen1] = 0;
                strcpy( strPageNum, tempStr1 );

                strPageNum[tempStrLen1] = '/';
    //            printf( "haha3\n" );
                char tempStr2[10];
                sprintf( tempStr2, "%5d", scrPageInfoInst->txtFileInfo->totalPageNum );
                short tempStrLen2 = strlen( tempStr2 );
                if ( tempStrLen2 > 5 )
                {
                    tempStrLen2 = 5;
                }
                tempStr2[tempStrLen2] = 0;

                strcpy( strPageNum + tempStrLen2 + 1, tempStr2 );

                scrPageInfoInst->tagPageNum.tagText.textLen = tempStrLen1 + tempStrLen2 + 1;

                //刷新页数和百分数进度
                AreaRange subModuAbsPos;
                //百分数进度
                status1 = getAbsPos( absPos, &(scrPageInfoInst->tagPagePercTageArea), &subModuAbsPos );
                status = status && status1;
                status1 = showTagBlock( &(scrPageInfoInst->tagPagePercTage), &subModuAbsPos );
                status = status && status1;

                //页数与总页数（格式为："1234/99999", 1234为当前页数，99999为总页数）
                status1 = getAbsPos( absPos, &(scrPageInfoInst->tagPageNumArea), &subModuAbsPos );
                status = status && status1;
                status1 = showTagBlock( &(scrPageInfoInst->tagPageNum), &subModuAbsPos );
                status = status && status1;
            }
        }
    }

    return status;
}


//页面进度横向进度条对触摸位置进行响应操作(返回值为true表示按到了进度条点击区，为false表示没有按到）
bool prgBarXResp( ProgressBarX *prgBarXInst, AreaRange *absPos, PosType touchPos )
{
    bool posValid;
    AreaRange clickBarAbsArea;
    getAbsPos( absPos, &(prgBarXInst->clickArea), &clickBarAbsArea );

    posValid = posValidInAbsPos( touchPos, &clickBarAbsArea );

    //用户点击到进度条点击区
    if ( posValid )
    {
        short xHitOffset = touchPos.x_pos - prgBarXInst->axisArea.x_min;
        if ( xHitOffset > prgBarXInst->xOffsetMax )
        {
            xHitOffset = prgBarXInst->xOffsetMax;
        }
        if ( xHitOffset < 0 )
        {
            xHitOffset = 0;
        }
        prgBarXInst->xOffset = xHitOffset;
        //显示进度条
        showProgressBarX( prgBarXInst, absPos );
    }
    return posValid;
}
//纵向目录滚动条响应操作
bool scrollBarYResp( ScrollBarY *scrollBarYInst, AreaRange *absPos, PosType touchPos, ScreenHome *scrHomeInst )
{
    bool status = true;
    bool status1;
    bool posValid;


    AreaRange subModuAbsArea;

    //如果点击上移键
    status1 = getAbsPos( absPos, &(scrollBarYInst->iconMoveUpArea), &subModuAbsArea );
    status = status && status1;
    posValid = posValidInAbsPos( touchPos, &subModuAbsArea );
    if ( posValid )
    {
        if ( scrollBarYInst->iconBarVisible )//可滚动
        {
            //修改目录列表显示偏移量
            scrHomeInst->listYOffset = scrHomeInst->listYOffset - scrHomeInst->listYStepSize;
            if ( scrHomeInst->listYOffset < 0 )
            {
                scrHomeInst->listYOffset = 0;
            }
            //修改滚动条的偏移量
            float ratio = 1.0 * scrHomeInst->listYOffset / scrHomeInst->listYOffsetMax;
            scrollBarYInst->barYoffset = ratio * scrollBarYInst->barYOffsetMax;
            if ( scrollBarYInst->barYoffset < 0 )
            {
                scrollBarYInst->barYoffset = 0;
            }
            //刷新主页面
            status1 = showScreenMain( &tftlcdScreenInst );
            status = status && status1;
        }
        return status;
    }

    //如果点击下移键
    status1 = getAbsPos( absPos, &(scrollBarYInst->iconMoveDownArea), &subModuAbsArea );
    status = status && status1;
    posValid = posValidInAbsPos( touchPos, &subModuAbsArea );
    if ( posValid )
    {
        if ( scrollBarYInst->iconBarVisible )//可滚动
        {
            //修改目录列表显示偏移量
            scrHomeInst->listYOffset = scrHomeInst->listYOffset + scrHomeInst->listYStepSize;
            if ( scrHomeInst->listYOffset > scrHomeInst->listYOffsetMax )
            {
                scrHomeInst->listYOffset = scrHomeInst->listYOffsetMax;
            }
            //修改滚动条的偏移量
            float ratio = 1.0 * scrHomeInst->listYOffset / scrHomeInst->listYOffsetMax;
            scrollBarYInst->barYoffset = ratio * scrollBarYInst->barYOffsetMax;
            if ( scrollBarYInst->barYoffset > scrollBarYInst->barYOffsetMax )
            {
                scrollBarYInst->barYoffset = scrollBarYInst->barYOffsetMax;
            }
            //刷新主页面
            status1 = showScreen( &tftlcdScreenInst );
            status = status && status1;
        }
        return status;
    }

    //如果点击进度条移动区
    status1 = getAbsPos( absPos, &(scrollBarYInst->barMoveArea), &subModuAbsArea );
    status = status && status1;
    posValid = posValidInAbsPos( touchPos, &subModuAbsArea );
    if ( posValid )
    {
        if ( scrollBarYInst->iconBarVisible )//可滚动
        {
            //修改滚动条的偏移量
            scrollBarYInst->barYoffset = touchPos.y_pos - subModuAbsArea.y_min - (scrollBarYInst->barHeight / 2);
            if ( scrollBarYInst->barYoffset < 0 )
            {
                scrollBarYInst->barYoffset = 0;
            }
            if ( scrollBarYInst->barYoffset > scrollBarYInst->barYOffsetMax )
            {
                scrollBarYInst->barYoffset = scrollBarYInst->barYOffsetMax;
            }

            //修改目录列表显示偏移量
            float ratio = 1.0 * scrollBarYInst->barYoffset / scrollBarYInst->barYOffsetMax;

            scrHomeInst->listYOffset = ratio * scrHomeInst->listYOffsetMax;
            if ( scrHomeInst->listYOffset < 0 )
            {
                scrHomeInst->listYOffset = 0;
            }
            if ( scrHomeInst->listYOffset > scrHomeInst->listYOffsetMax )
            {
                scrHomeInst->listYOffset = scrHomeInst->listYOffsetMax;
            }

            //刷新主页面
            status1 = showScreen( &tftlcdScreenInst );
            status = status && status1;
        }
        return status;
    }
    return status;
}
