/*
 * sdread.c
 *
 *  Created on: 2019��5��26��
 *      Author: 12757
 */


#include <errno.h>
#include <io.h>
#include <stdio.h>
#include <string.h>
#include "sdread.h"
#include "txt_read.h"
#include "stdlib.h"


//�Զ��庯��

//��ȡ�ı����ݣ���ȡsize���ֽڣ����뻺��buffer,����ֵΪ��ʵ��ȡ���ֽ���
int fread_txt( short file_handle, char *buffer, int numInByte )
{
	//У��file_handle
	if ( file_handle < 0 )
	{
		return 0;
	}

	return fread( buffer, 1, numInByte, txtFilesInfoSpace.fp );
}

//�ı�ָ��ƫ��(����0,�ɹ��� ����-1��ʧ��)
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


//��ȡ��ǰ��ȡ�ļ���λ��(����ֵ)
long ftell_txt( short file_handle )
{
	//У��file_handle
	if ( file_handle < 0 )
	{
		return -1;
	}

	return ftell( txtFilesInfoSpace.fp );
}


//���ļ�
//input: fileReadName( ���� .TXT )
//Output: file_handle
//An index to the file record assigned to the specified file.
//-1 is returned if the file could not be opened.
//Return -2 if the specified file has already been opened previously.
short fopen_txt( char *fileReadName )
{
	FILE *tempfp = fopen( fileReadName, "rb" );
	txtFilesInfoSpace.fp = tempfp;

	if ( tempfp != NULL )//���ļ��ɹ�
    {
        return 0;
    }
    else
    {
        return -1;
    }

}


//�ر��ļ�
//input: file_handle
//output: 0:�ɹ� -1:ʧ��(���ܹر���δ�򿪵��ļ�)
int fclose_txt( short file_handle )
{
	int status = fclose( txtFilesInfoSpace.fp );
	txtFilesInfoSpace.fp = NULL;

	return status;
}
//��ȡ�ļ���С
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
