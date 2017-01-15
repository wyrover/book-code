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

#ifndef __BANLANCE_PARALLEL_DLIST_H__
#define __BANLANCE_PARALLEL_DLIST_H__

typedef void* list_element;

typedef struct __dlist
{
	list_element info;
	struct __dlist *prev;
	struct __dlist *next;
} dlist;

#include "common.h"

typedef int ( *destroy_list_element_func )( list_element );
typedef int ( callback *list_ele_compare )( list_element element1, list_element element2 );

dlist* find_list_element_by_compare( dlist *destlist, list_element element, list_ele_compare compare_func );
int free_element_on_destroy( list_element element );
int add_list_element( dlist *destlist, list_element element );
int del_list_item( dlist *listitem, list_element *element );
dlist* find_list_element( dlist *destlist, list_element element );
dlist* get_list_item_ptr( list_element *list_item_member_addr );
int del_list_element2( dlist *destlist, int index, list_element *element );
int del_list_element( dlist *destlist, list_element element );
int init_list_element( dlist **dest_list );
int destroy_list( dlist *list_header, destroy_list_element_func des_func );
dlist* get_next_list( dlist *item );
dlist* get_prev_list( dlist *item );
dlist* get_next_list_element( dlist *item, list_element *element );
dlist* get_prev_list_elememt( dlist *item, list_element *element );

#endif