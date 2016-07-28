#ifndef PAGE_H
#define PAGE_H
#include"clist.h"

typedef struct Page_ {
    int number;
    int reference;
} Page;

int replace_page(CListElmt **current);
#endif
