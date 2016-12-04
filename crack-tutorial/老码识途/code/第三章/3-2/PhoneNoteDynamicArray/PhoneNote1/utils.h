#ifndef UTILS_H_XXXXX
#define UTILS_H_XXXXX

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

int readLn(FILE * fp, char * ln);
char * getArg(char * start);

struct DynamicArray
{
	char * ptrArray;
	int count;
	int elementSize;//you should use sizeof to get the size, because of align.
};

typedef DynamicArray * PDynamicArray;



void setArrayLength(PDynamicArray ptrArray, int elementCount);

void setArrayItem(int index, DynamicArray anArray, void *ptrValue);

void* getArrayItem(int index, DynamicArray anArray);

void initDynamicArray(PDynamicArray ptrArray, int elementSize);

#endif