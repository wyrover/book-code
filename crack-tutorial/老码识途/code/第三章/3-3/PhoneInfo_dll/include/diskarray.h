#ifndef DISKARRYA_H_XXXX
#define DISKARRYA_H_XXXX

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

extern "C" void __declspec(dllimport) closeSimpleDiskArray(SimpleDiskArray * array);

extern "C" void   __declspec(dllimport) initSimpleDiskArray(SimpleDiskArray * array, int elementSize, char * fileName);

extern "C" void __declspec(dllimport) finalizeSimpleDiskArray(SimpleDiskArray * array);

extern "C" BOOLEAN __declspec(dllimport) insertSimpleDiskArray(SimpleDiskArray * array, void * pValue);

extern "C" BOOLEAN __declspec(dllimport) deleteSimpleDiskArray(SimpleDiskArray * array, int index);

extern "C" BOOLEAN __declspec(dllimport) updateSimpleDiskArray(SimpleDiskArray * array, int index, void * pValue);

extern "C" BOOLEAN __declspec(dllimport) getSimpleDiskArray(SimpleDiskArray * array, int index, void * pValue);

extern "C" int __declspec(dllimport) getSimpleDiskArrayCount(SimpleDiskArray * array);


#endif

