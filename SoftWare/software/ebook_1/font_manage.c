/*
 * font_manage.c
 *
 *  Created on: 2019��4��28��
 *      Author: 12757
 */

#include "font_manage.h"

extern const alt_u8 ascii_font_x8y16[];

alt_u8 *getAsciiFont( char asciiVal )
{
	if ( asciiVal < 0 )
	{
		return (void*)0;
	}
	//ASCII�뷶Χ��0~127
	return ascii_font_x8y16 + (((int)asciiVal)*ASCII_LETTER_BYTES);
}

extern const alt_u8 gbk_font_x16y16[];

alt_u8 *getGbkFont( alt_u8 gbkHighByte, alt_u8 gbkLowByte )
{

	if ( gbkLowByte < 0x40 )//��GBK����
	{
		return (void*)0;
	}
	else
	{
		int wordPos = ( gbkHighByte - 0x81 ) * 190L;
		if ( gbkLowByte < 0x7F )
		{
			wordPos = wordPos + gbkLowByte - 0x40;
			return gbk_font_x16y16 + (wordPos*GBK_WORD_BYTES);
		}
		else if ( gbkLowByte > 0x7F && gbkLowByte < 0xFF )
		{
			wordPos = wordPos + gbkLowByte - 0x41;
			return gbk_font_x16y16 + (wordPos*GBK_WORD_BYTES);
		}
		else
		{
			return (void*)0;
		}
	}
}