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

#ifndef __COMMON_ANALYZE_H__
#define __COMMON_ANALYZE_H__

#define PE_FILE_TYPE 0x4d5a
#define LIB_FILE_TYPE 0x02
#define COFF_FILE_TYPE 0x041c

#ifdef __cplusplus
extern "C" {
#endif

typedef struct __code_infos
{
	char *func_name;
	byte *func_code;
	dword func_code_len;

} code_infos;

typedef struct __sym_infos
{
	char *sym_name;
	byte *sym_data;
	dword sym_data_len;
} sym_infos;

typedef struct __struct_infos
{
	byte *struct_data;
	dword struct_id;
	dword struct_index;
	dword struct_context;
	dword data_len;
	char added; 
	dword param2;
	dword param3;
	dword param4; 
} struct_infos;

typedef struct __error_infos
{
	dword err_code;
	char *desc;
} error_infos;

typedef struct __obj_file_info
{
	char *file_name;
	byte *file_data;
	dword file_data_len;
} obj_file_info;

typedef struct __name_info
{
	char *name;
	dword name_order;
} name_info;

typedef int ( *export_import_info_proc )( name_info *info, void *context );
typedef int ( *sym_info_proc )( sym_infos *sym_info, void *context );
typedef int ( *code_info_proc )( code_infos *sym_info, void *context );
typedef int ( *struct_info_proc )( struct_infos *struct_info, void *context );
typedef int ( *obj_file_info_proc )( obj_file_info *file_info, void *context );
typedef int ( *error_handler_proc )( error_infos *err_info );

typedef struct __pe_struct_info_list
{
	struct_infos *pe_dos_hdr; 
	struct_infos *pe_dos_stub; 
	struct_infos *pe_nt_hdr; 
	struct_infos *pe_optional_hdr; 
	dlist *pe_section_list; 
	struct_infos *hdr_fill_data; 
	struct_infos *pe_data; 
} pe_struct_info_list, *ppe_struct_info_list; 

typedef struct __file_analyzer
{
	sym_info_proc syms_analyze;
	sym_info_proc strs_analyze;
	code_info_proc code_analyze;
	obj_file_info_proc obj_file_analyze;
	struct_info_proc struct_analyze;
	export_import_info_proc name_analyze;
	error_handler_proc error_handler;
	void *context;
	byte *all_file_data;
	dword file_data_len;
	dword file_type;
	ppe_struct_info_list pe_write_info; 
} file_analyzer;

#ifdef __cplusplus
}
#endif

#endif //__COMMON_ANALYZE_H__