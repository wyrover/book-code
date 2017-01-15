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
#include "coff_file_analyzer.h"
#include "lib_file_analyzer.h"
#include "lib_struct_analyzer.h"

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

int32 coff_sect_hdr_analyze( const coff_sect_hdr *sect_hdr )
{

	sect_hdr->flags;
	sect_hdr->ln_table_offset;

	return 0;
}