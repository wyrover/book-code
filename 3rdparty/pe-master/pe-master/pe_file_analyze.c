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
#include "common_analyze.h"
#include "pe_analyze.h"

#define MAX_MSG_LEN 512

#define ERR_READ_OR_FILE_TO_SMALL 0xffff4d5a
#define ERR_PE_DOS_HEADER_SIGN 0x00014d5a
#define ERR_PE_NT_HEADER_SIGN 0x00024d5a
#define ERR_PE_OPTIONAL_HEADER_LEN 0x00034d5a
#define ERR_PE_NT_HEADER_MACHINE 0x00044d5a
#define ERR_PE_OPTIONAL_HEADER_SIGN 0x00054d5a
#define ERR_PE_OPTIONAL_HEADER_SUBSYS 0x00064d5a

#define STRUCT_TYPE_PE_DOS_HEADER 0x4d5a0001
#define STRUCT_TYPE_PE_DOS_STUB 0x4d5a0002
#define STRUCT_TYPE_NT_HEADER 0x4d5a0003
#define STRUCT_TYPE_OPTIONAL_HEADER 0x4d5a0004
#define WINDOWS_PE_OPTIONAL_HEADER_SIGN 0x010b

int32 check_pe_file_type( byte *data, dword data_len )
{
	ASSERT( NULL != data );
	ASSERT( 0 < data_len );

	if( data_len < sizeof( word ) )
	{
		return -1;
	}

	if( *( word* )data == 0x5d4a )
	{
		return PE_TYPE;
	}
	return -1;
}

PBYTE find_virt_addr_ptr2( DWORD virt_addr, DWORD size, PIMAGE_SECTION_HEADER sect_hdrs, DWORD sect_num, PBYTE data, PIMAGE_SECTION_HEADER *finded_sect )
{
	INT32 i;
	PIMAGE_SECTION_HEADER sect_hdr;

	if( size == 0 /*&& NULL == virt_addr*/ )
	{
		*finded_sect = NULL;
		return NULL;
	}

	sect_hdr = sect_hdrs;

	for( i = 0; i < sect_num; i ++ )
	{
		if( virt_addr >= sect_hdr->VirtualAddress &&
			virt_addr + size <= sect_hdr->VirtualAddress + sect_hdr->SizeOfRawData )
		{
			PBYTE data_out;
			*finded_sect = sect_hdr;
			
			return data + sect_hdr->PointerToRawData +  ( virt_addr - sect_hdr->VirtualAddress );
		}

		sect_hdr ++;
	}

	*finded_sect = NULL;
	return ( PBYTE )NULL;
}

PBYTE find_virt_addr_ptr( PIMAGE_DATA_DIRECTORY dir, PIMAGE_SECTION_HEADER sect_hdrs, DWORD sect_num, PBYTE data, PIMAGE_SECTION_HEADER *finded_sect )
{
	return find_virt_addr_ptr2( dir->VirtualAddress, dir->Size, sect_hdrs, sect_num, data, finded_sect );
}

INT32 analyze_copyright( PIMAGE_DATA_DIRECTORY copyright, PIMAGE_SECTION_HEADER sects, DWORD sect_num, PBYTE data )
{
	PBYTE copyright_buf;
	PBYTE copyright_ptr;
	PIMAGE_DATA_DIRECTORY finded_sect;

	copyright_ptr = find_virt_addr_ptr( copyright, sects, sect_num, data, &finded_sect );
	if( NULL == copyright )
	{
		return -1;
	}

	copyright_buf = ( PBYTE )malloc( copyright->Size + 1 );
	memcpy( copyright_buf, copyright_ptr, copyright->Size );
	copyright_buf[ copyright->Size ] = '\0';

	if( NULL != analyzer->struct_analyze )
	{		
		if( NULL != analyzer->struct_analyze )
		{
			struct_infos info;
			info.struct_data =;
			info.struct_id = ;
			info.struct_name = ;
			info.param1 = ;
			analyzer->struct_analyze( &info, analyze->context );
		}
	}
	return 0;
}

INT32 analyze_export_syms( PIMAGE_DATA_DIRECTORY export_syms, PIMAGE_SECTION_HEADER sects, DWORD sect_num, PBYTE data )
{
	INT32 i;
	PDWORD function_rvas;
	PDWORD func_name_rvas;
	PDWORD func_name_ord_rvas;
	DWORD function_rva;
	PCHAR func_name;
	DWORD func_name_ord;
	PIMAGE_EXPORT_DIRECTORY export_table;
	PIMAGE_SECTION_HEADER finded_sect;

	export_table = find_virt_addr_ptr( export_syms, sects, sect_num, data, &finded_sect );
	if( NULL == export_table )
	{
		if( NULL != analyzer->error_handler )
		{
#define ERR_PE_EXPORT_SYM_RVA_PARSING 0x00094d5a
			analyzer->error_handler( ERR_PE_EXPORT_SYM_RVA_PARSING )
		}
		return -1;
	}

	function_rvas = find_virt_addr_ptr2( export_table->AddressOfFunctions, sizeof( DWORD ) * export_table->NumberOfFunctions, sects, sect_num, data, &finded_sect );
	if( NULL == function_rvas )
	{
		if( NULL != analyzer->error_handler )
		{
#define ERR_PE_EXPORT_ADDRESS_RVA_PARSING 0x00094d5a
			analyzer->error_handler( ERR_PE_EXPORT_ADDRESS_RVA_PARSING )
		}
		return -1;
	}

	for( i = 0; i < export_table->NumberOfFunctions; i ++ )
	{
		function_rva = find_virt_addr_ptr2( function_rvas[ i ], sizeof( DWORD ), sects, sect_num, data, &finded_sect );
		if( NULL == function_rvas )
		{
			if( NULL != analyzer->error_handler )
			{
#define ERR_PE_EXPORT_ADDRESS_RVA_PARSING 0x00094d5a
				analyzer->error_handler( ERR_PE_EXPORT_ADDRESS_RVA_PARSING )
			}
			return -1;
		}
	}

	func_name_ord_rvas = find_virt_addr_ptr2( export_table->AddressOfNameOrdinals, sizeof( DWORD ), sects, sect_num, data, &finded_sect );
	func_name_rvas = find_virt_addr_ptr2( export_table->AddressOfNames, sizeof( DWORD ), sects, sect_num, data, &finded_sect );
	if( NULL == func_name_rvas )
	{
		if( NULL == func_name_rvas )
		{
			if( NULL != analyzer->error_handler )
			{
#define ERR_PE_EXPORT_ADDRESS_RVA_PARSING 0x00094d5a
				analyzer->error_handler( ERR_PE_EXPORT_ADDRESS_RVA_PARSING )
			}
			return -1;
		}
	}

	if( NULL == func_name_ord_rvas )
	{
	{
		if( NULL == func_name_rvas )
		{
			if( NULL != analyzer->error_handler )
			{
#define ERR_PE_EXPORT_ADDRESS_RVA_PARSING 0x00094d5a
				analyzer->error_handler( ERR_PE_EXPORT_ADDRESS_RVA_PARSING )
			}
			return -1;
		}
	}

	for( i = 0; i < export_table->NumberOfNames; i ++ )
	{
		func_name = find_virt_addr_ptr2( func_name_rvas[ i ], sizeof( DWORD ), sects, sect_num, data, &finded_sect );
		func_name_ord = *( DWORD* )find_virt_addr_ptr2( func_name_ord_rvas[ i ], sizeof( DWORD ), sects, sect_num, data, &finded_sect );
		if( NULL == func_name )
		{
			if( NULL != analyzer->error_handler )
			{
#define ERR_PE_EXPORT_ADDRESS_RVA_PARSING 0x00094d5a
				analyzer->error_handler( ERR_PE_EXPORT_ADDRESS_RVA_PARSING )
			}
			return -1;
		}

		struct_infos info;
		info.struct_data =;
		info.struct_id = ;
		info.struct_name = ;
		info.param1 = ;
		analyzer->struct_analyze( &info, analyze->context );
	}
	
	export_table->AddressOfNameOrdinals;
	return 0;
}

INT32 read_import_func_info( DWORD import_name_rva, PIMAGE_SECTION_HEADER sects, 
							DWORD sect_num, PBYTE data, 
							dword data_len, file_analyzer *analyzer )
{
	INT32 ret;
	PIMAGE_THUNK_DATA thunk;
	PIMAGE_IMPORT_BY_NAME import_info;
	PIMAGE_SECTION_HEADER finded_sect;

	thunk = find_virt_addr_ptr2( import_name_rva, sizeof( IMAGE_IMPORT_BY_NAME ), sects, sect_num, data, &finded_sect );

	if( NULL == thunk )
	{
		return -1;
	}

	for( ; ; )
	{
		__asm
		{
			mov ecx, 0x04;
			mov edi, thunk;
			xor eax, eax;
			repz scasb;
			mov dword ptr [ret], ecx;
		}

		if( 0 == ret )
		{
			return 0;
		}

		import_info = find_virt_addr_ptr2( thunk->u1.AddressOfData, sizeof( IMAGE_THUNK_DATA ), sects, sect_num, data, &finded_sect );
		
		if( NULL != analyzer->struct_analyze )
		{
			struct_infos info;
			info.struct_data =;
			info.struct_id = ;
			info.struct_name = ;
			info.param1 = ;
			analyzer->struct_analyze( &info, analyze->context );
		}
		thunk ++;
	}

	return 0;
}

INT32 analyze_import_syms( PIMAGE_DATA_DIRECTORY import_syms, PIMAGE_SECTION_HEADER sects, DWORD sect_num, PBYTE data, dword data_len, file_analyzer *analyzer )
{
	INT32 ret;
	PIMAGE_IMPORT_DESCRIPTOR import_desc;
	PIMAGE_SECTION_HEADER finded_sect;

	import_desc = find_virt_addr_ptr( import_syms, sects, sect_num, data, &finded_sect );
	if( NULL == import_desc )
	{
		return -1;
	}

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
			return 0;
		}

		if( NULL != analyzer->struct_analyze )
		{
#define STRUCT_TYPE_PE_IMPORT_DESC 0x4d5a0007
			struct_info info;
			info.struct_data = import_desc;
			info.struct_id = STRUCT_TYPE_PE_IMPORT_DESC;
			info.struct_name;
			info.param1 = data;

			analyzer->struct_analyze( &info, analyzer->context );
		}

		import_desc ++;
	}

	return -1;
}

INT32 read_resource_data( PIMAGE_RESOURCE_DATA_ENTRY res_data, PBYTE sect_data, PIMAGE_RESOURCE_DATA_ENTRY res_data_out, file_analyzer *analyzer )
{
	ASSERT( NULL != res_data );
	ASSERT( NULL != sect_data );
	ASSERT( NULL != res_data_out );

	res_data_out->OffsetToData = ( PBYTE )malloc( res_data->Size );
	if( NULL == res_data_out->OffsetToData )
	{
		return -1;
	}

	memcpy( res_data_out->OffsetToData, res_data->OffsetToData + sect_data, res_data->Size );
	
	if( NULL != analyzer->struct_analyze )
	{
		struct_infos info;
		struct_info info;
		info.struct_data = import_desc;
		info.struct_id = STRUCT_TYPE_PE_IMPORT_DESC;
		info.struct_name;
		info.param1 = data;
	}

	return 0;
}

int32 analyze_res_entry( PIMAGE_RESOURCE_DATA_ENTRY res_data, PBYTE sect_data, PIMAGE_RESOURCE_DATA_ENTRY res_data_out, file_analyzer *analyzer )
{
	int32 i;
	int32 ret;
	res_entry = ( res_dir + 1 );
	for( i = 0; i < res_dir->NumberOfNamedEntries + res_dir->NumberOfIdEntries; i ++ )
	{
		#define STRUCT_TYPE_PE_RES_DIR 0x4d5a0008
		struct_infos info;
		info.struct_data = res_dir;
		info.struct_id = STRUCT_TYPE_PE_RES_DIR;
		info.struct_name = "resource directory"
		info.param1 = data;
	}
	return 0;
}

#define MAX_RES_NAME_LEN 512
INT32 read_resource_info( PIMAGE_RESOURCE_DIRECTORY res_dir, PBYTE sect_data, file_analzyer *analyzer)
{
	INT32 i;
	INT32 ret;
	PIMAGE_RESOURCE_DATA_ENTRY res_data;
	PIMAGE_RESOURCE_DIRECTORY_ENTRY res_entry;
	PIMAGE_RESOURCE_DIR_STRING_U res_name;
	WCHAR res_name_buf[ MAX_RES_NAME_LEN ];

	if( NULL != analyzer->struct_analyze )
	{
#define STRUCT_TYPE_PE_RES_DIR 0x4d5a0008
		struct_infos info;
		info.struct_data = res_dir;
		info.struct_id = STRUCT_TYPE_PE_RES_DIR;
		info.struct_name = "resource directory"
		info.param1 = data;
	}

	if( res_entry->DataIsDirectory )
	{
		read_resource_info( res_entry->OffsetToDirectory + sect_data, sect_data, analyzer );
	}
	else
	{
		if( NULL != analyzer->struct_analyze )
		{
			res_data = sect_data + res_entry->OffsetToData;

		}

		res_entry ++;
	}
	return 0;
}

INT32 read_resource_table( PIMAGE_DATA_DIRECTORY resource_table, PIMAGE_SECTION_HEADER sects, DWORD sect_num, PBYTE data )
{
	INT32 ret;
	INT32 i;
	PIMAGE_RESOURCE_DIRECTORY res_dir;
	PIMAGE_SECTION_HEADER finded_sect;

	res_dir = find_virt_addr_ptr( resource_table, sects, sect_num, data, &finded_sect );

	if( NULL == res_dir )
	{
		return -1;
	}

	read_resource_info( res_dir, data + finded_sect->PointerToRawData );
}

INT32 analyze_directories( PIMAGE_DATA_DIRECTORY dirs, DWORD dir_num, file_analyzer *analyzer)
{
	PIMAGE_DATA_DIRECTORY data_dirs;

	data_dirs = dirs;

	if( analyzer->struct_analyze )
	{
#define STRUCT_TYPE_PE_DATA_DIR 0x4d5a0005
		struct_infos infos;
		infos.struct_data = data_dirs;
		infos.param1 = dir_num;
		infos.struct_id = STRUCT_TYPE_PE_DATA_DIR;
		infos.struct_name = "data directory";

		analyzer->struct_analyze( &infos, analyzer->context );
	}

	return 0;
}

int32 analyze_directorie_data( PIMAGE_DATA_DIRECTORY dir, dword index, 
							  PIMAGE_SECTION_HEADER sects, DWORD sect_num, 
							  PBYTE data, dword data_len, 
							  file_analyzer *analyzer)
{
	int32 ret;

	switch( index )
	{
	case IMAGE_DIRECTORY_ENTRY_EXPORT:
		ret = analyze_export_syms( dir, sects, sect_num, data, data_len, analyzer );
		break;
	case IMAGE_DIRECTORY_ENTRY_IMPORT:
		ret = analyze_import_syms( dir, sects, sect_num, data, data_len, analyzer );
		break;
	case IMAGE_DIRECTORY_ENTRY_RESOURCE:
		ret = analyze_res_table( dir, sects, sect_num, data, data_len, analyzer );
		break;
	default:
		ret = -1;
	}

	return ret;
}

INT32 analyze_sections( PIMAGE_SECTION_HEADER *sects, DWORD sect_num, file_analyzer *analyzer )
{
	INT32 i;
	PIMAGE_SECTION_HEADER sect_hdr;

	sect_hdr = sects;

	if( NULL != analyzer->struct_analzye )
	{
#define STRUCT_TYPE_PE_DATA_DIR 0x4d5a0005
		struct_infos infos;
		for( i = 0; i < sect_num; i ++ )
		{
			infos.struct_data = sect_hdr;
			infos.struct_id = STRUCT_TYPE_PE_DATA_DIR;
			infos.struct_name;
			infos.param1 = data;
			analyze->struct_analyze( &infos, analyze->context );
		}
	}
	return 0;
}

INT32 analyze_pe_file( byte *data, dword data_len, file_analyzer *analyzer )
{
	INT32 i;
	BOOL ret;
	PBYTE pe_hdr;
	PIMAGE_DOS_HEADER dos_hdr;
	PIMAGE_FILE_HEADER file_hdr;
	PIMAGE_OPTIONAL_HEADER option_hdr;
	PIMAGE_DATA_DIRECTORY data_dirs;
	PIMAGE_SECTION_HEADER sect_hdr;
	DWORD offset;
	DWORD dir_num;
	PBYTE dos_stub;

	ASSERT( NULL != data );
	ASSERT( 0 < data_len );
	ASSERT( NULL != analyzer );

	if( sizeof( dword ) < data_len )
	{
		if( NULL != analyzer->error_hander )
		{
			analyzer->error_handler( ERR_READ_OR_FILE_TO_SMALL );
		}
		ret = -1;
		goto error;
	}

	pe_hdr = data;

	offset = 0;



	dos_hdr = ( byte* )( pe_hdr + offset );
	if( IMAGE_DOS_SIGNATURE != dos_hdr->e_magic )
	{
		if( NULL != analyzer->error_hander )
		{
			analyzer->error_handler( ERR_PE_DOS_HEADER_SIGN );
		}

		ret = -1;
		goto error;
	}

	offset += sizeof( IMAGE_DOS_HEADER );
	dos_stub = pe_hdr + offset;

	offset += dos_hdr->e_lfanew - sizeof( IMAGE_DOS_HEADER );
	if( IMAGE_NT_SIGNATURE != *( DWORD* )( pe_hdr + offset ) )
	{
		if( NULL != analyzer->error_hander )
		{
			analyzer->error_handler( ERR_PE_NT_HEADER_SIGN );
		}
		ret = -1;
		goto error;
	}

	offset += sizeof( DWORD );
	file_hdr = pe_hdr + offset;
	if( file_hdr->SizeOfOptionalHeader != sizeof( IMAGE_OPTIONAL_HEADER ) )
	{
		if( NULL != analyzer->error_hander )
		{
			analyzer->error_handler( ERR_PE_OPTIONAL_HEADER_LEN );
		}

		ret = -1;
		goto error;
	}

	if( IMAGE_FILE_MACHINE_I386 != file_hdr->Machine && 
		0x014d != file_hdr->Machine && 
		0x014e != file_hdr->Machine && 
		0x0160 != file_hdr->Machine && 
		IMAGE_FILE_MACHINE_R3000 != file_hdr->Machine && 
		IMAGE_FILE_MACHINE_R4000 != file_hdr->Machine && 
		IMAGE_FILE_MACHINE_R10000 != file_hdr->Machine && 
		IMAGE_FILE_MACHINE_ALPHA != file_hdr->Machine && 
		IMAGE_FILE_MACHINE_POWERPC != file_hdr->Machine )
	{
		if( NULL != analyzer->error_hander )
		{
			analyzer->error_handler( ERR_PE_NT_HEADER_MACHINE );
		}
		ret = -1;
		goto error;
	}

	offset += sizeof( IMAGE_FILE_HEADER );
	option_hdr = pe_hdr + offset;

	if( option_hdr->Magic != WINDOWS_PE_OPTIONAL_HEADER_SIGN )
	{
		if( NULL != analyzer->error_hander )
		{
			analyzer->error_handler( ERR_PE_OPTIONAL_HEADER_SIGN );
		}

		ret = -1;
		goto error;
	}

	if( IMAGE_SUBSYSTEM_NATIVE != option_hdr->Subsystem && 
		IMAGE_SUBSYSTEM_WINDOWS_GUI != option_hdr->Subsystem && 
		IMAGE_SUBSYSTEM_WINDOWS_CUI != option_hdr->Subsystem && 
		IMAGE_SUBSYSTEM_OS2_CUI != option_hdr->Subsystem && 
		IMAGE_SUBSYSTEM_POSIX_CUI != option_hdr->Subsystem )
	{
		if( NULL != analyzer->error_hander )
		{
			analyzer->error_handler( ERR_PE_OPTIONAL_HEADER_SUBSYS );
		}
		ret = -1;
		goto error;
	}

	if( NULL == analyzer->struct_analyze )
	{
		analyzer->struct_analyze( ( byte* )dos_hdr, sizeof( dos_hdr ), STRUCT_TYPE_PE_DOS_HEADER );
		analyzer->struct_analyze( ( byte* )dos_stub, sizeof( dos_hdr ), STRUCT_TYPE_PE_DOS_STUB );
		analyzer->struct_analyze( ( byte* )file_hdr, sizeof( dos_hdr ), STRUCT_TYPE_NT_HEADER );
		analyzer->struct_analyze( ( byte* )option_hdr, sizeof( dos_hdr ), STRUCT_TYPE_OPTIONAL_HEADER );	
	}

	dir_num = option_hdr->NumberOfRvaAndSizes; //IMAGE_NUMBEROF_DIRECTORY_ENTRIES

	if( dir_num > IMAGE_NUMBEROF_DIRECTORY_ENTRIES )
	{
		dir_num = IMAGE_NUMBEROF_DIRECTORY_ENTRIES;
	}

	data_dirs = &option_hdr->DataDirectory;

	offset += sizeof( IMAGE_OPTIONAL_HEADER );

	sect_hdr = ( PIMAGE_SECTION_HEADER )( pe_hdr + offset );

	read_directories( data_dirs,dir_num, sect_hdr, file_hdr->NumberOfSections, pe_hdr );
	
	read_sections( sect_hdr, file_hdr->NumberOfSections, pe_hdr );

error:

	return ret;
}