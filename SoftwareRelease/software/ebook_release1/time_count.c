/*
 * time_count.c
 *
 *  Created on: 2019��4��27��
 *      Author: 12757
 */

#include "time_count.h"
#include "screen.h"
#include "sys/alt_irq.h"
#include "system.h"
#include "altera_avalon_timer_regs.h" // ��ʱ��
#include "screen_display.h"

TimeTaskInfo timeFlag;
unsigned int timer_isr_context;// ����ȫ�ֱ����Դ���isr_contextָ��

//�ϱ���ʱ����ʾ�仯�жϵ���Ӧ����(ÿ��1����ʱ��仯һ�Σ�
bool scrHeaderTimeResp( ScreenHeaderTime *scrHeaderInst, AreaRange *absPos )
{
    bool status = true;
    bool status1;

    //Ҫ����ʱ���л�����Ҫ�����Զ���ҳ,��Ҫ������ʱ�䵽ʱʱ����
    if ( timeFlag.timeChangeFlag )
    {
        timeFlag.timeChangeFlag = false;

        //ͬ���ϱ���ʱ���ַ���
        screenSyncHeaderTime();

        //��ʾʱ��
		AreaRange tmprange;
		//�õ�ʱ���ǩ�ľ���λ��
		getAbsPos(absPos, &(scrHeaderInst->tagTimeArea), &tmprange);
		//������ʾ�ַ���
		status1 = showTagBlock( &(scrHeaderInst->tagTime), &tmprange );
		status = status & status1;
    }

    if ( timeFlag.autoTurnPageFlag )
    {
        timeFlag.autoTurnPageFlag = false;


        //��ҳ
        if(tftlcdScreenInst.scrSetting.autoTurnPage && tftlcdScreenInst.mainAreaInfo.scrIDStack[tftlcdScreenInst.mainAreaInfo.curScrIndex] == SCR_BOOK)
        {
            //�Զ���ҳ
        	TxtFile *txtBookInst = tftlcdScreenInst.scrBook.pageInfo.txtFileInfo;
        	if ( txtBookInst != NULL )
        	{
				//�����δ�����һҳ
				if(txtBookInst->curPageBytePosEnd < txtBookInst->fileSizeInBytes){
					readBookOfTxtFile( tftlcdScreenInst.scrBook.pageInfo.txtFileInfo,  BOOK_PAGE_NEXT, 0 );
		            //ͬ���Ķ����ȱ�ǩ��Ϣ
		            status1 = screenSyncPageRatio( &(tftlcdScreenInst.scrBook.pageInfo ) );
		            status = status && status1;
		            //��ʾbookҳ��
					status1 = showScreenMainBook( &(tftlcdScreenInst.scrBook), &(tftlcdScreenInst.mainArea) );
					status = status && status1;

				}
				else
				{
//					setAutoTPGstate( false );//�ر��Զ���ҳ
				}
        	}
        }
    }

    if ( timeFlag.alarmFlag )
    {
    	timeFlag.alarmFlag = false;
    	//�뷽�跨���Ѷ�����Ϣ��������Ļ����"����Ϣ��",Ҳ���Է����LED�ơ�

		//�ϱ�������һ��һ��
    	if ( timeFlag.alarmSecCount == 0 )
		{
    		timeFlag.alarmSecCount = 1;
    		scrHeaderInst->bkgBorderColor.bkgColor = getColor16BitByRGB( 250, 0, 0 );
		}
		else
		{
			timeFlag.alarmSecCount = 0;
			scrHeaderInst->bkgBorderColor.bkgColor = getColor16BitByRGB( 210, 125, 33 );
		}
		//��ʾ����
		status1 = showBkgAndBorder( &(scrHeaderInst->bkgBorderColor), absPos, &(scrHeaderInst->tagTimeArea), 1 );
		status = status & status1;
    }

    return status;
}

//ʱ���жϺ���
void timeCount1s_handler( void* isr_context )
{


	//1.����ж�״̬�������µļ�ʱ
	IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_1S_BASE,
		  ~ ALTERA_AVALON_TIMER_STATUS_TO_MSK);

	//3.�޸�ʱ��
	timeFlag.curSecondsCount ++;

	//ÿ��һ�����޸�һ����ʾʱ��
//    if ( timeFlag.curSecondsCount % 60 == 0 )
//    {
		timeFlag.timeChangeFlag = true;
//    }

	//����Զ���ҳ����
	if ( tftlcdScreenInst.scrSetting.autoTurnPage && tftlcdScreenInst.mainAreaInfo.scrIDStack[tftlcdScreenInst.mainAreaInfo.curScrIndex] == SCR_BOOK )
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
	//���֮ǰ��������״̬
	if ( timeFlag.alarmState == false )
	{
		if ( timeFlag.curSecondsCount >= timeFlag.alarmSeconds )
		{
			//��ʼ����״̬
			timeFlag.alarmState = true;
			//��������alarm��־
			timeFlag.alarmFlag = true;
			timeFlag.alarmSecCount = 0;
			timeFlag.curSecondsCount = timeFlag.alarmSeconds;
		}
	}
	else//���֮ǰ������״̬
	{
		//��������alarm��־
		timeFlag.alarmFlag = true;
		timeFlag.curSecondsCount = timeFlag.alarmSeconds;
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

void Timer_Initial(void)
{
	//timeFlag��ʼ��
	timeFlag.curSecondsCount = 0;

	timeFlag.autoTPSeconds = 5;
	timeFlag.curATPSecCount = 0;

	timeFlag.alarmSecCount = 0;
	timeFlag.alarmSeconds = 15;
	timeFlag.alarmState = false;

	timeFlag.alarmFlag = false;
	timeFlag.autoTurnPageFlag = false;
	timeFlag.timeChangeFlag = false;

	// ��дtimer_isr_contextָ����ƥ��alt_irq_register()����ԭ��

	  void* isr_context_ptr = (void*) &timer_isr_context;
	  // ����PERIOD�Ĵ���
	  // PERIODH << 16 | PERIODL = �������������� * ϵͳʱ��Ƶ������ - 1
	  // PERIODH << 16 | PERIODL = 5m*100M - 1 = 499999 = 0x7A11F
	  IOWR_ALTERA_AVALON_TIMER_PERIODH(TIMER_1S_BASE, 0x0007);
	  IOWR_ALTERA_AVALON_TIMER_PERIODL(TIMER_1S_BASE, 0xA11F);

	  // ����CONTROL�Ĵ���
	  //    λ�� |  3   |  2   |  1   |  0  |
	  // CONTROL | STOP | START| CONT | ITO |
	  // ITO   1������IRO��                      0��������IRQ
	  // CONT  1����������������ֱ��STOP����һ��   0��������0ֹͣ
	  // START 1����������ʼ���У�                0����Ӱ��
	  // STOP  1��������ֹͣ���У�                0����Ӱ��
	  IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_1S_BASE,
	    ALTERA_AVALON_TIMER_CONTROL_START_MSK |// START = 1
	    ALTERA_AVALON_TIMER_CONTROL_CONT_MSK  |// CONT  = 1
	    ALTERA_AVALON_TIMER_CONTROL_ITO_MSK);  // ITO   = 1
	  // ע�ᶨʱ���ж�
	  alt_ic_isr_register(
		  TIMER_1S_IRQ_INTERRUPT_CONTROLLER_ID,// �жϿ�������ţ���system.h����
		  TIMER_1S_IRQ,    // Ӳ���жϺţ���system.h����
		  timeCount1s_handler,             // �жϷ����Ӻ���
	      isr_context_ptr,       // ָ�����豸����ʵ����ص����ݽṹ��
	      0x0);                  // flags������δ��
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