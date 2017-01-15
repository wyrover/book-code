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
#ifndef __FILE_ANALYZER_H__
#define __FILE_ANALYZER_H__

#define ERR_FILE_NOT_EXIST 0x0001000a

#ifdef __cplusplus
extern "C" {
#endif

int32 check_file_type( byte *data, dword data_len );
int init_analyzing( CHAR *filename, file_analyzer *analyzer );
struct_infos *find_struct_info( struct_infos *info );
struct_infos *find_struct_info_by_id( dword type, dword index );
int32 callback compare_struct_info( void *element1, void *element2 ); 
int32 callback check_max_index_ele( void *element1, void *element2 ); 

#ifdef __cplusplus
}
#endif
#endif //__FILE_ANALYZER_H__