#include <stdio.h>
#include <stdlib.h>

#include "screen.h"
#include "screen_display.h"
#include "test_value.h"
#include "touch_resp.h"
#include "time_count.h"

//extern TxtFilesInfo txtFilesInfoSpace;

int main( void )
{

	bool status = true, status1;

	printf( "haha\n" );

	//һ.Ӳ����ʼ��
    //��

	printf( "haha2\n" );
	//��.�����ʼ��

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

    //4.������Ļ��������
	printf( "\nstore screen picture data in image.txt\nstart\n" );
	storeScreenArrayToFile( );
	printf( "finished\n" );


	#define POS_NUM 4

	short posArray[POS_NUM][2] = {
                                    {129,121},
                                    {160,211},
                                    {300,350},
                                    {48 ,163}
                                 };


	//��.������ִ��
	//��Ļ��æ
	touchPosFlag.scrTaskBusy = false;

	int i;
	for ( i = 0; i < 2; i ++ )
    {
        //��ʱ���whileѭ��
        touchPosFlag.touchPos.x_pos = posArray[i][0];
        touchPosFlag.touchPos.y_pos = posArray[i][1];
        touchPosFlag.touchOccur = true;//����������־
        touchPosFlag.scrTaskBusy = true;
        screenRespTask( &tftlcdScreenInst );
    }

    printf( "\nstore screen picture data in image.txt\nstart\n" );
	storeScreenArrayToFile( );
	printf( "finished\n" );

    return 0;//С�ģ��˴�return���������


	while( 1 )
    {
		//ʱ����Ӧ����
//		scrHeaderTimeResp( &(tftlcdScreenInst.scrHeader), &(tftlcdScreenInst.headerArea) );

        //�����µ��λ�ã�ģ���õ������
        printf( "x = " );
        scanf( "%hd", &(touchPosFlag.touchPos.x_pos) );
        printf( "y = " );
        scanf( "%hd", &(touchPosFlag.touchPos.y_pos) );
        printf( "(x,y) = (%d,%d)\n", touchPosFlag.touchPos.x_pos, touchPosFlag.touchPos.y_pos );

        touchPosFlag.touchOccur = true;//����������־


        touchPosFlag.scrTaskBusy = true;
        screenRespTask( &tftlcdScreenInst );

        //debug
//        if ( tftlcdScreenInst.mainAreaInfo.scrIDStack[tftlcdScreenInst.mainAreaInfo.curScrIndex] == SCR_BOOK )
//        {
//            displayTxtBookSpace( );
//        }

        //�����������ļ���
        printf( "\nstore screen picture data in image.txt\nstart\n" );
        storeScreenArrayToFile( );
        printf( "finished\n" );


		touchPosFlag.scrTaskBusy = false;
    }

	return 0;
}
