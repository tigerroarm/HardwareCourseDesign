/*
 * touch_resp.c
 *
 *  Created on: 2019��4��27��
 *      Author: 12757
 */

#include <stdio.h>
#include <string.h>
#include "touch_resp.h"
#include "screen_display.h"
#include "txt_read.h"
#include "screen.h"

TouchTaskInfo touchPosFlag;

//�жϴ���λ���Ƿ��ھ���������
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

//�������Դ���λ�ý�����Ӧ����
bool screenRespTask( ScreenContainer *screenInst )
{
    bool status = true;
    bool status1;
    bool posValid;

    if ( !touchPosFlag.touchOccur )//�´���δ����
    {
        return status;
    }
    //��������
    touchPosFlag.touchOccur = false;//�������������־

    //��ʼ�жϴ�������

    //���Ϊ�ϱ���
    posValid = posValidInAbsPos( touchPosFlag.touchPos, &(screenInst->headerArea) );
//    printf( "posValid=%s\n", posValid?"true":"false" );
    if ( posValid )
    {
        //�ϱ���Ŀǰ�޲�����ֱ�ӷ���
        return status;
    }

    //���Ϊ�±���
    posValid = posValidInAbsPos( touchPosFlag.touchPos, &(screenInst->footerArea) );
//    printf( "posValid=%s\n", posValid?"true":"false" );
    if ( posValid )
    {
        status1 = scrFooterResp( &(screenInst->scrFooter), &(screenInst->mainAreaInfo),\
                                     &(screenInst->footerArea), touchPosFlag.touchPos );
        status = status && status1;
        return status;
    }

    //���Ϊ��ҳ��
    posValid = posValidInAbsPos( touchPosFlag.touchPos, &(screenInst->mainArea) );
//    printf( "posValid=%s\n", posValid?"true":"false" );
    if ( posValid )
    {
//**********************����ģʽʱ������TYPEΪ0;  ��������ģʽʱ������TYPEΪ1************************************
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

//�±����Դ���λ�ý�����Ӧ����
bool scrFooterResp( ScreenFooterBtn *scrFooterInst, ScrMainAreaInfo *mainAreaInfo,\
                        AreaRange *absPos, PosType touchPos )
{
    bool status = true;
    bool status1, status2;
    bool posValid = false;

    AreaRange tempAbsArea;//�ݴ�ͼ����Ե�ַ
    //�ȶ԰��µİ��������жϣ��ٽ�����Ӧ��Ļ����

    //�Ƿ�Ϊ���˼�����
    status1 = getAbsPos( absPos, &(scrFooterInst->iconBackArea), &tempAbsArea );// ���˼�����
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

    //�Ƿ�Ϊ��ҳ������
    status1 = getAbsPos( absPos, &(scrFooterInst->iconHomeArea), &tempAbsArea );// ��ҳ������
    posValid = posValidInAbsPos( touchPos, &tempAbsArea );

    if(posValid){

        mainAreaInfo->curScrIndex = 0;
        status2  = showScreenMain( &tftlcdScreenInst );
        return status&status1&status2;
    }

    //�Ƿ�Ϊ���ü�����
    status1 = getAbsPos( absPos, &(scrFooterInst->iconSettingArea), &tempAbsArea );// ���ü�����
    posValid = posValidInAbsPos( touchPos, &tempAbsArea );
    //�����ǰ��Ļҳ��ջ���ֹ����ã���indexָ��֮��������ջд������
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

//����home�Դ���λ�ý�����Ӧ����
bool scrMainHomeResp( ScreenHome *scrHomeInst, ScrMainAreaInfo *mainAreaInfo,\
                        AreaRange *absPos, PosType touchPos, ScreenBook *scrBookInst )
{
    bool status = true;
    bool status1;
    bool posValid;

    printf( "hitHome\n" );

    //�ȶ԰��µİ��������жϣ��ٽ�����Ӧ��Ļ����
    AreaRange subModuAbsArea;
//displayAreaRange( absPos, 0 );
//displayAreaRange( &(scrHomeInst->numBarArea), 0 );

    //����������������
    status1 = getAbsPos( absPos, &(scrHomeInst->numBarArea), &subModuAbsArea );
    posValid = posValidInAbsPos( touchPos, &subModuAbsArea );
//displayAreaRange( &subModuAbsArea, 0 );
    if ( posValid )
    {
        printf( "numbar\n" );
        //����������λ��
        short touchPosRelaY = touchPos.y_pos - subModuAbsArea.y_min;

        //���㴥�������б��е��������λ��
        short touchYInList = touchPosRelaY + scrHomeInst->listYOffset;//λ�ô�0��ʼ

        //���㴥����λ���б��еĵڼ���Ԫ��
        short touchListElemIndex = touchYInList / scrHomeInst->numBar.elemBaseArea.y_max;

        //���������ļ��������������
        status1 = openTxtScrollX( scrHomeInst, touchListElemIndex );
        status = status && status1;
        return status;
    }

    //����������Ŀ¼
    status1 = getAbsPos( absPos, &(scrHomeInst->txtCatalogArea), &subModuAbsArea );
    posValid = posValidInAbsPos( touchPos, &subModuAbsArea );
//displayAreaRange( &subModuAbsArea, 0 );
    if ( posValid )
    {
        printf( "txtCatalog\n" );
        //����������λ��
        short touchPosRelaY = touchPos.y_pos - subModuAbsArea.y_min;

        //���㴥�������б��е��������λ��
        short touchYInList = touchPosRelaY + scrHomeInst->listYOffset;//λ�ô�0��ʼ

        //���㴥����λ���б��еĵڼ���Ԫ��
        short touchListElemIndex = touchYInList / scrHomeInst->txtCatalog.elemBaseArea.y_max;

        //�򿪸��ļ�
        scrBookInst->pageInfo.txtFileInfo = openTxtFile( touchListElemIndex );
        //����ļ��ܴ򿪣�����ת��BOOKҳ��
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

    //�������������������
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

//����book�Դ���λ�ý�����Ӧ����
bool scrMainBookResp( ScreenBook *scrBookInst, ScrMainAreaInfo *mainAreaInfo,\
                        AreaRange *absPos, PosType touchPos )
{
    bool status = true;
    bool status1;
    bool posValid;

    AreaRange txtBookAbsArea;
    AreaRange subModuAbsArea;

    //��scrBookInst->pageInfoVisible��������һ������pageInfoVisible,��ֹscrBookInst->pageInfoVisible���޸ĺ�
    //ʹ��scrBookInst->pageInfoVisible����жϴ���
    bool pageInfoVisible = scrBookInst->pageInfoVisible;

    //�ȶ԰��µİ��������жϣ��ٽ�����Ӧ��Ļ����
    //���ҳ�������Ϣ�ɼ�
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


    //����������txtbook����
    if ( posValid )
    {
        printf( "hitBookArea\n" );
        //�ж��������������ĸ�
        //1.��������һҳ��
        status1 = getAbsPos( absPos, &(scrBookInst->turnBackPageAreaPos), &subModuAbsArea );
        status = status && status1;

        posValid = posValidInAbsPos( touchPos, &subModuAbsArea );
        if ( posValid )
        {
            readBookOfTxtFile( scrBookInst->pageInfo.txtFileInfo, BOOK_PAGE_PRE, 0 );

            status1 = showTagList( &(scrBookInst->txtBook), &txtBookAbsArea, 0 );
            status = status && status1;
        }

        //2.��������һҳ��
        status1 = getAbsPos( absPos, &(scrBookInst->turnNextPageAreaPos), &subModuAbsArea );
        status = status && status1;

        posValid = posValidInAbsPos( touchPos, &subModuAbsArea );
        if ( posValid )
        {
            readBookOfTxtFile( scrBookInst->pageInfo.txtFileInfo, BOOK_PAGE_NEXT, 0 );

            status1 = showTagList( &(scrBookInst->txtBook), &txtBookAbsArea, 0 );
            status = status && status1;
        }

        //3.�������򿪻�ر�ҳ�������Ϣ��
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
                //��ʾ������δ��ʾ����
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
                //��ʾ�������Ķ�������Ϣ
                status1 = getAbsPos( absPos, &(scrBookInst->pageInfoArea), &subModuAbsPos );
                status = status && status1;
                status1 = showScreenPageInfo( &(scrBookInst->pageInfo), &subModuAbsPos );
                status = status && status1;
            }
        }
    }

    //���ҳ�������Ϣ�ɼ�
    if ( pageInfoVisible )
    {
        status1 = getAbsPos( absPos, &(scrBookInst->pageInfoArea), &subModuAbsArea );
        status = status && status1;

        posValid = posValidInAbsPos( touchPos, &subModuAbsArea );
        //����������ҳ�������Ϣ
        if ( posValid )
        {
            scrPageInfoResp( &(scrBookInst->pageInfo), &subModuAbsArea, touchPos );
        }
    }


    return status;

}

//����setting�Դ���λ�ý�����Ӧ����
bool scrMainSettingResp( ScreenSetting *scrSettingInst, ScrMainAreaInfo *mainAreaInfo,\
                        AreaRange *absPos, PosType touchPos, ScreenColorPicker *scrColorPickerInst )
{

    bool status = true;
    bool status1, status2, status3;
    bool posValid = false;
    char *tempStr;
    AreaRange tempAbsArea, safeArea, AreatoShow;

    //�ȶ԰��µİ��������жϣ��ٽ�����Ӧ��Ļ����

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

//����colorPicker�Դ���λ�ý�����Ӧ����
bool scrMainColorPickerResp( ScreenColorPicker *scrColorPickerInst, ScrMainAreaInfo *mainAreaInfo,\
                        AreaRange *absPos, PosType touchPos )
{
    bool status = true;
    bool status1;
    bool posValid;



    //�ȶ԰��µİ��������жϣ��ٽ�����Ӧ��Ļ����

    AreaRange colorTableAbsArea;
    status1 = getAbsPos( absPos, &(scrColorPickerInst->colorPickerArea), &colorTableAbsArea );
    status = status && status1;

    posValid = posValidInAbsPos( touchPos, &colorTableAbsArea );

//    printf( "posValid=%s\n", posValid?"true":"false" );

    if ( posValid )
    {
        //ȡɫ��԰���������Ӧ����
        status1 = colorBoardResp( &(scrColorPickerInst->colorPicker), &colorTableAbsArea, touchPos );
        status = status && status1;
    }

    return status;

}

//ColorBoard�Դ���λ�ý�����Ӧ����
bool colorBoardResp( ColorBoard *colorPicker, AreaRange *absPos, PosType touchPos )
{
    bool status = true;
    bool status1;
    bool posValid;

//    displayAreaRange( absPos, 0 );

    //���㴥��λ����colorPicker�е����λ��
    PosType touchRelaPos;
    touchRelaPos.x_pos = touchPos.x_pos - absPos->x_min + 1;
    touchRelaPos.y_pos = touchPos.y_pos - absPos->y_min + 1;

    //���㴥����ɫ��λ��
    short colorXIndex, colorYIndex;

    colorXIndex = ( touchRelaPos.x_pos - 1) / colorPicker->colorXSize;//��0��ʼ
    colorYIndex = ( touchRelaPos.y_pos - 1) / colorPicker->colorYSize;//��0��ʼ

    //��ѡ�е���ɫ��256ɫ�е�λ��
    short newColorSel = colorXIndex + colorYIndex * colorPicker->colorXNum;



    //���ԭ��ѡ�е�ɫ��
    status1 = clearColorSelBorder( colorPicker, absPos );
    status = status && status1;



    //������ɫ�±걣������
    if ( colorPicker->colorIndex != (void*)0 )
    {
        *(colorPicker->colorIndex) = newColorSel;
    }
    else
    {
        status = false;
    }

    //��ʾ��ѡ�е���ɫ
    status1 = showColorSelBorder( colorPicker, absPos );
    status = status && status1;


    return status;
}

//ҳ�������Ϣ�Դ���λ�ý�����Ӧ����
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
        //���ȷʵ������������ɵ����
        if ( posValid )
        {
            //�޸��Ķ����ȣ���SD���ж�ȡ������
            float pageRatio = 1.0 * scrPageInfoInst->prgBarX.xOffset / scrPageInfoInst->prgBarX.xOffsetMax;
            printf( "offset = %d, max = %d\n", scrPageInfoInst->prgBarX.xOffset, scrPageInfoInst->prgBarX.xOffsetMax );
            readBookOfTxtFile( scrPageInfoInst->txtFileInfo, BOOK_PAGE_JUMP, pageRatio );
            //�޸Ľ��Ȱٷֱ�
            char *strPercent = scrPageInfoInst->tagPagePercTage.tagText.text;
            sprintf( strPercent, "%6.2f", pageRatio * 100 );
            strPercent[6] = '%';
            strPercent[7] = 0;
            scrPageInfoInst->tagPagePercTage.tagText.textLen = 7;
//            printf( "haha1\n" );

            //�޸�ҳ��
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

                //ˢ��ҳ���Ͱٷ�������
                AreaRange subModuAbsPos;
                //�ٷ�������
                status1 = getAbsPos( absPos, &(scrPageInfoInst->tagPagePercTageArea), &subModuAbsPos );
                status = status && status1;
                status1 = showTagBlock( &(scrPageInfoInst->tagPagePercTage), &subModuAbsPos );
                status = status && status1;

                //ҳ������ҳ������ʽΪ��"1234/99999", 1234Ϊ��ǰҳ����99999Ϊ��ҳ����
                status1 = getAbsPos( absPos, &(scrPageInfoInst->tagPageNumArea), &subModuAbsPos );
                status = status && status1;
                status1 = showTagBlock( &(scrPageInfoInst->tagPageNum), &subModuAbsPos );
                status = status && status1;
            }
        }
    }

    return status;
}


//ҳ����Ⱥ���������Դ���λ�ý�����Ӧ����(����ֵΪtrue��ʾ�����˽������������Ϊfalse��ʾû�а�����
bool prgBarXResp( ProgressBarX *prgBarXInst, AreaRange *absPos, PosType touchPos )
{
    bool posValid;
    AreaRange clickBarAbsArea;
    getAbsPos( absPos, &(prgBarXInst->clickArea), &clickBarAbsArea );

    posValid = posValidInAbsPos( touchPos, &clickBarAbsArea );

    //�û�����������������
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
        //��ʾ������
        showProgressBarX( prgBarXInst, absPos );
    }
    return posValid;
}
//����Ŀ¼��������Ӧ����
bool scrollBarYResp( ScrollBarY *scrollBarYInst, AreaRange *absPos, PosType touchPos, ScreenHome *scrHomeInst )
{
    bool status = true;
    bool status1;
    bool posValid;


    AreaRange subModuAbsArea;

    //���������Ƽ�
    status1 = getAbsPos( absPos, &(scrollBarYInst->iconMoveUpArea), &subModuAbsArea );
    status = status && status1;
    posValid = posValidInAbsPos( touchPos, &subModuAbsArea );
    if ( posValid )
    {
        if ( scrollBarYInst->iconBarVisible )//�ɹ���
        {
            //�޸�Ŀ¼�б���ʾƫ����
            scrHomeInst->listYOffset = scrHomeInst->listYOffset - scrHomeInst->listYStepSize;
            if ( scrHomeInst->listYOffset < 0 )
            {
                scrHomeInst->listYOffset = 0;
            }
            //�޸Ĺ�������ƫ����
            float ratio = 1.0 * scrHomeInst->listYOffset / scrHomeInst->listYOffsetMax;
            scrollBarYInst->barYoffset = ratio * scrollBarYInst->barYOffsetMax;
            if ( scrollBarYInst->barYoffset < 0 )
            {
                scrollBarYInst->barYoffset = 0;
            }
            //ˢ����ҳ��
            status1 = showScreenMain( &tftlcdScreenInst );
            status = status && status1;
        }
        return status;
    }

    //���������Ƽ�
    status1 = getAbsPos( absPos, &(scrollBarYInst->iconMoveDownArea), &subModuAbsArea );
    status = status && status1;
    posValid = posValidInAbsPos( touchPos, &subModuAbsArea );
    if ( posValid )
    {
        if ( scrollBarYInst->iconBarVisible )//�ɹ���
        {
            //�޸�Ŀ¼�б���ʾƫ����
            scrHomeInst->listYOffset = scrHomeInst->listYOffset + scrHomeInst->listYStepSize;
            if ( scrHomeInst->listYOffset > scrHomeInst->listYOffsetMax )
            {
                scrHomeInst->listYOffset = scrHomeInst->listYOffsetMax;
            }
            //�޸Ĺ�������ƫ����
            float ratio = 1.0 * scrHomeInst->listYOffset / scrHomeInst->listYOffsetMax;
            scrollBarYInst->barYoffset = ratio * scrollBarYInst->barYOffsetMax;
            if ( scrollBarYInst->barYoffset > scrollBarYInst->barYOffsetMax )
            {
                scrollBarYInst->barYoffset = scrollBarYInst->barYOffsetMax;
            }
            //ˢ����ҳ��
            status1 = showScreen( &tftlcdScreenInst );
            status = status && status1;
        }
        return status;
    }

    //�������������ƶ���
    status1 = getAbsPos( absPos, &(scrollBarYInst->barMoveArea), &subModuAbsArea );
    status = status && status1;
    posValid = posValidInAbsPos( touchPos, &subModuAbsArea );
    if ( posValid )
    {
        if ( scrollBarYInst->iconBarVisible )//�ɹ���
        {
            //�޸Ĺ�������ƫ����
            scrollBarYInst->barYoffset = touchPos.y_pos - subModuAbsArea.y_min - (scrollBarYInst->barHeight / 2);
            if ( scrollBarYInst->barYoffset < 0 )
            {
                scrollBarYInst->barYoffset = 0;
            }
            if ( scrollBarYInst->barYoffset > scrollBarYInst->barYOffsetMax )
            {
                scrollBarYInst->barYoffset = scrollBarYInst->barYOffsetMax;
            }

            //�޸�Ŀ¼�б���ʾƫ����
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

            //ˢ����ҳ��
            status1 = showScreen( &tftlcdScreenInst );
            status = status && status1;
        }
        return status;
    }
    return status;
}