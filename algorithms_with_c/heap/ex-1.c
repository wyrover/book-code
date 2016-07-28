#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

/*   print_heap  */

static void print_heap(Heap *heap)
{
    int                i;
    /*  Display the heap using in level order.*/
    fprintf(stdout, "Heap size is %d\n", heap_size(heap));

    for (i = 0; i < heap_size(heap); i++)
        fprintf(stdout, "Node=%03d\n", *(int *)heap->tree[i]);

    return;
}

/*compare_int*/
static int compare_int(const void *int1, const void *int2)
{
    /*Compare two integers.*/
    if (*(const int *)int1 > *(const int *)int2)
        return 1;
    else if (*(const int *)int1 < * (const int *)int2)
        return -1;
    else
        return 0;
}

int main(int argc, char **argv)
{
    Heap               heap;
    void               *data;
    int                intval[30], i;
    /*Initialize the heap.*/
    heap_init(&heap, compare_int, NULL);
    /*  Perform some heap operations.*/
    i = 0;
    intval[i] = 5;
    fprintf(stdout, "Inserting %03d\n", intval[i]);

    if (heap_insert(&heap, &intval[i]) != 0)
        return 1;

    print_heap(&heap);
    i++;
    intval[i] = 10;
    fprintf(stdout, "Inserting %03d\n", intval[i]);

    if (heap_insert(&heap, &intval[i]) != 0)
        return 1;

    print_heap(&heap);
    i++;
    intval[i] = 20;
    fprintf(stdout, "Inserting %03d\n", intval[i]);

    if (heap_insert(&heap, &intval[i]) != 0)
        return 1;

    print_heap(&heap);
    i++;
    intval[i] = 1;
    fprintf(stdout, "Inserting %03d\n", intval[i]);

    if (heap_insert(&heap, &intval[i]) != 0)
        return 1;

    print_heap(&heap);
    i++;
    intval[i] = 25;
    fprintf(stdout, "Inserting %03d\n", intval[i]);

    if (heap_insert(&heap, &intval[i]) != 0)
        return 1;

    print_heap(&heap);
    i++;
    intval[i] = 22;
    fprintf(stdout, "Inserting %03d\n", intval[i]);

    if (heap_insert(&heap, &intval[i]) != 0)
        return 1;

    print_heap(&heap);
    i++;
    intval[i] = 9;
    fprintf(stdout, "Inserting %03d\n", intval[i]);

    if (heap_insert(&heap, &intval[i]) != 0)
        return 1;

    print_heap(&heap);
    i++;

    while (heap_size(&heap) > 0) {
        if (heap_extract(&heap, (void **)&data) != 0)
            return 1;

        fprintf(stdout, "Extracting %03d\n", *(int *)data);
        print_heap(&heap);
    }

    /*Destroy the heap*/
    fprintf(stdout, "Destroying the heap\n");
    heap_destroy(&heap);
    return 0;
}
