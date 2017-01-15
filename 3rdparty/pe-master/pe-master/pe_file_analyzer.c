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
#include "pe_file_analyzer.h"
#include "file_analyzer.h"

int32 callback check_pe_file_type( byte *data, dword data_len )
{
	ASSERT( NULL != data );
	ASSERT( 0 < data_len );

	if( data_len < sizeof( word ) )
	{
		return -1;
	}

	if( *( word* )data == IMAGE_DOS_SIGNATURE )
	{
		return PE_FILE_TYPE;
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

INT32 analyze_copyright( PIMAGE_DATA_DIRECTORY copyright, PIMAGE_SECTION_HEADER sects, DWORD sect_num, PBYTE data, dword data_len, file_analyzer *analyzer )
{
	int32 ret;
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
		struct_infos *info;
		ret = add_new_record_info( &info, sizeof( *info ) );
		if( 0 > ret )
		{
			return ret;
		}

		info->struct_data = copyright_buf;
		info->struct_id = STRUCT_TYPE_PE_COPYRIGHT;
		info->struct_index = 0;
		info->struct_context = analyzer;
		analyzer->struct_analyze( info, analyzer->context );
	}
	return 0;
}

INT32 analyze_export_syms( PIMAGE_DATA_DIRECTORY export_syms, PIMAGE_SECTION_HEADER sects, DWORD sect_num, PBYTE data, dword data_len, file_analyzer *analyzer )
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

	export_table = ( PIMAGE_EXPORT_DIRECTORY )find_virt_addr_ptr( export_syms, sects, sect_num, data, &finded_sect );
	if( NULL == export_table )
	{
		if( NULL != analyzer->error_handler )
		{
			analyzer->error_handler( ERR_PE_EXPORT_SYM_RVA_PARSING );
		}
		return -1;
	}

	function_rvas = find_virt_addr_ptr2( export_table->AddressOfFunctions, sizeof( DWORD ) * export_table->NumberOfFunctions, sects, sect_num, data, &finded_sect );
	if( NULL == function_rvas )
	{
		if( NULL != analyzer->error_handler )
		{
			error_infos info;
			info.desc = NULL;
			info.err_code = ERR_PE_EXPORT_ADDRESS_RVA_PARSING;
			analyzer->error_handler( &info );
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
				error_infos info;
				info.desc = NULL;
				info.err_code = ERR_PE_EXPORT_ADDRESS_RVA_PARSING;
				analyzer->error_handler( &info );
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
				error_infos info;
				info.desc = NULL;
				info.err_code = ERR_PE_EXPORT_ADDRESS_RVA_PARSING;

				analyzer->error_handler( &info );
			}
			return -1;
		}
	}

	if( NULL == func_name_ord_rvas )
	{
		if( NULL != analyzer->error_handler )
		{
			error_infos info;
			info.desc = NULL;
			info.err_code = ERR_PE_EXPORT_ADDRESS_RVA_PARSING;

			analyzer->error_handler( &info );
		}
		return -1;
	}

	if( NULL == func_name_rvas )
	{
		if( NULL != analyzer->error_handler )
		{
			error_infos info;
			info.desc = NULL;
			info.err_code = ERR_PE_EXPORT_ADDRESS_RVA_PARSING;

			analyzer->error_handler( &info );
		}
		return -1;
	}

	for( i = 0; i < export_table->NumberOfNames; i ++ )
	{
		func_name = find_virt_addr_ptr2( func_name_rvas[ i ], sizeof( DWORD ), sects, sect_num, data, &finded_sect );
		func_name_ord = *( DWORD* )find_virt_addr_ptr2( func_name_ord_rvas[ i ], sizeof( DWORD ), sects, sect_num, data, &finded_sect );
		if( NULL == func_name )
		{
			if( NULL != analyzer->error_handler )
			{
				error_infos info;
				info.desc = NULL;
				info.err_code = ERR_PE_EXPORT_ADDRESS_RVA_PARSING;

				analyzer->error_handler( &info );
			}
			return -1;
		}

		if( NULL != analyzer->name_analyze )
		{
			name_info info;
			info.name = func_name;
			info.name_order = func_name_ord;

			analyzer->name_analyze( &info, analyzer->context );
		}
	}
	
	export_table->AddressOfNameOrdinals;
	return 0;
}

INT32 read_import_func_info( DWORD import_name_rva, PIMAGE_SECTION_HEADER sects, 
							DWORD sect_num, PBYTE data, 
							dword data_len, file_analyzer *analyzer )
{
	INT32 ret;
	int32 err_count;
	PIMAGE_THUNK_DATA thunk;
	PIMAGE_IMPORT_BY_NAME import_info;
	PIMAGE_SECTION_HEADER finded_sect;

	thunk = find_virt_addr_ptr2( import_name_rva, sizeof( IMAGE_IMPORT_BY_NAME ), sects, sect_num, data, &finded_sect );

	if( NULL == thunk )
	{
		return -1;
	}

	err_count = 0;
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
			struct_infos *info;
			ret = add_new_record_info( &info, sizeof( *info ) );
			if( 0 > ret )
			{
				err_count ++;
				continue;
			}

			info->struct_data = import_info;
			info->struct_id = STRUCT_TYPE_PE_IMPORT_BY_NAME;
			info->struct_index = 0;
			info->struct_context = analyzer;

			analyzer->struct_analyze( info, analyzer->context );
		}
		thunk ++;
	}

	return -err_count;
}

INT32 analyze_import_syms( PIMAGE_DATA_DIRECTORY import_syms, PIMAGE_SECTION_HEADER sects, DWORD sect_num, PBYTE data, dword data_len, file_analyzer *analyzer )
{
	INT32 ret;
	int32 err_count;
	PIMAGE_IMPORT_DESCRIPTOR import_desc;
	PIMAGE_SECTION_HEADER finded_sect;

	import_desc = find_virt_addr_ptr( import_syms, sects, sect_num, data, &finded_sect );
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
			return 0;
		}

		if( NULL != analyzer->struct_analyze )
		{
#define STRUCT_TYPE_PE_IMPORT_DESC 0x4d5a0007
			struct_infos *info;
			ret = add_new_record_info( &info, sizeof( *info ) );
			if( 0 > ret )
			{
				err_count ++;
			}

			info->struct_data = import_desc;
			info->struct_id = STRUCT_TYPE_PE_IMPORT_DESC;
			info->struct_index = 0;
			info->struct_context = analyzer;

			analyzer->struct_analyze( info, analyzer->context );
		}

		import_desc ++;
	}

	return -err_count;
}

INT32 read_resource_data( PIMAGE_RESOURCE_DATA_ENTRY res_data, PBYTE sect_data, PIMAGE_RESOURCE_DATA_ENTRY res_data_out, file_analyzer *analyzer )
{
	int32 ret;
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
		struct_infos *info;
		ret = add_new_record_info( &info, sizeof( *info ) );
		if( 0 > ret )
		{
			return ret;
		}

		info->struct_data = res_data_out;
		info->struct_id = STRUCT_TYPE_PE_IMPORT_DESC;
		info->struct_index = 0;
		info->struct_context = analyzer;
		analyzer->struct_analyze( info, analyzer->context );
	}

	return 0;
}

int32 analyze_res_dir( PIMAGE_RESOURCE_DIRECTORY res_dir, PBYTE sect_data, PIMAGE_RESOURCE_DATA_ENTRY res_data_out, file_analyzer *analyzer )
{
	int32 i;
	int32 ret;
	int32 err_count;
	PIMAGE_RESOURCE_DATA_ENTRY res_data;

	res_data = ( PIMAGE_RESOURCE_DATA_ENTRY )( ( byte* )res_dir + sizeof( IMAGE_RESOURCE_DIRECTORY ) );
	err_count = 0;
	for( i = 0; i < res_dir->NumberOfNamedEntries + res_dir->NumberOfIdEntries; i ++ )
	{
#define STRUCT_TYPE_PE_RES_DIR 0x4d5a0008
		struct_infos *info;
		ret = add_new_record_info( &info, sizeof( *info ) );
		if( 0 > ret )
		{
			err_count ++;
		}

		info->struct_data = res_dir;
		info->struct_id = STRUCT_TYPE_PE_RES_DIR;
		info->struct_index = 0;
		info->struct_context = analyzer;

		analyzer->struct_analyze( info, analyzer->context );
	}
	return -err_count;
}

#define MAX_RES_NAME_LEN 512
INT32 read_resource_info( PIMAGE_RESOURCE_DIRECTORY res_dir, PBYTE sect_data, file_analyzer *analyzer)
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
		struct_infos *info;
		ret = add_new_record_info( &info, sizeof( *info ) );
		if( 0 > ret )
		{
			goto __error;
		}

		info->struct_data = res_dir;
		info->struct_id = STRUCT_TYPE_PE_RES_DIR;
		info->struct_index = 0;
		info->struct_context = analyzer;

		analyzer->struct_analyze( info, analyzer->context );
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
			if( NULL != analyzer->struct_analyze )
			{
#define STRUCT_TYPE_PE_RES_DATA 0x4d5a000c
				struct_infos *info;
				ret = add_new_record_info( &info, sizeof( *info ) );
				if( 0 > ret )
				{
					goto __error;
				}

				info->struct_data = res_data;
				info->struct_id = STRUCT_TYPE_PE_RES_DATA;
				info->struct_index = 0;
				info->struct_context = analyzer;

				analyzer->struct_analyze( info, analyzer->context );
			}
		}

		res_entry ++;
	}

	return 0;
	
__error:
	return -1;
}

INT32 read_resource_table( PIMAGE_DATA_DIRECTORY resource_table, PIMAGE_SECTION_HEADER sects, DWORD sect_num, PBYTE data, dword data_len, file_analyzer *analyzer )
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

	read_resource_info( res_dir, data + finded_sect->PointerToRawData, analyzer );
	return 0;
}

INT32 analyze_directories( PIMAGE_DATA_DIRECTORY dirs, DWORD dir_num, file_analyzer *analyzer)
{
	int32 ret;
	PIMAGE_DATA_DIRECTORY data_dirs;

	data_dirs = dirs;

	if( analyzer->struct_analyze )
	{
#define STRUCT_TYPE_PE_DATA_DIR 0x4d5a0005
		struct_infos *info;
		ret = add_new_record_info( &info, sizeof( *info ) );
		if( 0 > ret )
		{
			return ret;
		}

		info->struct_data = data_dirs;
		info->struct_id = STRUCT_TYPE_PE_DATA_DIR;
		info->struct_index = 0;
		info->struct_context = analyzer;
		analyzer->struct_analyze( info, analyzer->context );
	}

	return 0;
}

int32 analyze_directories_data( PIMAGE_DATA_DIRECTORY dir, dword index, 
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
		//ret = analyze_res_table( dir, sects, sect_num, data, data_len, analyzer );
		break;
	default:
		ret = -1;
	}

	return ret;
}

__inline INT32 analyze_sections( PIMAGE_SECTION_HEADER *sects, DWORD sect_num, file_analyzer *analyzer )
{
	INT32 i;
	int32 ret;
	int32 err_count;
	PIMAGE_SECTION_HEADER sect_hdr;

	sect_hdr = sects;

	err_count = 0;
	if( NULL != analyzer->struct_analyze )
	{
		struct_infos *info;
		for( i = 0; i < sect_num; i ++ )
		{
			ret = add_new_record_info( &info, sizeof( *info ) );
			if( 0 > ret )
			{
				err_count;
			}

			info->struct_data = sect_hdr;
			info->struct_id = STRUCT_TYPE_PE_SECTION;
			info->struct_index = i;
			info->struct_context = analyzer;
			info->param3 = analyzer->all_file_data + ( sect_hdr + i )->PointerToRawData; 
			info->param4 = ( sect_hdr + i )->SizeOfRawData; 
			analyzer->struct_analyze( info, analyzer->context );
		}	
	}
	return -err_count;
}

INT32 analyze_pe_file_struct( byte *data, dword data_len, file_analyzer *analyzer )
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
	ppe_struct_info_list pe_write_info; 

	ASSERT( NULL != data );
	ASSERT( 0 < data_len );
	ASSERT( NULL != analyzer );

	pe_write_info = analyzer->pe_write_info; 

	if( sizeof( dword ) > data_len )
	{
		if( NULL != analyzer->error_handler )
		{
			error_infos info;
			info.desc = NULL;
			info.err_code = ERR_READ_OR_FILE_TO_SMALL;
			analyzer->error_handler( &info );
		}

		ret = -1;
		goto __error;
	}

	pe_hdr = data;

	offset = 0;

	dos_hdr = ( byte* )( pe_hdr + offset );
	if( IMAGE_DOS_SIGNATURE != dos_hdr->e_magic )
	{
		if( NULL != analyzer->error_handler )
		{
			error_infos info;
			info.desc = NULL;
			info.err_code = ERR_PE_DOS_HEADER_SIGN;
			analyzer->error_handler( &info );
		}

		ret = -1;
		goto __error;
	}

	offset += sizeof( IMAGE_DOS_HEADER );
	dos_stub = pe_hdr + offset;

	offset += dos_hdr->e_lfanew - sizeof( IMAGE_DOS_HEADER );
	if( IMAGE_NT_SIGNATURE != *( DWORD* )( pe_hdr + offset ) )
	{
		if( NULL != analyzer->error_handler )
		{
			error_infos info;
			info.desc = NULL;
			info.err_code = ERR_PE_NT_HEADER_SIGN;
			analyzer->error_handler( &info );
		}
		ret = -1;
		goto __error;
	}

	offset += sizeof( DWORD );
	file_hdr = pe_hdr + offset;
	if( file_hdr->SizeOfOptionalHeader != sizeof( IMAGE_OPTIONAL_HEADER ) )
	{
		if( NULL != analyzer->error_handler )
		{
			error_infos info;
			info.desc = NULL;
			info.err_code = ERR_PE_OPTIONAL_HEADER_LEN;
			analyzer->error_handler( &info );
		}

		ret = -1;
		goto __error;
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
		if( NULL != analyzer->error_handler )
		{
			error_infos info;
			info.desc = NULL;
			info.err_code = ERR_PE_NT_HEADER_MACHINE;
			analyzer->error_handler( &info );
		}
		ret = -1;
		goto __error;
	}

	offset += sizeof( IMAGE_FILE_HEADER );
	option_hdr = pe_hdr + offset;

	if( option_hdr->Magic != WINDOWS_PE_OPTIONAL_HEADER_SIGN )
	{
		if( NULL != analyzer->error_handler )
		{
			error_infos info;
			info.desc = NULL;
			info.err_code = ERR_PE_OPTIONAL_HEADER_SIGN;
			analyzer->error_handler( &info );
		}

		ret = -1;
		goto __error;
	}

	if( IMAGE_SUBSYSTEM_NATIVE != option_hdr->Subsystem && 
		IMAGE_SUBSYSTEM_WINDOWS_GUI != option_hdr->Subsystem && 
		IMAGE_SUBSYSTEM_WINDOWS_CUI != option_hdr->Subsystem && 
		IMAGE_SUBSYSTEM_OS2_CUI != option_hdr->Subsystem && 
		IMAGE_SUBSYSTEM_POSIX_CUI != option_hdr->Subsystem )
	{
		if( NULL != analyzer->error_handler )
		{
			error_infos info;
			info.desc = NULL;
			info.err_code = ERR_PE_OPTIONAL_HEADER_SUBSYS;
			analyzer->error_handler( &info );
		}
		ret = -1;
		goto __error;
	}

	if( NULL != analyzer->struct_analyze )
	{
		struct_infos *info;
		ret = add_new_record_info( &info, sizeof( *info ) );
		if( 0 > ret )
		{
			goto __error;
		}

		info->struct_data = dos_hdr;
		info->struct_id = STRUCT_TYPE_PE_DOS_HEADER;
		info->struct_index = 0;
		info->struct_context = analyzer;
		analyzer->struct_analyze( info, analyzer->context );
		if( pe_write_info != NULL )
		{
			add_pe_struct_info( pe_write_info, info ); 
		}
		
		ret = add_new_record_info( &info, sizeof( *info ) );
		if( 0 > ret )
		{
			goto __error;
		}

		info->struct_data = dos_stub;
		info->struct_id = STRUCT_TYPE_PE_DOS_STUB;
		info->struct_index = 0;
		info->struct_context = analyzer;
		info->data_len = dos_hdr->e_lfanew - sizeof( IMAGE_DOS_HEADER );
		analyzer->struct_analyze( info, analyzer->context );
		if( pe_write_info != NULL )
		{
			add_pe_struct_info( pe_write_info, info ); 
		}
		
		ret = add_new_record_info( &info, sizeof( *info ) );
		if( 0 > ret )
		{
			goto __error;
		}

		info->struct_data = ( byte* )file_hdr;
		info->struct_id = STRUCT_TYPE_PE_NT_HEADER;
		info->struct_index = 0;
		info->struct_context = analyzer;
		analyzer->struct_analyze( info, analyzer->context );
		if( pe_write_info != NULL )
		{
			add_pe_struct_info( pe_write_info, info ); 
		}

		ret = add_new_record_info( &info, sizeof( *info ) );
		if( 0 > ret )
		{
			goto __error;
		}

		info->struct_data = ( byte* )option_hdr;
		info->struct_id = STRUCT_TYPE_PE_OPTIONAL_HEADER;
		info->struct_index = 0;
		info->struct_context = analyzer;

		analyzer->struct_analyze( info, analyzer->context );
		if( pe_write_info != NULL )
		{
			add_pe_struct_info( pe_write_info, info ); 
		}
	}

	dir_num = option_hdr->NumberOfRvaAndSizes; //IMAGE_NUMBEROF_DIRECTORY_ENTRIES

	if( dir_num > IMAGE_NUMBEROF_DIRECTORY_ENTRIES )
	{
		dir_num = IMAGE_NUMBEROF_DIRECTORY_ENTRIES;
	}

	data_dirs = &option_hdr->DataDirectory;

	offset += sizeof( IMAGE_OPTIONAL_HEADER );

	sect_hdr = ( PIMAGE_SECTION_HEADER )( pe_hdr + offset );

	//read_directories( data_dirs,dir_num, sect_hdr, file_hdr->NumberOfSections, pe_hdr );

	for( i = 0; i < file_hdr->NumberOfSections; i ++ )
	{
		if( NULL != analyzer->struct_analyze )
		{
			struct_infos *info;
			ret = add_new_record_info( &info, sizeof( *info ) );
			if( 0 > ret )
			{
				goto __error;
			}

			info->struct_data = ( byte* )( sect_hdr + i );
			info->struct_id = STRUCT_TYPE_PE_SECTION;
			info->struct_index = i;
			info->struct_context = analyzer;
			info->param3 = data + ( sect_hdr + i )->PointerToRawData; 
			info->param4 = ( sect_hdr + i )->SizeOfRawData; 
			analyzer->struct_analyze( info, analyzer->context );
			if( pe_write_info != NULL )
			{
				add_pe_struct_info( pe_write_info, info ); 
			}
		}
	}

	{
		struct_infos *filled_data;
		ret = add_new_record_info( &filled_data, sizeof( *filled_data ) );
		if( 0 > ret )
		{
			goto __error;
		}

		filled_data->struct_data = ( byte* )( sect_hdr + file_hdr->NumberOfSections );
		filled_data->struct_id = STRUCT_TYPE_PE_FILL_DATA;
		filled_data->struct_index = 0;
		filled_data->struct_context = analyzer;
		filled_data->data_len = ( data + sect_hdr->PointerToRawData ) - filled_data->struct_data; 
		analyzer->struct_analyze( filled_data, analyzer->context );
		if( pe_write_info != NULL )
		{
			add_pe_struct_info( pe_write_info, filled_data ); 
		}
	}

__error:
	return ret;
}