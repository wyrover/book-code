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
#ifndef __COMMON_H__
#define __COMMON_H__

#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <malloc.h>
#include <errno.h>

#define callback __stdcall

#define TRUE 1
#define FALSE 0

#ifndef ASSERT
#ifdef _DEBUG
#include <assert.h>
#define ASSERT( x ) assert( x )
#else
#define ASSERT( x )
#endif
#endif

#define CONST_STR_LEN( string ) ( int )( sizeof( string ) - sizeof( char ) )

typedef unsigned long dword;
typedef unsigned char byte;
typedef unsigned short word;
typedef int int32;
typedef unsigned int uint32;

#include "dlist.h"

#ifdef __cplusplus
extern "C" {
#endif

void *find_record_info( void *info, list_ele_compare compare_func );
void littelendian2bigendian( byte *p, size_t size );
int write_to_new_file( char *file_path, char *file_name, byte *data, dword data_len );
int write_to_new_file_by_name( char *file_name, byte *data, dword data_len );
int32 read_all_file_data( char *file_name, byte **data, dword *data_len );
int32 mem_submem( byte* target_cont, int32 target_cont_len, byte* src_mem, int32 src_mem_len);
int dump_mem( void *mem, int size, char*str_out, dword *buff_len );
int release_file_data( byte **data );
int32 open_file_dlg( HWND owner, char *seled_file_name, dword buff_len, dword flags );
int32 del_record_info( void *info ); 
int32 add_new_record_info( void **info, dword size ); 

#ifdef __cplusplus
}
#endif

#endif //__COMMON_H__
