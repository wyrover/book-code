#include<stdio.h>
#include<stdlib.h>
#include<memory.h>
typedef struct Stu_ {
    int *age;
    char *name;
} Stu;
int main()
{
    Stu *p = malloc(sizeof(Stu));
    int *b = malloc(sizeof(int));
    *b = 10;
    p->age = b;
    p->name = "deardeng";
    free(p->age);
    free(p);
}
