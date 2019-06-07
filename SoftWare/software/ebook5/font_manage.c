/*
 * font_manage.c
 *
 *  Created on: 2019��4��28��
 *      Author: 12757
 */

#include "font_manage.h"

extern const alt_u8 ascii_font_x8y16[];

const alt_u8 *getAsciiFont( char asciiVal )
{
	if ( asciiVal < 0 )
	{
		return (void*)0;
	}
	//ASCII�뷶Χ��0~127
	if ( asciiVal < 0x20 )
    {
        return (void*)0;
    }
    //ASCII�뷶Χ��32~127
	return ascii_font_x8y16 + ((((int)asciiVal)-0x20)*ASCII_LETTER_BYTES);
}

extern const alt_u8 gbk_font_x16y16[];

const alt_u8 *getGbkFont( alt_u8 gbkHighByte, alt_u8 gbkLowByte )
{

	if ( gbkLowByte < 0x40 )//��GBK����
	{
		return (void*)0;
	}
	else
	{
		if ( gbkHighByte < 0x81 || gbkHighByte == 0xFF )//��GBK����
        {
            return (void*)0;
        }
		int wordPos = ( gbkHighByte - 0x81 ) * 191;

		if ( gbkLowByte < 0x7F )
		{
			wordPos = wordPos + gbkLowByte - 0x40;
			return gbk_font_x16y16 + (wordPos*GBK_WORD_BYTES);
		}
		else if ( gbkLowByte > 0x7F && gbkLowByte < 0xFF )
		{
//			wordPos = wordPos + gbkLowByte - 0x41;
			wordPos = wordPos + gbkLowByte - 0x40;
			return gbk_font_x16y16 + (wordPos*GBK_WORD_BYTES);
		}
		else//��GBK����
		{
			return (void*)0;
		}
	}
}

extern const alt_u8 ucs2_font_x16y16[];

//�õ�UCS2�����ַ��ĵ�ַ
const alt_u8 *getUCS2Font( alt_u8 ucs2HighByte, alt_u8 ucs2LowByte )
{
	int index = ( ucs2HighByte << 8 );
	index = index + ucs2LowByte;
	return ucs2_font_x16y16 + index * UCS2_WORD_BYTES;
}
