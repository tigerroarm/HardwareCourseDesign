/*
 * sdread.c
 *
 *  Created on: 2019��5��26��
 *      Author: 12757
 */


#include <errno.h>
#include <priv/alt_file.h>
#include <io.h>
#include <stdio.h>
#include <string.h>
#include "sdread.h"
#include <stdlib.h>


//ֱ�Ӵ�Altera_UP_SD_Card_Avalon_Interface.c�︴�Ƶĺ궨��ͽṹ�嶨��

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
// extern Variables( ʹ��Altera_UP_SD_Card_Avalon_Interface.c�еı��� )
///////////////////////////////////////////////////////////////////////////


// Pointers to currently opened files.
extern t_file_record active_files[MAX_FILES_OPENED];
extern t_FAT_12_16_boot_sector boot_sector_data;
extern int fat_partition_offset_in_512_byte_sectors;
extern unsigned int current_sector_index;
extern volatile char *buffer_memory;

extern t_find_data search_data;
///////////////////////////////////////////////////////////////////////////
// Local Functions
///////////////////////////////////////////////////////////////////////////





//�Զ��庯��


//��ȡ�ı����ݣ���ȡsize���ֽڣ����뻺��buffer,����ֵΪ��ʵ��ȡ���ֽ���
int fread_txt( short file_handle, char *buffer, int numInByte )
{
	//У��file_handle
	if ( file_handle < 0 || file_handle >= MAX_FILES_OPENED )
	{
		return 0;
	}
	//���ļ����û�д�
	if ( !active_files[file_handle].in_use )
	{
		return 0;
	}

	//���active_files[]��Ӧ��Ԫ��ָ��
	t_file_record *curActiveFile = active_files + file_handle;

	//numInByte����������ı�ʣ�µ��ֽ������ͽ�������
	if ( numInByte > (curActiveFile->file_size_in_bytes - curActiveFile->current_byte_position) )
	{
		numInByte = (curActiveFile->file_size_in_bytes - curActiveFile->current_byte_position);
	}

	//��ǰ��ȡ���ֽ���
	unsigned int bufferIndex = 0;



	//current_byte_position��0��ʼ��file_size_in_bytes-1Ϊ��ȡ���һ����file_size_in_bytes˵��������
	while ( curActiveFile->current_byte_position < curActiveFile->file_size_in_bytes && bufferIndex < numInByte )
	{
		int data_sector = boot_sector_data.data_sector_offset + \
				(curActiveFile->current_cluster_index - 2)*boot_sector_data.sectors_per_cluster + \
				 curActiveFile->current_sector_in_cluster;



		//��ǰ��ȡ���ֽڲ�Ϊ�ı��ĵ�һ���ֽڣ���������λ����������������֤FAT���������������������������⴦�������ܿ�FAT���ף�
		if ((curActiveFile->current_byte_position > 0) && ((curActiveFile->current_byte_position % 512) == 0))
		{
			// Read in a new sector of data.
			//���������һ���صĽ������Ͷ���һ����
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
						//���´��滻�ɴ�
						curActiveFile->current_cluster_index = next_cluster;
						//��������ƫ��Ϊ0
						curActiveFile->current_sector_in_cluster = 0;
						//���¼���data_sector
						data_sector = boot_sector_data.data_sector_offset + (curActiveFile->current_cluster_index - 2)*boot_sector_data.sectors_per_cluster +
						  curActiveFile->current_sector_in_cluster;

					}
				}
				else
				{
					//��֧�ֳ�FAT16���FAT��
					return 0;
				}
			}
			else//�ز���
			{
				curActiveFile->current_sector_in_cluster = curActiveFile->current_sector_in_cluster + 1;
				data_sector = data_sector + 1;

			}
		}

		//�����ȡ�����������仯���Ͷ���������current_sector_index�洢����֮ǰ��ȡ������λ��,
		//(data_sector + fat_partition_offset_in_512_byte_sectors)������Ҫ��ȡ������λ��
		if (current_sector_index != (data_sector + fat_partition_offset_in_512_byte_sectors))
		{
			if (!Read_Sector_Data(data_sector, fat_partition_offset_in_512_byte_sectors))
			{
				return 0;
			}
		}
		//�����������ݴ��뻺��
		int curByteOfSector = (curActiveFile->current_byte_position % 512);



		int i_max = curByteOfSector + numInByte - bufferIndex;

		if ( i_max > 512 )
		{
			i_max = 512;
		}



		int i;
		for ( i = curByteOfSector; i < i_max; i ++ )
		{
			//ȡ�������е�ֵ������
			buffer[bufferIndex] = (unsigned char) IORD_8DIRECT(buffer_memory, i );
			bufferIndex ++;
			//������ȡ���±�
			curActiveFile->current_byte_position = curActiveFile->current_byte_position + 1;
		}
	}


	return bufferIndex;//����ʵ�ʶ�ȡ���ֽ���
}

//�ı�ָ��ƫ��(����0,�ɹ��� ����-1��ʧ��)
short fseek_txt( short file_handle, int offset, enum fseekType fromwhere )
{
	//У��file_handle
	if ( file_handle < 0 || file_handle >= MAX_FILES_OPENED )
	{
		return -1;
	}
	//���ļ����û�д�
	if ( !active_files[file_handle].in_use )
	{
		return -1;
	}

	int status = 0;

	//���active_files[]��Ӧ��Ԫ��ָ��
	t_file_record *curActiveFile = active_files + file_handle;
	unsigned char sectors_per_cluster = boot_sector_data.sectors_per_cluster;
	//֮ǰ�Ĵ���
	int preCluster = ((curActiveFile->current_byte_position) >> 9) / sectors_per_cluster;


	//current_byte_position��0��ʼ��file_size_in_bytes-1Ϊ��ȡ���һ����file_size_in_bytes˵��������
	int currentByte = 0;

	switch( fromwhere )
	{
		case SEEK_SET_TXT://�Ƶ��ļ���ͷ
			currentByte = offset;
			break;

		case SEEK_CUR_TXT://��ǰλ��ƫ��
			currentByte = curActiveFile->current_byte_position + offset;
			break;

		case SEEK_END_TXT://�Ƶ��ļ���β
			currentByte = curActiveFile->file_size_in_bytes + offset;
			break;
		default:
			return -1;
	}


	//debug
//	curActiveFile->current_byte_position = currentByte;
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


	//���������������
	int curSector = (curActiveFile->current_byte_position) >> 9;
	int curCluster = curSector / sectors_per_cluster;
	int sectorInCluster = curSector % sectors_per_cluster;
	curActiveFile->current_sector_in_cluster = sectorInCluster;

	//���ݴ�������FAT��
	int fatClusterNum = 0;//����fat���Ĵ���(�������������)

	if ( curCluster < preCluster )//FAT����Ҫ������һ���أ���ֻ�ܴ�FAT����������
	{
		curActiveFile->current_cluster_index = curActiveFile->start_cluster_index;

		fatClusterNum = curCluster;
	}
	else//ֱ�������
	{
		fatClusterNum = curCluster - preCluster;

	}


	//FAT�����Ҵ�
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
				//���´��滻�ɴ�
				curActiveFile->current_cluster_index = next_cluster;
			}
		}
		else
		{
			//��֧�ֳ�FAT16���FAT��
			return -1;
		}
	}
	return 0;
}


//��ȡ��ǰ��ȡ�ļ���λ��(����ֵ)
long ftell_txt( short file_handle )
{
	//У��file_handle
	if ( file_handle < 0 || file_handle >= MAX_FILES_OPENED )
	{
		return -1;
	}
	//���ļ����û�д�
	if ( !active_files[file_handle].in_use )
	{
		return -1;
	}
	//���ص�ǰ��ȡ�ֽ���
	return active_files[file_handle].current_byte_position;
}


//���ļ�
//input: fileReadName( ���� .TXT )
//Output: file_handle
//An index to the file record assigned to the specified file.
//-1 is returned if the file could not be opened.
//Return -2 if the specified file has already been opened previously.
short fopen_txt( char *fileReadName )
{
	return alt_up_sd_card_fopen( fileReadName, false );
}


//�ر��ļ�
//input: file_handle
//output: true:�ɹ�  false:ʧ��(���ܹر���δ�򿪵��ļ�)
bool fclose_txt( short file_handle )
{
	bool status;
	if ( file_handle < 0 || file_handle >= MAX_FILES_OPENED )
	{
		status = false;
	}
	else
	{
		status = alt_up_sd_card_fclose( file_handle );
	}

	return status;
}
//��ȡ�ļ���С
int getfileSizeTxt( short file_handle )
{
	if ( file_handle < 0 || file_handle >= MAX_FILES_OPENED )
	{
		return -1;
	}
	return active_files[file_handle].file_size_in_bytes;
}

//��ȡ���ļ���������TextType���ͱ��������泤�ļ���
TextType readLongFileName( void )
{
	TextType longNameText;
	longNameText.fmt = UCS2_LITTLE;
	longNameText.text = NULL;
	longNameText.textLen = 0;

	int cluster = search_data.current_cluster_index;

	bool status = true;


	//���Ϊ��Ŀ¼
	if (cluster == 0)
	{
		alt_u8 fileNameStore[32]; //Ŀ¼���ݴ洢�ռ�


		//��ǰ�����±�(���������Ϊmax_root_dir_sectors)
		int sector_index = search_data.current_sector_in_cluster;
		//��ǰ�ļ��������е��±� (һ�����������Ŀ¼��Ϊ512/32=16)
		int file_counter = search_data.file_index_in_sector;


		//������ļ���У���
		Read_Sector_Data( sector_index + boot_sector_data.root_directory_sector_offset,
							fat_partition_offset_in_512_byte_sectors );

		int i;
		int fileOffset = ( file_counter << 5 );
		for ( i = 0; i < 11; i ++ )
		{
			fileNameStore[i] = (alt_u8) IORD_8DIRECT(buffer_memory, fileOffset + i );
		}
		alt_u8 checkNum = 0;
		for ( i = 0; i < 11; i ++ )
		{
			checkNum = ((checkNum&0x1)?0x80:0x00) + (checkNum>>1) + fileNameStore[i];
		}

		//��ȡ���ļ�
		file_counter -= 1;//�Ӷ��ļ�Ŀ¼��ǰһ����ʼ��
		if ( file_counter < 0 )
		{
			file_counter = 15;
			sector_index -= 1;
		}


#define MAX_LONG_NAME_NUIT_NUM 8 //���ļ������ռ8��Ŀ¼�Ŀռ�
#define LONG_NAME_STORE_SIZE (MAX_LONG_NAME_NUIT_NUM*26)

		alt_u8 longNameStore[LONG_NAME_STORE_SIZE]={0};//�ļ�����ʱ�洢�ռ�

		int curLongNameUnitNum = 0;//���ΪMAX_LONG_NAME_NUIT_NUM


		bool outWhileFlag = false;//���ڵ�������whileѭ��

		while( sector_index >= 0 )
		{
			//��ȡ����
			status = Read_Sector_Data ( sector_index + boot_sector_data.root_directory_sector_offset,
					fat_partition_offset_in_512_byte_sectors );
			//��ȡ�ļ�Ŀ¼

			while( file_counter >= 0 && curLongNameUnitNum < MAX_LONG_NAME_NUIT_NUM )
			{

				fileOffset = ( file_counter << 5 );
				for ( i = 0; i < 32; i ++ )
				{
					fileNameStore[i] = (alt_u8) IORD_8DIRECT(buffer_memory, fileOffset + i );
				}
				alt_u8 unitID = fileNameStore[0];

				//����б�ɾ���ļǺ�0xef�����߳��ļ���־�����ڣ�����У��Ͳ�ƥ�䣬���˳�
				if ( unitID == 0xe5 || ( fileNameStore[11] != 0x0f ) || fileNameStore[13] != checkNum )
				{
					outWhileFlag = true;
					break;
				}

				int realUnitID = ( unitID & 0x1f ) - 1;

				if ( realUnitID >= 0 && realUnitID < MAX_LONG_NAME_NUIT_NUM )
				{
					curLongNameUnitNum = realUnitID;
				}

				int nameStoreIndex = curLongNameUnitNum * 26;
				//����Ч��unicodeֵ����nameUCS2
				//��1~5��ucs2����
				for ( i = 0; i < 10; i ++ )
				{
					longNameStore[nameStoreIndex+i] = fileNameStore[i+1];
				}
				//��6~11��ucs2����
				for ( i = 10; i < 22; i ++ )
				{
					longNameStore[nameStoreIndex+i] = fileNameStore[i+4];
				}
				//��12~13��ucs2����
				for ( i = 22; i < 26; i ++ )
				{
					longNameStore[nameStoreIndex+i] = fileNameStore[i+6];
				}

				//���ļ�������
				if ( (unitID & 0x40) == 0x40 )
				{
					outWhileFlag = true;
					break;
				}

				file_counter --;
				curLongNameUnitNum ++;
			}//while of file_counter
			if ( outWhileFlag )
			{
				break;
			}

			file_counter = 15;

			sector_index--;
		}//while of sector_index



		//���������unicode�ļ���
		int i_max = LONG_NAME_STORE_SIZE;
		for ( i = 0; i < i_max; i += 2 )
		{
			if ( longNameStore[i] == 0 && longNameStore[i+1] == 0 )
			{
				break;
			}
		}
		//����ļ������ֽ���
		int textLen = i;

		longNameText.textLen = textLen;
		longNameText.text = malloc( textLen + 2 );

		char *tempStr = longNameText.text;
		for ( i = 0; i < textLen; i ++ )
		{
			tempStr[i] = longNameStore[i];
		}
		//������־����������ֽ�Ϊ0x00,0x00
		tempStr[textLen] = 0;
		tempStr[textLen+1] = 0;
	}

#undef LONG_NAME_STORE_SIZE
#undef MAX_LONG_NAME_NUIT_NUM


	return longNameText;
}