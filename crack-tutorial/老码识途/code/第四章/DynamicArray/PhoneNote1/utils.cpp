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
	{
		return NULL;
	}
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

void* DynamicArray::getArrayItem(int index)
{
	if (index + 1 > count)
		return NULL;
	return (void *)(ptrArray + index * elementSize);
};

void DynamicArray::setArrayItem(int index, void *ptrValue)
{
	if (index + 1 > count)
		return;//not good, need return the error msg
	memcpy(ptrArray + index * elementSize, ptrValue, elementSize);
};

void DynamicArray::setArrayLength(int elementCount)
{
	char *p;
	int moveSize, newSize;
	if (count == elementCount)
		return;
	if (elementCount <= 0)
	{
		free(ptrArray);
		count = 0;
		return;
	};
	newSize = elementCount * elementSize;
	p = (char *)malloc(newSize);
	if (count > 0)//has element , need copy.
	{
		if (count > elementCount)//pack
			moveSize = newSize;
		else //extension
			moveSize = count * elementSize;
		memcpy(p, ptrArray, moveSize);
		free(ptrArray);
	};
	ptrArray = p;
	count = elementCount;
};

DynamicArray::DynamicArray(int elementSize)
{
	ptrArray = NULL;
	this->elementSize = elementSize;
	count = 0;
}