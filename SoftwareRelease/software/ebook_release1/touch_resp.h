/*
 * touch_resp.h
 *
 * 本库实现对触摸操作的响应，进行相应的函数操作
 *
 *  Created on: 2019年4月27日
 *      Author: 12757
 */

#ifndef TOUCH_RESP_H_
#define TOUCH_RESP_H_

#include <stdbool.h>
#include "alt_types.h"
#include "screen_type.h"




//中断触摸函数与主函数通信结构体

typedef struct
{
    bool touchIntrStatus;//触摸中断开启关闭的状态记录

    bool touchOccur;//触摸到来标志
    bool scrTaskBusy;//主函数屏幕操作忙标志(主函数忙的时候，触摸中断函数啥也不做，乖乖等待）

    PosType touchPos;
} TouchTaskInfo;

extern TouchTaskInfo touchPosFlag;


//判断触摸位置是否在绝对区域内
bool posValidInAbsPos( PosType touchPos, AreaRange *absPos );

//主函数对触摸位置进行响应操作
bool screenRespTask( ScreenContainer *screenInst );

//上边栏对触摸位置进行响应操作
bool scrHeaderResp( ScreenHeaderTime *scrHeaderInst,\
                        AreaRange *absPos, PosType touchPos );

//下边栏对触摸位置进行响应操作
bool scrFooterResp( ScreenFooterBtn *scrFooterInst, ScrMainAreaInfo *mainAreaInfo,\
                        AreaRange *absPos, PosType touchPos );


//主栏home对触摸位置进行响应操作
bool scrMainHomeResp( ScreenHome *scrHomeInst, ScrMainAreaInfo *mainAreaInfo,\
                        AreaRange *absPos, PosType touchPos, ScreenBook *scrBookInst );

//主栏book对触摸位置进行响应操作
bool scrMainBookResp( ScreenBook *scrBookInst, ScrMainAreaInfo *mainAreaInfo,\
                        AreaRange *absPos, PosType touchPos );

//主栏setting对触摸位置进行响应操作
bool scrMainSettingResp( ScreenSetting *scrSettingInst, ScrMainAreaInfo *mainAreaInfo,\
                        AreaRange *absPos, PosType touchPos, ScreenColorPicker *scrColorPickerInst );

//主栏colorPicker对触摸位置进行响应操作
bool scrMainColorPickerResp( ScreenColorPicker *scrColorPickerInst, ScrMainAreaInfo *mainAreaInfo,\
                        AreaRange *absPos, PosType touchPos );


//ColorBoard对触摸位置进行响应操作
bool colorBoardResp( ColorBoard *colorPicker, AreaRange *absPos, PosType touchPos );

//页面进度信息对触摸位置进行响应操作
bool scrPageInfoResp( ScreenPageInfo *scrPageInfoInst, AreaRange *absPos, PosType touchPos );


//页面进度横向进度条对触摸位置进行响应操作(返回值为true表示按到了进度条点击区，为false表示没有按到）
bool prgBarXResp( ProgressBarX *prgBarXInst, AreaRange *absPos, PosType touchPos );

//纵向目录滚动条响应操作
bool scrollBarYResp( ScrollBarY *scrollBarYInst, AreaRange *absPos, PosType touchPos, ScreenHome *scrHomeInst );

#endif /* TOUCH_RESP_H_ */
