#include <stdio.h>
#include <stdlib.h>

#include "screen.h"
#include "screen_display.h"
#include "touch.h"
#include "touch_resp.h"
#include "tftlcd.h"
#include "time_count.h"

/*
注：如果SD卡文件打开后一片空白，请将Altera_UP_SD_Card_Avalon_Interface.h中添加
以下头文件:
#include <stdbool.h>
*/

//extern TxtFilesInfo txtFilesInfoSpace;

int main( void )
{

	bool status = true, status1;


	//一.硬件初始化
	//1.触摸芯片初始化
	TP_Init();
	//2.LCD屏幕初始化
	LCD_init();
	LCD_Clear(0x4698);//清屏
	//3.定时器初始化
	Timer_Initial();



	//二.软件初始化

	//1.读取并保存SD卡中的所有txt文件的文件名
	status = status && readAlltxtFilesOfSDcard( );


	//2.初始化ScreenContainer屏幕显示信息结构体
	status1 = screenInit();
	status = status && status1;


	//3.执行屏幕显示函数
	status1 = showScreen( );
	status = status && status1;


	//三.主程序执行
	//屏幕不忙
	touchPosFlag.scrTaskBusy = false;

	while( 1 )
    {
		//时间响应任务
		scrHeaderTimeResp( &(tftlcdScreenInst.scrHeader), &(tftlcdScreenInst.headerArea) );

		//触摸响应任务
		if ( touchPosFlag.touchOccur )
		{
			touchPosFlag.scrTaskBusy = true;
			screenRespTask( &tftlcdScreenInst );
		}
		touchPosFlag.scrTaskBusy = false;
    }

	return 0;
}
