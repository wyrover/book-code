#include <stdio.h>
#include <stdlib.h>
#include "pqueue.h"

/*print_pqueue*/
static void print_pqueue(PQueue *pqueue)
{
    int                i;
    /*  Display the priority queue in level order.*/
    fprintf(stdout, "Priority queue size is %d\n", pqueue_size(pqueue));

    for (i = 0; i < pqueue_size(pqueue); i++)
        fprintf(stdout, "Node=%03d\n", *(int *)pqueue->tree[i]);

    return;
}

/*compare_int*/
static int compare_int(const void *int1, const void *int2)
{
    /*  Compare two integers.*/
    if (*(const int *)int1 > *(const int *)int2)
        return 1;
    else if (*(const int *)int1 < * (const int *)int2)
        return -1;
    else
        return 0;
}

/*main*/
int main(int argc, char **argv)
{
    PQueue             pqueue;
    void               *data;
    int                intval[30],
                       i;
    /*Initialize the priority queue.*/
    pqueue_init(&pqueue, compare_int, NULL);
    /*Perform some priority queue operations.*/
    i = 0;
    intval[i] = 5;
    fprintf(stdout, "Inserting %03d\n", intval[i]);

    if (pqueue_insert(&pqueue, &intval[i]) != 0)
        return 1;

    print_pqueue(&pqueue);
    i++;
    intval[i] = 10;
    fprintf(stdout, "Inserting %03d\n", intval[i]);

    if (pqueue_insert(&pqueue, &intval[i]) != 0)
        return 1;

    print_pqueue(&pqueue);
    i++;
    intval[i] = 20;
    fprintf(stdout, "Inserting %03d\n", intval[i]);

    if (pqueue_insert(&pqueue, &intval[i]) != 0)
        return 1;

    print_pqueue(&pqueue);
    i++;
    intval[i] = 1;
    fprintf(stdout, "Inserting %03d\n", intval[i]);

    if (pqueue_insert(&pqueue, &intval[i]) != 0)
        return 1;

    print_pqueue(&pqueue);
    i++;
    intval[i] = 25;
    fprintf(stdout, "Inserting %03d\n", intval[i]);

    if (pqueue_insert(&pqueue, &intval[i]) != 0)
        return 1;

    print_pqueue(&pqueue);
    i++;
    intval[i] = 22;
    fprintf(stdout, "Inserting %03d\n", intval[i]);

    if (pqueue_insert(&pqueue, &intval[i]) != 0)
        return 1;

    print_pqueue(&pqueue);
    i++;
    intval[i] = 12;
    fprintf(stdout, "Inserting %03d\n", intval[i]);

    if (pqueue_insert(&pqueue, &intval[i]) != 0)
        return 1;

    print_pqueue(&pqueue);
    i++;

    while (pqueue_size(&pqueue) > 0) {
        fprintf(stdout, "Peeking at the highest priority element..Value=%03d\n",
                *(int *)pqueue_peek(&pqueue));

        if (pqueue_extract(&pqueue, (void **)&data) != 0)
            return 1;

        fprintf(stdout, "Extracting %03d\n", *(int *)data);
        print_pqueue(&pqueue);
    }

    /*Destroy the priority queue.*/
    fprintf(stdout, "Destroying the pqueue\n");
    pqueue_destroy(&pqueue);
    return 0;
}
