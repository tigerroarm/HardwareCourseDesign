/*
 * main.c
 *
 *  Created on: 2019年4月26日
 *      Author: 12757
 */

#include <stdio.h>
#include "alt_types.h"
#include <unistd.h>
#include "system.h"
#include "sys/alt_irq.h"
#include "altera_up_avalon_parallel_port_regs.h"

#include "screen.h"
#include "screen_display.h"

int main( void )
{

	bool status = true;

	//1.读取并保存SD卡中的所有txt文件的文件名
	status = status && readAlltxtFilesOfSDcard( );

	//2.初始化ScreenContainer屏幕显示信息结构体
	status = status && screenInit();

	//3.执行屏幕显示函数
	status = status && showScreen( );

	return 0;
}
