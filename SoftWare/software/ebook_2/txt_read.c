/*
 * txt_read.c
 *
 *  Created on: 2019年4月27日
 *      Author: 12757
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "txt_read.h"
#include "screen.h"
#include "test_value.h"


//临时存储从SD卡中读出的文件名
char sdCardFileName[TXT_FILE_NAME_SIZE];

TxtFilesInfo txtFilesInfoSpace;

//char txtBookSpace[BOOK_COL_NUM][BOOK_ROW_BYTES+1] = {"hahaha","lalala","gauguagua"};
//char txtBookSpace[BOOK_COL_NUM][BOOK_ROW_BYTES+1] = { "慕涤佬巨神","楼主说的对",{0},"我是华科菜鸡" };
char txtBookSpace[BOOK_COL_NUM][BOOK_ROW_BYTES+1] = {
    "内容简介:",
    "    唐门外门弟子唐三，因偷学内门绝",
    "学为唐门所不容，跳崖明志时却来到了",
    "另一个世界，一个属于武魂的世界。名",
    "叫斗罗大陆。",
    "    这里没有魔法，没有斗气，没有武",
    "术，却有神奇的武魂。这里的每个人，",
    "在自己六岁的时候，都会在武魂殿中令",
    "武魂觉醒。武魂有动物，有植物，有器",
    "物，它们可以辅助人们的日常生活。而",
    "其中一些特别出色的武魂却可以用来修",
    "炼，这个职业，是斗罗大陆上最为强大",
    "也是最重要的职业，——魂师。",
    "    当唐门暗器来到斗罗大陆，当唐三",
    "武魂觉醒，他能否在这片武魂的世界重",
    "塑唐门辉煌？",
    "    职业等级：魂士、魂师、大魂师、",
    "魂尊、魂宗、魂王、魂帝、魂圣、斗罗",
    "、封号斗罗。",
    "  Please do not split Enlish words",
    "into two lines"
};



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

	#define TXT_FILES_NUM 25
	int filesNum = TXT_FILES_NUM;
//    int filesNum = 1;
	txtFilesInfoSpace.txtFilesNum = filesNum;//文件总个数

	txtFilesInfoSpace.txtFileList = ( TxtFile *)malloc( sizeof(TxtFile) * filesNum );
//    txtFilesInfoSpace.txtFileList = NULL;

//    char tempFilesName[1][40] = { "哈哈哈哈哈哈活动和覅和覅u火速" };

	char tempFilesName[TXT_FILES_NUM][40] = {
			"华中科技大学电子信息与通信学院",
			"华科巨佬",
			"斗罗大陆",
			"ED2-115开发板资料",
			"仙剑奇侠传3",
			"Good night!",
			"What are you 弄啥嘞？？？？？你在吗",
			"Leave me alone",
			"なんでもないや",
			"哈哈哈哈哈哈哈哈嗝",
			"涤佬是怎么炼成的",
			"硬件课设",
			"微波电路",
			"Come on, baby",
			"altera",
			"nios 2",
			"七个白雪公主和一个小矮人",
			"无线电波",
			"人工智能",
			"快乐医疗",
			"还好没有乱码，GBK还是很优秀",
			"名侦探柯南",
			"滚筒洗衣机",
			"步步高点读机，哪里不会点哪里",
			"final music"
	};

	TxtFile *curTxtFile;
	int i;
	for ( i = 0; i < filesNum; i ++ )
	{
        //数组偏移
		curTxtFile = (txtFilesInfoSpace.txtFileList) + i;

		//文件信息填充
		curTxtFile->curBytePos = 0;//当前阅读到0字节
		curTxtFile->curPageNum = 1;//第一页
		curTxtFile->curPageRatio = 0;//进度(范围0~1)
		curTxtFile->fileSizeInBytes = 10000;//10000个字节
		curTxtFile->totalPageNum = curTxtFile->fileSizeInBytes / ( BOOK_ROW_BYTES * BOOK_COL_NUM );

		//字符串长度
		int textLen = strlen(tempFilesName[i]);
		(curTxtFile->txtFileName).textLen = textLen;

		//字符串存储空间malloc
        char *tempPtr = (char*)malloc( sizeof(char) * (textLen+1) );
        (curTxtFile->txtFileName).text = tempPtr;

        //存储文件名
        strcpy( tempPtr, tempFilesName[i] );
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
    //英文单词不可拆开于两行

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
