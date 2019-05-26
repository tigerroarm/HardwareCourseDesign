/*
 * time_count.h
 *
 * �����붨ʱ���йأ�����������ʱ���йص�����,������õ��Ķ�ʱ��
 *
 *
 *  Created on: 2019��4��27��
 *      Author: 12757
 */

#ifndef TIME_COUNT_H_
#define TIME_COUNT_H_


#include <stdbool.h>
#include "alt_types.h"
#include "screen_type.h"




//ʱ���жϺ�����������ͨ��
typedef struct
{
    bool timeCount1sStatus;//1s��ʱ�������رյ�״̬(trueΪ������falseΪ�رգ�
//    bool timeCountXmsStatus;//���������ʱ�������رյ�״̬(trueΪ������falseΪ�رգ�

    int curSecondsCount;//��ǰ��������

    int alarmSeconds;//���Ӷ�ʱʱ��

    int curATPSecCount;//��ǰ�Զ���ҳ��ʱ��������

    int autoTPSeconds;//�Զ���ҳ��ʱ

    bool timeChangeFlag;//��ʾʱ��仯�ı�־��ÿһ������һ�α仯
    bool alarmFlag;//���Ӷ�ʱ����ʱ�䵽���־
    bool autoTurnPageFlag;//�Զ���ҳ��ʱʱ�䵽�ı�־
//    bool timeScrollXFlag;//�������ʱ�䵽����־

    //��ǰʱ��
} TimeTaskInfo;

extern TimeTaskInfo timeFlag;

//�ϱ���ʱ����ʾ�仯�жϵ���Ӧ����(ÿ��1����ʱ��仯һ�Σ�
bool scrHeaderTimeResp( ScreenHeaderTime *scrHeaderInst, AreaRange *absPos );

//ʱ���ʽת����ʱ���� -> �� )
int getTimeInSeconds( TimeType* timePtr );

//ʱ���жϺ���
void timeCount1s_handler( void );

#endif /* TIME_COUNT_H_ */
