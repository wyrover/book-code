/*
 * Copyright 2010 JiJie Shi
 *
 * This file is part of PEMaster.
 *
 * PEMaster is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PEMaster is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PEMaster.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "common.h"
#include <CommDlg.h>

dlist *g_all_struct_info = NULL;
int32 prepare_analyzing()
{
	return init_list_element( &g_all_struct_info );
}

int32 add_new_record_info( void **info, dword size )
{
	int ret;
	byte *data;

	ASSERT( NULL != info );
	ASSERT( 0 < size );

	data = ( byte* )malloc( size );
	if( NULL == data )
	{
		*info = NULL;
		return -1;
	}

	memset( data, 0, size ); 

	ret = add_list_element( g_all_struct_info, data );
	if( 0 > ret )
	{
		*info = NULL;
		return -1;
	}

	*info = data;
	return 0;
}

int32 del_record_info( void *info )
{
	del_list_element( g_all_struct_info, info ); 
}

void *find_record_info( void *info, list_ele_compare compare_func )
{
	return find_list_element_by_compare( g_all_struct_info, info, compare_func );
}

int32 end_analyzing()
{
	int32 ret;
	if( NULL == g_all_struct_info )
	{
		return 0;
	}
	ret = destroy_list( g_all_struct_info, free_element_on_destroy );
	g_all_struct_info = NULL;
	return ret;
}

int32 open_file_dlg( HWND owner, char *seled_file_name, dword buff_len, dword flags )
{
	int32 ret;
	OPENFILENAME ofn;

	ASSERT( NULL != seled_file_name &&
		0 < buff_len );

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = owner;
	ofn.lpstrFile = seled_file_name;
	ofn.lpstrFile[ 0 ] = '\0';
	ofn.lpstrTitle = "select one file";
	ofn.nMaxFile = buff_len;
	ofn.lpstrFilter = "pe file (*.exe;*.dll)\0*.exe;*.dll\0coff file (*.lib;*.obj)\0*.lib;*.obj\0all file (*.*)\0*.*\0";
	ofn.lpstrInitialDir = NULL;

	if( flags == 0 )
	{
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	}
	else
	{
		ofn.Flags = OFN_OVERWRITEPROMPT;
	}
	ret = GetOpenFileName( &ofn );

	return ret == 0 ? -1 : 0;
}

void littelendian2bigendian( void * p, size_t size )
{
	int i;
	char * buf = ( char* )p;
	char temp;
	for ( i=0;i < size / 2; i++ ) {
		temp = buf[ i ];
		buf[ i ] = buf[ size - i - 1 ];
		buf[ size - i - 1 ] = temp;
	}
} 

int mem_submem( byte* target_cont, int target_cont_len, byte* src_mem, int src_mem_len)
{
	int __target_len;
	byte* __src_mem;
	int end_index;
	byte is_same;
	int i, j;

	if( src_mem_len < target_cont_len )
		return -1;

	__target_len = target_cont_len;
	__src_mem = src_mem;

	end_index = src_mem_len - target_cont_len + 1;

	for(i = 0; i < end_index; i ++)
	{
		is_same = TRUE;
		for( j = 0; j < target_cont_len; j++ )
		{
			if(target_cont[j] != __src_mem[i + j])
			{
				is_same = FALSE;
				break;
			}
			else
			{
				int j = 0;
			}
		}

		if( is_same )
			return i;
	}
	return -1;
}

int write_to_new_file_by_name( char *file_name, byte *data, dword data_len )
{
	int ret = 0;
	HANDLE hfile;
	dword writed;

	ASSERT( NULL != file_name );
	ASSERT( NULL != data );
	ASSERT( NULL != data_len );


	hfile = CreateFile( file_name, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_NEW, 0, NULL );
	if( INVALID_HANDLE_VALUE == hfile )
	{
		return -1;
	}

	ret = WriteFile( hfile, data, data_len, &writed, NULL );
	if( FALSE == ret || writed != data_len )
	{
		ret = -1;
		goto __error;
	}

__error:
	if( INVALID_HANDLE_VALUE != hfile )
	{
		CloseHandle( hfile );
	}

	return ret;
}

int write_to_new_file( char *file_path, char *file_name, byte *file_data, dword file_data_len )
{
	int ret = 0;
	HANDLE hfile;
	dword writed;
	char new_file_name[ MAX_PATH ];

	ASSERT( NULL != file_name );
	ASSERT( NULL != file_data );
	ASSERT( NULL != file_data_len );

	strcpy( new_file_name, file_path );
	strcat( new_file_name, "\\" );
	strcat( new_file_name, file_name );

	ret = write_to_new_file_by_name( new_file_name, file_data, file_data_len );

	return ret;
}

int read_all_file_data( char *file_name, byte **data, dword *data_len )
{
	int ret = 0;
	HANDLE hfile;
	dword file_len = 0;
	byte *__data = NULL;
	dword readed;

	ASSERT( NULL != data );
	ASSERT( NULL != data_len );

	hfile = CreateFile( file_name, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );
	if( INVALID_HANDLE_VALUE == hfile )
	{
		ret = -1;
		goto __return;
	}

	file_len = SetFilePointer( hfile, 0, NULL, SEEK_END );
	SetFilePointer( hfile, 0, NULL, SEEK_SET );

	__data = ( PBYTE )malloc( file_len );
	if( NULL == __data )
	{
		ret = -1;
		goto __return;
	}

	ret = ReadFile( hfile, __data, file_len, &readed, NULL );
	if( FALSE == ret || readed != file_len )
	{
		ret = -1;
		goto __error;
	}

	ret = prepare_analyzing();
	if( 0 > ret )
	{
		goto __error;
	}

	goto __return;

__error:
	if( NULL != __data )
	{
		free( __data );
		__data = NULL;
	}

	file_len = 0;

__return:
	if( INVALID_HANDLE_VALUE != hfile )
	{
		CloseHandle( hfile );
	}

	*data = __data;
	*data_len = file_len;
	return ret;
}

int release_file_data( byte **data )
{
	end_analyzing();
	ASSERT( NULL != data );
	ASSERT( NULL != *data );
	
	free( *data );
	*data = NULL;
}

int32 dump_mem( void *mem, int size, char*str_out, dword *buff_len )
{
	int ret;
	dword out_len;
	byte dump_str[ 256 ];
	char *str_cur;
	unsigned char str[20];
	unsigned char *m = mem;
	int i,j;

	ASSERT( NULL != str_out );
	ASSERT( NULL != buff_len );

	*str_out = '\0';
	out_len = 0;

	for (j = 0; j < size / 8; j++)
	{
		memset( str, 0, sizeof( str ) );
		for (i = 0; i < 8; i++) 
		{
			if (m[i] > ' ' && m[i] <= '~')
			{
				str[i] = m[i];
			}
			else
			{
				str[i] = '.';
			}
		}

		ret = sprintf( dump_str, "%02x %02x %02x %02x %02x %02x %02x %02x  %s\n",
			m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7], str );
		if( 0 >= ret )
		{
			return -1;
		}

		out_len += ret;

		if( out_len > *buff_len )
		{
			*buff_len = out_len;
			return -1;
		}

		strcat( str_out, dump_str );
		m+=8;
	}

	if( size % 8 )
	{
		memset( str, 0, sizeof( str ) );
		for (i = 0; i < size % 8; i++) 
		{
			if (m[i] > ' ' && m[i] <= '~')
			{
				str[i] = m[i];
			}
			else
			{
				str[i] = '.';
			}
		}

		*dump_str = '\0';
		str_cur = dump_str;
		m = ( byte* )mem + size - ( size % 8 );

		for (i = 0; i < size % 8; i++) 
		{
			ret = sprintf( str_cur, "%02x ",
				m[i] );
			if( 0 >= ret )
			{
				return -1;
			}

			str_cur += ret;
			out_len += ret;
		}

		if( out_len > *buff_len )
		{
			*buff_len = out_len;
			return -1;
		}

		strcat( str_out, dump_str );

		out_len += strlen( str ) + 2;
		if( out_len > *buff_len )
		{
			*buff_len = out_len;
			return -1;
		}

		strcat( str_out, " " );
		strcat( str_out, str );
	}

	*buff_len = out_len;
	return 0;
}

