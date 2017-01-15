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
#include "dlist.h"

int free_element_on_destroy( list_element element )
{
	free( element );
	return 0;
}

int add_list_element( dlist *destlist, list_element element )
{
	dlist *newlist;
	ASSERT( NULL != destlist );

	newlist = ( dlist* )malloc( sizeof( dlist ) );
	if( NULL == newlist )
	{
		return -E_OUTOFMEMORY;
	}

	memset( newlist, 0, sizeof( dlist ) );
	newlist->info = element;
	
	newlist->prev = destlist;
	newlist->next = destlist->next;
	if( NULL != destlist->next )
	{
		destlist->next->prev = newlist;
	}
	destlist->next = newlist;
	
	return 0;
}

int del_list_item( dlist *listitem, list_element *element )
{
	ASSERT( NULL != listitem );

	if( NULL != listitem->prev )
	{
		listitem->prev->next = listitem->next;
	}

	if( NULL != listitem->next )
	{
		listitem->next->prev = listitem->prev;
	}

	if( NULL != element )
	{
		*element = listitem->info;
	}
	free( listitem );
	return 0;
}

dlist* find_list_element( dlist *destlist, list_element element )
{
	dlist *listitem;
	ASSERT( NULL != destlist );

	listitem = destlist;
	for( ; NULL != listitem; listitem = listitem->next )
	{
		if( listitem->info == element )
		{
			return listitem;
		}
	}

	return NULL;
}

dlist* find_list_element_by_compare( dlist *destlist, list_element element, list_ele_compare compare_func )
{
	dlist *listitem;
	ASSERT( NULL != destlist );

	listitem = destlist;
	listitem = listitem->next;

	for( ; NULL != listitem; listitem = listitem->next )
	{
		if( 0 == compare_func( listitem->info, element ) )
		{
			return listitem->info;
		}
	}

	return NULL;
}

void* check_through_list_element( dlist *destlist, list_ele_compare check_func )
{
	dlist *listitem;
	void *eleret; 

	ASSERT( NULL != destlist );

	listitem = destlist;
	if( listitem == NULL )
	{
		ASSERT( FALSE ); 
		return NULL; 
	}

	listitem = listitem->next;
	if( listitem == NULL )
	{
		ASSERT( FALSE ); 
		return NULL; 
	}

	eleret = listitem->info; 
	for( ; NULL != listitem->next; listitem = listitem->next )
	{
		eleret = check_func( eleret, listitem->next->info );  
	}

	ASSERT( eleret != NULL ); 
	return eleret;
}

dlist* get_list_item_ptr( list_element *list_item_member_addr )
{
	dlist *listitem;
	ASSERT( NULL != list_item_member_addr );

	listitem = ( dlist* )list_item_member_addr;

	return listitem;
}

int del_list_element2( dlist *destlist, int index, list_element *element )
{
	int count;
	dlist *listitem;
	ASSERT( NULL != destlist );

	listitem = destlist;
	count = 0;
	for( ; NULL != listitem; listitem = listitem->next )
	{
		if( count == index )
		{
			del_list_item( listitem, element );
			return 0;
		}

		count ++;
	}

	return -1;
}

int del_list_element( dlist *destlist, list_element element )
{
	dlist *listitem;
	list_element tmp_elem;
	listitem = find_list_element( destlist, element );
	if( NULL != listitem )
	{
		del_list_item( listitem, &tmp_elem );
	}
	return 0;
}

int init_list_element( dlist **dest_list )
{
	dlist *new_list;
	new_list = ( dlist* )malloc( sizeof( dlist ) );
	if( NULL == new_list )
	{
		return -E_OUTOFMEMORY;
	}
	memset( new_list, 0, sizeof( dlist ) );
	*dest_list = new_list;
	return 0;
}

int destroy_list( dlist *list_header, destroy_list_element_func des_func )
{
	int ret;
	int failed_count;

	dlist *item;
	dlist *next_item;

	ASSERT( NULL != list_header );
	ASSERT( NULL == list_header->prev && NULL == list_header->info );

	item = list_header;
	failed_count = 0;

	for( ; ; )
	{
		if( NULL == item )
		{
			break;
		}

		next_item = get_next_list( item );
		if( NULL != des_func )
		{
			ret = des_func( item->info );
			if( 0 > ret )
			{
				failed_count ++;
			}
		}

		del_list_item( item, NULL );

		item = next_item;
	}
	
	return -failed_count;
}

dlist* get_next_list( dlist *item )
{
	ASSERT( item );
	ASSERT( item->prev != NULL ? item->prev->next == item : TRUE );
	ASSERT( item->next != NULL ? item->next->prev == item : TRUE );

	return item->next;
}

dlist* get_prev_list( dlist *item )
{
	ASSERT( item );
	ASSERT( item->prev != NULL ? item->prev->next == item : TRUE );
	ASSERT( item->next != NULL ? item->next->prev == item : TRUE );

	return item->prev;
}

dlist* get_next_list_element( dlist *item, list_element *element )
{
	dlist *next_item;

	ASSERT( NULL != item );

	next_item = get_next_list( item );
	
	ASSERT( NULL != item );
	ASSERT( NULL != element );

	if( NULL == next_item )
	{
		*element = NULL;
		return NULL;
	}

	*element = next_item->info;
	return next_item;
}

dlist* get_prev_list_elememt( dlist *item, list_element *element )
{
	dlist *prev_item;
	prev_item = get_prev_list( item );
	
	ASSERT( NULL != item );
	ASSERT( NULL != element );

	if( NULL == prev_item )
	{
		*element = NULL;
		return NULL;
	}

	*element = prev_item->info;
	return prev_item;
}