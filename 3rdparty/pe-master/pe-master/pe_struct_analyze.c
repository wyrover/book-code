#include "pe_struct_analyze.h"
#include <Windows.h>
#include <WinNT.h>
#include "common.h"

int32 get_standard_nt_hdr_desc( IMAGE_NT_FILE_HEADER *hdr )
{
switch( file_hdr->Machine )
	{
	case IMAGE_FILE_MACHINE_I386:
		break;
	case 0x014d:
		break;
	case 0x014e:
		break;
	case 0x0160:
		break;
	case IMAGE_FILE_MACHINE_R3000:
		break;
	case IMAGE_FILE_MACHINE_R4000:
		break;
	case IMAGE_FILE_MACHINE_R10000:
		break;
	case IMAGE_FILE_MACHINE_ALPHA:
		break;
	case IMAGE_FILE_MACHINE_POWERPC:
		break;
	default:
		ret = -1;
		goto error;
		break;
	}
	return 0;
}

int32 analyze_pe_func_info()
{
	import_info->Hint;
		import_info->Name;
}

int32 aa()
{
			res_data_out->CodePage = res_data->CodePage;
	res_data_out->Size = res_data->Size;
}

int32 get_standard_pe_optional_hdr_desc( PIMAGE_OPTIONAL_HEADER hdr )
{
	option_hdr->AddressOfEntryPoint;
	option_hdr->ImageBase;
	option_hdr->SectionAlignment;
	option_hdr->FileAlignment;
	option_hdr->MinorSubsystemVersion;
	option_hdr->SizeOfImage;
	option_hdr->SizeOfHeaders;
	option_hdr->CheckSum; //CheckSumMappedFile()
		option_hdr->SizeOfStackReserve;
	option_hdr->SizeOfStackCommit;
	option_hdr->SizeOfHeapReserve;
	option_hdr->SizeOfHeapCommit;
	return 0;
}

int32 get_standard_pe_optional_hdr_subsys_desc( PIMAGE_OPTIONAL_HEADER hdr )
{
	switch( option_hdr->Subsystem )
	{
	case IMAGE_SUBSYSTEM_NATIVE:
		break;
	case IMAGE_SUBSYSTEM_WINDOWS_GUI:
		break;
	case IMAGE_SUBSYSTEM_WINDOWS_CUI:
		break;
	case IMAGE_SUBSYSTEM_OS2_CUI:
		break;
	case IMAGE_SUBSYSTEM_POSIX_CUI:
		break;
	default:
		if( NULL != analyzer->error_hander )
		{
			analyzer->error_handler( ERR_PE_OPTIONAL_HEADER_SUBSYS );
		}
		ret = -1;
		goto error;
		break;
	}
	return 0;
}

int32 get_standard_nt_hdr_characer_desc( file_hdr )
{
	if( file_hdr->Characteristics & IMAGE_FILE_RELOCS_STRIPPED )
	{

	}

	if( file_hdr->Characteristics & IMAGE_FILE_EXECUTABLE_IMAGE )
	{

	}

	if( file_hdr->Characteristics & IMAGE_FILE_LINE_NUMS_STRIPPED )
	{

	}

	if( file_hdr->Characteristics & IMAGE_FILE_LOCAL_SYMS_STRIPPED )
	{

	}

	if( file_hdr->Characteristics & IMAGE_FILE_AGGRESIVE_WS_TRIM )
	{

	}

	if( file_hdr->Characteristics & IMAGE_FILE_LARGE_ADDRESS_AWARE )
	{

	}

	if( file_hdr->Characteristics & IMAGE_FILE_BYTES_REVERSED_LO )
	{

	}

	if( file_hdr->Characteristics & IMAGE_FILE_32BIT_MACHINE )
	{

	}


	if( file_hdr->Characteristics & IMAGE_FILE_DEBUG_STRIPPED )
	{

	}

	if( file_hdr->Characteristics & IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP )
	{

	}

	if( file_hdr->Characteristics & IMAGE_FILE_NET_RUN_FROM_SWAP )
	{

	}

	if( file_hdr->Characteristics & IMAGE_FILE_SYSTEM )
	{

	}

	if( file_hdr->Characteristics & IMAGE_FILE_DLL )
	{

	}

	if( file_hdr->Characteristics & IMAGE_FILE_UP_SYSTEM_ONLY )
	{

	}

	if( file_hdr->Characteristics & IMAGE_FILE_BYTES_REVERSED_HI )
	{

	}
	return 0;
}

int32 get_section_infos( PIMAGE_SECTION_HEADER *sect_hdr )
{
	sect_hdr->Name;
	sect_hdr->Misc.VirtualSize;
	sect_hdr->VirtualAddress;
	sect_hdr->SizeOfRawData;
	sect_hdr->PointerToRawData;
	sect_hdr->PointerToRelocations;
	sect_hdr->PointerToLinenumbers;
	sect_hdr->NumberOfRelocations;
	sect_hdr->NumberOfLinenumbers;
	return 0;
}

int get_standard_sect_character_desc( PIMAGE_SECTION_HEADER hdr )
{
	if( sect_hdr->Characteristics & IMAGE_SCN_TYPE_NO_PAD )
	{
	}
	if( sect_hdr->Characteristics & IMAGE_SCN_CNT_CODE )
	{
	}
	if( sect_hdr->Characteristics & IMAGE_SCN_CNT_INITIALIZED_DATA )
	{
	}
	if( sect_hdr->Characteristics & IMAGE_SCN_CNT_UNINITIALIZED_DATA )
	{
	}
	if( sect_hdr->Characteristics & IMAGE_SCN_LNK_OTHER )
	{
	}
	if( sect_hdr->Characteristics & IMAGE_SCN_LNK_INFO )
	{
	}
	if( sect_hdr->Characteristics & IMAGE_SCN_LNK_REMOVE )
	{
	}
	if( sect_hdr->Characteristics & IMAGE_SCN_LNK_COMDAT )
	{
	}
	if( sect_hdr->Characteristics & IMAGE_SCN_NO_DEFER_SPEC_EXC )
	{
	}
	if( sect_hdr->Characteristics & IMAGE_SCN_GPREL )
	{
	}
	if( sect_hdr->Characteristics & IMAGE_SCN_MEM_FARDATA )
	{
	}
	if( sect_hdr->Characteristics & IMAGE_SCN_MEM_PURGEABLE )
	{
	}	
	if( sect_hdr->Characteristics & IMAGE_SCN_MEM_16BIT )
	{
	}
	if( sect_hdr->Characteristics & IMAGE_SCN_MEM_LOCKED )
	{
	}
	if( sect_hdr->Characteristics & IMAGE_SCN_MEM_PRELOAD )
	{
	}
	if( sect_hdr->Characteristics & IMAGE_SCN_ALIGN_1BYTES )
	{
	}
	if( sect_hdr->Characteristics & IMAGE_SCN_ALIGN_2BYTES )
	{
	}
	if( sect_hdr->Characteristics & IMAGE_SCN_ALIGN_4BYTES )
	{
	}
	if( sect_hdr->Characteristics & IMAGE_SCN_ALIGN_8BYTES )
	{
	}
	if( sect_hdr->Characteristics & IMAGE_SCN_ALIGN_16BYTES )
	{
	}
	if( sect_hdr->Characteristics & IMAGE_SCN_ALIGN_32BYTES )
	{
	}
	if( sect_hdr->Characteristics & IMAGE_SCN_ALIGN_64BYTES )
	{
	}
	if( sect_hdr->Characteristics & IMAGE_SCN_ALIGN_128BYTES )
	{
	}
	if( sect_hdr->Characteristics & IMAGE_SCN_ALIGN_256BYTES )
	{
	}
	if( sect_hdr->Characteristics & IMAGE_SCN_ALIGN_512BYTES )
	{
	}
	if( sect_hdr->Characteristics & IMAGE_SCN_ALIGN_1024BYTES )
	{
	}
	if( sect_hdr->Characteristics & IMAGE_SCN_ALIGN_2048BYTES )
	{
	}
	if( sect_hdr->Characteristics & IMAGE_SCN_ALIGN_4096BYTES )
	{
	}
	if( sect_hdr->Characteristics & IMAGE_SCN_ALIGN_8192BYTES )
	{
	}
	if( sect_hdr->Characteristics & IMAGE_SCN_LNK_NRELOC_OVFL )
	{
	}
	if( sect_hdr->Characteristics & IMAGE_SCN_MEM_DISCARDABLE )
	{
	}
	if( sect_hdr->Characteristics & IMAGE_SCN_MEM_NOT_CACHED )
	{
	}
	if( sect_hdr->Characteristics & IMAGE_SCN_MEM_NOT_PAGED )
	{
	}
	if( sect_hdr->Characteristics & IMAGE_SCN_MEM_SHARED )
	{
	}
	if( sect_hdr->Characteristics & IMAGE_SCN_MEM_EXECUTE )
	{
	}
	if( sect_hdr->Characteristics & IMAGE_SCN_MEM_READ )
	{
	}
	if( sect_hdr->Characteristics & IMAGE_SCN_MEM_WRITE )
	{
	}

	return 0;
}

int get_standard_data_dir_desc( PIMAGE_DATA_DIRECTORY data_dir, dword index, 
							   char * desc_out, dword *out_buf_len )
{
#define MAX_DIR_DESC_LEN 1024
	char *type_desc;
	char data_dir_desc[ MAX_DIR_DESC_LEN ];
	int32 length;

	switch( index )
	{
	case IMAGE_DIRECTORY_ENTRY_EXPORT:
		type_desc = "Export directory: ";
		break;
	case IMAGE_DIRECTORY_ENTRY_IMPORT:
		type_desc = "Import directory: ";
		break;
	case IMAGE_DIRECTORY_ENTRY_RESOURCE:
		type_desc = "Resource directory: ";
		break;
	case IMAGE_DIRECTORY_ENTRY_EXCEPTION:
		type_desc = "Exception directory: ";
		break;
	case IMAGE_DIRECTORY_ENTRY_SECURITY:
		type_desc = "Secrity directory: ";
		break;
	case IMAGE_DIRECTORY_ENTRY_BASERELOC:
		type_desc = "Base Relocating directory: ";
		break;
	case IMAGE_DIRECTORY_ENTRY_DEBUG:
		type_desc = "Debug directory: ";
		break;
	case IMAGE_DIRECTORY_ENTRY_GLOBALPTR:
		type_desc = "Global pointer directory: ";
		break;
	case IMAGE_DIRECTORY_ENTRY_TLS:
		type_desc = "Tls directory: ";
		break;
	case IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG:
		type_desc = "Load configure directory: ";
		break;
	case IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT:
		break;
	case IMAGE_DIRECTORY_ENTRY_IAT:
		type_desc = "Import address table directory: ";
		break;
	default:
		return -1;
	}

#define DESC_SPAN_SPACE 3

	sprintf( data_dir_desc, "%40s Size: %d, Virtual address: %d \n", type_desc, data_dir->Size, data_dir->VirtualAddress );

	if( strlen( data_dir_desc ) + sizeof( char ) > *out_buf_len )
	{
		*desc_out = '\0';
		*out_buf_len = strlen( data_dir_desc ) + sizeof( char );
		return -2;
	}

	strcpy( desc_out, data_dir_desc );
	*out_buf_len = strlen( data_dir_len ) + sizeof( char );
	return 0;
}

int32 analyze_import_table( PIMAGE_IMPORT_DESCRIPTOR *import_desc )
{

}

INT32 ErrorHandle( DWORD dwErrorCode )
{
	DWORD dwLastError;
	CHAR szMsg[ MAX_MSG_LEN ];

	dwLastError = GetLastError();
}

int aaa()
{
		res_dir->Characteristics;
	res_dir->TimeDateStamp;
	res_dir->MajorVersion;
	res_dir->MinorVersion;
	res_dir->NumberOfNamedEntries;
	res_dir->NumberOfIdEntries;

}

int32 analyze_res_dir( PIMAGE_RESOURCE_DIRECTORY_ENTRY data_entry )
{
	if( res_entry->NameIsString )
	{
		res_name = sect_data + res_entry->Name;
		memcpy( res_name_buf, res_name->NameString, res_name->Length * sizeof( WCHAR ) );
		res_name_buf[ res_name->Length ] = L'\0';
		ret = WideCharToMultiByte( 0, 0, res_name_buf, 0, res_name_buf, MAX_RES_NAME_LEN * 2, NULL, NULL );
	}
	else
	{
		res_entry->Id;
	}

	return 0;
}

int32 analyzer_res_data( PIMAGE_RESOURCE_DATA_ENTRY res_data )
{
	res_data->OffsetToData;
	res_data->Size;
	res_data->CodePage;
	res_data->Reserved;
}

int32 analyzer_export_table( )
{
export_table->Characteristics;
	export_table->TimeDateStamp;
	export_table->MajorVersion;
	export_table->MinorVersion;
	export_table->Name;
	export_table->Base;
	return 0;
}