#include"clist.h"
#include"page.h"

int replace_page(CListElmt **current)
{
    while (((Page *)(*current)->data)->reference != 0) {
        ((Page *)(*current)->data)->reference = 0;
        *current = clist_next(*current);
    }

    return ((Page *)(*current)->data)->number;
}
