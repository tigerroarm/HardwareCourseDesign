/*
 * "Hello World" example.
 *
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example
 * designs. It runs with or without the MicroC/OS-II RTOS and requires a STDOUT
 * device in your system's hardware.
 * The memory footprint of this hosted application is ~69 kbytes by default
 * using the standard reference design.
 *
 * For a reduced footprint version of this template, and an explanation of how
 * to reduce the memory footprint for a given application, see the
 * "small_hello_world" template.
 *
 */

#include <stdio.h>
#include "tftlcd.h"

int main()
{
	//LCD��ʼ��
	LCD_init();

	//��Ļ���
	LCD_Clear(0xffff);
	LCD_Clear(0x1458);

	int i,j;

	for (i=0; i< 100; i++){
		for(j=0;  j< 100; j++){
			LCD_Fast_DrawPoint(i,j, 0x9784);
		}
	}
	for (i=1; i< 100; i++){
		for(j=1;  j< 100; j++){
			LCD_Fast_DrawPoint(i,j, 0x8877);
		}
	}

	alt_u16 colorTable[50*50];
	for ( i = 0; i < 50; i ++ )
	{
		for ( j = 0; j < 50; j ++ )
		{
			colorTable[i*50+j] = 0x4577;
		}
	}
	printf( "haha1\n" );

	LCD_Color_Fill(151,151,200,200,colorTable);
//	LCD_Fill(150,150,200,200,0x4987);

	printf( "haha2\n" );

	return 0;
}

