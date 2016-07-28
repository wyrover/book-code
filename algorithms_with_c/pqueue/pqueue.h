#ifndef PQUEUE_H
#define pQUEUE_H

#include"heap.h"

typedef Heap PQueue;

#define pqueue_init heap_init
#define pqueue_destroy heap_destroy
#define pqueue_insert heap_insert
#define pqueue_extract heap_extract
#define pqueue_peek(pqueue) ((pqueue)->tree == NULL?NULL:(pqueue)->tree[0])
#define pqueue_size heap_size

#endif
