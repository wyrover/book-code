#include "../include/types.h"
#include <stdio.h>

struct SimpleDiskArray
{
	FILE * fp;
	int elementSize;
	int count;
	char * fileName;
	void * tmpBlock;
};

extern "C" BOOLEAN __declspec(dllimport) openSimpleDiskArray(SimpleDiskArray * array);

extern "C" void __declspec(dllexport) closeSimpleDiskArray(SimpleDiskArray * array);

extern "C" void   initSimpleDiskArray(SimpleDiskArray * array, int elementSize, char * fileName);

extern "C" void __declspec(dllexport) finalizeSimpleDiskArray(SimpleDiskArray * array);

extern "C" BOOLEAN __declspec(dllexport) insertSimpleDiskArray(SimpleDiskArray * array, void * pValue);

extern "C" BOOLEAN __declspec(dllexport) deleteSimpleDiskArray(SimpleDiskArray * array, int index);

extern "C" BOOLEAN __declspec(dllexport) updateSimpleDiskArray(SimpleDiskArray * array, int index, void * pValue);

extern "C" BOOLEAN __declspec(dllexport) getSimpleDiskArray(SimpleDiskArray * array, int index, void * pValue);

extern "C" int __declspec(dllexport) getSimpleDiskArrayCount(SimpleDiskArray * array);
