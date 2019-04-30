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


//txt�ļ�����󳤶�(��ȡSD���ļ���ʱʹ��)
#define TXT_FILE_NAME_SIZE 51


typedef struct
{
	TextType txtFileName;//�ļ�����ʡ��.txt��
	int curPageNum;//��ǰ�Ķ�ҳ��
	int totalPageNum;//��ҳ��
	float curPagePercent;//�Ķ����Ȱٷ���(%)
	unsigned int curBytePos;
	unsigned int fileSizeInBytes;

} TxtFile;

//SD����Ŀ¼������txt�ļ�����Ϣ
typedef struct
{
	TxtFile *txtFileList;//��̬����
	int txtFilesNum;
	int curOpenFileIndex;//��ǰ�򿪵�txt�ļ��±�
} TxtFilesInfo;

extern TxtFilesInfo txtFilesInfoSpace;
extern char txtBookSpace[][BOOK_ROW_BYTES+1];


#endif /* TXTSD_H_ */