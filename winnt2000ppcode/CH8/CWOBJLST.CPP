// File CWOBJLST.CPP Object List class
//      Manages an array of pointers to objects.

#include <alloc.h>
#include "cwobj.h"

object_list::object_list (int max_items) {
	 if (!(the_list = (p_void *)malloc((size_t)
         (max_items + 1) * sizeof(p_void)))) {
		 MessageBox (NULL,"Memory allocation failed "
			  "in constructor.", "Object List",MB_OK |
	        MB_ICONSTOP);  return; }
	 // object_list uses indexes 1..max_items
	 the_list[0] = NULL;  // don't use first position
	 max_count = max_items;
	 item_count = 0; }

int object_list::add_item (void *item_addr) {
    if (item_count >= max_count) return 0;
    the_list[item_count + 1] = item_addr;
    item_count++;
    return item_count; }

int object_list::insert_item (int index,
       void *item_addr) {
    int i;
    if (item_count >= max_count) return 0;
    if (index > max_count) return 0;
    for (i=item_count;i>=index;i--)
	the_list [i+1] = the_list[i];
    the_list[index] = item_addr;
    item_count++;
    return 1; }

void *object_list::at (int index) {
    if (index <= 0) return NULL;
    if (index > item_count) return NULL;
    return the_list[index]; }

int object_list::get_count () {
    return item_count; }

/* Note:  Since the object_list does not allocate the
   actual items in the list (it merely keeps track of
   them), it does not delete them upon termination,
   when delete_item is called.  It is the calling
   application's responsibility to delete the actual
   list items.  */

void object_list::delete_item (int index) {
     int i;
     if (index <= 0) return;
     if (index > item_count) return;
     for (i=index;i<=item_count-1;i++)
	 the_list[i] = the_list[i+1];
     item_count--;
     return; }

object_list::~object_list() {
     delete the_list;
     }
