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
#include "test_value.h"


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
	//txtFilesInfoSpace.txtFileListָ����Ҫmalloc����
	//((txtFilesInfoSpace.txtFileList)[i]).TextType.text����洢���ַ����ռ�Ҳ��Ҫmalloc

	/*
	 *
	 *
	 * ********************************************�뿪ʼ��ı���
	 *
	 *
	 */


	//����Ϊ���Ի����µĴ��룬�������һ�����ݸ�����txtFilesInfoSpace������������SD����δ��ͨʱ��Ӱ��������������
	//��Ȼ����Զ�ܿ��Բο����´���дSD����ͨʱ��readAlltxtFilesOfSDcard��������

	txtFilesInfoSpace.curOpenFileIndex = 0;//��һ���ı�

	#define TXT_FILES_NUM 25
	int filesNum = TXT_FILES_NUM;
//    int filesNum = 1;
	txtFilesInfoSpace.txtFilesNum = filesNum;//�ļ��ܸ���

	txtFilesInfoSpace.txtFileList = ( TxtFile *)malloc( sizeof(TxtFile) * filesNum );
//    txtFilesInfoSpace.txtFileList = NULL;

//    char tempFilesName[1][40] = { "���������������҅��҅u����" };

	char tempFilesName[TXT_FILES_NUM][40] = {
			"���пƼ���ѧ������Ϣ��ͨ��ѧԺ",
			"���ƾ���",
			"���޴�½",
			"ED2-115����������",
			"�ɽ�������3",
			"Good night!",
			"What are you Ūɶ�ϣ���������������",
			"Leave me alone",
			"�ʤ�Ǥ�ʤ���",
			"������������������",
			"��������ô���ɵ�",
			"Ӳ������",
			"΢����·",
			"Come on, baby",
			"altera",
			"nios 2",
			"�߸���ѩ������һ��С����",
			"���ߵ粨",
			"�˹�����",
			"����ҽ��",
			"����û�����룬GBK���Ǻ�����",
			"����̽����",
			"��Ͳϴ�»�",
			"�����ߵ���������ﲻ�������",
			"final music"
	};

	TxtFile *curTxtFile;
	int i;
	for ( i = 0; i < filesNum; i ++ )
	{
        //����ƫ��
		curTxtFile = (txtFilesInfoSpace.txtFileList) + i;

		//�ļ���Ϣ���
		curTxtFile->curPageBytePosStart = 0;//��ǰ�Ķ���0�ֽ�
		curTxtFile->curPageBytePosEnd = 0;//��ǰ�Ķ���0�ֽ�
		curTxtFile->curPageNum = 1;//��һҳ
		curTxtFile->curPageRatio = 0;//����(��Χ0~1)
		curTxtFile->fileSizeInBytes = 10000;//10000���ֽ�
		curTxtFile->totalPageNum = curTxtFile->fileSizeInBytes / ( BOOK_ROW_BYTES * BOOK_COL_NUM );

		//�ַ�������
		int textLen = strlen(tempFilesName[i]);
		(curTxtFile->txtFileName).textLen = textLen;

		//�ַ����洢�ռ�malloc
        char *tempPtr = (char*)malloc( sizeof(char) * (textLen+1) );
        (curTxtFile->txtFileName).text = tempPtr;

        //�洢�ļ���
        strcpy( tempPtr, tempFilesName[i] );
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