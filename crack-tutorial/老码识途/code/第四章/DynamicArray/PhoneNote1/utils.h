#ifndef UTILS_H_XXXXX
#define UTILS_H_XXXXX

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

int readLn(FILE * fp, char * ln);
char * getArg(char * start);

class DynamicArray
{
public:
	DynamicArray(int elementSize);
	char * ptrArray;
	int count;
	int elementSize;//you should use sizeof to get the size, because of align.
	void setArrayLength(int elementCount);
	void setArrayItem(int index, void *ptrValue);
	void* getArrayItem(int index);
};


#endif