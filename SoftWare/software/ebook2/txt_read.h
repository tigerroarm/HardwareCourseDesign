/*
 * txt_read.h
 *
 * ��ȡSD��������txt�ļ�������¼ÿ��txt�ļ����Ķ�����
 *
 *  Created on: 2019��4��27��
 *      Author: 12757
 */

#ifndef TXT_READ_H_
#define TXT_READ_H_

#include "screen_type.h"
#include "sdread.h"

//txt�ļ�����󳤶�(��ȡSD���ļ���ʱʹ��)
#define TXT_FILE_NAME_SIZE 51
//����ȡSD���е�99���ļ�
#define TXT_FILES_NUM_MAX 99
#define SAFE_STR_LEN BOOK_COL_NUM*(BOOK_ROW_BYTES+2)


//SD����Ŀ¼������txt�ļ�����Ϣ
typedef struct
{
	TxtFile txtFileList[TXT_FILES_NUM_MAX];//��̬���飬��СΪTXT_FILES_NUM_MAX
	int txtFilesNum;//���ֵΪ99��������ȡSD���е�99���ļ�
	int curOpenFileIndex;//��ǰ�򿪵�txt�ļ��±�
	short int curFileHandle;
} TxtFilesInfo;

extern TxtFilesInfo txtFilesInfoSpace;
extern char txtBookSpace[][(BOOK_ROW_BYTES+1)*2];




//��ȡSD����Ŀ¼�µ�����.txt�ļ����ļ����洢ʱȥ��.txt��
//��ȡ��������ļ���Ϣ�洢������txtFilesInfoSpace��
bool readAlltxtFilesOfSDcard( );

//��ҳģʽö������
//enum bookTurnPageType { BOOK_PAGE_SET=0, BOOK_PAGE_END=1, BOOK_PAGE_NEXT=2, BOOK_PAGE_PRE=3, BOOK_PAGE_JUMP=4 };
enum bookTurnPageType { BOOK_PAGE_NEXT=0, BOOK_PAGE_PRE=1, BOOK_PAGE_JUMP=2 };

//��ȡSD���е��ı�����(txtBookInst�����е�ǰ�򿪵�������Ķ�����)
bool readBookOfTxtFile( TxtFile *txtBookInst,  enum bookTurnPageType turnPageSel, float jumpPageRatio );


//��SD����ĳ���ı�(flieIndex��0��ʼ)
TxtFile *openTxtFile( short fileIndex );

//�������һҳ, ��ʼλ��ָByte���������������ǵڼ���Byte
bool readPageGBK( TxtFile *txtBookInst,  short file_handle, unsigned int startpos, unsigned int* pagesize );

bool readPageUNI( TxtFile *txtBookInst,  short file_handle, unsigned int startpos, unsigned int* pagesize );

bool readLastPage( TxtFile *txtBookInst,  short file_handle );

bool readNextPage( TxtFile *txtBookInst,  short file_handle );

bool readPageByRatio( TxtFile *txtBookInst,  short file_handle, float position );

//�ж��ַ��Ƿ�ΪӢ����ĸ
bool isEnglishletter( char ch);

bool isUNIEnglishletter( char ch1, char ch2);



#endif /* TXTSD_H_ */