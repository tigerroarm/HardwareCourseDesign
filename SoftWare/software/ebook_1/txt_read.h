/*
 * txt_read.h
 *
 * 读取SD卡的所有txt文件，并记录每个txt文件的阅读进度
 *
 *  Created on: 2019年4月27日
 *      Author: 12757
 */

#ifndef TXT_READ_H_
#define TXT_READ_H_

#include "screen_type.h"


//txt文件名最大长度(读取SD卡文件名时使用)
#define TXT_FILE_NAME_SIZE 51
//最多读取SD卡中的99个文件
#define TXT_FILES_NUM_MAX 99



//SD卡根目录下所有txt文件的信息
typedef struct
{
	TxtFile *txtFileList;//动态数组（第一次使用时需要malloc分配空间）
	int txtFilesNum;//最大值为99，即最多读取SD卡中的99个文件
	int curOpenFileIndex;//当前打开的txt文件下标
} TxtFilesInfo;

extern TxtFilesInfo txtFilesInfoSpace;
extern char txtBookSpace[][BOOK_ROW_BYTES+1];




//读取SD卡根目录下的所有.txt文件（文件名存储时去掉.txt）
//读取后的所有文件信息存储到变量txtFilesInfoSpace中
bool readAlltxtFilesOfSDcard( );

//翻页模式枚举类型
enum bookTurnPageType { BOOK_PAGE_SET=0, BOOK_PAGE_END=1, BOOK_PAGE_NEXT=2, BOOK_PAGE_PRE=3, BOOK_PAGE_JUMP=4 };

//读取SD卡中的文本内容(txtBookInst里面有当前打开电子书的阅读进度)
bool readBookOfTxtFile( TxtFile *txtBookInst,  enum bookTurnPageType turnPageSel, float jumpPagePct );


#endif /* TXTSD_H_ */
