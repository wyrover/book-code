#include<stdlib.h>
#include<string.h>
#include"list.h"
void list_init(List *list, void (*destroy)(void *data))
{
    list->size = 0;
    list->destroy = destroy;
    list->head = NULL;
    list->tail = NULL;
    return;
}
void list_destroy(List *list)
{
    void *data;

    while (list_size(list) > 0) {
        if (list_rem_next(list, NULL, (void **)&data) == 0 && list->destroy != NULL) {
            list->destroy(data);
        }

        //while(list_size(list)>0&&list->destroy){
        //    list_rem_next(list,NULL,(void **)&data);
        // if(list_rem_next(list,NULL,(void **)&data)==0&&list->destroy!=NULL){
        //     list->destroy(data);
        // }
    }

    memset(list, 0, sizeof(List));
}

int list_ins_next(List *list, ListElmt *element, const void *data)
{
    ListElmt *new_element;

    if ((new_element = (ListElmt *)malloc(sizeof(ListElmt))) == NULL)
        return -1;

    new_element->data = (void *)data;

    if (element == NULL) {
        if (list_size(list) == 0)
            list->tail = new_element;

        new_element->next = list->head;
        list->head = new_element;
    } else {
        if (element->next == NULL)
            list->tail = new_element;

        new_element->next = element->next;
        element->next = new_element;
    }

    list->size++;
    return 0;
}

int list_rem_next(List *list, ListElmt *element, void **dat)
{
    ListElmt *old_element;

    if (list_size(list) == 0)
        return -1;

    if (element == NULL) {
        *dat = list->head->data;
        old_element = list->head;
        list->head = list->head->next;

        if (list_size(list) == 1)
            list->tail = NULL;
    } else {
        if (element->next == NULL)
            return -1;

        *dat = element->next->data;
        old_element = element->next;
        element->next = element->next->next;

        if (element->next == NULL)
            list->tail = element;
    }

    //free(old_element->data);如果加上这句，stack pop的时候将出错，double free，在ex-1.c中程序负责释放data所指向的内存。但是出错信息却在push 后的malloc出错，molloc似乎没有新分配内存，而是用原来的地址，莫名bug？
    // free(*dat);
    free(old_element);
    list->size--;
    return 0;
}
