/*
 * font_manage.h
 *
 *  Created on: 2019��4��28��
 *      Author: 12757
 */

#ifndef FONT_MANAGE_H_
#define FONT_MANAGE_H_


typedef unsigned char alt_u8;

//ASCII���ֿ���GBK�����ֿ�߶ȱ������
#define OBJ_Y_SIZE 16

//ASCII�ַ��ߴ磺8*16
//�����С����8*16��/8*128
#define ASCII_X_SIZE 8
#define ASCII_Y_SIZE OBJ_Y_SIZE
#define ASCII_LETTER_BYTE_X 1 //(ASCII_X_SIZE/8=1)
#define ASCII_LETTER_BYTE_Y ASCII_Y_SIZE
#define ASCII_LETTER_BYTES (ASCII_LETTER_BYTE_X*ASCII_LETTER_BYTE_Y)//1����ĸ���ֽ���
#define ASCII_X8_Y16_ARRAY_SIZE ( ASCII_LETTER_BYTES * 128 )

//�õ�ASCII�ַ��ĵ�ַ
const alt_u8 *getAsciiFont( char asciiVal );

//GBK�ַ��ߴ磺16*16
#define GBK_X_SIZE 16
#define GBK_Y_SIZE OBJ_Y_SIZE
#define GBK_WORD_BYTE_X 2 //(GBK_X_SIZE/8=2)
#define GBK_WORD_BYTE_Y GBK_Y_SIZE
#define GBK_WORD_BYTES (GBK_WORD_BYTE_X*GBK_WORD_BYTE_Y)//1�����ֵ��ֽ���
#define GBK_X16_Y16_ARRAY_SIZE ( GBK_WORD_BYTES * 24066 )

//�õ�GBK�����ַ��ĵ�ַ
const alt_u8 *getGbkFont( alt_u8 gbkHighByte, alt_u8 gbkLowByte );

//UCS2�ַ��ߴ�
#define UCS2_X_SIZE 16
#define UCS2_Y_SIZE OBJ_Y_SIZE
#define UCS2_WORD_BYTE_X 2 //(UCS2_X_SIZE/8=2)
#define UCS2_WORD_BYTE_Y UCS2_Y_SIZE
#define UCS2_WORD_BYTES (UCS2_WORD_BYTE_X*UCS2_WORD_BYTE_Y)//1�����ֵ��ֽ���
#define UCS2_X16_Y16_ARRAY_SIZE ( UCS2_WORD_BYTES * 65536 )

//�õ�UCS2�����ַ��ĵ�ַ
const alt_u8 *getUCS2Font( alt_u8 ucs2HighByte, alt_u8 ucs2LowByte );

//����icon�ĳߴ綨��

//setting����ͼ��
#define ICON_SETTING_X_SIZE 16
#define ICON_SETTING_Y_SIZE 16
#define ICON_SETTING_BYTE_X 2 // (ICON_SETTING_X_SIZE/8 = 2)
#define ICON_SETTING_BYTE_Y ICON_SETTING_Y_SIZE
#define ICON_SETTING_BYTES (ICON_SETTING_BYTE_X * ICON_SETTING_BYTE_Y)


//home����ͼ��
#define ICON_HOME_X_SIZE 16
#define ICON_HOME_Y_SIZE 16
#define ICON_HOME_BYTE_X 2 // (ICON_HOME_X_SIZE/8 = 2)
#define ICON_HOME_BYTE_Y ICON_HOME_Y_SIZE
#define ICON_HOME_BYTES (ICON_HOME_BYTE_X * ICON_HOME_BYTE_Y)


//back����ͼ��
#define ICON_BACK_X_SIZE 16
#define ICON_BACK_Y_SIZE 16
#define ICON_BACK_BYTE_X 2 // (ICON_BACK_X_SIZE/8 = 2)
#define ICON_BACK_BYTE_Y ICON_BACK_Y_SIZE
#define ICON_BACK_BYTES (ICON_BACK_BYTE_X * ICON_BACK_BYTE_Y)


//plus����ͼ��
#define ICON_PLUS_X_SIZE 8
#define ICON_PLUS_Y_SIZE 8
#define ICON_PLUS_BYTE_X 1 // (ICON_PLUS_X_SIZE/8 = 2)
#define ICON_PLUS_BYTE_Y ICON_PLUS_Y_SIZE
#define ICON_PLUS_BYTES (ICON_PLUS_BYTE_X * ICON_PLUS_BYTE_Y)

//minus����ͼ��
#define ICON_MINUS_X_SIZE 8
#define ICON_MINUS_Y_SIZE 8
#define ICON_MINUS_BYTE_X 1 // (ICON_MINUS_X_SIZE/8 = 2)
#define ICON_MINUS_BYTE_Y ICON_MINUS_Y_SIZE
#define ICON_MINUS_BYTES (ICON_MINUS_BYTE_X * ICON_MINUS_BYTE_Y)


//�������Ľ��ȵ�ͼ��
#define ICON_DOT_X_SIZE 12
#define ICON_DOT_Y_SIZE 12
#define ICON_DOT_BYTE_X 2 // (ICON_DOT_X_SIZE/8 = 2)
#define ICON_DOT_BYTE_Y ICON_DOT_Y_SIZE
#define ICON_DOT_BYTES (ICON_DOT_BYTE_X * ICON_DOT_BYTE_Y)


//���Ƽ���ͼ��
#define ICON_MOVE_UP_X_SIZE 12
#define ICON_MOVE_UP_Y_SIZE 8
#define ICON_MOVE_UP_BYTE_X 2 // (ICON_MOVE_UP_X_SIZE/8 = 2)
#define ICON_MOVE_UP_BYTE_Y ICON_MOVE_UP_Y_SIZE
#define ICON_MOVE_UP_BYTES (ICON_MOVE_UP_BYTE_X * ICON_MOVE_UP_BYTE_Y)

//���Ƽ�ͼ��
#define ICON_MOVE_DOWN_X_SIZE 12
#define ICON_MOVE_DOWN_Y_SIZE 8
#define ICON_MOVE_DOWN_BYTE_X 2 // (ICON_MOVE_DOWN_X_SIZE/8 = 2)
#define ICON_MOVE_DOWN_BYTE_Y ICON_MOVE_DOWN_Y_SIZE
#define ICON_MOVE_DOWN_BYTES (ICON_MOVE_DOWN_BYTE_X * ICON_MOVE_DOWN_BYTE_Y)

#endif /* FONT_MANAGE_H_ */