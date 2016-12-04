// dynamicArray.cpp : 定义控制台应用程序的入口点。
//
#include <stdio.h>
#include <memory.h>
#include <malloc.h>

struct DynamicArray
{
	char * ptrArray;
	int count;
	int elementSize;//you should use sizeof to get the size, because of align.
};

typedef DynamicArray * PDynamicArray;



void* getArrayItem(int index, DynamicArray anArray)
{
	if (index + 1 > anArray.count)
		return NULL;
	return (void *)(anArray.ptrArray + (index) * anArray.elementSize);
};

void setArrayItem(int index, DynamicArray anArray, void *ptrValue)
{
	if (index + 1 > anArray.count)
		return;//not good, need return the error msg
	memcpy(anArray.ptrArray + (index) * anArray.elementSize, ptrValue, anArray.elementSize);
};

void setArrayLength(PDynamicArray ptrArray, int elementCount)
{
	char *p;
	int moveSize, newSize;
	if (ptrArray->count == elementCount)
		return;
	if (elementCount <= 0)
	{
		free(ptrArray->ptrArray);
		ptrArray->count = 0;
		return;
	};
	newSize = elementCount * ptrArray->elementSize;
	p = (char *)malloc(newSize);
	if (ptrArray->count > 0)//has element , need copy.
	{
		if (ptrArray->count > elementCount)//pack
			moveSize = newSize;
		else //extension
			moveSize = ptrArray->count * ptrArray->elementSize;
		memcpy(p, ptrArray->ptrArray, moveSize);
		free(ptrArray->ptrArray);
	};
	ptrArray->ptrArray = p;
	ptrArray->count = elementCount;
};

void initDynamicArray(PDynamicArray ptrArray, int elementSize)
{
	ptrArray->ptrArray = NULL;
	ptrArray->elementSize = elementSize;
	ptrArray->count = 0;
}

void main()
{
	DynamicArray array;
	initDynamicArray(&array, sizeof(int));
	int i = 3;
	setArrayLength(&array, 1);	
	setArrayItem(0, array, &i);
	setArrayLength(&array, 2);	
	i = 4;
	setArrayItem(1, array, &i);
	for (i = 0; i < array.count; i++)
	{
		printf("array[%d]=%d\n", i, *(int *)getArrayItem(i, array));
	}	
}