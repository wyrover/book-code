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
#include "file_analyzer.h"
#include "pe_file_analyzer.h"
#include "pe_writer.h"

int32 init_pe_writer( ppe_struct_info_list *all_pe_struct_infos )
{
	int32 ret; 
	ppe_struct_info_list _all_pe_struct_infos; 

	_all_pe_struct_infos = ( ppe_struct_info_list )malloc( sizeof( pe_struct_info_list ) ); 
	if( NULL == all_pe_struct_infos )
	{
		*all_pe_struct_infos = NULL; 
		return -ENOMEM; 
	}

	memset( _all_pe_struct_infos, 0, sizeof( *all_pe_struct_infos ) ); 

	ret = init_list_element( &_all_pe_struct_infos->pe_section_list );
	if( ret < 0 )
	{
		free( _all_pe_struct_infos ); 

		*all_pe_struct_infos = NULL; 
		return ret; 
	}

	*all_pe_struct_infos = _all_pe_struct_infos; 
	return ret; 
}

int32 add_pe_struct_info( ppe_struct_info_list all_pe_struct_infos, struct_infos *info )
{
	int ret;

	ASSERT( NULL != info );

	if( info->struct_id == STRUCT_TYPE_PE_DOS_HEADER )
	{
		all_pe_struct_infos->pe_dos_hdr = info; 
	}
	else if( info->struct_id == STRUCT_TYPE_PE_DOS_STUB )
	{
		all_pe_struct_infos->pe_dos_stub = info; 
	}
	else if( info->struct_id == STRUCT_TYPE_PE_NT_HEADER )
	{
		all_pe_struct_infos->pe_nt_hdr = info; 
	}
	else if( info->struct_id == STRUCT_TYPE_PE_OPTIONAL_HEADER )
	{
		all_pe_struct_infos->pe_optional_hdr = info; 
	}
	else if( info->struct_id == STRUCT_TYPE_PE_SECTION )
	{
		ret = add_list_element( all_pe_struct_infos->pe_section_list, info );
		if( 0 > ret )
		{
			return ret;
		}
	}
	else if( info->struct_id == STRUCT_TYPE_PE_FILL_DATA )
	{
		all_pe_struct_infos->hdr_fill_data = info; 
	}

	return 0;
}

void *find_pe_struct_info( ppe_struct_info_list all_pe_struct_infos, void *info )
{
	struct_infos *find_info; 

	find_info = ( struct_infos* )info; 

	if( find_info->struct_id == STRUCT_TYPE_PE_DOS_HEADER )
	{
		return all_pe_struct_infos->pe_dos_hdr; 
	}

	if( find_info->struct_id == STRUCT_TYPE_PE_DOS_STUB )
	{
		return all_pe_struct_infos->pe_dos_stub; 
	}

	if( find_info->struct_id == STRUCT_TYPE_PE_NT_HEADER )
	{
		return all_pe_struct_infos->pe_nt_hdr; 
	}

	if( find_info->struct_id == STRUCT_TYPE_PE_OPTIONAL_HEADER )
	{
		return all_pe_struct_infos->pe_optional_hdr; 
	}

	if( find_info->struct_id == STRUCT_TYPE_PE_SECTION )
	{
		return find_list_element_by_compare( all_pe_struct_infos->pe_section_list, info, compare_struct_info );
	}
	else
	{
		ASSERT( FALSE ); 
		return NULL; 
	}
}

void *find_end_section( ppe_struct_info_list all_pe_struct_infos )
{
	return check_through_list_element( all_pe_struct_infos->pe_section_list, check_max_index_ele ); 
}

void *find_section_by_id( ppe_struct_info_list all_pe_struct_infos, dword index )
{
	struct_infos info; 
	info.struct_id = STRUCT_TYPE_PE_SECTION; 
	info.struct_index = index; 

	return find_list_element_by_compare( all_pe_struct_infos->pe_section_list, &info, compare_struct_info ); 
}

int32 add_section_hdr_offset( ppe_struct_info_list all_pe_struct_infos, dword add_len )
{
	int32 ret = 0;
	if( all_pe_struct_infos->pe_section_list == NULL )
	{
		ASSERT( FALSE ); 
		ret = -1;
		goto _error_ret; 
	}
	else
	{
		dword count = 0; 
		struct_infos *pe_section; 
		PIMAGE_SECTION_HEADER sect_hdr; 
		ULONG virt_addr; 
		ULONG phys_addr; 

		pe_section = find_section_by_id( all_pe_struct_infos, count ); 
		if( pe_section == NULL )
		{
			if( count == 0 )
			{
				ASSERT( FALSE ); 
				ret = -1;
				goto _error_ret; 
			}

			goto _error_ret; 
		}

		sect_hdr = ( PIMAGE_SECTION_HEADER )pe_section->struct_data; 
		sect_hdr->PointerToRawData += add_len; 
		sect_hdr->VirtualAddress += add_len; 

		sect_hdr->PointerToRawData = ( sect_hdr->PointerToRawData + PAGE_SIZE - 1 ) & ( ~( PAGE_SIZE - 1 ) ); 
		sect_hdr->VirtualAddress = ( sect_hdr->VirtualAddress + PAGE_SIZE - 1 ) & ( ~( PAGE_SIZE - 1 ) ); 

#define ALIGN_LEN 0x200
		virt_addr = ( sect_hdr->VirtualAddress + sect_hdr->Misc.VirtualSize + ALIGN_LEN - 1 ) & (  ~( ALIGN_LEN - 1 ) );  
		phys_addr = sect_hdr->PointerToRawData + sect_hdr->SizeOfRawData; 

		for( ; ; )
		{
			pe_section = find_section_by_id( all_pe_struct_infos, count ); 
			if( pe_section == NULL )
			{
				if( count == 0 )
				{
					ASSERT( FALSE ); 
					ret = -1;
					goto _error_ret; 
				}

				break; 
			}

			sect_hdr = ( PIMAGE_SECTION_HEADER )pe_section->struct_data; 
			sect_hdr->PointerToRawData = phys_addr; 
			sect_hdr->VirtualAddress = virt_addr; 

			phys_addr += sect_hdr->SizeOfRawData; 
			virt_addr += sect_hdr->Misc.VirtualSize; 
			virt_addr = ( virt_addr + ALIGN_LEN - 1 ) & (  ~( ALIGN_LEN - 1 ) );  

			count ++; 
		}
	}

_error_ret:
	return ret; 
}

int32 write_pe_structs_to_file( ppe_struct_info_list all_pe_struct_infos, char *file_name, char*org_file_name )
{
	int ret = 0;
	HANDLE hfile;
	dword writed;
	CHAR bak_file_name[ MAX_PATH ]; 

	ASSERT( NULL != file_name );
	ASSERT( NULL != all_pe_struct_infos );

	if( all_pe_struct_infos == NULL )
	{
		ASSERT( FALSE ); 
		ret = -1;
		goto _error_ret; 
	}
	
	strcpy( bak_file_name, file_name ); 
	strcat( bak_file_name, "bak" ); 
	CopyFile( file_name, bak_file_name, FALSE ); 

	hfile = CreateFile( file_name, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, 0, NULL );
	if( INVALID_HANDLE_VALUE == hfile )
	{
		ret = -1;
		goto _error_ret; 
	}

	if( all_pe_struct_infos->pe_dos_hdr == NULL )
	{
		ASSERT( FALSE ); 
		ret = -1;
		goto _error_ret; 
	}
	else
	{
		ret = WriteFile( hfile, all_pe_struct_infos->pe_dos_hdr->struct_data, sizeof( IMAGE_DOS_HEADER )/*all_pe_struct_infos->pe_dos_hdr->data_len*/, &writed, NULL );
		if( FALSE == ret || writed != sizeof( IMAGE_DOS_HEADER )/*all_pe_struct_infos->pe_dos_hdr->data_len*/ )
		{
			ret = -1;
			goto _error_ret; 
		}
	}

	if( all_pe_struct_infos->pe_dos_stub == NULL )
	{
		ASSERT( FALSE ); 
		ret = -1;
		return -1; 
	}
	else
	{
		ret = WriteFile( hfile, all_pe_struct_infos->pe_dos_stub->struct_data, all_pe_struct_infos->pe_dos_stub->data_len, &writed, NULL );
		if( FALSE == ret || writed != all_pe_struct_infos->pe_dos_stub->data_len )
		{
			ret = -1;
			goto _error_ret; 
		}
	}

	if( all_pe_struct_infos->pe_nt_hdr == NULL )
	{
		ASSERT( FALSE ); 
		ret = -1;
		goto _error_ret; 
	}
	else
	{
		DWORD nt_sig = IMAGE_NT_SIGNATURE; 
		ret = WriteFile( hfile, &nt_sig, sizeof( nt_sig )/*all_pe_struct_infos->pe_nt_hdr->data_len*/, &writed, NULL );
		if( FALSE == ret || writed != sizeof( nt_sig ) )
		{
			ret = -1;
			goto _error_ret; 
		}

		ret = WriteFile( hfile, all_pe_struct_infos->pe_nt_hdr->struct_data, sizeof( IMAGE_FILE_HEADER )/*all_pe_struct_infos->pe_nt_hdr->data_len*/, &writed, NULL );
		if( FALSE == ret || writed != sizeof( IMAGE_FILE_HEADER ) )
		{
			ret = -1;
			goto _error_ret; 
		}
	}

	if( all_pe_struct_infos->pe_optional_hdr == NULL )
	{
		ASSERT( FALSE ); 
		ret = -1;
		goto _error_ret; 
	}
	else
	{
		ret = WriteFile( hfile, all_pe_struct_infos->pe_optional_hdr->struct_data, sizeof( IMAGE_OPTIONAL_HEADER ), &writed, NULL );
		if( FALSE == ret || writed != sizeof( IMAGE_OPTIONAL_HEADER ) )
		{
			ret = -1;
			goto _error_ret; 
		}
	}

	if( all_pe_struct_infos->pe_section_list == NULL )
	{
		ASSERT( FALSE ); 
		ret = -1;
		goto _error_ret; 
	}
	else
	{
		dword count = 0; 
		struct_infos *pe_section; 

		for( ; ; )
		{
			pe_section = find_section_by_id( all_pe_struct_infos, count ); 
			if( pe_section == NULL )
			{
				if( count == 0 )
				{
					ASSERT( FALSE ); 
					ret = -1;
					goto _error_ret; 
				}

				break; 
			}
			else
			{
				ret = WriteFile( hfile, pe_section->struct_data, sizeof( IMAGE_SECTION_HEADER ), &writed, NULL );
				if( FALSE == ret || writed != sizeof( IMAGE_SECTION_HEADER )/*pe_section->data_len*/ )
				{
					ret = -1;
					goto _error_ret; 
				}
			}

			count ++; 
		}
	}

	if( all_pe_struct_infos->pe_data == NULL )
	{
		ASSERT( FALSE ); 
		ret = -1;
		goto _error_ret; 
	}
	else
	{
		ret = WriteFile( hfile, all_pe_struct_infos->hdr_fill_data->struct_data, all_pe_struct_infos->hdr_fill_data->data_len, &writed, NULL );
		if( FALSE == ret || writed != all_pe_struct_infos->hdr_fill_data->data_len )
		{
			ret = -1;
			goto _error_ret; 
		}
	}

	{
		struct_infos *pe_section; 
		dlist *listitem;
		DWORD ptr; 
		PIMAGE_SECTION_HEADER sect_hdr; 
		dword count = 0; 
		int32 diff; 

		pe_section = find_section_by_id( all_pe_struct_infos, count ); 
		ASSERT( NULL != pe_section ); 

		sect_hdr = ( PIMAGE_SECTION_HEADER )pe_section->struct_data; 

		ptr = SetFilePointer( hfile, sect_hdr->PointerToRawData, NULL, FILE_BEGIN );
		if( 0xffffffff == ptr )
		{
			ASSERT( FALSE ); 
			ret = -1;
			goto _error_ret; 
		}

		diff = ptr - sect_hdr->PointerToRawData; 

		for( ; ; )
		{
			pe_section = find_section_by_id( all_pe_struct_infos, count ); 
			if( pe_section == NULL )
			{
				if( count == 0 )
				{
					ASSERT( FALSE ); 
					ret = -1;
					goto _error_ret; 
				}

				break; 
			}
			else
			{
				if( pe_section->added == FALSE )
				{
					ret = WriteFile( hfile, pe_section->param3, pe_section->param4, &writed, NULL );
					if( FALSE == ret || writed != pe_section->param4 )
					{
						dword err = GetLastError(); 
						ret = -1;
						goto _error_ret; 
					}
				}
				else
				{
					PIMAGE_SECTION_HEADER sect_hdr; 
					char *new_sect_data; 

					sect_hdr = ( PIMAGE_SECTION_HEADER )pe_section->struct_data; 

					new_sect_data = malloc( sect_hdr->SizeOfRawData ); 
					if( NULL == new_sect_data )
					{
						ret = -ENOMEM; 
						goto _error_ret; 
					}

					memset( new_sect_data, 0xcc, sect_hdr->SizeOfRawData ); 
					ret = WriteFile( hfile, new_sect_data, sect_hdr->SizeOfRawData, &writed, NULL );
					if( FALSE == ret || writed != sect_hdr->SizeOfRawData )
					{
						ret = -1;
						goto _error_ret; 
					}
				}
			}

			count ++; 
		}
	}

	CloseHandle( hfile ); 
	MessageBox( NULL, "Write new pe file successfully. \n", "PE-Master", 0 ); 
	return 0; 

_error_ret:
	if( INVALID_HANDLE_VALUE != hfile )
	{
		CloseHandle( hfile );
	}

	DeleteFile( file_name ); 
	MessageBox( NULL, "Write new pe file failed. \n", "PE-Master", 0 ); 

	return ret;
}

int32 del_added_info( list_element element )
{
	struct_infos *info; 
	
	info = ( struct_infos* )element; 

	if( info == NULL )
	{
		return -1; 
	}

	if( info->added == TRUE )
	{
		ASSERT( info->struct_data != NULL ); 

		free( info->struct_data ); 
	}

	//del_record_info( info ); 
}

int32 destroy_pe_writer( ppe_struct_info_list all_pe_struct_infos )
{
	int32 ret;

	if( NULL == all_pe_struct_infos->pe_section_list )
	{
		ASSERT( FALSE ); 
		free( all_pe_struct_infos ); 
		return 0;
	}
	
	ret = destroy_list( all_pe_struct_infos->pe_section_list, del_added_info );
	free( all_pe_struct_infos ); 
	all_pe_struct_infos = NULL;
	return 0;
}

int32 clean_overlapped_data( PIMAGE_OPTIONAL_HEADER opt_hdr, dword offset, dword len )
{
	int32 i; 
	dword overlap_begin; 
	dword overlap_end; 

	for( i = 0; i < IMAGE_NUMBEROF_DIRECTORY_ENTRIES; i ++ )
	{
//#define IMAGE_DIRECTORY_ENTRY_EXPORT          0   // Export Directory
//#define IMAGE_DIRECTORY_ENTRY_IMPORT          1   // Import Directory
//#define IMAGE_DIRECTORY_ENTRY_RESOURCE        2   // Resource Directory
//#define IMAGE_DIRECTORY_ENTRY_EXCEPTION       3   // Exception Directory
//#define IMAGE_DIRECTORY_ENTRY_SECURITY        4   // Security Directory
//#define IMAGE_DIRECTORY_ENTRY_BASERELOC       5   // Base Relocation Table
//#define IMAGE_DIRECTORY_ENTRY_DEBUG           6   // Debug Directory
//		//      IMAGE_DIRECTORY_ENTRY_COPYRIGHT       7   // (X86 usage)
//#define IMAGE_DIRECTORY_ENTRY_ARCHITECTURE    7   // Architecture Specific Data
//#define IMAGE_DIRECTORY_ENTRY_GLOBALPTR       8   // RVA of GP
//#define IMAGE_DIRECTORY_ENTRY_TLS             9   // TLS Directory
//#define IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG    10   // Load Configuration Directory
//#define IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT   11   // Bound Import Directory in headers
//#define IMAGE_DIRECTORY_ENTRY_IAT            12   // Import Address Table
//#define IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT   13   // Delay Load Import Descriptors
//#define IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR 14   // COM Runtime descriptor

		if( i == IMAGE_DIRECTORY_ENTRY_EXPORT || 
			i == IMAGE_DIRECTORY_ENTRY_IMPORT || 
			i == IMAGE_DIRECTORY_ENTRY_BASERELOC || 
			i == IMAGE_DIRECTORY_ENTRY_DEBUG || 
			i == IMAGE_DIRECTORY_ENTRY_IAT || 
			i == IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR || 
			i == IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT || 
			i == IMAGE_DIRECTORY_ENTRY_TLS )
		{
			continue; 
		}

		if( opt_hdr->DataDirectory[ i ].VirtualAddress > offset )
		{
			overlap_begin = opt_hdr->DataDirectory[ i ].VirtualAddress; 
		}
		else
		{
			overlap_begin = offset; 
		}

		if( ( offset + len ) > opt_hdr->DataDirectory[ i ].VirtualAddress + opt_hdr->DataDirectory[ i ].Size )
		{
			overlap_end = opt_hdr->DataDirectory[ i ].VirtualAddress + opt_hdr->DataDirectory[ i ].Size; 
		}
		else
		{
			overlap_end = offset + len; 
		}

		if( overlap_end > overlap_begin )
		{
			opt_hdr->DataDirectory[ i ].VirtualAddress = 0; 
			opt_hdr->DataDirectory[ i ].Size = 0; 
		}
	}

	return 0; 
}

int32 add_new_section( PIMAGE_SECTION_HEADER sect_hdr, ppe_struct_info_list pe_write_info, file_analyzer *analyzer )
{
	int32 ret; 
	PIMAGE_SECTION_HEADER sect_output = NULL; 
	PIMAGE_SECTION_HEADER end_sect_hdr; 
	struct_infos *new_section_info = NULL; 
	struct_infos *end_section; 
	
	if( pe_write_info == NULL )
	{
		ASSERT( FALSE ); 
		return -1; 
	}

	end_section = find_end_section( pe_write_info ); 
	if( end_section == NULL )
	{
		ASSERT( FALSE ); 
		return -1; 
	}

	end_sect_hdr = end_section->struct_data; 
	if( end_sect_hdr == NULL )
	{
		ASSERT( FALSE ); 
		return -1; 
	}

	memcpy( sect_hdr->Name, NEW_SECTION_NAME_PREFIX, sizeof( NEW_SECTION_NAME_PREFIX ) ); 
	sect_hdr->NumberOfLinenumbers = 0; 
	sect_hdr->NumberOfRelocations = 0; 
	sect_hdr->PointerToLinenumbers = 0; 
	sect_hdr->PointerToRelocations = 0; 
	if( sect_hdr->Characteristics == 0 )
	{
		sect_hdr->Characteristics = DEFAULT_SECTION_CHARS; 
	}

	sect_hdr->PointerToRawData = end_sect_hdr->PointerToRawData + end_sect_hdr->SizeOfRawData; 
	sect_hdr->VirtualAddress = ( end_sect_hdr->VirtualAddress + end_sect_hdr->Misc.VirtualSize + PAGE_SIZE ) & ( ~( PAGE_SIZE - 1 ) ); 
	if( sect_hdr->VirtualAddress < sect_hdr->PointerToRawData )
	{
		sect_hdr->VirtualAddress = sect_hdr->PointerToRawData; 
	}

	sect_hdr->SizeOfRawData = ( sect_hdr->Misc.VirtualSize + PAGE_SIZE ) & ( ~( PAGE_SIZE - 1 ) ); 

	sect_output = ( PIMAGE_SECTION_HEADER )malloc( sizeof( *sect_output ) );
	if( NULL == sect_output )
	{
		return -ENOMEM; 
	}

	ret = add_new_record_info( &new_section_info, sizeof( *new_section_info ) ); 
	if( 0 > ret )
	{
		goto __error; 
	}
	
	memcpy( sect_output, sect_hdr, sizeof( *sect_output ) ); 
	new_section_info->data_len = sizeof( *sect_output ); 
	new_section_info->added = TRUE; 
	new_section_info->struct_data = sect_output; 
	new_section_info->struct_context = analyzer; 
	new_section_info->struct_id = STRUCT_TYPE_PE_SECTION; 
	new_section_info->struct_index = end_section->struct_index + 1; 
	new_section_info->param3 = 0; 
	new_section_info->param4 = 0; 

	add_pe_struct_info( pe_write_info, new_section_info ); 
	ASSERT( NULL != pe_write_info->pe_nt_hdr && 
		NULL != pe_write_info->pe_nt_hdr->struct_data ); 

	//ret = add_section_hdr_offset( pe_write_info, sizeof( IMAGE_SECTION_HEADER ) ); 
	//if( 0 > ret )
	//{
	//	return ret; 
	//}

	( ( PIMAGE_FILE_HEADER )pe_write_info->pe_nt_hdr->struct_data )->NumberOfSections += 1; 
	( ( PIMAGE_OPTIONAL_HEADER )pe_write_info->pe_optional_hdr->struct_data )->SizeOfImage += sect_output->SizeOfRawData; 
	clean_overlapped_data( ( PIMAGE_OPTIONAL_HEADER )pe_write_info->pe_optional_hdr->struct_data, pe_write_info->hdr_fill_data->struct_data - analyzer->all_file_data, sizeof( IMAGE_SECTION_HEADER ) ); 
	return ret; 

__error:
	if( sect_output == NULL )
	{
		free( sect_output ); 
	}

	if( new_section_info != NULL )
	{
		del_record_info( new_section_info ); 
	}

	return ret; 
}
