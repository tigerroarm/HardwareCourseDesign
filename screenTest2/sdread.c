/*
 * sdread.c
 *
 *  Created on: 2019年5月26日
 *      Author: 12757
 */


#include <errno.h>
#include <io.h>
#include <stdio.h>
#include <string.h>
#include "sdread.h"
#include "txt_read.h"
#include "stdlib.h"


//自定义函数

//读取文本内容，读取size个字节，放入缓存buffer,返回值为真实读取的字节数
int fread_txt( short file_handle, char *buffer, int numInByte )
{
	//校验file_handle
	if ( file_handle < 0 )
	{
		return 0;
	}

	return fread( buffer, 1, numInByte, txtFilesInfoSpace.fp );
}

//文本指针偏移(返回0,成功； 返回-1，失败)
short fseek_txt( short file_handle, int offset, enum fseekType fromwhere )
{
	if ( file_handle < 0 )
	{
		return 0;
	}

	int status = fseek( txtFilesInfoSpace.fp, offset, (int)fromwhere );

	//debug
	printf( "current_byte_position = %ld\n", ftell_txt( file_handle ) );

	return status;
}


//获取当前读取文件的位置(返回值)
long ftell_txt( short file_handle )
{
	//校验file_handle
	if ( file_handle < 0 )
	{
		return -1;
	}

	return ftell( txtFilesInfoSpace.fp );
}


//打开文件
//input: fileReadName( 包含 .TXT )
//Output: file_handle
//An index to the file record assigned to the specified file.
//-1 is returned if the file could not be opened.
//Return -2 if the specified file has already been opened previously.
short fopen_txt( char *fileReadName )
{
	FILE *tempfp = fopen( fileReadName, "rb" );
	txtFilesInfoSpace.fp = tempfp;

	if ( tempfp != NULL )//打开文件成功
    {
        return 0;
    }
    else
    {
        return -1;
    }

}


//关闭文件
//input: file_handle
//output: 0:成功 -1:失败(可能关闭了未打开的文件)
int fclose_txt( short file_handle )
{
	int status = fclose( txtFilesInfoSpace.fp );
	txtFilesInfoSpace.fp = NULL;

	return status;
}
//获取文件大小
int getfileSizeTxt( short file_handle )
{
	if ( file_handle < 0 )
	{
		return -1;
	}
	else
    {
        fseek_txt( file_handle, 0, SEEK_END_TXT );
        return ftell_txt( file_handle );
    }
    return 0;
}
