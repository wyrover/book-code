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

#ifndef __COFF_FILE_ANALYZER_H__
#define __COFF_FILE_ANALYZER_H__

#define STYP_TEXT 0x0020
#define STYP_DATA 0x0040
#define STYP_BSS 0x0080

#define SYM_PTR_TYPE 0x01
#define SYM_FUNC_TYPE 0x02 
#define SYM_ARRAY_TYPE 0x03
#define SYM_NONE_TYPE 0x00

#define SYM_NONE_STORE_TYPE 0
#define SYM_AUTOMATIC_STORE_TYPE 1
#define SYM_EXTERNAL_STORE_TYPE 2
#define SYM_STATIC_STORE_TYPE 3 //offset 0 is the section name
#define SYM_REGISTER_STORE_TYPE 4
#define SYM_MEMBER_OF_STRUCT_STORE_TYPE 8
#define SYM_STRUCT_TAG_STORE_TYPE 10
#define SYM_MEMBER_OF_UNION_STORE_TYPE 11 //value is order of symbol in the enum
#define SYM_UNION_TAG_STORE_TYPE 12
#define SYM_TYPE_DEFINITION_STORE_TYPE 13
#define SYM_FUNCTION_STORE_TYPE 13
#define SYM_FILE_STORE_TYPE 13

#define I386_COFF_FILE_MAGIC 0x014c
#define F_RELFLG 0x0001
#define F_EXEC 0x0002
#define F_LNNO 0x0004
#define F_LSYMS 0x0008
#define F_AR32WR 0x0100

#define STRUCT_TYPE_COFF_FILE_HEADER 0x014c0001
#define STRUCT_TYPE_COFF_OPTIONAL_28_HEADER 0x014c0002
#define STRUCT_TYPE_COFF_SECTION_HEADER 0x14c00003
#define STRUCT_TYPE_COFF_STR_TABLE 0x14c00004

#pragma pack( push )
#pragma pack( 1 )

typedef struct __coff_file_hdr{
  unsigned short magic;
  unsigned short sect_num;
  unsigned long  time;
  unsigned long  syms_offset;
  unsigned long  syms_num;
  unsigned short opt_hdr_size;
  unsigned short flags;
} coff_file_hdr;

typedef struct __coff_opt_hdr28{
  unsigned short magic;
  unsigned short version;
  unsigned long  text_size;
  unsigned long  init_data_size;
  unsigned long  uninit_data_size;
  unsigned long  entry;
  unsigned long  text_base;
  unsigned long  data_base;
} coff_opt_hdr28;


typedef struct __coff_sect_hdr{
  char           name[8];
  unsigned long  virt_size;
  unsigned long  virt_addr;
  unsigned long  size;
  unsigned long  sect_offset;
  unsigned long  sect_rel_offset;
  unsigned long  ln_table_offset;
  unsigned short rel_offset_num;
  unsigned short ln_num;
  unsigned long  flags;
  //unsigned long reserved;
} coff_sect_hdr;

typedef struct __coff_reloc{
  unsigned long  ulAddr;
  unsigned long  ulSymbol;
  unsigned short usType;
} coff_reloc;

typedef struct {
    unsigned long addr_symbol;
    unsigned short ln_no;
} coff_ln_info;

typedef struct __coff_sym_ent{
  union {
    char name[8];
    struct {
      unsigned long zero;
      unsigned long offset;
    } id;
  } sym_id;
  unsigned long value;
  short section;
  unsigned short type;
  unsigned char __class;
  unsigned char aux_num;
} coff_sym_ent;

typedef struct __coff_str_table
{
	dword size;
	char strings[1];
} coff_str_table;
#pragma pack( pop )

#ifdef __cplusplus
extern "C" {
#endif

	int32 callback check_coff_file_type( byte *data, dword data_len );

#ifdef __cplusplus
}
#endif

#endif //__COFF_FILE_ANALYZER_H__