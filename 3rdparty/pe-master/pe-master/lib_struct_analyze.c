#include "common.h"
#include "common_analyze.h"
#include "lib_struct_analyze.h"

int32 analyze_lib_section_str_table( byte *data, dword data_len, file_analyzer *analyzer )
{
	int ret;
	char *syms_str_table;
	dword syms_str_table_len;
	dword syms_str_offset;

	syms_str_table = ( byte* )( data + offset );
	syms_str_table_len = atoi( section1->Size ) - ( sizeof( dword ) * syms_num + sizeof( dword ) );

	syms_str_offset = 0;
	for(;; )
	{
		syms_str_table;
		syms_str_offset += strlen( syms_str_table ) + sizeof( char );
		syms_str_table += strlen( syms_str_table ) + sizeof( char );

		assert( syms_str_offset <= syms_str_table_len );
		if( syms_str_offset == syms_str_table_len )
		{
			break;
		}
	}
	return 0;
}

int32 coff_optional32_hdr_analyze( const coff_opt_hdr28 *opt_hdr )
{
	coff_opt_hdr28 *opt_hdr28;

	opt_hdr28 = opt_hdr;

	opt_hdr28->magic == 0x010b; //exe
	opt_hdr28->magic == 0x0107; //rom 
	opt_hdr28->entry;
	opt_hdr28->version;
	opt_hdr28->text_base;

	return 0;
}

int32 coff_file_hdr_analyze( const coff_file_hdr *file_hdr )
{

	ASSERT( NULL != file_hdr );
	file_hdr->time;
	file_hdr->sect_num;
	file_hdr->syms_num;
	file_hdr->syms_offset;

	return 0;
}

int32 coff_sect_hdr_analyze( const coff_section_hdr *sect_hdr )
{

	sect_hdr->flags;
	sect_hdr->ln_table_offset;

	return 0;
}