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

#ifndef __LIB_FILE_ANALYZER_H__
#define __LIB_FILE_ANALYZER_H__

#include "coff_file_analyzer.h"

#define LIB_SECT_HDR_END_SIGN "`\n"

#define STRUCT_TYPE_LIB_SECTION1 0x00020001
#define STRUCT_TYPE_LIB_SECTION2 0x00020002
#define STRUCT_TYPE_LIB_SECTION_LONGNAME 0x00020003
#define STRUCT_TYPE_LIB_SECTION_OBJ_FILE 0x00020004
#define STRUCT_TYPE_SYM_TABLE 0x00020005
#define STRUCT_TYPE_SECTION1_STR_TABLE 0x00020006
#define STRUCT_TYPE_OBJ_OFFSETS 0x00020007
#define STRUCT_TYPE_SYM_INDEXES 0x00020008
#define STRUCT_TYPE_SECTION2_STR_TABLE 0x0002000c
#define STRUCT_TYPE_LONGNAME_SECTION_STR_TABLE 0x0002000d

typedef struct __lib_section_hdr
{
	char name[16];
	char time[12];
	char user_id[6];
	char group_id[6];
	char mode[8];
	char size[10];
	char end_of_header[2];

} lib_section_hdr;

typedef struct __section1_data{
	unsigned long SymbolNum;
	unsigned long *SymbolOffset;
	char *StrTable;
} section1_data;

typedef struct __section2_data{
	unsigned long ObjNum;
	unsigned long *ObjOffset;
	unsigned long SymbolNum;
	unsigned short *SymbolIdx;
	char *StrTable;
} section2_data;

#ifdef __cplusplus
extern "C" {
#endif

	void clean_hdr_filled_bytes( lib_section_hdr *hdr );
	int find_section_2( byte *data );
	int callback check_lib_file_type( byte *data, dword data_len );
	int read_obj_section( lib_section_hdr* sect, byte *data );
	int analyze_lib_file_struct( byte *data, dword data_len, file_analyzer *analyzer );

#ifdef __cplusplus
}
#endif

#endif //__LIB_FILE_ANALYZER_H__