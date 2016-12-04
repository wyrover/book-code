#include "utils.h"

int readLn(FILE * fp, char * ln)
{
	int c, ret;
	ret = 0;

	while (1)
	{
		c = fgetc(fp);
		if (c == EOF)
		{
			*ln = 0;
			return ret;
		}
		*ln = (char)c;
		ln++;
		ret++;
		if (c == '\n')
		{
			if (*(ln - 2) == '\r')
			{
				*(ln - 2) = 0;
				ret--;
			}
			else
				*(ln - 1) = 0;
			return ret;
		}
	}
	return ret;
}

char * getArg(char * start)
{
	int i = 0;
	if (start[0] == NULL) 
		return NULL;

	//strip space before
	while (start[i] == ' ')
	{
		i++;		
	}
	if (start[i] == NULL)
	{
		return NULL;
	}
	return &start[i];
}

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