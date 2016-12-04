#ifndef SIMPLEDISKARRAY_XXXX 

#define SIMPLEDISKARRAY_XXXX 

#include <stdio.h>
#include "../include/types.h"

struct SimpleDiskArray
{
	FILE * fp;
	int elementSize;
	int count;
	char * fileName;
	void * tmpBlock;
};

BOOLEAN openSimpleDiskArray(SimpleDiskArray * array);

void closeSimpleDiskArray(SimpleDiskArray * array);

void initSimpleDiskArray(SimpleDiskArray * array, int elementSize, char * fileName);

void finalizeSimpleDiskArray(SimpleDiskArray * array);

BOOLEAN insertSimpleDiskArray(SimpleDiskArray * array, void * pValue);

BOOLEAN deleteSimpleDiskArray(SimpleDiskArray * array, int index);

BOOLEAN updateSimpleDiskArray(SimpleDiskArray * array, int index, void * pValue);

BOOLEAN getSimpleDiskArray(SimpleDiskArray * array, int index, void * pValue);

int getSimpleDiskArrayCount(SimpleDiskArray * array);

#endif