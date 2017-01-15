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
#ifndef __PE_FILE_ANALYZER_H__
#define __PE_FILE_ANALYZER_H__

#define PE_HEADER_SIGN 0x4d5a

#define ERR_READ_OR_FILE_TO_SMALL 0xffff4d5a
#define ERR_PE_DOS_HEADER_SIGN 0x00014d5a
#define ERR_PE_NT_HEADER_SIGN 0x00024d5a
#define ERR_PE_OPTIONAL_HEADER_LEN 0x00034d5a
#define ERR_PE_NT_HEADER_MACHINE 0x00044d5a
#define ERR_PE_OPTIONAL_HEADER_SIGN 0x00054d5a
#define ERR_PE_OPTIONAL_HEADER_SUBSYS 0x00064d5a
#define ERR_PE_EXPORT_SYM_RVA_PARSING 0x00094d5a
#define ERR_PE_EXPORT_ADDRESS_RVA_PARSING 0x00094d5a
#define ERR_PE_EXPORT_ADDRESS_RVA_PARSING 0x00094d5a
#define ERR_PE_EXPORT_ADDRESS_RVA_PARSING 0x00094d5a
#define ERR_PE_EXPORT_ADDRESS_RVA_PARSING 0x00094d5a
#define ERR_PE_EXPORT_ADDRESS_RVA_PARSING 0x00094d5a

#define STRUCT_TYPE_PE_DOS_HEADER 0x4d5a0001
#define STRUCT_TYPE_PE_DOS_STUB 0x4d5a0002
#define STRUCT_TYPE_PE_NT_HEADER 0x4d5a0003
#define STRUCT_TYPE_PE_OPTIONAL_HEADER 0x4d5a0004
#define STRUCT_TYPE_PE_COPYRIGHT 0x4d5a000a
#define	STRUCT_TYPE_PE_IMPORT_BY_NAME 0x4d5a000b
#define STRUCT_TYPE_PE_SECTION 0x4d5a000d
#define STRUCT_TYPE_PE_SECTIONS_DATA 0x4d5a000e
#define STRUCT_TYPE_PE_FILL_DATA 0x4d5a000b

#define MAX_MSG_LEN 512
#define WINDOWS_PE_OPTIONAL_HEADER_SIGN 0x010b

#ifdef __cplusplus
extern "C" {
#endif

int32 callback check_pe_file_type( byte *data, dword data_len );
INT32 analyze_pe_file_struct( byte *data, dword data_len, file_analyzer *analyzer );

#ifdef __cplusplus
}
#endif

#endif //__PE_FILE_ANALYZER_H__