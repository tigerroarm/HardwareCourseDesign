/*
 * lcdPinTest.c
 *
 *  Created on: 2019��5��28��
 *      Author: 12757
 */

#include <stdio.h>
#include "system.h"
#include "altera_avalon_pio_regs.h"
#include "alt_types.h"

int main( void )
{
//	alt_u16 lcd_data = 0x0000;
	alt_u16 lcd_data = 0xa0aa;
	printf( "lcd_data = %04x\n", lcd_data );

	IOWR_ALTERA_AVALON_PIO_DIRECTION(TFTLCD_DATA_BASE , 0xffff); //data����Ϊ���

	IOWR_ALTERA_AVALON_PIO_DATA(TFTLCD_DATA_BASE, lcd_data); // д����


	return 0;
}