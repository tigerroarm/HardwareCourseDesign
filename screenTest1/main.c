#include <stdio.h>
#include <stdlib.h>

#include "screen.h"
#include "screen_display.h"
#include "test_value.h"
#include "touch_resp.h"

extern TxtFilesInfo txtFilesInfoSpace;

int main( void )
{

	bool status = true, status1;

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



	//4.保存屏幕数组数据
	printf( "\nstore screen picture data in image.txt\nstart\n" );
	storeScreenArrayToFile( );
	printf( "finished\n" );



	//5.debug
	while( 1 )
    {
        printf( "x = " );
        scanf( "%hd", &(touchPosFlag.touchPos.x_pos) );
        printf( "y = " );
        scanf( "%hd", &(touchPosFlag.touchPos.y_pos) );
        printf( "(x,y) = (%d,%d)\n", touchPosFlag.touchPos.x_pos, touchPosFlag.touchPos.y_pos );

        touchPosFlag.touchOccur = true;//开启工作标志

        screenRespTask( &tftlcdScreenInst );


        printf( "\nstore screen picture data in image.txt\nstart\n" );
        storeScreenArrayToFile( );
        printf( "finished\n" );

    }

	return 0;
}
