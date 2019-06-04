#include <stdio.h>
#include <stdlib.h>

#include "screen.h"
#include "screen_display.h"
#include "test_value.h"
#include "touch.h"
#include "touch_resp.h"
#include "tftlcd.h"
#include "time_count.h"

//extern TxtFilesInfo txtFilesInfoSpace;

int main( void )
{

	bool status = true, status1;

	printf( "haha\n" );

	//һ.Ӳ����ʼ��
	//1.����оƬ��ʼ��
	TP_Init();
//	//2.LCD��Ļ��ʼ��
	LCD_init();
	LCD_Clear(0x4698);//����
	//3.��ʱ����ʼ��
	Timer_Initial();


	printf( "haha2\n" );
	//��.������ʼ��

	//1.��ȡ������SD���е�����txt�ļ����ļ���
	status = status && readAlltxtFilesOfSDcard( );

    displayStatus( "readAlltxtFilesOfSDcard", "main", status );

//    printf( "txtFilesInfoSpace = {\n" );
//    displayTxtFilesInfo( &txtFilesInfoSpace, 4 );
//    printf( "}\n" );


	//2.��ʼ��ScreenContainer��Ļ��ʾ��Ϣ�ṹ��
	status1 = screenInit();
	status = status && status1;

	displayStatus( "screenInit", "main", status );

//    printf( "tftlcdScreenInst = {\n" );
//    displayScreenContainer( &tftlcdScreenInst, 4 );
//    printf( "}\n" );

	//3.ִ����Ļ��ʾ����
	status1 = showScreen( );
	status = status && status1;

	displayStatus( "showScreen", "main", status );

	printf( "haha3\n" );


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
			printf( "(x,y) = (%d,%d)\n", touchPosFlag.touchPos.x_pos, touchPosFlag.touchPos.y_pos );
			touchPosFlag.scrTaskBusy = true;
			screenRespTask( &tftlcdScreenInst );

		}
		touchPosFlag.scrTaskBusy = false;
    }

	return 0;
}