#include <stdio.h>
#include <stdlib.h>

#include "screen.h"
#include "screen_display.h"
#include "test_value.h"

extern TxtFilesInfo txtFilesInfoSpace;

int main( void )
{

	bool status = true, status1;

	//1.��ȡ������SD���е�����txt�ļ����ļ���
	status = status && readAlltxtFilesOfSDcard( );

    displayStatus( "readAlltxtFilesOfSDcard", "main", status );

//    printf( "txtFilesInfoSpace = {\n" );
//    displayTxtFilesInfo( &txtFilesInfoSpace, 4 );
//    printf( "}\n" );

	//2.��ʼ��ScreenContainer��Ļ��ʾ��Ϣ�ṹ��
	status1 = screenInit();
	status = status && status1;

	displayStatus( "screenInit", "main", status );

//    printf( "tftlcdScreenInst = {\n" );
//    displayScreenContainer( &tftlcdScreenInst, 4 );
//    printf( "}\n" );

	//3.ִ����Ļ��ʾ����
	status1 = showScreen( );
	status = status && status1;

	displayStatus( "showScreen", "main", status );

	//4.������Ļ��������
	printf( "\nstore screen picture data in image.txt\nstart\n" );
	storeScreenArrayToFile( );
	printf( "finished\n" );

	return 0;
}
