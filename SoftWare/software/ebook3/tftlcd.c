/*
 * tftflcd.c
 *
 *  Created on: 2019��5��28��
 *      Author: 12757
 */


#include "tftlcd.h"
#include "system.h"

//��ʾ������

TftLcdInfo lcddev;


//����ֵ:����������
alt_u16 LCD_ReadReg(alt_u16 LCD_Reg)
{
	LCD_WR_REG(LCD_Reg);		//д��Ҫ���ļĴ������
	delay(1000);
	return LCD_RD_DATA();		//���ض�����ֵ
}

void LCD_init()
{
	usleep(100000);
    alt_u8 data = 0x4 | 0x0 | 0x1;
    IOWR_ALTERA_AVALON_PIO_DATA(TFTLCD_BASE_CTRL_BASE, data);
    usleep(10000);
    data = 0x4 | 0x2 | 0x0;
    IOWR_ALTERA_AVALON_PIO_DATA(TFTLCD_BASE_CTRL_BASE, data);
    usleep(10000);


	IOWR_ALTERA_AVALON_PIO_DIRECTION(TFTLCD_BASE_CTRL_BASE, 0xffff); //Ĭ������out
	IOWR_ALTERA_AVALON_PIO_DIRECTION(TFTLCD_CMD_BASE, 0xffff); //Ĭ������out
	IOWR_ALTERA_AVALON_PIO_DIRECTION(TFTLCD_DATA_BASE , 0xffff); //Ĭ������out
	IOWR_ALTERA_AVALON_PIO_DATA(TFTLCD_CMD_BASE, 0x4|0x2|0x1); //дʹ�� ��д����

	usleep(50000);

	lcddev.id=LCD_ReadReg(0x0000);
	LCD_WR_REG(0xD4);
	lcddev.id=LCD_RD_DATA();	//dummy read
	lcddev.id=LCD_RD_DATA();	//����0X00
	lcddev.id=LCD_RD_DATA();   	//��ȡ93
	lcddev.id<<=8;
	lcddev.id|=LCD_RD_DATA();  	//��ȡ41
	printf("id =  %x \n",lcddev.id);

	if ( lcddev.id == 0x5310 )
	{
		LCD5310_init();
	}

	LCD_Display_Dir(0);		 	//Ĭ��Ϊ����
}

//������
alt_u16 LCD_RD_DATA()
{
#if DELAY_SWITCH == 1
	delay( DELAY_NUM );
#endif
	IOWR_ALTERA_AVALON_PIO_DIRECTION(TFTLCD_DATA_BASE, 0x0000); //data����Ϊ����
	IOWR_ALTERA_AVALON_PIO_DATA(TFTLCD_CMD_BASE, 0x0|0x2|0x1); //��ʹ�� �����͵�ƽ ������
	alt_u16 data =  IORD_ALTERA_AVALON_PIO_DATA(TFTLCD_DATA_BASE); //��������
	IOWR_ALTERA_AVALON_PIO_DATA(TFTLCD_CMD_BASE, 0x4|0x2|0x1); //��ʹ��  ������
	return data;
}

//дLCD����
void LCD_WR_REG(alt_u16 cmd)
{
#if DELAY_SWITCH == 1
	delay( DELAY_NUM );
#endif
	IOWR_ALTERA_AVALON_PIO_DIRECTION(TFTLCD_DATA_BASE, 0xffff); //data����Ϊ���
	// дRS
	IOWR_ALTERA_AVALON_PIO_DATA(TFTLCD_CMD_BASE, 0x4|0x0|0x0); //дʹ�� �����͵�ƽ д����
	IOWR_ALTERA_AVALON_PIO_DATA(TFTLCD_DATA_BASE, cmd); // д����
	IOWR_ALTERA_AVALON_PIO_DATA(TFTLCD_CMD_BASE, 0x4|0x0|0x1); //�����ߵ�ƽ������
}

void LCD_WR_DATA(alt_u16 data)
{
#if DELAY_SWITCH == 1
	delay( DELAY_NUM );
#endif
	IOWR_ALTERA_AVALON_PIO_DIRECTION(TFTLCD_DATA_BASE , 0xffff); //data����Ϊ���
	IOWR_ALTERA_AVALON_PIO_DATA(TFTLCD_CMD_BASE,0x4|0x2|0x0); //дʹ�� �����͵�ƽ д����
	IOWR_ALTERA_AVALON_PIO_DATA(TFTLCD_DATA_BASE, data); // д����
	IOWR_ALTERA_AVALON_PIO_DATA(TFTLCD_CMD_BASE, 0x4|0x2|0x1); //�����ߵ�ƽ������
}
//д�Ĵ���
//LCD_Reg:�Ĵ������
//LCD_RegValue:Ҫд���ֵ
void LCD_WriteReg(alt_u16 LCD_Reg,alt_u16 LCD_RegValue)
{
	LCD_WR_REG(LCD_Reg);
	LCD_WR_DATA(LCD_RegValue);
}

void LCD5310_init()
{
	LCD_WR_REG(0xED);
	LCD_WR_DATA(0x01);
	LCD_WR_DATA(0xFE);

	LCD_WR_REG(0xEE);
	LCD_WR_DATA(0xDE);
	LCD_WR_DATA(0x21);

	LCD_WR_REG(0xF1);
	LCD_WR_DATA(0x01);
	LCD_WR_REG(0xDF);
	LCD_WR_DATA(0x10);

	//VCOMvoltage//
	LCD_WR_REG(0xC4);
	LCD_WR_DATA(0x8F);	  //5f

	LCD_WR_REG(0xC6);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xE2);
	LCD_WR_DATA(0xE2);
	LCD_WR_DATA(0xE2);
	LCD_WR_REG(0xBF);
	LCD_WR_DATA(0xAA);

	LCD_WR_REG(0xB0);
	LCD_WR_DATA(0x0D);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x0D);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x11);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x19);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x21);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x2D);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x3D);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x5D);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x5D);
	LCD_WR_DATA(0x00);

	LCD_WR_REG(0xB1);
	LCD_WR_DATA(0x80);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x8B);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x96);
	LCD_WR_DATA(0x00);

	LCD_WR_REG(0xB2);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x02);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x03);
	LCD_WR_DATA(0x00);

	LCD_WR_REG(0xB3);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);

	LCD_WR_REG(0xB4);
	LCD_WR_DATA(0x8B);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x96);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xA1);
	LCD_WR_DATA(0x00);

	LCD_WR_REG(0xB5);
	LCD_WR_DATA(0x02);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x03);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x04);
	LCD_WR_DATA(0x00);

	LCD_WR_REG(0xB6);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);

	LCD_WR_REG(0xB7);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x3F);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x5E);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x64);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x8C);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xAC);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xDC);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x70);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x90);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xEB);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xDC);
	LCD_WR_DATA(0x00);

	LCD_WR_REG(0xB8);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);

	LCD_WR_REG(0xBA);
	LCD_WR_DATA(0x24);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);

	LCD_WR_REG(0xC1);
	LCD_WR_DATA(0x20);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x54);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xFF);
	LCD_WR_DATA(0x00);

	LCD_WR_REG(0xC2);
	LCD_WR_DATA(0x0A);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x04);
	LCD_WR_DATA(0x00);

	LCD_WR_REG(0xC3);
	LCD_WR_DATA(0x3C);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x3A);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x39);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x37);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x3C);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x36);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x32);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x2F);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x2C);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x29);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x26);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x24);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x24);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x23);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x3C);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x36);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x32);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x2F);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x2C);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x29);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x26);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x24);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x24);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x23);
	LCD_WR_DATA(0x00);

	LCD_WR_REG(0xC4);
	LCD_WR_DATA(0x62);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x05);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x84);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xF0);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x18);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xA4);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x18);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x50);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x0C);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x17);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x95);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xF3);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xE6);
	LCD_WR_DATA(0x00);

	LCD_WR_REG(0xC5);
	LCD_WR_DATA(0x32);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x44);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x65);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x76);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x88);
	LCD_WR_DATA(0x00);

	LCD_WR_REG(0xC6);
	LCD_WR_DATA(0x20);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x17);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x01);
	LCD_WR_DATA(0x00);

	LCD_WR_REG(0xC7);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);

	LCD_WR_REG(0xC8);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);

	LCD_WR_REG(0xC9);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);

	LCD_WR_REG(0xE0);
	LCD_WR_DATA(0x16);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x1C);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x21);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x36);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x46);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x52);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x64);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x7A);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x8B);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x99);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xA8);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xB9);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xC4);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xCA);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xD2);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xD9);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xE0);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xF3);
	LCD_WR_DATA(0x00);

	LCD_WR_REG(0xE1);
	LCD_WR_DATA(0x16);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x1C);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x22);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x36);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x45);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x52);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x64);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x7A);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x8B);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x99);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xA8);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xB9);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xC4);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xCA);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xD2);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xD8);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xE0);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xF3);
	LCD_WR_DATA(0x00);

	LCD_WR_REG(0xE2);
	LCD_WR_DATA(0x05);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x0B);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x1B);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x34);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x44);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x4F);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x61);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x79);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x88);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x97);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xA6);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xB7);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xC2);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xC7);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xD1);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xD6);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xDD);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xF3);
	LCD_WR_DATA(0x00);
	LCD_WR_REG(0xE3);
	LCD_WR_DATA(0x05);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xA);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x1C);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x33);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x44);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x50);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x62);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x78);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x88);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x97);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xA6);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xB7);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xC2);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xC7);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xD1);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xD5);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xDD);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xF3);
	LCD_WR_DATA(0x00);

	LCD_WR_REG(0xE4);
	LCD_WR_DATA(0x01);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x01);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x02);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x2A);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x3C);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x4B);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x5D);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x74);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x84);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x93);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xA2);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xB3);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xBE);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xC4);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xCD);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xD3);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xDD);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xF3);
	LCD_WR_DATA(0x00);
	LCD_WR_REG(0xE5);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x02);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x29);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x3C);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x4B);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x5D);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x74);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x84);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x93);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xA2);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xB3);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xBE);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xC4);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xCD);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xD3);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xDC);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xF3);
	LCD_WR_DATA(0x00);

	LCD_WR_REG(0xE6);
	LCD_WR_DATA(0x11);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x34);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x56);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x76);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x77);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x66);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x88);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x99);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xBB);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x99);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x66);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x55);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x55);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x45);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x43);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x44);
	LCD_WR_DATA(0x00);

	LCD_WR_REG(0xE7);
	LCD_WR_DATA(0x32);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x55);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x76);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x66);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x67);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x67);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x87);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x99);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xBB);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x99);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x77);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x44);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x56);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x23);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x33);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x45);
	LCD_WR_DATA(0x00);

	LCD_WR_REG(0xE8);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x99);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x87);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x88);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x77);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x66);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x88);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xAA);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xBB);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x99);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x66);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x55);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x55);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x44);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x44);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x55);
	LCD_WR_DATA(0x00);

	LCD_WR_REG(0xE9);
	LCD_WR_DATA(0xAA);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);

	LCD_WR_REG(0x00);
	LCD_WR_DATA(0xAA);

	LCD_WR_REG(0xCF);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);

	LCD_WR_REG(0xF0);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x50);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);

	LCD_WR_REG(0xF3);
	LCD_WR_DATA(0x00);

	LCD_WR_REG(0xF9);
	LCD_WR_DATA(0x06);
	LCD_WR_DATA(0x10);
	LCD_WR_DATA(0x29);
	LCD_WR_DATA(0x00);

	LCD_WR_REG(0x3A);
	LCD_WR_DATA(0x55);	//66

	LCD_WR_REG(0x11);
	usleep(100000);
	LCD_WR_REG(0x29);
	LCD_WR_REG(0x35);
	LCD_WR_DATA(0x00);

	LCD_WR_REG(0x51);
	LCD_WR_DATA(0xFF);
	LCD_WR_REG(0x53);
	LCD_WR_DATA(0x2C);
	LCD_WR_REG(0x55);
	LCD_WR_DATA(0x82);
	LCD_WR_REG(0x2c);
}

//���ù��λ��
//Xpos:������
//Ypos:������
void LCD_SetCursor(alt_u16 Xpos, alt_u16 Ypos)
{
	LCD_WR_REG(lcddev.setxcmd);
	LCD_WR_DATA(Xpos>>8);LCD_WR_DATA(Xpos&0XFF);
	LCD_WR_REG(lcddev.setycmd);
	LCD_WR_DATA(Ypos>>8);LCD_WR_DATA(Ypos&0XFF);
}
//����
//x,y:����
//POINT_COLOR:�˵����ɫ
void LCD_DrawPoint(alt_u16 x,alt_u16 y,alt_u16 color)
{
	LCD_SetCursor(x,y);		//���ù��λ��
	LCD_WriteRAM_Prepare();	//��ʼд��GRAM
	LCD_WR_DATA(color);
}

void LCD_Fast_DrawPoint(alt_u16 x,alt_u16 y,alt_u16 color)
{
	LCD_WR_REG(lcddev.setxcmd);
	LCD_WR_DATA(x>>8);
	LCD_WR_DATA(x&0XFF);
	LCD_WR_REG(lcddev.setycmd);
	LCD_WR_DATA(y>>8);
	LCD_WR_DATA(y&0XFF);

	LCD_WriteRAM_Prepare();	//��ʼд��GRAM
	LCD_WR_DATA(color);
}

//��ʼдGRAM
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.wramcmd);
}
//LCDдGRAM
//RGB_Code:��ɫֵ
void LCD_WriteRAM(alt_u16 RGB_Code)
{
	LCD_WR_DATA(RGB_Code);//дʮ��λGRAM
}
//����
//x1,y1:�������
//x2,y2:�յ�����
void LCD_DrawLine(alt_u16 x1, alt_u16 y1, alt_u16 x2, alt_u16 y2, alt_u16 color)
{
	alt_u16 t;
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x2-x1; //������������
	delta_y=y2-y1;
	uRow=x1;
	uCol=y1;
	if(delta_x>0)incx=1; //���õ�������
	else if(delta_x==0)incx=0;//��ֱ��
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if(delta_y==0)incy=0;//ˮƽ��
	else{incy=-1;delta_y=-delta_y;}
	if( delta_x>delta_y)distance=delta_x; //ѡȡ��������������
	else distance=delta_y;
	for(t=0;t<=distance+1;t++ )//�������
	{
		LCD_DrawPoint(uRow,uCol, color);//����
		xerr+=delta_x ;
		yerr+=delta_y ;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}

//��������
void LCD_Display_Dir(alt_u8 dir)
{
	if(lcddev.id==0X5310)
	{
		if(dir==0)			//����
		{
			lcddev.dir=0;	//����
			lcddev.width=320;
			lcddev.height=480;

			lcddev.wramcmd=0X2C;
			lcddev.setxcmd=0X2A;
			lcddev.setycmd=0X2B;
		}
		else 				//����
		{
			lcddev.dir=1;	//����
			lcddev.width=480;
			lcddev.height=320;

			lcddev.wramcmd=0X2C;
			lcddev.setxcmd=0X2A;
			lcddev.setycmd=0X2B;
		}
	}
	LCD_Scan_Dir(DFT_SCAN_DIR);	//Ĭ��ɨ�跽��
}
//��ָ�����������ָ����ɫ
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)
//color:Ҫ������ɫ
void LCD_Fill(alt_u16 sx,alt_u16 sy,alt_u16 ex,alt_u16 ey,alt_u16 color)//��䵥ɫ
{
	alt_u16 i,j;
	alt_u16 xlen=0;

	xlen=ex-sx+1;
	for(i=sy;i<=ey;i++)
	{
		LCD_SetCursor(sx,i);      				//���ù��λ��
		LCD_WriteRAM_Prepare();     			//��ʼд��GRAM
		for(j=0;j<xlen;j++)
		{
			LCD_WR_DATA(color);
		}
//		printf( "sx,i = %d, %d\n", sx, i );
	}
}
//��ָ�����������ָ����ɫ��
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)
//color:Ҫ������ɫ
void LCD_Color_Fill(alt_u16 sx,alt_u16 sy,alt_u16 ex,alt_u16 ey,alt_u16 *color)
{
	alt_u16 height,width;
	alt_u16 i,j;
	width=ex-sx+1; 			//�õ����Ŀ���
	height=ey-sy+1;			//�߶�
 	for(i=0;i<height;i++)
	{
 		LCD_SetCursor(sx,sy+i);   	//���ù��λ��
		LCD_WriteRAM_Prepare();     //��ʼд��GRAM
		for(j=0;j<width;j++)
		{
			LCD_WR_DATA(color[i*width+j]);//д������
		}
//		printf( "x,y = %d, %d\n", sx, sy+i );
	}
}


void delay(int time){
	int i;
	for(i=0; i<time; i++);
}

void LCD_Scan_Dir(alt_u8 dir)
{
	alt_u16 regval=0;
	alt_u16 dirreg=0;
	alt_u16 temp;
	if(lcddev.dir==1)//��ĻΪ����
	{
		switch(dir)//dir����ת��
		{
			case 0:dir=6;break;
			case 1:dir=7;break;
			case 2:dir=4;break;
			case 3:dir=5;break;
			case 4:dir=1;break;
			case 5:dir=0;break;
			case 6:dir=3;break;
			case 7:dir=2;break;
		}
	}
	if(lcddev.id==0X5310)//5310���⴦��
	{
		switch(dir)
		{
			case L2R_U2D://������,���ϵ���
				regval|=(0<<7)|(0<<6)|(0<<5);
				break;
			case L2R_D2U://������,���µ���
				regval|=(1<<7)|(0<<6)|(0<<5);
				break;
			case R2L_U2D://���ҵ���,���ϵ���
				regval|=(0<<7)|(1<<6)|(0<<5);
				break;
			case R2L_D2U://���ҵ���,���µ���
				regval|=(1<<7)|(1<<6)|(0<<5);
				break;
			case U2D_L2R://���ϵ���,������
				regval|=(0<<7)|(0<<6)|(1<<5);
				break;
			case U2D_R2L://���ϵ���,���ҵ���
				regval|=(0<<7)|(1<<6)|(1<<5);
				break;
			case D2U_L2R://���µ���,������
				regval|=(1<<7)|(0<<6)|(1<<5);
				break;
			case D2U_R2L://���µ���,���ҵ���
				regval|=(1<<7)|(1<<6)|(1<<5);
				break;
		}

		dirreg=0X36;

		LCD_WriteReg(dirreg,regval);

		if(regval&0X20)
		{
			if(lcddev.width<lcddev.height)//����X,Y
			{
				temp=lcddev.width;
				lcddev.width=lcddev.height;
				lcddev.height=temp;
			}
		}
		else
		{
			if(lcddev.width>lcddev.height)//����X,Y
			{
				temp=lcddev.width;
				lcddev.width=lcddev.height;
				lcddev.height=temp;
			}
		}

		LCD_WR_REG(lcddev.setxcmd);
		LCD_WR_DATA(0);LCD_WR_DATA(0);
		LCD_WR_DATA((lcddev.width-1)>>8);LCD_WR_DATA((lcddev.width-1)&0XFF);
		LCD_WR_REG(lcddev.setycmd);
		LCD_WR_DATA(0);LCD_WR_DATA(0);
		LCD_WR_DATA((lcddev.height-1)>>8);LCD_WR_DATA((lcddev.height-1)&0XFF);
  	}
}

void LCD_Clear(alt_u16 color)
{
	alt_u32 index=0;
	alt_u32 totalpoint=lcddev.width;
	totalpoint*=lcddev.height; 			//�õ��ܵ���
	LCD_SetCursor(0x00,0x0000);	//���ù��λ��
	LCD_WriteRAM_Prepare();     		//��ʼд��GRAM
	for(index=0;index<totalpoint;index++)
	{
		LCD_WR_DATA(color);
	}
}

//���ô���,���Զ����û������굽�������Ͻ�(sx,sy).
//sx,sy:������ʼ����(���Ͻ�)
//width,height:���ڿ��Ⱥ͸߶�,�������0!!
//�����С:width*height.
void LCD_Set_Window(alt_u16 sx,alt_u16 sy,alt_u16 width,alt_u16 height)
{
	alt_u16 twidth,theight;
	twidth=sx+width-1;
	theight=sy+height-1;
	if(lcddev.id==0X5310)
	{
		LCD_WR_REG(lcddev.setxcmd);
		LCD_WR_DATA(sx>>8);
		LCD_WR_DATA(sx&0XFF);
		LCD_WR_DATA(twidth>>8);
		LCD_WR_DATA(twidth&0XFF);
		LCD_WR_REG(lcddev.setycmd);
		LCD_WR_DATA(sy>>8);
		LCD_WR_DATA(sy&0XFF);
		LCD_WR_DATA(theight>>8);
		LCD_WR_DATA(theight&0XFF);
	}
}