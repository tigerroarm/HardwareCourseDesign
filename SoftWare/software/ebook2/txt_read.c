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
#include "sdread.h"
#include "system.h"

//��ʱ�洢��SD���ж������ļ���
char sdCardFileName[TXT_FILE_NAME_SIZE];

TxtFilesInfo txtFilesInfoSpace;


char txtBookSpace[BOOK_COL_NUM][(BOOK_ROW_BYTES+1)*2] = {
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

				curTxtFile->curPageBytePosStart = 0;
				curTxtFile->curPageBytePosEnd = 0;
				curTxtFile->fileSizeInBytes = 0;
				curTxtFile->encodingFmt = 'g';
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

    TxtFile *txtFile = txtFilesInfoSpace.txtFileList + txtFilesInfoSpace.curOpenFileIndex;

    //���ô��ļ�����


	char *fileReadName = txtFile->txtFileReadName.text;
    short file_handle = fopen_txt( fileReadName );
    printf( "fileReadName = %s\n", fileReadName );
    printf( "file_handle = %d\n", file_handle );
    txtFile->fileSizeInBytes = getfileSizeTxt( file_handle );
    printf( "fileSizeInBytes = %d\n", txtFile->fileSizeInBytes );

    //�Ƚ���������Ϊ��һҳ
    if ( txtFile->curPageBytePosStart == 0 )
    {
    	txtFile->curPageBytePosEnd = 0;
    }
    else
    {
    	txtFile->curPageBytePosEnd = txtFile->curPageBytePosStart-1;
    }

    //�򿪵�ǰҳ��ͨ������һҳ����һҳ��
    readBookOfTxtFile( txtFile, BOOK_PAGE_NEXT, 0 );
    printf( "readBookOfTxtFile finish\n" );

    return txtFilesInfoSpace.txtFileList + fileIndex;
}

//�ر�TxtFile�ļ�
bool closeTxtFile( )
{
    int status = fclose_txt( txtFilesInfoSpace.curFileHandle );
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

    printf(" txtlength = %d, SBL = %d \n", txtlength, SAFE_STR_LEN);
    printf("*%d*\n", (int)buff[379]);
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

                for(lineindex = 0; lineindex < blanknum; lineindex ++){
                	txtBookSpace[linecounter][lineindex] = 0x20;
                }// fix the blank, read the '\t'

                txtIndex ++;
                break;

            default:
                // currently we may meet an ANSI character
                // we must make sure that English words will not be split into two lines
                if( isEnglishletter(ch) ){
                    step = 1;


                    if((step+txtIndex+1)<SAFE_STR_LEN){
                    	 for(;1;step++){
							ch = buff[txtIndex + step];
							if( (!isEnglishletter(ch)) || ((step+txtIndex+1)>=SAFE_STR_LEN) ){
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
    printf("quit outer while\n");
    *pagesize = txtIndex;
    return  ((startpos + txtIndex) < txtBookInst->fileSizeInBytes);

}

bool readPageUNI( TxtFile *txtBookInst,  short file_handle, unsigned int startpos, unsigned int* pagesize ){
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
                txtIndex += 2;
                linecounter++; lineindex = 0; linetaken = 0;
                if( linecounter >= BOOK_COL_NUM){
                    *pagesize = txtIndex;
                    return  ((startpos + txtIndex) < txtBookInst->fileSizeInBytes);
                }
                break;

            case 0x09://\t
                step = 4;
                if((linetaken + step)>BOOK_ROW_BYTES ){
                    //need a new line
                	txtBookSpace[linecounter][lineindex] = 0x00;
                	txtBookSpace[linecounter][lineindex+1] = 0x00;
                    linecounter++;
                    lineindex = 0;
                    if( linecounter >= BOOK_COL_NUM){
                        *pagesize = txtIndex;
                        return  ((startpos + txtIndex) < txtBookInst->fileSizeInBytes);
                    }
                }
                blanknum = linetaken + step - BOOK_ROW_BYTES;
                lineindex = 0; linetaken = 0;
                for(; linetaken < blanknum; linetaken ++){
                	txtBookSpace[linecounter][lineindex] = 0x00;
                	txtBookSpace[linecounter][lineindex+1] = 0x20;
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
                        if( (!isUNIEnglishletter(ch1, ch2)) | ((txtIndex + 2*step + 2)>=2*SAFE_STR_LEN) ){
                            break;
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

    *pagesize = txtIndex;
    return  ((startpos + txtIndex) < txtBookInst->fileSizeInBytes);

}

bool readLastPage( TxtFile *txtBookInst,  short file_handle ){
    unsigned int searchsize, startpos;
    bool bookunfinish;
    char buff[2*SAFE_STR_LEN];
    unsigned int searchindex, txtlength;

    if( (txtBookInst->curPageBytePosStart <= 1) && (txtBookInst->encodingFmt == 'g' ) ){
    	return false;
    }

    if( (txtBookInst->curPageBytePosStart <= 3) && (txtBookInst->encodingFmt == 'u' ) ){
    	return false;
    }

    switch( txtBookInst->encodingFmt ){
    case 'g':
        //read enough content to find an available start for new page
        searchsize = SAFE_STR_LEN;
        if( (txtBookInst->curPageBytePosStart) < (1+searchsize) ){
            startpos = 0;
        }else{
            startpos = (txtBookInst->curPageBytePosStart)-searchsize-1;
        }

//        fseek(fp, startpos, SEEK_SET);
//        txtlength = fread(buff, sizeof(char), searchsize, fp);
        fseek_txt( file_handle, startpos, SEEK_SET_TXT);
        txtlength = fread_txt(file_handle, buff, searchsize);


        //find the start
        for(searchindex=0; searchindex<txtlength; searchindex++){
            if( (buff[searchindex]== 0x0d) && (searchindex+2)<txtlength ){
                startpos += (searchindex+2);
                break;
            }
            if( (buff[searchindex]<0x80) && (buff[searchindex]>0x00) && (searchindex+1)<txtlength ){
                startpos += (searchindex+1);
                break;
            }
        }
        //read the page

        bookunfinish = readPageGBK(txtBookInst, file_handle, startpos, &txtlength);

        txtBookInst->curPageBytePosStart = startpos+1;
        txtBookInst->curPageBytePosEnd = startpos+txtlength;
        txtBookInst->curPageRatio = (float)txtBookInst->curPageBytePosStart/(float)txtBookInst->fileSizeInBytes;
        return bookunfinish;
        break;

    case 'u':
        //unicode ���룬�����ַ�ռ2BYTE������Ҫ���Ĳ��Һ��ʵ���ʼλ��
        searchsize = 2*SAFE_STR_LEN;
        if( (txtBookInst->curPageBytePosStart) < (1+searchsize+2) ){
            startpos = 2;
        }else{
            startpos = (txtBookInst->curPageBytePosStart)-searchsize-1;
        }

        bookunfinish = readPageUNI(txtBookInst, file_handle, startpos, &txtlength);

        txtBookInst->curPageBytePosStart = startpos+1;
        txtBookInst->curPageBytePosEnd = startpos+txtlength;
        txtBookInst->curPageRatio = (float)txtBookInst->curPageBytePosStart/(float)txtBookInst->fileSizeInBytes;
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

    printf("stage1 enter switch, %c\n",  txtBookInst->encodingFmt );
    switch( txtBookInst->encodingFmt ){
    case 'g':
        //��ǰҳ�Ľ���Byteλ�ã�ǡ��Ϊ��һҳByte���鿪ʼ��index
        //bookunfinish = readPageGBK(txtBookInst, fp, txtBookInst->curPageBytePosEnd , &pagesize);
    	printf("stage2 readGBK start %c\n",  txtBookInst->encodingFmt );
    	bookunfinish = readPageGBK(txtBookInst, file_handle, txtBookInst->curPageBytePosEnd , &pagesize);
    	printf("stage3 readGBK leave %c\n",  txtBookInst->encodingFmt );
    	txtBookInst->curPageBytePosStart = (txtBookInst->curPageBytePosEnd)+1;
        txtBookInst->curPageBytePosEnd += pagesize;
        txtBookInst->curPageRatio = (float)txtBookInst->curPageBytePosStart/(float)txtBookInst->fileSizeInBytes;

        return bookunfinish;
        break;
    case 'u':
        //��ǰҳ�Ľ���Byteλ�ã�ǡ��Ϊ��һҳByte���鿪ʼ��index
        //bookunfinish = readPageUNI(txtBookInst, fp, txtBookInst->curPageBytePosEnd , &pagesize);
    	bookunfinish = readPageUNI(txtBookInst, file_handle, txtBookInst->curPageBytePosEnd , &pagesize);
        txtBookInst->curPageBytePosStart = (txtBookInst->curPageBytePosEnd)+1;
        txtBookInst->curPageBytePosEnd += pagesize;
        txtBookInst->curPageRatio = (float)txtBookInst->curPageBytePosStart/(float)txtBookInst->fileSizeInBytes;

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
    char buff[2*SAFE_STR_LEN];
    unsigned int searchindex, txtlength;
    approstar = (txtBookInst->fileSizeInBytes) * position;

    if(position<0 || position >1){
        return false;
    }

    if(txtBookInst->encodingFmt == 'u'){
        approstar = approstar|0x00000001;
        // for unicode format, a character always starts at an odd position(counter by byte).
    }

    switch( txtBookInst->encodingFmt ){
    case 'g':
        //read enough content to find an available start for new page
        searchsize = SAFE_STR_LEN;
        if( approstar < (1+searchsize) ){
            startpos = 0;
        }else{
            startpos = approstar-searchsize-1;
        }

//        fseek(fp, startpos, SEEK_SET);
//        txtlength = fread(buff, sizeof(char), searchsize, fp);
        fseek_txt( file_handle, startpos, SEEK_SET_TXT);
        txtlength = fread_txt(file_handle, buff, searchsize);

        //find the start
        for(searchindex=0; searchindex<txtlength; searchindex++){
            if( (buff[searchindex]== 0x0d) && (searchindex+2)<txtlength ){
                startpos += (searchindex+2);
                break;
            }
            if( (buff[searchindex]<0x80) && (buff[searchindex]>0x00) && (searchindex+1)<txtlength ){
                startpos += (searchindex+1);
                break;
            }
        }
        //read the page

        bookunfinish = readPageGBK(txtBookInst, file_handle, startpos, &txtlength);

        txtBookInst->curPageBytePosStart = startpos+1;
        txtBookInst->curPageBytePosEnd = startpos+txtlength;
        txtBookInst->curPageRatio = (float)txtBookInst->curPageBytePosStart/(float)txtBookInst->fileSizeInBytes;
        return bookunfinish;
        break;

    case 'u':
        //unicode ���룬�����ַ�ռ2BYTE������Ҫ���Ĳ��Һ��ʵ���ʼλ��
        searchsize = 2*SAFE_STR_LEN;
        if( (txtBookInst->curPageBytePosStart) < (1+searchsize+2) ){
            startpos = 2;
        }else{
            startpos = (txtBookInst->curPageBytePosStart)-searchsize-1;
        }

        bookunfinish = readPageUNI(txtBookInst, file_handle, startpos, &txtlength);

        txtBookInst->curPageBytePosStart = startpos+1;
        txtBookInst->curPageBytePosEnd = startpos+txtlength;
        txtBookInst->curPageRatio = (float)txtBookInst->curPageBytePosStart/(float)txtBookInst->fileSizeInBytes;

        break;

    default:
        return false;
        break;
    }// end switch


    return false;
}


bool isEnglishletter( char ch){
    return ((ch>0x40)&(ch<0x5b)) | ((ch>0x60)&(ch<0x7b));
}

bool isUNIEnglishletter( char ch1, char ch2){
    return ( (ch1==0x00) & isEnglishletter(ch2) );
}