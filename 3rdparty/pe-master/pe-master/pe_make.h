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
#ifndef __PE_MAKE_H__
#define __PE_MAKE_H__

#define FILE_ALIGN_MODE 0x01
#define MEM_ALIGN_MODE 0x02
#define CODE_ENTRY_POINT 0x00000001

#ifdef __cplusplus
extern "C" {
#endif

dword get_aligned_val( byte* pe_file_data, dword org_val, dword mode );
int32 create_pe_file( char *file_name, dword *pe_handle );
int32 add_codes( dword pe_handle, byte *codes, dword code_len, dword flags );
int32 init_pe_make();
int32 uninit_pe_make();

#ifdef __cplusplus
}
#endif

#endif //__PE_MAKE_H__