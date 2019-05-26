/*
 * time_count.c
 *
 *  Created on: 2019��4��27��
 *      Author: 12757
 */

#include "time_count.h"
#include "screen.h"

TimeTaskInfo timeFlag;

//�ϱ���ʱ����ʾ�仯�жϵ���Ӧ����(ÿ��1����ʱ��仯һ�Σ�
bool scrHeaderTimeResp( ScreenHeaderTime *scrHeaderInst, AreaRange *absPos )
{
    bool status = true;
    bool status1;

    //Ҫ����ʱ���л�����Ҫ�����Զ���ҳ,��Ҫ������ʱ�䵽ʱʱ����
    if ( timeFlag.timeChangeFlag )
    {
        timeFlag.timeChangeFlag = false;
        //�޸��ϱ�����ʾʱ��

        /*

        д��





        */
    }

    if ( timeFlag.autoTurnPageFlag )
    {
        timeFlag.autoTurnPageFlag = false;
        //��ҳ

        /*

        ����Զ���ҳģʽ���ˣ����ҵ�ǰҳ��Ϊhomeҳ�棬�ʹ�SD���ж�ȡ��һҳ���ݣ�����ʾ

        д��

        */

    }

    if ( timeFlag.alarmFlag )
    {
        timeFlag.alarmFlag = false;
        //�뷽�跨���Ѷ�����Ϣ��������Ļ����"����Ϣ��",Ҳ���Է����LED�ơ�

        /*

        д��

        */
    }

    return status;
}

//ʱ���жϺ���
void timeCount1s_handler( void )
{
    //1.����ж�״̬
    //2.�����µļ�ʱ

    //3.�޸�ʱ��
    timeFlag.curSecondsCount ++;

    //ÿ��һ�����޸�һ����ʾʱ��
    if ( timeFlag.curSecondsCount % 60 == 0 )
    {
        timeFlag.timeChangeFlag = true;
    }

    //����Զ���ҳ����
    if ( tftlcdScreenInst.scrSetting.autoTurnPage )
    {

        if ( timeFlag.curATPSecCount < 0 )
        {
            timeFlag.curATPSecCount = 0;
        }
        else
        {
            //��ʱ+1
            timeFlag.curATPSecCount ++;
            //����Զ���ҳ��ʱ
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

    //�����������ʱ��
    if ( timeFlag.curSecondsCount == timeFlag.alarmSeconds )
    {
        timeFlag.curSecondsCount = 0;
        timeFlag.alarmFlag = true;
    }

    //����24Сʱ,����ʱ������
    if ( timeFlag.curSecondsCount < 0 || timeFlag.curSecondsCount >= 86400 )
    {
        timeFlag.curSecondsCount = 0;
    }

    return;
}

//ʱ���ʽת����ʱ���� -> �� )
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

////Ŀ¼���ݺ���������񣨲����ˣ�
//bool scrCatalogScrollXTask( ScreenHome *scrMainHomeInst, AreaRange *absPos )
//{
//    bool status = true;
//    bool status1;
//    bool posValid;
//
//    �ȶ԰��µİ��������жϣ��ٽ�����Ӧ����
//
//
//
//
//    return status;
//}
