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
#ifndef __PE_WRITER_H__
#define __PE_WRITER_H__

#define DEFAULT_SECTION_CHARS 0xE00000E0
#define NEW_SECTION_NAME_PREFIX "pemastr"
#define PAGE_SIZE 0x1000

#ifdef __cplusplus
extern "C" {
#endif

int32 init_pe_writer( ppe_struct_info_list *all_pe_struct_infos ); 
int32 add_pe_struct_info( ppe_struct_info_list all_pe_struct_infos, struct_infos *info ); 
void *find_pe_struct_info( ppe_struct_info_list all_pe_struct_infos, void *info ); 
void *find_end_section( ppe_struct_info_list all_pe_struct_infos ); 
int32 write_pe_structs_to_file( ppe_struct_info_list all_pe_struct_infos, char *file_name, char *org_file_name ); 
int32 destroy_pe_writer( ppe_struct_info_list all_pe_struct_infos );
int32 add_new_section( PIMAGE_SECTION_HEADER sect_hdr, ppe_struct_info_list pe_write_info, file_analyzer *analyzer ); 


#ifdef __cplusplus
}
#endif

#endif //__PE_WRITER_H__

