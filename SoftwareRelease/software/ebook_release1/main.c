#include <stdio.h>
#include <stdlib.h>

#include "screen.h"
#include "screen_display.h"
#include "touch.h"
#include "touch_resp.h"
#include "tftlcd.h"
#include "time_count.h"

/*
ע�����SD���ļ��򿪺�һƬ�հף��뽫Altera_UP_SD_Card_Avalon_Interface.h�����
����ͷ�ļ�:
#include <stdbool.h>
*/

//extern TxtFilesInfo txtFilesInfoSpace;

int main( void )
{

	bool status = true, status1;


	//һ.Ӳ����ʼ��
	//1.����оƬ��ʼ��
	TP_Init();
	//2.LCD��Ļ��ʼ��
	LCD_init();
	LCD_Clear(0x4698);//����
	//3.��ʱ����ʼ��
	Timer_Initial();



	//��.�����ʼ��

	//1.��ȡ������SD���е�����txt�ļ����ļ���
	status = status && readAlltxtFilesOfSDcard( );


	//2.��ʼ��ScreenContainer��Ļ��ʾ��Ϣ�ṹ��
	status1 = screenInit();
	status = status && status1;


	//3.ִ����Ļ��ʾ����
	status1 = showScreen( );
	status = status && status1;


	//��.������ִ��
	//��Ļ��æ
	touchPosFlag.scrTaskBusy = false;

	while( 1 )
    {
		//ʱ����Ӧ����
		scrHeaderTimeResp( &(tftlcdScreenInst.scrHeader), &(tftlcdScreenInst.headerArea) );

		//������Ӧ����
		if ( touchPosFlag.touchOccur )
		{
			touchPosFlag.scrTaskBusy = true;
			screenRespTask( &tftlcdScreenInst );
		}
		touchPosFlag.scrTaskBusy = false;
    }

	return 0;
}
