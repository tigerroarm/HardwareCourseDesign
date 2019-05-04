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

typedef struct
{
	TextType txtFileName;//文件名（省略.txt）
	int curPageNum;//当前阅读页数
	int totalPageNum;//总页数
	float curPagePercent;//阅读进度百分数(%)
	unsigned int curBytePos;
	unsigned int fileSizeInBytes;

} TxtFile;

//SD卡根目录下所有txt文件的信息
typedef struct
{
	TxtFile *txtFileList;//动态数组
	int txtFilesNum;//最大值为99，即最多读取SD卡中的99个文件
	int curOpenFileIndex;//当前打开的txt文件下标
} TxtFilesInfo;

extern TxtFilesInfo txtFilesInfoSpace;
extern char txtBookSpace[][BOOK_ROW_BYTES+1];


#endif /* TXTSD_H_ */
