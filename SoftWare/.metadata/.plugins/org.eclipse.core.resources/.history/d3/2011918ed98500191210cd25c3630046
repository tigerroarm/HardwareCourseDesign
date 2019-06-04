/*
 * sdread.h
 *
 *  Created on: 2019年5月26日
 *      Author: 12757
 */

#ifndef SDREAD_H_
#define SDREAD_H_

#include "Altera_UP_SD_Card_Avalon_Interface.h"


//读取文本内容，读取size个字节，放入缓存buffer,返回值为真实读取的字节数
int fread_txt( short file_handle, char *buffer, int numInByte );
//fseek_txt的fromwhere类型
enum fseekType { SEEK_SET_TXT=0, SEEK_CUR_TXT=1, SEEK_END_TXT=2 };
//文本指针偏移(返回0,成功； 返回-1，失败)
short fseek_txt( short file_handle, int offset, enum fseekType fromwhere );
//获取当前读取文件的位置(返回值)
long ftell_txt( short file_handle );
//打开文件
//input: fileReadName( 包含 .TXT )
//Output: file_handle
//An index to the file record assigned to the specified file.
//-1 is returned if the file could not be opened.
//Return -2 if the specified file has already been opened previously.
short fopen_txt( char *fileReadName );
//关闭文件
//input: file_handle
//output: 0:成功 -1:失败(可能关闭了未打开的文件)
int fclose_txt( short file_handle );
//获取文件大小
int getfileSizeTxt( short file_handle );

#endif /* SDREAD_H_ */
