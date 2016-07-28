#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "stack.h"
static void print_stack(const Stack *stack)
{
    ListElmt           *element;
    int                *data,
                       size,
                       i;
    fprintf(stdout, "Stack size is %d\n", size = stack_size(stack));
    i = 0;
    element = list_head(stack);

    while (i < size) {
        data = list_data(element);
        fprintf(stdout, "stack[%03d]=%03d\n", i, *data);
        element = list_next(element);
        i++;
    }

    return;
}
int main(int argc, char **argv)
{
    Stack              stack;
    int                *data,
                       i;
    stack_init(&stack, free);
    fprintf(stdout, "Pushing 10 elements\n");

    for (i = 0; i < 10; i++) {
        if ((data = (int *)malloc(sizeof(int))) == NULL)
            return 1;

        *data = i + 1;

        if (stack_push(&stack, data) != 0)
            return 1;
    }

    print_stack(&stack);
    fprintf(stdout, "Popping 5 elements\n");

    for (i = 0; i < 5; i++) {
        // stack_pop(&stack, (void **)&data);
        if (stack_pop(&stack, (void **)&data) == 0)
            free(data);//double free处，假如在list.c中就释放data指向的内存。
        else
            return 1;
    }

    print_stack(&stack);
    fprintf(stdout, "Pushing 100 and 200\n");

    if ((data = (int *)malloc(sizeof(int))) == NULL)
        return 1;

    *data = 100;

    if (stack_push(&stack, data) != 0)
        return 1;

    if ((data = (int *)malloc(sizeof(int))) == NULL)//stack pop的时候将出错，double free，在ex-1.c中程序负责释放data所指向的内存。但是出错信息却在push 后的malloc出错，molloc似乎没有新分配内存，而是用原来的地址，莫名bug？
        return 1;

    *data = 200;

    if (stack_push(&stack, data) != 0)
        return 1;

    print_stack(&stack);

    if ((data = stack_peek(&stack)) != NULL)
        fprintf(stdout, "Peeking at the top element...Value=%03d\n", *data);
    else
        fprintf(stdout, "Peeking at the top element...Value=NULL\n");

    print_stack(&stack);
    fprintf(stdout, "Popping all elements\n");

    while (stack_size(&stack) > 0) {
        if (stack_pop(&stack, (void **)&data) == 0)
            free(data);
    }

    if ((data = stack_peek(&stack)) != NULL)
        fprintf(stdout, "Peeking at an empty stack...Value=%03d\n", *data);
    else
        fprintf(stdout, "Peeking at an empty stack...Value=NULL\n");

    print_stack(&stack);
    fprintf(stdout, "Destroying the stack\n");
    stack_destroy(&stack);
    return 0;
}
