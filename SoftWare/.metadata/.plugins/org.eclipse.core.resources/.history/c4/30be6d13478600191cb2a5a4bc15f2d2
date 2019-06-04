/*
 * time_count.h
 *
 * 本库与定时器有关，处理所有与时间有关的任务,如计数得到阅读时间
 *
 *
 *  Created on: 2019年4月27日
 *      Author: 12757
 */

#ifndef TIME_COUNT_H_
#define TIME_COUNT_H_


#include <stdbool.h>
#include "alt_types.h"
#include "screen_type.h"



extern unsigned int timer_isr_context;// 定义全局变量以储存isr_context指针
//时间中断函数与主函数通信
typedef struct
{
    bool timeCount1sStatus;//1s定时器开启关闭的状态(true为开启，false为关闭）
//    bool timeCountXmsStatus;//横向滚动定时器开启关闭的状态(true为开启，false为关闭）

    int curSecondsCount;//当前秒数计数

    int alarmSeconds;//闹钟定时时间

    int curATPSecCount;//当前自动翻页计时秒数计数
    int autoTPSeconds;//自动翻页定时

    int alarmSecCount; //alarm的计数(用来使上边栏背景会一闪一闪)
    bool alarmState;//为true时，为闹钟状态，上边栏背景会一闪一闪

    bool timeChangeFlag;//显示时间变化的标志，每一分钟来一次变化
    bool alarmFlag;//闹钟定时护眼时间到达标志
    bool autoTurnPageFlag;//自动翻页定时时间到的标志

//    bool timeScrollXFlag;//横向滚动时间到来标志

    //当前时间
} TimeTaskInfo;

extern TimeTaskInfo timeFlag;

//上边栏时间显示变化中断的响应任务(每隔1分钟时间变化一次）
bool scrHeaderTimeResp( ScreenHeaderTime *scrHeaderInst, AreaRange *absPos );

//时间格式转换（时分秒 -> 秒 )
int getTimeInSeconds( TimeType* timePtr );

//时间中断函数
void timeCount1s_handler( void* isr_context );

void Timer_Initial(void);
void Timer_ISR(void* isr_context);



#endif /* TIME_COUNT_H_ */
