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

#ifdef __cplusplus
extern "C" {
#endif

int32 check_pe_file_type( byte *data, dword data_len );
INT32 analyze_pe_file( byte *data, dword data_len, file_analyzer *analyzer );

#ifdef __cplusplus
}
#endif

#endif //__PE_FILE_ANALYZER_H__