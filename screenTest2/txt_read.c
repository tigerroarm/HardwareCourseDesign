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
#include "sdread.h"

//临时存储从SD卡中读出的文件名
char sdCardFileName[TXT_FILE_NAME_SIZE];

TxtFilesInfo txtFilesInfoSpace;


char txtBookSpace[BOOK_COL_NUM][(BOOK_ROW_BYTES+1)*2] = {
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
	//((txtFilesInfoSpace.txtFileList)[i]).TextType.text里面存储的字符串空间需要malloc

	txtFilesInfoSpace.curOpenFileIndex = 0;//第一个文本
	txtFilesInfoSpace.curFileHandle = -1;
	txtFilesInfoSpace.txtFilesNum = 0;


	//读取根目录下所有文件

    #define TXT_FILES_NUM 7
	int filesNum = TXT_FILES_NUM;
	txtFilesInfoSpace.txtFilesNum = filesNum;//文件总个数

    char tempFilesName[TXT_FILES_NUM][40] = {
        "dilaoqia.txt",
        "usc2_big.txt",
        "usc2_little.txt",
        "测试.txt",
        "斗罗大陆.txt",
        "斗罗大陆gbk.txt",
        "巨佬黎冰凌.txt"
	};

	#undef TXT_FILES_NUM

	TxtFile *curTxtFile;
	int i;
	for ( i = 0; i < filesNum; i ++ )
	{
        //数组偏移
		curTxtFile = (txtFilesInfoSpace.txtFileList) + i;

		//文件信息填充
		curTxtFile->curPageBytePosStart = 0;//当前阅读到0字节
		curTxtFile->curPageBytePosEnd = 0;//当前阅读到0字节
		curTxtFile->curPageRatio = 0;//进度(范围0~1)
		curTxtFile->fileSizeInBytes = 10000;//10000个字节

		//字符串长度
		int textLen = strlen(tempFilesName[i]);
		curTxtFile->txtFileName.textLen = textLen;
		curTxtFile->txtFileReadName.textLen = textLen;
		curTxtFile->txtFileName.fmt = GBK;
		curTxtFile->txtFileReadName.fmt = GBK;

		//字符串存储空间malloc
        char *tempPtr1 = (char*)malloc( sizeof(char) * (textLen+1) );
        char *tempPtr2 = (char*)malloc( sizeof(char) * (textLen+1) );
        (curTxtFile->txtFileName).text = tempPtr1;
        (curTxtFile->txtFileReadName).text = tempPtr2;

        //存储文件名
        strcpy( tempPtr1, tempFilesName[i] );
        strcpy( tempPtr2, tempFilesName[i] );

	}

	return status;

}


//读取SD卡中的文本内容(txtBookInst里面有当前打开电子书的阅读进度),存储于全局变量txtBookSpace中
bool readBookOfTxtFile( TxtFile *txtBookInst,  enum bookTurnPageType turnPageSel, float jumpPageRatio )
{
	bool status = true;

	//检验，防止为空
	if ( txtBookInst == NULL )
	{
		return false;
	}

	if ( turnPageSel == BOOK_PAGE_PRE )
    {
        printf( "pageChangePre\n" );
        readLastPage( txtBookInst, txtFilesInfoSpace.curFileHandle );
    }
    else if ( turnPageSel == BOOK_PAGE_NEXT )
    {
        printf( "pageChangeNext\n" );
        readNextPage( txtBookInst,  txtFilesInfoSpace.curFileHandle );
    }
    else if ( turnPageSel == BOOK_PAGE_JUMP )
    {
        printf( "pageJump\n" );
        readPageByRatio( txtBookInst, txtFilesInfoSpace.curFileHandle, jumpPageRatio );
    }

	//计算进度比率
	if ( txtBookInst->fileSizeInBytes == 0 )
	{
		txtBookInst->curPageRatio = 0;
	}
	else
	{
		txtBookInst->curPageRatio = ((float)(txtBookInst->curPageBytePosStart)) / txtBookInst->fileSizeInBytes;
	}

	return status;
}

//打开SD卡的某个文本
TxtFile *openTxtFile( short fileIndex )
{
    if ( fileIndex < 0 || fileIndex >= txtFilesInfoSpace.txtFilesNum )
    {
        return (void*)0;
    }
    txtFilesInfoSpace.curOpenFileIndex = fileIndex;

    TxtFile *txtFile = txtFilesInfoSpace.txtFileList + txtFilesInfoSpace.curOpenFileIndex;

    //调用打开文件函数
	char *fileReadName = txtFile->txtFileReadName.text;
    short file_handle = fopen_txt( fileReadName );
    txtFilesInfoSpace.curFileHandle = file_handle;

    printf( "openTxtFile: open FileHandle = %d\n", txtFilesInfoSpace.curFileHandle );

    if( file_handle < 0 )
    {
    	return NULL;
    }
    printf( "fileReadName = %s\n", fileReadName );
    printf( "file_handle = %d\n", file_handle );
    txtFile->fileSizeInBytes = getfileSizeTxt( file_handle );
    printf( "fileSizeInBytes = %d\n", txtFile->fileSizeInBytes );


    //读取文件第1和第2个字节，判断编码格式
    if ( txtFile->fileSizeInBytes >= 2 )
    {
    	alt_u8 store[2]={0};
    	fseek_txt( file_handle, 0, SEEK_SET_TXT );
    	fread_txt( file_handle, (char*)store, 2 );
    	if ( store[0] == 0xfe && store[1] == 0xff )
    	{
    		txtFile->fmt = UCS2_BIG;
    	}
    	else if ( store[0] == 0xff && store[1] == 0xfe )
    	{
    		txtFile->fmt = UCS2_LITTLE;
    	}
    	else
    	{
    		txtFile->fmt = GBK;
    	}
    }
    else
    {
    	txtFile->fmt = GBK;
    }

//    txtFile->fmt = GBK;

    //同步编码格式到Book的TagList中
    screenSyncBookEcdfmt( txtFile->fmt );

    switch( txtFile->fmt )
    {
		case GBK:
			printf( "txtFile->fmt = %s\n", "GBK" );
			break;

		case UCS2_BIG:
			printf( "txtFile->fmt = %s\n", "UCS2_BIG" );
			break;

		case UCS2_LITTLE:
			printf( "txtFile->fmt = %s\n", "UCS2_LITTLE" );
			break;
		default:
			printf( "txtFile->fmt = %s\n", "NONE" );
			break;
    }

//    //打开当前页（通过读上一页的下一页）
//    readBookOfTxtFile( txtFile, BOOK_PAGE_NEXT, 0 );

    //打开当前页
    readCurPage( txtFile, file_handle );

    printf( "readBookOfTxtFile finish\n" );

    return txtFile;
}

//关闭TxtFile文件
bool closeTxtFile( )
{
    int status = fclose_txt( txtFilesInfoSpace.curFileHandle );

    printf( "closeTxtFile: close FileHandle = %d\n", txtFilesInfoSpace.curFileHandle );

    txtFilesInfoSpace.curFileHandle = -1;

    if ( status == 0 )
    {
    	return true;
    }
    else
    {
    	return false;
    }
}


// read  a page in GBK format
//0 no more content, 1 book has not been finished
bool readPageGBK( TxtFile *txtBookInst,  short file_handle, unsigned int startpos, unsigned int* pagesize){

    //initial
    int j;
    for(j=0; j< BOOK_COL_NUM;j++){
    	txtBookSpace[j][0] = 0x00;
    }

    int linecounter = 0; // the index of line in present page
    unsigned int txtIndex = 0;
    int lineindex = 0;
    int blanknum = 0;
    int step, i, txtlength;
    char ch;
    char buff[SAFE_STR_LEN];
    fseek_txt( file_handle, startpos, SEEK_SET_TXT);
    txtlength = fread_txt(file_handle, buff, SAFE_STR_LEN);

//    printf(" txtlength = %d, SBL = %d \n", txtlength, SAFE_STR_LEN);
//    printf("*%d*\n", (int)buff[379]);
    while( txtIndex<txtlength ){
        ch = buff[txtIndex];

        if( ch < 0){
            step = 2;
            if((lineindex + step)>BOOK_ROW_BYTES ){
                //need a new line
            	txtBookSpace[linecounter][lineindex] = 0x00;
                linecounter++;
                lineindex = 0;
                if( linecounter >= BOOK_COL_NUM)
                {
                    *pagesize = txtIndex;
                    return  ((startpos + txtIndex) < txtBookInst->fileSizeInBytes);
                }
            }
            // currently we get the lst byte of a Chinese character
            txtBookSpace[linecounter][lineindex] = ch;
            txtIndex++; lineindex++;
            // push the 2nd byte of the Chinese character into buff
            ch = buff[txtIndex];
            txtBookSpace[linecounter][lineindex] = ch;
            txtIndex++; lineindex++;
        }
        else{
            // currently we meet an ANSI character
            switch(ch){
            case 0x20://' '
                step = 1;
                if((lineindex + step)>BOOK_ROW_BYTES ){
                    //need a new line
                	txtBookSpace[linecounter][lineindex] = 0x00;
                    linecounter++;
                    lineindex = 0;
                    if( linecounter >= BOOK_COL_NUM){
                        *pagesize = txtIndex;
                        return  ((startpos + txtIndex) < txtBookInst->fileSizeInBytes);
                    }
                }
                txtBookSpace[linecounter][lineindex] = 0x20;

                txtIndex++; lineindex++;
                break;

            case 0x0d:
                //printf("\ni see you $r %d\n", txtIndex);
            	txtBookSpace[linecounter][lineindex] = 0x00;
                txtIndex += 2;
                linecounter++; lineindex = 0;
                if( linecounter >= BOOK_COL_NUM){
                    *pagesize = txtIndex;
                    return  ((startpos + txtIndex) < txtBookInst->fileSizeInBytes);
                }
                break;

            case 0x0a:
                //printf("\ni see you $n %d\n", txtIndex);
            	txtBookSpace[linecounter][lineindex] = 0x00;
                txtIndex += 1;
                linecounter++; lineindex = 0;
                if( linecounter >= BOOK_COL_NUM){
                    *pagesize = txtIndex;
                    return  ((startpos + txtIndex) < txtBookInst->fileSizeInBytes);
                }
                break;

            case 0x09://'\t'
                step = 4;
                blanknum = step;
                if((lineindex + step)>BOOK_ROW_BYTES ){
                    //need a new line
                	blanknum = lineindex + step - BOOK_ROW_BYTES;
                	txtBookSpace[linecounter][lineindex] = 0x00;
                    linecounter++;
                    lineindex = 0;
                    if( linecounter >= BOOK_COL_NUM){
                        *pagesize = txtIndex;
                        return  ((startpos + txtIndex) < txtBookInst->fileSizeInBytes);
                    }
                }

                for(j = 0; j < blanknum; j++){
                	txtBookSpace[linecounter][lineindex] = 0x20;
                	lineindex ++;
                }// fix the blank, read the '\t'

                txtIndex ++;
                break;

            default:
                // currently we may meet an ANSI character
                // we must make sure that English words will not be split into two lines
                if( isEnglishletter(ch) ){
                    step = 1;

                    if((step+txtIndex+1)<txtlength){
                    	 for(;1;step++){
							ch = buff[txtIndex + step];
							if( (!isEnglishletter(ch)) || ((step+txtIndex+1)>=txtlength) ){
								break;
							}
                    	}
                    }

                    if((lineindex + step)<=BOOK_ROW_BYTES){
                        // don not need a new line
                        for(i = 0; i < step; i++){
                            ch = buff[txtIndex+i];
                            txtBookSpace[linecounter][lineindex+i] = ch;
                        }
                        lineindex += step;
                        txtIndex += step;
                    }
                    else if( step <= BOOK_ROW_BYTES){
                        //need a new line
                        //one line is enough to show the word
                    	txtBookSpace[linecounter][lineindex] = 0x00;
                        linecounter++;
                        lineindex = 0;
                        if( linecounter >= BOOK_COL_NUM){
                            *pagesize = txtIndex;
                            return  ((startpos + txtIndex) < txtBookInst->fileSizeInBytes);
                        }
                    }
                    else{
                        //one line is not enough to show the string
                        //we just push the letter into screen one by one
                        for(i = 0; i < step; i++){
                            ch = buff[txtIndex];
                            if( lineindex >= BOOK_ROW_BYTES){
                            	txtBookSpace[linecounter][BOOK_ROW_BYTES] = 0x00;
                                linecounter++;
                                lineindex = 0;
                                if( linecounter >= BOOK_COL_NUM){
                                    *pagesize = txtIndex;
                                    return  ((startpos + txtIndex) < txtBookInst->fileSizeInBytes);
                                }
                            }

                            txtBookSpace[linecounter][lineindex] = ch;
                            txtIndex++; lineindex++;
                        }//end for: store the long string

                    }//end else:store the long string

                }//end if: get an English letter
                else{
                    step = 1;
                    if((lineindex + step)>BOOK_ROW_BYTES ){
                        //need a new line
                    	txtBookSpace[linecounter][lineindex] = 0x00;
                        linecounter++;
                        lineindex = 0;
                        if( linecounter >= BOOK_COL_NUM){
                            *pagesize = txtIndex;
                            return  ((startpos + txtIndex) < txtBookInst->fileSizeInBytes);
                        }
                    }

                    txtBookSpace[linecounter][lineindex] = ch;
                    txtIndex++; lineindex++;
                }//end else: get an ANSI character but not a English letter

            }//end of switch: get an ANSI character

        }//end of else: get an ANSI character

    }// end of while: get character
    txtBookSpace[linecounter][lineindex] = 0x00;
    printf("quit outer while\n");
    *pagesize = txtIndex;
    return  ((startpos + txtIndex) < txtBookInst->fileSizeInBytes);

}

bool readPageUNI(TxtFile *txtBookInst,  short file_handle, unsigned int startpos, unsigned int* pagesize ){
	if(txtBookInst->fmt == UCS2_BIG){
		return readPageUNI_BIG( txtBookInst,  file_handle,  startpos,  pagesize );
	}else{
		return readPageUNI_LITTLE( txtBookInst, file_handle, startpos, pagesize );
	}
}

bool readPageUNI_BIG( TxtFile *txtBookInst,  short file_handle, unsigned int startpos, unsigned int* pagesize ){
    //init txtBookSpace[][]
    int j;
    for(j=0; j< BOOK_COL_NUM;j++){
    	txtBookSpace[j][0] = 0x00;
    	txtBookSpace[j][1] = 0x00;
    }

    int linecounter = 0; // the index of line in present page
    unsigned int txtIndex = 0;
    int lineindex = 0;
    int linetaken = 0;
    int blanknum;
    int step, i, txtlength;
    char ch1, ch2;
    char buff[2*SAFE_STR_LEN];

    fseek_txt( file_handle, startpos, SEEK_SET_TXT);
    txtlength = fread_txt(file_handle, buff, 2*SAFE_STR_LEN);

    if( txtlength>0 ){
        if((txtlength&0x00000001)>0){
            txtlength--;
        }
    }

    while( txtIndex<txtlength ){
        ch1 = buff[txtIndex];
        ch2 = buff[txtIndex+1];

        if( (ch1==0x00)&(ch2<0x80) ){
            // currently we meet an ASCII character
            switch(ch2){
            case 0x20://' '
                step = 1;
                if((linetaken + step)>BOOK_ROW_BYTES ){
                    //need a new line
                	txtBookSpace[linecounter][lineindex] = 0x00;
                	txtBookSpace[linecounter][lineindex+1] = 0x00;
                    linecounter++;
                    lineindex = 0; linetaken = 0;
                    if( linecounter >= BOOK_COL_NUM){
                        *pagesize = txtIndex;
                        return  ((startpos + txtIndex) < txtBookInst->fileSizeInBytes);
                    }
                }
                txtBookSpace[linecounter][lineindex] = 0x00;
                txtBookSpace[linecounter][lineindex+1] = 0x20;

                txtIndex+=2; lineindex+=2; linetaken+=1;
                break;

            case 0x0d://change line
                //printf("\ni see you $r %d\n", txtIndex);
            	txtBookSpace[linecounter][lineindex] = 0x00;
            	txtBookSpace[linecounter][lineindex+1] = 0x00;
                txtIndex += 4;
                linecounter++; lineindex = 0; linetaken = 0;
                if( linecounter >= BOOK_COL_NUM){
                    *pagesize = txtIndex;
                    return  ((startpos + txtIndex) < txtBookInst->fileSizeInBytes);
                }
                break;

            case 0x09://\t
                step = 4;
                blanknum = step;
                if((linetaken + step)>BOOK_ROW_BYTES ){
                    //need a new line
                    blanknum = linetaken + step - BOOK_ROW_BYTES;
                	txtBookSpace[linecounter][lineindex] = 0x00;
                	txtBookSpace[linecounter][lineindex+1] = 0x00;
                    linecounter++;
                    lineindex = 0; linetaken = 0;
                    if( linecounter >= BOOK_COL_NUM){
                        *pagesize = txtIndex;
                        return  ((startpos + txtIndex) < txtBookInst->fileSizeInBytes);
                    }
                }

                for(j = 0; j < blanknum; j ++){
                	txtBookSpace[linecounter][lineindex] = 0x00;
                	txtBookSpace[linecounter][lineindex+1] = 0x20;
                	linetaken++;
                    lineindex+=2;
                }
                txtIndex+=2;
                break;

            default:
                // currently we may meet an ASCII character
                // we must make sure that English words will not be split into two lines
                if( isUNIEnglishletter(ch1, ch2) ){
                    step = 1;
                    if((step+txtIndex+1)<txtlength){
                        for(;1;step++){
                            ch1 = buff[txtIndex + 2*step];
                            ch2 = buff[txtIndex + 2*step + 1];
                            if( (!isUNIEnglishletter(ch1, ch2)) | ((txtIndex + 2*step + 2)>=txtlength) ){
                                break;
                            }
                        }
                    }

                    if((linetaken + step)<=BOOK_ROW_BYTES){
                        // don not need a new line
                        for(i = 0; i < step; i++){
                            ch1 = buff[txtIndex+2*i];
                            ch2 = buff[txtIndex+2*i+1];
                            txtBookSpace[linecounter][lineindex+2*i] = ch1;
                            txtBookSpace[linecounter][lineindex+2*i+1] = ch2;
                        }
                        lineindex += 2*step;
                        linetaken += step;
                        txtIndex += 2*step;
                    }
                    else if( step <= BOOK_ROW_BYTES){
                        //need a new line
                        //one line is enough to show the word
                    	txtBookSpace[linecounter][lineindex] = 0x00;
                    	txtBookSpace[linecounter][lineindex+1] = 0x00;
                        linecounter++;
                        lineindex = 0;
                        linetaken = 0;
                        if( linecounter >= BOOK_COL_NUM){
                            *pagesize = txtIndex;
                            return  ((startpos + txtIndex) < txtBookInst->fileSizeInBytes);
                        }
                    }
                    else{
                        //one line is not enough to show the string
                        //we just push the letter into screen one by one
                        for(i = 0; i < step; i++){
                            ch1 = buff[txtIndex];
                            ch2 = buff[txtIndex+1];

                            if( linetaken >= BOOK_ROW_BYTES){
                            	txtBookSpace[linecounter][lineindex] = 0x00;
                            	txtBookSpace[linecounter][lineindex+1] = 0x00;
                                linecounter++;
                                lineindex = 0;
                                linetaken = 0;
                                if( linecounter >= BOOK_COL_NUM){
                                    *pagesize = txtIndex;
                                    return  ((startpos + txtIndex) < txtBookInst->fileSizeInBytes);
                                }
                            }

                            txtBookSpace[linecounter][lineindex] = ch1;
                            txtBookSpace[linecounter][lineindex+1] = ch2;
                            txtIndex+=2; lineindex+=2; linetaken+=1;
                        }//end for: store the long string

                    }//end else:store the long string
                }//end if: get an English letter
                else{//else: get an ASCII character but not a English letter

                    step = 1;
                    if((linetaken + step)>BOOK_ROW_BYTES ){
                        //need a new line
                    	txtBookSpace[linecounter][lineindex] = 0x00;
                    	txtBookSpace[linecounter][lineindex+1] = 0x00;
                        linecounter++;
                        lineindex = 0; linetaken = 0;
                        if( linecounter >= BOOK_COL_NUM){
                            *pagesize = txtIndex;
                            return  ((startpos + txtIndex) < txtBookInst->fileSizeInBytes);
                        }
                    }
                    txtBookSpace[linecounter][lineindex] = 0x00;
                    txtBookSpace[linecounter][lineindex+1] = ch2;
                    txtIndex+=2; lineindex+=2; linetaken+=1;
                }//end else: get an ASCII character but not a English letter

            }//end of switch: get an ASCII character
        }//end if: get an ASCII character
        else{// get a not-ASCII character
            step = 2;
            if((linetaken + step)>BOOK_ROW_BYTES ){
                //need a new line
            	txtBookSpace[linecounter][lineindex] = 0x00;
            	txtBookSpace[linecounter][lineindex+1] = 0x00;
                linecounter++;
                lineindex = 0; linetaken = 0;
                if( linecounter >= BOOK_COL_NUM){
                    *pagesize = txtIndex;
                    return  ((startpos + txtIndex) < txtBookInst->fileSizeInBytes);
                }
            }
            txtBookSpace[linecounter][lineindex] = ch1;
            txtBookSpace[linecounter][lineindex+1] = ch2;

            txtIndex+=2; lineindex+=2; linetaken+=2;
        }// end of else: get a not-ASCII character

    }// end of while: get character
    txtBookSpace[linecounter][lineindex] = 0x00;
    txtBookSpace[linecounter][lineindex+1] = 0x00;
    *pagesize = txtIndex;
    return  ((startpos + txtIndex) < txtBookInst->fileSizeInBytes);

}

bool readPageUNI_LITTLE( TxtFile *txtBookInst,  short file_handle, unsigned int startpos, unsigned int* pagesize ){
    //init txtBookSpace[][]
    int j;
    for(j=0; j< BOOK_COL_NUM;j++){
    	txtBookSpace[j][0] = 0x00;
    	txtBookSpace[j][1] = 0x00;
    }

    int linecounter = 0; // the index of line in present page
    unsigned int txtIndex = 0;
    int lineindex = 0;
    int linetaken = 0;
    int blanknum;
    int step, i, txtlength;
    char ch1, ch2;
    char buff[2*SAFE_STR_LEN];

    fseek_txt( file_handle, startpos, SEEK_SET_TXT);
    txtlength = fread_txt(file_handle, buff, 2*SAFE_STR_LEN);

    if( txtlength>0 ){
        if((txtlength&0x00000001)>0){
            txtlength--;
        }
    }

    while( txtIndex<txtlength ){
        ch1 = buff[txtIndex];
        ch2 = buff[txtIndex+1];

        if( (ch2==0x00)&(ch1<0x80) ){
            // currently we meet an ASCII character
            switch(ch1){
            case 0x20://' '
                step = 1;
                if((linetaken + step)>BOOK_ROW_BYTES ){
                    //need a new line
                	txtBookSpace[linecounter][lineindex] = 0x00;
                	txtBookSpace[linecounter][lineindex+1] = 0x00;
                    linecounter++;
                    lineindex = 0; linetaken = 0;
                    if( linecounter >= BOOK_COL_NUM){
                        *pagesize = txtIndex;
                        return  ((startpos + txtIndex) < txtBookInst->fileSizeInBytes);
                    }
                }
                txtBookSpace[linecounter][lineindex] = 0x20;
                txtBookSpace[linecounter][lineindex+1] = 0x00;

                txtIndex+=2; lineindex+=2; linetaken+=1;
                break;

            case 0x0d://change line
                //printf("\ni see you $r %d\n", txtIndex);
            	txtBookSpace[linecounter][lineindex] = 0x00;
            	txtBookSpace[linecounter][lineindex+1] = 0x00;
                txtIndex += 4;
                linecounter++; lineindex = 0; linetaken = 0;
                if( linecounter >= BOOK_COL_NUM){
                    *pagesize = txtIndex;
                    return  ((startpos + txtIndex) < txtBookInst->fileSizeInBytes);
                }
                break;

            case 0x09://\t
                step = 4;
                blanknum = step;
                if((linetaken + step)>BOOK_ROW_BYTES ){
                    //need a new line
                    blanknum = linetaken + step - BOOK_ROW_BYTES;
                	txtBookSpace[linecounter][lineindex] = 0x00;
                	txtBookSpace[linecounter][lineindex+1] = 0x00;
                    linecounter++;
                    lineindex = 0; linetaken = 0;
                    if( linecounter >= BOOK_COL_NUM){
                        *pagesize = txtIndex;
                        return  ((startpos + txtIndex) < txtBookInst->fileSizeInBytes);
                    }
                }

                for(j = 0; j < blanknum; j ++){
                	txtBookSpace[linecounter][lineindex] = 0x20;
                	txtBookSpace[linecounter][lineindex+1] = 0x00;
                	linetaken++;
                    lineindex+=2;
                }
                txtIndex+=2;
                break;

            default:
                // currently we may meet an ASCII character
                // we must make sure that English words will not be split into two lines
                if( isUNIEnglishletter(ch2, ch1) ){
                    step = 1;
                    if((step+txtIndex+1)<txtlength){
                        for(;1;step++){
                            ch1 = buff[txtIndex + 2*step];
                            ch2 = buff[txtIndex + 2*step + 1];
                            if( (!isUNIEnglishletter(ch2, ch1)) | ((txtIndex + 2*step + 2)>=txtlength) ){
                                break;
                            }
                        }
                    }

                    if((linetaken + step)<=BOOK_ROW_BYTES){
                        // don not need a new line
                        for(i = 0; i < step; i++){
                            ch1 = buff[txtIndex+2*i];
                            ch2 = buff[txtIndex+2*i+1];
                            txtBookSpace[linecounter][lineindex+2*i] = ch1;
                            txtBookSpace[linecounter][lineindex+2*i+1] = ch2;
                        }
                        lineindex += 2*step;
                        linetaken += step;
                        txtIndex += 2*step;
                    }
                    else if( step <= BOOK_ROW_BYTES){
                        //need a new line
                        //one line is enough to show the word
                    	txtBookSpace[linecounter][lineindex] = 0x00;
                    	txtBookSpace[linecounter][lineindex+1] = 0x00;
                        linecounter++;
                        lineindex = 0;
                        linetaken = 0;
                        if( linecounter >= BOOK_COL_NUM){
                            *pagesize = txtIndex;
                            return  ((startpos + txtIndex) < txtBookInst->fileSizeInBytes);
                        }
                    }
                    else{
                        //one line is not enough to show the string
                        //we just push the letter into screen one by one
                        for(i = 0; i < step; i++){
                            ch1 = buff[txtIndex];
                            ch2 = buff[txtIndex+1];

                            if( linetaken >= BOOK_ROW_BYTES){
                            	txtBookSpace[linecounter][lineindex] = 0x00;
                            	txtBookSpace[linecounter][lineindex+1] = 0x00;
                                linecounter++;
                                lineindex = 0;
                                linetaken = 0;
                                if( linecounter >= BOOK_COL_NUM){
                                    *pagesize = txtIndex;
                                    return  ((startpos + txtIndex) < txtBookInst->fileSizeInBytes);
                                }
                            }

                            txtBookSpace[linecounter][lineindex] = ch1;
                            txtBookSpace[linecounter][lineindex+1] = ch2;
                            txtIndex+=2; lineindex+=2; linetaken+=1;
                        }//end for: store the long string

                    }//end else:store the long string
                }//end if: get an English letter
                else{//else: get an ASCII character but not a English letter

                    step = 1;
                    if((linetaken + step)>BOOK_ROW_BYTES ){
                        //need a new line
                    	txtBookSpace[linecounter][lineindex] = 0x00;
                    	txtBookSpace[linecounter][lineindex+1] = 0x00;
                        linecounter++;
                        lineindex = 0; linetaken = 0;
                        if( linecounter >= BOOK_COL_NUM){
                            *pagesize = txtIndex;
                            return  ((startpos + txtIndex) < txtBookInst->fileSizeInBytes);
                        }
                    }
                    txtBookSpace[linecounter][lineindex] = ch1;
                    txtBookSpace[linecounter][lineindex+1] = ch2;
                    txtIndex+=2; lineindex+=2; linetaken+=1;
                }//end else: get an ASCII character but not a English letter

            }//end of switch: get an ASCII character
        }//end if: get an ASCII character
        else{// get a not-ASCII character
            step = 2;
            if((linetaken + step)>BOOK_ROW_BYTES ){
                //need a new line
            	txtBookSpace[linecounter][lineindex] = 0x00;
            	txtBookSpace[linecounter][lineindex+1] = 0x00;
                linecounter++;
                lineindex = 0; linetaken = 0;
                if( linecounter >= BOOK_COL_NUM){
                    *pagesize = txtIndex;
                    return  ((startpos + txtIndex) < txtBookInst->fileSizeInBytes);
                }
            }
            txtBookSpace[linecounter][lineindex] = ch1;
            txtBookSpace[linecounter][lineindex+1] = ch2;

            txtIndex+=2; lineindex+=2; linetaken+=2;
        }// end of else: get a not-ASCII character

    }// end of while: get character
    txtBookSpace[linecounter][lineindex] = 0x00;
    txtBookSpace[linecounter][lineindex+1] = 0x00;
    *pagesize = txtIndex;
    return  ((startpos + txtIndex) < txtBookInst->fileSizeInBytes);

}

bool readCurPage( TxtFile *txtBookInst,  short file_handle ){
    //initial
    int j;
    for(j=0; j< BOOK_COL_NUM;j++){
    	txtBookSpace[j][0] = 0x00;
    }

    unsigned int pagesize;
    bool bookunfinish;

	switch( txtBookInst->fmt ){
	case GBK:
		if(txtBookInst->curPageBytePosStart == 0){
			txtBookInst->curPageBytePosStart = 1;
		}
		//当前页的开始Byte位置，恰好为本页Byte数组开始的index+1
		//bookunfinish = readPageGBK(txtBookInst, fp, txtBookInst->curPageBytePosEnd , &pagesize);
		bookunfinish = readPageGBK(txtBookInst, file_handle, (txtBookInst->curPageBytePosStart)-1 , &pagesize);
		//txtBookInst->curPageBytePosStart = txtBookInst->curPageBytePosStart;
		txtBookInst->curPageBytePosEnd = pagesize +(txtBookInst->curPageBytePosStart - 1);
		if ( txtBookInst->fileSizeInBytes == 0 )
        {
            txtBookInst->curPageRatio = 0;
        }
        else
        {
            txtBookInst->curPageRatio = (float)txtBookInst->curPageBytePosStart/(float)txtBookInst->fileSizeInBytes;
        }


		return bookunfinish;
		break;

	case UCS2_LITTLE:
	case UCS2_BIG:
		if(txtBookInst->curPageBytePosStart < 3){
			txtBookInst->curPageBytePosStart = 3;
		}
		//当前页开始的Byte位置，恰好为本页Byte数组开始的index+1
		//bookunfinish = readPageUNI(txtBookInst, fp, txtBookInst->curPageBytePosEnd , &pagesize);
		bookunfinish = readPageUNI(txtBookInst, file_handle, (txtBookInst->curPageBytePosStart)-1 , &pagesize);
		//txtBookInst->curPageBytePosStart = txtBookInst->curPageBytePosStart;
		txtBookInst->curPageBytePosEnd = pagesize +(txtBookInst->curPageBytePosStart - 1);
        if ( txtBookInst->fileSizeInBytes == 0 )
        {
            txtBookInst->curPageRatio = 0;
        }
        else
        {
            txtBookInst->curPageRatio = (float)txtBookInst->curPageBytePosStart/(float)txtBookInst->fileSizeInBytes;
        }

		return bookunfinish;
		break;

	default:
		break;
	}// end switch

	return false;

}

bool readLastPage( TxtFile *txtBookInst,  short file_handle ){
    unsigned int searchsize, startpos;
    bool bookunfinish;
    char buff[10*SAFE_STR_LEN];
    unsigned int searchindex, txtlength;

    if( (txtBookInst->curPageBytePosStart <= 1) && (txtBookInst->fmt == GBK ) ){
    	return false;
    }

    if( (txtBookInst->curPageBytePosStart <= 3) && (txtBookInst->fmt == UCS2_BIG ) ){
    	return false;
    }

    switch( txtBookInst->fmt ){
    case GBK:
        //read enough content to find an available start for new page
        searchsize = 10*SAFE_STR_LEN;
        if( (txtBookInst->curPageBytePosStart) < (1+searchsize) ){
            startpos = 0;
        }else{
            startpos = (txtBookInst->curPageBytePosStart)-searchsize-1;
        }

        if(startpos > 0){
            fseek_txt( file_handle, startpos, SEEK_SET_TXT);
            txtlength = fread_txt(file_handle, buff, searchsize);

            //greedily find the start byte of a character
            for(searchindex=0; searchindex<txtlength; searchindex++){
                if( (buff[searchindex]<0x80) && (buff[searchindex]>0x00) && (searchindex+1)<txtlength ){
                    startpos += (searchindex+1);
                    break;
                }
            }
            //greedily find the /r /n
            for(searchindex=startpos; searchindex<txtlength; searchindex++){
                if( (buff[searchindex]==0x0d) && (buff[searchindex+1]==0x0a) && (searchindex+2)<txtlength ){
                    startpos += (searchindex+2);
                    break;
                }
            }
        }
        else{
            startpos = 0;
        }
        //使用循环链表找到准确的上一页起始点
        int GBK_length = ((int)txtBookInst->curPageBytePosStart)-1-startpos;
        startpos = findPageStart_GBK(txtBookInst, file_handle, startpos, GBK_length, false);

        //read the page
        bookunfinish = readPageGBK(txtBookInst, file_handle, startpos, &txtlength);

        txtBookInst->curPageBytePosStart = startpos+1;
        txtBookInst->curPageBytePosEnd = startpos+txtlength;
        if ( txtBookInst->fileSizeInBytes == 0 )
        {
            txtBookInst->curPageRatio = 0;
        }
        else
        {
            txtBookInst->curPageRatio = (float)txtBookInst->curPageBytePosStart/(float)txtBookInst->fileSizeInBytes;
        }
        return bookunfinish;
        break;

    case UCS2_LITTLE:
    case UCS2_BIG:
        //unicode 编码，所有字符占2BYTE，不需要担心查找正确的字符起始位置
        searchsize = 10*SAFE_STR_LEN;
        if( (txtBookInst->curPageBytePosStart) < (1+searchsize+2) ){
            startpos = 2;
        }else{
            startpos = (txtBookInst->curPageBytePosStart)-searchsize-1;
        }

        if(startpos > 2){
            fseek_txt( file_handle, startpos, SEEK_SET_TXT);
            txtlength = fread_txt(file_handle, buff, searchsize);

            //greedily find the character of new line
            if( txtBookInst->fmt == UCS2_BIG){
                for(searchindex=0; searchindex<txtlength; searchindex+=2){
                    if( (buff[searchindex]==0x00) && (buff[searchindex+1]==0x0d) && (searchindex+4)<txtlength ){
                        startpos += (searchindex+4);
                        break;
                    }
                }
            }
            else{
                for(searchindex=0; searchindex<txtlength; searchindex+=2){
                    if( (buff[searchindex]==0x0d) && (buff[searchindex+1]==0x00) && (searchindex+4)<txtlength ){
                        startpos += (searchindex+4);
                        break;
                    }
                }
            }
        }
        else{
            startpos = 2;
        }
        //使用循环链表找到更加合适的起始点
        int UNI_length = ((int)txtBookInst->curPageBytePosStart)-1-startpos;
        startpos = findPageStart_UNI(txtBookInst, file_handle, startpos, UNI_length, false);

        bookunfinish = readPageUNI(txtBookInst, file_handle, startpos, &txtlength);

        txtBookInst->curPageBytePosStart = startpos+1;
        txtBookInst->curPageBytePosEnd = startpos+txtlength;
        if ( txtBookInst->fileSizeInBytes == 0 )
        {
            txtBookInst->curPageRatio = 0;
        }
        else
        {
            txtBookInst->curPageRatio = (float)txtBookInst->curPageBytePosStart/(float)txtBookInst->fileSizeInBytes;
        }
        return bookunfinish;
        break;

    default:
        return false;
        break;
    }// end switch
    return false;
}

bool readNextPage( TxtFile *txtBookInst,  short file_handle ){

    unsigned int pagesize;
    bool bookunfinish;

    if(txtBookInst->curPageBytePosEnd >= txtBookInst->fileSizeInBytes){
    	return false;
    }

//    printf("stage1 enter switch, %c\n",  txtBookInst->fmt );
    switch( txtBookInst->fmt ){
    case GBK:
        //当前页的结束Byte位置，恰好为下一页Byte数组开始的index
        //bookunfinish = readPageGBK(txtBookInst, fp, txtBookInst->curPageBytePosEnd , &pagesize);
//    	printf("stage2 readGBK start %c\n",  txtBookInst->fmt );
    	bookunfinish = readPageGBK(txtBookInst, file_handle, txtBookInst->curPageBytePosEnd , &pagesize);
//    	printf("stage3 readGBK leave %c\n",  txtBookInst->fmt );
    	txtBookInst->curPageBytePosStart = (txtBookInst->curPageBytePosEnd)+1;
        txtBookInst->curPageBytePosEnd += pagesize;
        if ( txtBookInst->fileSizeInBytes == 0 )
        {
            txtBookInst->curPageRatio = 0;
        }
        else
        {
            txtBookInst->curPageRatio = (float)txtBookInst->curPageBytePosStart/(float)txtBookInst->fileSizeInBytes;
        }

        return bookunfinish;
        break;

    case UCS2_LITTLE:
    case UCS2_BIG:
        //当前页的结束Byte位置，恰好为下一页Byte数组开始的index
        //bookunfinish = readPageUNI(txtBookInst, fp, txtBookInst->curPageBytePosEnd , &pagesize);
    	bookunfinish = readPageUNI(txtBookInst, file_handle, txtBookInst->curPageBytePosEnd , &pagesize);
        txtBookInst->curPageBytePosStart = (txtBookInst->curPageBytePosEnd)+1;
        txtBookInst->curPageBytePosEnd += pagesize;
        if ( txtBookInst->fileSizeInBytes == 0 )
        {
            txtBookInst->curPageRatio = 0;
        }
        else
        {
            txtBookInst->curPageRatio = (float)txtBookInst->curPageBytePosStart/(float)txtBookInst->fileSizeInBytes;
        }

        return bookunfinish;
        break;

    default:
        break;
    }// end switch

    return false;
}

bool readPageByRatio( TxtFile *txtBookInst,  short file_handle, float position ){

    unsigned int approstar;
    unsigned int searchsize, startpos;
    bool bookunfinish;
    char buff[10*SAFE_STR_LEN];
    unsigned int searchindex, txtlength;
    approstar = (txtBookInst->fileSizeInBytes) * position;

    printf("float ratio = %f\n", position);

    if(position<0 || position >1){
        return false;
    }

    if( (txtBookInst->fmt == UCS2_BIG) || (txtBookInst->fmt == UCS2_LITTLE) ){
        approstar = approstar|0x00000001;
        // for unicode format, a character always starts at an odd position(counter by byte).
    }

    switch( txtBookInst->fmt ){
    case GBK:
        //read enough content to find an available start for new page
        searchsize = 10*SAFE_STR_LEN;
        if( approstar < (1+searchsize) ){
            startpos = 0;
        }else{
            startpos = approstar-searchsize-1;
        }

        if(startpos > 0){
            fseek_txt( file_handle, startpos, SEEK_SET_TXT);
            txtlength = fread_txt(file_handle, buff, searchsize);

            //greedily find the start byte of a character
            for(searchindex=0; searchindex<txtlength; searchindex++){
                if( (buff[searchindex]<0x80) && (buff[searchindex]>0x00) && (searchindex+1)<txtlength ){
                    searchindex += 1;
                    break;
                }
            }
            //greedily find the /r /n
            for(; searchindex<txtlength; searchindex++){
                if( (buff[searchindex]==0x0d) && (buff[searchindex+1]==0x0a) && (searchindex+2)<txtlength ){
                    startpos += (searchindex+2);
                    break;
                }
            }

        }
        else{
        	//start of txt can be regarded as a new line
            startpos = 0;
        }

        //使用循环链表找到准确的行起始点
        int GBK_length = ((int)approstar)-1-startpos;
        startpos = findPageStart_GBK(txtBookInst, file_handle, startpos, GBK_length, true);
        //read the page
        bookunfinish = readPageGBK(txtBookInst, file_handle, startpos, &txtlength);

        txtBookInst->curPageBytePosStart = startpos+1;
        txtBookInst->curPageBytePosEnd = startpos+txtlength;
        if ( txtBookInst->fileSizeInBytes == 0 )
        {
            txtBookInst->curPageRatio = 0;
        }
        else
        {
            txtBookInst->curPageRatio = (float)txtBookInst->curPageBytePosStart/(float)txtBookInst->fileSizeInBytes;
        }
        return bookunfinish;
        break;

    case UCS2_LITTLE:
    case UCS2_BIG:
        searchsize = 10*SAFE_STR_LEN;
        //unicode 编码，所有字符占2BYTE，不需要担心查找合适的字符起始位置
        if( approstar < (1+2+searchsize) ){
            startpos = 2;
        }else{
            startpos = approstar-1-searchsize;
        }

        printf("approx start= %d, search start = %d, searchsize = %d\n",approstar, startpos, searchsize);

        if(startpos > 2){
            fseek_txt( file_handle, startpos, SEEK_SET_TXT);
            txtlength = fread_txt(file_handle, buff, searchsize);
            printf("txtlength = %d\n", txtlength);

            //debug
//            displayArray( buff, txtlength );

            //greedily find the character of new line
            if( txtBookInst->fmt == UCS2_BIG){
                 printf("fmt = BIG\n");
                for(searchindex=0; searchindex<txtlength; searchindex+=2){
                    if( (buff[searchindex]==0x00) && (buff[searchindex+1]==0x0d) && (searchindex+4)<txtlength ){
                        startpos += (searchindex+4);
                        printf("find newline character at %d\n", searchindex);
                        break;
                    }
                }
            }
            else{
                printf("fmt = LITTLE\n");
                for(searchindex=0; searchindex<txtlength; searchindex+=2){
                    if( (buff[searchindex]==0x0d) && (buff[searchindex+1]==0x00) && (searchindex+4)<txtlength ){
                        startpos += (searchindex+4);
                        break;
                    }
                }
            }
        }
        else{
            startpos = 2;
        }

        //使用循环链表找到准确的行起始点
        int UNI_length = ((int)approstar)-1-startpos;
        printf("UNILENGTH = %d, starpos = %d\n", UNI_length, startpos);

        //之后可能有问题***********************************先调试到这里

        startpos = findPageStart_UNI(txtBookInst, file_handle, startpos, UNI_length, true);

        bookunfinish = readPageUNI(txtBookInst, file_handle, startpos, &txtlength);

        txtBookInst->curPageBytePosStart = startpos+1;
        txtBookInst->curPageBytePosEnd = startpos+txtlength;
        if ( txtBookInst->fileSizeInBytes == 0 )
        {
            txtBookInst->curPageRatio = 0;
        }
        else
        {
            txtBookInst->curPageRatio = (float)txtBookInst->curPageBytePosStart/(float)txtBookInst->fileSizeInBytes;
        }

        break;

    default:
        return false;
        break;
    }// end switch


    return false;
}

unsigned int findPageStart_UNI( TxtFile *txtBookInst, short file_handle, unsigned int startpos, int UNI_length, bool line){
	if(txtBookInst->fmt == UCS2_BIG){
		return findPageStart_UNI_BIG( txtBookInst,  file_handle,  startpos, UNI_length, line );
	}else{
		return findPageStart_UNI_LITTLE( txtBookInst, file_handle, startpos, UNI_length, line );
	}
}

unsigned int findPageStart_GBK( TxtFile *txtBookInst, short file_handle, unsigned int startpos, int GBK_length, bool line){

    if(GBK_length < 1){
        return startpos;
    }
    //initial
    int j;
    unsigned int temp[BOOK_COL_NUM];
    for(j=0; j< BOOK_COL_NUM;j++){
    	temp[j] = 0;
    }

    int linecounter = 0; // the index of line in present page
    unsigned int txtIndex = 0;
    int lineindex = 0;
    int blanknum = 0;
    int step, i, txtlength;
    char ch;
    char buff[GBK_length];
    fseek_txt( file_handle, startpos, SEEK_SET_TXT);
    txtlength = fread_txt(file_handle, buff, GBK_length);

    while( txtIndex<txtlength ){
        ch = buff[txtIndex];

        if( ch < 0){
            step = 2;
            if((lineindex + step)>BOOK_ROW_BYTES ){ // the line has not enough space
                //need a new line
                linecounter++;
                lineindex = 0;
                if(linecounter >= BOOK_COL_NUM){
                    linecounter = 0;
                }
                temp[linecounter] = txtIndex;
            }
            txtIndex += step; lineindex += step;
        }
        else{
            // currently we meet an ASCII character
            switch(ch){
            case 0x20://' '
                step = 1;
                if((lineindex + step)>BOOK_ROW_BYTES ){// the line has not enough space
                    linecounter++;
                    lineindex = 0;
                    if( linecounter >= BOOK_COL_NUM){
                        linecounter = 0;
                    }
                    temp[linecounter] = txtIndex;
                }
                else{// the line the line has enough space
                    txtIndex++; lineindex++;
                }
                break;

            case 0x0d:// has to create a new line
                txtIndex += 2;
                linecounter++; lineindex = 0;
                if( linecounter >= BOOK_COL_NUM){
                    linecounter = 0;
                }
                temp[linecounter] = txtIndex;
                break;

            case 0x0a:
                txtIndex += 1;
                linecounter++; lineindex = 0;
                if( linecounter >= BOOK_COL_NUM){
                    linecounter = 0;
                }
                temp[linecounter] = txtIndex;
                break;

            case 0x09://'\t'
                step = 4;
                blanknum = step;
                if((lineindex + step)>BOOK_ROW_BYTES ){// the line has not enough space
                	blanknum = lineindex + step - BOOK_ROW_BYTES;
                    linecounter++;
                    lineindex = 0;
                    if( linecounter >= BOOK_COL_NUM){
                        linecounter = 0;
                    }
                    temp[linecounter] = txtIndex;
                }
                for(j = 0; j < blanknum; j ++){
                	lineindex ++;
                }// fix the blank, read the '\t'
                txtIndex ++;

                break;

            default:
                // currently we may meet an ANSI character
                // we must make sure that English words will not be split into two lines
                if( isEnglishletter(ch) ){// if: get an English letter
                    step = 1;
                    if((step+txtIndex+1)<txtlength){
                    	 for(;1;step++){
							ch = buff[txtIndex + step];
							if( (!isEnglishletter(ch)) || ((step+txtIndex+1)>=txtlength) ){
								break;
							}
                    	}
                    }

                    if((lineindex + step)<=BOOK_ROW_BYTES){// don not need a new line
                        lineindex += step;
                        txtIndex += step;
                    }
                    else if( step <= BOOK_ROW_BYTES){//one line is enough to show the word
                        //need a new line
                        linecounter++;
                        lineindex = 0;
                        if( linecounter >= BOOK_COL_NUM){
                            linecounter = 0;
                        }
                        temp[linecounter] = txtIndex;
                    }
                    else{//one line is not enough to show the string
                        //we just push the letter into screen one by one
                        for(i = 0; i < step; i++){
                            if( lineindex >= BOOK_ROW_BYTES){
                                linecounter++;
                                lineindex = 0;
                                if( linecounter >= BOOK_COL_NUM){
                                    linecounter = 0;
                                }
                                temp[linecounter] = txtIndex;
                            }
                            txtIndex++; lineindex++;
                        }//end for: store the long string

                    }//end else:store the long string

                }//end if: get an English letter
                else{// else: get a not English letter
                    step = 1;
                    if((lineindex + step)>BOOK_ROW_BYTES ){
                        //need a new line
                        linecounter++;
                        lineindex = 0;
                        if( linecounter >= BOOK_COL_NUM){
                            linecounter = 0;
                        }
                        temp[linecounter] = txtIndex;
                    }
                    txtIndex++; lineindex++;
                }//end else: get an ANSI character but not a English letter
            }//end of switch: get an ANSI character
        }//end of else: get an ANSI character
    }// end of while: get character

    if( line ){
        return (temp[linecounter]+startpos);
    }

    if( linecounter >= (BOOK_COL_NUM-1)){
        return (temp[0] + startpos);
    }

    return  (temp[linecounter+1] + startpos);
}

unsigned int findPageStart_UNI_BIG( TxtFile *txtBookInst, short file_handle, unsigned int startpos, int UNI_length, bool line){

    if( (startpos&0x00000001)>0 ){// make sure start from a even-index byte
        startpos++;
        UNI_length--;
    }

    if(UNI_length < 1){
        return startpos;
    }
    //initial
    int j;
    unsigned int temp[BOOK_COL_NUM];
    for(j=0; j< BOOK_COL_NUM;j++){
    	temp[j] = 0;
    }

    int linecounter = 0; // the index of line in present page
    unsigned int txtIndex = 0;
    int lineindex = 0;
    int linetaken = 0;
    int blanknum = 0;
    int step, i, txtlength;
    char ch1, ch2;
    char buff[UNI_length];

    printf( "startpos = %u\n", startpos );
    fseek_txt( file_handle, startpos, SEEK_SET_TXT);
    txtlength = fread_txt(file_handle, buff, UNI_length);
    //debug
//    displayArray( buff, txtlength );

    if( txtlength>0 ){
        if((txtlength&0x00000001)>0){
            txtlength--;
        }
    }

    while( txtIndex<txtlength ){
        ch1 = buff[txtIndex];
        ch2 = buff[txtIndex+1];

        if( (ch1==0x00)&(ch2<0x80) ){
            // currently we meet an ASCII character
            switch(ch2){
            case 0x20://' '
                step = 1;
                if((linetaken + step)>BOOK_ROW_BYTES ){
                    //need a new line
                    linecounter++;
                    lineindex = 0; linetaken = 0;
                    if( linecounter >= BOOK_COL_NUM){
                        linecounter = 0;
                    }
                    temp[linecounter] = txtIndex;
                }

                txtIndex+=2; lineindex+=2; linetaken+=1;
                break;

            case 0x0d://change line
                txtIndex += 4;
                linecounter++; lineindex = 0; linetaken = 0;
                if( linecounter >= BOOK_COL_NUM){
                    linecounter = 0;
                }
                temp[linecounter] = txtIndex;
                break;

            case 0x09://\t
                step = 4;
                blanknum = step;
                if((linetaken + step)>BOOK_ROW_BYTES ){
                    //need a new line
                    blanknum = linetaken + step - BOOK_ROW_BYTES;
                    linecounter++;
                    lineindex = 0; linetaken = 0;
                    if( linecounter >= BOOK_COL_NUM){
                        linecounter = 0;
                    }
                    temp[linecounter] = txtIndex;
                }

                for(j = 0; j < blanknum; j ++){
                	linetaken++;
                    lineindex+=2;
                }
                txtIndex+=2;
                break;

            default:
                // currently we may meet an ASCII character
                // we must make sure that English words will not be split into two lines
                if( isUNIEnglishletter(ch1, ch2) ){
                    step = 1;
                    for(;1;step++){
                        ch1 = buff[txtIndex + 2*step];
                        ch2 = buff[txtIndex + 2*step + 1];
                        if( (!isUNIEnglishletter(ch1, ch2)) | ((txtIndex + 2*step + 2)>=txtlength) ){
                            break;
                        }
                    }

                    if((linetaken + step)<=BOOK_ROW_BYTES){
                        // don not need a new line
                        lineindex += 2*step;
                        linetaken += step;
                        txtIndex += 2*step;
                    }
                    else if( step <= BOOK_ROW_BYTES){
                        //need a new line
                        //one line is enough to show the word
                        linecounter++;
                        lineindex = 0;
                        linetaken = 0;
                        if( linecounter >= BOOK_COL_NUM){
                            linecounter = 0;
                        }
                        temp[linecounter] = txtIndex;
                    }
                    else{
                        //one line is not enough to show the string
                        //we just push the letter into screen one by one
                        for(i = 0; i < step; i++){
                            if( linetaken >= BOOK_ROW_BYTES){
                                linecounter++;
                                lineindex = 0;
                                linetaken = 0;
                                if( linecounter >= BOOK_COL_NUM){
                                    linecounter = 0;
                                }
                                temp[linecounter] = txtIndex;
                            }
                            txtIndex+=2; lineindex+=2; linetaken+=1;
                        }//end for: store the long string
                    }//end else:store the long string
                }//end if: get an English letter
                else{//else: get an ASCII character but not a English letter

                    step = 1;
                    if((linetaken + step)>BOOK_ROW_BYTES ){
                        //need a new line
                        linecounter++;
                        lineindex = 0; linetaken = 0;
                        if( linecounter >= BOOK_COL_NUM){
                            linecounter = 0;
                        }
                        temp[linecounter] = txtIndex;
                    }
                    txtIndex+=2; lineindex+=2; linetaken+=1;
                }//end else: get an ASCII character but not a English letter

            }//end of switch: get an ASCII character
        }//end if: get an ASCII character
        else{// get a not-ASCII character
            step = 2;
            if((linetaken + step)>BOOK_ROW_BYTES ){
                //need a new line
                linecounter++;
                lineindex = 0; linetaken = 0;
                if( linecounter >= BOOK_COL_NUM){
                    linecounter = 0;
                }
                temp[linecounter] = txtIndex;
            }
            txtIndex+=2; lineindex+=2; linetaken+=2;
        }// end of else: get a not-ASCII character

    }// end of while: get character

    if( line ){
        return (temp[linecounter]+startpos);
    }

    if( linecounter >= (BOOK_COL_NUM-1)){
        return (temp[0] + startpos);
    }

    return  (temp[linecounter+1] + startpos);

}

unsigned int findPageStart_UNI_LITTLE( TxtFile *txtBookInst, short file_handle, unsigned int startpos, int UNI_length, bool line){

    if( (startpos&0x00000001)>0 ){// make sure start from a even-index byte
        startpos++;
        UNI_length--;
    }

    if(UNI_length < 1){
        return startpos;
    }
    //initial
    int j;
    unsigned int temp[BOOK_COL_NUM];
    for(j=0; j< BOOK_COL_NUM;j++){
    	temp[j] = 0;
    }

    int linecounter = 0; // the index of line in present page
    unsigned int txtIndex = 0;
    int lineindex = 0;
    int linetaken = 0;
    int blanknum = 0;
    int step, i, txtlength;
    char ch1, ch2;
    char buff[UNI_length];

    fseek_txt( file_handle, startpos, SEEK_SET_TXT);
    txtlength = fread_txt(file_handle, buff, UNI_length);

    if( txtlength>0 ){
        if((txtlength&0x00000001)>0){
            txtlength--;
        }
    }

    while( txtIndex<txtlength ){
        ch2 = buff[txtIndex];
        ch1 = buff[txtIndex+1];

        if( (ch1==0x00)&(ch2<0x80) ){
            // currently we meet an ASCII character
            switch(ch2){
            case 0x20://' '
                step = 1;
                if((linetaken + step)>BOOK_ROW_BYTES ){
                    //need a new line
                    linecounter++;
                    lineindex = 0; linetaken = 0;
                    if( linecounter >= BOOK_COL_NUM){
                        linecounter = 0;
                    }
                    temp[linecounter] = txtIndex;
                }

                txtIndex+=2; lineindex+=2; linetaken+=1;
                break;

            case 0x0d://change line
                txtIndex += 4;
                linecounter++; lineindex = 0; linetaken = 0;
                if( linecounter >= BOOK_COL_NUM){
                    linecounter = 0;
                }
                temp[linecounter] = txtIndex;
                break;

            case 0x09://\t
                step = 4;
                blanknum = step;
                if((linetaken + step)>BOOK_ROW_BYTES ){
                    //need a new line
                    blanknum = linetaken + step - BOOK_ROW_BYTES;
                    linecounter++;
                    lineindex = 0; linetaken = 0;
                    if( linecounter >= BOOK_COL_NUM){
                        linecounter = 0;
                    }
                    temp[linecounter] = txtIndex;
                }

                for(j = 0; j < blanknum; j ++){
                	linetaken++;
                    lineindex+=2;
                }
                txtIndex+=2;
                break;

            default:
                // currently we may meet an ASCII character
                // we must make sure that English words will not be split into two lines
                if( isUNIEnglishletter(ch1, ch2) ){
                    step = 1;
                    for(;1;step++){
                        ch1 = buff[txtIndex + 2*step];
                        ch2 = buff[txtIndex + 2*step + 1];
                        if( (!isUNIEnglishletter(ch1, ch2)) | ((txtIndex + 2*step + 2)>=txtlength) ){
                            break;
                        }
                    }

                    if((linetaken + step)<=BOOK_ROW_BYTES){
                        // don not need a new line
                        lineindex += 2*step;
                        linetaken += step;
                        txtIndex += 2*step;
                    }
                    else if( step <= BOOK_ROW_BYTES){
                        //need a new line
                        //one line is enough to show the word
                        linecounter++;
                        lineindex = 0;
                        linetaken = 0;
                        if( linecounter >= BOOK_COL_NUM){
                            linecounter = 0;
                        }
                        temp[linecounter] = txtIndex;
                    }
                    else{
                        //one line is not enough to show the string
                        //we just push the letter into screen one by one
                        for(i = 0; i < step; i++){
                            if( linetaken >= BOOK_ROW_BYTES){
                                linecounter++;
                                lineindex = 0;
                                linetaken = 0;
                                if( linecounter >= BOOK_COL_NUM){
                                    linecounter = 0;
                                }
                                temp[linecounter] = txtIndex;
                            }
                            txtIndex+=2; lineindex+=2; linetaken+=1;
                        }//end for: store the long string
                    }//end else:store the long string
                }//end if: get an English letter
                else{//else: get an ASCII character but not a English letter

                    step = 1;
                    if((linetaken + step)>BOOK_ROW_BYTES ){
                        //need a new line
                        linecounter++;
                        lineindex = 0; linetaken = 0;
                        if( linecounter >= BOOK_COL_NUM){
                            linecounter = 0;
                        }
                        temp[linecounter] = txtIndex;
                    }
                    txtIndex+=2; lineindex+=2; linetaken+=1;
                }//end else: get an ASCII character but not a English letter

            }//end of switch: get an ASCII character
        }//end if: get an ASCII character
        else{// get a not-ASCII character
            step = 2;
            if((linetaken + step)>BOOK_ROW_BYTES ){
                //need a new line
                linecounter++;
                lineindex = 0; linetaken = 0;
                if( linecounter >= BOOK_COL_NUM){
                    linecounter = 0;
                }
                temp[linecounter] = txtIndex;
            }
            txtIndex+=2; lineindex+=2; linetaken+=2;
        }// end of else: get a not-ASCII character

    }// end of while: get character

    if( line ){
        return (temp[linecounter]+startpos);
    }

    if( linecounter >= (BOOK_COL_NUM-1)){
        return (temp[0] + startpos);
    }
    return  (temp[linecounter+1] + startpos);

}

bool isEnglishletter( char ch){
    return ((ch>0x40)&(ch<0x5b)) | ((ch>0x60)&(ch<0x7b));
}

bool isUNIEnglishletter( char ch1, char ch2){
    return ( (ch1==0x00) & isEnglishletter(ch2) );
}
