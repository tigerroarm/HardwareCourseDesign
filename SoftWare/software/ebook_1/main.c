/*
 * main.c
 *
 *  Created on: 2019��4��26��
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

	//1.��ȡ������SD���е�����txt�ļ����ļ���
	status = status && readAlltxtFilesOfSDcard( );

	//2.��ʼ��ScreenContainer��Ļ��ʾ��Ϣ�ṹ��
	status = status && screenInit();

	//3.ִ����Ļ��ʾ����
	status = status && showScreen( );

	return 0;
}
