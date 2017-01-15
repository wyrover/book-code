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
#include "pe_make.h"

typedef struct __pe_file_info
{
	byte *pe_file_data;
	dword pe_file_len;
	char pe_file_name[ MAX_PATH ];
	HANDLE hfile;
} pe_file_info;

#define	PRESERVED_SECTION_NUM 10
#define INIT_PE_FILE_LEN ( dword )( sizeof( IMAGE_DOS_HEADER ) + sizeof( IMAGE_NT_HEADERS32 )  + sizeof( IMAGE_NT_HEADERS ) + sizeof( IMAGE_SECTION_HEADER ) * PRESERVED_SECTION_NUM )
#define DOS_SUB_LEN 0x100
#define PE_MASTER_DEF_CODE_SECT_NAME "pmcode"
#define PM_SECT_CODE_SIGN ( *( dword* )( char*)"pmcd" )

#define CODE_BLOCK_FILLED_BYTE 0xcc
#define CODE_BLOCK_FILLED_BYTE_LEN ( int32 )( sizeof( dword ) + 1 )

dlist *g_list_pe_handles;

int32 init_pe_make()
{
	int32 ret;
	ret = init_list_element( &g_list_pe_handles );
	return ret;
}

int32 uninit_pe_make()
{
	destroy_list( g_list_pe_handles, free_element_on_destroy );
	return 0;
}

int32 create_pe_file( char *file_name, dword *pe_handle )
{
	int32 i;
	IMAGE_DOS_HEADER *dos_hdr;
	IMAGE_NT_HEADERS *nt_hdrs;
	IMAGE_SECTION_HEADER *sect_hdr;
	int32 ret;
	dword writed;
	pe_file_info *new_file_info;

	byte *pe_cont;

	ASSERT( NULL != file_name );

	pe_cont = ( byte* )malloc( INIT_PE_FILE_LEN );

	if( NULL == pe_cont )
	{
		return -1;
	}

	memset( pe_cont, 0, INIT_PE_FILE_LEN );

	dos_hdr = ( IMAGE_DOS_HEADER* )pe_cont;
	dos_hdr->e_lfanew = sizeof( IMAGE_DOS_HEADER ) + DOS_SUB_LEN;
	dos_hdr->e_magic = IMAGE_DOS_SIGNATURE;
	dos_hdr->e_csum = 0;
	dos_hdr->e_crlc = 0;
	dos_hdr->e_cblp = 0;
	dos_hdr->e_cp = 2;
	dos_hdr->e_cparhdr = 4;
	dos_hdr->e_cs = 0;
	dos_hdr->e_ip = 0;
	dos_hdr->e_lfarlc = 0;
	dos_hdr->e_maxalloc = 0xffff;
	dos_hdr->e_minalloc = 0;
	dos_hdr->e_oemid = 0;
	dos_hdr->e_oeminfo = 0;
	dos_hdr->e_ovno = 0;
	memset( &dos_hdr->e_res, 0, sizeof( dos_hdr->e_res ) );
	memset( &dos_hdr->e_res2, 0, sizeof( dos_hdr->e_res2 ) );
	dos_hdr->e_sp = 0;
	dos_hdr->e_ss = 0;

	nt_hdrs = ( IMAGE_NT_HEADERS* )( pe_cont + dos_hdr->e_lfanew );
	nt_hdrs->Signature = IMAGE_NT_SIGNATURE;
	nt_hdrs->FileHeader.Characteristics = IMAGE_FILE_RELOCS_STRIPPED |
		IMAGE_FILE_EXECUTABLE_IMAGE |
		IMAGE_FILE_LINE_NUMS_STRIPPED |
		IMAGE_FILE_LOCAL_SYMS_STRIPPED |
		IMAGE_FILE_32BIT_MACHINE;
	nt_hdrs->FileHeader.Machine = IMAGE_FILE_MACHINE_I386;
	nt_hdrs->FileHeader.NumberOfSections = PRESERVED_SECTION_NUM;
	nt_hdrs->FileHeader.NumberOfSymbols = 0;
	nt_hdrs->FileHeader.PointerToSymbolTable = 0;
	nt_hdrs->FileHeader.SizeOfOptionalHeader = sizeof( IMAGE_OPTIONAL_HEADER );
	nt_hdrs->FileHeader.TimeDateStamp = 0;

	nt_hdrs->OptionalHeader.AddressOfEntryPoint = 0;
	nt_hdrs->OptionalHeader.BaseOfCode = 0;
	nt_hdrs->OptionalHeader.CheckSum = 0;
	nt_hdrs->OptionalHeader.DllCharacteristics = 0;
	nt_hdrs->OptionalHeader.FileAlignment = 0x1000;
	nt_hdrs->OptionalHeader.ImageBase = 0x40000;
	nt_hdrs->OptionalHeader.SectionAlignment = 0x1000;
	nt_hdrs->OptionalHeader.SizeOfCode = 0;
	nt_hdrs->OptionalHeader.SizeOfHeaders = 0x1000;
	nt_hdrs->OptionalHeader.SizeOfHeapCommit = 0x4000;
	nt_hdrs->OptionalHeader.SizeOfHeapReserve = 0x100000;
	nt_hdrs->OptionalHeader.SizeOfStackCommit = 0x4000;
	nt_hdrs->OptionalHeader.SizeOfStackReserve = 0x100000;
	nt_hdrs->OptionalHeader.SizeOfImage = 0x10000;
	nt_hdrs->OptionalHeader.SizeOfInitializedData = 0;
	nt_hdrs->OptionalHeader.SizeOfUninitializedData = 0;
	nt_hdrs->OptionalHeader.Subsystem = 0x0002;
	nt_hdrs->OptionalHeader.Win32VersionValue = 0;

	sect_hdr = ( IMAGE_SECTION_HEADER* )( ( byte* )nt_hdrs + sizeof( IMAGE_NT_HEADERS ) );
	sect_hdr[0].Characteristics = IMAGE_SCN_MEM_EXECUTE;
	strncpy( sect_hdr[0].Name, PE_MASTER_DEF_CODE_SECT_NAME, sizeof( sect_hdr[0].Name ) );
	sect_hdr[0].PointerToRawData = ( dword )get_aligned_val( pe_cont, ( ( byte* )sect_hdr + sizeof( IMAGE_SECTION_HEADER ) * PRESERVED_SECTION_NUM - pe_cont  ), FILE_ALIGN_MODE );
	sect_hdr[0].SizeOfRawData = 0;

	for( i = 1; i < nt_hdrs->FileHeader.NumberOfSections; i ++ )
	{
		sect_hdr[i].PointerToRawData = sect_hdr[0].PointerToRawData;
		sect_hdr[i].SizeOfRawData = 0;
	}

	new_file_info = malloc( sizeof( pe_file_info ) );
	if( NULL == new_file_info )
	{
		goto __error;
	}

	add_list_element( g_list_pe_handles, ( list_element )new_file_info );
	new_file_info->pe_file_data = pe_cont;
	new_file_info->pe_file_len = INIT_PE_FILE_LEN;
	strcpy( new_file_info->pe_file_name, file_name );

	*pe_handle = ( dword )new_file_info;
	return 0;

__error:
	if( NULL != pe_cont )
	{
		free( pe_cont );
	}

	return -1;
}

int32 get_pe_hdrs( byte *pe_file, IMAGE_DOS_HEADER **dos_hdr, IMAGE_NT_HEADERS **nt_hdrs, IMAGE_SECTION_HEADER **sect_hdr )
{
	IMAGE_DOS_HEADER *__dos_hdr;
	IMAGE_NT_HEADERS *__nt_hdrs;
	IMAGE_SECTION_HEADER *__sect_hdr;

	ASSERT( NULL != pe_file );

	__dos_hdr = ( IMAGE_DOS_HEADER* )pe_file;
	__nt_hdrs = ( IMAGE_NT_HEADERS* )( pe_file + __dos_hdr->e_lfanew );
	__sect_hdr = ( IMAGE_SECTION_HEADER* )( ( byte* )__nt_hdrs + sizeof( IMAGE_NT_HEADERS ) );
	
	if( dos_hdr != NULL )
	{
		*dos_hdr = __dos_hdr;
	}

	if( nt_hdrs != NULL )
	{
		*nt_hdrs = __nt_hdrs;
	}

	if( sect_hdr != NULL )
	{
		*sect_hdr = __sect_hdr;
	}

	return 0;
}

int32 add_section( byte *pe_file, char* sect_name, dword sect_char, dword sect_len, IMAGE_SECTION_HEADER **sect_hdr )
{
	return 0;
}

int32 add_code_block_record( byte *codes, dword code_len )
{
	return 0;
}

dword get_aligned_val( byte* pe_file_data, dword org_val, dword mode )
{

	int32 ret;
	IMAGE_NT_HEADERS *nt_hdrs;
	dword aligned_setting;
	dword aligned_val;

	ASSERT( NULL != pe_file_data );

	ret = get_pe_hdrs( pe_file_data, NULL, &nt_hdrs, NULL );

	if( 0 > ret )
	{
		return -1;
	}

	aligned_val = 0;
	if( mode == FILE_ALIGN_MODE )
	{
		aligned_setting = nt_hdrs->OptionalHeader.FileAlignment;
	}
	else if( mode == MEM_ALIGN_MODE )
	{
		aligned_setting = nt_hdrs->OptionalHeader.SectionAlignment;
	}

	aligned_val = ( org_val + ( aligned_setting - 1 ) ) / aligned_setting * aligned_setting;
	return aligned_val;
}

int32 add_codes( dword pe_handle, byte *codes, dword code_len, dword flags )
{
	int32 ret;
	int32 i;
	int32 finded;
	IMAGE_DOS_HEADER *dos_hdr;
	IMAGE_NT_HEADERS *nt_hdrs;
	IMAGE_SECTION_HEADER *sect_hdr;
	byte *sect_data;
	byte *ins_code_ptr;
	dword sect_remain_len;
	byte *pe_file;
	byte *new_pe_file;
	dword pe_file_len;
	pe_file_info *info;

	ASSERT( NULL != pe_handle );
	ASSERT( FALSE == IsBadReadPtr( ( void* )pe_handle, sizeof( pe_file_info ) ) );

	info = ( pe_file_info* )pe_handle;
	pe_file_len = info->pe_file_len;
	pe_file = info->pe_file_data;

	get_pe_hdrs( pe_file, &dos_hdr, &nt_hdrs, &sect_hdr );

	ret = IsBadReadPtr( sect_hdr, sizeof( *sect_hdr ) );
	if( TRUE == ret )
	{
		new_pe_file = ( byte* )malloc( ( ( byte* )sect_hdr - pe_file ) + sizeof( *sect_hdr ) );
		if( NULL == new_pe_file )
		{
			return -1;
		}

		pe_file = new_pe_file;
		get_pe_hdrs( pe_file, &dos_hdr, &nt_hdrs, &sect_hdr );
		sect_hdr = ( IMAGE_SECTION_HEADER* )( ( byte* )nt_hdrs + sizeof( IMAGE_NT_HEADERS ) );
	}

	finded = FALSE;
	for( i = 0; i < nt_hdrs->FileHeader.NumberOfSections; i ++ )
	{
		if( sect_hdr[i].Characteristics | IMAGE_SCN_MEM_EXECUTE )
		{
			finded = TRUE;
			break;
		}
	}

	if( FALSE == finded )
	{
		ret = add_section( pe_file, PE_MASTER_DEF_CODE_SECT_NAME, IMAGE_SCN_MEM_EXECUTE, -1, &sect_hdr );
	}

	//if( *( dword* )sect_data == PM_SECT_CODE_SIGN )
	//{
	//	sect_remain_len = *( dword* )( sect_data + sizeof( dword ) );
	//}

	//if( sect_remain_len < code_len )
	//{

	//}

	ret = add_sect_len( pe_handle, i, code_len );
	if( 0 > ret )
	{
		return -1;
	}

	if( 0 < ret )
	{
		pe_file = info->pe_file_data;
		pe_file_len = info->pe_file_len;

		ret = get_pe_hdrs( pe_file, &dos_hdr, &nt_hdrs, &sect_hdr );
		if( 0 > ret )
		{
			return -1;
		}
	}

	sect_data = pe_file + sect_hdr[ i ].PointerToRawData;
	ins_code_ptr = sect_data + sect_hdr[ i ].SizeOfRawData;

	memcpy( ins_code_ptr, codes, code_len );

	if( flags & CODE_ENTRY_POINT )
	{
		nt_hdrs->OptionalHeader.AddressOfEntryPoint = ( dword )( ins_code_ptr - pe_file );
	}
	add_code_block_record( codes, code_len );

	return 0;
}

int32 add_sect_len( dword pe_handle, int32 index, int32 len_add )
{
	int32 ret;
	byte *pe_file;
	pe_file_info *info;
	dword pe_file_len;
	IMAGE_DOS_HEADER *dos_hdr;
	IMAGE_NT_HEADERS *nt_hdrs;
	IMAGE_SECTION_HEADER *sect_hdr;
	IMAGE_SECTION_HEADER *sect_hdr_end;
	IMAGE_SECTION_HEADER *sect_hdr_cur;
	dword all_file_len;
	dword aligned_len;

	ASSERT( NULL != pe_handle );
	info = ( pe_file_info* )pe_handle;

	pe_file = info->pe_file_data;
	pe_file_len = info->pe_file_len;

	ret = get_pe_hdrs( pe_file, &dos_hdr, &nt_hdrs, &sect_hdr );
	if( 0 > ret )
	{
		return -1;
	}

	sect_hdr_end = sect_hdr + nt_hdrs->FileHeader.NumberOfSections;

	nt_hdrs->OptionalHeader.SizeOfImage += len_add;

	arrange_sections( pe_handle, index, len_add );

	sect_hdr_cur = &sect_hdr[ index ];
	//aligned_len = get_aligned_val( pe_file, len_add, FILE_ALIGN_MODE );
	sect_hdr_cur->SizeOfRawData += len_add;
	sect_hdr_cur ++;

	for( ; ; )
	{
		if( sect_hdr_cur > sect_hdr_end )
		{
			break;
		}

		sect_hdr_cur->PointerToRawData += len_add;
		sect_hdr_cur++;
	}

	sect_hdr_cur = sect_hdr;
	ret = 0;
	for( ; ; )
	{
		if( sect_hdr_cur > sect_hdr_end )
		{
			break;
		}

		all_file_len = sect_hdr_cur->PointerToRawData + sect_hdr_cur->SizeOfRawData;

		if( all_file_len > pe_file_len )
		{
			ret = set_pe_file_len( ( dword )info, all_file_len );
			if( 0 > ret )
			{
				return ret;
			}

			pe_file = info->pe_file_data;
			pe_file_len = info->pe_file_len;
		}

		sect_hdr_cur ++;
	}

	return ret;
}

int32 add_import_table_item( dword pe_handle, byte *func, char *func_name, dword *func_order )
{
	int32 err_count;
	int32 ret;
	pe_file_info *info;
	byte *pe_file;
	dword pe_file_len;
	IMAGE_DOS_HEADER *dos_hdr;
	IMAGE_NT_HEADERS *nt_hdrs;
	IMAGE_SECTION_HEADER *sect_hdr;
	IMAGE_SECTION_HEADER *sect_finded;
	IMAGE_DATA_DIRECTORY *dir;
	IMAGE_IMPORT_DESCRIPTOR *import_desc;

	ASSERT( NULL != pe_handle );

	info = ( pe_file_info* )pe_handle;
	pe_file = info->pe_file_data;
	pe_file_len = info->pe_file_len;

	ret = get_pe_hdrs( pe_file, &dos_hdr, &nt_hdrs, &sect_hdr );
	if( 0 > ret )
	{
		return -1;
	}

	dir = &nt_hdrs->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_IMPORT ];

	import_desc = find_virt_addr_ptr( dir, sect_hdr, nt_hdrs->FileHeader.NumberOfSections, pe_file, &sect_finded );
	if( NULL == import_desc )
	{
		return -1;
	}

	err_count = 0;
	for( ; ; )
	{
		__asm
		{
			mov ecx, 0x14;
			mov edi, import_desc;
			xor eax, eax;
			repz scasb;
			mov dword ptr [ret], ecx;
		}

		if( ret == 0 )
		{
			break;
		}

		import_desc ++;
	}

	//__add_import_table_item( import_dest, pe_file, func, 
	return 0;
}

int32 check_sects_valid( IMAGE_SECTION_HEADER *sect_hdrs, dword sect_num )
{
	int32 ret;
	int32 i;
	ASSERT( NULL != sect_hdrs );

	for( i = 1; i < sect_num; i ++ )
	{
		IMAGE_SECTION_HEADER *pre_sect_hdr;
		pre_sect_hdr = &sect_hdrs[ i - 1];

		if( sect_hdrs[ i ].PointerToRawData < pre_sect_hdr->PointerToRawData + pre_sect_hdr->SizeOfRawData )
		{
			return -1;
		}
	}

	return 0;
}

int32 set_pe_file_len( dword pe_handle, dword new_len )
{
	pe_file_info *info;
	byte *pe_file;
	dword pe_file_len;
	IMAGE_DOS_HEADER *dos_hdr;
	IMAGE_NT_HEADERS *nt_hdrs;
	IMAGE_SECTION_HEADER *sect_hdr;

	ASSERT( NULL != pe_handle );
	info = ( pe_file_info* )pe_handle;

	pe_file = info->pe_file_data;
	pe_file_len = info->pe_file_len;

	if( new_len > pe_file_len )
	{
		byte *new_file_data;
		new_file_data = realloc( pe_file, new_len );
		if( NULL == new_file_data )
		{
			return -1;
		}

		memset( new_file_data + info->pe_file_len, 0, new_len - info->pe_file_len );
		info->pe_file_data = new_file_data;
		info->pe_file_len = new_len;
		return new_len;
	}

	return 0;
}

int32 arrange_sections( dword pe_handle, int32 sect_index, int32 offset )
{
	int32 ret;
	int32 i;
	pe_file_info *info;
	IMAGE_DOS_HEADER *dos_hdr;
	IMAGE_NT_HEADERS *nt_hdrs;
	IMAGE_SECTION_HEADER *sect_hdr;
	IMAGE_SECTION_HEADER *pre_sect_hdr;

	byte *pe_file;
	dword pe_file_len;

	ASSERT( NULL != pe_handle );

	info = ( pe_file_info* )pe_handle;
	pe_file = info->pe_file_data;
	pe_file_len = info->pe_file_len;

	ret = get_pe_hdrs( pe_file, &dos_hdr, &nt_hdrs, &sect_hdr );
	if( 0 > ret )
	{
		return -1;
	}

	ret = check_sects_valid( sect_hdr, nt_hdrs->FileHeader.NumberOfSections );
	if( 0 > ret )
	{
		ASSERT( FALSE );
		return -1;
	}

	for( i = nt_hdrs->FileHeader.NumberOfSections - 1; i >= sect_index; i -- )
	{
		memmove( pe_file + sect_hdr[ i ].PointerToRawData + offset, pe_file + sect_hdr[ i ].PointerToRawData, sect_hdr[ i ].SizeOfRawData );
	}

	return 0;
}

//int32 add_pe_hdr_len( dword pe_handle, dword new_len )
//{
//	byte *pe_file;
//	dword pe_file_len;
//	dword aligned_len;
//
//	pe_file_info *info;
//	IMAGE_DOS_HEADER *dos_hdr;
//	IMAGE_NT_HEADERS *nt_hdrs;
//	IMAGE_SECTION_HEADER *sect_hdr;
//
//	ASSERT( NULL != pe_handle );
//
//	info = ( pe_file_info* )pe_handle;
//	pe_file = info->pe_file_data;
//	pe_file_len = info->pe_file_len;
//
//	ret = get_pe_hdrs( pe_file, &dos_hdr, &nt_hdrs, &sect_hdr );
//
//
//}

int32 align_pe_datas( dword pe_handle )
{
	int32 i;
	int32 ret;

	byte *pe_file;
	dword pe_file_len;
	dword aligned_len;

	pe_file_info *info;
	IMAGE_DOS_HEADER *dos_hdr;
	IMAGE_NT_HEADERS *nt_hdrs;
	IMAGE_SECTION_HEADER *sect_hdr;

	ASSERT( NULL != pe_handle );

	info = ( pe_file_info* )pe_handle;
	pe_file = info->pe_file_data;
	pe_file_len = info->pe_file_len;

	ret = get_pe_hdrs( pe_file, &dos_hdr, &nt_hdrs, &sect_hdr );

	aligned_len = get_aligned_val( pe_file, sect_hdr->PointerToRawData, FILE_ALIGN_MODE );
	ASSERT( aligned_len != sect_hdr->PointerToRawData );

	//if( aligned_len > sect_hdr->PointerToRawData )
	//{
	//	add_pe_hdr_len( pe_file, aligned_len );
	//}
	
	for( i = 1; i < nt_hdrs->FileHeader.NumberOfSections; i ++ )
	{
		aligned_len = get_aligned_val( pe_file, sect_hdr->PointerToRawData, FILE_ALIGN_MODE );
		if( aligned_len > sect_hdr[ i ].PointerToRawData )
		{
			arrange_sections( pe_handle, i - 1, aligned_len - sect_hdr->PointerToRawData );
		}

		aligned_len = get_aligned_val( pe_file, sect_hdr->SizeOfRawData, FILE_ALIGN_MODE );
		if( aligned_len > sect_hdr[ i ].PointerToRawData )
		{
			arrange_sections( pe_handle, i, aligned_len - sect_hdr->PointerToRawData );
		}
	}

	return 0;
}

int32 pe_file_build( dword pe_handle )
{
	int32 ret;
	pe_file_info *info;
	HANDLE hfile;
	dword writed;

	ASSERT( NULL != pe_handle );

	ret = 0;

	info = ( pe_file_info* )pe_handle;

	hfile = CreateFile( info->pe_file_name, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, NULL, NULL );
	if( INVALID_HANDLE_VALUE == hfile )
	{
		ret = -1;
		goto __error;
	}

	ret = WriteFile( hfile, info->pe_file_data, info->pe_file_len, &writed, NULL );
	if( FALSE == ret )
	{
		ret = -1;
		goto __error;
	}

	if( info->pe_file_len != writed )
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