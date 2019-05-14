/*
 * txt_read.c
 *
 *  Created on: 2019年4月27日
 *      Author: 12757
 */

#include <stdlib.h>
#include <string.h>
#include "txt_read.h"
#include "screen.h"


//临时存储从SD卡中读出的文件名
char sdCardFileName[TXT_FILE_NAME_SIZE];

TxtFilesInfo txtFilesInfoSpace;

char txtBookSpace[BOOK_COL_NUM][BOOK_ROW_BYTES+1];


//读取SD卡根目录下的所有.txt文件(最多99个,宏定义为TXT_FILES_NUM_MAX=99)（文件名存储时去掉.txt）
//读取后的所有文件信息存储到变量txtFilesInfoSpace中
bool readAlltxtFilesOfSDcard( )
{
	bool status = true;

	//txtFilesInfoSpace是全局变量，可直接使用
	//txtFilesInfoSpace.txtFileList指针需要malloc数组
	//((txtFilesInfoSpace.txtFileList)[i]).TextType.text里面存储的字符串空间也需要malloc

	/*
	 *
	 *
	 * ********************************************请开始你的表演
	 *
	 *
	 */


	//以下为测试环境下的代码，即随便塞一堆数据给变量txtFilesInfoSpace，这样就能在SD卡尚未调通时不影响其他函数测试
	//当然，李远哲可以参考以下代码写SD卡调通时的readAlltxtFilesOfSDcard函数代码

	txtFilesInfoSpace.curOpenFileIndex = 0;//第一个文本
	int filesNum = 6;
	txtFilesInfoSpace.txtFilesNum = filesNum;//文件总个数
	txtFilesInfoSpace.txtFileList = (TxtFile *)malloc( sizeof(TxtFile) * filesNum );

	char tempFilesName[6][20] = {
			"电信骄子",
			"华科巨佬",
			"斗罗大陆",
			"ED2-115开发板资料",
			"仙剑奇侠传3",
			"Good night!"
	};

	TxtFile *curTxtFile;
	int i;
	for ( i = 0; i < filesNum; i ++ )
	{
		curTxtFile = txtFilesInfoSpace.txtFileList + i;
		curTxtFile->curBytePos = 0;//当前阅读到0字节
		curTxtFile->curPageNum = 1;//第一页
		curTxtFile->curPagePercent = 0;//进度0%
		curTxtFile->fileSizeInBytes = 10000;//10000个字节
		curTxtFile->totalPageNum = curTxtFile->fileSizeInBytes / ( BOOK_ROW_BYTES * BOOK_COL_NUM );
		(curTxtFile->txtFileName).textLen = strlen(tempFilesName[i]);
		(curTxtFile->txtFileName).text = malloc( sizeof(char) * (curTxtFile->txtFileName).textLen );
		strcpy( (curTxtFile->txtFileName).text, tempFilesName[i] );
	}


	return status;
}


//读取SD卡中的文本内容(txtBookInst里面有当前打开电子书的阅读进度),存储于全局变量txtBookSpace中
bool readBookOfTxtFile( TxtFile *txtBookInst,  enum bookTurnPageType turnPageSel, float jumpPagePct )
{
	bool status = true;

	//设计要求：修改txtBookInst中阅读进度，并读取翻页后的文本内容于txtBookSpace中
	//翻页时，如果是上下翻页时，要保证文本连续性，还要保证换行符对齐一行的开头
	//如果是页首，页尾，就向页首页尾文本顶齐
	//如果是页面跳转，就通过进度百分数jumpPagePct计算要跳转的页数，然后找到附近最近的换行符，如果一个显示域内找不到换行符,就随便

	//填充文本内容要保证不超出BOOK文本显示框宽度，BOOK文本字符的间隔可以在screen_type.h中查询到

	//在screen.h库中又一个方便的函数，可以获取文本中汉字与字母的分布表(返回动态数组，记得free)
	//该函数为:char *getWordLetterArrange( char *str, short len );

	/*
	 *
	 *
	 * ********************************************请开始你的表演
	 *
	 *
	 */





	return status;
}
