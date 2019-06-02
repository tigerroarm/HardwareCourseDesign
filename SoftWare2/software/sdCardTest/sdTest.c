/*
 * sdTest.c
 *
 *  Created on: 2019��5��17��
 *      Author: 12757
 */

#include <stdio.h>
#include <stdlib.h>
#include "Altera_UP_SD_Card_Avalon_Interface.h"
#include "system.h"

int main( void )
{

	//SD���豸��Ϣ��ʼ��
	alt_up_sd_card_dev* sdDevPtr =\
			alt_up_sd_card_open_dev( ALTERA_UP_SD_CARD_AVALON_INTERFACE_0_NAME );

	//���SD���Ƿ��ڰ����ϲ���
	bool sdPresent = alt_up_sd_card_is_Present();
	printf( "sdPresent=%s\n", sdPresent ? "true":"false" );

	//���SD���Ƿ�ΪFAT16�ļ�ϵͳ
	bool sdFat16 = alt_up_sd_card_is_FAT16();
	printf( "sdFat16=%s\n", sdFat16 ? "true":"false" );


	//��ȡ��Ŀ¼�������ļ�
	//��ȡ��Ŀ¼�µ�һ��Ŀ¼
	int curFileIndex = 1;
	char rootDir[2] = { '.', 0 };
//	char rootDir[10] = { "lala/." };
	char fileNameStore[100];
	short catalogStatus;
	catalogStatus = alt_up_sd_card_find_first( rootDir, fileNameStore );

	if ( catalogStatus == 0 )
	{
		printf( "file[%d]:%s\n", curFileIndex, fileNameStore );
		curFileIndex ++;
	}
	else
	{
		printf( "rootDir == \" %s \" is valid\n", rootDir );
		return 0;
	}

	//��ȡ֮����ļ�
	while( catalogStatus == 0 )
	{
		catalogStatus = alt_up_sd_card_find_next( fileNameStore );
		if ( catalogStatus == 0 )
		{
			printf( "file[%d]:%s\n", curFileIndex, fileNameStore );
			curFileIndex++;
		}
		else if ( catalogStatus == -1 )
		{
			printf( "end of files\n" );
			break;
		}
	}

	return 0;
}