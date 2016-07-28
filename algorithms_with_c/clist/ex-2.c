#include <stdio.h>
#include "page.h"
int main(int argc, char **argv)
{
    CList              list;
    CListElmt          *current;
    Page               *p;
    int                i;
    clist_init(&list, free);
    current = NULL;

    for (i = 0; i < 10; i++) {
        if ((p = (Page *)malloc(sizeof(Page))) == NULL)
            return 1;

        if (i < 5)
            p->reference = 1;
        else
            p->reference = 0;

        p->number = i;

        if (clist_ins_next(&list, current, p) != 0)
            return 1;

        if (current == NULL)
            current = clist_next(clist_head(&list));
        else
            current = clist_next(current);
    }

    current = clist_head(&list);

    for (i = 0; i < 10; i++) {
        p = clist_data(current);
        fprintf(stdout, "p[%d].number=%d, p[%d].reference=%d\n", i, p->number, i,
                p->reference);
        current = clist_next(current);
    }

    current = clist_head(&list);
    i = replace_page(&current);
    fprintf(stdout, "Selected %d\n", i);
    current = clist_head(&list);

    for (i = 0; i < 10; i++) {
        p = clist_data(current);
        fprintf(stdout, "p[%d].number=%d, p[%d].reference=%d\n", i, p->number, i,
                p->reference);
        current = clist_next(current);
    }

    fprintf(stdout, "Destroying the list\n");
    clist_destroy(&list);
    return 0;
}
