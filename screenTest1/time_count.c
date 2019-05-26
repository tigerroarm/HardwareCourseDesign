/*
 * time_count.c
 *
 *  Created on: 2019年4月27日
 *      Author: 12757
 */

#include "time_count.h"
#include "screen.h"

TimeTaskInfo timeFlag;

//上边栏时间显示变化中断的响应任务(每隔1分钟时间变化一次）
bool scrHeaderTimeResp( ScreenHeaderTime *scrHeaderInst, AreaRange *absPos )
{
    bool status = true;
    bool status1;

    //要管理时间切换，还要管理自动翻页,还要在闹钟时间到时时报警
    if ( timeFlag.timeChangeFlag )
    {
        timeFlag.timeChangeFlag = false;
        //修改上边栏显示时间

        /*

        写吧





        */
    }

    if ( timeFlag.autoTurnPageFlag )
    {
        timeFlag.autoTurnPageFlag = false;
        //翻页

        /*

        如果自动翻页模式打开了，而且当前页面为home页面，就从SD卡中读取下一页内容，并显示

        写吧

        */

    }

    if ( timeFlag.alarmFlag )
    {
        timeFlag.alarmFlag = false;
        //想方设法提醒读者休息，可以屏幕弹出"该休息了",也可以疯狂闪LED灯。

        /*

        写吧

        */
    }

    return status;
}

//时间中断函数
void timeCount1s_handler( void )
{
    //1.清除中断状态
    //2.开启新的计时

    //3.修改时间
    timeFlag.curSecondsCount ++;

    //每隔一分钟修改一次显示时间
    if ( timeFlag.curSecondsCount % 60 == 0 )
    {
        timeFlag.timeChangeFlag = true;
    }

    //如果自动翻页开启
    if ( tftlcdScreenInst.scrSetting.autoTurnPage )
    {

        if ( timeFlag.curATPSecCount < 0 )
        {
            timeFlag.curATPSecCount = 0;
        }
        else
        {
            //计时+1
            timeFlag.curATPSecCount ++;
            //如果自动翻页到时
            if ( timeFlag.curATPSecCount >= timeFlag.autoTPSeconds )
            {
                timeFlag.curATPSecCount = 0;
                timeFlag.autoTurnPageFlag = true;
            }
        }
    }
    else
    {
        timeFlag.curATPSecCount = 0;
    }

    //如果到了闹钟时间
    if ( timeFlag.curSecondsCount == timeFlag.alarmSeconds )
    {
        timeFlag.curSecondsCount = 0;
        timeFlag.alarmFlag = true;
    }

    //到了24小时,或者时间有误
    if ( timeFlag.curSecondsCount < 0 || timeFlag.curSecondsCount >= 86400 )
    {
        timeFlag.curSecondsCount = 0;
    }

    return;
}

//时间格式转换（时分秒 -> 秒 )
int getTimeInSeconds( TimeType* timePtr )
{
    if ( timePtr == (void*)0 )
    {
        return -1;
    }

    int secVal;

    secVal = timePtr->timeHour * 3600 + timePtr->timeMinute * 60 + timePtr->timeSecond;

    return secVal;
}

////目录内容横向滚动任务（不搞了）
//bool scrCatalogScrollXTask( ScreenHome *scrMainHomeInst, AreaRange *absPos )
//{
//    bool status = true;
//    bool status1;
//    bool posValid;
//
//    先对按下的按键进行判断，再进行相应操作
//
//
//
//
//    return status;
//}
