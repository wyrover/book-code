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
#include "lib_file_analyzer.h"

#define LIB_FILE_HEADER "!<arch>\n"
#define STRTAB_END_SIGN "/\n"

int32 callback check_lib_file_type( byte *data, dword data_len )
{
	int ret;

	ASSERT( NULL != data );
	ASSERT( 0 < data_len );

	if( CONST_STR_LEN( LIB_FILE_HEADER ) > data_len )
	{
		return -1;
	}

	if( 0 == memcmp( LIB_FILE_HEADER, data, CONST_STR_LEN( LIB_FILE_HEADER ) ) )
	{
		return LIB_FILE_TYPE;
	}

	return -1;
}

int locate_next_sect( lib_section_hdr **sect_hdr, unsigned long *cur_offset, unsigned long data_len )
{
	lib_section_hdr *__sect_hdr;
	dword offset;

	__sect_hdr = *sect_hdr;
	offset = *cur_offset;
	while( 0 != memcmp( __sect_hdr->end_of_header, LIB_SECT_HDR_END_SIGN, CONST_STR_LEN( LIB_SECT_HDR_END_SIGN ) ) )
	{
		offset += 1;
		if( offset == data_len )
		{
			return -1;
		}

		__sect_hdr = ( lib_section_hdr* )( ( byte* )__sect_hdr + 1 );
	}

	*cur_offset = offset;
	*sect_hdr = __sect_hdr;
	return 0;
}

#define LIB_HDR_FILLED_BYTE 0x20
void clean_hdr_filled_bytes( lib_section_hdr *hdr )
{
	int i;
	byte *data;
	
	data = ( byte* )hdr;

	for( i = 0; i < sizeof( lib_section_hdr ); i ++ )
	{
		if( LIB_HDR_FILLED_BYTE == data[ i ] )
		{
			data[ i ] = 0;
		}
	}
}

int32 analyze_lib_section1_data( byte *data, dword data_len, file_analyzer *analyzer )
{
	int32 i;
	int32 ret;
	byte *section1_data;
	dword syms_num;
	dword syms_offsets;
	dword sym_offset;
	dword offset;

	offset = 0;
	section1_data = data; // + offset;
	syms_num = *( dword* )section1_data;
	littelendian2bigendian( &syms_num, sizeof( syms_num ) );
	offset += sizeof( dword );
	syms_offsets = data + offset;
	for( i = 0; i < syms_num; i ++ )
	{
		sym_offset = *( dword* )( syms_offsets );
		littelendian2bigendian( &sym_offset, sizeof( sym_offset ) );
		syms_offsets += sizeof( dword );
	}
	return 0;
}

int analyze_obj_sections( lib_section_hdr *sect_hdr, dword obj_sect_num )
{
	int32 i;
	//for( i = 0; i < obj_sect_num; i ++ )
	//{
	//	obj_sect_offset[ i ];
	//}

	return 0;
}

int analyze_sym_idxs( word *syms_idx, dword syms_num )
{
	int32 i;
	for( i = 0; i < syms_num; i ++ )
	{
		syms_idx[ i ];
	}

	return 0;
}

int analyze_lib_section2( byte *section2_data, dword data_len )
{
	dword obj_sect_num;
	dword offset;
	dword syms_num;
	dword syms_idx;
	char *section2_str_table;
	dword section2_str_table_len;

	offset = 0;

	obj_sect_num = *( dword* )section2_data;
	offset += sizeof( dword );

	offset += sizeof( dword ) * obj_sect_num;
	syms_num = *( dword* )( section2_data + offset );

	offset += sizeof( dword );
	syms_idx = ( word* )( section2_data + offset );

	offset += sizeof( word ) * syms_num;
	section2_str_table = ( char* )( section2_data + offset );

	section2_str_table_len =data_len - ( ( obj_sect_num )* sizeof( dword ) + sizeof( dword ) + syms_num * sizeof( word ) + sizeof( dword ) );

	offset += section2_str_table_len;

	return 0;
}

int32 analyze_long_section_data( byte *data, dword data_len )
{
	//long_name_sect_strings = ( char* )( data + offset ) + sizeof( dword );
	return 0;
}

int analyze_lib_file_struct( byte *data, dword data_len, file_analyzer *analyzer )
{
	int ret = 0;
	lib_section_hdr sect_copy;
	lib_section_hdr *section1;
	lib_section_hdr *section2;
	dword sym_offset;
	char *sym_name;
	dword offset;

	byte *section1_data;
	byte *syms_offsets;
	byte *syms_str_table;
	dword syms_str_offset;
	dword syms_str_table_len;

	byte *section2_data;
	dword obj_sect_num;
	dword *obj_sect_offset;
	dword syms_num;
	word *syms_idx;
	byte* section2_str_table; 
	dword section2_str_offset;
	dword section2_str_table_len;

	lib_section_hdr *long_name_sect;
	byte* long_name_str_table;
	dword long_name_str_offset;
	dword long_name_str_table_len;

	lib_section_hdr *obj_file_sect;
	dword obj_file_name_offset;
	char *obj_file_name;

	char *long_name_sect_strings;

	int i;

	ASSERT( NULL != data );
	ASSERT( 0 <= data_len );

	if( 0 == data_len )
	{
		ret = -1;
		goto __error;
	}

	offset = 0;

	ret = check_lib_file_type( data, data_len );
	if( LIB_FILE_TYPE != ret )
	{
		ret = -1;
		goto __error;
	}

	offset += CONST_STR_LEN( LIB_FILE_HEADER );
	section1 = ( lib_section_hdr* )( data + offset );
	offset += sizeof( lib_section_hdr );

	sect_copy = *section1;

	clean_hdr_filled_bytes( &sect_copy );
	ASSERT( 0 == strcmp( sect_copy.name, "/" ) );
	ASSERT( 0 == memcmp( sect_copy.end_of_header, LIB_SECT_HDR_END_SIGN, CONST_STR_LEN( LIB_SECT_HDR_END_SIGN ) ) );

	if( NULL != analyzer->struct_analyze )
	{
		struct_infos *info;
		byte *__offset;
		dword data_size;
		ret = add_new_record_info( &info, sizeof( *info ) );
		if( 0 > ret )
		{
			goto __error;
		}

		info->struct_data = ( byte* )section1;
		info->struct_id = STRUCT_TYPE_LIB_SECTION1;
		info->struct_index = 0;
		info->struct_context = analyzer;

		analyzer->struct_analyze( info, analyzer->context );

		__offset = ( byte* )section1 + sizeof( lib_section_hdr );
		ret = add_new_record_info( &info, sizeof( *info ) );
		if( 0 > ret )
		{
			goto __error;
		}

		info->data_len = *( dword* )__offset;
		littelendian2bigendian( &info->data_len, sizeof( info->data_len ) );

		info->struct_data = __offset + sizeof( dword ); ;
		info->struct_id = STRUCT_TYPE_SYM_TABLE;
		info->struct_index = 0;
		info->struct_context = analyzer;

		__offset += info->data_len * sizeof( dword ) + sizeof( dword );
		analyzer->struct_analyze( info, analyzer->context );

		ret = add_new_record_info( &info, sizeof( *info ) );
		if( 0 > ret )
		{
			goto __error;
		}

		data_size = atoi( sect_copy.size );

		info->data_len = data_size - ( __offset - ( ( byte* )section1 + sizeof( lib_section_hdr ) ) );
		ASSERT( info->data_len > 0 );
		info->struct_data = __offset;
		info->struct_id = STRUCT_TYPE_SECTION1_STR_TABLE;
		info->struct_index = 0;
		info->struct_context = analyzer;

		analyzer->struct_analyze( info, analyzer->context );
	}

	offset += atoi( sect_copy.size );

	section2 = ( lib_section_hdr* )( data + offset );

	ret = locate_next_sect( &section2, &offset, data_len );
	if( 0 > ret )
	{
		return -1;
	}

	sect_copy = *section2;
	clean_hdr_filled_bytes( &sect_copy );
	assert( 0 == strcmp( sect_copy.name, "/" ) ); 

	if( NULL != analyzer->struct_analyze )
	{
		struct_infos *info;
		byte *__offset;
		dword data_size;

		ret = add_new_record_info( &info, sizeof( *info ) );
		if( 0 > ret )
		{
			goto __error;
		}

		info->struct_data = section2;
		info->struct_id = STRUCT_TYPE_LIB_SECTION2;
		info->struct_index = 0;
		info->struct_context = analyzer;

		analyzer->struct_analyze( info, analyzer->context );

		ret = add_new_record_info( &info, sizeof( *info ) );
		if( 0 > ret )
		{
			goto __error;
		}

		__offset = ( byte* )section2 + sizeof( lib_section_hdr );
		info->data_len = *( dword* )__offset;
		//littelendian2bigendian( &info->data_len, sizeof( info->data_len ) );
		info->struct_data = __offset + sizeof( dword ); ;
		info->struct_id = STRUCT_TYPE_OBJ_OFFSETS;
		info->struct_index = 0;
		info->struct_context = analyzer;

		__offset += info->data_len * sizeof( dword ) + sizeof( dword );

		analyzer->struct_analyze( info, analyzer->context );

		ret = add_new_record_info( &info, sizeof( *info ) );
		if( 0 > ret )
		{
			goto __error;
		}

		info->data_len = *( dword* )__offset;
		//littelendian2bigendian( &info->data_len, sizeof( info->data_len ) );
		info->struct_data = __offset + sizeof( dword ); ;
		info->struct_id = STRUCT_TYPE_SYM_INDEXES;
		info->struct_index = 0;
		info->struct_context = analyzer;

		__offset += info->data_len * sizeof( dword ) + sizeof( dword );
		analyzer->struct_analyze( info, analyzer->context );

		ret = add_new_record_info( &info, sizeof( *info ) );
		if( 0 > ret )
		{
			goto __error;
		}

		data_size = atoi( sect_copy.size );

		//littelendian2bigendian( &data_size, sizeof( data_size ) );

		info->data_len = data_size - ( __offset - ( ( byte* )section2 + sizeof( lib_section_hdr ) ) );
		ASSERT( info->data_len > 0 );
		info->struct_data = __offset;
		info->struct_id = STRUCT_TYPE_SECTION2_STR_TABLE;
		info->struct_index = 0;
		info->struct_context = analyzer;

		analyzer->struct_analyze( info, analyzer->context );
	}

	offset += sizeof( lib_section_hdr );

	obj_sect_num = *( dword* )( data + offset );
	offset += atoi( sect_copy.size );
	
	long_name_sect = ( lib_section_hdr* )( data + offset );

	ret = locate_next_sect( &long_name_sect, &offset, data_len );
	if( 0 > ret )
	{
		return -1;
	}

	sect_copy = *long_name_sect;
	clean_hdr_filled_bytes( &sect_copy );

	long_name_sect_strings = NULL;
	if( 0 == strcmp( sect_copy.name, "//" ) )
	{
		offset += sizeof( lib_section_hdr );
		long_name_sect_strings = data + offset + sizeof( dword );

		if( NULL != analyzer->struct_analyze )
		{
			struct_infos *info;
			ret = add_new_record_info( &info, sizeof( *info ) );
			if( 0 > ret )
			{
				goto __error;
			}

			info->struct_data = long_name_sect;
			info->struct_id = STRUCT_TYPE_LIB_SECTION_LONGNAME;
			info->struct_index = 0;
			info->struct_context = analyzer;

			analyzer->struct_analyze( info, analyzer->context );

			ret = add_new_record_info( &info, sizeof( *info ) );
			if( 0 > ret )
			{
				goto __error;
			}

			info->struct_data = ( byte* )long_name_sect + sizeof( lib_section_hdr ); ;
			info->struct_id = STRUCT_TYPE_LONGNAME_SECTION_STR_TABLE;
			info->struct_index = 0;
			info->struct_context = analyzer;
			info->data_len = atoi( long_name_sect->size );
			//littelendian2bigendian( &info->data_len, sizeof( info->data_len ) );

			analyzer->struct_analyze( info, analyzer->context );
		}

		offset += atoi( sect_copy.size );
	}
	
	for( i = 0; i < obj_sect_num; i ++ )
	{
		obj_file_sect = ( lib_section_hdr* )( data + offset );
		
		ret = locate_next_sect( &obj_file_sect, &offset, data_len );
		if( 0 > ret )
		{
			return -1;
		}

		sect_copy = *obj_file_sect;
		clean_hdr_filled_bytes( &sect_copy );

		if( obj_file_sect->name[0] ==  '/' )
		{
			obj_file_name_offset = atoi( &sect_copy.name[ 1 ] );
			obj_file_name = &long_name_sect_strings[ obj_file_name_offset ];
		}
		else
		{
			char *obj_file_name_end;
			obj_file_name_end = strchr( sect_copy.name, '/' );
			if( NULL != obj_file_name_end )
			{
				*obj_file_name_end = '\0';
			}

			obj_file_name = sect_copy.name;
		}

		if( NULL != analyzer->struct_analyze )
		{
			struct_infos *info;
			ret = add_new_record_info( &info, sizeof( *info ) );
			if( 0 > ret )
			{
				goto __error;
			}

			info->struct_data = obj_file_sect;
			info->struct_id = STRUCT_TYPE_LIB_SECTION_OBJ_FILE;
			info->struct_index = i;
			info->struct_context = analyzer;

			analyzer->struct_analyze( info, analyzer->context );
		}

		if( NULL != analyzer->obj_file_analyze )
		{
			obj_file_info info;
			info.file_name = obj_file_name;
			info.file_data = ( byte* )obj_file_sect + sizeof( lib_section_hdr );
			info.file_data_len = atoi( obj_file_sect->size );
			analyzer->obj_file_analyze( &info, analyzer->context );
		}

		analyze_coff_file_struct( ( byte* )obj_file_sect + sizeof( lib_section_hdr ), atoi( obj_file_sect->size ), i, analyzer );

		offset += sizeof( lib_section_hdr );
		offset += atoi( obj_file_sect->size );
	}

__error:
	return ret;
}

int just_search_symbols( char *data, dword data_len )
{
	#define remain_len( pointer ) ( dword )( data + data_len - pointer )

	byte *func_name_begin_dash;
	byte *func_name_begin_question;
	byte *func_name_begin;
	byte *func_name_end;
	func_name_begin = data;
	for( ; ; )
	{
		func_name_begin_question = memchr( func_name_begin, '?', remain_len( func_name_begin ) );
		func_name_begin_dash = memchr( func_name_begin, '_', remain_len( func_name_begin ) );

		if( func_name_begin_question != NULL && func_name_begin_dash > func_name_begin_question )
		{
			func_name_begin = func_name_begin_question;
		}
		else
		{
			func_name_begin = func_name_begin_dash;
		}

		if( NULL == func_name_begin )
		{
			break;
		}

		func_name_end = memchr( func_name_begin, 0, remain_len( func_name_begin ) );
		if( NULL == func_name_end )
		{
			break;
		}

		func_name_begin = func_name_end + sizeof( char );
	}

	return 0;
}

