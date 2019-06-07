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

	//一.硬件初始化
    //略

	printf( "haha2\n" );
	//二.软件初始化

	//1.读取并保存SD卡中的所有txt文件的文件名
	status = status && readAlltxtFilesOfSDcard( );

    displayStatus( "readAlltxtFilesOfSDcard", "main", status );

//    printf( "txtFilesInfoSpace = {\n" );
//    displayTxtFilesInfo( &txtFilesInfoSpace, 4 );
//    printf( "}\n" );


	//2.初始化ScreenContainer屏幕显示信息结构体
	status1 = screenInit();
	status = status && status1;

	displayStatus( "screenInit", "main", status );

//    printf( "tftlcdScreenInst = {\n" );
//    displayScreenContainer( &tftlcdScreenInst, 4 );
//    printf( "}\n" );

	//3.执行屏幕显示函数
	status1 = showScreen( );
	status = status && status1;

	displayStatus( "showScreen", "main", status );

	printf( "haha3\n" );

    //4.保存屏幕数组数据
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


	//三.主程序执行
	//屏幕不忙
	touchPosFlag.scrTaskBusy = false;

	int i;
	for ( i = 0; i < 2; i ++ )
    {
        //临时打断while循环
        touchPosFlag.touchPos.x_pos = posArray[i][0];
        touchPosFlag.touchPos.y_pos = posArray[i][1];
        touchPosFlag.touchOccur = true;//开启工作标志
        touchPosFlag.scrTaskBusy = true;
        screenRespTask( &tftlcdScreenInst );
    }

    printf( "\nstore screen picture data in image.txt\nstart\n" );
	storeScreenArrayToFile( );
	printf( "finished\n" );

    return 0;//小心：此处return，程序结束


	while( 1 )
    {
		//时间响应任务
//		scrHeaderTimeResp( &(tftlcdScreenInst.scrHeader), &(tftlcdScreenInst.headerArea) );

        //输入新点击位置，模拟用点击操作
        printf( "x = " );
        scanf( "%hd", &(touchPosFlag.touchPos.x_pos) );
        printf( "y = " );
        scanf( "%hd", &(touchPosFlag.touchPos.y_pos) );
        printf( "(x,y) = (%d,%d)\n", touchPosFlag.touchPos.x_pos, touchPosFlag.touchPos.y_pos );

        touchPosFlag.touchOccur = true;//开启工作标志


        touchPosFlag.scrTaskBusy = true;
        screenRespTask( &tftlcdScreenInst );

        //debug
//        if ( tftlcdScreenInst.mainAreaInfo.scrIDStack[tftlcdScreenInst.mainAreaInfo.curScrIndex] == SCR_BOOK )
//        {
//            displayTxtBookSpace( );
//        }

        //保存数组至文件中
        printf( "\nstore screen picture data in image.txt\nstart\n" );
        storeScreenArrayToFile( );
        printf( "finished\n" );


		touchPosFlag.scrTaskBusy = false;
    }

	return 0;
}
