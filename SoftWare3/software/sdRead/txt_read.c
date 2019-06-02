/*
 * txt_read.c
 *
 *  Created on: 2019��4��27��
 *      Author: 12757
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "txt_read.h"
#include "screen.h"


//��ʱ�洢��SD���ж������ļ���
char sdCardFileName[TXT_FILE_NAME_SIZE];

TxtFilesInfo txtFilesInfoSpace;

//char txtBookSpace[BOOK_COL_NUM][BOOK_ROW_BYTES+1] = {"hahaha","lalala","gauguagua"};
//char txtBookSpace[BOOK_COL_NUM][BOOK_ROW_BYTES+1] = { "Ľ���о���","¥��˵�Ķ�",{0},"���ǻ��Ʋ˼�" };
char txtBookSpace[BOOK_COL_NUM][BOOK_ROW_BYTES+1] = {
    "���ݼ��:",
    "    �������ŵ�����������͵ѧ���ž�",
    "ѧΪ���������ݣ�������־ʱȴ������",
    "��һ�����磬һ�������������硣��",
    "�ж��޴�½��",
    "    ����û��ħ����û�ж�����û����",
    "����ȴ���������ꡣ�����ÿ���ˣ�",
    "���Լ������ʱ�򣬶�������������",
    "�����ѡ�����ж����ֲ�����",
    "����ǿ��Ը������ǵ��ճ������",
    "����һЩ�ر��ɫ�����ȴ����������",
    "�������ְҵ���Ƕ��޴�½����Ϊǿ��",
    "Ҳ������Ҫ��ְҵ��������ʦ��",
    "    �����Ű����������޴�½��������",
    "�����ѣ����ܷ�����Ƭ����������",
    "�����ŻԻͣ�",
    "    ְҵ�ȼ�����ʿ����ʦ�����ʦ��",
    "���𡢻��ڡ���������ۡ���ʥ������",
    "����Ŷ��ޡ�",
    "  Please do not split Enlish words",
    "into two lines"
};



//��ȡSD����Ŀ¼�µ�����.txt�ļ�(���99��,�궨��ΪTXT_FILES_NUM_MAX=99)���ļ����洢ʱȥ��.txt��
//��ȡ��������ļ���Ϣ�洢������txtFilesInfoSpace��
bool readAlltxtFilesOfSDcard( )
{
	bool status = true;

	//txtFilesInfoSpace��ȫ�ֱ�������ֱ��ʹ��
	//((txtFilesInfoSpace.txtFileList)[i]).TextType.text����洢���ַ����ռ���Ҫmalloc

	txtFilesInfoSpace.curOpenFileIndex = 0;//��һ���ı�
	txtFilesInfoSpace.curFileHandle = 0;
	txtFilesInfoSpace.txtFilesNum = 0;


	//SD���豸��Ϣ��ʼ��
	alt_up_sd_card_dev* sdDevPtr =\
			alt_up_sd_card_open_dev( ALTERA_UP_SD_CARD_AVALON_INTERFACE_0_NAME );

	//���SD���Ƿ��ڰ����ϲ���
	bool sdPresent = alt_up_sd_card_is_Present();
	printf( "sdPresent=%s\n", sdPresent ? "true":"false" );
	if ( !sdPresent )
	{
		return false;
	}

	//���SD���Ƿ�ΪFAT16�ļ�ϵͳ
	bool sdFat16 = alt_up_sd_card_is_FAT16();
	printf( "sdFat16=%s\n", sdFat16 ? "true":"false" );
	if ( !sdFat16 )
	{
		return false;
	}

	//�жϸ��ļ����ж��ٸ�TXT�ļ�

	//��ȡ��Ŀ¼�������ļ�
	//��ȡ��Ŀ¼�µ�һ��Ŀ¼

	char rootDir[2] = { '.', 0 };
	char fileReadNameStore[13];
	short catalogStatus;
	catalogStatus = alt_up_sd_card_find_first( rootDir, fileReadNameStore );

	if ( catalogStatus == -1 )//��Ŀ¼��û���ļ�
	{
		printf( "No file in root dir\n" );
		return false;
	}
	else if ( catalogStatus == 0 )//�ɹ�������һ���ļ�
	{
		printf( "file[%d]:%s\n", txtFilesInfoSpace.txtFilesNum, fileReadNameStore );
		txtFilesInfoSpace.txtFilesNum = 0;
	}
	else
	{
		printf( "rootDir == \" %s \" is valid\n", rootDir );
		return false;
	}
	int i;
	while( catalogStatus == 0 && txtFilesInfoSpace.txtFilesNum < TXT_FILES_NUM_MAX )
	{
		//���fileReadNameStore
		for ( i = 0; i < 13; i ++ )
		{
			fileReadNameStore[i] = 0;
		}
		catalogStatus = alt_up_sd_card_find_next( fileReadNameStore );
		if ( catalogStatus == 0 )
		{
			//printf( "file[%d]:%s\n", txtFilesInfoSpace.txtFilesNum, fileReadNameStore );
			//���ΪTXT�ļ����ͱ�������

			int dotOccur = 0;
			int dotOccurIndex = 0;
			for ( i = 0; i < 13; i ++ )//Ѱ��'.'
			{
				if ( fileReadNameStore[i] == '.' )
				{
					dotOccur = 1;
					dotOccurIndex = i;
				}
			}
			if ( dotOccur &&
				fileReadNameStore[dotOccurIndex+1] == 'T' &&
				fileReadNameStore[dotOccurIndex+2] == 'X' && \
				fileReadNameStore[dotOccurIndex+3] == 'T' )
			{
				TxtFile *curTxtFile = txtFilesInfoSpace.txtFileList + txtFilesInfoSpace.txtFilesNum;
				txtFilesInfoSpace.txtFilesNum ++;

				curTxtFile->txtFileReadName.textLen = strlen( fileReadNameStore );
				curTxtFile->txtFileReadName.text = malloc( curTxtFile->txtFileReadName.textLen + 1 );
				strcpy( curTxtFile->txtFileReadName.text, fileReadNameStore );

				curTxtFile->txtFileName.textLen = curTxtFile->txtFileReadName.textLen - 4;
				curTxtFile->txtFileName.text = malloc( curTxtFile->txtFileName.textLen + 1 );
				char *tempStr = curTxtFile->txtFileName.text;
				for( i = 0; i < dotOccurIndex; i ++ )
				{
					tempStr[i] = fileReadNameStore[i];
				}
				tempStr[dotOccurIndex] = 0;//������־
			}
		}
		else if ( catalogStatus == -1 )
		{
			printf( "end of files\n" );
			break;
		}
	}

	return status;

}


//��ȡSD���е��ı�����(txtBookInst�����е�ǰ�򿪵�������Ķ�����),�洢��ȫ�ֱ���txtBookSpace��
bool readBookOfTxtFile( TxtFile *txtBookInst,  enum bookTurnPageType turnPageSel, float jumpPageRatio )
{
	bool status = true;

	if ( turnPageSel == BOOK_PAGE_PRE )
    {
        printf( "pageChangePre\n" );
    }
    else if ( turnPageSel == BOOK_PAGE_NEXT )
    {
        printf( "pageChangeNext\n" );
    }
    else if ( turnPageSel == BOOK_PAGE_JUMP )
    {
        printf( "pageJump\n" );
    }

	//���Ҫ���޸�txtBookInst���Ķ����ȣ�����ȡ��ҳ����ı�������txtBookSpace��
	//��ҳʱ����������·�ҳʱ��Ҫ��֤�ı������ԣ���Ҫ��֤���з�����һ�еĿ�ͷ
	//�����ҳ�ף�ҳβ������ҳ��ҳβ�ı�����
	//�����ҳ����ת����ͨ�����Ȱٷ���jumpPagePct����Ҫ��ת��ҳ����Ȼ���ҵ���������Ļ��з������һ����ʾ�����Ҳ������з�,�����
    //Ӣ�ĵ��ʲ��ɲ�������

	//����ı�����Ҫ��֤������BOOK�ı���ʾ����ȣ�BOOK�ı��ַ��ļ��������screen_type.h�в�ѯ��

	//��screen.h������һ������ĺ��������Ի�ȡ�ı��к�������ĸ�ķֲ���(���ض�̬���飬�ǵ�free)
	//�ú���Ϊ:char *getWordLetterArrange( char *str, short len );

	/*
	 *
	 *
	 * ********************************************�뿪ʼ��ı���
	 *
	 *
	 */





	return status;
}

//��SD����ĳ���ı�
TxtFile *openTxtFile( short fileIndex )
{
    if ( fileIndex < 0 || fileIndex >= txtFilesInfoSpace.txtFilesNum )
    {
        return (void*)0;
    }
    txtFilesInfoSpace.curOpenFileIndex = fileIndex;

    //���ô��ļ�����
    /*


    д��

    */

    return txtFilesInfoSpace.txtFileList + fileIndex;
}

//�ر�TxtFile�ʼ�
bool closeTxtFile( )
{
    txtFilesInfoSpace.curOpenFileIndex = -1;

    //���ùر��ļ�����


    /*


    д��

    */
    return true;
}


