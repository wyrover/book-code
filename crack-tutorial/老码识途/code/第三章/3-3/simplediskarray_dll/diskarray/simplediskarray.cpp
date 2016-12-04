#include "stdafx.h"
#include "simplediskarray.h"
#include <string.h>
#include <malloc.h>
#include <io.h>
#include <memory.h>

int getSimpleDiskArrayCount(SimpleDiskArray * array)
{
	return array->count;
}

BOOLEAN openSimpleDiskArray(SimpleDiskArray * array)
{
	array->fp = fopen(array->fileName, "rb+");
	if (array->fp == NULL)
	{
		array->fp = fopen(array->fileName, "wb+");
		if (array->fp == NULL)
		{
			return FALSE;
		}
		array->count = 0;
		fwrite(&(array->count), sizeof(array->count), 1, array->fp);		
	}
	else
	{
		if (fread(&(array->count), sizeof(array->count), 1, array->fp) != 1)
		{
			return FALSE;
		}
	}
	return TRUE;
}

void closeSimpleDiskArray(SimpleDiskArray * array)
{
	fclose(array->fp);
}

void initSimpleDiskArray(SimpleDiskArray * array, int elementSize, char * fileName)
{
	memset(array, 0, sizeof(SimpleDiskArray));
	array->elementSize = elementSize;
	array->fileName = (char *)malloc(strlen(fileName) + 1);
	strcpy(array->fileName, fileName);	
}

void finalizeSimpleDiskArray(SimpleDiskArray * array)
{
	if (array->fileName != NULL)
	{
		free(array->fileName);
	}
	if (array->tmpBlock != NULL)
	{
		free(array->tmpBlock);
	}
}
BOOLEAN insertSimpleDiskArray(SimpleDiskArray * array, void * pValue)
{
	fseek(array->fp, 0, SEEK_END);
	if (fwrite(pValue, array->elementSize, 1, array->fp) != 1)
		return FALSE;
	array->count++;
	fseek(array->fp, 0, SEEK_SET);
	fwrite(&(array->count), sizeof(array->count), 1, array->fp);
	return TRUE;
}

void seekSimpleDiskArray(SimpleDiskArray * array, int index)
{
	fseek(array->fp, index * array->elementSize + sizeof(array->count), SEEK_SET);
}

void changeSimpleDiskArrayCount(SimpleDiskArray * array, int count)
{
	fseek(array->fp, 0, SEEK_SET);
	fwrite(&count, sizeof(array->count), 1, array->fp);
	array->count = count;
	// 也可以不删除，等下次open时，chsize
	fflush(array->fp);//不刷新，那么可能chsize变化了，但fclose时再次写入缓存中的。
	chsize(array->fp->_file, array->count * array->elementSize + sizeof(array->count));
}

#define CHECK_BOUND(index, array)	{if (index + 1 > array->count) return FALSE;}


BOOLEAN deleteSimpleDiskArray(SimpleDiskArray * array, int index)
{
	CHECK_BOUND(index, array);
	if (array->tmpBlock == NULL)
		array->tmpBlock = malloc(array->elementSize);
	void * pTmp = array->tmpBlock;
	for (int i = index + 1; i < array->count; i++)
	{
		if (getSimpleDiskArray(array, i, pTmp) == FALSE)
			return FALSE;
		if (updateSimpleDiskArray(array, i - 1, pTmp) == FALSE)
			return FALSE;
	}
	array->count--;		
	changeSimpleDiskArrayCount(array, array->count);
	return TRUE;
}

BOOLEAN updateSimpleDiskArray(SimpleDiskArray * array, int index, void * pValue)
{
	CHECK_BOUND(index, array);
	seekSimpleDiskArray(array, index);
	if (fwrite(pValue, array->elementSize, 1, array->fp) != 1)
		return FALSE;
	return TRUE;
}

BOOLEAN getSimpleDiskArray(SimpleDiskArray * array, int index, void * pValue)
{
	CHECK_BOUND(index, array);
	seekSimpleDiskArray(array, index);
	if (fread(pValue, array->elementSize, 1, array->fp) != 1)
		return FALSE;
	return TRUE;
}
