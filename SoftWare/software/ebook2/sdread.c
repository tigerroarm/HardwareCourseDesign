/*
 * sdread.c
 *
 *  Created on: 2019年5月26日
 *      Author: 12757
 */


#include <errno.h>
#include <priv/alt_file.h>
#include <io.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "sdread.h"



//直接从Altera_UP_SD_Card_Avalon_Interface.c里复制的宏定义和结构体定义

///////////////////////////////////////////////////////////////////////////
// Local Define Statements
///////////////////////////////////////////////////////////////////////////

#define CHAR_TO_UPPER(ch)	((char) (((ch >= 'a') && (ch <= 'z')) ? ((ch-'a')+'A'): ch))

// Data Buffer Address
#define SD_CARD_BUFFER(base, x)			(base + x)
// 128-bit Card Identification Number
#define SD_CARD_CID(base, x)			(base + 0x0200 + x)
// 128-bit Card Specific Data Register
#define SD_CARD_CSD(base, x)			(base + 0x0210 + x)
// 32-bit Operating Conditions Register
#define SD_CARD_OCR(base)				(base + 0x0220)
// 32-bit Card Status Register
#define SD_CARD_STATUS(base)			(base + 0x0224)
// 16-bit Relative Card Address Register
#define SD_CARD_RCA(base)				(base + 0x0228)
// 32-bit Card Argument Register
#define SD_CARD_ARGUMENT(base)			(base + 0x022C)
// 16-bit Card Command Register
#define SD_CARD_COMMAND(base)			(base + 0x0230)
// 16-bit Card Auxiliary Status Register
#define SD_CARD_AUX_STATUS(base)		(base + 0x0234)
// 32-bit R1 Response Register
#define SD_CARD_R1_RESPONSE(base)		(base + 0x0238)

#define CMD_READ_BLOCK					17
#define CMD_WRITE_BLOCK					24

// FAT 12/16 related stuff
//#define BOOT_SECTOR_DATA_SIZE			0x005A
#define MAX_FILES_OPENED				20

/******************************************************************************/
/******  LOCAL DATA STRUCTURES  ***********************************************/
/******************************************************************************/



typedef struct s_FAT_12_16_boot_sector {
	unsigned char jump_instruction[3];
	char OEM_name[8];
	unsigned short int sector_size_in_bytes;
	unsigned char sectors_per_cluster;
	unsigned short int reserved_sectors;
	unsigned char number_of_FATs;
	unsigned short int max_number_of_dir_entires;
	unsigned short int number_of_sectors_in_partition;
	unsigned char media_descriptor;
	unsigned short int number_of_sectors_per_table;
	unsigned short int number_of_sectors_per_track;
	unsigned short int number_of_heads;
	unsigned int number_of_hidden_sectors;
	unsigned int total_sector_count_if_above_32MB;
	unsigned char drive_number;
	unsigned char current_head;
	unsigned char boot_signature;
	unsigned char volume_id[4];
	char volume_label[11];
	unsigned char file_system_type[8];
	unsigned char bits_for_cluster_index;
	unsigned int first_fat_sector_offset;
	unsigned int second_fat_sector_offset;
	unsigned int root_directory_sector_offset;
	unsigned int data_sector_offset;
} t_FAT_12_16_boot_sector;


typedef struct s_file_record {
	unsigned char name[8];
	unsigned char extension[3];
	unsigned char attributes;
	unsigned short int create_time;
	unsigned short int create_date;
	unsigned short int last_access_date;
	unsigned short int last_modified_time;
	unsigned short int last_modified_date;
	unsigned short int start_cluster_index;
	unsigned int file_size_in_bytes;
	/* The following fields are only used when a file has been created or opened. */
	unsigned int current_cluster_index;
    unsigned int current_sector_in_cluster;
	unsigned int current_byte_position;
    // Absolute location of the file record on the SD Card.
    unsigned int file_record_cluster;
    unsigned int file_record_sector_in_cluster;
    short int    file_record_offset;
    // Is this record in use and has the file been modified.
    unsigned int home_directory_cluster;
    bool         modified;
	bool		 in_use;
} t_file_record;


typedef struct s_find_data {
	unsigned int directory_root_cluster; // 0 means root directory.
	unsigned int current_cluster_index;
	unsigned int current_sector_in_cluster;
	short int file_index_in_sector;
	bool valid;
} t_find_data;

///////////////////////////////////////////////////////////////////////////
// extern Variables( 使用Altera_UP_SD_Card_Avalon_Interface.c中的变量 )
///////////////////////////////////////////////////////////////////////////


// Pointers to currently opened files.
extern t_file_record active_files[MAX_FILES_OPENED];
extern t_FAT_12_16_boot_sector boot_sector_data;
extern int fat_partition_offset_in_512_byte_sectors;
extern unsigned int current_sector_index;
extern volatile char *buffer_memory;
///////////////////////////////////////////////////////////////////////////
// Local Functions
///////////////////////////////////////////////////////////////////////////





//自定义函数


//读取文本内容，读取size个字节，放入缓存buffer,返回值为真实读取的字节数
int fread_txt( short file_handle, char *buffer, int numInByte )
{
	//校验file_handle
	if ( file_handle < 0 || file_handle >= MAX_FILES_OPENED )
	{
		return 0;
	}
	//该文件如果没有打开
	if ( !active_files[file_handle].in_use )
	{
		return 0;
	}

	//获得active_files[]对应的元素指针
	t_file_record *curActiveFile = active_files + file_handle;

	//numInByte如果超过了文本剩下的字节数，就进行限制
	if ( numInByte > (curActiveFile->file_size_in_bytes - curActiveFile->current_byte_position) )
	{
		numInByte = (curActiveFile->file_size_in_bytes - curActiveFile->current_byte_position);
	}

	//当前读取的字节数
	unsigned int bufferIndex = 0;

	//current_byte_position从0开始，file_size_in_bytes-1为读取最后一个，file_size_in_bytes说明读完了
	while ( curActiveFile->current_byte_position < curActiveFile->file_size_in_bytes && bufferIndex < numInByte )
	{
		int data_sector = boot_sector_data.data_sector_offset + \
				(curActiveFile->current_cluster_index - 2)*boot_sector_data.sectors_per_cluster + \
				 curActiveFile->current_sector_in_cluster;

		//当前读取的字节不为文本的第一个字节，而且正好位于新扇区，这样保证FAT表可以正常工作，链表首做特殊处理（即避开FAT表首）
		if ((curActiveFile->current_byte_position > 0) && ((curActiveFile->current_byte_position % 512) == 0))
		{
			// Read in a new sector of data.
			//如果读到了一个簇的结束，就读下一个簇
			if (curActiveFile->current_sector_in_cluster == boot_sector_data.sectors_per_cluster - 1)
			{
				// Go to the next cluster.
				unsigned short int next_cluster;
				if (get_cluster_flag(curActiveFile->current_cluster_index, &next_cluster))
				{
					if ((next_cluster & 0x0000fff8) == 0x0000fff8)
					{
						/* End of file */
						break;
					}
					else
					{
						//用新簇替换旧簇
						curActiveFile->current_cluster_index = next_cluster;
						//簇内扇区偏移为0
						curActiveFile->current_sector_in_cluster = 0;
						//重新计算data_sector
						data_sector = boot_sector_data.data_sector_offset + (curActiveFile->current_cluster_index - 2)*boot_sector_data.sectors_per_cluster +
						  curActiveFile->current_sector_in_cluster;
					}
				}
				else
				{
					//不支持除FAT16外的FAT表
					return 0;
				}
			}
			else//簇不变
			{
				curActiveFile->current_sector_in_cluster = curActiveFile->current_sector_in_cluster + 1;
				data_sector = data_sector + 1;
			}
		}

		//如果读取的扇区发生变化，就读新扇区。current_sector_index存储的是之前读取的扇区位置,
		//(data_sector + fat_partition_offset_in_512_byte_sectors)是现在要读取的扇区位置
		if (current_sector_index != (data_sector + fat_partition_offset_in_512_byte_sectors))
		{
			if (!Read_Sector_Data(data_sector, fat_partition_offset_in_512_byte_sectors))
			{
				return 0;
			}
		}
		//将读到的数据存入缓存
		int curByteOfSector = (curActiveFile->current_byte_position % 512);

		int i_max = curByteOfSector + numInByte - bufferIndex;
		if ( i_max > 512 )
		{
			i_max = 512;
		}

		int i;
		for ( i = curByteOfSector; i < i_max; i ++ )
		{
			//取出扇区中的值，返回
			buffer[bufferIndex] = (unsigned char) IORD_8DIRECT(buffer_memory, i );
			bufferIndex ++;
			//调整读取的下标
			curActiveFile->current_byte_position = curActiveFile->current_byte_position + 1;
		}
	}

	return bufferIndex;//返回实际读取的字节数
}

//文本指针偏移(返回0,成功； 返回-1，失败)
short fseek_txt( short file_handle, int offset, enum fseekType fromwhere )
{
	//校验file_handle
	if ( file_handle < 0 || file_handle >= MAX_FILES_OPENED )
	{
		return -1;
	}
	//该文件如果没有打开
	if ( !active_files[file_handle].in_use )
	{
		return -1;
	}

	int status = 0;

	//获得active_files[]对应的元素指针
	t_file_record *curActiveFile = active_files + file_handle;
	unsigned char sectors_per_cluster = boot_sector_data.sectors_per_cluster;
	//之前的簇数
	int preCluster = ((curActiveFile->current_byte_position) >> 9) / sectors_per_cluster;


	//current_byte_position从0开始，file_size_in_bytes-1为读取最后一个，file_size_in_bytes说明读完了
	int currentByte = 0;

	switch( fromwhere )
	{
		case SEEK_SET_TXT://移到文件开头
			currentByte = offset;
			break;

		case SEEK_CUR_TXT://当前位置偏移
			currentByte = curActiveFile->current_byte_position + offset;
			break;

		case SEEK_END_TXT://移到文件结尾
			currentByte = curActiveFile->file_size_in_bytes + offset;
			break;
		default:
			return -1;
	}

	if ( currentByte > curActiveFile->file_size_in_bytes )
	{
		curActiveFile->current_byte_position = curActiveFile->file_size_in_bytes;
	}
	else if ( currentByte < 0 )
	{
		curActiveFile->current_byte_position = 0;
	}
	else
	{
		curActiveFile->current_byte_position = currentByte;
	}

	//计算簇数、扇区数
	int curSector = (curActiveFile->current_byte_position) >> 9;
	int curCluster = curSector / sectors_per_cluster;
	int sectorInCluster = curSector % sectors_per_cluster;
	curActiveFile->current_sector_in_cluster = sectorInCluster;

	//根据簇数查找FAT表
	int fatClusterNum = 0;//查找fat表的次数(延着链表向后找)
	if ( curSector < preCluster )//FAT表需要查找上一个簇，则只能从FAT表首重新找
	{
		curActiveFile->current_cluster_index = curActiveFile->start_cluster_index;
		fatClusterNum = curCluster;
	}
	else//直接向后找
	{
		fatClusterNum = curCluster - preCluster;
	}

	//FAT表查找簇
	int i;
	unsigned short int next_cluster;
	for ( i = 0; i < fatClusterNum; i ++ )
	{
		if (get_cluster_flag(curActiveFile->current_cluster_index, &next_cluster))
		{
			if ((next_cluster & 0x0000fff8) == 0x0000fff8)
			{
				/* End of file */
				break;
			}
			else
			{
				//用新簇替换旧簇
				curActiveFile->current_cluster_index = next_cluster;
			}
		}
		else
		{
			//不支持除FAT16外的FAT表
			return -1;
		}
	}
	return 0;
}


//获取当前读取文件的位置(返回值)
long ftell_txt( short file_handle )
{
	//校验file_handle
	if ( file_handle < 0 || file_handle >= MAX_FILES_OPENED )
	{
		return -1;
	}
	//该文件如果没有打开
	if ( !active_files[file_handle].in_use )
	{
		return -1;
	}
	//返回当前读取字节数
	return active_files[file_handle].current_byte_position;
}


//打开文件
//input: fileReadName( 包含 .TXT )
//Output: file_handle
//An index to the file record assigned to the specified file.
//-1 is returned if the file could not be opened.
//Return -2 if the specified file has already been opened previously.
short fopen_txt( char *fileReadName )
{
	fclose_txt( 0 );//无论上一个关没关，先关了上一个
	return alt_up_sd_card_fopen( fileReadName, false );
}


//关闭文件
//input: file_handle
//output: 0:成功 -1:失败(可能关闭了未打开的文件)
int fclose_txt( short file_handle )
{
	bool status = alt_up_sd_card_fclose( file_handle );
	if ( status )
	{
		return 0;
	}
	return -1;
}
//获取文件大小
int getfileSizeTxt( short file_handle )
{
	if ( file_handle < 0 || file_handle >= MAX_FILES_OPENED )
	{
		return -1;
	}
	return active_files[file_handle].file_size_in_bytes;
}
