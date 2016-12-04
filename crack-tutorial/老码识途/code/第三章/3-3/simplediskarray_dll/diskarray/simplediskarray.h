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

extern "C" BOOLEAN __declspec(dllexport) openSimpleDiskArray(SimpleDiskArray * array);

extern "C" void __declspec(dllexport) closeSimpleDiskArray(SimpleDiskArray * array);

extern "C" void __declspec(dllexport) initSimpleDiskArray(SimpleDiskArray * array, int elementSize, char * fileName);

extern "C" void __declspec(dllexport) finalizeSimpleDiskArray(SimpleDiskArray * array);

extern "C" BOOLEAN __declspec(dllexport) insertSimpleDiskArray(SimpleDiskArray * array, void * pValue);

extern "C" BOOLEAN __declspec(dllexport) deleteSimpleDiskArray(SimpleDiskArray * array, int index);

extern "C" BOOLEAN __declspec(dllexport) updateSimpleDiskArray(SimpleDiskArray * array, int index, void * pValue);

extern "C" BOOLEAN __declspec(dllexport) getSimpleDiskArray(SimpleDiskArray * array, int index, void * pValue);

extern "C" int __declspec(dllexport) getSimpleDiskArrayCount(SimpleDiskArray * array);

#endif